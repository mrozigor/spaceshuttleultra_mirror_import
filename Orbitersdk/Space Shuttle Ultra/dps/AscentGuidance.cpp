#include "AscentGuidance.h"
#include "../Atlantis.h"
#include <UltraMath.h>
#include "SSME_SOP.h"
#include "SSME_Operations.h"
#include "ATVC_SOP.h"
#include "SRBSepSequence.h"
#include "ETSepSequence.h"
#include "IDP.h"

namespace dps
{
	
AscentGuidance::AscentGuidance(SimpleGPCSystem* _gpc)
: SimpleGPCSoftware(_gpc, "AscentGuidance"),
  hEarth(NULL),
  lastSBTCCommand(0.0),
  stage1GuidanceVelTable(DEFAULT_STAGE1_GUIDANCE_TABLE_VEL, DEFAULT_STAGE1_GUIDANCE_TABLE_VEL+STAGE1_GUIDANCE_TABLE_SIZE),
  stage1GuidancePitchTable(DEFAULT_STAGE1_GUIDANCE_TABLE_PITCH, DEFAULT_STAGE1_GUIDANCE_TABLE_PITCH+STAGE1_GUIDANCE_TABLE_SIZE),
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

	enaSERC = false;

	glimiting = false;
	dt_thrt_glim = 0;

	MEFail[0] = false;
	MEFail[1] = false;
	MEFail[2] = false;
	NSSME = 3;

	finecount = false;
	finecountthrottle[0] = FINECOUNT_THROTTLE_2EO;
	finecountthrottle[1] = FINECOUNT_THROTTLE_1EO;
	finecountthrottle[2] = FINECOUNT_THROTTLE;// TODO update to correct MPL from mission file

	SSMETailoffDV[0] = SSME_TAILOFF_DV_91_2EO;
	SSMETailoffDV[1] = SSME_TAILOFF_DV_91_1EO;
	SSMETailoffDV[2] = SSME_TAILOFF_DV_67;

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

	bNullSRBNozzles = false;

	EOVI[0] = 0;
	EOVI[1] = 0;

	dogleg = false;
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

	pBundle=BundleManager()->CreateBundle("SPDBKTHROT_CONTROLS", 16);
	SpdbkThrotAutoIn.Connect(pBundle, 0);
	SpdbkThrotAutoOut.Connect(pBundle, 0);
	SpdbkThrotPLT.Connect(pBundle, 2);
	SpdbkThrotAutoOut.SetLine(); // default to auto throttling

	pBundle=STS()->BundleManager()->CreateBundle("HC_INPUT", 16);
	SpdbkThrotPort.Connect(pBundle, 6);

	pBundle = STS()->BundleManager()->CreateBundle( "THRUSTER_CMD", 16 );
	SERC.Connect( pBundle, 6 );

	pSSME_SOP = static_cast<SSME_SOP*> (FindSoftware( "SSME_SOP" ));
	pSSME_Operations = static_cast<SSME_Operations*> (FindSoftware( "SSME_Operations" ));
	pATVC_SOP = static_cast<ATVC_SOP*> (FindSoftware( "ATVC_SOP" ));
	pSRBSepSequence = static_cast<SRBSepSequence*> (FindSoftware( "SRBSepSequence" ));
	pETSepSequence = static_cast<ETSepSequence*> (FindSoftware( "ETSepSequence" ));
}

void AscentGuidance::OnPreStep(double SimT, double DeltaT, double MJD)
{
	switch(GetMajorMode()) {
		case 102:
			STS()->CalcSSMEThrustAngles(0, ThrAngleP, ThrAngleY);
			Throttle(DeltaT);
			FirstStageRateCommand();
			if (bNullSRBNozzles == true)
			{
				STS()->SetSRBGimbalAngles( static_cast<SIDE> (0), 0, 0 );
				STS()->SetSRBGimbalAngles( static_cast<SIDE> (1), 0, 0 );
			}
			else GimbalSRBs(DeltaT);
			GimbalSSMEs(DeltaT);
			break;
		case 103:
			Navigate();// update speed post MECO
			if (pSSME_Operations->GetZeroThrustFlag() == true)
			{
				// stop any SERC firing, TransDAP takes over now
				SERC.ResetLine();
				return;
			}

			if (pSSME_Operations->GetMECOCommandFlag() == false){
				STS()->CalcSSMEThrustAngles(0, ThrAngleP, ThrAngleY);
				//Navigate();
				if(STS()->GetMET() >= (tLastMajorCycle + ASCENT_MAJOR_CYCLE)) {
					MajorCycle();
					tLastMajorCycle = STS()->GetMET();
				}
				SecondStageRateCommand();
				GimbalSSMEs(DeltaT);
				Throttle(DeltaT);
			}
			else { //post MECO
				// hold attitude
				degReqdRates.data[PITCH] = 0.0;
				degReqdRates.data[YAW] = 0.0;
				degReqdRates.data[ROLL] = 0.0;

				// stop any existing OMS dump
				OMSCommand[LEFT].ResetLine();
				OMSCommand[RIGHT].ResetLine();

				GimbalSSMEs(DeltaT);
			}
			break;
	}
}

