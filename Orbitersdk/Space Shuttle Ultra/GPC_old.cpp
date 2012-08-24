// GPC Code
#include "Atlantis.h"
#include "Atlantis_defs.h"
#include "ParameterValues.h"
#include "util/Stopwatch.h"
#include <OrbiterSoundSDK35.h>
#include <cstdio>
#include <UltraMath.h>

//extern int tpir(const double* list, int n_items, double target);
//extern double linterp(double x0, double y0, double x1, double y1, double x);
//extern double listerp(const vector<double> &listx, const vector<double> &listy, double x);
//extern double tableterp(const double* table, const double* listrow, int n_row, const double* listcol, int n_col, double rowlookup, double collookup);

void Atlantis::InitializeAutopilot()
{
	// if there is a mission file, it overrrides values in scn file
	if(pMission) {
		TgtInc=pMission->GetMECOInc()*DEG;
		TgtFPA=pMission->GetMECOFPA()*DEG;
		TgtAlt=pMission->GetMECOAlt();
		TgtSpd=pMission->GetMECOVel();

		//RollToHeadsUp=pMission->GetRTHUVelocity()*fps_to_ms;
		if(pMission->PerformRTHU()) RollToHeadsUp = 12200.0/MPS2FPS;
		else RollToHeadsUp = 100000.0; // velocity faster than orbital speed; RTHU will not occur
		if(pMission->UseOMSAssist()) {
			OMS_Assist[0] = pMission->GetOMSAssistStart();
			OMS_Assist[1] = pMission->GetOMSAssistEnd();
		}
		else OMS_Assist[0] = OMS_Assist[1] = 0.0;
	}

	OBJHANDLE hRef=GetSurfaceRef();
	//calculate heading
	double latitude, /*Radius,*/ longitude;
	GetEquPos(longitude, latitude, Radius);
	if(abs(cos(TgtInc*RAD))>abs(cos(latitude))) {
		THeading=PI/2;
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
		THeading=atan2(xVel, yVel); // northerly heading
		if(TgtInc > 65.0) THeading = PI - THeading; // if heading is negative, this is retrograde inclination; use southerly heading
	}

	mu=GGRAV*oapiGetMass(hRef);
	SidDay = oapiGetPlanetPeriod(hRef);
	//calculate apogee/perigee
	Radius = oapiGetSize(hRef);
	TgtRad = TgtAlt+Radius;
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
	GetRelativePos(hRef,rh0);
	rh0=rh0*(1/length(rh0));
	A=0.0;
	B=0.0;
	C=0.0;
	//D=0.0;
}

double Atlantis::CalculateAzimuth()
{
	double true_azimuth=0;
	double latitude, longitude;
	double azimuth,equator_v/*,target_radius*/;
	double tgt_orbit_v[2],lnch_v[2],current_vel[2]; //vectors
	VECTOR3 vel, horizonvel;
	double temp;

	// removed DEG to RAD conversion
	GetEquPos(longitude, latitude, temp);
	GetRelativeVel (GetSurfaceRef(), vel);
	MATRIX3 rot;
	GetRotationMatrix (rot);
	vel = tmul (rot, vel);
	HorizonRot(vel, horizonvel);
	current_vel[1]= horizonvel.data[0]; //East
	current_vel[0]= horizonvel.data[2];  // North
	sprintf_s(oapiDebugString(), 255, "current_vel: %f %f", current_vel[0], current_vel[1]);


	azimuth= asin(cos(TgtInc*RAD)/cos(latitude));  // this equ doesn't take rotation into accout
	equator_v=Radius*(2*PI/SidDay);   //equator velocity
	tgt_orbit_v[0]=TgtSpd*cos(TgtFPA*RAD)*cos(azimuth); // northern velocity
	tgt_orbit_v[1]=TgtSpd*cos(TgtFPA*RAD)*sin(azimuth); // eastern velocity
	lnch_v[0]= abs(tgt_orbit_v[0]) - abs(current_vel[0]); // taking current velocity into accout for CC (North); assume both values have same sign
	lnch_v[1]= abs(tgt_orbit_v[1]) - abs(current_vel[1]); // taking current velocity into accout for CC (East); assume both values have same sign

	//if (lnch_v[0]==0) lnch_v[0]=0.01; //div by zero protection	
	if(lnch_v[0]==0.0) { //div by zero protection
		if(lnch_v[1]>0) true_azimuth=PI/2;
		else true_azimuth=-PI/2;
	}
	else true_azimuth = atan(lnch_v[1]/lnch_v[0]); // tan(azimuth) = eastern_vel / northern_vel
	if(current_vel[0] < 0.0) true_azimuth = PI - true_azimuth; // we are heading south, so need to use southerly heading
	if(current_vel[1] < 0.0) true_azimuth = 2*PI - true_azimuth; // retrograde inclination

	/*VECTOR3 pos;
	GetRelativeVel(GetSurfaceRef(), vel);
	GetRelativePos(GetSurfaceRef(), pos);
	oapiGetPlanetObliquityMatrix(GetSurfaceRef(), &rot);
	vel=mul(rot, vel);
	pos=mul(rot, vel);
	temp=vel.y;
	vel.y=vel.z;
	vel.z=temp;
	temp=pos.y;
	pos.y=pos.z;
	pos.z=temp;

	VECTOR3 h=crossp(pos, vel);
	VECTOR3 n=_V(-h.y, h.x, 0);
	double LAN=acos(n.x/length(n));
	sprintf_s(oapiDebugString(), 255, "LAN: %f", LAN*DEG);*/

	return true_azimuth;
}

