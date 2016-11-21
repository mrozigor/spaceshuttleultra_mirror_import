#include "OrbitDAP.h"
#include "IDP.h"
#include <UltraMath.h>
#include "../ParameterValues.h"
#include "RHC_SOP.h"
#include "THC_SOP.h"


namespace dps
{

void SaveAttManeuver(FILEHANDLE scn, char* item, const AttManeuver& maneuver)
{
	char cbuf[255];
	VECTOR3 eulerAngles = GetYZX_PYRAnglesFromMatrix(maneuver.tgtMatrix);
	sprintf_s(cbuf, 255, "%d %f %f %f", maneuver.Type, eulerAngles.data[PITCH], eulerAngles.data[YAW], eulerAngles.data[ROLL]);
	oapiWriteScenario_string(scn, item, cbuf);
}

void LoadAttManeuver(const char* value, AttManeuver& maneuver)
{
	int nTemp;
	VECTOR3 vTemp;
	sscanf_s(value, "%d%lf%lf%lf", &nTemp, &vTemp.data[PITCH], &vTemp.data[YAW], &vTemp.data[ROLL]);

	if(nTemp == AttManeuver::MNVR || nTemp == AttManeuver::TRK) {
		maneuver.IsValid = true;
		maneuver.Type = static_cast<AttManeuver::TYPE>(nTemp);
		maneuver.tgtMatrix = GetRotationMatrixYZX(_V(vTemp.data[ROLL], vTemp.data[PITCH], vTemp.data[YAW]));
	}
}

OrbitDAP::OrbitDAP(SimpleGPCSystem* pGPC)
: SimpleGPCSoftware(pGPC, "OrbitDAP"),
OMSTVCControlP(5.0, 0.0, 0.5), OMSTVCControlY(4.0, 0.0, 0.75), OMSTVCControlR(5.0, 0.0, 0.5),
ControlMode(RCS),
DAPMode(PRI), DAPSelect(A), DAPControlMode(INRTL), editDAP(-1),
ManeuverStatus(MNVR_OFF),
bFirstStep(true), lastStepDeltaT(1.0),
PCTActive(false),
pStateVector(NULL) 
{
	OMSTrim = _V(0.0, 0.0, 0.0);
	degReqdRates = _V(0.0, 0.0, 0.0);
	TransPulseDV = _V(0.0, 0.0, 0.0);
	ATT_ERR = _V(0.0, 0.0, 0.0);
	REQD_ATT = _V(0.0, 0.0, 0.0);

	for(unsigned int i=0;i<4;i++) START_TIME[i] = 0;
	MNVR_OPTION = _V(0.0, 0.0, 0.0);
	TGT_ID=2;
	BODY_VECT=1;
	P=0;
	Y=0;
	OM=-1;

	ActiveManeuver.IsValid = false;
	CurManeuver.IsValid = false;
	FutManeuver.IsValid = false;

	Torque.data[PITCH]=ORBITER_PITCH_TORQUE;
	Torque.data[YAW]=ORBITER_YAW_TORQUE;
	Torque.data[ROLL]=ORBITER_ROLL_TORQUE;

	for(unsigned int i=0;i<3;i++) {
		RotMode[i] = DISC_RATE;
		TransMode[i] = NORM;
		RotPulseInProg[i] = false;
		TransPulseInProg[i]=false;
		RotatingAxis[i] = false;
		NullingRates[i] = false;

		//Initialize DAP Config
		DAPConfiguration[i].PRI_ROT_RATE=2.0;
		DAPConfiguration[i].PRI_ATT_DB=5.0;
		DAPConfiguration[i].PRI_RATE_DB=0.2;
		DAPConfiguration[i].PRI_ROT_PLS=0.1;
		DAPConfiguration[i].PRI_COMP=0.0;
		DAPConfiguration[i].PRI_TRAN_PLS=0.1;
		DAPConfiguration[i].PRI_P_OPTION=0;
		DAPConfiguration[i].PRI_Y_OPTION=0;
		DAPConfiguration[i].ALT_RATE_DB=0.2;
		DAPConfiguration[i].ALT_ON_TIME=0.08;
		DAPConfiguration[i].ALT_DELAY=0.0;
		DAPConfiguration[i].ALT_JET_OPT=0;
		DAPConfiguration[i].ALT_JETS=2;
		DAPConfiguration[i].VERN_ROT_RATE=0.1;
		DAPConfiguration[i].VERN_ATT_DB=1.0;
		DAPConfiguration[i].VERN_RATE_DB=0.020;
		DAPConfiguration[i].VERN_ROT_PLS=0.01;
		DAPConfiguration[i].VERN_COMP=0.0;
		DAPConfiguration[i].VERN_CNTL_ACC=0;
	}

	ERRTOT = true;
}

OrbitDAP::~OrbitDAP()
{
}

void OrbitDAP::UseOMSTVC(CONTROL_MODE Mode, const VECTOR3& Trim)
{
	if(Mode != RCS) {
		ControlMode = Mode;
		OMSTVCControlP.Reset();
		OMSTVCControlY.Reset();
		OMSTVCControlR.Reset();
		OMSTrim = Trim;
	}
}

void OrbitDAP::UseRCS()
{
	ControlMode = RCS;
}

OrbitDAP::DAP_CONTROL_MODE OrbitDAP::GetDAPMode() const
{
	return DAPControlMode;
}

/*void OrbitDAP::ManeuverToLVLHAttitude(const VECTOR3& degLVLHAtt)
{
	LoadCurLVLHManeuver(degLVLHAtt*RAD);
}*/

void OrbitDAP::ManeuverToINRTLAttitude(const VECTOR3& degINRTLAtt)
{
	//VECTOR3 radOrbiterAtt = ConvertAnglesBetweenM50AndOrbiter(degINRTLAtt*RAD, true);
	MATRIX3 OrbiterAtt = GetRotationMatrixYZX(_V(degINRTLAtt.data[ROLL], degINRTLAtt.data[PITCH], degINRTLAtt.data[YAW])*RAD);
	LoadCurINRTLManeuver(OrbiterAtt);
}

void OrbitDAP::LoadCurLVLHManeuver(const MATRIX3& tgtMatrixLVLH)
{
	CurManeuver.IsValid = true;
	CurManeuver.tgtMatrix = tgtMatrixLVLH;
	CurManeuver.Type = AttManeuver::TRK;
	if(DAPControlMode == AUTO) StartCurManeuver();
}

void OrbitDAP::LoadFutLVLHManeuver(const MATRIX3& tgtMatrixLVLH)
{
	FutManeuver.IsValid = true;
	FutManeuver.tgtMatrix = tgtMatrixLVLH;
	FutManeuver.Type = AttManeuver::TRK;
}

void OrbitDAP::LoadCurINRTLManeuver(const MATRIX3& tgtMatrixM50)
{
	CurManeuver.IsValid = true;
	CurManeuver.tgtMatrix = tgtMatrixM50;
	CurManeuver.Type = AttManeuver::MNVR;
	if(DAPControlMode == AUTO) StartCurManeuver();
}

void OrbitDAP::LoadFutINRTLManeuver(const MATRIX3& tgtMatrixM50)
{
	FutManeuver.IsValid = true;
	FutManeuver.tgtMatrix = tgtMatrixM50;
	FutManeuver.Type = AttManeuver::MNVR;
}

void OrbitDAP::StartCurManeuver()
{
	if(CurManeuver.IsValid) {
		//if(CurManeuver.Type == AttManeuver::MNVR) StartINRTLManeuver(CurManeuver.radTargetAttOrbiter);
		//else if(CurManeuver.Type == AttManeuver::TRK) StartLVLHManeuver(CurManeuver.radTargetLVLHAtt);
		StartManeuver(CurManeuver.tgtMatrix, CurManeuver.Type);
	}
}
	
void OrbitDAP::StartManeuver(const MATRIX3& tgtAtt, AttManeuver::TYPE type)
{
	ActiveManeuver.tgtMatrix = tgtAtt;
	ActiveManeuver.Type = type;
	ActiveManeuver.IsValid = true;
	
	ManeuverStatus = MNVR_IN_PROGRESS;
		
	if(ActiveManeuver.Type == AttManeuver::MNVR) {
		degNullRates = _V(0, 0, 0);
		// calculate M50 target att
		REQD_ATT = GetYZX_PYRAnglesFromMatrix(ActiveManeuver.tgtMatrix)*DEG;
		// calculate time to reach target attitude
		VECTOR3 Axis;
		MATRIX3 AttError = GetRotationErrorMatrix(curM50Matrix, tgtAtt);
		double Angle = CalcEulerAngle(AttError, Axis);
		mnvrCompletionMET = STS()->GetMET() + (Angle*DEG)/degRotRate;
		lastUpdateTime = 0.0;
	}
	else {
		mnvrCompletionMET = STS()->GetMET();
		lastUpdateTime = -100.0;
	}
}

bool OrbitDAP::GetRHCRequiredRates()
{
	bool outOfDetent = false;
	double RHC[3];
	RHC[0] = pRHC_SOP->GetPitchCommand();
	RHC[1] = pRHC_SOP->GetYawCommand();
	RHC[2] = pRHC_SOP->GetRollCommand();
	bool RHCdetent[3];
	RHCdetent[0] = pRHC_SOP->GetPitchDetent();
	RHCdetent[1] = pRHC_SOP->GetYawDetent();
	RHCdetent[2] = pRHC_SOP->GetRollDetent();
	bool RHCpastsoftstop[3];
	RHCpastsoftstop[0] = pRHC_SOP->GetPitchPastSoftStop();
	RHCpastsoftstop[1] = pRHC_SOP->GetYawPastSoftStop();
	RHCpastsoftstop[2] = pRHC_SOP->GetRollPastSoftStop();

	for(unsigned int i=0;i<3;i++) {
		if (RHCdetent[i] == false)
		{
			outOfDetent = true;
			if (RHCpastsoftstop[i] == false)
			{
				if(RotMode[i]==DISC_RATE) { // DISC RATE
					// if RHC was pushed past soft stop previously (high rotation rates), maintain high rotation rate; otherwise rotate at specified rate
					degReqdRates.data[i] = max(degRotRate, abs(degAngularVelocity.data[i]))*sign( RHC[i] );
				}
				else if(!RotPulseInProg[i]) { // PULSE
					degReqdRates.data[i] = degReqdRates.data[i]+degRotPulse*sign( RHC[i] );
					RotPulseInProg[i]=true;
				}
			}
			else {
				degReqdRates.data[i] = 1000*sign( RHC[i] );
			}
		}
	}
	return outOfDetent;
}

void OrbitDAP::HandleTHCInput(double DeltaT)
{
	int THC[3];
	THC[0] = pTHC_SOP->GetXCommand();
	THC[1] = pTHC_SOP->GetYCommand();
	THC[2] = pTHC_SOP->GetZCommand();

	for(int i=0;i<3;i++) {
		if (THC[i] != 0)
		{
			//if PCT is in progress, disable it when THC is moved out of detent
			if(PCTActive) StopPCT();

			if(TransMode[i]==NORM) {
				TransThrusterCommands[i].SetLine(static_cast<float>( sign( THC[i] ) ));
			}
			else if(TransMode[i]==TRANS_PULSE && !TransPulseInProg[i]) {
				TransPulseInProg[i]=true;
				TransPulseDV.data[i] = TransPulse*sign( THC[i] );
			}
			else {
				TransThrusterCommands[i].ResetLine();
			}
		}
		else {
			TransThrusterCommands[i].ResetLine();
		}

		if(TransPulseInProg[i]) {
			if(!Eq(TransPulseDV.data[i], 0.0, 0.001)) {
				TransThrusterCommands[i].SetLine(static_cast<float>( sign(TransPulseDV.data[i]) ));
				// calculate DV so far
				VECTOR3 ThrustVector;
				STS()->GetThrustVector(ThrustVector);
				ThrustVector = _V(ThrustVector.z, ThrustVector.x, -ThrustVector.y); // convert from Orbiter frame to shuttle frame
				double acc = (ThrustVector.data[i]/OrbiterMass)*DeltaT;
				if(abs(TransPulseDV.data[i]) <= abs(acc/2.0)) { //minimize error
					TransPulseDV.data[i]=0.000;
				}
				else {
					TransPulseDV.data[i] -= acc;
				}
			}
			else {
				//if THC is in detent and pulse is complete, allow further pulses
				if (THC[i] == 0) TransPulseInProg[i]=false;
				TransThrusterCommands[i].ResetLine();
			}
		}
	}
}

void OrbitDAP::CalcEulerAxisRates()
{	
	double timeAcc = max(1.0, oapiGetTimeAcceleration());

	VECTOR3 RotationAxis;
	double RotationAngle=CalcEulerAngle(attErrorMatrix, RotationAxis);
	//Rates=RotationAxis*-RotRate;
	RotationAxis = _V(RotationAxis.y, RotationAxis.z, RotationAxis.x); // change rotation axis so PYR axes are mapped correctly
	for(unsigned int i=0;i<3;i++) {
		if(DAPControlMode==AUTO || RotMode[i]==DISC_RATE) {
			degReqdRates.data[i]=RotationAxis.data[i]*degRotRate;
			if(abs(ATT_ERR.data[i]) <= NullStartAngle(abs(radAngularVelocity.data[i]), OrbiterMass, PMI.data[i], Torque.data[i]/timeAcc)) {
				degReqdRates.data[i] = 0.0;
				RotatingAxis[i] = false;
				NullingRates[i] = true;
			}
			else RotatingAxis[i] = true;
		}
		else { // PULSE mode; maintain current rate
			degReqdRates.data[i] = degAngularVelocity.data[i];
		}
	}
}

void OrbitDAP::CalcMultiAxisRates(const VECTOR3& degNullRatesLocal)
{
	double timeAcc = max(1.0, oapiGetTimeAcceleration());

	for(unsigned int i=0;i<3;i++) {
		if(DAPControlMode==AUTO || RotMode[i]==DISC_RATE) {
			degReqdRates.data[i]=0.0;
			if((RotatingAxis[i] || abs(ATT_ERR.data[i])>degAttDeadband)) {
				if(abs(ATT_ERR.data[i])<0.05) {
					RotatingAxis[i]=false;
					NullingRates[i] = true;
				}
				else {
					RotatingAxis[i]=true;
					if(abs(ATT_ERR.data[i]) <= NullStartAngle(abs(radAngularVelocity.data[i]), OrbiterMass, PMI.data[i], Torque.data[i]/timeAcc)) {
						degReqdRates.data[i] = 0.0;
					}
					else {
						degReqdRates.data[i] = sign(ATT_ERR.data[i])*range(0.05*degRotRate, abs(ATT_ERR.data[i])/5.0, 0.1*degRotRate);
					}
				}
			}
		}
		else { // PULSE mode; maintain current rate
			degReqdRates.data[i] = degAngularVelocity.data[i];
		}
	}

	// add null rates to maintain attitude in rotation frame
	for(int i=0;i<3;i++) {
		if(DAPControlMode==AUTO || RotMode[i]==DISC_RATE) degReqdRates.data[i]+=degNullRatesLocal.data[i];
	}
}

void OrbitDAP::UpdateNullRates()
{
	ELEMENTS el;
	ORBITPARAM param;
	STS()->GetElements(STS()->GetGravityRef(), el, &param);
	double orb_rad = 360.0/param.T;
	
	VECTOR3 tgtLVLHAtt = GetYZX_PYRAnglesFromMatrix(ActiveManeuver.tgtMatrix);
	degNullRates.data[ROLL] = -orb_rad*sin(tgtLVLHAtt.data[YAW]);
	degNullRates.data[PITCH] = -orb_rad*cos(tgtLVLHAtt.data[YAW])*cos(tgtLVLHAtt.data[ROLL]);
	degNullRates.data[YAW] = orb_rad*cos(tgtLVLHAtt.data[YAW])*sin(tgtLVLHAtt.data[ROLL]);
}

void OrbitDAP::SetRates(const VECTOR3 &degRates, double DeltaT)
{
	const VECTOR3 PRI_LIMITS = _V(0.005, 0.005, 0.005);
	const VECTOR3 VERN_LIMITS = _V(0.0015, 0.0015, 0.0015);
	//static char buf[100];


	//double dDiff;
	VECTOR3 Error = degRates-degAngularVelocity;
	Error.data[YAW] = Error.data[YAW]; // temporary
	Error.data[ROLL] = Error.data[ROLL];



	VECTOR3 Limits;
	double MaxThrusterLevel;
	double timeAcc = max(1.0, oapiGetTimeAcceleration());
	if(DAPMode != VERN) { // PRI/ALT
		for(unsigned int i=0;i<3;i++) {
			//if(RotatingAxis[i] || NullingRates[i]) Limits.data[i] = max(PRI_LIMITS.data[i], 0.5*RotationRateChange(OrbiterMass, PMI.data[i], Torque.data[i]/timeAcc, DeltaT));
			if(RotatingAxis[i]) Limits.data[i] = max(2.0*PRI_LIMITS.data[i], 0.5*RotationRateChange(OrbiterMass, PMI.data[i], Torque.data[i]/timeAcc, DeltaT));
			else if(NullingRates[i] || RotPulseInProg[i]) Limits.data[i] = max(PRI_LIMITS.data[i], 0.5*RotationRateChange(OrbiterMass, PMI.data[i], Torque.data[i]/timeAcc, DeltaT));
			else Limits.data[i] = degRateDeadband;
		}
		MaxThrusterLevel = 1.0/timeAcc;
	}
	else { // VERN
		for(unsigned int i=0;i<3;i++) {
			if(RotatingAxis[i] || NullingRates[i]) Limits.data[i] = max(VERN_LIMITS.data[i], 0.5*RotationRateChange(OrbiterMass, PMI.data[i], 0.1*Torque.data[i]/timeAcc, DeltaT));
			else Limits.data[i] = degRateDeadband;
		}
		MaxThrusterLevel = 0.1/timeAcc;
	}
	//if(ManeuverStatus==MNVR_IN_PROGRESS) Limits=Limits*5.0;

	bool RHCdetent[3];
	RHCdetent[0] = pRHC_SOP->GetPitchDetent();
	RHCdetent[1] = pRHC_SOP->GetYawDetent();
	RHCdetent[2] = pRHC_SOP->GetRollDetent();

	for(unsigned int i=0;i<3;i++) {
		if(abs(Error.data[i])>Limits.data[i]) {
			//RotThrusterCommands[i].SetLine(static_cast<float>(MaxThrusterLevel*sign(Error.data[i])));
			double thrusterLevel = MaxThrusterLevel;
			if(DAPMode != VERN) { // for PRI/ALT, there are multiple thrusters in each direction, and we don't need to fire all of them
				double scale = abs(Error.data[i])/Limits.data[i];
				if(scale < 2) thrusterLevel = MaxThrusterLevel/3.0;
				else if(scale < 5) thrusterLevel = MaxThrusterLevel*(0.667);
			}
			RotThrusterCommands[i].SetLine(static_cast<float>(thrusterLevel*sign(Error.data[i])));
		}
		else {
			RotThrusterCommands[i].ResetLine();
			NullingRates[i] = false;
			//If RHC is out of detent, pretend pulse is still in progress
			if (RHCdetent[i] == true) RotPulseInProg[i]=false;
		}
	}




}

void OrbitDAP::OMSTVC(const VECTOR3 &Rates, double SimDT)
{
	VECTOR3 CurrentRates=radAngularVelocity*DEG;
	double pitchDelta=Rates.data[PITCH]-CurrentRates.data[PITCH]; //if positive, vessel is pitching down
	double yawDelta=Rates.data[YAW]-CurrentRates.data[YAW]; //if positive, vessel is rotating to right
	double rollDelta=Rates.data[ROLL]-CurrentRates.data[ROLL]; //if positive, vessel is rolling to left
	bool RCSWraparound=(abs(rollDelta)>2.05 || abs(pitchDelta)>2.05 || abs(yawDelta)>2.05);

	double dPitch=OMSTVCControlP.Step(-pitchDelta, SimDT);
	double dYaw=OMSTVCControlY.Step(-yawDelta, SimDT);
	double dRoll = OMSTVCControlR.Step(-rollDelta, SimDT);

	if(ControlMode!=RIGHT_OMS) //left OMS engine burning
	{
		//double dPitch=2.5*pitchDelta, dYaw=3.0*yawDelta; //changes in gimbal position from default (trim) angle
		double Pitch=OMSTrim.data[0]+dPitch, Yaw=OMSTrim.data[1]+dYaw;
		if(ControlMode==BOTH_OMS) Pitch-=dRoll;
		if(!GimbalOMS(LEFT, Pitch, Yaw)) RCSWraparound=true;
	}
	if(ControlMode!=LEFT_OMS) //right OMS engine burning
	{
		//double dPitch=2.5*pitchDelta, dYaw=3.0*yawDelta; //changes in gimbal position from default (trim) angle
		double Pitch=OMSTrim.data[0]+dPitch, Yaw=OMSTrim.data[2]+dYaw;
		if(ControlMode==BOTH_OMS) Pitch+=dRoll;
		if(!GimbalOMS(RIGHT, Pitch, Yaw)) RCSWraparound=true;
	}

	if(RCSWraparound) SetRates(Rates, SimDT);
	else if(ControlMode!=BOTH_OMS) SetRates(_V(0.0, 0.0, Rates.data[ROLL]), SimDT); //for single-engine burns, use RCS for roll control
	else {
		// turn off RCS thrusters
		for(int i=0;i<3;i++) RotThrusterCommands[i].ResetLine();
	}
}

bool OrbitDAP::GimbalOMS(SIDE side, double pitch, double yaw)
{
	double correctedPitch = range(-OMS_PITCH_RANGE, pitch, OMS_PITCH_RANGE);
	double correctedYaw = range(-OMS_YAW_RANGE, yaw, OMS_YAW_RANGE);
	POMSGimbalCommand[side].SetLine(static_cast<float>(correctedPitch/OMS_PITCH_RANGE));
	YOMSGimbalCommand[side].SetLine(static_cast<float>(correctedYaw/OMS_YAW_RANGE));
	return abs(pitch)<=OMS_PITCH_RANGE && abs(yaw)<=OMS_YAW_RANGE;
}

void OrbitDAP::GetAttitudeData()
{
	STS()->GetAngularVel(radAngularVelocity);
	radAngularVelocity = _V(radAngularVelocity.x, -radAngularVelocity.y, radAngularVelocity.z); // convert from Orbitersim to body axis frame
	degAngularVelocity = radAngularVelocity*DEG;
	
	STS()->GetRotationMatrix(curM50Matrix);
	curM50Matrix = ConvertOrbitersimRotationMatrixToM50(curM50Matrix);
	CUR_ATT = GetYZX_PYRAnglesFromMatrix(curM50Matrix)*DEG;
	
	OrbiterMass = STS()->GetMass();
	STS()->GetPMI(PMI);
}

void OrbitDAP::Realize()
{
	DiscreteBundle* pBundle = BundleManager()->CreateBundle("DAP_PBIS1", 16);
	for(int i=0;i<16;i++) {
		PBI_input[i].Connect(pBundle, i);
		PBI_output[i].Connect(pBundle, i);
	}

	pBundle=BundleManager()->CreateBundle("DAP_PBIS2", 16);
	for(int i=16;i<24;i++) {
		PBI_input[i].Connect(pBundle, i-16);
		PBI_output[i].Connect(pBundle, i-16);
	}
	
	/*for(int i=0;i<24;i++) {
		PBI_state[i] = GetPBIState(i);
		if(PBI_state[i]) PBI_output[i].SetLine();
		else PBI_output[i].ResetLine();
	}*/	

	pBundle = BundleManager()->CreateBundle("LOMS", 5);
	POMSGimbalCommand[LEFT].Connect(pBundle, 3);
	YOMSGimbalCommand[LEFT].Connect(pBundle, 4);
	pBundle = BundleManager()->CreateBundle("ROMS", 5);
	POMSGimbalCommand[RIGHT].Connect(pBundle, 3);
	YOMSGimbalCommand[RIGHT].Connect(pBundle, 4);

	pBundle=STS()->BundleManager()->CreateBundle("THRUSTER_CMD", 16);
	for(unsigned int i=0;i<3;i++) {
		RotThrusterCommands[i].Connect(pBundle, i);
		TransThrusterCommands[i].Connect(pBundle, i+3);
	}

	pBundle=BundleManager()->CreateBundle("SPDBKTHROT_CONTROLS", 16);
	PCTArmed.Connect(pBundle, 0);

	pBundle=BundleManager()->CreateBundle("BODYFLAP_CONTROLS", 16);
	BodyFlapAuto.Connect(pBundle, 0);
	port_PCTActive[0].Connect(pBundle, 0);
	port_PCTActive[1].Connect(pBundle, 1);
	
	pStateVector = static_cast<StateVectorSoftware*>(FindSoftware("StateVectorSoftware"));
	pRHC_SOP = static_cast<RHC_SOP*>(FindSoftware( "RHC_SOP" ));
	pTHC_SOP = static_cast<THC_SOP*>(FindSoftware( "THC_SOP" ));

	UpdateDAPParameters();
}

void OrbitDAP::OnPreStep(double SimT, double DeltaT, double MJD)
{
	GetAttitudeData();

	for(int i=0;i<24;i++) {
		if(PBI_input[i] && !PBI_state[i]) {
			ButtonPress(i);
		}
		PBI_state[i] = GetPBIState(i);
		if(PBI_state[i]) PBI_output[i].SetLine();
		else PBI_output[i].ResetLine();
	}

	// in AUTO, INRTL or LVLH modes, start maneuver
	if(bFirstStep) {
		if(DAPControlMode == AUTO) {
			if(CurManeuver.IsValid) StartCurManeuver();
		}
		else if(DAPControlMode == INRTL) {
			StartManeuver(curM50Matrix, AttManeuver::MNVR);
		}
		else if(DAPControlMode == LVLH) {
			StartManeuver(GetCurrentLVLHAttMatrix(), AttManeuver::TRK);
		}
		bFirstStep = false;
	}

	// monitor future loaded maneuver
	if(FutManeuver.IsValid) {
		if(FutMnvrStartTime<=STS()->GetMET()) {
			CurManeuver = FutManeuver;
			if(DAPControlMode == AUTO) StartCurManeuver();
			FutManeuver.IsValid = false;
		}
	}

	if ((GetMajorMode() / 100) == 2)
	{
		if(PCTArmed && BodyFlapAuto && !PCTActive) StartPCT();
		else if(!BodyFlapAuto && PCTActive) StopPCT();
		if(!PCTActive) HandleTHCInput(DeltaT);
		else PCTControl(SimT);
	}
	else HandleTHCInput(DeltaT);

	if(GetRHCRequiredRates()) {
		if(DAPControlMode == AUTO) {
			DAPControlMode = INRTL;
			StartManeuver(curM50Matrix, AttManeuver::MNVR);
		}
		else if(DAPControlMode == INRTL) {
			StartManeuver(curM50Matrix, AttManeuver::MNVR);
		}
		else if(DAPControlMode == LVLH) {
			StartManeuver(GetCurrentLVLHAttMatrix(), AttManeuver::TRK);
		}
		ATT_ERR = _V(0.0, 0.0, 0.0);
	}
	else if(DAPControlMode != FREE) { // if DAP is in FREE, we only care about RHC input; otherwise, we want to maintain target attitude
		MATRIX3 tgtM50Matrix;  // target M50 attitude for this timestep
		if(ActiveManeuver.Type == AttManeuver::TRK) { // get (instantaneous) target M50 attitude
			MATRIX3 curLVLHMatrix = GetCurrentLVLHRefMatrix();
			tgtM50Matrix = mul(curLVLHMatrix, ActiveManeuver.tgtMatrix);
			REQD_ATT = GetYZX_PYRAnglesFromMatrix(tgtM50Matrix)*DEG;

			if((STS()->GetMET()-lastUpdateTime) > 60.0) {
				UpdateNullRates();
				if(ManeuverStatus < MNVR_COMPLETE) mnvrCompletionMET = STS()->GetMET() + CalcManeuverCompletionTime(curM50Matrix, ActiveManeuver.tgtMatrix, curLVLHMatrix, length(degNullRates));
				lastUpdateTime = STS()->GetMET();
			}
			if(ManeuverStatus == MNVR_IN_PROGRESS) {
				// calculate M50 matrix corresponding to target attitude at end of maneuver
				double rotationAngle = RAD*length(degNullRates)*(mnvrCompletionMET-STS()->GetMET());
				MATRIX3 LVLHRotation;
				GetRotMatrixY(-rotationAngle, LVLHRotation);
				tgtM50Matrix = mul(mul(curLVLHMatrix, LVLHRotation), ActiveManeuver.tgtMatrix);
			}
		}
		else {
			tgtM50Matrix = ActiveManeuver.tgtMatrix;

			if (((STS()->GetMET()-lastUpdateTime) > 10.0) && (ManeuverStatus < MNVR_COMPLETE))
			{
				// recalculate time to reach target attitude
				VECTOR3 Axis;
				MATRIX3 AttError = GetRotationErrorMatrix(curM50Matrix, ActiveManeuver.tgtMatrix);
				double Angle = CalcEulerAngle(AttError, Axis);
				mnvrCompletionMET = STS()->GetMET() + (Angle*DEG)/degRotRate;
				lastUpdateTime = STS()->GetMET();
			}
		}
		
		attErrorMatrix = GetRotationErrorMatrix(curM50Matrix, tgtM50Matrix);
		ATT_ERR=GetYZX_PYRAnglesFromMatrix(attErrorMatrix)*DEG;
		if(ManeuverStatus == MNVR_COMPLETE) {
			CalcMultiAxisRates(degNullRates);
		}
		else if(ManeuverStatus == MNVR_IN_PROGRESS) {
			CalcEulerAxisRates();
			if(!RotatingAxis[PITCH] && !RotatingAxis[YAW] && !RotatingAxis[ROLL]) {
				ManeuverStatus=MNVR_COMPLETE; //now maintaining targ. attitude
			}
		}
		else { // MNVR_STARTING
			degReqdRates = _V(0.0, 0.0, 0.0);
		}
	}
	else { // FREE
		degReqdRates = degAngularVelocity;
	}
	if(ControlMode == RCS) SetRates(degReqdRates, DeltaT);
	else OMSTVC(ATT_ERR*0.1, DeltaT);

	lastStepDeltaT = DeltaT;
}

bool OrbitDAP::OnMajorModeChange(unsigned int newMajorMode)
{
	if(newMajorMode >= 104 && newMajorMode <= 303) {
		// perform initialization

		// turn FCS lights off
		DiscreteBundle* pBundle = BundleManager()->CreateBundle( "CSS_CONTROLS", 16 );
		DiscOutPort port;
		for (int i = 1; i <= 15; i += 2)
		{
			port.Connect( pBundle, i );
			port.ResetLine();
		}
		return true;
	}
	return false;
}

bool OrbitDAP::ItemInput(int spec, int item, const char* Data)
{
	if(spec==dps::MODE_UNDEFINED) {
		if(GetMajorMode()!=201) return false;

		if(item>=1 && item<=4) {
			int nNew=atoi(Data);
			if((item==1 && nNew<365) || (item==2 && nNew<24) || (item>2 && nNew<60)) {
				START_TIME[item-1]=nNew;
			}
		}
		else if(item==5 || item==6) {
			double dNew=atof(Data);
			if(dNew>=0.0 && dNew<360.0) {
				if(item==5) MNVR_OPTION.data[ROLL]=dNew;
				else MNVR_OPTION.data[PITCH]=dNew;
			}
		}
		else if(item==7) {
			double dNew=atof(Data);
			if((dNew>=0.0 && dNew<=90.0) || (dNew>=270.0 && dNew<360.0)) {
				MNVR_OPTION.data[YAW]=dNew;
			}
		}
		if(item==8) {
			int nNew=atoi(Data);
			if(nNew==2 || nNew==4) {
				TGT_ID=nNew;
			}
		}
		if(item==14) {
			int nNew=atoi(Data);
			if(nNew>=1 && nNew<=5) {
				BODY_VECT=nNew;
				if(BODY_VECT==1) {
					P=0.0;
					Y=0.0;
				}
				else if(BODY_VECT==2) {
					P=180.0;
					Y=0.0;
				}
				else if(BODY_VECT==3) {
					P=90.0;
					Y=0.0;
				}
				else if(BODY_VECT==4) {
					P=90.0;
					Y=79.333;
				}
			}
		}
		if(item==15 && BODY_VECT==5) {
			double dNew=atof(Data);
			if(dNew>=0.0 && dNew<360.0) {
				P=dNew;
			}
		}
		if(item==16 && BODY_VECT==5) {
			double dNew=atof(Data);
			if((dNew>=0.0 && dNew<=90.0) || (dNew>=270.0 && dNew<360.0)) {
				Y=dNew;
			}
		}
		if(item==17) {
			double dNew=atof(Data);
			if(dNew>=0.0 && dNew<360.0) {
				OM=dNew;
			}
		}
		else if(item==18) {
			//VECTOR3 radTargetAtt = ConvertAnglesBetweenM50AndOrbiter(MNVR_OPTION*RAD, true);
			MATRIX3 tgtAtt = GetRotationMatrixYZX(_V(MNVR_OPTION.data[ROLL], MNVR_OPTION.data[PITCH], MNVR_OPTION.data[YAW])*RAD);
			double startTime = START_TIME[0]*86400.0+ START_TIME[1]*3600.0 + START_TIME[2]*60.0 + START_TIME[3];
			if(startTime <= STS()->GetMET()) {
				LoadCurINRTLManeuver(tgtAtt);
			}
			else {
				FutMnvrStartTime = startTime;
				LoadFutINRTLManeuver(tgtAtt);
			}
		}
		else if(item==19) {
			//VECTOR3 radTargetAtt = ConvertPYOMToBodyAngles(P*RAD, Y*RAD, OM*RAD);
			MATRIX3 tgtAtt = ConvertPYOMToLVLH(P*RAD, Y*RAD, OM*RAD);
			double startTime = START_TIME[0]*86400.0 + START_TIME[1]*3600.0 + START_TIME[2]*60.0 + START_TIME[3];
			if(startTime <= STS()->GetMET()) {
				if(TGT_ID == 2) {
					LoadCurLVLHManeuver(tgtAtt);
				}
			}
			else {
				FutMnvrStartTime = startTime;
				if(TGT_ID == 2) {
					LoadFutLVLHManeuver(tgtAtt);
				}
			}
		}
		/*else if(item==20) {
		LoadRotationManeuver();
		}*/
		else if(item==21) {
			CurManeuver.IsValid = false;
			FutManeuver.IsValid = false;
			RotatingAxis[YAW]=false;
			RotatingAxis[PITCH]=false;
			RotatingAxis[ROLL]=false;

			DAPControlMode=INRTL;
			//StartINRTLManeuver(radCurrentOrbiterAtt);
			StartManeuver(curM50Matrix, AttManeuver::MNVR);
		}
		else if (item == 23) ERRTOT = true;// ERR TOT
		else if (item == 24) ERRTOT = false;// ERR DAP
		return true;
	}
	else if(spec==20) {
		if(item==3 || item==4) {
			editDAP=item-3;
			DAPConfiguration[2]=DAPConfiguration[editDAP];
		}
		else if(item==5) {
			if(editDAP != -1) DAPConfiguration[editDAP]=DAPConfiguration[2];
			editDAP=-1;
		}
		if(item==10 || item==30 || item==50) {
			double dNew=atof(Data);
			if(dNew>=0.05 && dNew<=2.0) {
				DAPConfiguration[convert[item]].PRI_ROT_RATE=dNew;
				if(convert[item]==DAPSelect && DAPMode==PRI) UpdateDAPParameters();
			}
		}
		else if(item==11 || item==31 || item==51) {
			double dNew=atof(Data);
			if(dNew>0.10 && dNew<=40.0) {
				DAPConfiguration[convert[item]].PRI_ATT_DB=dNew;
				if(convert[item]==DAPSelect && DAPMode==PRI) UpdateDAPParameters();
			}
		}
		else if(item==12 || item==32 || item==52) {
			double dNew=atof(Data);
			if(dNew>=0.10 && dNew<=5.0) {
				DAPConfiguration[convert[item]].PRI_RATE_DB=dNew;
				if(convert[item]==DAPSelect && DAPMode==PRI) UpdateDAPParameters();
			}
		}
		else if(item==13 || item==33 || item==53) {
			double dNew=atof(Data);
			if(dNew>=0.04 && dNew<=1.0) {
				DAPConfiguration[convert[item]].PRI_ROT_PLS=dNew;
				if(convert[item]==DAPSelect && DAPMode==PRI) UpdateDAPParameters();
			}
		}
		else if(item==14 || item==34 || item==54) {
			double dNew=atof(Data);
			if(dNew>=0.0 && dNew<=0.999) {
				DAPConfiguration[convert[item]].PRI_COMP=dNew;
				if(convert[item]==DAPSelect && DAPMode==PRI) UpdateDAPParameters();
			}
		}
		else if(item==15 || item==35 || item==55) {
			if(DAPConfiguration[convert[item]].PRI_P_OPTION<2) {
				DAPConfiguration[convert[item]].PRI_P_OPTION++;
				if(DAPMode==PRI) {
					if(DAPConfiguration[DAPSelect].PRI_P_OPTION==1) {
						STS()->DisableThrusters(AftPitchThrusters, 2);
						UpdateDAPParameters();
					}
					else if(DAPConfiguration[DAPSelect].PRI_P_OPTION==2) {
						STS()->EnableThrusters(AftPitchThrusters, 2);
						STS()->DisableThrusters(NosePitchThrusters, 2);
						UpdateDAPParameters();
					}
				}
			}
			else {
				DAPConfiguration[convert[item]].PRI_P_OPTION=0;
				if(DAPConfiguration[DAPSelect].PRI_P_OPTION==0) {
					STS()->EnableThrusters(NosePitchThrusters, 2);
					UpdateDAPParameters();
				}
			}
		}
		else if(item==16 || item==36 || item==56) {
			if(DAPConfiguration[convert[item]].PRI_Y_OPTION<2) {
				DAPConfiguration[convert[item]].PRI_Y_OPTION++;
				if(DAPMode==PRI) {
					if(DAPConfiguration[DAPSelect].PRI_Y_OPTION==1) {
						STS()->DisableThrusters(AftYawThrusters, 2);
						UpdateDAPParameters();
					}
					else if(DAPConfiguration[DAPSelect].PRI_Y_OPTION==2) {
						STS()->EnableThrusters(AftYawThrusters, 2);
						STS()->DisableThrusters(NoseYawThrusters, 2);
						UpdateDAPParameters();
					}
				}
			}
			else {
				DAPConfiguration[convert[item]].PRI_Y_OPTION=0;
				if(DAPConfiguration[DAPSelect].PRI_Y_OPTION==0) {
					STS()->EnableThrusters(NoseYawThrusters, 2);
					UpdateDAPParameters();
				}
			}
		}
		else if(item==17 || item==37 || item==57) {
			double dNew=atof(Data);
			if(dNew>=0.01 && dNew<=5.0) {
				DAPConfiguration[convert[item]].PRI_TRAN_PLS=dNew;
				if(convert[item]== DAPSelect && DAPMode==PRI) UpdateDAPParameters();
			}
		}
		else if(item==18 || item==38 || item==58) {
			double dNew=atof(Data);
			if(dNew>=0.05 && dNew<=5.0) {
				DAPConfiguration[convert[item]].ALT_RATE_DB=dNew;
				if(convert[item]==DAPSelect && DAPMode==ALT) UpdateDAPParameters();
			}
		}
		else if(item==19 || item==39 || item==59) {
			if(DAPConfiguration[convert[item]].ALT_JET_OPT==0) {
				DAPConfiguration[convert[item]].ALT_JET_OPT=2;
				if(DAPMode==ALT) {
					if(DAPConfiguration[DAPSelect].ALT_JET_OPT==2) {
						STS()->DisableThrusters(NoseRotThrusters, 6);
						STS()->EnableThrusters(AftRotThrusters, 6);
						UpdateDAPParameters();
					}
				}
			}
			else {
				DAPConfiguration[convert[item]].ALT_JET_OPT=0;
				if(DAPMode==ALT) {
					if(DAPConfiguration[DAPSelect].ALT_JET_OPT==0) {
						STS()->EnableThrusters(NoseRotThrusters, 6);
						UpdateDAPParameters();
					}
				}
			}
		}
		else if(item==20 || item==40 || item==60) {
			int nNew=atoi(Data);
			if(nNew>=1 && nNew<=3) {
				DAPConfiguration[convert[item]].ALT_JETS=nNew;
				if(convert[item]==DAPSelect && DAPMode==ALT) UpdateDAPParameters();
			}
		}
		else if(item==21 || item==41 || item==61) {
			double dNew=atof(Data);
			if(dNew>=0.08 && dNew<=9.99) {
				DAPConfiguration[convert[item]].ALT_ON_TIME=dNew;
				if(convert[item]==DAPSelect && DAPMode==ALT) UpdateDAPParameters();
			}
		}
		else if(item==22 || item==42 || item==62) {
			double dNew=atof(Data);
			if(dNew>=0.0 && dNew<=99.99) {
				DAPConfiguration[convert[item]].ALT_DELAY=dNew;
				if(convert[item]==DAPSelect && DAPMode==ALT) UpdateDAPParameters();
			}
		}
		else if(item==23 || item==43 || item==63) {
			double dNew=atof(Data);
			if(dNew>=0.002 && dNew<=1.0) {
				DAPConfiguration[convert[item]].VERN_ROT_RATE=dNew;
				if(convert[item]==DAPSelect && DAPMode==VERN) UpdateDAPParameters();
			}
		}
		else if(item==24 || item==44 || item==64) {
			double dNew=atof(Data);
			if(dNew>=0.01 && dNew<=40.0) {
				DAPConfiguration[convert[item]].VERN_ATT_DB=dNew;
				if(convert[item]==DAPSelect && DAPMode==VERN) UpdateDAPParameters();
			}
		}
		else if(item==25 || item==45 || item==65) {
			double dNew=atof(Data);
			if(dNew>=0.01 && dNew<=0.5) {
				DAPConfiguration[convert[item]].VERN_RATE_DB=dNew;
				if(convert[item]==DAPSelect && DAPMode==VERN) UpdateDAPParameters();
			}
		}
		else if(item==26 || item==46 || item==66) {
			double dNew=atof(Data);
			if(dNew>=0.001 && dNew<=0.5) {
				DAPConfiguration[convert[item]].VERN_ROT_PLS=dNew;
				if(convert[item]==DAPSelect && DAPMode==VERN) UpdateDAPParameters();
			}
		}
		else if(item==27 || item==47 || item==67) {
			double dNew=atof(Data);
			if(dNew>=0.0 && dNew<=0.999) {
				DAPConfiguration[convert[item]].VERN_COMP=dNew;
				if(convert[item]==DAPSelect && DAPMode==VERN) UpdateDAPParameters();
			}
		}
		else if(item==28 || item==48 || item==68) {
			int nNew=atoi(Data);
			if(nNew>=0 && nNew<=9) {
				DAPConfiguration[convert[item]].VERN_CNTL_ACC=nNew;
				if(convert[item]==DAPSelect && DAPMode==VERN) UpdateDAPParameters();
			}
		}
		return true;
	}
	return false;
}

bool OrbitDAP::OnPaint(int spec, vc::MDU* pMDU) const
{
	if(spec==dps::MODE_UNDEFINED && GetMajorMode()==201) {
		PaintUNIVPTGDisplay(pMDU);
		return true;
	}
	else if(spec==20) {
		PaintDAPCONFIGDisplay(pMDU);
		return true;
	}
	return false;
}

void OrbitDAP::PaintUNIVPTGDisplay(vc::MDU* pMDU) const
{
	char cbuf[255];
	PrintCommonHeader("    UNIV PTG", pMDU);

	double CUR_MNVR_COMPL[4];
	if(DAPControlMode == INRTL || DAPControlMode == FREE) ConvertSecondsToDDHHMMSS(STS()->GetMET(), CUR_MNVR_COMPL);
	else ConvertSecondsToDDHHMMSS(mnvrCompletionMET, CUR_MNVR_COMPL);
	pMDU->mvprint(3, 1, "CUR MNVR COMPL");
	sprintf_s(cbuf, 255, "%.2d:%.2d:%.2d", static_cast<int>(CUR_MNVR_COMPL[1]), static_cast<int>(CUR_MNVR_COMPL[2]), static_cast<int>(CUR_MNVR_COMPL[3]));
	pMDU->mvprint(18, 1, cbuf);
	sprintf_s(cbuf, 255, "1 START TIME %.3d/%.2d:%.2d:%.2d", 
		START_TIME[0], START_TIME[1], START_TIME[2], START_TIME[3]);
	pMDU->mvprint(1, 2, cbuf);
	pMDU->Underline( 14, 2 );
	pMDU->Underline( 15, 2 );
	pMDU->Underline( 16, 2 );
	pMDU->Underline( 18, 2 );
	pMDU->Underline( 19, 2 );
	pMDU->Underline( 21, 2 );
	pMDU->Underline( 22, 2 );
	pMDU->Underline( 24, 2 );
	pMDU->Underline( 25, 2 );

	pMDU->mvprint(0, 4, "MNVR OPTION");
	sprintf_s(cbuf, 255, "5 R %6.2f", MNVR_OPTION.data[ROLL]);
	pMDU->mvprint(1, 5, cbuf);
	pMDU->Underline( 5, 5 );
	pMDU->Underline( 6, 5 );
	pMDU->Underline( 7, 5 );
	pMDU->Underline( 8, 5 );
	pMDU->Underline( 9, 5 );
	pMDU->Underline( 10, 5 );
	sprintf_s(cbuf, 255, "6 P %6.2f", MNVR_OPTION.data[PITCH]);
	pMDU->mvprint(1, 6, cbuf);
	sprintf_s(cbuf, 255, "7 Y %6.2f", MNVR_OPTION.data[YAW]);
	pMDU->mvprint(1, 7, cbuf);

	pMDU->mvprint(0, 9, "TRK/ROT OPTIONS");
	sprintf_s(cbuf, 255, "8 TGT ID %3d", TGT_ID);
	pMDU->mvprint(1, 10, cbuf);
	pMDU->Underline( 10, 10 );
	pMDU->Underline( 11, 10 );
	pMDU->Underline( 12, 10 );

	pMDU->mvprint(1, 12, "9  RA");
	pMDU->mvprint(1, 13, "10 DEC");
	pMDU->mvprint(1, 14, "11 LAT");
	pMDU->mvprint(1, 15, "12 LON");
	pMDU->mvprint(1, 16, "13 ALT");

	sprintf_s(cbuf, 255, "14 BODY VECT %d", BODY_VECT);
	pMDU->mvprint(1, 18, cbuf);
	sprintf_s(cbuf, 255, "15 P  %6.2f", P);
	pMDU->mvprint(1, 20, cbuf);
	pMDU->Underline( 7, 20 );
	pMDU->Underline( 8, 20 );
	pMDU->Underline( 9, 20 );
	pMDU->Underline( 10, 20 );
	pMDU->Underline( 11, 20 );
	pMDU->Underline( 12, 20 );
	sprintf_s(cbuf, 255, "16 Y  %6.2f", Y);
	pMDU->mvprint(1, 21, cbuf);
	if(OM>=0.0) {
		sprintf_s(cbuf, 255, "17 OM %6.2f", OM);
		pMDU->mvprint(1, 22, cbuf);
	}
	else pMDU->mvprint(1, 22, "17 OM");

	pMDU->mvprint(14, 4, "START MNVR 18");
	pMDU->mvprint(20, 5, "TRK  19");
	pMDU->mvprint(20, 6, "ROT  20");
	pMDU->mvprint(19, 7, "CNCL  21");
	pMDU->mvprint(28, 3, "CUR");
	pMDU->mvprint(32, 3, "FUT");
	if(CurManeuver.IsValid) {
		if(CurManeuver.Type == AttManeuver::MNVR) {
			pMDU->mvprint(29, 4, "*");
		}
		else if(CurManeuver.Type == AttManeuver::TRK) {
			pMDU->mvprint(29, 5, "*");
		}
		else {
			pMDU->mvprint(29, 6, "*");
		}
	}
	if(FutManeuver.IsValid) {
		if(FutManeuver.Type == AttManeuver::MNVR) {
			pMDU->mvprint(33, 4, "*");
		}
		else if(FutManeuver.Type == AttManeuver::TRK) {
			pMDU->mvprint(33, 5, "*");
		}
		else {
			pMDU->mvprint(33, 6, "*");
		}
	}

	pMDU->mvprint(19, 9, "ATT MON");
	pMDU->mvprint(20, 10, "22 MON AXIS");
	pMDU->mvprint(20, 11, "ERR TOT 23");
	pMDU->mvprint(20, 12, "ERR DAP 24");
	if (ERRTOT == true) pMDU->mvprint( 31, 11, "*" );// ERR TOT
	else pMDU->mvprint( 31, 12, "*" );// ERR DAP

	pMDU->mvprint(26, 14, "ROLL   PITCH    YAW");
	sprintf_s(cbuf, 255, "CUR   %6.2f  %6.2f  %6.2f", CUR_ATT.data[ROLL], CUR_ATT.data[PITCH], CUR_ATT.data[YAW]);
	pMDU->mvprint(19, 15, cbuf);
	sprintf_s(cbuf, 255, "REQD  %6.2f  %6.2f  %6.2f", REQD_ATT.data[ROLL], REQD_ATT.data[PITCH], REQD_ATT.data[YAW]);
	pMDU->mvprint(19, 16, cbuf);
	sprintf_s(cbuf, 255, "ERR   %6.2f  %6.2f  %6.2f", fabs( ATT_ERR.data[ROLL] ), fabs( ATT_ERR.data[PITCH] ), fabs( ATT_ERR.data[YAW] ));
	pMDU->mvprint(19, 17, cbuf);
	pMDU->NumberSign( 24, 17, ATT_ERR.data[ROLL] );
	pMDU->NumberSign( 32, 17, ATT_ERR.data[PITCH] );
	pMDU->NumberSign( 40, 17, ATT_ERR.data[YAW] );
	sprintf_s(cbuf, 255, "RATE  %6.3f  %6.3f  %6.3f", fabs( degAngularVelocity.data[ROLL] ), fabs( degAngularVelocity.data[PITCH] ), fabs( degAngularVelocity.data[YAW] ));
	pMDU->mvprint(19, 18, cbuf);
	pMDU->NumberSign( 24, 18, degAngularVelocity.data[ROLL] );
	pMDU->NumberSign( 32, 18, degAngularVelocity.data[PITCH] );
	pMDU->NumberSign( 40, 18, degAngularVelocity.data[YAW] );
}

void OrbitDAP::PaintDAPCONFIGDisplay(vc::MDU* pMDU) const
{
	char *strings[3]={" ALL", "NOSE", "TAIL"};
	char cbuf[255];
	int lim[3]={3, 5, 5};
	int i, n;

	PrintCommonHeader("   DAP CONFIG", pMDU);

	pMDU->mvprint(4, 2, "PRI");
	pMDU->mvprint(9, 2, "1 DAP A");
	pMDU->mvprint(20, 2, "2 DAP B");
	pMDU->mvprint(33, 2, "PRI");
	pMDU->mvprint(0, 3, "ROT RATE");
	pMDU->mvprint(0, 4, "ATT DB");
	pMDU->mvprint(0, 5, "RATE DB");
	pMDU->mvprint(0, 6, "ROT PLS");
	pMDU->mvprint(0, 7, "COMP");
	pMDU->mvprint(0, 8, "P OPTION");
	pMDU->mvprint(0, 9, "Y OPTION");
	pMDU->mvprint(0, 10, "TRAN PLS");

	pMDU->mvprint(4, 11, "ALT");
	pMDU->mvprint(33, 11, "ALT");
	pMDU->mvprint(0, 12, "RATE DB");
	pMDU->mvprint(0, 13, "JET OPT");
	pMDU->mvprint(0, 14, "# JETS");
	pMDU->mvprint(0, 15, "ON TIME");
	pMDU->mvprint(0, 16, "DELAY");

	pMDU->mvprint(4, 17, "VERN");
	pMDU->mvprint(33, 17, "VERN");
	pMDU->mvprint(0, 18, "ROT RATE");
	pMDU->mvprint(0, 19, "ATT DB");
	pMDU->mvprint(0, 20, "RATE DB");
	pMDU->mvprint(0, 21, "ROT PLS");
	pMDU->mvprint(0, 22, "COMP");
	pMDU->mvprint(0, 23, "CNTL ACC");

	pMDU->mvprint( 16, 2, "01" );
	pMDU->Underline( 16, 2 );
	pMDU->Underline( 17, 2 );
	pMDU->mvprint( 27, 2, "02" );
	pMDU->Underline( 27, 2 );
	pMDU->Underline( 28, 2 );

	int edit=2; //temporary
	for(n=1, i=0;n<=lim[edit];n+=2, i++) {
		sprintf_s(cbuf, 255, "%d %.4f", 10*n, DAPConfiguration[i].PRI_ROT_RATE);
		pMDU->mvprint(9+11*i, 3, cbuf);
		pMDU->Underline( 12 + 11 * i, 3 );
		pMDU->Underline( 13 + 11 * i, 3 );
		pMDU->Underline( 14 + 11 * i, 3 );
		pMDU->Underline( 15 + 11 * i, 3 );
		pMDU->Underline( 16 + 11 * i, 3 );
		pMDU->Underline( 17 + 11 * i, 3 );
		sprintf_s(cbuf, 255, "%d  %05.2f", 10*n+1, DAPConfiguration[i].PRI_ATT_DB);
		pMDU->mvprint(9+11*i, 4, cbuf);
		pMDU->Underline( 13 + 11 * i, 4 );
		pMDU->Underline( 14 + 11 * i, 4 );
		pMDU->Underline( 15 + 11 * i, 4 );
		pMDU->Underline( 16 + 11 * i, 4 );
		pMDU->Underline( 17 + 11 * i, 4 );
		sprintf_s(cbuf, 255, "%d   %.2f", 10*n+2, DAPConfiguration[i].PRI_RATE_DB);
		pMDU->mvprint(9+11*i, 5, cbuf);
		pMDU->Underline( 14 + 11 * i, 5 );
		pMDU->Underline( 15 + 11 * i, 5 );
		pMDU->Underline( 16 + 11 * i, 5 );
		pMDU->Underline( 17 + 11 * i, 5 );
		sprintf_s(cbuf, 255, "%d  %.3f", 10*n+3, DAPConfiguration[i].PRI_ROT_PLS);
		pMDU->mvprint(9+11*i, 6, cbuf);
		pMDU->Underline( 13 + 11 * i, 6 );
		pMDU->Underline( 14 + 11 * i, 6 );
		pMDU->Underline( 15 + 11 * i, 6 );
		pMDU->Underline( 16 + 11 * i, 6 );
		pMDU->Underline( 17 + 11 * i, 6 );
		sprintf_s(cbuf, 255, "%d  %.3f", 10*n+4, DAPConfiguration[i].PRI_COMP);
		pMDU->mvprint(9+11*i, 7, cbuf);
		pMDU->mvprint( 13 + 11 * i, 7, " " );
		pMDU->Underline( 14 + 11 * i, 7 );
		pMDU->Underline( 15 + 11 * i, 7 );
		pMDU->Underline( 16 + 11 * i, 7 );
		pMDU->Underline( 17 + 11 * i, 7 );
		sprintf_s(cbuf, 255, "%d   %s", 10*n+5, strings[DAPConfiguration[i].PRI_P_OPTION]);
		pMDU->mvprint(9+11*i, 8, cbuf);
		sprintf_s(cbuf, 255, "%d   %s", 10*n+6, strings[DAPConfiguration[i].PRI_Y_OPTION]);
		pMDU->mvprint(9+11*i, 9, cbuf);
		sprintf_s(cbuf, 255, "%d  %.3f", 10*n+7, DAPConfiguration[i].PRI_TRAN_PLS);
		pMDU->mvprint(9+11*i, 10, cbuf);
		pMDU->Underline( 13 + 11 * i, 10 );
		pMDU->Underline( 14 + 11 * i, 10 );
		pMDU->Underline( 15 + 11 * i, 10 );
		pMDU->Underline( 16 + 11 * i, 10 );
		pMDU->Underline( 17 + 11 * i, 10 );

		sprintf_s(cbuf, 255, "%d  %.3f", 10*n+8, DAPConfiguration[i].ALT_RATE_DB);
		pMDU->mvprint(9+11*i, 12, cbuf);
		pMDU->Underline( 13 + 11 * i, 12 );
		pMDU->Underline( 14 + 11 * i, 12 );
		pMDU->Underline( 15 + 11 * i, 12 );
		pMDU->Underline( 16 + 11 * i, 12 );
		pMDU->Underline( 17 + 11 * i, 12 );
		sprintf_s(cbuf, 255, "%d   %s", 10*n+9, strings[DAPConfiguration[i].ALT_JET_OPT]);
		pMDU->mvprint(9+11*i, 13, cbuf);
		sprintf_s(cbuf, 255, "%d      %d", 10*n+10, DAPConfiguration[i].ALT_JETS);
		pMDU->mvprint(9+11*i, 14, cbuf);
		pMDU->Underline( 17 + 11 * i, 14 );
		sprintf_s(cbuf, 255, "%d   %.2f", 10*n+11, DAPConfiguration[i].ALT_ON_TIME);
		pMDU->mvprint(9+11*i, 15, cbuf);
		pMDU->Underline( 14 + 11 * i, 15 );
		pMDU->Underline( 15 + 11 * i, 15 );
		pMDU->Underline( 16 + 11 * i, 15 );
		pMDU->Underline( 17 + 11 * i, 15 );
		sprintf_s(cbuf, 255, "%d  %05.2f", 10*n+12, DAPConfiguration[i].ALT_DELAY);
		pMDU->mvprint(9+11*i, 16, cbuf);
		pMDU->Underline( 13 + 11 * i, 16 );
		pMDU->Underline( 14 + 11 * i, 16 );
		pMDU->Underline( 15 + 11 * i, 16 );
		pMDU->Underline( 16 + 11 * i, 16 );
		pMDU->Underline( 17 + 11 * i, 16 );

		sprintf_s(cbuf, 255, "%d %.4f", 10*n+13, DAPConfiguration[i].VERN_ROT_RATE);
		pMDU->mvprint(9+11*i, 18, cbuf);
		pMDU->Underline( 12 + 11 * i, 18 );
		pMDU->Underline( 13 + 11 * i, 18 );
		pMDU->Underline( 14 + 11 * i, 18 );
		pMDU->Underline( 15 + 11 * i, 18 );
		pMDU->Underline( 16 + 11 * i, 18 );
		pMDU->Underline( 17 + 11 * i, 18 );
		sprintf_s(cbuf, 255, "%d %06.3f", 10*n+14, DAPConfiguration[i].VERN_ATT_DB);
		pMDU->mvprint(9+11*i, 19, cbuf);
		pMDU->Underline( 12 + 11 * i, 19 );
		pMDU->Underline( 13 + 11 * i, 19 );
		pMDU->Underline( 14 + 11 * i, 19 );
		pMDU->Underline( 15 + 11 * i, 19 );
		pMDU->Underline( 16 + 11 * i, 19 );
		pMDU->Underline( 17 + 11 * i, 19 );
		sprintf_s(cbuf, 255, "%d  %.3f", 10*n+15, DAPConfiguration[i].VERN_RATE_DB);
		pMDU->mvprint(9+11*i, 20, cbuf);
		pMDU->mvprint( 13 + 11 * i, 20, " " );
		pMDU->Underline( 14 + 11 * i, 20 );
		pMDU->Underline( 15 + 11 * i, 20 );
		pMDU->Underline( 16 + 11 * i, 20 );
		pMDU->Underline( 17 + 11 * i, 20 );
		sprintf_s(cbuf, 255, "%d  %05.3f", 10*n+16, DAPConfiguration[i].VERN_ROT_PLS);
		pMDU->mvprint(9+11*i, 21, cbuf);
		pMDU->Underline( 13 + 11 * i, 21 );
		pMDU->Underline( 14 + 11 * i, 21 );
		pMDU->Underline( 15 + 11 * i, 21 );
		pMDU->Underline( 16 + 11 * i, 21 );
		pMDU->Underline( 17 + 11 * i, 21 );
		sprintf_s(cbuf, 255, "%d  %.3f", 10*n+17, DAPConfiguration[i].VERN_COMP);
		pMDU->mvprint(9+11*i, 22, cbuf);
		pMDU->mvprint( 13 + 11 * i, 22, " " );
		pMDU->Underline( 14 + 11 * i, 22 );
		pMDU->Underline( 15 + 11 * i, 22 );
		pMDU->Underline( 16 + 11 * i, 22 );
		pMDU->Underline( 17 + 11 * i, 22 );
		sprintf_s(cbuf, 255, "%d      %d", 10*n+18, DAPConfiguration[i].VERN_CNTL_ACC);
		pMDU->mvprint(9+11*i, 23, cbuf);
		pMDU->Underline( 17 + 11 * i, 23 );
	}

	pMDU->mvprint( 41, 2, "DAP EDIT" );
	pMDU->mvprint( 41, 3, "3 DAP A" );
	pMDU->mvprint( 41, 4, "4 DAP B" );
	pMDU->mvprint( 41, 5, "5" );

	pMDU->Line( 190, 28, 190, 336 );
	pMDU->Line( 300, 28, 300, 336 );
	pMDU->Line( 410, 112, 410, 336 );
	pMDU->Line( 410, 112, 510, 112 );

	pMDU->mvprint( 41, 9, "NOTCH FLTR" );
	pMDU->mvprint( 43, 10, "ENA  6" );

	pMDU->mvprint( 42, 12, "XJETS ROT" );
	pMDU->mvprint( 44, 13, "ENA  7" );

	pMDU->mvprint( 42, 15, "REBOOST" );
	pMDU->mvprint( 43, 16, "8 CFG" );
	pMDU->mvprint( 43, 17, "9 INTVL" );
}

bool OrbitDAP::OnParseLine(const char* keyword, const char* value)
{
	if(!_strnicmp(keyword, "TGT_ID", 6)) {
		sscanf_s(value, "%d", &TGT_ID);
		return true;
	}
	else if(!_strnicmp(keyword, "BODY_VECT", 9)) {
		sscanf_s(value, "%d", &BODY_VECT);
		return true;
	}
	else if(!_strnicmp(keyword, "P_ANGLE", 7)) {
		sscanf_s(value, "%lf", &P);
		return true;
	}
	else if(!_strnicmp(keyword, "Y_ANGLE", 7)) {
		sscanf_s(value, "%lf", &Y);
		return true;
	}
	else if(!_strnicmp(keyword, "OM_ANGLE", 8)) {
		sscanf_s(value, "%lf", &OM);
		return true;
	}
	else if(!_strnicmp(keyword, "ROLL", 4)) {
		sscanf_s(value, "%lf", &MNVR_OPTION.data[ROLL]);
		return true;
	}
	else if(!_strnicmp(keyword, "PITCH", 5)) {
		sscanf_s(value, "%lf", &MNVR_OPTION.data[PITCH]);
		return true;
	}
	else if(!_strnicmp(keyword, "YAW", 3)) {
		sscanf_s(value, "%lf", &MNVR_OPTION.data[YAW]);
		return true;
	}
	else if(!_strnicmp(keyword, "DAP_MODE", 8)) {
		int nTemp1, nTemp2;
		sscanf_s(value, "%d %d", &nTemp1, &nTemp2);
		DAPSelect = static_cast<DAP_SELECT>(nTemp1);
		DAPMode = static_cast<DAP_MODE>(nTemp2);
		return true;
	}
	else if(!_strnicmp(keyword, "ROT_MODE", 8)) {
		int nTemp[3];
		sscanf_s(value, "%d %d %d", &nTemp[0], &nTemp[1], &nTemp[2]);
		for(int i=0;i<3;i++) RotMode[i] = static_cast<ROT_MODE>(nTemp[i]);
		return true;
	}
	else if(!_strnicmp(keyword, "TRANS_MODE", 10)) {
		int nTemp[3];
		sscanf_s(value, "%d %d %d", &nTemp[0], &nTemp[1], &nTemp[2]);
		for(int i=0;i<3;i++) TransMode[i] = static_cast<TRANS_MODE>(nTemp[i]);
		return true;
	}
	else if(!_strnicmp(keyword, "CONTROL_MODE", 12)) {
		//sscanf(value, "%d", &ControlMode);
		int nTemp;
		sscanf_s(value, "%d", &nTemp);
		if(nTemp==0) DAPControlMode=AUTO;
		else if(nTemp==1) DAPControlMode=INRTL;
		else if(nTemp==2) DAPControlMode=LVLH;
		else if(nTemp==3) {
			DAPControlMode=FREE;
			for(unsigned int i=0;i<3;i++) {
				RotMode[i]=ROT_PULSE;
				RotPulseInProg[i]=false;
			}
		}
		return true;
	}
	else if(!_strnicmp (keyword, "CUR_MNVR", 8)) {
		LoadAttManeuver(value, CurManeuver);
		return true;
	}
	else if(!_strnicmp (keyword, "FUT_MNVR", 8)) {
		LoadAttManeuver(value, FutManeuver);
		return true;
	}
	else if(!_strnicmp (keyword, "FUT_START_TIME", 14)) {
		sscanf_s(value, "%lf", &FutMnvrStartTime);
		return true;
	}

	return false;
}

void OrbitDAP::OnSaveState(FILEHANDLE scn) const
{
	char cbuf[256];
	oapiWriteScenario_int (scn, "TGT_ID", TGT_ID);
	oapiWriteScenario_int (scn, "BODY_VECT", BODY_VECT);
	oapiWriteScenario_float (scn, "ROLL", MNVR_OPTION.data[ROLL]);
	oapiWriteScenario_float (scn, "PITCH", MNVR_OPTION.data[PITCH]);
	oapiWriteScenario_float (scn, "YAW", MNVR_OPTION.data[YAW]);
	oapiWriteScenario_float (scn, "P_ANGLE", P);
	oapiWriteScenario_float (scn, "Y_ANGLE", Y);
	oapiWriteScenario_float (scn, "OM_ANGLE", OM);
	sprintf_s(cbuf, 256, "%d %d", DAPSelect, DAPMode);
	oapiWriteScenario_string (scn, "DAP_MODE", cbuf);
	sprintf_s(cbuf, 256, "%d %d %d", RotMode[0], RotMode[1], RotMode[2]);
	oapiWriteScenario_string (scn, "ROT_MODE", cbuf);
	sprintf_s(cbuf, 256, "%d %d %d", TransMode[0], TransMode[1], TransMode[2]);
	oapiWriteScenario_string (scn, "TRANS_MODE", cbuf);
	sprintf_s(cbuf, 256, "%d", DAPControlMode);
	oapiWriteScenario_string (scn, "CONTROL_MODE", cbuf);
	if(CurManeuver.IsValid) SaveAttManeuver(scn, "CUR_MNVR", CurManeuver);
	if(FutManeuver.IsValid) {
		SaveAttManeuver(scn, "FUT_MNVR", FutManeuver);
		oapiWriteScenario_float(scn, "FUT_START_TIME", FutMnvrStartTime);
	}
}

bool OrbitDAP::GetPBIState(int id) const
{
	switch(id) {
	case 0: //A
		return DAPSelect == A;
	case 1: //B
		return DAPSelect == B;
	case 2: //AUTO
		return DAPControlMode == AUTO;
	case 3: //INRTL
		return DAPControlMode == INRTL;
	case 4: //LVLH
		return DAPControlMode == LVLH;
	case 5: //FREE
		return DAPControlMode == FREE;
	case 6: //PCT
		return PCTActive;
		//return (STS()->PostContactThrusting[1]);
		break;
	case 7: //LOW Z
		break;
	case 8: //HIGH Z
		break;
	case 9: //PRI
		return DAPMode == PRI;
	case 10: //ALT
		return DAPMode == ALT;
	case 11: //VERN
		return DAPMode == VERN;
	case 12: //TRANS X NORM
		return TransMode[0]==NORM;
	case 13: //TRANS Y NORM
		return TransMode[1]==NORM;
	case 14: //TRANS Z NORM
		return TransMode[2]==NORM;
	case 15: //ROT ROLL DISC RATE
		return RotMode[ROLL] == DISC_RATE;
	case 16: //ROT PITCH DISC RATE
		return RotMode[PITCH] == DISC_RATE;
	case 17: //ROT YAW DISC RATE
		return RotMode[YAW] == DISC_RATE;
	case 18: //TRANS X PULSE
		return TransMode[0]==TRANS_PULSE;
	case 19: //TRANS Y PULSE
		return TransMode[1]==TRANS_PULSE;
	case 20: //TRANS Z PULSE
		return TransMode[2]==TRANS_PULSE;
	case 21: //ROT ROLL PULSE
		return RotMode[ROLL] == ROT_PULSE;
	case 22: //ROT PITCH PULSE
		return RotMode[PITCH] == ROT_PULSE;
	case 23: //ROT YAW PULSE
		return RotMode[YAW] == ROT_PULSE;
	}
	return false;
}

void OrbitDAP::ButtonPress(int id)
{
	switch(id) {
	case 0: // A
		DAPSelect = A;
		UpdateDAPParameters();
		break;
	case 1: // B
		DAPSelect = B;
		UpdateDAPParameters();
		break;
	case 2: // AUTO
		DAPControlMode = AUTO;
		if(CurManeuver.IsValid) StartCurManeuver();
		break;
	case 3: // INRTL
		DAPControlMode = INRTL;
		StartManeuver(curM50Matrix, AttManeuver::MNVR);
		break;
	case 4: // LVLH
		DAPControlMode = LVLH;
		StartManeuver(GetCurrentLVLHAttMatrix(), AttManeuver::TRK);
		break;
	case 5: // FREE
		DAPControlMode = FREE;
		for(unsigned int i=0;i<3;i++) {
			RotMode[i]=ROT_PULSE;
			RotPulseInProg[i]=false;
		}		
		if(PCTActive) StopPCT();
		//if(STS()->PostContactThrusting[1]) STS()->TogglePCT();
		//InitializeControlMode();
		break;
	case 6: // PCT
		if(PCTArmed && !PCTActive) {
			StartPCT();
		}
		/*if(STS()->PostContactThrusting[0]) {
			STS()->TogglePCT();
		}*/
		break;
	case 9: // PRI
		if(DAPMode != PRI) {
			DAPMode = PRI;
			UpdateDAPParameters();
		}
		break;
	case 10: // ALT
		if(DAPMode != ALT) {
			DAPMode = ALT;
			UpdateDAPParameters();
		}
		break;
	case 11: // VERN
		if(DAPMode != VERN) {
			DAPMode = VERN;
			UpdateDAPParameters();
		}
		break;
	case 12: // TRANS X NORM
		if(TransMode[0]!=NORM) {
			TransMode[0]=NORM;
			TransPulseInProg[0]=false;
			TransPulseDV.x=0.0;
		}
		break;
	case 13: // TRANS Y NORM
		if(TransMode[1]!=NORM) {
			TransMode[1]=NORM;
			TransPulseInProg[1]=false;
			TransPulseDV.y=0.0;
		}
		break;
	case 14: // TRANS Z NORM
		if(TransMode[2]!=NORM) {
			TransMode[2]=NORM;
			TransPulseInProg[2]=false;
			TransPulseDV.z=0.0;
		}
		break;
	case 15: // ROLL DISC RATE
		if(RotMode[ROLL]!=DISC_RATE) {
			RotMode[ROLL]=DISC_RATE;
			RotPulseInProg[ROLL]=false;
		}
		break;
	case 16: // PITCH DISC RATE
		if(RotMode[PITCH]!=DISC_RATE) {
			RotMode[PITCH]=DISC_RATE;
			RotPulseInProg[PITCH]=false;
		}
		break;
	case 17: // YAW DISC RATE
		if(RotMode[YAW]!=DISC_RATE) {
			RotMode[YAW]=DISC_RATE;
			RotPulseInProg[YAW]=false;
		}
		break;
	case 18: // X PULSE
		if(TransMode[0]!=TRANS_PULSE && (GetMajorMode()==201 || GetMajorMode()==202)) {
			TransMode[0]=TRANS_PULSE;
			TransPulseInProg[0]=false;
			TransPulseDV.x=0.0;
		}
		break;
	case 19: // Y PULSE
		if(TransMode[1]!=TRANS_PULSE  && (GetMajorMode()==201 || GetMajorMode()==202)) {
			TransMode[1]=TRANS_PULSE;
			TransPulseInProg[1]=false;
			TransPulseDV.y=0.0;
		}
		break;
	case 20: // Z PULSE
		if(TransMode[2]!=TRANS_PULSE  && (GetMajorMode()==201 || GetMajorMode()==202)) {
			TransMode[2]=TRANS_PULSE;
			TransPulseInProg[2]=false;
			TransPulseDV.z=0.0;
		}
		break;
	case 21: // ROLL PULSE
		if(RotMode[ROLL]!=ROT_PULSE) {
			RotMode[ROLL]=ROT_PULSE;
			RotPulseInProg[ROLL] = false;
		}
		break;
	case 22: // PITCH PULSE
		if(RotMode[PITCH]!=ROT_PULSE) {
			RotMode[PITCH]=ROT_PULSE;
			RotPulseInProg[PITCH] = false;
		}
		break;
	case 23: // YAW PULSE
		if(RotMode[YAW]!=ROT_PULSE) {
			RotMode[YAW]=ROT_PULSE;
			RotPulseInProg[YAW] = false;
		}
		break;
	}
}

void OrbitDAP::StartPCT()
{
	PCTActive=true;
	PCTStartTime=oapiGetSimTime();
	DAPMode=PRI; //PRI
	DAPControlMode=FREE;

	//set Body Flap PBIs
	port_PCTActive[0].SetLine();
	port_PCTActive[1].SetLine();
	//BodyFlapAutoOut.SetLine();
	//BodyFlapManOut.SetLine();
}

void OrbitDAP::StopPCT()
{
	PCTActive=false;
	DAPSelect=A;
	DAPMode=VERN;
	// TODO: mode to A9/B9 DAP CONFIGs

	//stop firing RCS jets
	//SetThrusterGroupLevel(thg_transup, 0.0);

	//set Body Flap PBIs
	port_PCTActive[0].ResetLine();
	port_PCTActive[1].ResetLine();
	//BodyFlapAutoOut.ResetLine();
	//BodyFlapManOut.ResetLine();
}

void OrbitDAP::PCTControl(double SimT)
{
	double dT=SimT-PCTStartTime;

	//prevent translation thrusters except (up group) from firing
	TransThrusterCommands[0].ResetLine();
	TransThrusterCommands[1].ResetLine();

	//fire thrusters as appropriate
	if(dT<=PCT_STAGE1) TransThrusterCommands[2].SetLine(-0.5f);
	else if(dT<=PCT_STAGE2) TransThrusterCommands[2].ResetLine();
	else if(dT<=PCT_STAGE3) TransThrusterCommands[2].SetLine(-0.5f);
	else {
		TransThrusterCommands[2].ResetLine();
		StopPCT();
	}
}

void OrbitDAP::UpdateDAPParameters()
{
	TransPulse=DAPConfiguration[DAPSelect].PRI_TRAN_PLS*fps_to_ms; //same for all modes
	if(DAPMode==PRI) {
		degRotRate=DAPConfiguration[DAPSelect].PRI_ROT_RATE;
		degAttDeadband=DAPConfiguration[DAPSelect].PRI_ATT_DB;
		degRateDeadband=DAPConfiguration[DAPSelect].PRI_RATE_DB;
		degRotPulse=DAPConfiguration[DAPSelect].PRI_ROT_PLS;

		if(DAPConfiguration[DAPSelect].PRI_P_OPTION!=0)
			Torque.data[PITCH]=0.5*ORBITER_PITCH_TORQUE;
		else Torque.data[PITCH]=ORBITER_PITCH_TORQUE;
		if(DAPConfiguration[DAPSelect].PRI_Y_OPTION!=0)
			Torque.data[YAW]=0.5*ORBITER_YAW_TORQUE;
		else Torque.data[YAW]=ORBITER_YAW_TORQUE;
		Torque.data[ROLL]=ORBITER_ROLL_TORQUE;
	}
	else if(DAPMode==ALT) {
		degRotRate=DAPConfiguration[DAPSelect].PRI_ROT_RATE;
		degAttDeadband=DAPConfiguration[DAPSelect].PRI_ATT_DB;
		degRateDeadband=DAPConfiguration[DAPSelect].ALT_RATE_DB;
		degRotPulse=DAPConfiguration[DAPSelect].PRI_ROT_PLS;
		if(DAPConfiguration[DAPSelect].ALT_JET_OPT==2) {
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
	else if(DAPMode==VERN) {
		degRotRate=DAPConfiguration[DAPSelect].VERN_ROT_RATE;
		degAttDeadband=DAPConfiguration[DAPSelect].VERN_ATT_DB;
		degRateDeadband=DAPConfiguration[DAPSelect].VERN_RATE_DB;
		degRotPulse=DAPConfiguration[DAPSelect].VERN_ROT_PLS;
		Torque.data[PITCH]=0.1*ORBITER_PITCH_TORQUE;
		Torque.data[YAW]=0.1*ORBITER_YAW_TORQUE;
		Torque.data[ROLL]=0.1*ORBITER_ROLL_TORQUE;
	}
}

double OrbitDAP::CalcManeuverCompletionTime(const MATRIX3& curM50Matrix, const MATRIX3& tgtLVLHMatrix, const MATRIX3& curLVLHMatrix, double degOrbitalRate) const
{
	double mnvrTime = 0.0;
	double lastMnvrTime = 0.0;
	int counter = 0;
	VECTOR3 Axis;
	MATRIX3 PYR;
	do {
		counter++;
		lastMnvrTime=mnvrTime;
		if(mnvrTime < 0) {
			mnvrTime=0.0;
		}
		
		double rotationAngle = degOrbitalRate*mnvrTime;
		MATRIX3 LVLHRotation;
		GetRotMatrixY(-rotationAngle*RAD, LVLHRotation);
		MATRIX3 tgtM50Matrix = mul(mul(curLVLHMatrix, LVLHRotation), tgtLVLHMatrix);
				
		PYR = GetRotationErrorMatrix(curM50Matrix, tgtM50Matrix);
		double Angle=CalcEulerAngle(PYR, Axis);
		mnvrTime=(Angle*DEG)/degRotRate;
	} while(abs(mnvrTime-lastMnvrTime)>0.05 && counter<50);
	return max(mnvrTime, 0.0);
}

MATRIX3 OrbitDAP::GetCurrentLVLHRefMatrix() const
{
	VECTOR3 pos, vel;
	pStateVector->GetCurrentStateVectorsM50(pos, vel);
	return Transpose(GetGlobalToLVLHMatrix(pos, vel));
}

MATRIX3 OrbitDAP::GetCurrentLVLHAttMatrix() const
{
	return GetRotationErrorMatrix(GetCurrentLVLHRefMatrix(), curM50Matrix);
}

VECTOR3 OrbitDAP::GetAttitudeErrors( void ) const
{
	return ATT_ERR;
}

bool OrbitDAP::GetTimeToAttitude( double& time ) const
{
	if (ManeuverStatus != MNVR_IN_PROGRESS) return false;
	time = max( mnvrCompletionMET - STS()->GetMET(), 0 );
	return true;
}

};