bool AscentGuidance::OnMajorModeChange(unsigned int newMajorMode)
{
	if(newMajorMode == 102 || newMajorMode == 103) {
		if(newMajorMode == 102) InitializeAutopilot();
		else if(newMajorMode == 103) tSRBSep = STS()->GetMET();
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
	TgtSpd=pMission->GetMECOVel() - (SSMETailoffDV[2] / MPS2FPS);
	MaxThrust = pMission->GetMaxSSMEThrust();
	THROT[0] = MaxThrust;
	THROT[1] = MaxThrust;
	THROT[3] = MaxThrust;
	PerformRTHU=pMission->PerformRTHU();
	QPOLY[2] = pMission->GetTHdownVelocity()/MPS2FPS;
	QPOLY[3] = pMission->GetTHupVelocity()/MPS2FPS;
	if(pMission->UseOMSAssist()) {
		OMSAssistDuration = pMission->GetOMSAssistDuration();
	}
	else OMSAssistDuration = 0;

	hEarth = STS()->GetSurfaceRef();
	//calculate heading
	double latitude, longitude;
	STS()->GetEquPos(longitude, latitude, EarthRadius);
	if(abs(cos(TgtInc*RAD))>abs(cos(latitude))) {
		radTargetHeading=PI/2;
		//TgtInc = latitude*DEG;
		dogleg = true;
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
		//pATVC_SOP->SetSRBActPos( i + 1, pitchGimbal+rollGimbal, yawGimbal );
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

	switch (((int)pSSME_Operations->GetFailFlag( 3 ) * 4) + ((int)pSSME_Operations->GetFailFlag( 2 ) * 2) + (int)pSSME_Operations->GetFailFlag( 1 ))
	{
		case 0:// nom
			pitchGimbal[0] = -range(-8.0, degRateError.data[PITCH], 8.0);
			yawGimbal[0] = -range(-8.0, degRateError.data[YAW], 8.0) - range(-8.0, 0.75*degRateError.data[ROLL], 8.0);
			pitchGimbal[1] = -range(-8.0, degRateError.data[PITCH], 8.0) + range(-7.0, 2.5*degRateError.data[ROLL], 7.0);
			yawGimbal[1] = -range(-8.0, degRateError.data[YAW], 8.0);
			pitchGimbal[2] = -range(-8.0, degRateError.data[PITCH], 8.0) - range(-7.0, 2.5*degRateError.data[ROLL], 7.0);
			yawGimbal[2] = -range(-8.0, degRateError.data[YAW], 8.0);

			// In this case ME-2 and 3 not pointing thru c.g. in yaw, instead pointing strait forward.
			// Need hard data to be sure, but looks like it's this way in reality.
			pitchGimbal[0] += ThrAngleP + 16;
			yawGimbal[0] += ThrAngleY;
			pitchGimbal[1] += ThrAngleP + 10;
			yawGimbal[1] += ThrAngleY + 3.5;
			pitchGimbal[2] += ThrAngleP + 10;
			yawGimbal[2] += ThrAngleY - 3.5;

			pATVC_SOP->SetSSMEActPos( 1, pitchGimbal[0], yawGimbal[0] );
			pATVC_SOP->SetSSMEActPos( 2, pitchGimbal[1], yawGimbal[1] );
			pATVC_SOP->SetSSMEActPos( 3, pitchGimbal[2], yawGimbal[2] );
			break;
		case 1:// C out
			pitchGimbal[1] = -range(-8.0, degRateError.data[PITCH], 8.0) + range(-7.0, 1.5*degRateError.data[ROLL], 7.0);
			yawGimbal[1] = -range(-8.0, degRateError.data[YAW], 8.0);
			pitchGimbal[2] = -range(-8.0, degRateError.data[PITCH], 8.0) - range(-7.0, 1.5*degRateError.data[ROLL], 7.0);
			yawGimbal[2] = -range(-8.0, degRateError.data[YAW], 8.0);

			STS()->CalcSSMEThrustAngles( 2, ThrAngleP, ThrAngleY );
			pitchGimbal[1] += ThrAngleP + 10;
			yawGimbal[1] += ThrAngleY + 3.5;

			STS()->CalcSSMEThrustAngles( 3, ThrAngleP, ThrAngleY );
			pitchGimbal[2] += ThrAngleP + 10;
			yawGimbal[2] += ThrAngleY - 3.5;

			pATVC_SOP->SetSSMEActPos( 2, pitchGimbal[1], yawGimbal[1] );
			pATVC_SOP->SetSSMEActPos( 3, pitchGimbal[2], yawGimbal[2] );
			break;
		case 2:// L out
			pitchGimbal[0] = -range(-8.0, degRateError.data[PITCH], 8.0);
			yawGimbal[0] = -range(-8.0, degRateError.data[YAW], 8.0) - range(-8.0, 0.75*degRateError.data[ROLL], 8.0);
			pitchGimbal[2] = -range(-8.0, degRateError.data[PITCH], 8.0) - range(-7.0, 2.5*degRateError.data[ROLL], 7.0);
			yawGimbal[2] = -range(-8.0, degRateError.data[YAW], 8.0);

			STS()->CalcSSMEThrustAngles( 1, ThrAngleP, ThrAngleY );
			pitchGimbal[0] += ThrAngleP + 16;
			yawGimbal[0] += ThrAngleY;

			STS()->CalcSSMEThrustAngles( 3, ThrAngleP, ThrAngleY );
			pitchGimbal[2] += ThrAngleP + 10;
			yawGimbal[2] += ThrAngleY - 3.5;

			pATVC_SOP->SetSSMEActPos( 1, pitchGimbal[0], yawGimbal[0] );
			pATVC_SOP->SetSSMEActPos( 3, pitchGimbal[2], yawGimbal[2] );
			break;
		case 3:// C, L out
			pitchGimbal[2] = -range(-8.0, degRateError.data[PITCH], 8.0);
			yawGimbal[2] = -range(-8.0, degRateError.data[YAW], 8.0);

			pitchGimbal[2] += ThrAngleP + 10;
			yawGimbal[2] += ThrAngleY - 3.5;

			pATVC_SOP->SetSSMEActPos( 3, pitchGimbal[2], yawGimbal[2] );
			break;
		case 4:// R out
			pitchGimbal[0] = -range(-8.0, degRateError.data[PITCH], 8.0);
			yawGimbal[0] = -range(-8.0, degRateError.data[YAW], 8.0) - range(-8.0, 0.75*degRateError.data[ROLL], 8.0);
			pitchGimbal[1] = -range(-8.0, degRateError.data[PITCH], 8.0) + range(-7.0, 2.5*degRateError.data[ROLL], 7.0);
			yawGimbal[1] = -range(-8.0, degRateError.data[YAW], 8.0);

			STS()->CalcSSMEThrustAngles( 1, ThrAngleP, ThrAngleY );
			pitchGimbal[0] += ThrAngleP + 16;
			yawGimbal[0] += ThrAngleY;

			STS()->CalcSSMEThrustAngles( 2, ThrAngleP, ThrAngleY );
			pitchGimbal[1] += ThrAngleP + 10;
			yawGimbal[1] += ThrAngleY + 3.5;

			pATVC_SOP->SetSSMEActPos( 1, pitchGimbal[0], yawGimbal[0] );
			pATVC_SOP->SetSSMEActPos( 2, pitchGimbal[1], yawGimbal[1] );
			break;
		case 5:// C, R out
			pitchGimbal[1] = -range(-8.0, degRateError.data[PITCH], 8.0);
			yawGimbal[1] = -range(-8.0, degRateError.data[YAW], 8.0);

			pitchGimbal[1] += ThrAngleP + 10;
			yawGimbal[1] += ThrAngleY + 3.5;

			pATVC_SOP->SetSSMEActPos( 2, pitchGimbal[1], yawGimbal[1] );
			break;
		case 6:// L, R out
			pitchGimbal[0] = -range(-8.0, degRateError.data[PITCH], 8.0);
			yawGimbal[0] = -range(-8.0, degRateError.data[YAW], 8.0);
			
			pitchGimbal[0] += ThrAngleP + 16;
			yawGimbal[0] += ThrAngleY;

			pATVC_SOP->SetSSMEActPos( 1, pitchGimbal[0], yawGimbal[0] );
			break;
	}

	// SERC
	if (enaSERC == true)
	{
		if (abs( degRateError.data[ROLL] ) < 0.1) SERC.ResetLine();
		else SERC.SetLine( static_cast<float>(-range( -1, 0.5 * degRateError.data[ROLL], 1 )) );
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
			degReqdRates.data[YAW] = range(-2.5, 0.5*(DEG*(radHeading-radTargetHeading) - (sign( cos( degBank ) ) * ThrAngleY)), 2.5);
			// applied the "- (sign( cos( degBank ) ) * ThrAngleY)" factor to correct for "sideways" thrust (ME-2 or 3 out/low thrust)

			if(!PerformRTHU || STS()->GetAirspeed()<ROLL_TO_HEADS_UP_VELOCITY) 
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

	// detect EO
	for (int i = 0; i < 3; i++)
	{
		if (MEFail[i] != pSSME_Operations->GetFailFlag( i + 1 ))
		{
			MEFail[i] = true;
			NSSME--;
			// record EO VI
			VECTOR3 v3vi;
			STS()->GetRelativeVel( STS()->GetSurfaceRef(), v3vi );
			double vi = length( v3vi ) * MPS2FPS;
			if (NSSME == 2) EOVI[0] = vi;
			else if (NSSME == 1) EOVI[1] = vi;
			// update 1� stage throttle table to not throttle
			THROT[1] = THROT[0];
			THROT[2] = THROT[0];
			AGT_done = true;// don't do AGT
			glimiting = false;// reset g-limiting
			dt_thrt_glim = -2;// HACK delay g-limiting action by 2sec (if it re-triggers) to account for failed engine tailoff thrust
			// throttle to mission power level
			throttlecmd = THROT[0];
			if (J > 0) J--;
			// update MECO targets
			if (NSSME > 0) TgtSpd = STS()->pMission->GetMECOVel() - (SSMETailoffDV[NSSME - 1] / MPS2FPS);
		}
	}

	// SERC
	switch (GetMajorMode())
	{
		case 102:
			if ((NSSME == 1) && (pSRBSepSequence->GetPC50Flag() == true))// enable SERC in MM102 only at SRB tailoff
			{
				enaSERC = true;
			}
			break;
		case 103:
			if (NSSME == 1)// enable SERC automatically in MM103
			{// TODO enable SERC when "sensed acceleration falls below a predefined limit"
				enaSERC = true;
			}
			break;
	}

	// low-level sensor arm
	if (STS()->GetMass() < (LOWLEVEL_ARM_MASS * LBM)) pSSME_Operations->SetLowLevelSensorArmFlag();


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
				break;
			case 103: // STAGE 3
				//OMS Assist
				if ((STS()->GetMET() >= (tSRBSep + 10)) && (STS()->GetMET() < (tSRBSep + 10 + OMSAssistDuration)))
				{
					OMSCommand[LEFT].SetLine();
					OMSCommand[RIGHT].SetLine();
				}
				else
				{
					OMSCommand[LEFT].ResetLine();
					OMSCommand[RIGHT].ResetLine();
				}

				if(inertialVelocity>=TgtSpd) {
					//reached target speed
					if (pSSME_Operations->GetMECOCommandFlag() == false)
					{
						pSSME_Operations->SetMECOCommandFlag();

						char buffer[64];
						sprintf_s( buffer, 64, "MECO @ MET %.2f", STS()->GetMET() );
						oapiWriteLog( buffer );
						return;
					}
				}

				// g limiting
				if (thrustAcceleration > ALIM2) glimiting = true;
				if ((glimiting == true) && (thrustAcceleration > ALIM1))
				{
					// TODO use correct MPL value below
					if (throttlecmd != 67)// if at MPL can't do more
					{
						if (dt_thrt_glim >= 0.1)// wait while throttling (10%/sec throttle change = 0.1s delay)
						{
							throttlecmd--;// throttle back 1%
							throttlecmd = (double)Round( throttlecmd );// round avoid x.5% cmds
							if (throttlecmd < 67) throttlecmd = 67;// don't go below MPL because it won't work
							pSSME_SOP->SetThrottlePercent( throttlecmd );
							dt_thrt_glim = 0;// reset
						}
						else dt_thrt_glim += DeltaT;
					}
				}

				// fine count
				// HACK only throttle back, no real count for now
				if ((timeRemaining <= 6) && (finecount == false))
				{
					throttlecmd = finecountthrottle[NSSME - 1];
					pSSME_SOP->SetThrottlePercent( throttlecmd );
					finecount = true;
					char buffer[64];
					sprintf_s( buffer, 64, "Fine Count (throttle to %.0f%%) @ MET %.2f", throttlecmd, STS()->GetMET() );
					oapiWriteLog( buffer );
				}
				break;
		}
	}
	else { // manual throttling
		pSSME_SOP->SetThrottlePercent( Round( 2 * SBTCCommand ) * 0.5 );// rounded to the nearest 0.5%
	}
	
	lastSBTCCommand = SBTCCommand;
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
	double azimuth/*,equator_v/*,target_radius*/;
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

	if (latitude > (TgtInc * RAD)) azimuth = PI / 2;
	else azimuth = asin(range(-1, cos(TgtInc*RAD)/cos(latitude), 1));  // this equ doesn't take rotation into accout
	//equator_v=EarthRadius*(2*PI/SidDay);   //equator velocity
	if ((dogleg == true) && (latitude < (TgtInc * RAD))) tgt_orbit_v[0]=-TgtSpd*cos(TgtFPA*RAD)*cos(azimuth);
	else tgt_orbit_v[0]=TgtSpd*cos(TgtFPA*RAD)*cos(azimuth) * sign( 65 - TgtInc ); // northern velocity
	tgt_orbit_v[1]=TgtSpd*cos(TgtFPA*RAD)*sin(azimuth); // eastern velocity
	//lnch_v[0]= abs(tgt_orbit_v[0]) - abs(current_vel[0]); // taking current velocity into accout for CC (North); assume both values have same sign
	//lnch_v[1]= abs(tgt_orbit_v[1]) - abs(current_vel[1]); // taking current velocity into accout for CC (East); assume both values have same sign
	lnch_v[0]= fabs( tgt_orbit_v[0] - current_vel[0] );
	lnch_v[1]= fabs( tgt_orbit_v[1] - current_vel[1] );

	//if (lnch_v[0]==0) lnch_v[0]=0.01; //div by zero protection	
	/*if(lnch_v[0]==0.0) { //div by zero protection
		if(lnch_v[1]>0) true_azimuth=PI/2;
		else true_azimuth=-PI/2;
	}*/
	//else true_azimuth = atan(lnch_v[1]/lnch_v[0]); // tan(azimuth) = eastern_vel / northern_vel
	true_azimuth = atan2(lnch_v[1], lnch_v[0]); // tan(azimuth) = eastern_vel / northern_vel
	if ((current_vel[0] < 0.0) && (TgtInc > 65)) true_azimuth = PI + true_azimuth;
	if ((tgt_orbit_v[1] > 0.0) && (TgtInc > 65)) true_azimuth = 2*PI - true_azimuth;
	//if(current_vel[0] < 0.0) true_azimuth = PI - true_azimuth; // we are heading south, so need to use southerly heading
	//if(current_vel[1] < 0.0) true_azimuth = 2*PI - true_azimuth; // retrograde inclination

	if ((dogleg == true) && (latitude < (TgtInc * RAD))) true_azimuth = PI - true_azimuth;
	
	return true_azimuth;
}

void AscentGuidance::Navigate()
{
	VECTOR3 rv, vv, hv;
	STS()->GetRelativePos(STS()->GetSurfaceRef(),rv);
	STS()->GetRelativeVel(STS()->GetSurfaceRef(),vv);
	radius=length(rv);
	inertialVelocity=length(vv);
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
		if ((STS()->GetAirspeed() * MPS2FPS) > Vref_adjust)
		{
			double TDEL_adjust = STS()->GetMET() - Tref_adjust;// STS-117 data: between -0.21 and 0.21 is nominal
			// HACK using -0.2 to +0.2 for nominal, and maximum adjust if outside -1 to +1
			// TODO should be using ILOAD tables
			// TODO should also change QPOLY and pitch profile
			if (TDEL_adjust < -1)// hot
			{
				THROT[1] = Round( THROT[1] - 21.5 );
			}
			else if (TDEL_adjust < -0.2)// hot
			{
				THROT[1] = Round( THROT[1] + ((26.25 * TDEL_adjust) + 4.75) );
			}
			else if (TDEL_adjust > 1)// cold
			{
				THROT[2] += 8;
			}
			else if (TDEL_adjust > 0.2)// cold
			{
				THROT[2] += Round( (7.5 * TDEL_adjust) + 0.5 );
			}

			AGT_done = true;

			char buffer[256];
			sprintf_s( buffer, 256, "TDEL_adjust:%.2f THROT2:%.1f THROT3:%.1f", TDEL_adjust, THROT[1], THROT[2] );
			oapiWriteLog( buffer );
		}
	}
	return;
}

