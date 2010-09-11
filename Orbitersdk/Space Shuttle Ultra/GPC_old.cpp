// GPC Code
#include "Atlantis.h"
#include "ParameterValues.h"
#include "util/Stopwatch.h"
#include <OrbiterSoundSDK35.h>
#include <cstdio>
#include "SSUMath.h"

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

		RollToHeadsUp=pMission->GetRTHUVelocity()*fps_to_ms;
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
	if(cos(TgtInc*RAD)>cos(latitude)) {
		THeading=PI/2;
		TgtInc = latitude*DEG;
	}
	else {
		double InHeading = asin(cos(TgtInc*RAD)/cos(latitude));
		double xVel, yVel;
		xVel = TgtSpd*cos(TgtFPA*RAD)*sin(InHeading)-464.581*cos(latitude);
		yVel = TgtSpd*cos(TgtFPA*RAD)*cos(InHeading);
		THeading=atan2(xVel, yVel);
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
	tgt_orbit_v[0]=sqrt(mu/TgtRad)*cos(azimuth); // northern velocity
	tgt_orbit_v[1]=sqrt(mu/TgtRad)*sin(azimuth); // eastern velocity
	lnch_v[0]= tgt_orbit_v[0] - (current_vel[0]); // taking current velocity into accout for CC (North)
	lnch_v[1]= tgt_orbit_v[1] - (current_vel[1]); // taking current velocity into accout for CC (East)

	//if (lnch_v[0]==0) lnch_v[0]=0.01; //div by zero protection	
	if(lnch_v[0]==0.0) { //div by zero protection
		if(lnch_v[1]>0) true_azimuth=PI/2;
		else true_azimuth=-PI/2;
	}
	else true_azimuth = atan(lnch_v[1]/lnch_v[0]); // tan(azimuth) = eastern_vel / northern_vel

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
				SetThrusterGroupLevel(THGROUP_MAIN, 100.0/109.0);
			else if(GetAirspeed()>=Throttle_Bucket[0] && GetAirspeed()<=Throttle_Bucket[1]) {
				if(GetThrusterGroupLevel(THGROUP_MAIN) > (72.0/109.0)) IncThrusterGroupLevel(THGROUP_MAIN, -0.005);
				else SetThrusterGroupLevel(THGROUP_MAIN, 72.0/109.0);
			}
			else {
				if(GetThrusterGroupLevel(THGROUP_MAIN) < (MaxThrust/109.0)) IncThrusterGroupLevel(THGROUP_MAIN, 0.005);
				else SetThrusterGroupLevel(THGROUP_MAIN, MaxThrust/109.0);
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
					if(GetThrusterLevel(th_main[i])> (67.0/109.0))
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
//	Stopwatch st;
//	st.Start();

	/*switch(ops) {
		case 101:
			if(GetThrusterGroupLevel(THGROUP_MAIN)>0.865) Throttle(dt);
			break;
		case 102:
			Throttle(dt);
			RateCommand();
			AutoMainGimbal();
			break;
		case 103:
			if(!bMECO && status==2) {
				RateCommand();
				SteerGimbal();
				Throttle(dt);
			}
			else { //post MECO
				if(status==2 && !TransPulseInProg[2] && tMECO+ET_SEP_TIME<=met)
				{
					SeparateTank();
					bManualSeparate = false;
					TransPulseInProg[2]=true;
					TransPulseDV.z=-ET_SEP_RATE;
					//dV=0.0;
					//SetThrusterGroupLevel(THGROUP_ATT_UP, 1.00);
					//SetThrusterGroupLevel(thg_transup, 1.00);
				}
				else if(status==3 && Eq(TransPulseDV.z, 0.0, 0.001)) { //Z thrusting complete
					ops=104;
					/*GetThrustVector(Thrust);
					dV+=(Thrust.y/GetMass())*dt;
					if(dV>=ET_SEP_RATE) {
						//SetThrusterGroupLevel(THGROUP_ATT_UP, 0.00);
						SetThrusterGroupLevel(thg_transup, 0.00);
						bZThrust=false;
						ops=104;
					}*
				}
				AttControl(dt);
				TransControl(simt, dt);
			}
			break;
		case 104:
			AttControl(dt);
			TransControl(simt, dt);
			if(!BurnCompleted && MNVRLOAD) Maneuver(dt);
			break;
		case 105:
			AttControl(dt);
			TransControl(simt, dt);
			if(!BurnCompleted && MNVRLOAD) Maneuver(dt);
			break;
		case 106:
			AttControl(dt);
			TransControl(simt, dt);
			break;
		case 201:
			AttControl(dt);
			TransControl(simt, dt);
			break;
		case 202:
			AttControl(dt);
			TransControl(simt, dt);
			if(!BurnCompleted && MNVRLOAD) Maneuver(dt);
			break;
		case 301:
			AttControl(dt);
			TransControl(simt, dt);
			break;
		case 302:
			AttControl(dt);
			TransControl(simt, dt);
			if(!BurnCompleted && MNVRLOAD) Maneuver(dt);
			break;
		case 303:
			AttControl(dt);
			TransControl(simt, dt);
			break;
		case 304:
			AerojetDAP(dt);
			break;
	}*/

	switch(ops) {
		case 101:
			if(GetThrusterGroupLevel(THGROUP_MAIN)>0.865) Throttle(dt);
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
				if(status==2 && !TransPulseInProg[2] && tMECO+ET_SEP_TIME<=met)
				{
					SeparateTank();
					bManualSeparate = false;
					TransPulseInProg[2]=true;
					TransPulseDV.z=-ET_SEP_RATE;
				}
				else if(status==3 && Eq(TransPulseDV.z, 0.0, 0.001)) { //Z thrusting complete
					WT=GetMass()*kg_to_pounds;
					ops=104;
				}
				AttControl(dt);
				TransControl(simt, dt);
			}
			break;
		case 104:
		case 105:
		case 202:
		case 302:
			if(!BurnCompleted && MNVRLOAD && MnvrExecute) Maneuver(dt);
			/*AttControl(dt);
			TransControl(simt, dt);
			break;*/
		case 106:
		case 201:
		case 301:
		case 303:
			AttControl(dt);
			TransControl(simt, dt);
			break;
		case 304:
			AerojetDAP(dt);
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

void Atlantis::AerojetDAP(double SimdT)
{
	//for the moment, use RHC input to control thruster firings
	if(PitchActive) {
		if(RHCInput.data[PITCH]>0.01) {
			SetThrusterGroupLevel(thg_pitchup, 1.0);
			SetThrusterGroupLevel(thg_pitchdown, 0.0);
		}
		else if(RHCInput.data[PITCH]<-0.01) {
			SetThrusterGroupLevel(thg_pitchdown, 1.0);
			SetThrusterGroupLevel(thg_pitchup, 0.0);
		}
		else {
			SetThrusterGroupLevel(thg_pitchup, 0.0);
			SetThrusterGroupLevel(thg_pitchdown, 0.0);
		}
	}
	else {
		SetThrusterGroupLevel(thg_pitchup, 0.0);
		SetThrusterGroupLevel(thg_pitchdown, 0.0);
	}

	if(YawActive) {
		if(RHCInput.data[YAW]>0.01) {
			SetThrusterGroupLevel(thg_yawright, 1.0);
			SetThrusterGroupLevel(thg_yawleft, 0.0);
		}
		else if(RHCInput.data[YAW]<-0.01) {
			SetThrusterGroupLevel(thg_yawleft, 1.0);
			SetThrusterGroupLevel(thg_yawright, 0.0);
		}
		else {
			SetThrusterGroupLevel(thg_yawright, 0.0);
			SetThrusterGroupLevel(thg_yawleft, 0.0);
		}
	}
	else {
		SetThrusterGroupLevel(thg_yawright, 0.0);
		SetThrusterGroupLevel(thg_yawleft, 0.0);
	}

	if(RollActive) {
		if(RHCInput.data[ROLL]>0.01) {
			SetThrusterGroupLevel(thg_rollright, 1.0);
			SetThrusterGroupLevel(thg_rollleft, 0.0);
		}
		else if(RHCInput.data[ROLL]<-0.01) {
			SetThrusterGroupLevel(thg_rollleft, 1.0);
			SetThrusterGroupLevel(thg_rollright, 0.0);
		}
		else {
			SetThrusterGroupLevel(thg_rollright, 0.0);
			SetThrusterGroupLevel(thg_rollleft, 0.0);
		}
	}
	else {
		SetThrusterGroupLevel(thg_rollright, 0.0);
		SetThrusterGroupLevel(thg_rollleft, 0.0);
	}
}

void Atlantis::Maneuver(double dt)
{
	sprintf_s(oapiDebugString(), 255, "Maneuver %f %f %f %f", tig, met, BurnTime, IgnitionTime);
	if(BurnInProg)
	{
		if(met>=(IgnitionTime+BurnTime)) {
			//sprintf(oapiDebugString(), "Shutdown");
			SetThrusterGroupLevel(thg_main, 0.00);
			BurnCompleted=true;
			BurnInProg=false;
		}
		else {
			VECTOR3 ThrustVector;
			GetThrustVector(ThrustVector); //inefficient
			//only shows errors caused by burn timing
			VGO.x-=((ThrustVector.z/GetMass())*dt)/fps_to_ms;
			VGO.y-=((ThrustVector.x/GetMass())*dt)/fps_to_ms;
			VGO.z+=((ThrustVector.y/GetMass())*dt)/fps_to_ms;
		}
	}
	else if(met>=tig)
	{
		if(OMS<3) {
			//sprintf(oapiDebugString(), "Burning");
			BurnInProg=true;
			IgnitionTime=met;
			//ignite engines
			if(OMS==0) SetThrusterGroupLevel(thg_main, 1.00);
			else SetThrusterLevel(th_oms[OMS-1], 1.00);
		}
	}
	//else sprintf(oapiDebugString(), "Maneuver %f %f %f %f", tig, met, BurnTime, IgnitionTime);
}

void Atlantis::OMSTVC(const VECTOR3 &Rates, double SimDT)
{
	VECTOR3 CurrentRates=AngularVelocity*DEG;
	double pitchDelta=Rates.data[PITCH]-CurrentRates.data[PITCH]; //if positive, vessel is pitching down
	double yawDelta=Rates.data[YAW]-CurrentRates.data[YAW]; //if positive, vessel is rotating to right
	double rollDelta=Rates.data[ROLL]-CurrentRates.data[ROLL]; //if positive, vessel is rolling to left
	bool RCSWraparound=(abs(rollDelta)>0.5 || abs(pitchDelta)>0.25 || abs(yawDelta)>0.25);

	double dPitch=OMSTVCControlP.Step(pitchDelta, SimDT);
	double dYaw=OMSTVCControlY.Step(yawDelta, SimDT);

	if(OMS!=2) //left OMS engine burning
	{
		//double dPitch=2.5*pitchDelta, dYaw=3.0*yawDelta; //changes in gimbal position from default (trim) angle
		double Pitch=Trim.data[0]+dPitch, Yaw=Trim.data[1]+dYaw;
		if(OMS==0) Pitch-=rollDelta;
		if(!GimbalOMS(0, Pitch, Yaw)) RCSWraparound=true;
	}
	if(OMS!=1) //right OMS engine burning
	{
		//double dPitch=2.5*pitchDelta, dYaw=3.0*yawDelta; //changes in gimbal position from default (trim) angle
		double Pitch=Trim.data[0]+dPitch, Yaw=Trim.data[2]+dYaw;
		if(OMS==0) Pitch+=rollDelta;
		if(!GimbalOMS(1, Pitch, Yaw)) RCSWraparound=true;
	}
	sprintf_s(oapiDebugString(), 255, "OMS TVC: %f %f %f %f dPitch: %f", OMSGimbal[0][0], OMSGimbal[0][1], OMSGimbal[1][0], OMSGimbal[1][1], pitchDelta);

	if(RCSWraparound) SetRates(Rates);
	else if(OMS!=0) SetRates(_V(0.0, 0.0, Rates.data[ROLL])); //for single-engine burns, use RCS for roll control
}

/*void Atlantis::GimbalOMS(const VECTOR3 &Targets)
{
	VECTOR3 Dir;
	OMSGimbal[0][0]=OMSGimbal[1][0]=-(ORBITER_OMS_PITCH+Targets.data[0]);
	OMSGimbal[0][1]=ORBITER_OMS_YAW+Targets.data[1];
	OMSGimbal[1][1]=-ORBITER_OMS_YAW+Targets.data[2];
	
	// L OMS Engine
	Dir.z=sqrt(1-pow(sin(OMSGimbal[0][0]*RAD), 2)-pow(sin(OMSGimbal[0][1]*RAD), 2));
	Dir.y=sin(OMSGimbal[0][0]*RAD);
	Dir.x=sin(OMSGimbal[0][1]*RAD);
	SetThrusterDir(th_oms[0], Dir);
	// R OMS Engine
	Dir.z=sqrt(1-pow(sin(OMSGimbal[1][0]*RAD), 2)-pow(sin(OMSGimbal[1][1]*RAD), 2));
	Dir.y=sin(OMSGimbal[1][0]*RAD);
	Dir.x=sin(OMSGimbal[1][1]*RAD);
	SetThrusterDir(th_oms[1], Dir);

	OMSGimbal[0][0]=OMSGimbal[1][0]=Targets.data[0];
	OMSGimbal[0][1]=Targets.data[1];
	OMSGimbal[1][1]=Targets.data[2];
}*/

void Atlantis::LoadManeuver()
{
	int i;
	double StartWeight, EndWeight, EndWeightLast=0.0, FuelRate, ThrustFactor=1.0;
	//VECTOR3 ThrustVector;
	bool bDone=false;
	MNVRLOAD=true;
	tig=TIG[0]*86400+TIG[1]*3600+TIG[2]*60+TIG[3];
	
	//dV
	for(i=0;i<3;i++) {
		DeltaV.data[i]=PEG7.data[i]*fps_to_ms;
	}
	DeltaVTot=length(PEG7);
	double DeltaVTotms=length(DeltaV);
	//GimbalOMS(Trim);
	GimbalOMS(0, Trim.data[0], Trim.data[1]);
	GimbalOMS(1, Trim.data[0], Trim.data[2]);

	OMSTVCControlP.Reset();
	OMSTVCControlY.Reset();

	VECTOR3 ThrustDir; //direction of net thrust (in Orbiter frame)
	if(OMS==0) {
		VECTOR3 temp1, temp2;
		GetThrusterDir(th_oms[0], temp1);
		GetThrusterDir(th_oms[1], temp2);
		ThrustDir=temp1+temp2;
		ThrustFactor=length(ThrustDir); //get thrust produced by engines
		ThrustDir=ThrustDir/ThrustFactor; //normalize vector
	}
	else if(OMS==1 || OMS==2) {
		GetThrusterDir(th_oms[OMS-1], ThrustDir);
	}
	else {
		ThrustDir=_V(0.0, 0.0, 1.0); //+X RCS
	}
	ThrustDir=RotateVectorZ(ThrustDir, TV_ROLL);

	sprintf_s(oapiDebugString(), 255, "Thrust Dir: %f %f %f %f", ThrustDir.x, ThrustDir.y, ThrustDir.z, TV_ROLL);
	BurnAtt.data[PITCH]=-asin(ThrustDir.y)*DEG;
	BurnAtt.data[YAW]=-asin(ThrustDir.x)*DEG; //check signs here
	// compensate for roll
	/*if(TV_ROLL!=0.0) {
		double dTemp=BurnAtt.data[PITCH];
		BurnAtt.data[PITCH]-=BurnAtt.data[PITCH]*(1.0-cos(TV_ROLL*RAD))+BurnAtt.data[YAW]*(1.0-sin(TV_ROLL*RAD));
		BurnAtt.data[YAW]+=BurnAtt.data[YAW]*(1.0-sin(TV_ROLL*RAD))-dTemp*(1.0-abs(cos(TV_ROLL*RAD)));
	}*/
	
	//Burn Attitude
	if(DeltaV.x!=0.0 || DeltaV.z!=0.0) {
		if(DeltaV.z<=0) BurnAtt.data[PITCH]+=DEG*acos(DeltaV.x/sqrt((pow(DeltaV.x, 2)+pow(DeltaV.z, 2))));
		else BurnAtt.data[PITCH]-=DEG*acos(DeltaV.x/sqrt((pow(DeltaV.x, 2)+pow(DeltaV.z, 2))));
	}
	if(DeltaV.x!=0.0 || DeltaV.y!=0.0) BurnAtt.data[YAW]+=DEG*asin(DeltaV.y/sqrt((pow(DeltaV.x, 2)+pow(DeltaV.y, 2))));
	BurnAtt.data[ROLL]=TV_ROLL;
	/*if(TV_ROLL!=0.0) {
		double dTemp=BurnAtt.data[PITCH];
		BurnAtt.data[PITCH]-=BurnAtt.data[PITCH]*(1.0-cos(TV_ROLL*RAD))+BurnAtt.data[YAW]*(1.0-sin(TV_ROLL*RAD));
		BurnAtt.data[YAW]+=BurnAtt.data[YAW]*(1.0-sin(TV_ROLL*RAD))-dTemp*(1.0-cos(TV_ROLL*RAD));
	}*/

	//use rocket equation (TODO: Check math/formulas here)
	StartWeight=WT/kg_to_pounds;
	EndWeight=StartWeight/exp(DeltaVTotms/GetThrusterIsp(th_oms[0]));
	FuelRate=ORBITER_OMS_THRUST/(GetPropellantEfficiency(ph_oms)*GetThrusterIsp(th_oms[0]));
	FuelRate*=ThrustFactor; //if two-engine burn, compensate for # of engines; TODO: Make sure this is valid; we're calculating fuel, not thrust (SiameseCat)
	BurnTime=(StartWeight-EndWeight)/FuelRate;
	//TGO[0]=BurnTime/60;
	//TGO[1]=BurnTime-(TGO[0]*60);
	VGO.x=DeltaVTot*ThrustDir.z;
	VGO.y=DeltaVTot*ThrustDir.x;
	VGO.z=-DeltaVTot*ThrustDir.y;
}

void Atlantis::UpdateDAP()
{
	TranPls=DAP[DAPMode[0]].PRI_TRAN_PLS*fps_to_ms; //same for all modes
	if(DAPMode[1]==0) {
		RotRate=DAP[DAPMode[0]].PRI_ROT_RATE;
		AttDeadband=DAP[DAPMode[0]].PRI_ATT_DB;
		RateDeadband=DAP[DAPMode[0]].PRI_RATE_DB;
		RotPls=DAP[DAPMode[0]].PRI_ROT_PLS;
		if(DAP[DAPMode[0]].PRI_P_OPTION!=0)
			Torque.data[PITCH]=0.5*ORBITER_PITCH_TORQUE;
		else Torque.data[PITCH]=ORBITER_PITCH_TORQUE;
		if(DAP[DAPMode[0]].PRI_Y_OPTION!=0)
			Torque.data[YAW]=0.5*ORBITER_YAW_TORQUE;
		else Torque.data[YAW]=ORBITER_YAW_TORQUE;
		Torque.data[ROLL]=ORBITER_ROLL_TORQUE;
	}
	else if(DAPMode[1]==1) {
		RotRate=DAP[DAPMode[0]].PRI_ROT_RATE;
		AttDeadband=DAP[DAPMode[0]].PRI_ATT_DB;
		RateDeadband=DAP[DAPMode[0]].ALT_RATE_DB;
		RotPls=DAP[DAPMode[0]].PRI_ROT_PLS;
		if(DAP[DAPMode[0]].ALT_JET_OPT==2) {
			Torque.data[PITCH]=0.5*ORBITER_PITCH_TORQUE;
			Torque.data[YAW]=0.5*ORBITER_YAW_TORQUE;
			Torque.data[ROLL]=0.5*ORBITER_ROLL_TORQUE;
		}
		else {
			Torque.data[PITCH]=ORBITER_PITCH_TORQUE;
			Torque.data[YAW]=ORBITER_YAW_TORQUE;
			Torque.data[ROLL]=ORBITER_ROLL_TORQUE;
		}
	}
	else if(DAPMode[1]==2) {
		RotRate=DAP[DAPMode[0]].VERN_ROT_RATE;
		AttDeadband=DAP[DAPMode[0]].VERN_ATT_DB;
		RateDeadband=DAP[DAPMode[0]].VERN_RATE_DB;
		RotPls=DAP[DAPMode[0]].VERN_ROT_PLS;
		Torque.data[PITCH]=0.1*ORBITER_PITCH_TORQUE;
		Torque.data[YAW]=0.1*ORBITER_YAW_TORQUE;
		Torque.data[ROLL]=0.1*ORBITER_ROLL_TORQUE;
	}
}

void Atlantis::StartAttManeuver()
{
	int i;
	if(CurManeuver.Type!=AttManeuver::OFF) {
		for(i=0;i<4;i++) START_TIME[i]=CurManeuver.START_TIME[i];
		if(CurManeuver.Type==AttManeuver::MNVR) {
			TargetAttM50=REQD_ATT=CurManeuver.TargetAttM50;
			for(i=0;i<3;i++) {
				if(TargetAttM50.data[i]>PI) TargetAttM50.data[i]-=2*PI;
			}
			REQD_ATT=REQD_ATT*DEG;
			TargetAttOrbiter=ConvertAnglesBetweenM50AndOrbiter(TargetAttM50, true);
		}
		else if(CurManeuver.Type==AttManeuver::TRK) {
			LVLHTgtOrientationMatrix=CurManeuver.LVLHTgtOrientationMatrix;
			LVLHOrientationReqd=GetAnglesFromMatrix(LVLHTgtOrientationMatrix)*DEG;
		}
		/*Yaw=false;
		Pitch=false;
		Roll=false;*/
		for(i=0;i<3;i++) RotatingAxis[i] = false;
		ManeuverinProg=false;
	}
}

void Atlantis::TransControl(double SimT, double SimdT)
{
	VECTOR3 ThrusterLevel=_V(0.0, 0.0, 0.0);

	for(int i=0;i<3;i++) {
		if(abs(THCInput.data[i])>0.01) {
			//if PCT is in progress, disable it when THC is moved out of detent
			if(PostContactThrusting[1]) TogglePCT();

			if(TransMode[i]==0) { //NORM
				if(THCInput.data[i]>0.0) ThrusterLevel.data[i]=1.0;
				else ThrusterLevel.data[i]=-1.0;
			}
			else if(TransMode[i]==1 && !TransPulseInProg[i]) { //PULSE
				TransPulseInProg[i]=true;
				if(THCInput.data[i]>0.0) TransPulseDV.data[i]=TranPls;
				else TransPulseDV.data[i]=-TranPls;
			}
		}

		if(TransPulseInProg[i]) {
			if(!Eq(TransPulseDV.data[i], 0.0, 0.001)) {
				if(TransPulseDV.data[i]>0.0) ThrusterLevel.data[i]=1.0;
				else ThrusterLevel.data[i]=-1.0;
			}
			//if THC is in detent and pulse is complete, allow further pulses
			else if(abs(THCInput.data[i])<0.01) TransPulseInProg[i]=false;
		}
	}

	if(PostContactThrusting[1]) {
		PCTControl(SimT);
		return; // thruster levels already set
	}

	//play RCS sounds
	if(!IsPlaying3(SoundID, RCS_SOUND)) {
		for(int i=0;i<3;i++) {
			if(!Eq(abs(ThrusterLevel.data[i]), 0.00, 0.001) && Eq(abs(TranslationCommand.data[i]), 0.0, 0.001)) {
				PlayVesselWave3(SoundID, RCS_SOUND, NOLOOP);
				break;
			}
		}
	}
	//store thruster commands
	TranslationCommand=ThrusterLevel;

	//fire appropriate sets of thrusters
	if(ThrusterLevel.x>0.05) {
		SetThrusterGroupLevel(thg_transfwd, 1.0);
		SetThrusterGroupLevel(thg_transaft, 0.0);
		if(TransPulseInProg[0]) {
			double acc=(TransForce[0].x/GetMass())*SimdT;
			TransPulseDV.x-=acc;
			if(TransPulseDV.x<=(acc/2.0)) { //minimize error
				TransPulseDV.x=0.000;
			}
		}
	}
	else if(ThrusterLevel.x<-0.05) {
		SetThrusterGroupLevel(thg_transaft, 1.0);
		SetThrusterGroupLevel(thg_transfwd, 0.0);
		if(TransPulseInProg[0]) {
			double acc=(TransForce[1].x/GetMass())*SimdT;
			TransPulseDV.x+=acc;
			if(TransPulseDV.x>0.0 || -TransPulseDV.x<=(acc/2.0)) { //minimize error
				TransPulseDV.x=0.000;
			}
		}
	}
	else {
		SetThrusterGroupLevel(thg_transfwd, 0.0);
		SetThrusterGroupLevel(thg_transaft, 0.0);
	}
	if(ThrusterLevel.y>0.05) {
		SetThrusterGroupLevel(thg_transright, 1.0);
		SetThrusterGroupLevel(thg_transleft, 0.0);
		if(TransPulseInProg[1]) {
			double acc=(TransForce[0].y/GetMass())*SimdT;
			TransPulseDV.y-=acc;
			if(TransPulseDV.y<=(acc/2.0)) { //minimize error
				TransPulseDV.y=0.000;
			}
		}
	}
	else if(ThrusterLevel.y<-0.05) {
		SetThrusterGroupLevel(thg_transleft, 1.0);
		SetThrusterGroupLevel(thg_transright, 0.0);
		if(TransPulseInProg[1]) {
			double acc=(TransForce[1].y/GetMass())*SimdT;
			TransPulseDV.y+=acc;
			if(TransPulseDV.y>0.0 || -TransPulseDV.y<=(acc/2.0)) { //minimize error
				TransPulseDV.y=0.000;
			}
		}
	}
	else {
		SetThrusterGroupLevel(thg_transleft, 0.0);
		SetThrusterGroupLevel(thg_transright, 0.0);
	}
	if(ThrusterLevel.z>0.05) {
		SetThrusterGroupLevel(thg_transdown, 1.0);
		SetThrusterGroupLevel(thg_transup, 0.0);
		if(TransPulseInProg[2]) {
			double acc=(TransForce[0].z/GetMass())*SimdT;
			TransPulseDV.z-=acc;
			if(TransPulseDV.z<=(acc/2.0)) { //minimize error
				TransPulseDV.z=0.000;
			}
		}
	}
	else if(ThrusterLevel.z<-0.05) {
		SetThrusterGroupLevel(thg_transup, 1.0);
		SetThrusterGroupLevel(thg_transdown, 0.0);
		if(TransPulseInProg[2]) {
			double acc=(TransForce[1].z/GetMass())*SimdT;
			TransPulseDV.z+=acc;
			if(TransPulseDV.z>0.0 || -TransPulseDV.z<=(acc/2.0)) { //minimize error
				TransPulseDV.z=0.000;
			}
		}
	}
	else {
		SetThrusterGroupLevel(thg_transup, 0.0);
		SetThrusterGroupLevel(thg_transdown, 0.0);
	}
}

void Atlantis::PCTControl(double SimT)
{
	double dT=SimT-PCTStartTime;

	//prevent translation thrusters except (up group) from firing
	SetThrusterGroupLevel(thg_transdown, 0.0);
	SetThrusterGroupLevel(thg_transfwd, 0.0);
	SetThrusterGroupLevel(thg_transaft, 0.0);
	SetThrusterGroupLevel(thg_transleft, 0.0);
	SetThrusterGroupLevel(thg_transright, 0.0);

	//fire thrusters as appropriate
	if(dT<=PCT_STAGE1) SetThrusterGroupLevel(thg_transup, 0.5);
	else if(dT<=PCT_STAGE2) SetThrusterGroupLevel(thg_transup, 0.0);
	else if(dT<=PCT_STAGE3) SetThrusterGroupLevel(thg_transup, 0.5);
	else {
		SetThrusterGroupLevel(thg_transup, 0.0);
		TogglePCT();
	}
}

void Atlantis::UpdateAttControlVariables()
{
	GetAngularVel(AngularVelocity);
	GetGlobalOrientation(InertialOrientationRad);
	CurrentAttitude=ConvertAnglesBetweenM50AndOrbiter(InertialOrientationRad);
	GetGlobalPos(GVesselPos);	
	Mass=GetMass();
	GetPMI(PMI);
}

void Atlantis::AttControl(double SimdT)
{
	//int i;
	UpdateAttControlVariables();

	for(int i=0;i<3;i++) {
		if(!RotPulseInProg[i]) ReqdRates.data[i]=AngularVelocity.data[i]*DEG;
	}

	//if(!Eq(RHCInput.x, 0.0, 0.01) || !Eq(RHCInput.y, 0.0, 0.01) || !Eq(RHCInput.z, 0.0, 0.01)) {
	if(!Eq(RHCInput, _V(0.0, 0.0, 0.0), 0.01)) {
		//if PCT is in progress, disable it when RHC is moved out of detent
		if(PostContactThrusting[1]) TogglePCT();

		//TRK=ROT=false;
		//MNVR=true;
		if(ControlMode==AUTO) {
			ControlMode=INRTL;
			//panelc3->UpdateVC(); //update PBI lights to reflect change in mode
		}
		if(ControlMode==INRTL) {
			TargetAttOrbiter=InertialOrientationRad;
			TargetAttM50=CurrentAttitude;
			REQD_ATT=CurrentAttitude*DEG;
		}
		else if(ControlMode==LVLH) {
			//GetStatus(Status);
			LVLHOrientationReqd=CalcLVLHAttitude()*DEG;
			ReqdAttMatrix=ConvertLVLHAnglesToM50Matrix(LVLHOrientationReqd*RAD);
		}
		ManeuverinProg=true;
		ManeuverStatus=MNVR_COMPLETE; //(check value set here)

		RHCInput.data[YAW]=-RHCInput.data[YAW]; //difference in Orbiter and STS frames
		for(int i=0;i<3;i++) {
			if(RHCInput.data[i]>0.01) {
				if(RHCInput.data[i]>0.75) ReqdRates.data[i]=1000;
				else {
					if(RotMode[i]==0) ReqdRates.data[i]=RotRate; //DISC RATE
					else if(!RotPulseInProg[i]) {
						ReqdRates.data[i]=ReqdRates.data[i]+RotPls; //PULSE
						RotPulseInProg[i]=true;
					}
				}
			}
			else if(RHCInput.data[i]<-0.01) {
				if(RHCInput.data[i]<-0.75) ReqdRates.data[i]=-1000;
				else {
					if(RotMode[i]==0) ReqdRates.data[i]=-RotRate; //DISC RATE
					else if(!RotPulseInProg[i]) {
						ReqdRates.data[i]=ReqdRates.data[i]-RotPls; //PULSE
						RotPulseInProg[i]=true;
					}
				}
			}
			else ReqdRates.data[i]=0.0;
		}
	}
	else if(ControlMode!=FREE) {
		MATRIX3 LastReqdAttMatrix;
		VECTOR3 NullRates, NullRatesLocal;
		if((ControlMode==AUTO && TRK) || ControlMode==LVLH) {
			//GetStatus(Status);
			//LastReqdAtt=REQD_ATT;
			LastReqdAttMatrix=ReqdAttMatrix;
			ReqdAttMatrix=ConvertLVLHAnglesToM50Matrix(LVLHOrientationReqd*RAD);
			REQD_ATT=GetAnglesFromMatrix(ReqdAttMatrix)*DEG;
			//sprintf_s(oapiDebugString(), 255, "LVLH REQD: %f %f %f", LVLHOrientationReqd.x, LVLHOrientationReqd.y, LVLHOrientationReqd.z);
		}
		
		if(!ManeuverinProg) {
			ManeuverinProg=true;
			for(int i=0;i<4;i++) {
				if(MET[i]<START_TIME[i]) {
					ManeuverinProg=false;
					break;
				}
			}
			if(ManeuverinProg) {
				sprintf(oapiDebugString(), "Starting maneuver %f", oapiRand());
				ManeuverStatus=MNVR_STARTING;
				sprintf_s(oapiDebugString(), 255, "MNVR STARTING");
			}
			return; // wait until next timestep so we can calculate null rates
		}
		//if(!ManeuverinProg) return; //no need for further calculations

		if(ControlMode==LVLH || ((TRK || ROT) && ControlMode==AUTO)) {
			//VECTOR3 TargAttOrbiter=ConvertAnglesBetweenM50AndOrbiter(REQD_ATT*RAD, true);
			MATRIX3 TargAttMatrix=ConvertMatrixBetweenM50AndOrbiter(ReqdAttMatrix, true);
			VECTOR3 TargAttOrbiter=GetAnglesFromMatrix(TargAttMatrix);
			//VECTOR3 LastTargAttOrbiter=ConvertAnglesBetweenM50AndOrbiter(LastReqdAtt*RAD, true);
			MATRIX3 LastTargAttMatrix=ConvertMatrixBetweenM50AndOrbiter(LastReqdAttMatrix, true);
			VECTOR3 LastTargAttOrbiter=GetAnglesFromMatrix(LastTargAttMatrix);

			NullRatesLocal=(ConvertOrbiterAnglesToLocal(TargAttOrbiter)-ConvertOrbiterAnglesToLocal(LastTargAttOrbiter))/SimdT;
			double NullRatesMag=abs(NullRatesLocal.x)+abs(NullRatesLocal.y)+abs(NullRatesLocal.z);
			if(NullRatesMag>10.0) {
				//error; rates should not be this high
				return;
			}

			if(ManeuverStatus==MNVR_STARTING || (ManeuverStatus==MNVR_IN_PROGRESS && TimeSinceTgtUpdate>=60.0)) {
				// when maneuver starts (and at 60-sec intervals) calculate inertial attitude at mnvr completion
				if(ManeuverStatus==MNVR_STARTING) MNVR_TIME=0.0;
				NullRates=(TargAttOrbiter-LastTargAttOrbiter)/SimdT;
				CalcManeuverTargets(NullRates);
				ManeuverStatus=MNVR_IN_PROGRESS;
				TimeSinceTgtUpdate=0.0;
				sprintf_s(oapiDebugString(), 255, "MNVR IN PROGRESS");
			}
			else if(ManeuverStatus==MNVR_COMPLETE) {
				TargetAttOrbiter=TargAttOrbiter;
				MNVR_TIME=0.0;
				TimeSinceTgtUpdate=0.0;
			}
			else {
				// mnvr in progress; update timers
				MNVR_TIME-=SimdT;
				TimeSinceTgtUpdate+=SimdT;
			}
		}
		else NullRates=NullRatesLocal=_V(0, 0, 0); //MNVR or ControlMode==INRTL
		
		//PitchYawRoll=CalcPitchYawRollAngles()*DEG;
		PitchYawRollMatrix=CalcPitchYawRollRotMatrix();
		PitchYawRoll=GetAnglesFromMatrix(PitchYawRollMatrix)*DEG;
		//if(ControlMode==INRTL) sprintf_s(oapiDebugString(), 255, "RatesN: %f %f %f", ReqdRates.x, ReqdRates.y, ReqdRates.z);
		if(ManeuverStatus==MNVR_COMPLETE) CalcRequiredRates(ReqdRates, NullRatesLocal*DEG);
		else CalcRequiredRates(ReqdRates);
	}
	if(!BurnInProg) SetRates(ReqdRates);
	else OMSTVC(ReqdRates, SimdT); //OMS burn is going on, so use OMS TVC for control
}

void Atlantis::CalcManeuverTargets(VECTOR3 NullRates) //calculates TargetAttitude and time to reach attitude
{
	int counter=0;
	double LastMnvrTime;
	double Angle;
	MATRIX3 PYR;
	VECTOR3 Axis;
	VECTOR3 REQDATTOrbiter=ConvertAnglesBetweenM50AndOrbiter(REQD_ATT*RAD, true);
	/*TargetAttOrbiter=REQDATTOrbiter;
	//PYR=CalcPitchYawRollAngles()*DEG;
	PYR=CalcPitchYawRollRotMatrix();
	//MNVR_TIME=RotRate*(abs(PYR.x)+abs(PYR.y)+abs(PYR.z));
	Angle=CalcEulerAngle(IdentityMatrix, PYR, Axis);
	MNVR_TIME=R(Angle*DEG)/RotRate;
	sprintf_s(oapiDebugString(), 255, "Initial MNVR TIME: %f", MNVR_TIME);*/
	//oapiWriteLog(oapiDebugString());
	do {
		counter++;
		LastMnvrTime=MNVR_TIME;
		if(MNVR_TIME<0) {
			MNVR_TIME=0.0;
			//break;
		}
		TargetAttOrbiter=REQDATTOrbiter+NullRates*MNVR_TIME; //check TargetAtt frame
		PYR=CalcPitchYawRollRotMatrix();
		Angle=CalcEulerAngle(IdentityMatrix, PYR, Axis);
		MNVR_TIME=(Angle*DEG)/RotRate;

		//sprintf(oapiDebugString(), "Iterating: %d %f", counter, MNVR_TIME);
		sprintf(oapiDebugString(), "Iterating: %d %f %f %f", counter, MNVR_TIME, NullRates.data[YAW]*DEG, TargetAttOrbiter.data[ROLL]*DEG);
		//oapiWriteLog(oapiDebugString());
	}while(abs(MNVR_TIME-LastMnvrTime)>0.05 && counter<50);
	if(counter>=50 && abs(MNVR_TIME-LastMnvrTime)>1.0) {
		//TargetAttOrbiter=REQDATTOrbiter;
		//VECTOR3 Axis;
		//double Angle=CalcEulerAngle(_V(0, 0, 0), PYR*RAD, Axis);
		//sprintf_s(oapiDebugString(), 255, "Axis: %f %f %f Angle: %f", Axis.x, Axis.y, Axis.z, Angle);
		sprintf_s(oapiDebugString(), 255, "ERROR: Targets could not be calculated: %f %f", MNVR_TIME, LastMnvrTime);
		oapiWriteLog(oapiDebugString());
	}
	else {
		TargetAttOrbiter=REQDATTOrbiter+NullRates*MNVR_TIME;
		RotationAxis=Axis;
		RotationAngle=Angle;

		sprintf_s(oapiDebugString(), 255, "Target Attitude: %f %f %f NR: %f %f %f Time: %f", TargetAttOrbiter.data[PITCH]*DEG,
			TargetAttOrbiter.data[YAW]*DEG, TargetAttOrbiter.data[ROLL]*DEG, NullRates.data[PITCH]*DEG,
			NullRates.data[YAW]*DEG, NullRates.data[ROLL]*DEG, MNVR_TIME);
		oapiWriteLog(oapiDebugString());
		sprintf_s(oapiDebugString(), 255, "Axis: %f %f %f Angle: %f", Axis.x, Axis.y, Axis.z, Angle*DEG);
		oapiWriteLog(oapiDebugString());
	}
}

void Atlantis::SetRates(const VECTOR3 &Rates)
{
	const VECTOR3 PRI_LIMITS = _V(0.01, 0.01, 0.01);
	const VECTOR3 VERN_LIMITS = _V(0.0015, 0.0015, 0.0015);
	//double dDiff;
	VECTOR3 ThrusterLevel;
	VECTOR3 CurrentRates;
	CurrentRates=AngularVelocity*DEG;
	VECTOR3 Error = Rates-CurrentRates;

	/*VECTOR3 PriLimits, VernLimits;
	PriLimits=_V(0.05, 0.05, 0.05);
	VernLimits=_V(0.0015, 0.0015, 0.0015);*/
	double dTimeAcc=oapiGetTimeAcceleration();

	VECTOR3 Limits;
	double MaxThrusterLevel;
	if(DAPMode[1]!=2) { // PRI/ALT
		Limits = PRI_LIMITS;
		MaxThrusterLevel = 1.0;
	}
	else { // VERN
		Limits = VERN_LIMITS;
		MaxThrusterLevel = 0.1;
	}
	if(ManeuverStatus==MNVR_IN_PROGRESS) Limits=Limits*5.0;

	if(abs(Error.data[PITCH])>Limits.data[PITCH]) {
		if(Error.data[PITCH]>0) {
			ThrusterLevel.data[PITCH] = MaxThrusterLevel;
			SetThrusterGroupLevel(thg_pitchup, MaxThrusterLevel/dTimeAcc);
			SetThrusterGroupLevel(thg_pitchdown, 0.0);
		}
		else {
			ThrusterLevel.data[PITCH] = -MaxThrusterLevel;
			SetThrusterGroupLevel(thg_pitchdown, MaxThrusterLevel/dTimeAcc);
			SetThrusterGroupLevel(thg_pitchup, 0.0);
		}
	}
	else {
		ThrusterLevel.data[PITCH] = 0.0;
		SetThrusterGroupLevel(thg_pitchup, 0.0);
		SetThrusterGroupLevel(thg_pitchdown, 0.0);
		//If RHC is out of detent, pretend pulse is still in progress
		if(abs(RHCInput.data[PITCH])<0.01) RotPulseInProg[PITCH]=false;
	}

	if(abs(Error.data[YAW])>Limits.data[YAW]) {
		if(Error.data[YAW]>0) {
			ThrusterLevel.data[YAW] = MaxThrusterLevel;
			SetThrusterGroupLevel(thg_yawleft, MaxThrusterLevel/dTimeAcc);
			SetThrusterGroupLevel(thg_yawright, 0.0);
		}
		else {
			ThrusterLevel.data[YAW] = -MaxThrusterLevel;
			SetThrusterGroupLevel(thg_yawright, MaxThrusterLevel/dTimeAcc);
			SetThrusterGroupLevel(thg_yawleft, 0.0);
		}
	}
	else {
		ThrusterLevel.data[YAW] = 0.0;
		SetThrusterGroupLevel(thg_yawright, 0.0);
		SetThrusterGroupLevel(thg_yawleft, 0.0);
		if(abs(RHCInput.data[YAW])<0.01) RotPulseInProg[YAW]=false;
	}

	if(abs(Error.data[ROLL])>Limits.data[ROLL]) {
		if(Error.data[ROLL]>0) {
			ThrusterLevel.data[ROLL] = MaxThrusterLevel;
			SetThrusterGroupLevel(thg_rollright, MaxThrusterLevel/dTimeAcc);
			SetThrusterGroupLevel(thg_rollleft, 0.0);
		}
		else {
			ThrusterLevel.data[ROLL] = -MaxThrusterLevel;
			SetThrusterGroupLevel(thg_rollleft, MaxThrusterLevel/dTimeAcc);
			SetThrusterGroupLevel(thg_rollright, 0.0);
		}
	}
	else {
		ThrusterLevel.data[ROLL] = 0.0;
		SetThrusterGroupLevel(thg_rollright, 0.0);
		SetThrusterGroupLevel(thg_rollleft, 0.0);
		if(abs(RHCInput.data[ROLL])<0.01) RotPulseInProg[ROLL]=false;
	}

	//play RCS sounds
	if(!IsPlaying3(SoundID, RCS_SOUND)) {
		for(int i=0;i<3;i++) {
			if(!Eq(abs(ThrusterLevel.data[i]), 0.00, 0.001) && Eq(abs(RotationCommand.data[i]), 0.0, 0.001)) {
				PlayVesselWave3(SoundID, RCS_SOUND, NOLOOP);
				break;
			}
		}
	}
	//store thruster commands
	RotationCommand=ThrusterLevel;
}

void Atlantis::CalcRequiredRates(VECTOR3 &Rates)
{
	//convert rotation around RotationAxis to rates around x,y,z axes
	//Rates.data[PITCH]=0.0;
	//Rates.data[YAW]=0.0;
	//Rates.data[ROLL]=0.0;

	//RotationAngle=CalcEulerAngle(_V(0, 0, 0), PitchYawRoll*RAD, RotationAxis);
	RotationAngle=CalcEulerAngle(IdentityMatrix, PitchYawRollMatrix, RotationAxis);
	//Rates=RotationAxis*-RotRate;
	for(int i=0;i<3;i++) {
		if(ControlMode==AUTO || RotMode[i]==0) Rates.data[i]=RotationAxis.data[i]*-RotRate;

		if(abs(PitchYawRoll.data[i]) <= NullStartAngle(abs(AngularVelocity.data[i]), static_cast<AXIS>(i))) {
			if(ControlMode==AUTO || RotMode[i]==0) Rates.data[i] = 0.0;
			RotatingAxis[i] = false;
		}
		else RotatingAxis[i] = true;
	}

	/*if(abs(PitchYawRoll.data[PITCH])<=NullStartAngle(abs(AngularVelocity.data[PITCH]), PITCH)) {
		if(ControlMode==AUTO || RotMode[PITCH]==0) Rates.data[PITCH]=0.0;
		RotatingAxis[PITCH]=false;
	}
	else RotatingAxis[PITCH]=true;
	if(abs(PitchYawRoll.data[YAW])<=NullStartAngle(abs(AngularVelocity.data[YAW]), YAW)) {
		if(ControlMode==AUTO || RotMode[YAW]==0) Rates.data[YAW]=0.0;
		RotatingAxis[YAW]=false;
	}
	else RotatingAxis[YAW]=true;
	if(abs(PitchYawRoll.data[ROLL])<=NullStartAngle(abs(AngularVelocity.data[ROLL]), ROLL)) {
		if(ControlMode==AUTO || RotMode[ROLL]==0) Rates.data[ROLL]=0.0;
		RotatingAxis[ROLL]=false;
	}
	else RotatingAxis[ROLL]=true;*/

	if(!RotatingAxis[PITCH] && !RotatingAxis[YAW] && !RotatingAxis[ROLL]) {
		ManeuverStatus=MNVR_COMPLETE; //now maintaining targ. attitude
		//sprintf_s(oapiDebugString(), 255, "MNVR COMPLETE");
	}
	//if(ControlMode==INRTL) sprintf_s(oapiDebugString(), 255, "RATES1: %f %f %f %i", Rates.data[ROLL], Rates.data[PITCH], Rates.data[YAW], DAPMode[1]);
}

void Atlantis::CalcRequiredRates(VECTOR3 &Rates, const VECTOR3 &NullRates) //vectors in degrees
{
	if(ControlMode!=FREE) {
		for(int i=2;i>=0;i--) {
			if(ControlMode==AUTO || RotMode[i]==0) {
				Rates.data[i]=0.0;
				if((RotatingAxis[i] || abs(PitchYawRoll.data[i])>AttDeadband)) {
					if(abs(PitchYawRoll.data[i])<0.05) RotatingAxis[i]=false;
					else {
						RotatingAxis[i]=true;
						if(abs(PitchYawRoll.data[i]) <= NullStartAngle(abs(AngularVelocity.data[i]), static_cast<AXIS>(i))) {
							Rates.data[i] = 0.0;
						}
						else {
							if(PitchYawRoll.data[i] > 0) {
								if(ManeuverStatus==MNVR_COMPLETE) Rates.data[i] = -range(RotRate/10.0, PitchYawRoll.data[i]/5.0, RotRate);
								else Rates.data[i] = -RotRate;
							}
							else {
								if(ManeuverStatus==MNVR_COMPLETE) Rates.data[i] = range(RotRate/10.0, -PitchYawRoll.data[i]/5.0, RotRate);
								else Rates.data[i] = RotRate;
							}
						}
					}
				}
			}
		}
		/*if(ControlMode==AUTO || RotMode[ROLL]==0) {
			if(/*!Yaw && !Pitch &&* (RotatingAxis[ROLL] || abs(PitchYawRoll.data[ROLL])>AttDeadband)) {
				if(abs(PitchYawRoll.data[ROLL])<0.05) RotatingAxis[ROLL]=false;
				else {
					RotatingAxis[ROLL]=true;
				}
				if(RotatingAxis[ROLL]) {
					//sprintf(oapiDebugString(), "%f %f %f", abs(PitchYawRoll.data[ROLL]), 
					//	NullStartAngle(abs(AngularVelocity.data[ROLL]), ORBITER_ROLL_TORQUE, ROLL), PMI.data[ROLL]);
					if(abs(PitchYawRoll.data[ROLL])<=NullStartAngle(abs(AngularVelocity.data[ROLL]), ROLL)) {
						Rates.data[ROLL]=0.0;
						//nRoll=1;
						//Roll=false;
					}
					else {
						if(PitchYawRoll.data[ROLL]>0) {
							//if(ManeuverStatus==MNVR_COMPLETE) Rates.data[ROLL]=max(-RotRate, -PitchYawRoll.data[ROLL]/5.0);
							if(ManeuverStatus==MNVR_COMPLETE) Rates.data[ROLL]=-range(RotRate/10.0, PitchYawRoll.data[ROLL]/5.0, RotRate);
							else Rates.data[ROLL]=-RotRate;
						}
						else {
							//if(ManeuverStatus==MNVR_COMPLETE) Rates.data[ROLL]=min(RotRate, -PitchYawRoll.data[ROLL]/5.0);
							if(ManeuverStatus==MNVR_COMPLETE) Rates.data[ROLL]=range(RotRate/10.0, -PitchYawRoll.data[ROLL]/5.0, RotRate);
							else Rates.data[ROLL]=RotRate;
						}
					}
				}
				else Rates.data[ROLL]=0.0;
			}
			else Rates.data[ROLL]=0.0;
		}

		if(ControlMode==AUTO || RotMode[YAW]==0) {
			if(/*!Roll && !Pitch &&* (RotatingAxis[YAW] || abs(PitchYawRoll.data[YAW])>AttDeadband)) {
				if(abs(PitchYawRoll.data[YAW])<0.05) RotatingAxis[YAW]=false;
				else {
					RotatingAxis[YAW]=true;
				}
				if(RotatingAxis[YAW]) {
					if(abs(PitchYawRoll.data[YAW])<=NullStartAngle(abs(AngularVelocity.data[YAW]), YAW)) {
						Rates.data[YAW]=0.0;
						//Pitch=false;
					}
					else {
						if(PitchYawRoll.data[YAW]>0) {
							//if(ManeuverStatus==MNVR_COMPLETE) Rates.data[YAW]=max(-RotRate, -PitchYawRoll.data[YAW]/5.0);
							if(ManeuverStatus==MNVR_COMPLETE) Rates.data[YAW]=-range(RotRate/10.0, PitchYawRoll.data[YAW]/5.0, RotRate);
							else Rates.data[YAW]=-RotRate;
						}
						else {
							//if(ManeuverStatus==MNVR_COMPLETE) Rates.data[YAW]=min(RotRate, -PitchYawRoll.data[YAW]/5.0);
							if(ManeuverStatus==MNVR_COMPLETE) Rates.data[YAW]=range(RotRate/10.0, -PitchYawRoll.data[YAW]/5.0, RotRate);
							else Rates.data[YAW]=RotRate;
						}
					}
				}
				else Rates.data[YAW]=0.0;
				//sprintf(oapiDebugString(), "AttControl");
				//sprintf(oapiDebugString(), "%f", Rates.data[YAW]);
				//sprintf(oapiDebugString(), "roll %f", Torque.data[YAW]);
			}
			else Rates.data[YAW]=0.0;
		}

		if(ControlMode==AUTO || RotMode[PITCH]==0) {
			if(/*!Roll && !Yaw &&* (RotatingAxis[PITCH] || abs(PitchYawRoll.data[PITCH])>AttDeadband)) {
				if(abs(PitchYawRoll.data[PITCH])<0.05) {
					RotatingAxis[PITCH]=false;
					//ManeuverComplete=true;
					sprintf(oapiDebugString(), "Maneuver completed");
				}
				else {
					RotatingAxis[PITCH]=true;
				}
				if(RotatingAxis[PITCH]) {
					if(abs(PitchYawRoll.data[PITCH])<=NullStartAngle(abs(AngularVelocity.data[PITCH]), PITCH)) {
						//sprintf(oapiDebugString(), "%f %f", abs(PitchYawRoll.data[PITCH]), NullStartAngle(abs(AngularVelocity.data[PITCH]), ORBITER_PITCH_TORQUE, PITCH));
						Rates.data[PITCH]=0.00000;
						//Pitch=false;
					}
					else {
						if(PitchYawRoll.data[PITCH]>0) {
							//if(ManeuverStatus==MNVR_COMPLETE) Rates.data[PITCH]=max(-RotRate, -PitchYawRoll.data[PITCH]/5.0);
							if(ManeuverStatus==MNVR_COMPLETE) Rates.data[PITCH]=-range(RotRate/10.0, PitchYawRoll.data[PITCH]/5.0, RotRate);
							else Rates.data[PITCH]=-RotRate;
						}
						else {
							//if(ManeuverStatus==MNVR_COMPLETE) Rates.data[PITCH]=min(RotRate, -PitchYawRoll.data[PITCH]/5.0);
							if(ManeuverStatus==MNVR_COMPLETE) Rates.data[PITCH]=range(RotRate/10.0, -PitchYawRoll.data[PITCH]/5.0, RotRate);
							else Rates.data[PITCH]=RotRate;
						}
					}
				}
				else Rates.data[PITCH]=0.0000000;
				//sprintf(oapiDebugString(), "AttControl");
				//sprintf(oapiDebugString(), "%f", Rates.data[PITCH]);
				//sprintf(oapiDebugString(), "roll %f", Torque.data[PITCH]);
			}
			else Rates.data[PITCH]=0.0000;
		}*/

		if(!RotatingAxis[PITCH] && !RotatingAxis[YAW] && !RotatingAxis[ROLL]) {
			ManeuverStatus=MNVR_COMPLETE; //now maintaining targ. attitude
			//sprintf_s(oapiDebugString(), 255, "MNVR COMPLETE");
		}

		if(oapiGetTimeAcceleration()>10.0)
		{
			Rates=Rates/(2.0*oapiGetTimeAcceleration()/10.0);
		}
		
		if(ManeuverStatus==MNVR_COMPLETE) {
			//Rates=Rates+NullRates;
			for(int i=0;i<3;i++) {
				if(ControlMode==AUTO || RotMode[i]==0) Rates.data[i]+=NullRates.data[i];
			}
		}
	}
}
