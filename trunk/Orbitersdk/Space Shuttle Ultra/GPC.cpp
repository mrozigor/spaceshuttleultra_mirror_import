// GPC Code
#include "Atlantis.h"
#include "PanelC3.h"

#include "util/Stopwatch.h"
#include <cstdio>

extern int tpir(const double* list, int n_items, double target);
extern double linterp(double x0, double y0, double x1, double y1, double x);
extern double listerp(const double* listx, const double* listy, int n_items, double x);
extern double tableterp(const double* table, const double* listrow, int n_row, const double* listcol, int n_col, double rowlookup, double collookup);

void Atlantis::InitializeAutopilot()
{
	//calculate heading
	double latitude, /*Radius,*/ longitude;
	GetEquPos(longitude, latitude, Radius);
	if(cos(TgtInc*RAD)>cos(latitude)) THeading=PI/2;
	else {
		double InHeading = asin(cos(TgtInc*RAD)/cos(latitude));
		double xVel, yVel;
		xVel = TgtSpd*cos(TgtFPA*RAD)*sin(InHeading)-464.581*cos(latitude);
		yVel = TgtSpd*cos(TgtFPA*RAD)*cos(InHeading);
		THeading=atan2(xVel, yVel);
	}

	mu=GGRAV*oapiGetMass(GetGravityRef());
	SidDay = oapiGetPlanetPeriod(GetGravityRef());
	//calculate apogee/perigee
	Radius = oapiGetSize(GetGravityRef());
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
	GetRelativePos(GetGravityRef(),rh0);
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
	//the following lines may be incorrect
	//GetRelativePos (hRef, pos);
	GetRelativeVel (GetGravityRef(), vel);
	MATRIX3 rot;
	GetRotationMatrix (rot);
	//oapiGetRotationMatrix(GetGravityRef(), &rot);
	//pos = tmul (rot, pos);
	vel = tmul (rot, vel);
	HorizonRot(vel, horizonvel);
	//Crt2Pol (pos, vel); // translate pos. and vel. from cartesian to polar
	//m_pvVessel->GetEquPos(m_dLongitude,m_dLatitude,m_dRadius);
	current_vel[1]= horizonvel.data[0]; //East
	current_vel[0]= horizonvel.data[2];  // North
	sprintf_s(oapiDebugString(), 255, "current_vel: %f %f", current_vel[0], current_vel[1]);


	azimuth= asin(cos(TgtInc*RAD)/cos(latitude));  // this equ doesn't take rotation into accout
	equator_v=Radius*(2*PI/SidDay);   //equator velocity
	// removed DEG to RAD conversion
	//base_v[1]= equator_v * cos (m_dLatitude);   // base velocity vector item (east)
	//target_radius= Radius + m_dAlt;
	//mi=GGRAV*bodyPhys.mass;  //gravitational parameter
	tgt_orbit_v[0]=sqrt(mu/TgtRad)*cos(azimuth); // northern velocity
	tgt_orbit_v[1]=sqrt(mu/TgtRad)*sin(azimuth); // eastern velocity
	//for(int i=0; i<2; i++) lnch_v[i]=tgt_orbit_v[i]-abs(current_vel[i]); // taking launch site rotation into accout 
	//final_vel_norm= sqrt(tgt_orbit_v[0]*tgt_orbit_v[0] + tgt_orbit_v[1]*tgt_orbit_v[1]);
	lnch_v[0]= tgt_orbit_v[0] - fabs(current_vel[0]); // taking current velocity into accout for CC (North)
	lnch_v[1]= tgt_orbit_v[1] - (current_vel[1]); // taking current velocity into accout for CC (East)
	//lnch_v_norm=sqrt(lnch_v[0]*lnch_v[0] + lnch_v[1]*lnch_v[1]); // normalizing launch vector

	if (lnch_v[0]==0) lnch_v[0]=0.01; //div by zero protection	
	true_azimuth = atan(lnch_v[1]/lnch_v[0]); // tan(azimuth) = eastern_vel / northern_vel

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
	GetRelativePos(GetGravityRef(),rv);
	GetRelativeVel(GetGravityRef(),vv);
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
	double Heading/*, Slip*/;
	if(GetPitch()*DEG>=88.0) {
		VECTOR3 wingBody=_V(1,0,0);
		VECTOR3 wingHorizon;
		HorizonRot(wingBody,wingHorizon);
		Heading=atan2(wingHorizon.x,wingHorizon.z)+PI/2;
		while(Heading<0)Heading+=2*PI;
	}
	else oapiGetHeading(GetHandle(), &Heading);
	//Heading*=DEG;
	//Slip=GetSlipAngle()*DEG;

	if(status==1) {
		double TargetPitch=listerp(stage1guidance[0], stage1guidance[1], stage1guidance_size, GetAirspeed());
		if(bAutopilot) {
			if(GetAirspeed()<32.00) {
				ReqdRates.data[PITCH] = 0.0;
				ReqdRates.data[YAW] = 0.0;
				ReqdRates.data[ROLL] = 0.0;
			}
			else if(GetPitch()*DEG>=88.5) {
				ReqdRates.data[PITCH] = 10.0*(GetPitch()*DEG-TargetPitch);
				if(ReqdRates.data[PITCH]>10.0) ReqdRates.data[PITCH]=10.0;
				ReqdRates.data[YAW]=0.0;
				if((Heading-THeading)>RAD) ReqdRates.data[ROLL]=8.0;
				else if((Heading-THeading)<-RAD) ReqdRates.data[ROLL]=-8.0;
				else ReqdRates.data[ROLL]=0.0;
			}
			else {
				ReqdRates.data[PITCH] = TargetPitch-GetPitch()*DEG;
				if(ReqdRates.data[PITCH]>2.5) ReqdRates.data[PITCH]=2.5;
				else if(ReqdRates.data[PITCH]<-2.5) ReqdRates.data[PITCH]=-2.5;
				if(GetPitch()*DEG>50.0) {
					ReqdRates.data[YAW] = range(-8.0, 2.5*DEG*(Heading-THeading), 8.0);
				}
				else {
					ReqdRates.data[YAW] = 0.0;
				}
				if(GetBank()>0.0) ReqdRates.data[ROLL]=2.5*(GetBank()*DEG-180.0);
				else ReqdRates.data[ROLL]=2.5*(GetBank()*DEG+180.0);
				ReqdRates=RotateVectorZ(ReqdRates, -GetBank()*DEG);
				ReqdRates.data[ROLL] = range(-8.0, ReqdRates.data[ROLL], 8.0);
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
				if(T>TPEGStop) {
					ReqdRates.data[PITCH] = CmdPDot;
					ReqdRates.data[YAW] = range(-2.5, -2.5*DEG*(THeading-Heading), 2.5);
					/*if(abs(GetBank()*DEG)>90.0) 
						ReqdRates.data[YAW] = range(-2.5, -2.5*DEG*(THeading-Heading), 2.5);
						//ReqdRates.data[YAW] = range(-2.5, -2.5*(GetSlipAngle()*DEG), 2.5);
					else ReqdRates.data[YAW] = range(-2.5, -2.5*DEG*(THeading-Heading), 2.5);*/

					if(v<RollToHeadsUp) {
						if(GetBank()>0) ReqdRates.data[ROLL] = 2.5*(GetBank()*DEG-180.0);
						else ReqdRates.data[ROLL] = 2.5*(GetBank()*DEG+180.0);
						ReqdRates.data[ROLL]=range(-5.0, ReqdRates.data[ROLL], 5.0);
					}
					else {
						ReqdRates.data[ROLL] = 2.5*(GetBank()*DEG);
						ReqdRates.data[ROLL]=range(-5.0, ReqdRates.data[ROLL], 5.0);
						if(abs(GetBank()*DEG)>2.5) { //roll in progress
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
	return;
}

void Atlantis::Throttle(double dt)
{
	switch(status) {
		case 1:
			if(!bThrottle) return;
			if(GetAirspeed()<18.288) SetThrusterGroupLevel(THGROUP_MAIN, 100.0/MaxThrust);
			else if(GetAirspeed()>=Throttle_Bucket[0] && GetAirspeed()<=Throttle_Bucket[1]) {
				if(GetThrusterGroupLevel(THGROUP_MAIN)>0.692) IncThrusterGroupLevel(THGROUP_MAIN, -0.005);
				else SetThrusterGroupLevel(THGROUP_MAIN, 0.692);
			}
			else {
				if(GetThrusterGroupLevel(THGROUP_MAIN)<0.99) IncThrusterGroupLevel(THGROUP_MAIN, 0.005);
				else SetThrusterGroupLevel(THGROUP_MAIN, 1.00);
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
					if(GetThrusterLevel(th_main[i])>67.0/MaxThrust)
						IncThrusterLevel(th_main[i], -0.01);
					else SetThrusterLevel(th_main[i], 67.0/MaxThrust);
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

void Atlantis::GPC(double dt)
{
//	Stopwatch st;
//	st.Start();

	switch(ops) {
		case 101:
			if(GetThrusterGroupLevel(THGROUP_MAIN)>0.865) Throttle(dt);
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
						SetThrusterGroupLevel(THGROUP_ATT_UP, 0.00);
						bZThrust=false;
						ops=104;
					}*/
				}
				AttControl(dt);
				TransControl(dt);
			}
			break;
		case 104:
			//GetThrustVector(Thrust);
			/*GetGlobalOrientation(InertialOrientationRad);
			GetAngularVel(AngularVelocity);
			GetGlobalPos(GVesselPos);
			GetStatus(Status);
			GetElements(NULL, el, &oparam, 0, FRAME_EQU);
			//add ManeuverinProg code (may not be needed)
			ManeuverinProg=true;
			for(i=0;i<4;i++) {
				if(MET[i]<START_TIME[i]) {
					ManeuverinProg=false;
					break;
				}
			}*/
			AttControl(dt);
			TransControl(dt);
			if(!BurnCompleted && MNVRLOAD) Maneuver(dt);
			break;
		case 105:
			/*GetGlobalOrientation(InertialOrientationRad);
			GetAngularVel(AngularVelocity);
			GetGlobalPos(GVesselPos);
			GetStatus(Status);
			GetElements(NULL, el, &oparam, 0, FRAME_EQU);
			//add ManeuverinProg code (may not be needed)
			ManeuverinProg=true;
			for(i=0;i<4;i++) {
				if(MET[i]<START_TIME[i]) {
					ManeuverinProg=false;
					break;
				}
			}*/
			AttControl(dt);
			TransControl(dt);
			if(!BurnCompleted && MNVRLOAD) Maneuver(dt);
			break;
		case 106:
			/*GetGlobalOrientation(InertialOrientationRad);
			GetAngularVel(AngularVelocity);
			GetGlobalPos(GVesselPos);
			GetStatus(Status);
			GetElements(NULL, el, &oparam, 0, FRAME_EQU);
			//add ManeuverinProg code
			ManeuverinProg=true;
			for(i=0;i<4;i++) {
				if(MET[i]<START_TIME[i]) {
					ManeuverinProg=false;
					break;
				}
			}*/
			AttControl(dt);
			TransControl(dt);
			break;
		case 201:
			AttControl(dt);
			TransControl(dt);
			break;
		case 202:
			/*GetGlobalOrientation(InertialOrientationRad);
			GetAngularVel(AngularVelocity);
			GetGlobalPos(GVesselPos);
			GetStatus(Status);
			GetElements(NULL, el, &oparam, 0, FRAME_EQU);
			//add ManeuverinProg code
			ManeuverinProg=true;
			for(i=0;i<4;i++) {
				if(MET[i]<START_TIME[i]) {
					ManeuverinProg=false;
					break;
				}
			}*/
			AttControl(dt);
			TransControl(dt);
			if(!BurnCompleted && MNVRLOAD) Maneuver(dt);
			break;
		case 301:
			/*GetGlobalOrientation(InertialOrientationRad);
			GetAngularVel(AngularVelocity);
			GetGlobalPos(GVesselPos);
			GetStatus(Status);
			GetElements(NULL, el, &oparam, 0, FRAME_EQU);
			//add ManeuverinProg code
			ManeuverinProg=true;
			for(i=0;i<4;i++) {
				if(MET[i]<START_TIME[i]) {
					ManeuverinProg=false;
					break;
				}
			}*/
			AttControl(dt);
			TransControl(dt);
			break;
		case 302:
			/*GetGlobalOrientation(InertialOrientationRad);
			GetAngularVel(AngularVelocity);
			GetGlobalPos(GVesselPos);
			GetStatus(Status);
			GetElements(NULL, el, &oparam, 0, FRAME_EQU);
			//add ManeuverinProg code
			ManeuverinProg=true;
			for(i=0;i<4;i++) {
				if(MET[i]<START_TIME[i]) {
					ManeuverinProg=false;
					break;
				}
			}*/
			AttControl(dt);
			TransControl(dt);
			if(!BurnCompleted && MNVRLOAD) Maneuver(dt);
			break;
		case 303:
			/*GetGlobalOrientation(InertialOrientationRad);
			GetAngularVel(AngularVelocity);
			GetGlobalPos(GVesselPos);
			GetStatus(Status);
			GetElements(NULL, el, &oparam, 0, FRAME_EQU);
			//add ManeuverinProg code
			ManeuverinProg=true;
			for(i=0;i<4;i++) {
				if(MET[i]<START_TIME[i]) {
					ManeuverinProg=false;
					break;
				}
			}*/
			AttControl(dt);
			TransControl(dt);
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

void Atlantis::Maneuver(double dt)
{
	VECTOR3 ThrustVector;
	//sprintf(oapiDebugString(), "Maneuver %f %f %f %f", tig, met, BurnTime, IgnitionTime);
	if(BurnInProg)
	{
		if(met>=(IgnitionTime+BurnTime)) {
			//sprintf(oapiDebugString(), "Shutdown");
			SetThrusterGroupLevel(thg_main, 0.00);
			BurnCompleted=true;
		}
		else {
			GetThrustVector(ThrustVector); //inefficient
			//sprintf(oapiDebugString(), "%f %f %f", ThrustVector.z, ThrustVector.y, ThrustVector.x);
			/*for(int i=0;i<3;i++)
			{
				VGO.data[2-i]-=((ThrustVector.data[i]/GetMass())*dt)/fps_to_ms;
			}*/
			VGO.x-=((ThrustVector.z/GetMass())*dt)/fps_to_ms;
			VGO.y-=((ThrustVector.x/GetMass())*dt)/fps_to_ms;
			VGO.z+=((ThrustVector.y/GetMass())*dt)/fps_to_ms;
		}
	}
	else if(met>=tig)
	{
		//sprintf(oapiDebugString(), "Burning");
		BurnInProg=true;
		IgnitionTime=met;
		//ignite engines
		if(OMS==0) SetThrusterGroupLevel(thg_main, 1.00);
		else if(OMS!=3) SetThrusterLevel(th_oms[OMS-1], 1.00);
	}
	//else sprintf(oapiDebugString(), "Maneuver %f %f %f %f", tig, met, BurnTime, IgnitionTime);
}

void Atlantis::GimbalOMS(VECTOR3 Targets)
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
	return;
}

void Atlantis::LoadManeuver()
{
	int i;
	double StartWeight, EndWeight, EndWeightLast=0.0, FuelRate;
	//VECTOR3 ThrustVector;
	bool bDone=false;
	MNVRLOAD=true;
	tig=TIG[0]*86400+TIG[1]*3600+TIG[2]*60+TIG[3];
	
	//dV
	for(i=0;i<3;i++) {
		DeltaV.data[i]=PEG7.data[i]*fps_to_ms;
		//sts->VGO.data[i]=sts->PEG7.data[i];
	}
	DeltaVTot=length(PEG7);
	DeltaVTotms=length(DeltaV); //make local variable?
	GimbalOMS(Trim);
	
	//Burn Attitude
	if(OMS==0) {
		BurnAtt.data[PITCH]=ORBITER_OMS_PITCH+Trim.data[0]; //no Z DV component
		if(DeltaV.x!=0.0 || DeltaV.z!=0.0) {
			if(DeltaV.z<=0) BurnAtt.data[PITCH]+=DEG*acos(DeltaV.x/sqrt((pow(DeltaV.x, 2)+pow(DeltaV.z, 2))));
			else BurnAtt.data[PITCH]-=DEG*acos(DeltaV.x/sqrt((pow(DeltaV.x, 2)+pow(DeltaV.z, 2))));
		}
		if(DeltaV.x!=0.0 || DeltaV.y!=0.0)
			BurnAtt.data[YAW]=DEG*asin(DeltaV.y/sqrt((pow(DeltaV.x, 2)+pow(DeltaV.y, 2))))+Trim.data[1]+Trim.data[2];
		else BurnAtt.data[YAW]=Trim.data[1]+Trim.data[2];
		BurnAtt.data[ROLL]=TV_ROLL;
	}
	else if(OMS<3) {
		BurnAtt.data[PITCH]=ORBITER_OMS_PITCH+Trim.data[0];
		if(DeltaV.x!=0.0 || DeltaV.z!=0.0) {
			if(DeltaV.z<=0) BurnAtt.data[PITCH]+=DEG*acos(DeltaV.x/sqrt((pow(DeltaV.x, 2)+pow(DeltaV.z, 2))));
			else BurnAtt.data[PITCH]-=DEG*acos(DeltaV.x/sqrt((pow(DeltaV.x, 2)+pow(DeltaV.z, 2))));
		}
		if(DeltaV.x!=0.0 || DeltaV.y!=0.0)
			BurnAtt.data[YAW]=DEG*asin(DeltaV.y/sqrt((pow(DeltaV.x, 2)+pow(DeltaV.y, 2))))+Trim.data[OMS-1]+ORBITER_OMS_YAW*pow(-1.0, -OMS);
		else BurnAtt.data[YAW]=Trim.data[OMS-1]+ORBITER_OMS_YAW*pow(-1.0,- OMS);
		BurnAtt.data[ROLL]=TV_ROLL;
	}
	else {
		if(DeltaV.z<=0) BurnAtt.data[PITCH]=DEG*acos(DeltaV.x/sqrt((pow(DeltaV.x, 2)+pow(DeltaV.z, 2))));
		else BurnAtt.data[PITCH]=-DEG*acos(DeltaV.x/sqrt((pow(DeltaV.x, 2)+pow(DeltaV.z, 2))));
		BurnAtt.data[YAW]=DEG*asin(DeltaV.y/sqrt((pow(DeltaV.x, 2)+pow(DeltaV.y, 2))));
		BurnAtt.data[ROLL]=TV_ROLL;
	}
	if(TV_ROLL!=0.0) {
		double dTemp=BurnAtt.data[PITCH];
		BurnAtt.data[PITCH]-=BurnAtt.data[PITCH]*(1.0-cos(TV_ROLL*RAD));
		BurnAtt.data[YAW]+=BurnAtt.data[YAW]*(1.0-sin(TV_ROLL*RAD));
	}
	//use rocket equation
	StartWeight=WT/kg_to_pounds;
	EndWeight=StartWeight/exp(DeltaVTotms/GetThrusterIsp(th_oms[0]));
	FuelRate=ORBITER_OMS_THRUST/(GetPropellantEfficiency(ph_oms)*GetThrusterIsp(th_oms[0]));
	if(OMS==0) FuelRate=FuelRate*2.0;
	BurnTime=(StartWeight-EndWeight)/FuelRate;
	//TGO[0]=BurnTime/60;
	//TGO[1]=BurnTime-(TGO[0]*60);
	VGO.x=DeltaVTot*cos((ORBITER_OMS_PITCH+Trim.data[0])*RAD);
	VGO.y=DeltaVTot*sin((Trim.data[1]+Trim.data[2])*RAD);
	VGO.z=DeltaVTot*sin((ORBITER_OMS_PITCH+Trim.data[0])*RAD);
}

void Atlantis::UpdateDAP()
{
	//sprintf(oapiDebugString(), "UpdateDAP() called %f", oapiRand());
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
		oapiWriteLog("StartAttManeuver() called");
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
		Yaw=false;
		Pitch=false;
		Roll=false;
		ManeuverinProg=false;
	}
}

void Atlantis::TransControl(double SimdT)
{
	VECTOR3 ThrusterLevel=_V(0.0, 0.0, 0.0);

	for(int i=0;i<3;i++) {
		if(abs(THCInput.data[i])>0.01) {
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

void Atlantis::AttControl(double SimdT)
{
	int i;

	GetAngularVel(AngularVelocity);
	GetGlobalOrientation(InertialOrientationRad);
	CurrentAttitude=ConvertAnglesBetweenM50AndOrbiter(InertialOrientationRad);
	//ConvertLVLHAnglesToM50(_V(0, 0, 0)); //debugging

	GetGlobalPos(GVesselPos);
	GetStatus(Status);
	GetElements(NULL, el, &oparam, 0, FRAME_EQU);

	for(i=0;i<3;i++) {
		if(!RotPulseInProg[i]) ReqdRates.data[i]=AngularVelocity.data[i]*DEG;
	}

	if(!Eq(RHCInput.x, 0.0, 0.01) || !Eq(RHCInput.y, 0.0, 0.01) || !Eq(RHCInput.z, 0.0, 0.01)) {
		//TRK=ROT=false;
		//MNVR=true;
		if(ControlMode==AUTO) {
			ControlMode=INRTL;
			panelc3->UpdateVC(); //update PBI lights to reflect change in mode
		}
		if(ControlMode==INRTL) {
			TargetAttOrbiter=InertialOrientationRad;
			TargetAttM50=CurrentAttitude;
			REQD_ATT=CurrentAttitude*DEG;
		}
		else if(ControlMode==LVLH) {
			LVLHOrientationReqd=CalcLVLHAttitude()*DEG;
			ReqdAttMatrix=ConvertLVLHAnglesToM50Matrix(LVLHOrientationReqd*RAD);
		}
		ManeuverinProg=true;
		ManeuverStatus=MNVR_COMPLETE; //(check value set here)

		RHCInput.data[YAW]=-RHCInput.data[YAW]; //difference in Orbiter and STS frames
		for(i=0;i<3;i++) {
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
			if(ControlMode==INRTL) sprintf_s(oapiDebugString(), 255, "Rates: %f %f %f", ReqdRates.x, ReqdRates.y, ReqdRates.z);
		}
	}
	//else if(MNVR || TRK || ROT) {
	else if(ControlMode!=FREE) {
		MATRIX3 LastReqdAttMatrix;
		VECTOR3 NullRates, NullRatesLocal;
		if((ControlMode==AUTO && TRK) || ControlMode==LVLH) {
			//LastReqdAtt=REQD_ATT;
			LastReqdAttMatrix=ReqdAttMatrix;
			ReqdAttMatrix=ConvertLVLHAnglesToM50Matrix(LVLHOrientationReqd*RAD);
			REQD_ATT=GetAnglesFromMatrix(ReqdAttMatrix)*DEG;
			/*REQD_ATT=ConvertLVLHAnglesToM50(LVLHOrientationReqd*RAD)*DEG;

			MATRIX3 Test=ConvertLVLHAnglesToM50Matrix(LVLHOrientationReqd*RAD);
			VECTOR3 Test2=GetAnglesFromMatrix(Test)*DEG;
			sprintf_s(oapiDebugString(), 255, "TEST: %f %f %f", Test2.data[PITCH], Test2.data[YAW], Test2.data[ROLL]);*/
			//REQD_ATT=ConvertLocalAnglesToM50(_V(90, 0, 0)*RAD)*DEG;
			//sprintf(oapiDebugString(), "LVLHOReqd: %f %f %f Check: %f", LVLHOrientationReqd.x, LVLHOrientationReqd.y, LVLHOrientationReqd.z, oapiRand());
			//sprintf(oapiDebugString(), "AttControl: %f %f %f", LVLHOrientationReqd.x, LVLHOrientationReqd.y, LVLHOrientationReqd.z);
		}
		//REQD_ATT=ConvertLVLHAnglesToM50(LVLHOrientationReqd*-RAD)*DEG;
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
				//ManeuverStart=true;
				ManeuverStatus=MNVR_STARTING;
				sprintf_s(oapiDebugString(), 255, "MNVR STARTING");
			}
		}

		/*if(ManeuverinProg) {
			GetGlobalPos(GVesselPos);
			GetStatus(Status);
			GetElements(NULL, el, &oparam, 0, FRAME_EQU);
		}
		else return; //no need for further calculations*/
		if(!ManeuverinProg) return; //no need for further calculations

		if(ControlMode==LVLH || ((TRK || ROT) && ControlMode==AUTO)) {
			//VECTOR3 TargAttOrbiter=ConvertAnglesBetweenM50AndOrbiter(REQD_ATT*RAD, true);
			MATRIX3 TargAttMatrix=ConvertMatrixBetweenM50AndOrbiter(ReqdAttMatrix, true);
			VECTOR3 TargAttOrbiter=GetAnglesFromMatrix(TargAttMatrix);
			//VECTOR3 LastTargAttOrbiter=ConvertAnglesBetweenM50AndOrbiter(LastReqdAtt*RAD, true);
			MATRIX3 LastTargAttMatrix=ConvertMatrixBetweenM50AndOrbiter(LastReqdAttMatrix, true);
			VECTOR3 LastTargAttOrbiter=GetAnglesFromMatrix(LastTargAttMatrix);

			NullRatesLocal=(ConvertOrbiterAnglesToLocal(TargAttOrbiter)-ConvertOrbiterAnglesToLocal(LastTargAttOrbiter))/SimdT;
			double NullRatesMag=abs(NullRatesLocal.x)+abs(NullRatesLocal.y)+abs(NullRatesLocal.z);
			//double NullRatesMag=abs(NullRates.x)+abs(NullRates.y)+abs(NullRates.z);
			if(NullRatesMag>10.0) {
				//error; rates should not be this high
				return;
			}
			/*NullRatesMag=abs(NullRatesDiff.x)+abs(NullRatesDiff.y)+abs(NullRatesDiff.z);
			if(NullRatesMag>10.0) {
				//error; rates should not be this high
				return;
			}*/

			if(ManeuverStatus==MNVR_STARTING) {
				NullRates=(TargAttOrbiter-LastTargAttOrbiter)/SimdT;
				CalcManeuverTargets(NullRates);
				ManeuverStatus=MNVR_IN_PROGRESS;
				sprintf_s(oapiDebugString(), 255, "MNVR IN PROGRESS");
				//sprintf(oapiDebugString(), "Rates calculated %f", oapiRand());
				//oapiWriteLog(oapiDebugString());
			}
			else if(ManeuverStatus==MNVR_COMPLETE) {
				TargetAttOrbiter=TargAttOrbiter;
				MNVR_TIME=0.0;
			}
			else {
				MNVR_TIME-=SimdT;
			}
		}
		else NullRates=NullRatesLocal=_V(0, 0, 0); //MNVR or ControlMode==INRTL
		
		//PitchYawRoll=CalcPitchYawRollAngles()*DEG;
		PitchYawRollMatrix=CalcPitchYawRollRotMatrix();
		PitchYawRoll=GetAnglesFromMatrix(PitchYawRollMatrix)*DEG;
		//NullRates=ConvertOrbiterAnglesToLocal(NullRates);
		//sprintf(oapiDebugString(), "NR: %f %f %f", DEG*NullRatesDiff.x, DEG*NullRatesDiff.y, DEG*NullRatesDiff.z);
		//sprintf(oapiDebugString(), "NR: %f %f %f", DEG*NullRatesLocal.x, DEG*NullRatesLocal.y, DEG*NullRatesLocal.z);
		if(ManeuverStatus==MNVR_COMPLETE) CalcRequiredRates(ReqdRates, NullRatesLocal*DEG);
		else CalcRequiredRates(ReqdRates);
	}
	SetRates(ReqdRates);
}

void Atlantis::CalcManeuverTargets(VECTOR3 NullRates) //calculates TargetAttitude and time to reach attitude
{
	int counter=0;
	double LastMnvrTime;
	double Angle;
	MATRIX3 PYR;
	//VECTOR3 PYR, Axis;
	VECTOR3 Axis;
	VECTOR3 REQDATTOrbiter=ConvertAnglesBetweenM50AndOrbiter(REQD_ATT*RAD, true);
	TargetAttOrbiter=REQDATTOrbiter;
	//PYR=CalcPitchYawRollAngles()*DEG;
	PYR=CalcPitchYawRollRotMatrix();
	//MNVR_TIME=RotRate*(abs(PYR.x)+abs(PYR.y)+abs(PYR.z));
	Angle=CalcEulerAngle(IdentityMatrix, PYR, Axis);
	MNVR_TIME=RotRate*Angle*DEG;
	sprintf_s(oapiDebugString(), 255, "Initial MNVR TIME: %f", MNVR_TIME);
	//oapiWriteLog(oapiDebugString());
	do {
		counter++;
		LastMnvrTime=MNVR_TIME;
		if(MNVR_TIME<0) {
			MNVR_TIME=0.0;
			//break;
		}
		TargetAttOrbiter=REQDATTOrbiter+NullRates*MNVR_TIME; //check TargetAtt frame
		//double TargetAttMag=0.0;
		//for(int i=0;i<3;i++) TargetAttMag+=DEG*abs(TargetAttOrbiter.data[i]);
		//PYR=CalcPitchYawRollAngles()*DEG;
		PYR=CalcPitchYawRollRotMatrix();
		Angle=CalcEulerAngle(IdentityMatrix, PYR, Axis);
		MNVR_TIME=RotRate*Angle*DEG;
		//MNVR_TIME=RotRate*(abs(PYR.x)+abs(PYR.y)+abs(PYR.z));
		//sprintf(oapiDebugString(), "Iterating: %d %f", counter, MNVR_TIME);
		sprintf(oapiDebugString(), "Iterating: %d %f %f %f", counter, MNVR_TIME, NullRates.data[YAW]*DEG, TargetAttOrbiter.data[ROLL]*DEG);
		//oapiWriteLog(oapiDebugString());
	}while(abs(MNVR_TIME-LastMnvrTime)>0.05 && counter<50);
	if(counter>=50) {
		//TargetAttOrbiter=REQDATTOrbiter;
		//VECTOR3 Axis;
		//double Angle=CalcEulerAngle(_V(0, 0, 0), PYR*RAD, Axis);
		//sprintf_s(oapiDebugString(), 255, "Axis: %f %f %f Angle: %f", Axis.x, Axis.y, Axis.z, Angle);
		sprintf_s(oapiDebugString(), 255, "ERROR: Targets could not be calculated");
		oapiWriteLog(oapiDebugString());
	}
	else {
		TargetAttOrbiter=REQDATTOrbiter+NullRates*MNVR_TIME;
		RotationAxis=Axis;
		RotationAngle=Angle;
		/*sprintf_s(oapiDebugString(), 255, "Target Attitude: %f %f %f NR: %f %f %f Time: %f", TargetAttOrbiter.data[PITCH]*DEG,
			TargetAttOrbiter.data[YAW]*DEG, TargetAttOrbiter.data[ROLL]*DEG, NullRates.data[PITCH]*DEG,
			NullRates.data[YAW]*DEG, NullRates.data[ROLL]*DEG, MNVR_TIME);*/
		sprintf_s(oapiDebugString(), 255, "Axis: %f %f %f Angle: %f", Axis.x, Axis.y, Axis.z, Angle*DEG);
		oapiWriteLog(oapiDebugString());
	}
}

void Atlantis::SetRates(VECTOR3 &Rates)
{
	double dDiff;
	VECTOR3 CurrentRates;
	CurrentRates=AngularVelocity*DEG;
	//sprintf(oapiDebugString(), "%f", CurrentRates.data[PITCH]);
	//if(ControlMode==INRTL) sprintf_s(oapiDebugString(), 255, "REQD RATES: %f %f %f",
		//Rates.x, Rates.y, Rates.z);
	dDiff=Rates.data[PITCH]-CurrentRates.data[PITCH];
	if(DAPMode[1]!=2 && abs(dDiff)>0.05) {
		if(dDiff>0) {
			SetThrusterGroupLevel(thg_pitchup, 1.0);
			SetThrusterGroupLevel(thg_pitchdown, 0.0);
		}
		else if(dDiff<0) {
			SetThrusterGroupLevel(thg_pitchdown, 1.0);
			SetThrusterGroupLevel(thg_pitchup, 0.0);
		}
	}
	else if(abs(dDiff)>0.0009) {
		//sprintf(oapiDebugString(), "%f", dDiff);
		if(dDiff>0) {
			SetThrusterGroupLevel(thg_pitchup, 0.1);
			SetThrusterGroupLevel(thg_pitchdown, 0.0);
		}
		else if(dDiff<0) {
			SetThrusterGroupLevel(thg_pitchup, 0.0);
			SetThrusterGroupLevel(thg_pitchdown, 0.1);
		}
	}
	else {
		SetThrusterGroupLevel(thg_pitchup, 0.0);
		SetThrusterGroupLevel(thg_pitchdown, 0.0);
		//If RHC is out of detent, pretend pulse is still in progress
		if(abs(RHCInput.data[PITCH])<0.01) RotPulseInProg[PITCH]=false;
	}

	dDiff=Rates.data[YAW]-CurrentRates.data[YAW];
	if(DAPMode[1]!=2 && abs(dDiff)>0.05) {
		if(dDiff>0) {
			SetThrusterGroupLevel(thg_yawleft, 1.0);
			SetThrusterGroupLevel(thg_yawright, 0.0);
		}
		else if(dDiff<0) {
			SetThrusterGroupLevel(thg_yawright, 1.0);
			SetThrusterGroupLevel(thg_yawleft, 0.0);
		}
	}
	else if(abs(dDiff)>0.0009) {
		if(dDiff>0) {
			SetThrusterGroupLevel(thg_yawleft, 0.1);
			SetThrusterGroupLevel(thg_yawright, 0.0);
		}
		else if(dDiff<0) {
			SetThrusterGroupLevel(thg_yawright, 0.1);
			SetThrusterGroupLevel(thg_yawleft, 0.0);
		}
	}
	else {
		SetThrusterGroupLevel(thg_yawright, 0.0);
		SetThrusterGroupLevel(thg_yawleft, 0.0);
		if(abs(RHCInput.data[YAW])<0.01) RotPulseInProg[YAW]=false;
	}

	dDiff=Rates.data[ROLL]-CurrentRates.data[ROLL];
	if(DAPMode[1]!=2 && abs(dDiff)>0.05) {
		if(dDiff>0) {
			SetThrusterGroupLevel(thg_rollright, 1.0);
			SetThrusterGroupLevel(thg_rollleft, 0.0);
		}
		else if(dDiff<0) {
			SetThrusterGroupLevel(thg_rollleft, 1.0);
			SetThrusterGroupLevel(thg_rollright, 0.0);
		}
	}
	else if(abs(dDiff)>0.0009) {
		if(dDiff>0) {
			SetThrusterGroupLevel(thg_rollright, 0.1);
			SetThrusterGroupLevel(thg_rollleft, 0.0);
		}
		else if(dDiff<0) {
			SetThrusterGroupLevel(thg_rollleft, 0.1);
			SetThrusterGroupLevel(thg_rollright, 0.0);
		}
	}
	else {
		SetThrusterGroupLevel(thg_rollright, 0.0);
		SetThrusterGroupLevel(thg_rollleft, 0.0);
		if(abs(RHCInput.data[ROLL])<0.01) RotPulseInProg[ROLL]=false;
	}
}

/*void Atlantis::SetRates(VECTOR3 &Rates)
{
	double dDiff;
	VECTOR3 CurrentRates;
	CurrentRates=AngularVelocity*DEG;
	//sprintf(oapiDebugString(), "%f", CurrentRates.data[PITCH]);
	if(DAPMode[1]==0) {
		dDiff=Rates.data[PITCH]-CurrentRates.data[PITCH];
		if(abs(dDiff)>0.05) {
			if(dDiff>0) {
				SetThrusterGroupLevel(thg_pitchup, 1.0);
				SetThrusterGroupLevel(thg_pitchdown, 0.0);
			}
			else if(dDiff<0) {
				SetThrusterGroupLevel(thg_pitchdown, 1.0);
				SetThrusterGroupLevel(thg_pitchup, 0.0);
			}
		}
		else if(abs(dDiff)>0.0009) {
			//sprintf(oapiDebugString(), "%f", dDiff);
			if(dDiff>0) {
				SetThrusterGroupLevel(thg_pitchup, 0.1);
				SetThrusterGroupLevel(thg_pitchdown, 0.0);
			}
			else if(dDiff<0) {
				SetThrusterGroupLevel(thg_pitchup, 0.0);
				SetThrusterGroupLevel(thg_pitchdown, 0.1);
			}
		}
		else {
			SetThrusterGroupLevel(thg_pitchup, 0.0);
			SetThrusterGroupLevel(thg_pitchdown, 0.0);
		}
		dDiff=Rates.data[YAW]-CurrentRates.data[YAW];
		if(abs(dDiff)>0.05) {
			if(dDiff>0) {
				SetThrusterGroupLevel(thg_yawleft, 1.0);
				SetThrusterGroupLevel(thg_yawright, 0.0);
			}
			else if(dDiff<0) {
				SetThrusterGroupLevel(thg_yawright, 1.0);
				SetThrusterGroupLevel(thg_yawleft, 0.0);
			}
		}
		else if(abs(dDiff)>0.0009) {
			if(dDiff>0) {
				SetThrusterGroupLevel(thg_yawleft, 0.1);
				SetThrusterGroupLevel(thg_yawright, 0.0);
			}
			else if(dDiff<0) {
				SetThrusterGroupLevel(thg_yawright, 0.1);
				SetThrusterGroupLevel(thg_yawleft, 0.0);
			}
		}
		else {
			SetThrusterGroupLevel(thg_yawright, 0.0);
			SetThrusterGroupLevel(thg_yawleft, 0.0);
		}
		dDiff=Rates.data[ROLL]-CurrentRates.data[ROLL];
		if(abs(dDiff)>0.05) {
			if(dDiff>0) {
				SetThrusterGroupLevel(thg_rollright, 1.0);
				SetThrusterGroupLevel(thg_rollleft, 0.0);
			}
			else if(dDiff<0) {
				SetThrusterGroupLevel(thg_rollleft, 1.0);
				SetThrusterGroupLevel(thg_rollright, 0.0);
			}
		}
		else if(abs(dDiff)>0.0009) {
			if(dDiff>0) {
				SetThrusterGroupLevel(thg_rollright, 0.1);
				SetThrusterGroupLevel(thg_rollleft, 0.0);
			}
			else if(dDiff<0) {
				SetThrusterGroupLevel(thg_rollleft, 0.1);
				SetThrusterGroupLevel(thg_rollright, 0.0);
			}
		}
		else {
			SetThrusterGroupLevel(thg_rollright, 0.0);
			SetThrusterGroupLevel(thg_rollleft, 0.0);
		}
	}
	else if(DAPMode[1]==2) {
		dDiff=Rates.data[PITCH]-CurrentRates.data[PITCH];
		if(abs(dDiff)>0.00009) {
			//sprintf(oapiDebugString(), "%f", dDiff);
			if(dDiff>0) {
				SetThrusterGroupLevel(thg_pitchup, 0.1);
				SetThrusterGroupLevel(thg_pitchdown, 0.0);
			}
			else if(dDiff<0) {
				SetThrusterGroupLevel(thg_pitchdown, 0.1);
				SetThrusterGroupLevel(thg_pitchup, 0.0);
			}
		}
		else {
			SetThrusterGroupLevel(thg_pitchup, 0.0);
			SetThrusterGroupLevel(thg_pitchdown, 0.0);
		}
		dDiff=Rates.data[YAW]-CurrentRates.data[YAW];
		if(abs(dDiff)>0.00009) {
			if(dDiff>0) {
				SetThrusterGroupLevel(thg_yawleft, 0.1);
				SetThrusterGroupLevel(thg_yawright, 0.0);
			}
			else if(dDiff<0) {
				SetThrusterGroupLevel(thg_yawright, 0.1);
				SetThrusterGroupLevel(thg_yawleft, 0.0);
			}
		}
		else {
			SetThrusterGroupLevel(thg_yawleft, 0.0);
			SetThrusterGroupLevel(thg_yawright, 0.0);
		}
		dDiff=Rates.data[ROLL]-CurrentRates.data[ROLL];
		if(abs(dDiff)>0.00009) {
			if(dDiff>0) {
				SetThrusterGroupLevel(thg_rollright, 0.1);
				SetThrusterGroupLevel(thg_rollleft, 0.0);
			}
			else if(dDiff<0) {
				SetThrusterGroupLevel(thg_rollleft, 0.1);
				SetThrusterGroupLevel(thg_rollright, 0.0);
			}
		}
		else {
			SetThrusterGroupLevel(thg_rollleft, 0.0);
			SetThrusterGroupLevel(thg_rollright, 0.0);
		}
	}
}*/

void Atlantis::CalcRequiredRates(VECTOR3 &Rates)
{
	//convert rotation around RotationAxis to rates around x,y,z axes

	VECTOR3 CurrentRates;
	CurrentRates=ToDeg(AngularVelocity);
	//sprintf(oapiDebugString(), "AttControl");
	Mass=GetMass();
	GetPMI(PMI);
	//Rates.data[PITCH]=0.0;
	//Rates.data[YAW]=0.0;
	//Rates.data[ROLL]=0.0;

	//RotationAngle=CalcEulerAngle(_V(0, 0, 0), PitchYawRoll*RAD, RotationAxis);
	RotationAngle=CalcEulerAngle(IdentityMatrix, PitchYawRollMatrix, RotationAxis);
	//Rates=RotationAxis*-RotRate;
	for(int i=0;i<3;i++) {
		if(ControlMode==AUTO || RotMode[i]==0) Rates.data[i]=RotationAxis.data[i]*-RotRate;
	}

	if(abs(PitchYawRoll.data[PITCH])<=NullStartAngle(abs(AngularVelocity.data[PITCH]), PITCH)) {
		if(ControlMode==AUTO || RotMode[PITCH]==0) Rates.data[PITCH]=0.0;
		Pitch=false;
	}
	else Pitch=true;
	if(abs(PitchYawRoll.data[YAW])<=NullStartAngle(abs(AngularVelocity.data[YAW]), YAW)) {
		if(ControlMode==AUTO || RotMode[YAW]==0) Rates.data[YAW]=0.0;
		Yaw=false;
	}
	else Yaw=true;
	if(abs(PitchYawRoll.data[ROLL])<=NullStartAngle(abs(AngularVelocity.data[ROLL]), ROLL)) {
		if(ControlMode==AUTO || RotMode[ROLL]==0) Rates.data[ROLL]=0.0;
		Roll=false;
	}
	else Roll=true;

	if(!Pitch && !Yaw && !Roll) {
		ManeuverStatus=MNVR_COMPLETE; //now maintaining targ. attitude
		sprintf_s(oapiDebugString(), 255, "MNVR COMPLETE");
	}
	if(ControlMode==INRTL) sprintf_s(oapiDebugString(), 255, "RATES1: %f %f %f %i", Rates.data[ROLL], Rates.data[PITCH], Rates.data[YAW], DAPMode[1]);
}

void Atlantis::CalcRequiredRates(VECTOR3 &Rates, const VECTOR3 &NullRates) //vectors in degrees
{
	VECTOR3 CurrentRates;
	CurrentRates=AngularVelocity*DEG;
	//sprintf(oapiDebugString(), "AttControl");
	Mass=GetMass();
	GetPMI(PMI);
	//Rates=_V(0, 0, 0);
	//if(MNVR || TRK || ROT) {
	if(ControlMode!=FREE) {
		if(ControlMode==AUTO || RotMode[ROLL]==0) {
			if(!Yaw && !Pitch && (Roll || abs(PitchYawRoll.data[ROLL])>AttDeadband)) {
				if(abs(PitchYawRoll.data[ROLL])<0.05) Roll=false;
				else {
					Roll=true;
				}
				if(Roll) {
					//sprintf(oapiDebugString(), "%f %f %f", abs(PitchYawRoll.data[ROLL]), 
					//	NullStartAngle(abs(AngularVelocity.data[ROLL]), ORBITER_ROLL_TORQUE, ROLL), PMI.data[ROLL]);
					if(abs(PitchYawRoll.data[ROLL])<=NullStartAngle(abs(AngularVelocity.data[ROLL]), ROLL)) {
						Rates.data[ROLL]=0.0;
						//nRoll=1;
						//Roll=false;
					}
					else {
						if(PitchYawRoll.data[ROLL]>0) {
							if(ManeuverStatus==MNVR_COMPLETE) Rates.data[ROLL]=max(-RotRate, -PitchYawRoll.data[ROLL]/10.0);
							else Rates.data[ROLL]=-RotRate;
						}
						else {
							if(ManeuverStatus==MNVR_COMPLETE) Rates.data[ROLL]=min(RotRate, -PitchYawRoll.data[ROLL]/10.0);
							else Rates.data[ROLL]=RotRate;
						}
					}
				}
				else Rates.data[ROLL]=0.0;
			}
			else Rates.data[ROLL]=0.0;
		}

		if(ControlMode==AUTO || RotMode[YAW]==0) {
			if(!Roll && !Pitch && (Yaw || abs(PitchYawRoll.data[YAW])>AttDeadband)) {
				if(abs(PitchYawRoll.data[YAW])<0.05) Yaw=false;
				else {
					Yaw=true;
				}
				if(Yaw) {
					if(abs(PitchYawRoll.data[YAW])<=NullStartAngle(abs(AngularVelocity.data[YAW]), YAW)) {
						Rates.data[YAW]=0.0;
						//Pitch=false;
					}
					else {
						if(PitchYawRoll.data[YAW]>0) {
							if(ManeuverStatus==MNVR_COMPLETE) Rates.data[YAW]=max(-RotRate, -PitchYawRoll.data[YAW]/10.0);
							else Rates.data[YAW]=-RotRate;
						}
						else {
							if(ManeuverStatus==MNVR_COMPLETE) Rates.data[YAW]=min(RotRate, -PitchYawRoll.data[YAW]/10.0);
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
			if(!Roll && !Yaw && (Pitch || abs(PitchYawRoll.data[PITCH])>AttDeadband)) {
				if(abs(PitchYawRoll.data[PITCH])<0.05) {
					Pitch=false;
					//ManeuverComplete=true;
					sprintf(oapiDebugString(), "Maneuver completed");
				}
				else {
					Pitch=true;
				}
				if(Pitch) {
					if(abs(PitchYawRoll.data[PITCH])<=NullStartAngle(abs(AngularVelocity.data[PITCH]), PITCH)) {
						//sprintf(oapiDebugString(), "%f %f", abs(PitchYawRoll.data[PITCH]), NullStartAngle(abs(AngularVelocity.data[PITCH]), ORBITER_PITCH_TORQUE, PITCH));
						Rates.data[PITCH]=0.00000;
						//Pitch=false;
					}
					else {
						if(PitchYawRoll.data[PITCH]>0) {
							if(ManeuverStatus==MNVR_COMPLETE) Rates.data[PITCH]=max(-RotRate, -PitchYawRoll.data[PITCH]/10.0);
							else Rates.data[PITCH]=-RotRate;
						}
						else {
							if(ManeuverStatus==MNVR_COMPLETE) Rates.data[PITCH]=min(RotRate, -PitchYawRoll.data[PITCH]/10.0);
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
		}

		if(!Pitch && !Yaw && !Roll) {
			ManeuverStatus=MNVR_COMPLETE; //now maintaining targ. attitude
			//sprintf_s(oapiDebugString(), 255, "MNVR COMPLETE");
		}

		if(ManeuverStatus==MNVR_COMPLETE) {
			//Rates=Rates+NullRates;
			for(int i=0;i<3;i++) {
				if(ControlMode==AUTO || RotMode[i]==0) Rates.data[i]+=NullRates.data[i];
			}
		}
	}
}