void AscentGuidance::NullSRBNozzles( void )
{
	bNullSRBNozzles = true;
	return;
}

bool AscentGuidance::OnPaint( int spec, vc::MDU* pMDU ) const
{
	if (spec != dps::MODE_UNDEFINED) return false;

	if (GetMajorMode() == 102) ASCENTTRAJ1( pMDU );
	else ASCENTTRAJ2( pMDU );
	return true;
}

void AscentGuidance::ASCENTTRAJ1( vc::MDU* pMDU ) const// OI-32 PASS ASCENT TRAJ 1
{
	PrintCommonHeader( "  ASCENT TRAJ 1", pMDU );
	
	// static parts (labels)
	pMDU->mvprint( 7, 5, "CONT ABORT" );
	pMDU->mvprint( 8, 6, "3EO BLUE" );
	pMDU->mvprint( 1, 7, "2  ARM 2EO BLUE" );
	pMDU->mvprint( 1, 8, "4  ABORT" );
	pMDU->mvprint( 1, 10, "5 INH YAW STEER" );
	pMDU->mvprint( 1, 12, "6  SERC" );
	pMDU->mvprint( 38, 17, "THROT" );
	pMDU->mvprint( 18, 22, "7 INH DRP 1EO" );

	pMDU->mvprint( 31, 15, "40" );
	pMDU->mvprint( 24, 18, "50" );
	pMDU->mvprint( 19, 20, "60" );
	pMDU->mvprint( 11, 23, "70" );

	// static parts (lines)
	pMDU->Line( 39, 208, 58, 205 );
	pMDU->Line( 58, 205, 77, 195 );
	pMDU->Line( 77, 195, 104, 170 );
	pMDU->Line( 104, 170, 177, 114 );
	pMDU->Line( 177, 114, 214, 82 );
	pMDU->Line( 214, 82, 222, 70 );
	pMDU->Line( 222, 70, 230, 45 );

	pMDU->Line( 157, 124, 157, 131 );
	pMDU->Line( 123, 151, 123, 158 );
	pMDU->Line( 95, 173, 95, 180 );
	pMDU->Line( 55, 203, 55, 210 );

	// dynamic parts
	char cbuf[64];

	sprintf_s( cbuf, 64, "%3d", (int)throttlecmd );
	pMDU->mvprint( 44, 17, cbuf );

	if (pSRBSepSequence->GetPC50Flag() == true) pMDU->mvprint( 22, 9, "PC<50", dps::DEUATT_OVERBRIGHT | dps::DEUATT_FLASHING );

	//if (pSRBSepSequence->GetSRBSEPINHFlag() == true) pMDU->mvprint( 10, 11, "SEP INH" );

	if (enaSERC == true) pMDU->mvprint( 9, 12, "ON", dps::DEUATT_OVERBRIGHT );

	if (EOVI[0] != 0)
	{
		sprintf_s( cbuf, 64, "1ST EO VI %5.0f", EOVI[0] );
		pMDU->mvprint( 34, 22, cbuf );
	}
	if (EOVI[1] != 0)
	{
		sprintf_s( cbuf, 64, "2ND EO VI %5.0f", EOVI[1] );
		pMDU->mvprint( 34, 23, cbuf, dps::DEUATT_OVERBRIGHT );
	}

	double VR = STS()->GetAirspeed() * MPS2FPS;
	double Altitude = STS()->GetAltitude() * MPS2FPS;

	//Draw triangle for state vector
	short stY = static_cast<short>(208 - (Altitude * 0.00105556));
	short stX = static_cast<short>(27 + (VR * 0.0506667));
	pMDU->Line( stX, stY - 3, stX - 3, stY + 3, dps::DEUATT_OVERBRIGHT );
	pMDU->Line( stX - 3, stY + 3, stX + 3, stY + 3, dps::DEUATT_OVERBRIGHT );
	pMDU->Line( stX + 3, stY + 3, stX, stY - 3, dps::DEUATT_OVERBRIGHT );

	VECTOR3 thr;
	STS()->GetThrustVector( thr );
	thr /= STS()->GetMass();
	VECTOR3 LVLH_Vel;
	STS()->GetGPCLVLHVel(0, LVLH_Vel);
	// 20s predictor
	VR += (length( thr ) * cos( -STS()->GetAOA() - atan( thr.y / thr.z ) ) - G * sin( PI - STS()->GetPitch() - STS()->GetAOA() )) * 20 * MPS2FPS;
	Altitude += -LVLH_Vel.z * 20 + ((length( thr ) * sin( STS()->GetPitch() + atan( thr.y / thr.z ) ) - G) * 400) * MPS2FPS;
	if (VR < 0) VR = 0;
	if (Altitude < 0) Altitude = 0;
	stY = static_cast<short>(208 - (Altitude * 0.00105556));
	stX = static_cast<short>(27 + (VR * 0.0506667));
	pMDU->Ellipse( stX - 3, stY - 3, stX + 3, stY + 3, dps::DEUATT_OVERBRIGHT );
	return;
}