void Atlantis::MajorCycle()
{
	Navigate();
	THeading=CalculateAzimuth();
	Estimate();
	Guide();
}

void Atlantis::Navigate() {
	GetRelativePos(GetSurfaceRef(),rv);
	GetRelativeVel(GetSurfaceRef(),vv);
	r=length(rv);
	v=length(vv);
	hv=crossp(rv,vv);
	h=length(hv);
	rh=rv*(1/r);
	hh=hv*(1/h);
	thetah=crossp(hh,rh);
	vr=dotp(vv,rh);
	vtheta=dotp(vv,thetah);
	omega=vtheta/r;
	//w=x/(r*r);
	last_target_pitch=target_pitch;
	last_target_yaw=target_yaw;
	last_target_roll=target_roll;
	oapiGetFocusPitch(&pitch);

	F=CalcNetThrust();
	m=GetMass();
	Isp=GetThrusterIsp(th_main[0]);
	a0=F/m;
	tau=Isp/a0;
	theta=acos(dotp(rh0,rh));
	phi=acos(dotp(rh,rmh));
	VECTOR3 rhxrmh=crossp(rh,rmh);
	if(rhxrmh.y>0)phi=2*PI-phi;
}

void Atlantis::Estimate() {
	rbar=(r+TgtRad)/2;
	double num=mu/(rbar*rbar)-omega*omega*r;
	fr=A+num/a0;
	fdotr=B+(num/a(T)-fr)/T;
	double fh=0; //No yaw guidance yet
	double fdoth=0;
	ftheta=1-fr*fr/2-fh*fh/2;  //Small number approximation to hypot
	fdottheta=-(fr*fdotr+fh*fdoth);
	fdotdottheta=-(fdotr*fdotr+fdoth*fdoth)/2;

	ELEMENTS currentEle;
	double MJD;
	GetElements(currentEle,MJD);
	eCurrent=currentEle.e;



	//Estimate true anomaly at cutoff
	d3=h*vr/(r*r*r);
	d4=(h*(TgtSpd*sin(TgtFPA*RAD))/(TgtRad*TgtRad*TgtRad)-d3)/T;
	deltatheta=(h/(r*r))*T+(ftheta*c0(T)+fdottheta*cn(T,1)+fdotdottheta*cn(T,2))/rbar-d3*T*T-d4*T*T*T/3.0;
	thetaT=theta+deltatheta;
	p=(TPe/(1-TEcc))*(1-TEcc*TEcc);
	//Estimate time of cutoff
	hT=sqrt(mu*p);
	deltah=hT-h;
	deltav=deltah/rbar;
	T=tau*(1-exp(-deltav/Isp));
	if(T>1000)T=1000;
	//Any comparison with NaN is false, so
	//invert these tests so it's true if NaN
	if(!(tau>T)|!(0<T))T=tau-10;
	//  if(!(tau>T)|!(0<T))T=T2;
	metCutoff=T+met;
}

void Atlantis::Guide() {
	//Calculate pitch steering coefficients
	double a=b0(T);
	double b=bn(T,1);
	double c=c0(T);
	double d=cn(T,1);
	double y1=TgtSpd*sin(TgtFPA*RAD)-vr;
	double y2=TgtRad-vr*T-r;
	double D=a*d-b*c;
	A=(d*y1-b*y2)/D;
	B=(a*y2-c*y1)/D;
	TLastAB=met;

	//VECTOR3 ang_vel;
	C=(mu/(r*r)-(omega*omega*r))/a0;
	target_roll=0;
	target_yaw=0;
	fhdotrh=A+B*(met-TLastAB)+C;
	if(fabs(fhdotrh)>1) {
		//Commanded to do a cos(pitch) out of range, turn
		//off pitch guidance so pilot has a chance to do something
		//"Handing over manual control... Good Luck! :)"
		//   --Eddie, flight computer of the Heart of Gold
		//bAutopilot=false;
	} 
	else {
		target_pitch=(PI/2-acos(fhdotrh))*DEG;
	}

	//target_pitch = target_pitch+ThrAngleP; //yaw angle ignored for now
	CmdPDot=(target_pitch-ThrAngleP*cos(GetBank())-GetPitch()*DEG)/(2*TMajorCycle);
	//CmdPDot=(target_pitch-ThrAngleP*cos(GetBank())-GetPitch()*DEG)/TMajorCycle;
}

