#include "AscentGuidance.h"
#include "../Atlantis.h"
#include "../ParameterValues.h"
#include <UltraMath.h>
#include "SSME_SOP.h"
#include "SSME_Operations.h"

namespace dps
{
	
AscentGuidance::AscentGuidance(SimpleGPCSystem* _gpc)
: SimpleGPCSoftware(_gpc, "AscentGuidance"),
  hEarth(NULL),
  lastSBTCCommand(0.0),
  stage1GuidanceVelTable(DEFAULT_STAGE1_GUIDANCE_TABLE_VEL, DEFAULT_STAGE1_GUIDANCE_TABLE_VEL+STAGE1_GUIDANCE_TABLE_SIZE),
  stage1GuidancePitchTable(DEFAULT_STAGE1_GUIDANCE_TABLE_PITCH, DEFAULT_STAGE1_GUIDANCE_TABLE_PITCH+STAGE1_GUIDANCE_TABLE_SIZE),
  bMECO(false),
  ETSepTranslationInProg(false), ETSepMinusZDV(ET_SEP_RATE),
  tLastMajorCycle(-1.0)
{
	// set PID controller gains
	for(int i=0;i<2;i++) {
		SRBGimbal[i][PITCH].SetGains(-0.5, -0.05, 0.0);
		SRBGimbal[i][YAW].SetGains(-0.5, -0.05, 0.0);
	}
	SRBGimbal[0][ROLL].SetGains(0.75, 0.05, 0.0);
	SRBGimbal[1][ROLL].SetGains(-0.75, -0.05, 0.0);

	throttlecmd = 100;

	glimiting = false;

	// generic values, updated in InitializeAutopilot()
	THROT[0] = THROT1;
	THROT[1] = THROT2;
	THROT[2] = THROT3;
	THROT[3] = THROT4;
	QPOLY[0] = QPOLY1;
	QPOLY[1] = QPOLY2;
	QPOLY[2] = QPOLY3;
	QPOLY[3] = QPOLY4;
	QPOLY[4] = QPOLY5;
	J = 0;

	AGT_done = false;
}

AscentGuidance::~AscentGuidance()
{
}

void AscentGuidance::Realize()
{
	DiscreteBundle* pBundle = BundleManager()->CreateBundle("LOMS", 5);
	OMSCommand[LEFT].Connect(pBundle, 2);
	pBundle = BundleManager()->CreateBundle("ROMS", 5);
	OMSCommand[RIGHT].Connect(pBundle, 2);

	pBundle = STS()->BundleManager()->CreateBundle("THRUSTER_CMD", 16);
	ZTransCommand.Connect(pBundle, 5);

	pBundle=BundleManager()->CreateBundle("SPDBKTHROT_CONTROLS", 16);
	SpdbkThrotAutoIn.Connect(pBundle, 0);
	SpdbkThrotAutoOut.Connect(pBundle, 0);
	SpdbkThrotPLT.Connect(pBundle, 2);
	SpdbkThrotAutoOut.SetLine(); // default to auto throttling

	pBundle=STS()->BundleManager()->CreateBundle("HC_INPUT", 16);
	SpdbkThrotPort.Connect(pBundle, 6);	

	pSSME_SOP = static_cast<SSME_SOP*> (FindSoftware( "SSME_SOP" ));
	pSSME_Operations = static_cast<SSME_Operations*> (FindSoftware( "SSME_Operations" ));
}

void AscentGuidance::OnPreStep(double SimT, double DeltaT, double MJD)
{
	switch(GetMajorMode()) {
		case 102:
			STS()->CalcSSMEThrustAngles(ThrAngleP, ThrAngleY);
			Throttle(DeltaT);
			FirstStageRateCommand();
			GimbalSRBs(DeltaT);
			GimbalSSMEs(DeltaT);
			break;
		case 103:
			if (pSSME_Operations->GetMECOCommandFlag() == false){//if(!bMECO) {
				STS()->CalcSSMEThrustAngles(ThrAngleP, ThrAngleY);
				Navigate();
				if(STS()->GetMET() >= (tLastMajorCycle + ASCENT_MAJOR_CYCLE)) {
					MajorCycle();
					tLastMajorCycle = STS()->GetMET();
				}
				SecondStageRateCommand();
				GimbalSSMEs(DeltaT);
				Throttle(DeltaT);
			}
			else { //post MECO
				if (bMECO == false)// for low level c/o
				{
					bMECO = true;
					tMECO = STS()->GetMET();
				}
				else if(STS()->HasTank() && !ETSepTranslationInProg && tMECO+ET_SEP_TIME<=STS()->GetMET() && pSSME_Operations->GetMECOConfirmedFlag())
				{
					STS()->SeparateTank();
					ETSepTranslationInProg = true;
				}
				else if(!STS()->HasTank()) {
					if(ETSepMinusZDV <= 0.001) { //Z thrusting complete
						ZTransCommand.ResetLine();
						ETSepTranslationInProg = false;
						SetMajorMode(104);
					}
					else { // -Z thrusting in progress
						ZTransCommand.SetLine(-1.0f);
						// calculate DV so far
						VECTOR3 ThrustVector;
						STS()->GetThrustVector(ThrustVector);
						ETSepMinusZDV -= (ThrustVector.y/STS()->GetMass())*DeltaT;
					}
				}
			}
			break;
	}
}

bool AscentGuidance::OnMajorModeChange(unsigned int newMajorMode)
{
	if(newMajorMode == 102 || newMajorMode == 103) {
		if(newMajorMode == 102) InitializeAutopilot();
		else if(newMajorMode == 103) tSRBSep = STS()->GetMET();
		//STS()->CalcSSMEThrustAngles(ThrAngleP, ThrAngleY);
		return true;
	}
	return false;
}

bool AscentGuidance::OnParseLine(const char* keyword, const char* value)
{
	return false;
}

void AscentGuidance::OnSaveState(FILEHANDLE scn) const
{
}

void AscentGuidance::InitializeAutopilot()
{
	mission::Mission* pMission = STS()->GetMissionData();
	TgtInc=pMission->GetMECOInc()*DEG;
	TgtFPA=pMission->GetMECOFPA()*DEG;
	TgtAlt=pMission->GetMECOAlt();
	TgtSpd=pMission->GetMECOVel();
	MaxThrust = pMission->GetMaxSSMEThrust();
	THROT[0] = MaxThrust;
	THROT[1] = MaxThrust;
	THROT[3] = MaxThrust;
	PerformRTHU=pMission->PerformRTHU();
	QPOLY[2] = pMission->GetTHdownVelocity()/MPS2FPS;
	QPOLY[3] = pMission->GetTHupVelocity()/MPS2FPS;
	if(pMission->UseOMSAssist()) {
		OMSAssistStart = pMission->GetOMSAssistStart();
		OMSAssistEnd = pMission->GetOMSAssistEnd();
	}
	else OMSAssistStart = OMSAssistEnd = 0.0;

	hEarth = STS()->GetSurfaceRef();
	//calculate heading
	double latitude, longitude;
	STS()->GetEquPos(longitude, latitude, EarthRadius);
	if(abs(cos(TgtInc*RAD))>abs(cos(latitude))) {
		radTargetHeading=PI/2;
		TgtInc = latitude*DEG;
	}
	else {
		// there are two possible launch headings: going north and going south
		// for TgtInc < 65 deg (KSC launches) we want northerly heading; max. inclination of KSC launch was 62 degrees (with dog-leg)
		// for TgtInc > 65 deg (VAFB) we want southerly heading
		double InHeading = asin(cos(TgtInc*RAD)/cos(latitude));
		double xVel, yVel;
		xVel = TgtSpd*cos(TgtFPA*RAD)*sin(InHeading)-464.581*cos(latitude);
		yVel = TgtSpd*cos(TgtFPA*RAD)*cos(InHeading);
		radTargetHeading=atan2(xVel, yVel); // northerly heading
		if(TgtInc > 65.0) radTargetHeading = PI - radTargetHeading; // if heading is negative, this is retrograde inclination; use southerly heading
	}

	mu=GGRAV*oapiGetMass(hEarth);
	SidDay = oapiGetPlanetPeriod(hEarth);
	//calculate apogee/perigee
	EarthRadius = oapiGetSize(hEarth);
	TgtRad = TgtAlt+EarthRadius;
	double C = (2*mu)/(TgtRad*TgtSpd*TgtSpd);
	double det = C*C + (4*(1-C)*pow(cos(TgtFPA*RAD), 2));
	double root1 = (-C-sqrt(det))/(2*(1-C));
	double root2 = (-C+sqrt(det))/(2*(1-C));
	if(root1>root2) {
		TAp=root1*TgtRad;
		TPe=root2*TgtRad;
	}
	else {
		TAp=root2*TgtRad;
		TPe=root1*TgtRad;
	}
	//calclate Ecc
	TEcc= (2*TAp)/(TPe+TAp) - 1.0;
	//TPe-=Radius;
	//TAp-=Radius;
	//calculate TrA
	double h = TgtRad*TgtSpd*sin((90+TgtFPA)*RAD);
	TTrA=asin((h*TgtSpd*sin(TgtFPA*RAD))/(mu*TEcc)); //angle in Rad
	//Calculate orbit parameters for this ta
	//aOrbit=TPe/(1-TEcc);
	//p=aOrbit*(1-TEcc*TEcc);

	//PEG
	STS()->GetRelativePos(hEarth,rh0);
	rh0=rh0*(1/length(rh0));
	A=0.0;
	//B=0.0;
	C=0.0;
	//D=0.0;
}

double AscentGuidance::GetCurrentHeading() const
{
	double Heading;
	if(STS()->GetPitch()*DEG>=88.0) 
	{
		VECTOR3 wingBody=_V(1,0,0);
		VECTOR3 wingHorizon;
		STS()->HorizonRot(wingBody,wingHorizon);
		Heading=atan2(wingHorizon.x,wingHorizon.z)+PI/2;
		if(Heading >= 2 * PI)
			Heading -= 2 * PI;
	}
	else 
	{
		oapiGetHeading(STS()->GetHandle(), &Heading);
	}
	return Heading;
}

void AscentGuidance::GimbalSRBs(double DeltaT)
{
	VECTOR3 AngularVelocity;
	STS()->GetAngularVel(AngularVelocity);
	VECTOR3 degRateError = degReqdRates - AngularVelocity*DEG;

	for(int i=0;i<2;i++) {
		double pitchGimbal = SRBGimbal[i][PITCH].Step(degRateError.data[PITCH], DeltaT);
		double yawGimbal = SRBGimbal[i][YAW].Step(degRateError.data[YAW], DeltaT);
		double rollGimbal = SRBGimbal[i][ROLL].Step(degRateError.data[ROLL], DeltaT);
		STS()->SetSRBGimbalAngles(static_cast<SIDE>(i), pitchGimbal+rollGimbal, yawGimbal);
	}
}

void AscentGuidance::GimbalSSMEs(double DeltaT)
{
	VECTOR3 AngularVelocity;
	STS()->GetAngularVel(AngularVelocity);
	VECTOR3 degRateError = degReqdRates - AngularVelocity*DEG;
	
	// see Section 5.4.3.4 of Ascent Guidance & Flight Control Workbook for more information on how roll is added to SSME gimbal angles
	// TODO: handle engine failures
	double pitchGimbal[3], yawGimbal[3];
	pitchGimbal[0] = -range(-8.0, degRateError.data[PITCH], 8.0);
	yawGimbal[0] = -range(-8.0, degRateError.data[YAW], 8.0) - range(-8.0, 0.75*degRateError.data[ROLL], 8.0);
	pitchGimbal[1] = -range(-8.0, degRateError.data[PITCH], 8.0) + range(-7.0, 2.5*degRateError.data[ROLL], 7.0);
	yawGimbal[1] = -range(-8.0, degRateError.data[YAW], 8.0);
	pitchGimbal[2] = -range(-8.0, degRateError.data[PITCH], 8.0) - range(-7.0, 2.5*degRateError.data[ROLL], 7.0);
	yawGimbal[2] = -range(-8.0, degRateError.data[YAW], 8.0);

	for(int i=0;i<3;i++) {
		STS()->SetSSMEGimbalAngles(i+1, pitchGimbal[i], yawGimbal[i]);
	}
}
void AscentGuidance::FirstStageRateCommand()
{
	if(STS()->GetAirspeed()<stage1GuidanceVelTable[1]) {
		degReqdRates.data[PITCH] = 0.0;
		degReqdRates.data[YAW] = 0.0;
		degReqdRates.data[ROLL] = 0.0;
	}
	else {
		double degTargetPitch=listerp(stage1GuidanceVelTable, stage1GuidancePitchTable, STS()->GetAirspeed());
		double radHeading = GetCurrentHeading();

		double degPitch = STS()->GetPitch()*DEG;
		double degBank = STS()->GetBank()*DEG;
		if(degPitch>=88.5) { // roll to correct heading
			degReqdRates.data[PITCH] = range(-10.0, 0.48*(degPitch-degTargetPitch), 10.0);
			degReqdRates.data[YAW]=0.0;
			if((radHeading-radTargetHeading)>RAD) degReqdRates.data[ROLL]=8.0;
			else if((radHeading-radTargetHeading)<-RAD) degReqdRates.data[ROLL]=-8.0;
			else degReqdRates.data[ROLL]=0.0;
			degReqdRates=RotateVectorZ(degReqdRates, (radTargetHeading-radHeading)*DEG);
		}
		else {
			degReqdRates.data[PITCH] = range(-2.5, 0.48*(degTargetPitch-degPitch), 2.5);

			if(degPitch>60.0) {
				degReqdRates.data[YAW] = range(-8.0, 0.25*DEG*(radHeading-radTargetHeading), 8.0);
			}
			else {
				degReqdRates.data[YAW] = 0.0;
			}
			if(degBank>0.0) degReqdRates.data[ROLL]=0.5*(degBank-180.0);
			else degReqdRates.data[ROLL]=0.5*(degBank+180.0);
			degReqdRates=RotateVectorZ(degReqdRates, -degBank);
			degReqdRates.data[ROLL] = range(-12.0, degReqdRates.data[ROLL], 12.0);
		}
	}
}

void AscentGuidance::SecondStageRateCommand()
{
	if(STS()->GetMET()<=(tSRBSep+4.0)) {
		degReqdRates.data[PITCH]=0.0;
		degReqdRates.data[YAW]=0.0;
		degReqdRates.data[ROLL]=0.0;
	}
	else {
		if(timeRemaining>PEG_STOP_TIME) 
		{
			double radHeading = GetCurrentHeading();
			double degBank = STS()->GetBank()*DEG;

			degReqdRates.data[PITCH] = CmdPDot;
			degReqdRates.data[YAW] = range(-2.5, 0.5*DEG*(radHeading-radTargetHeading), 2.5);

			if(!PerformRTHU ||  relativeVelocity<ROLL_TO_HEADS_UP_VELOCITY) 
			{
				if(degBank>0) degReqdRates.data[ROLL] = 2.5*(degBank-180.0);
				else degReqdRates.data[ROLL] = 2.5*(degBank+180.0);
				degReqdRates.data[ROLL]=range(-5.0, degReqdRates.data[ROLL], 5.0);
			}
			else {
				if(abs(degBank) > 20.0) degReqdRates.data[ROLL] = -5.0;
				else degReqdRates.data[ROLL] = range(-2.0, 0.5*degBank, 2.0);
				if(abs(degBank)>2.5) { //roll in progress
					VECTOR3 vel;
					STS()->GetHorizonAirspeedVector(vel);
					//double cyaw = -DEG*atan(vel.x/vel.z) + DEG*radHeading;
					double cyaw = DEG*radHeading - DEG*atan2(vel.x, vel.z); // error between heading of stack and velocity vector
					double tpitch=target_pitch-ThrAngleP*cos(degBank*RAD);
					degReqdRates.data[PITCH]=tpitch-STS()->GetPitch()*DEG;
					degReqdRates.data[YAW]=-ThrAngleP*sin(degBank*RAD)+cyaw;
				}
			}
			degReqdRates=RotateVectorZ(degReqdRates, -degBank);
		}
		else {
			degReqdRates.data[PITCH] = 0.0;
			degReqdRates.data[YAW] = 0.0;
			degReqdRates.data[ROLL] = 0.0;
		}
	}
}

void AscentGuidance::Throttle(double DeltaT)
{
	double SBTCCommand = (MaxThrust-67.0)*SpdbkThrotPort.GetVoltage() + 67.0;
	//sprintf_s(oapiDebugString(), 255, "SBTCCommand: %f", SBTCCommand);

	if(SpdbkThrotAutoIn) { // auto throttling
		// check for manual takeover
		if(Eq(STS()->GetSSMEThrustLevel(0), SBTCCommand, 4.0) && !Eq(SBTCCommand, lastSBTCCommand, 1e-2)) {
			SpdbkThrotAutoOut.ResetLine();
			SpdbkThrotPLT.SetLine();
		}
		else SpdbkThrotPLT.ResetLine();

		switch(GetMajorMode()) {
			case 102: // STAGE 1
				AdaptiveGuidanceThrottling();
				
				if (STS()->GetAirspeed() >= QPOLY[J])
				{
					throttlecmd = THROT[J];
					pSSME_SOP->SetThrottlePercent( throttlecmd );
					J++;
				}
				/*if(STS()->GetAirspeed()<18.288) 
					STS()->SetSSMEThrustLevel(0, 100.0);
				else if(STS()->GetAirspeed()>=ThrottleBucketStartVel && STS()->GetAirspeed()<=ThrottleBucketEndVel) {
					//if(GetThrusterGroupLevel(THGROUP_MAIN) > (72.0/109.0)) IncThrusterGroupLevel(THGROUP_MAIN, -0.005);
					//else SetSSMEThrustLevel(0, 72.0/109.0);
					for(unsigned short i=1;i<=3;i++) {
						double thrustLevel = STS()->GetSSMEThrustLevel(i);
						if(thrustLevel > 72.0) STS()->SetSSMEThrustLevel(i, thrustLevel - 10.0*DeltaT);
						else STS()->SetSSMEThrustLevel(i, 72.0);
					}
				}
				else {
					//if(GetThrusterGroupLevel(THGROUP_MAIN) < (MaxThrust/109.0)) IncThrusterGroupLevel(THGROUP_MAIN, 0.005);
					//else SetSSMEThrustLevel(0, MaxThrust/109.0);
					for(unsigned short i=1;i<=3;i++) {
						double thrustLevel = STS()->GetSSMEThrustLevel(i);
						if(thrustLevel < MaxThrust) STS()->SetSSMEThrustLevel(i, thrustLevel + 10.0*DeltaT);
						else STS()->SetSSMEThrustLevel(i, MaxThrust);
					}
				}*/
				break;
			case 103: // STAGE 3
				//OMS Assist
				if(STS()->GetMET()>OMSAssistStart && STS()->GetMET()<OMSAssistEnd && !bMECO)
				{
					OMSCommand[LEFT].SetLine();
					OMSCommand[RIGHT].SetLine();
				}
				else
				{
					OMSCommand[LEFT].ResetLine();
					OMSCommand[RIGHT].ResetLine();
				}
				// g limiting
				if (thrustAcceleration > ALIM2) glimiting = true;
				if ((glimiting == true) && (thrustAcceleration > ALIM1))
				{
					// TODO use correct MPL value below
					if (throttlecmd != 67)// if at MPL can't do more
					{
						if (pSSME_SOP->GetPercentChamberPressVal( 1 ) - throttlecmd < 0.15)// wait while throttling
						{
							throttlecmd--;// throttle back 1%
							throttlecmd = (double)round( throttlecmd );// round avoid x.5% cmds
							if (throttlecmd < 67) throttlecmd = 67;// don't go below MPL because it won't work
							pSSME_SOP->SetThrottlePercent( throttlecmd );
						}
					}
				}
				/*if(thrustAcceleration>=29.00) { //28.42
					for(int i=1;i<=3;i++) {
						if(STS()->GetSSMEThrustLevel(i) > 67.0)
							STS()->SetSSMEThrustLevel(i, STS()->GetSSMEThrustLevel(i)-1.0);
						else STS()->SetSSMEThrustLevel(i, 67.0);
					}
				}*/
				if(relativeVelocity>=TgtSpd) {
					//reached target speed
					if (pSSME_Operations->GetMECOCommandFlag() == false)
					{
						pSSME_Operations->SetMECOCommandFlag();
						bMECO = true;
						tMECO = STS()->GetMET();

						char buffer[64];
						sprintf_s( buffer, 64, "MECO @ %.2f", STS()->GetMET() );
						oapiWriteLog( buffer );
					}
					//STS()->SetSSMEThrustLevel(0, 0.0);
					//bMECO=true;
					//tMECO = STS()->GetMET();
				}
				sprintf_s( oapiDebugString(), 255, "tr%f", timeRemaining );
				break;
		}
	}
	else { // manual throttling
		pSSME_SOP->SetThrottlePercent( SBTCCommand );
		//STS()->SetSSMEThrustLevel(0, SBTCCommand);
	}

	lastSBTCCommand = SBTCCommand;
	
	//// detect MECO
	//if(Eq(STS()->GetSSMEThrustLevel(0), 0.0, 0.01)) {
	//	// should use SSME SOP, but better leave as is because MECO confirmed is still not fool proof
	//	bMECO = true;
	//	tMECO = STS()->GetMET();
	//}
}

void AscentGuidance::MajorCycle()
{
	radTargetHeading = CalculateAzimuth();
	Estimate();
	Guide();
}

double AscentGuidance::CalculateAzimuth()
{
	double true_azimuth=0;
	double latitude, longitude;
	double azimuth,equator_v/*,target_radius*/;
	double tgt_orbit_v[2],lnch_v[2],current_vel[2]; //vectors
	VECTOR3 vel, horizonvel;
	double temp;

	STS()->GetEquPos(longitude, latitude, temp);
	STS()->GetRelativeVel(hEarth, vel);
	MATRIX3 rot;
	STS()->GetRotationMatrix(rot);
	vel = tmul(rot, vel);
	STS()->HorizonRot(vel, horizonvel);
	current_vel[1] = horizonvel.data[0]; //East
	current_vel[0] = horizonvel.data[2];  // North

	azimuth = asin(range(-1, cos(TgtInc*RAD)/cos(latitude), 1));  // this equ doesn't take rotation into accout
	equator_v=EarthRadius*(2*PI/SidDay);   //equator velocity
	tgt_orbit_v[0]=TgtSpd*cos(TgtFPA*RAD)*cos(azimuth); // northern velocity
	tgt_orbit_v[1]=TgtSpd*cos(TgtFPA*RAD)*sin(azimuth); // eastern velocity
	lnch_v[0]= abs(tgt_orbit_v[0]) - abs(current_vel[0]); // taking current velocity into accout for CC (North); assume both values have same sign
	lnch_v[1]= abs(tgt_orbit_v[1]) - abs(current_vel[1]); // taking current velocity into accout for CC (East); assume both values have same sign

	//sprintf_s(oapiDebugString(), 255, "current_vel: %f %f target vel: %f %f", current_vel[0], current_vel[1], tgt_orbit_v[0], tgt_orbit_v[1]);

	//if (lnch_v[0]==0) lnch_v[0]=0.01; //div by zero protection	
	/*if(lnch_v[0]==0.0) { //div by zero protection
		if(lnch_v[1]>0) true_azimuth=PI/2;
		else true_azimuth=-PI/2;
	}*/
	//else true_azimuth = atan(lnch_v[1]/lnch_v[0]); // tan(azimuth) = eastern_vel / northern_vel
	true_azimuth = atan2(lnch_v[1], lnch_v[0]); // tan(azimuth) = eastern_vel / northern_vel
	if(current_vel[0] < 0.0) true_azimuth = PI - true_azimuth; // we are heading south, so need to use southerly heading
	if(current_vel[1] < 0.0) true_azimuth = 2*PI - true_azimuth; // retrograde inclination

	return true_azimuth;
}

void AscentGuidance::Navigate()
{
	VECTOR3 rv, vv, hv;
	STS()->GetRelativePos(STS()->GetSurfaceRef(),rv);
	STS()->GetRelativeVel(STS()->GetSurfaceRef(),vv);
	radius=length(rv);
	relativeVelocity=length(vv);
	hv=crossp(rv,vv);
	h=length(hv);
	rh=rv*(1/radius);
	hh=hv*(1/h);
	thetah=crossp(hh,rh);
	vr=dotp(vv,rh);
	vtheta=dotp(vv,thetah);
	omega=vtheta/radius;
	//w=x/(radius*radius);
	//last_target_pitch=target_pitch;
	//last_target_yaw=target_yaw;
	//last_target_roll=target_roll;
	//oapiGetFocusPitch(&pitch);

	double F=STS()->CalcNetSSMEThrust();
	double m=STS()->GetMass();
	Isp=STS()->GetSSMEISP();
	thrustAcceleration=F/m;
	tau=Isp/thrustAcceleration;
	//theta=acos(dotp(rh0,rh));
	//phi=acos(dotp(rh,rmh));
	//VECTOR3 rhxrmh=crossp(rh,rmh);
	//if(rhxrmh.y>0)phi=2*PI-phi;
}

void AscentGuidance::Estimate()
{
	double rbar=(radius+TgtRad)/2;
	/*double num=mu/(rbar*rbar)-omega*omega*radius;
	double fr=A+num/a0;
	double fdotr=B+(num/a(T)-fr)/T;
	double fh=0; //No yaw guidance yet
	double fdoth=0;
	double ftheta=1-fr*fr/2-fh*fh/2;  //Small number approximation to hypot
	double fdottheta=-(fr*fdotr+fh*fdoth);
	double fdotdottheta=-(fdotr*fdotr+fdoth*fdoth)/2;*/

	//Estimate true anomaly at cutoff
	//double d3=h*vr/(radius*radius*radius);
	//double d4=(h*(TgtSpd*sin(TgtFPA*RAD))/(TgtRad*TgtRad*TgtRad)-d3)/T;
	//double deltatheta=(h/(radius*radius))*T+(ftheta*c0(T)+fdottheta*cn(T,1)+fdotdottheta*cn(T,2))/rbar-d3*T*T-d4*T*T*T/3.0;
	//thetaT=theta+deltatheta;
	double p=(TPe/(1-TEcc))*(1-TEcc*TEcc);
	//Estimate time of cutoff
	double hT=sqrt(mu*p);
	double deltah=hT-h;
	double deltav=deltah/rbar;
	timeRemaining=tau*(1-exp(-deltav/Isp));
	if(timeRemaining>1000) timeRemaining=1000;
	//Any comparison with NaN is false, so
	//invert these tests so it's true if NaN
	if(!(tau>timeRemaining)|!(0<timeRemaining)) timeRemaining=tau-10;
	//  if(!(tau>T)|!(0<T))T=T2;
	//metCutoff=timeRemaining + met;
}

void AscentGuidance::Guide()
{
	//Calculate pitch steering coefficients
	double a=b0(timeRemaining);
	double b=bn(timeRemaining,1);
	double c=c0(timeRemaining);
	double d=cn(timeRemaining,1);
	double y1=TgtSpd*sin(TgtFPA*RAD)-vr;
	double y2=TgtRad-vr*timeRemaining-radius;
	double D=a*d-b*c;
	A=(d*y1-b*y2)/D;
	//B=(a*y2-c*y1)/D;

	//VECTOR3 ang_vel;
	C=(mu/(radius*radius)-(omega*omega*radius))/thrustAcceleration;
	//target_roll=0;
	//target_yaw=0;
	//fhdotrh=A+B*(met-TLastAB)+C;
	double fhdotrh=A+C;
	if(fabs(fhdotrh)>1) {
		//Commanded to do a cos(pitch) out of range
		target_pitch = STS()->GetPitch()*DEG;
	} 
	else {
		target_pitch=(PI/2-acos(fhdotrh))*DEG;
	}

	//target_pitch = target_pitch+ThrAngleP; //yaw angle ignored for now
	CmdPDot=(target_pitch-ThrAngleP*cos(STS()->GetBank())-STS()->GetPitch()*DEG)/(2*ASCENT_MAJOR_CYCLE);
	//CmdPDot=(target_pitch-ThrAngleP*cos(GetBank())-GetPitch()*DEG)/TMajorCycle;
}

void AscentGuidance::AdaptiveGuidanceThrottling( void )
{
	/*char buffer[100];
	sprintf_s( buffer, 100, "MET%f|VREL%f", STS()->GetMET(), STS()->GetAirspeed() );
	oapiWriteLog( buffer );*/
	if (AGT_done == false)
	{
		if (STS()->GetAirspeed() >= Vref_adjust)
		{
			double TDEL_adjust = STS()->GetMET() - Tref_adjust;// STS-117 data: between -0.21 and 0.21 is nominal
			// HACK using -0.5 to +0.5 for nominal, and maximum adjust if outside -1 to +1
			// TODO should be using ILOAD tables
			// TODO should also change QPOLY (?) and pitch profile
			if (TDEL_adjust < -1)// hot
			{
				THROT[1] -= 21;
			}
			else if (TDEL_adjust < -0.5)// hot
			{
				THROT[1] += round( 21 * TDEL_adjust );
			}
			else if (TDEL_adjust > 1)// cold
			{
				THROT[2] += 6;
			}
			else if (TDEL_adjust > 0.5)// cold
			{
				THROT[2] += round( 6 * TDEL_adjust );
			}

			AGT_done = true;

			char buffer[256];
			sprintf_s( buffer, 256, "TDEL_adjust:%.2f THROT2:%.1f THROT3:%.1f", TDEL_adjust, THROT[1], THROT[2] );
			oapiWriteLog( buffer );
			sprintf_s( oapiDebugString(), 256, "TDEL_adjust:%.2f THROT2:%.1f THROT3:%.1f", TDEL_adjust, THROT[1], THROT[2] );
		}
	}
	return;
}

};