void AscentGuidance::ASCENTTRAJ2( vc::MDU* pMDU ) const// OI-32 PASS ASCENT TRAJ 2
{
	PrintCommonHeader( "  ASCENT TRAJ 2", pMDU );
	
	// static parts (labels)
	pMDU->mvprint( 9, 4, "25" );
	pMDU->mvprint( 49, 4, "26" );
	pMDU->mvprint( 38, 5, "TGO   :" );
	pMDU->mvprint( 7, 5, "CONT ABORT" );
	pMDU->mvprint( 8, 6, "3EO" );
	pMDU->mvprint( 1, 7, "2  ARM 2EO" );
	pMDU->mvprint( 1, 8, "4  ABORT" );
	pMDU->mvprint( 1, 10, "5 INH YAW STEER" );
	pMDU->mvprint( 1, 12, "6  SERC" );
	pMDU->mvprint( 38, 17, "THROT" );
	pMDU->mvprint( 38, 18, "PRPLT" );
	pMDU->mvprint( 38, 20, "TMECO   :" );
	pMDU->mvprint( 19, 19, "DROOP ALT   8" );
	pMDU->mvprint( 18, 22, "7     DRP 1EO" );


	// static parts (lines)
	//Nominal ascent line
	pMDU->Line( 58, 186, 63, 165 );
	pMDU->Line( 63, 165, 68, 152 );
	pMDU->Line( 68, 152, 73, 140 );
	pMDU->Line( 73, 140, 83, 126 );
	pMDU->Line( 83, 126, 98, 115 );
	pMDU->Line( 98, 115, 106, 111 );
	pMDU->Line( 106, 111, 121, 108 );
	pMDU->Line( 121, 108, 136, 108 );
	pMDU->Line( 136, 108, 188, 114 );
	pMDU->Line( 188, 114, 225, 116 );
	pMDU->Line( 225, 116, 248, 115 );

	pMDU->Line( 135, 97, 161, 102 );
	pMDU->Line( 161, 102, 194, 107 );
	pMDU->Line( 194, 107, 234, 107 );

	// Vr line
	pMDU->Line( 50, 31, 249, 31 );
	// 25K mark
	pMDU->Line( 50, 31, 50, 36 );
	// 26K mark
	pMDU->Line( 249, 31, 249, 36 );

	// dynamic parts
	char cbuf[64];
	int tmp = 0;

	// Vr scale
	if (((TgtSpd * MPS2FPS) > 25000) && ((TgtSpd * MPS2FPS) < 26000))// TODO update for MECO speeds > 25K???
	{
		// CO mark
		tmp = Round( ((((TgtSpd * MPS2FPS) - 25000)) * 0.199) ) + 50;
		pMDU->Line( tmp, 31, tmp, 36 );
		tmp = Round( tmp * 0.2 ) - 1;
		pMDU->mvprint( tmp, 4, "CO" );
	}
	if ((inertialVelocity * MPS2FPS) < 25000) tmp = 50;
	else if ((inertialVelocity * MPS2FPS) > 26000) tmp = 249;
	else tmp = Round( ((((inertialVelocity * MPS2FPS) - 25000)) * 0.199) ) + 50;
	pMDU->Line( tmp, 30, tmp - 3, 26, dps::DEUATT_OVERBRIGHT );
	pMDU->Line( tmp - 3, 26, tmp + 3, 26, dps::DEUATT_OVERBRIGHT );
	pMDU->Line( tmp + 3, 26, tmp, 30, dps::DEUATT_OVERBRIGHT );

	sprintf_s( cbuf, 64, "%3d", (int)throttlecmd );
	pMDU->mvprint( 44, 17, cbuf );

	tmp = STS()->GetETPropellant();
	if (tmp < 0) tmp = 0;
	sprintf_s( cbuf, 64, "%2d", tmp );
	pMDU->mvprint( 44, 18, cbuf );

	// contigency abort boundaries
	// 3EO
	if ((inertialVelocity * MPS2FPS) < 18000) pMDU->mvprint( 12, 6, "GREEN" );
	// 2EO
	if ((inertialVelocity * MPS2FPS) < 12100)
	{
		VECTOR3 vr3;
		STS()->GetHorizonAirspeedVector( vr3 );
		if ((vr3.y * MPS2FPS) < 1850) pMDU->mvprint( 12, 7, "GREEN" );
		else pMDU->mvprint( 13, 7, "BLUE" );
	}

	//if (pETSepSequence->GetETSEPINHFlag() == true) pMDU->mvprint( 10, 11, "SEP INH" );

	if (enaSERC == true) pMDU->mvprint( 9, 12, "ON", dps::DEUATT_OVERBRIGHT );

	if (EOVI[0] != 0)
	{
		sprintf_s( cbuf, 64, "1ST EO VI %5.0f", EOVI[0] );
		pMDU->mvprint( 34, 22, cbuf );
	}
	if (EOVI[1] != 0)
	{
		sprintf_s( cbuf, 64, "2ND EO VI %5.0f", EOVI[1] );
		pMDU->mvprint( 34, 23, cbuf, dps::DEUATT_OVERBRIGHT );
	}
	else pMDU->mvprint( 20, 22, "INH" );

	if ((pSSME_Operations->GetMECOConfirmedFlag() == false) && (pSSME_Operations->GetMECOCommandFlag() == false))
	{
		// TGO
		tmp = Round( timeRemaining );
		sprintf_s( cbuf, 64, "%2d", (tmp - (tmp % 60)) / 60 );
		pMDU->mvprint( 42, 5, cbuf );
		sprintf_s( cbuf, 64, "%02d", (tmp % 60) );
		pMDU->mvprint( 45, 5, cbuf );
		// TMECO
		tmp = Round( STS()->GetMET() + timeRemaining );
		sprintf_s( cbuf, 64, "%2d", (tmp - (tmp % 60)) / 60 );
		pMDU->mvprint( 44, 20, cbuf );
		sprintf_s( cbuf, 64, "%02d", (tmp % 60) );
		pMDU->mvprint( 47, 20, cbuf );

		double droopH = DROOP_ALT;// TODO
		char att = 0;
		if (droopH < DROOP_ALT) att = dps::DEUATT_FLASHING;
		sprintf_s( cbuf, 64, "DROOP ALT %3.0f", droopH * 0.001 );
		pMDU->mvprint( 19, 19, cbuf, att );
	}

	VECTOR3 LVLH_Vel;
	STS()->GetGPCLVLHVel(0, LVLH_Vel);

	//Current vehicle state (57NM insertion altitude):
	double VHI = LVLH_Vel.x;
	double Altitude = STS()->GetAltitude() * MPS2FPS;
	//Draw triangle for state vector
	short stY = static_cast<short>(315.358974 - (Altitude * 0.000641026));
	short stX = static_cast<short>(VHI * 0.0102);
	pMDU->Line( stX, stY - 3, stX - 3, stY + 3, dps::DEUATT_OVERBRIGHT );
	pMDU->Line( stX - 3, stY + 3, stX + 3, stY + 3, dps::DEUATT_OVERBRIGHT );
	pMDU->Line( stX + 3, stY + 3, stX, stY - 3, dps::DEUATT_OVERBRIGHT );
		
	// HACK using constant 12� for SSME offset
	// 30s predictor
	double dv30 = thrustAcceleration * cos( STS()->GetSlipAngle() ) * 30 * MPS2FPS;
	VHI += dv30;
	Altitude += -LVLH_Vel.z * 30 + (((thrustAcceleration * sin( STS()->GetPitch() - (12 * RAD * sign( cos( STS()->GetBank() ) )) )) - G ) * 450 * MPS2FPS);
	//Draw circle for 30s predictor
	stY = static_cast<short>(315.358974 - (Altitude * 0.000641026));
	stX = static_cast<short>(VHI * 0.0102);
	pMDU->Ellipse( stX - 3, stY - 3, stX + 3, stY + 3, dps::DEUATT_OVERBRIGHT );

	// 60s predictor
	VHI += dv30;
	Altitude = (STS()->GetAltitude() * MPS2FPS) - (LVLH_Vel.z * 60) + ((thrustAcceleration * sin( STS()->GetPitch() - (12 * RAD * sign( cos( STS()->GetBank() ) )) )) - G ) * 1800 * MPS2FPS;
	//Draw circle for 60s predictor
	stY = static_cast<short>(315.358974 - (Altitude * 0.000641026));
	stX = static_cast<short>(VHI * 0.0102);
	pMDU->Ellipse( stX - 3, stY - 3, stX + 3, stY + 3, dps::DEUATT_OVERBRIGHT );
	return;
}