void Atlantis::RateCommand()
{
	VECTOR3 oldRates=ReqdRates;

	double Heading/*, Slip*/;
	if(GetPitch()*DEG>=88.0) 
	{
		VECTOR3 wingBody=_V(1,0,0);
		VECTOR3 wingHorizon;
		HorizonRot(wingBody,wingHorizon);
		Heading=atan2(wingHorizon.x,wingHorizon.z)+PI/2;
		if(Heading >= 2 * PI)
			Heading -= 2 * PI;
	}
	else 
	{
		oapiGetHeading(GetHandle(), &Heading);
	}
	//Heading*=DEG;
	//Slip=GetSlipAngle()*DEG;

	if(status==1) {
		double TargetPitch=listerp(stage1guidance[0], stage1guidance[1], GetAirspeed());
		if(bAutopilot) {
			if(GetAirspeed()<32.00) {
				ReqdRates.data[PITCH] = 0.0;
				ReqdRates.data[YAW] = 0.0;
				ReqdRates.data[ROLL] = 0.0;
			}
			else if(GetPitch()*DEG>=88.5) {
				ReqdRates.data[PITCH] = range(-10.0, 10.0*(GetPitch()*DEG-TargetPitch), 10.0);
				ReqdRates.data[YAW]=0.0;
				if((Heading-THeading)>RAD) ReqdRates.data[ROLL]=8.0;
				else if((Heading-THeading)<-RAD) ReqdRates.data[ROLL]=-8.0;
				else ReqdRates.data[ROLL]=0.0;
				ReqdRates=RotateVectorZ(ReqdRates, (THeading-Heading)*DEG);
			}
			else {
				ReqdRates.data[PITCH] = TargetPitch-GetPitch()*DEG;
				if(ReqdRates.data[PITCH]>2.5) 
					ReqdRates.data[PITCH]=2.5;
				else if(ReqdRates.data[PITCH]<-2.5) 
					ReqdRates.data[PITCH]=-2.5;

				if(GetPitch()*DEG>60.0) {
					ReqdRates.data[YAW] = range(-8.0, 0.25*DEG*(Heading-THeading), 8.0);
				}
				else {
					ReqdRates.data[YAW] = 0.0;
				}
				if(GetBank()>0.0) ReqdRates.data[ROLL]=0.5*(GetBank()*DEG-180.0);
				else ReqdRates.data[ROLL]=0.5*(GetBank()*DEG+180.0);
				ReqdRates=RotateVectorZ(ReqdRates, -GetBank()*DEG);
				ReqdRates.data[ROLL] = range(-12.0, ReqdRates.data[ROLL], 12.0);
			}
		}
		else {
			ReqdRates.data[PITCH] = 5.0*(GetManualControlLevel(THGROUP_ATT_PITCHUP, MANCTRL_ANYMODE, MANCTRL_ANYDEVICE) - GetManualControlLevel(THGROUP_ATT_PITCHDOWN, MANCTRL_ANYMODE, MANCTRL_ANYDEVICE));
			ReqdRates.data[YAW] = 2.5*(GetManualControlLevel(THGROUP_ATT_YAWLEFT, MANCTRL_ANYMODE, MANCTRL_ANYDEVICE) - GetManualControlLevel(THGROUP_ATT_YAWRIGHT, MANCTRL_ANYMODE, MANCTRL_ANYDEVICE));
			ReqdRates.data[ROLL] = -10.0*(GetManualControlLevel(THGROUP_ATT_BANKLEFT, MANCTRL_ANYMODE, MANCTRL_ANYDEVICE) - GetManualControlLevel(THGROUP_ATT_BANKRIGHT, MANCTRL_ANYMODE, MANCTRL_ANYDEVICE));
		}
	}
	else if(status==2) {
		if(bAutopilot) {
			if(met>(TLastMajorCycle+TMajorCycle)) 
			{
				MajorCycle();
				TLastMajorCycle=met;
			}
			else Navigate();



			if(met<=(tSRBSep+4.0)) {
				ReqdRates.data[PITCH]=0.0;
				ReqdRates.data[YAW]=0.0;
				ReqdRates.data[ROLL]=0.0;
			}
			else {
				if(T>TPEGStop) 
				{
					ReqdRates.data[PITCH] = CmdPDot;
					ReqdRates.data[YAW] = range(-2.5, 0.5*DEG*(Heading-THeading), 2.5);
					/*if(abs(GetBank()*DEG)>90.0) 
					ReqdRates.data[YAW] = range(-2.5, -2.5*DEG*(THeading-Heading), 2.5);
					//ReqdRates.data[YAW] = range(-2.5, -2.5*(GetSlipAngle()*DEG), 2.5);
					else ReqdRates.data[YAW] = range(-2.5, -2.5*DEG*(THeading-Heading), 2.5);*/

					if(v<RollToHeadsUp) 
					{
						if(GetBank()>0) ReqdRates.data[ROLL] = 2.5*(GetBank()*DEG-180.0);
						else ReqdRates.data[ROLL] = 2.5*(GetBank()*DEG+180.0);
						ReqdRates.data[ROLL]=range(-5.0, ReqdRates.data[ROLL], 5.0);
					}
					else {
						ReqdRates.data[ROLL] = 2.5*(GetBank()*DEG);
						ReqdRates.data[ROLL]=range(-5.0, ReqdRates.data[ROLL], 5.0);
						if(abs(GetBank()*DEG)>2.5) 
						{ //roll in progress
							VECTOR3 vel;
							GetHorizonAirspeedVector(vel);
							double cyaw=-DEG*atan(vel.x/vel.z)+DEG*Heading;
							double tpitch=target_pitch-ThrAngleP*cos(GetBank());
							ReqdRates.data[PITCH]=tpitch-GetPitch()*DEG;
							//ReqdRates.data[YAW]=ReqdRates.data[ROLL]*cos(GetBank())*tan(ThrAngleP*RAD);
							ReqdRates.data[YAW]=-ThrAngleP*sin(GetBank())+cyaw;
							//sprintf(oapiDebugString(), "%f %f %f %f", ReqdRates.data[PITCH], ReqdRates.data[YAW], ReqdRates.data[ROLL], cyaw);
						}
					}
					//ReqdRates.data[ROLL]=range(-5.0, ReqdRates.data[ROLL], 5.0);
					ReqdRates=RotateVectorZ(ReqdRates, -GetBank()*DEG);
					//sprintf(oapiDebugString(), "%f %f %f %f", ReqdRates.data[PITCH], ReqdRates.data[YAW], ReqdRates.data[ROLL], cyaw);
				}
				else {
					ReqdRates.data[PITCH] = 0.0;
					ReqdRates.data[YAW] = 0.0;
					ReqdRates.data[ROLL] = 0.0;
				}
			}
		}
		else {
			ReqdRates.data[PITCH] = 5.0*(GetManualControlLevel(THGROUP_ATT_PITCHUP, MANCTRL_ANYMODE, MANCTRL_ANYDEVICE) - GetManualControlLevel(THGROUP_ATT_PITCHDOWN, MANCTRL_ANYMODE, MANCTRL_ANYDEVICE));
			ReqdRates.data[YAW] = 2.5*(GetManualControlLevel(THGROUP_ATT_YAWLEFT, MANCTRL_ANYMODE, MANCTRL_ANYDEVICE) - GetManualControlLevel(THGROUP_ATT_YAWRIGHT, MANCTRL_ANYMODE, MANCTRL_ANYDEVICE));
			ReqdRates.data[ROLL] = -10.0*(GetManualControlLevel(THGROUP_ATT_BANKLEFT, MANCTRL_ANYMODE, MANCTRL_ANYDEVICE) - GetManualControlLevel(THGROUP_ATT_BANKRIGHT, MANCTRL_ANYMODE, MANCTRL_ANYDEVICE));
		}
	}
	// make sure rates change gradually, to avoid PID bugs
	for(int i=0;i<3;i++) ReqdRates.data[i]=range(oldRates.data[i]-1, ReqdRates.data[i], oldRates.data[i]+1);
}

void Atlantis::Throttle(double dt)
{
	switch(status) {
		case 1:
			if(!bThrottle) return;
			if(GetAirspeed()<18.288) 
				SetSSMEThrustLevel(0, 100.0/109.0);
			else if(GetAirspeed()>=Throttle_Bucket[0] && GetAirspeed()<=Throttle_Bucket[1]) {
				//if(GetThrusterGroupLevel(THGROUP_MAIN) > (72.0/109.0)) IncThrusterGroupLevel(THGROUP_MAIN, -0.005);
				//else SetSSMEThrustLevel(0, 72.0/109.0);
				for(unsigned short i=1;i<=3;i++) {
					double thrustLevel = GetSSMEThrustLevel(i);
					if(thrustLevel > (72.0/109.0)) SetSSMEThrustLevel(i, thrustLevel - 0.1*dt);
					else SetSSMEThrustLevel(i, 72.0/109.0);
				}
			}
			else {
				//if(GetThrusterGroupLevel(THGROUP_MAIN) < (MaxThrust/109.0)) IncThrusterGroupLevel(THGROUP_MAIN, 0.005);
				//else SetSSMEThrustLevel(0, MaxThrust/109.0);
				for(unsigned short i=1;i<=3;i++) {
					double thrustLevel = GetSSMEThrustLevel(i);
					if(thrustLevel < (MaxThrust/109.0)) SetSSMEThrustLevel(i, thrustLevel + 0.1*dt);
					else SetSSMEThrustLevel(i, MaxThrust/109.0);
				}
			}
			break;
		case 2:
			//OMS Assist
			if(met>OMS_Assist[0] && met<OMS_Assist[1] && !bMECO)
			{
				SetThrusterLevel(th_oms[0], 1.00);
				SetThrusterLevel(th_oms[1], 1.00);
			}
			else
			{
				SetThrusterLevel(th_oms[0], 0.00);
				SetThrusterLevel(th_oms[1], 0.00);
			}
			if(!bThrottle) return;
			if(a0>=29.00) { //28.42
				for(int i=0;i<3;i++) {
					if(GetThrusterLevel(th_main[i]) > (67.0/109.0))
						IncThrusterLevel(th_main[i], -0.01);
					else SetThrusterLevel(th_main[i], 67.0/109.0);
				}
			}
			if(bAutopilot) {
				if(v>=TgtSpd) {
					//reached target speed
					for(int i=0;i<3;i++) {
						SetThrusterLevel(th_main[i], 0.0);
					}
					bAutopilot=false; //launch autopilot now finished
				}
			}
			break;
	}
	return;
}