// INFO below is the pre OI-32 PASS ASCENT TRAJ display
//bool AscentGuidance::OnPaint( int spec, vc::MDU* pMDU ) const
//{
//	if (spec != dps::MODE_UNDEFINED) return false;
//	
//	// PASS LAUNCH TRJ/PASS ASCENT TRAJ
//	switch(GetMajorMode())
//	{
//		case 101:
//			PrintCommonHeader( "  LAUNCH TRAJ", pMDU );
//			break;
//		case 102:
//		case 103:
//			PrintCommonHeader( "  ASCENT TRAJ", pMDU );
//			break;
//	}
//
//	// static parts (labels)
//	pMDU->mvprint( 9, 5, "CO" );
//	pMDU->mvprint( 25, 5, "PD" );
//	pMDU->mvprint( 32, 5, "PD3" );
//
//	pMDU->mvprint( 36, 6, "ABORT    ARM" );
//	pMDU->mvprint( 33, 7, "3 E/O" );
//	pMDU->mvprint( 33, 8, "2 E/O" );
//	pMDU->mvprint( 46, 8, "2" );
//	pMDU->mvprint( 31, 9, "ABORT      4" );
//	pMDU->mvprint( 31, 10, "YAW STEER  5" );
//
//	pMDU->mvprint( 10, 7, "GUID" );
//	pMDU->mvprint( 10, 8, "TMECO   :" );
//	pMDU->mvprint( 10, 9, "PRPLT" );
//	pMDU->mvprint( 7, 12, "6  SERC" );
//
//	pMDU->mvprint( 5, 13, "O" );
//
//	pMDU->mvprint( 46, 13, "GO" );
//	pMDU->mvprint( 39, 15, "RTLS" );
//
//
//	// static parts (lines)
//
//	//Nominal ascent line
//	pMDU->Line( 181, 212, 191, 176 );
//	pMDU->Line( 191, 176, 233, 128 );
//	pMDU->Line( 233, 128, 255, 117 );
//
//	//EO at Lift-Off line
//	pMDU->Line( 159, 212, 174, 138 );
//	pMDU->Line( 174, 138, 78, 195 );
//	pMDU->Line( 78, 195, 58, 201 );
//	pMDU->Line( 58, 201, 21, 176 );
//
//	pMDU->Line( 183, 119, 176, 122 );
//	pMDU->Line( 176, 122, 81, 187 );
//
//	//Q = 2 line
//	pMDU->Line( 38, 187, 11, 179 );
//
//	//Q = 10 line
//	pMDU->Line( 38, 171, 11, 157 );
//
//	//Hdot indicator
//	pMDU->Line( 17, 30, 11, 30 );
//	pMDU->Line( 11, 30, 11, 206 );
//	pMDU->Line( 11, 206, 17, 206 );
//	pMDU->Line( 11, 118, 17, 118 );
//
//	//DR indicator
//	pMDU->Line( 26, 27, 247, 27 );
//
//	//PD3 Mark
//	pMDU->Line( 164, 27, 164, 35 );
//
//	//PD Mark
//	pMDU->Line( 128, 27, 128, 35 );
//
//	//CO Mark
//	pMDU->Line( 47, 27, 47, 35 );
//
//
//	// dynamic parts
//	char cbuf[64];
//	int tmp = 0;
//
//	if ((GetMajorMode() == 103) && (pSSME_Operations->GetMECOConfirmedFlag() == false))
//	{
//		tmp = Round( STS()->GetMET() + timeRemaining );
//		sprintf_s( cbuf, 64, "%02d", (tmp - (tmp % 60)) / 60 );
//		pMDU->mvprint( 16, 8, cbuf );
//		sprintf_s( cbuf, 64, "%02d", (tmp % 60) );
//		pMDU->mvprint( 19, 8, cbuf );
//	}
//
//	tmp = STS()->GetETPropellant();
//	if (tmp < 0) tmp = 0;
//	sprintf_s( cbuf, 64, "%2d", tmp );
//	pMDU->mvprint( 19, 9, cbuf );
//
//	if ((pSRBSepSequence->GetPC50Flag() == true) && (GetMajorMode() == 102)) pMDU->mvprint( 10, 10, "PC<50" );
//
//	if ((pSRBSepSequence->GetSRBSEPINHFlag() == true) || (pETSepSequence->GetETSEPINHFlag() == true)) pMDU->mvprint( 10, 11, "SEP INH" );
//
//	if (enaSERC == true) pMDU->mvprint( 15, 12, "ON", dps::DEUATT_OVERBRIGHT );
//
//	if (EOVI[0] != 0)
//	{
//		sprintf_s( cbuf, 64, "EO VI %5.0f", EOVI[0] );
//		pMDU->mvprint( 7, 13, cbuf );
//	}
//	if (EOVI[1] != 0)
//	{
//		sprintf_s( cbuf, 64, "EO VI %5.0f", EOVI[1] );
//		pMDU->mvprint( 7, 14, cbuf, dps::DEUATT_OVERBRIGHT );
//	}
//
//	VECTOR3 LVLH_Vel;
//	STS()->GetGPCLVLHVel(0, LVLH_Vel);
//
//	double Ref_hdot = 0.0;
//	bool bShowHDot = (STS()->GetGPCRefHDot(0, Ref_hdot) == VARSTATE_OK);
//
//	//Hdot indicator
//	if(bShowHDot)
//	{
//		double HDot_Error = -LVLH_Vel.z - Ref_hdot;
//		char att = dps::DEUATT_OVERBRIGHT;
//
//		if(HDot_Error > 200.0)
//		{
//			HDot_Error = 200.0;
//			att += dps::DEUATT_FLASHING;
//		}
//		else if (HDot_Error < -200.0)
//		{
//			HDot_Error = -200.0;
//			att += dps::DEUATT_FLASHING;
//		}
//
//		short sHDot_pry = -static_cast<short>(HDot_Error/200.0 * 88);
//
//		pMDU->Line( 11, 118 + sHDot_pry, 6, 113 + sHDot_pry, att );
//		pMDU->Line( 6, 113 + sHDot_pry, 6, 123 + sHDot_pry, att );
//		pMDU->Line( 6, 123 + sHDot_pry, 11, 118 + sHDot_pry, att );
//	}
//
//	if (GetMajorMode() >= 103)// HACK because thrustAcceleration is only calculated in MM103
//	{
//		//Current vehicle state:
//		double VHI = LVLH_Vel.x;
//		double Altitude = STS()->GetAltitude() * MPS2FPS;
//
//		if(Altitude > 155500 && VHI < 10000)
//		{
//			//Draw triangle for state vector
//			short stY = static_cast<short>(255*(1.13256 - (Altitude/513955.985)));
//			short stX = static_cast<short>(255*(0.36194 + (VHI/15672.3964)));
//			pMDU->Line( stX, stY - 3, stX - 3, stY + 3 );
//			pMDU->Line( stX - 3, stY + 3, stX + 3, stY + 3 );
//			pMDU->Line( stX + 3, stY + 3, stX, stY - 3 );
//		}
//		
//		// HACK using constant 12� for SSME offset
//		// 30s predictor
//		double dv30 = thrustAcceleration * cos( STS()->GetSlipAngle() ) * 30 * MPS2FPS;
//		VHI += dv30;
//		Altitude += -LVLH_Vel.z * 30 + (((thrustAcceleration * sin( STS()->GetPitch() - (12 * RAD * sign( cos( STS()->GetBank() ) )) )) - G ) * 450 * MPS2FPS);
//
//		if(Altitude > 155500 && VHI < 10000)
//		{
//			//Draw circle for 30s predictor
//			short stY = static_cast<short>(255*(1.13256 - (Altitude/513955.985)));
//			short stX = static_cast<short>(255*(0.36194 + (VHI/15672.3964)));
//			pMDU->Ellipse( stX - 3, stY - 3, stX + 3, stY + 3 );
//		}
//
//		// 60s predictor
//		VHI += dv30;
//		Altitude = (STS()->GetAltitude() * MPS2FPS) - (LVLH_Vel.z * 60) + ((thrustAcceleration * sin( STS()->GetPitch() - (12 * RAD * sign( cos( STS()->GetBank() ) )) )) - G ) * 1800 * MPS2FPS;
//
//		if(Altitude > 155500 && VHI < 10000)
//		{
//			//Draw circle for 60s predictor
//			short stY = static_cast<short>(255*(1.13256 - (Altitude/513955.985)));
//			short stX = static_cast<short>(255*(0.36194 + (VHI/15672.3964)));
//			pMDU->Ellipse( stX - 3, stY - 3, stX + 3, stY + 3 );
//		}
//	}
//	return true;
//}

bool AscentGuidance::GetAutoThrottleState( void ) const
{
	return SpdbkThrotAutoIn.IsSet();
}

VECTOR3 AscentGuidance::GetAttitudeErrors( void ) const
{
	// HACK this is not the the attitude error (but it's better than nothing...?)
	return degReqdRates;
}

};