void Atlantis::GPC(double simt, double dt)
{
	switch(GetGPCMajorMode()) {
		case 101:
			//if(GetThrusterGroupLevel(THGROUP_MAIN)>0.865) Throttle(dt);
			if(status>=STATE_STAGE1) Throttle(dt);
			break;
		case 102:
			Throttle(dt);
			RateCommand();
			AutoMainGimbal(dt);
			break;
		case 103:
			if(!bMECO && status==2) {
				RateCommand();
				SteerGimbal(dt);
				Throttle(dt);
			}
			else { //post MECO
				if(status==2 && !ETSepTranslationInProg && tMECO+ET_SEP_TIME<=met)
				{
					SeparateTank();
					bManualSeparate = false;
					//TransPulseInProg[2]=true;
					ETSepTranslationInProg = true;
					//TransPulseDV.z=-ET_SEP_RATE;
				}
				else if(status==3) {
					if(ETSepMinusZDV <= 0.001) { //Z thrusting complete
						//WT=GetMass()*kg_to_pounds;
						ZTransCommand.ResetLine();
						ETSepTranslationInProg = false;
						SetGPCMajorMode(104);
					}
					else { // -Z thrusting in progress
						ZTransCommand.SetLine(-1.0f);
						// calculate DV so far
						VECTOR3 ThrustVector;
						GetThrustVector(ThrustVector);
						ETSepMinusZDV -= (ThrustVector.y/GetMass())*dt;
					}
				}
				//AttControl(dt);
				//TransControl(simt, dt);
			}
			break;
		case 104:
		case 105:
		case 202:
		case 302:
			//if(!BurnCompleted && MNVRLOAD && MnvrExecute) Maneuver(dt);
			/*AttControl(dt);
			TransControl(simt, dt);
			break;*/
		case 106:
		case 201:
		case 301:
		case 303:
			//AttControl(dt);
			//TransControl(simt, dt);
			break;
		case 304:
			if(GetMachNumber() < 2.5) SetGPCMajorMode(305);
			break;
		case 305:
			//AerojetDAP(dt);
			break;
	}

//	double time_for_GPC = st.Stop();
//
//	if(time_for_GPC > 50000.0)
//	{
//		char buffer[100];
//		sprintf(buffer, "GPC Stopwatch: GPC function needed more than 50 ms for completition (t_GPC = %f µs)\n", time_for_GPC);
//		oapiWriteLog(buffer);
//	}
	return;
}
