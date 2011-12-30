#include "OrbitDAP.h"
#include "IDP.h"
#include <UltraMath.h>
#include "../ParameterValues.h"

namespace dps
{

void SaveAttManeuver(FILEHANDLE scn, char* item, const AttManeuver& maneuver)
{
	char cbuf[255];
	if(maneuver.Type==AttManeuver::MNVR) {
		sprintf_s(cbuf, 255, "%d %f %f %f", maneuver.Type,
			maneuver.radTargetAttOrbiter.data[PITCH], maneuver.radTargetAttOrbiter.data[YAW], maneuver.radTargetAttOrbiter.data[ROLL]);
	}
	else if(maneuver.Type==AttManeuver::TRK) {
		sprintf_s(cbuf, 255, "%d %f %f %f", maneuver.Type,
			maneuver.radTargetLVLHAtt.data[PITCH], maneuver.radTargetLVLHAtt.data[YAW], maneuver.radTargetLVLHAtt.data[ROLL]);
	}
	oapiWriteScenario_string(scn, item, cbuf);
}

void LoadAttManeuver(const char* value, AttManeuver& maneuver)
{
	int nTemp;
	VECTOR3 vTemp;
	sscanf(value, "%d%lf%lf%lf", &nTemp, &vTemp.data[PITCH], &vTemp.data[YAW], &vTemp.data[ROLL]);

	if(nTemp == AttManeuver::MNVR) {
		maneuver.IsValid = true;
		maneuver.Type = AttManeuver::MNVR;
		maneuver.radTargetAttOrbiter = vTemp;
	}
	else if(nTemp == AttManeuver::TRK) {
		maneuver.IsValid = true;
		maneuver.Type = AttManeuver::TRK;
		maneuver.radTargetLVLHAtt = vTemp;
	}
}

OrbitDAP::OrbitDAP(SimpleGPCSystem* pGPC)
: SimpleGPCSoftware(pGPC, "OrbitDAP"),
OMSTVCControlP(3.5, 0.0, 0.75), OMSTVCControlY(4.0, 0.0, 0.75), OMSTVCControlR(3.5, 0.0, 0.75),
ControlMode(RCS),
DAPMode(PRI), DAPSelect(A), DAPControlMode(INRTL), editDAP(-1),
ManeuverStatus(MNVR_OFF),
bFirstStep(true), lastStepDeltaT(1.0),
PCTActive(false)
{
	OMSTrim = _V(0.0, 0.0, 0.0);
	degReqdRates = _V(0.0, 0.0, 0.0);
	TransPulseDV = _V(0.0, 0.0, 0.0);
	ATT_ERR = _V(0.0, 0.0, 0.0);

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

		//Initialize DAP Config
		DAPConfiguration[i].PRI_ROT_RATE=0.2;
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
		DAPConfiguration[i].VERN_ROT_RATE=0.2;
		DAPConfiguration[i].VERN_ATT_DB=1.0;
		DAPConfiguration[i].VERN_RATE_DB=0.020;
		DAPConfiguration[i].VERN_ROT_PLS=0.01;
		DAPConfiguration[i].VERN_COMP=0.0;
		DAPConfiguration[i].VERN_CNTL_ACC=0;
	}
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

void OrbitDAP::ManeuverToLVLHAttitude(const VECTOR3& degLVLHAtt)
{
	LoadCurLVLHManeuver(degLVLHAtt*RAD);
}

void OrbitDAP::ManeuverToINRTLAttitude(const VECTOR3& degINRTLAtt)
{
	VECTOR3 radOrbiterAtt = ConvertAnglesBetweenM50AndOrbiter(degINRTLAtt*RAD, true);
	LoadCurINRTLManeuver(radOrbiterAtt);
}

void OrbitDAP::LoadCurLVLHManeuver(const VECTOR3& radTargetLVLHAtt)
{
	CurManeuver.IsValid = true;
	//CurManeuver.LVLHTgtOrientationMatrix = RotMatrix;
	CurManeuver.radTargetLVLHAtt = radTargetLVLHAtt;
	CurManeuver.Type = AttManeuver::TRK;
	if(DAPControlMode == AUTO) StartLVLHManeuver(radTargetLVLHAtt);
}

void OrbitDAP::LoadFutLVLHManeuver(const VECTOR3& radTargetLVLHAtt)
{
	FutManeuver.IsValid = true;
	FutManeuver.radTargetLVLHAtt = radTargetLVLHAtt;
	FutManeuver.Type = AttManeuver::TRK;
}

void OrbitDAP::LoadCurINRTLManeuver(const VECTOR3& radTargetAttOrbiter)
{
	CurManeuver.IsValid = true;
	CurManeuver.radTargetAttOrbiter = radTargetAttOrbiter;
	CurManeuver.Type = AttManeuver::MNVR;
	if(DAPControlMode == AUTO) StartINRTLManeuver(radTargetAttOrbiter);
}

void OrbitDAP::LoadFutINRTLManeuver(const VECTOR3& radTargetAttOrbiter)
{
	FutManeuver.IsValid = true;
	FutManeuver.radTargetAttOrbiter = radTargetAttOrbiter;
	FutManeuver.Type = AttManeuver::MNVR;
}

void OrbitDAP::StartCurManeuver()
{
	if(CurManeuver.IsValid) {
		if(CurManeuver.Type == AttManeuver::MNVR) StartINRTLManeuver(CurManeuver.radTargetAttOrbiter);
		else if(CurManeuver.Type == AttManeuver::TRK) StartLVLHManeuver(CurManeuver.radTargetLVLHAtt);
	}
}

void OrbitDAP::StartINRTLManeuver(const VECTOR3& radTargetAttOrbiter)
{
	ActiveManeuver.IsValid = true;
	ActiveManeuver.radTargetAttOrbiter = radTargetAttOrbiter;
	ActiveManeuver.Type = AttManeuver::MNVR;
	REQD_ATT = ConvertAnglesBetweenM50AndOrbiter(radTargetAttOrbiter)*DEG;
	ManeuverStatus = MNVR_IN_PROGRESS;
}

void OrbitDAP::StartLVLHManeuver(const VECTOR3& radTargetLVLHAtt)
{
	sprintf_s(oapiDebugString(), 255, "Starting LVLH mnvr: %f %f %f",
		radTargetLVLHAtt.data[PITCH]*DEG, radTargetLVLHAtt.data[YAW]*DEG, radTargetLVLHAtt.data[ROLL]*DEG);
	ActiveManeuver.IsValid = true;
	ActiveManeuver.radTargetLVLHAtt = radTargetLVLHAtt;
	ActiveManeuver.Type = AttManeuver::TRK;
	ManeuverStatus = MNVR_STARTING;
	lastUpdateTime= 0.0;
	mnvrCompletionMET = STS()->GetMET();
}

bool OrbitDAP::GetRHCRequiredRates()
{
	bool outOfDetent = false;
	for(unsigned int i=0;i<3;i++) {
		if(abs(RHCInput[i].GetVoltage()) > RHC_DETENT) {
			outOfDetent = true;
			if(abs(RHCInput[i].GetVoltage()) < RHC_SOFT_STOP) {
				if(RotMode[i]==DISC_RATE) {
					degReqdRates.data[i] = degRotRate*sign(RHCInput[i].GetVoltage()); //DISC RATE
				}
				else if(!RotPulseInProg[i]) {
					degReqdRates.data[i] = degReqdRates.data[i]+degRotPulse*sign(RHCInput[i].GetVoltage()); //PULSE
					RotPulseInProg[i]=true;
				}
			}
			else {
				degReqdRates.data[i] = 1000*sign(RHCInput[i].GetVoltage());
			}
		}
	}
	return outOfDetent;
}

void OrbitDAP::HandleTHCInput(double DeltaT)
{
	VECTOR3 ThrusterLevel=_V(0.0, 0.0, 0.0);

	for(int i=0;i<3;i++) {
		if(abs(THCInput[i].GetVoltage())>0.01) {
			//if PCT is in progress, disable it when THC is moved out of detent
			if(PCTActive) StopPCT();

			if(TransMode[i]==NORM) {
				TransThrusterCommands[i].SetLine(static_cast<float>( sign(THCInput[i].GetVoltage()) ));
			}
			else if(TransMode[i]==TRANS_PULSE && !TransPulseInProg[i]) {
				TransPulseInProg[i]=true;
				TransPulseDV.data[i] = TransPulse*sign(THCInput[i].GetVoltage());
			}
			else {
				TransThrusterCommands[i].ResetLine();
			}
		}
		else {
			TransThrusterCommands[i].ResetLine();
		}

		//sprintf_s(oapiDebugString(), 255, "Pulse DV: %f %f %f", TransPulseDV.x, TransPulseDV.y, TransPulseDV.z);
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
				if(abs(THCInput[i].GetVoltage())<0.01) {
					TransPulseInProg[i]=false;
				}
				TransThrusterCommands[i].ResetLine();
			}
		}
	}
}

void OrbitDAP::CalcEulerAxisRates()
{	
	VECTOR3 RotationAxis;
	double RotationAngle=CalcEulerAngle(IdentityMatrix, attErrorMatrix, RotationAxis);
	//Rates=RotationAxis*-RotRate;
	for(unsigned int i=0;i<3;i++) {
		if(DAPControlMode==AUTO || RotMode[i]==DISC_RATE) {
			degReqdRates.data[i]=RotationAxis.data[i]*-degRotRate;
			if(abs(ATT_ERR.data[i]) <= NullStartAngle(abs(radAngularVelocity.data[i]), OrbiterMass, PMI.data[i], Torque.data[i])) {
				degReqdRates.data[i] = 0.0;
				RotatingAxis[i] = false;
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
	for(unsigned int i=0;i<3;i++) {
		if(DAPControlMode==AUTO || RotMode[i]==DISC_RATE) {
			degReqdRates.data[i]=0.0;
			if((RotatingAxis[i] || abs(ATT_ERR.data[i])>degAttDeadband)) {
				if(abs(ATT_ERR.data[i])<0.05) RotatingAxis[i]=false;
				else {
					RotatingAxis[i]=true;
					if(abs(ATT_ERR.data[i]) <= NullStartAngle(abs(radAngularVelocity.data[i]), OrbiterMass, PMI.data[i], Torque.data[i])) {
						degReqdRates.data[i] = 0.0;
					}
					else {
						degReqdRates.data[i] = -sign(ATT_ERR.data[i])*range(degRotRate/10.0, abs(ATT_ERR.data[i])/5.0, degRotRate);
					}
				}
			}
		}
		else { // PULSE mode; maintain current rate
			degReqdRates.data[i] = degAngularVelocity.data[i];
		}
	}

	if(oapiGetTimeAcceleration()>10.0)
	{
		degReqdRates=degReqdRates/(2.0*oapiGetTimeAcceleration()/10.0);
	}
	// add null rates to maintain attitude in rotation frame
	for(int i=0;i<3;i++) {
		if(DAPControlMode==AUTO || RotMode[i]==DISC_RATE) degReqdRates.data[i]+=degNullRatesLocal.data[i];
	}
}

void OrbitDAP::SetRates(const VECTOR3 &degRates, double DeltaT)
{
	const VECTOR3 PRI_LIMITS = _V(0.01, 0.01, 0.01);
	const VECTOR3 VERN_LIMITS = _V(0.0015, 0.0015, 0.0015);
	//double dDiff;
	VECTOR3 Error = degRates-degAngularVelocity;
	Error.data[YAW] = -Error.data[YAW]; // temporary
	Error.data[ROLL] = -Error.data[ROLL];

	VECTOR3 Limits;
	double MaxThrusterLevel;
	double timeAcc = max(1.0, oapiGetTimeAcceleration());
	if(DAPMode != VERN) { // PRI/ALT
		for(unsigned int i=0;i<3;i++) {
			Limits.data[i] = max(PRI_LIMITS.data[i], 0.5*RotationRateChange(OrbiterMass, PMI.data[i], Torque.data[i]/timeAcc, DeltaT));
		}
		MaxThrusterLevel = 1.0/timeAcc;
	}
	else { // VERN
		for(unsigned int i=0;i<3;i++) {
			Limits.data[i] = max(VERN_LIMITS.data[i], 0.5*RotationRateChange(OrbiterMass, PMI.data[i], 0.1*Torque.data[i]/timeAcc, DeltaT));
		}
		MaxThrusterLevel = 0.1/timeAcc;
	}
	//if(ManeuverStatus==MNVR_IN_PROGRESS) Limits=Limits*5.0;

	for(unsigned int i=0;i<3;i++) {
		if(abs(Error.data[i])>Limits.data[i]) {
			RotThrusterCommands[i].SetLine(static_cast<float>(MaxThrusterLevel*sign(Error.data[i])));
		}
		else {
			RotThrusterCommands[i].ResetLine();
			//If RHC is out of detent, pretend pulse is still in progress
			if(abs(RHCInput[i].GetVoltage())<RHC_DETENT) RotPulseInProg[i]=false;
		}
	}
}

void OrbitDAP::OMSTVC(const VECTOR3 &Rates, double SimDT)
{
	VECTOR3 CurrentRates=radAngularVelocity*DEG;
	double pitchDelta=Rates.data[PITCH]-CurrentRates.data[PITCH]; //if positive, vessel is pitching down
	double yawDelta=Rates.data[YAW]-CurrentRates.data[YAW]; //if positive, vessel is rotating to right
	double rollDelta=Rates.data[ROLL]-CurrentRates.data[ROLL]; //if positive, vessel is rolling to left
	bool RCSWraparound=(abs(rollDelta)>0.5 || abs(pitchDelta)>0.25 || abs(yawDelta)>0.25);

	sprintf_s(oapiDebugString(), 255, "OMSTVC: %f %f %f", pitchDelta, yawDelta, rollDelta);

	double dPitch=OMSTVCControlP.Step(pitchDelta, SimDT);
	double dYaw=OMSTVCControlY.Step(yawDelta, SimDT);

	if(ControlMode!=RIGHT_OMS) //left OMS engine burning
	{
		//double dPitch=2.5*pitchDelta, dYaw=3.0*yawDelta; //changes in gimbal position from default (trim) angle
		double Pitch=OMSTrim.data[0]+dPitch, Yaw=OMSTrim.data[1]+dYaw;
		if(ControlMode==BOTH_OMS) Pitch-=rollDelta;
		if(!GimbalOMS(LEFT, Pitch, Yaw)) RCSWraparound=true;
	}
	if(ControlMode!=LEFT_OMS) //right OMS engine burning
	{
		//double dPitch=2.5*pitchDelta, dYaw=3.0*yawDelta; //changes in gimbal position from default (trim) angle
		double Pitch=OMSTrim.data[0]+dPitch, Yaw=OMSTrim.data[2]+dYaw;
		if(ControlMode==BOTH_OMS) Pitch+=rollDelta;
		if(!GimbalOMS(RIGHT, Pitch, Yaw)) RCSWraparound=true;
	}
	//sprintf_s(oapiDebugString(), 255, "OMS TVC: %f %f %f %f dPitch: %f", OMSGimbal[0][0], OMSGimbal[0][1], OMSGimbal[1][0], OMSGimbal[1][1], pitchDelta);

	if(RCSWraparound) SetRates(Rates, SimDT);
	else if(ControlMode!=BOTH_OMS) SetRates(_V(0.0, 0.0, Rates.data[ROLL]), SimDT); //for single-engine burns, use RCS for roll control
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
	degAngularVelocity = radAngularVelocity*DEG;
	STS()->GetGlobalOrientation(radCurrentOrbiterAtt);
	CUR_ATT = ConvertAnglesBetweenM50AndOrbiter(radCurrentOrbiterAtt)*DEG;
	STS()->GetGlobalPos(GlobalPos);	
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
	
	for(int i=0;i<24;i++) {
		PBI_state[i] = GetPBIState(i);
		if(PBI_state[i]) PBI_output[i].SetLine();
		else PBI_output[i].ResetLine();
	}	

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

	pBundle=STS()->BundleManager()->CreateBundle("HC_INPUT", 16);
	for(int i=0;i<3;i++) {
		RHCInput[i].Connect(pBundle, i);
		THCInput[i].Connect(pBundle, i+3);
	}

	pBundle = BundleManager()->CreateBundle("CSS_CONTROLS", 4);
	PitchAuto.Connect(pBundle, 0);
	PitchCSS.Connect(pBundle, 1);
	RollYawAuto.Connect(pBundle, 2);
	RollYawCSS.Connect(pBundle, 3);

	pBundle=BundleManager()->CreateBundle("SBDBKTHROT_CONTROLS", 16);
	PCTArmed.Connect(pBundle, 0);

	pBundle=BundleManager()->CreateBundle("BODYFLAP_CONTROLS", 16);
	BodyFlapAuto.Connect(pBundle, 0);
	port_PCTActive[0].Connect(pBundle, 0);
	port_PCTActive[1].Connect(pBundle, 1);

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
			StartINRTLManeuver(radCurrentOrbiterAtt);
		}
		else if(DAPControlMode == LVLH) {
			StartLVLHManeuver(GetCurrentLVLHAttitude());
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

	/*if(ControlMode == RCS) {
		if(PCTArmed && BodyFlapAuto && !PCTActive) StartPCT();
		else if(!BodyFlapAuto && PCTActive) StopPCT();
		if(!PCTActive) HandleTHCInput(DeltaT);
		else PCTControl(SimT);

		if(GetRHCRequiredRates()) {
			if(DAPControlMode == AUTO) {
				DAPControlMode = INRTL;
				StartINRTLManeuver(radCurrentOrbiterAtt);
			}
			else if(DAPControlMode == INRTL) {
				StartINRTLManeuver(radCurrentOrbiterAtt);
			}
			else if(DAPControlMode == LVLH) {
				StartLVLHManeuver(GetCurrentLVLHAttitude());
			}
			ATT_ERR = _V(0.0, 0.0, 0.0);
		}
		else if(DAPControlMode != FREE) { // if DAP is in FREE, we only care about RHC input
			//sprintf_s(oapiDebugString(), 255, "RHC in detent");
			// calc rates for DAP
			VECTOR3 radTargetAttOrbiter;
			VECTOR3 degNullRatesLocal = _V(0.0, 0.0, 0.0);
			if(ActiveManeuver.Type == AttManeuver::TRK) {
				VECTOR3 radLastTgtAttOrbiter = ActiveManeuver.radTargetAttOrbiter;
				MATRIX3 reqdAttM50Matrix = ConvertLVLHAnglesToM50Matrix(ActiveManeuver.radTargetLVLHAtt);
				MATRIX3 reqdAttOrbiterMatrix = ConvertMatrixBetweenM50AndOrbiter(reqdAttM50Matrix, true);
				ActiveManeuver.radTargetAttOrbiter = GetAnglesFromMatrix(reqdAttOrbiterMatrix);
				REQD_ATT = GetAnglesFromMatrix(reqdAttM50Matrix)*DEG;

				if(ManeuverStatus == MNVR_STARTING) {
					ManeuverStatus = MNVR_IN_PROGRESS;
					lastUpdateTime = -100.0;
				}
				else {
					degNullRatesLocal = (ConvertOrbiterAnglesToLocal(ActiveManeuver.radTargetAttOrbiter)-ConvertOrbiterAnglesToLocal(radLastTgtAttOrbiter))/lastStepDeltaT;
					degNullRatesLocal*=DEG;
					VECTOR3 radNullRatesOrbiter = (ActiveManeuver.radTargetAttOrbiter-radLastTgtAttOrbiter)/lastStepDeltaT;

					if((STS()->GetMET()-lastUpdateTime) > 60.0) {
						mnvrCompletionMET = STS()->GetMET() + CalcManeuverCompletionTime(ActiveManeuver.radTargetAttOrbiter, radNullRatesOrbiter);
						lastUpdateTime = STS()->GetMET();
					}
					/*sprintf_s(oapiDebugString(), 255, "Target LVLH: %f %f %f TargetAttOrbiter: %f %f %f",
						ActiveManeuver.radTargetLVLHAtt.data[PITCH]*DEG, ActiveManeuver.radTargetLVLHAtt.data[YAW]*DEG, ActiveManeuver.radTargetLVLHAtt.data[ROLL]*DEG,
						ActiveManeuver.radTargetAttOrbiter.data[PITCH]*DEG, ActiveManeuver.radTargetAttOrbiter.data[YAW]*DEG, ActiveManeuver.radTargetAttOrbiter.data[ROLL]*DEG);*/
					/*sprintf_s(oapiDebugString(), 255, "Null Rates: %f %f %f %f %f %f Time: %f",
						degNullRatesLocal.data[PITCH], degNullRatesLocal.data[YAW], degNullRatesLocal.data[ROLL],
						DEG*radNullRatesOrbiter.data[PITCH], DEG*radNullRatesOrbiter.data[YAW], DEG*radNullRatesOrbiter.data[ROLL],
						mnvrCompletionMET);*

					if(ManeuverStatus == MNVR_IN_PROGRESS) {
						radTargetAttOrbiter = ActiveManeuver.radTargetAttOrbiter + radNullRatesOrbiter*(mnvrCompletionMET-STS()->GetMET());
					}
					else {
						radTargetAttOrbiter = ActiveManeuver.radTargetAttOrbiter;
					}
				}
				// calculate null rates
			}
			else {
				radTargetAttOrbiter = ActiveManeuver.radTargetAttOrbiter;
			}
			attErrorMatrix=CalcPitchYawRollRotMatrix(radTargetAttOrbiter);
			ATT_ERR=GetAnglesFromMatrix(attErrorMatrix)*DEG;
			if(ManeuverStatus == MNVR_COMPLETE) {
				CalcMultiAxisRates(degNullRatesLocal);
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
			//sprintf_s(oapiDebugString(), 255, "Tgt Rates: P: %f Y: %f R: %f Torque: P: %f Y: %f R: %f",
				//degReqdRates.data[PITCH], degReqdRates.data[YAW], degReqdRates.data[ROLL], Torque.data[PITCH], Torque.data[YAW], Torque.data[ROLL]);
		}
		else { // FREE
			degReqdRates = degAngularVelocity;
		}
		//SetRates(STS()->ReqdRates, DeltaT);
		SetRates(degReqdRates, DeltaT);
	}
	else {
		OMSTVC(STS()->ReqdRates, DeltaT); // for the moment, use data calculated by GPC_old code
	}*/

	if(PCTArmed && BodyFlapAuto && !PCTActive) StartPCT();
	else if(!BodyFlapAuto && PCTActive) StopPCT();
	if(!PCTActive) HandleTHCInput(DeltaT);
	else PCTControl(SimT);

	if(GetRHCRequiredRates()) {
		if(DAPControlMode == AUTO) {
			DAPControlMode = INRTL;
			StartINRTLManeuver(radCurrentOrbiterAtt);
		}
		else if(DAPControlMode == INRTL) {
			StartINRTLManeuver(radCurrentOrbiterAtt);
		}
		else if(DAPControlMode == LVLH) {
			StartLVLHManeuver(GetCurrentLVLHAttitude());
		}
		ATT_ERR = _V(0.0, 0.0, 0.0);
	}
	else if(DAPControlMode != FREE) { // if DAP is in FREE, we only care about RHC input
		//sprintf_s(oapiDebugString(), 255, "RHC in detent");
		// calc rates for DAP
		VECTOR3 radTargetAttOrbiter;
		VECTOR3 degNullRatesLocal = _V(0.0, 0.0, 0.0);
		if(ActiveManeuver.Type == AttManeuver::TRK) {
			VECTOR3 radLastTgtAttOrbiter = ActiveManeuver.radTargetAttOrbiter;
			MATRIX3 reqdAttM50Matrix = ConvertLVLHAnglesToM50Matrix(ActiveManeuver.radTargetLVLHAtt);
			MATRIX3 reqdAttOrbiterMatrix = ConvertMatrixBetweenM50AndOrbiter(reqdAttM50Matrix, true);
			ActiveManeuver.radTargetAttOrbiter = GetXYZAnglesFromMatrix(reqdAttOrbiterMatrix);
			REQD_ATT = GetXYZAnglesFromMatrix(reqdAttM50Matrix)*DEG;

			if(ManeuverStatus == MNVR_STARTING) {
				ManeuverStatus = MNVR_IN_PROGRESS;
				lastUpdateTime = -100.0;
			}
			else {
				degNullRatesLocal = (ConvertOrbiterAnglesToLocal(ActiveManeuver.radTargetAttOrbiter)-ConvertOrbiterAnglesToLocal(radLastTgtAttOrbiter))/lastStepDeltaT;
				degNullRatesLocal*=DEG;
				VECTOR3 radNullRatesOrbiter = (ActiveManeuver.radTargetAttOrbiter-radLastTgtAttOrbiter)/lastStepDeltaT;

				if((STS()->GetMET()-lastUpdateTime) > 60.0) {
					mnvrCompletionMET = STS()->GetMET() + CalcManeuverCompletionTime(ActiveManeuver.radTargetAttOrbiter, radNullRatesOrbiter);
					lastUpdateTime = STS()->GetMET();
				}
				/*sprintf_s(oapiDebugString(), 255, "Target LVLH: %f %f %f TargetAttOrbiter: %f %f %f",
				ActiveManeuver.radTargetLVLHAtt.data[PITCH]*DEG, ActiveManeuver.radTargetLVLHAtt.data[YAW]*DEG, ActiveManeuver.radTargetLVLHAtt.data[ROLL]*DEG,
				ActiveManeuver.radTargetAttOrbiter.data[PITCH]*DEG, ActiveManeuver.radTargetAttOrbiter.data[YAW]*DEG, ActiveManeuver.radTargetAttOrbiter.data[ROLL]*DEG);*/
				/*sprintf_s(oapiDebugString(), 255, "Null Rates: %f %f %f %f %f %f Time: %f",
				degNullRatesLocal.data[PITCH], degNullRatesLocal.data[YAW], degNullRatesLocal.data[ROLL],
				DEG*radNullRatesOrbiter.data[PITCH], DEG*radNullRatesOrbiter.data[YAW], DEG*radNullRatesOrbiter.data[ROLL],
				mnvrCompletionMET);*/

				if(ManeuverStatus == MNVR_IN_PROGRESS) {
					radTargetAttOrbiter = ActiveManeuver.radTargetAttOrbiter + radNullRatesOrbiter*(mnvrCompletionMET-STS()->GetMET());
				}
				else {
					radTargetAttOrbiter = ActiveManeuver.radTargetAttOrbiter;
				}
			}
			// calculate null rates
		}
		else {
			radTargetAttOrbiter = ActiveManeuver.radTargetAttOrbiter;
		}
		attErrorMatrix=CalcPitchYawRollRotMatrix(radTargetAttOrbiter);
		ATT_ERR=GetXYZAnglesFromMatrix(attErrorMatrix)*DEG;
		if(ManeuverStatus == MNVR_COMPLETE) {
			CalcMultiAxisRates(degNullRatesLocal);
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
		//sprintf_s(oapiDebugString(), 255, "Tgt Rates: P: %f Y: %f R: %f Torque: P: %f Y: %f R: %f",
		//degReqdRates.data[PITCH], degReqdRates.data[YAW], degReqdRates.data[ROLL], Torque.data[PITCH], Torque.data[YAW], Torque.data[ROLL]);
	}
	else { // FREE
		degReqdRates = degAngularVelocity;
	}
	//SetRates(STS()->ReqdRates, DeltaT);
	if(ControlMode == RCS) SetRates(degReqdRates, DeltaT);
	else OMSTVC(degReqdRates, DeltaT); // for the moment, use data calculated by GPC_old code

	// set entry DAP mode PBIs to OFF
	PitchAuto.ResetLine();
	PitchCSS.ResetLine();
	RollYawAuto.ResetLine();
	RollYawCSS.ResetLine();

	lastStepDeltaT = DeltaT;
}

bool OrbitDAP::OnMajorModeChange(unsigned int newMajorMode)
{
	if(newMajorMode >= 104 && newMajorMode <= 303) {
		// perform initialization
		return true;
	}
	return false;
}

bool OrbitDAP::ItemInput(int spec, int item, const char* Data)
{
	if(GetMajorMode()!=201) return false;

	if(spec==dps::MODE_UNDEFINED) {
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
			VECTOR3 radTargetAtt = ConvertAnglesBetweenM50AndOrbiter(MNVR_OPTION*RAD, true);
			double startTime = START_TIME[0]*86400.0+ START_TIME[1]*3600.0 + START_TIME[2]*60.0 + START_TIME[3];
			if(startTime <= STS()->GetMET()) {
				LoadCurINRTLManeuver(radTargetAtt);
			}
			else {
				FutMnvrStartTime = startTime;
				LoadFutINRTLManeuver(radTargetAtt);
			}
		}
		else if(item==19) {
			VECTOR3 radTargetAtt = ConvertPYOMToBodyAngles(P*RAD, Y*RAD, OM*RAD);
			double startTime = START_TIME[0]*86400.0 + START_TIME[1]*3600.0 + START_TIME[2]*60.0 + START_TIME[3];
			if(startTime <= STS()->GetMET()) {
				if(TGT_ID == 2) {
					LoadCurLVLHManeuver(radTargetAtt);
				}
			}
			else {
				FutMnvrStartTime = startTime;
				if(TGT_ID == 2) {
					LoadFutLVLHManeuver(radTargetAtt);
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
			StartINRTLManeuver(radCurrentOrbiterAtt);
		}
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
	if(GetMajorMode()==201) {
		if(spec==dps::MODE_UNDEFINED) {
			PaintUNIVPTGDisplay(pMDU);
			return true;
		}
		else if(spec==20) {
			PaintDAPCONFIGDisplay(pMDU);
			return true;
		}
	}
	return false;
}

void OrbitDAP::PaintUNIVPTGDisplay(vc::MDU* pMDU) const
{
	char cbuf[255];
	PrintCommonHeader("UNIV PTG", pMDU);

	pMDU->mvprint(3, 2, "CUR MNVR COMPL");
	sprintf_s(cbuf, 255, "1 START TIME %.3d/%.2d:%.2d:%.2d", 
		START_TIME[0], START_TIME[1], START_TIME[2], START_TIME[3]);
	pMDU->mvprint(1, 2, cbuf);

	pMDU->mvprint(0, 4, "MNVR OPTION");
	sprintf_s(cbuf, 255, "5 R %6.2f", MNVR_OPTION.data[ROLL]);
	pMDU->mvprint(1, 5, cbuf);
	sprintf_s(cbuf, 255, "6 P %6.2f", MNVR_OPTION.data[PITCH]);
	pMDU->mvprint(1, 6, cbuf);
	sprintf_s(cbuf, 255, "7 Y %6.2f", MNVR_OPTION.data[YAW]);
	pMDU->mvprint(1, 7, cbuf);

	pMDU->mvprint(0, 9, "TRK/ROT OPTIONS");
	sprintf_s(cbuf, 255, "8 TGT ID %03d", TGT_ID);
	pMDU->mvprint(1, 10, cbuf);

	pMDU->mvprint(1, 12, "9  RA");
	pMDU->mvprint(1, 13, "10 DEC");
	pMDU->mvprint(1, 14, "11 LAT");
	pMDU->mvprint(1, 15, "12 LON");
	pMDU->mvprint(1, 16, "13 ALT");

	sprintf_s(cbuf, 255, "14 BODY VECT %d", BODY_VECT);
	pMDU->mvprint(1, 18, cbuf);
	sprintf_s(cbuf, 255, "15 P  %6.2f", P);
	pMDU->mvprint(1, 20, cbuf);
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
	pMDU->mvprint(20, 11, "ERR DAP 24");

	pMDU->mvprint(26, 14, "ROLL    PITCH    YAW");
	sprintf_s(cbuf, 255, "CUR   %6.2f  %6.2f  %6.2f", CUR_ATT.data[ROLL], CUR_ATT.data[PITCH], CUR_ATT.data[YAW]);
	pMDU->mvprint(19, 15, cbuf);
	sprintf_s(cbuf, 255, "REQD  %6.2f  %6.2f  %6.2f", REQD_ATT.data[ROLL], REQD_ATT.data[PITCH], REQD_ATT.data[YAW]);
	pMDU->mvprint(19, 16, cbuf);
	sprintf_s(cbuf, 255, "ERR  %+7.2f %+7.2f %+7.2f", ATT_ERR.data[ROLL], ATT_ERR.data[PITCH], ATT_ERR.data[YAW]);
	pMDU->mvprint(19, 17, cbuf);
	sprintf_s(cbuf, 255, "RATE %+7.3f %+7.3f %+7.3f", degAngularVelocity.data[ROLL], degAngularVelocity.data[PITCH], degAngularVelocity.data[YAW]);
	pMDU->mvprint(19, 18, cbuf);
}

void OrbitDAP::PaintDAPCONFIGDisplay(vc::MDU* pMDU) const
{
	char *strings[3]={" ALL", "NOSE", "TAIL"};
	char cbuf[255];
	int lim[3]={3, 5, 5};
	int i, n;

	PrintCommonHeader("DAP CONFIG", pMDU);

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

	int edit=2; //temporary
	for(n=1, i=0;n<=lim[edit];n+=2, i++) {
		sprintf_s(cbuf, 255, "%d %.4f", 10*n, DAPConfiguration[i].PRI_ROT_RATE);
		pMDU->mvprint(9+11*i, 3, cbuf);
		sprintf_s(cbuf, 255, "%d  %05.2f", 10*n+1, DAPConfiguration[i].PRI_ATT_DB);
		pMDU->mvprint(9+11*i, 4, cbuf);
		sprintf_s(cbuf, 255, "%d   %.2f", 10*n+2, DAPConfiguration[i].PRI_RATE_DB);
		pMDU->mvprint(9+11*i, 5, cbuf);
		sprintf_s(cbuf, 255, "%d   %.2f", 10*n+3, DAPConfiguration[i].PRI_ROT_PLS);
		pMDU->mvprint(9+11*i, 6, cbuf);
		sprintf_s(cbuf, 255, "%d  %.3f", 10*n+4, DAPConfiguration[i].PRI_COMP);
		pMDU->mvprint(9+11*i, 7, cbuf);
		sprintf_s(cbuf, 255, "%d   %s", 10*n+5, strings[DAPConfiguration[i].PRI_P_OPTION]);
		pMDU->mvprint(9+11*i, 8, cbuf);
		sprintf_s(cbuf, 255, "%d   %s", 10*n+6, strings[DAPConfiguration[i].PRI_Y_OPTION]);
		pMDU->mvprint(9+11*i, 9, cbuf);
		sprintf_s(cbuf, 255, "%d   %.2f", 10*n+7, DAPConfiguration[i].PRI_TRAN_PLS);
		pMDU->mvprint(9+11*i, 10, cbuf);

		sprintf(cbuf, "%d  %.3f", 10*n+8, DAPConfiguration[i].ALT_RATE_DB);
		pMDU->mvprint(9+11*i, 12, cbuf);
		sprintf(cbuf, "%d   %s", 10*n+9, strings[DAPConfiguration[i].ALT_JET_OPT]);
		pMDU->mvprint(9+11*i, 13, cbuf);
		sprintf(cbuf, "%d      %d", 10*n+10, DAPConfiguration[i].ALT_JETS);
		pMDU->mvprint(9+11*i, 14, cbuf);
		sprintf(cbuf, "%d   %.2f", 10*n+11, DAPConfiguration[i].ALT_ON_TIME);
		pMDU->mvprint(9+11*i, 15, cbuf);
		sprintf(cbuf, "%d   %.2f", 10*n+12, DAPConfiguration[i].ALT_DELAY);
		pMDU->mvprint(9+11*i, 16, cbuf);

		sprintf(cbuf, "%d %.4f", 10*n+13, DAPConfiguration[i].VERN_ROT_RATE);
		pMDU->mvprint(9+11*i, 18, cbuf);
		sprintf(cbuf, "%d   %.2f", 10*n+14, DAPConfiguration[i].VERN_ATT_DB);
		pMDU->mvprint(9+11*i, 19, cbuf);
		sprintf(cbuf, "%d  %.3f", 10*n+15, DAPConfiguration[i].VERN_RATE_DB);
		pMDU->mvprint(9+11*i, 20, cbuf);
		sprintf(cbuf, "%d   %.2f", 10*n+16, DAPConfiguration[i].VERN_ROT_PLS);
		pMDU->mvprint(9+11*i, 21, cbuf);
		sprintf(cbuf, "%d  %.3f", 10*n+17, DAPConfiguration[i].VERN_COMP);
		pMDU->mvprint(9+11*i, 22, cbuf);
		sprintf(cbuf, "%d      %d", 10*n+18, DAPConfiguration[i].VERN_CNTL_ACC);
		pMDU->mvprint(9+11*i, 23, cbuf);
	}
}

bool OrbitDAP::OnParseLine(const char* keyword, const char* value)
{
	if(!_strnicmp(keyword, "TGT_ID", 6)) {
		sscanf(value, "%d", &TGT_ID);
		return true;
	}
	else if(!_strnicmp(keyword, "BODY_VECT", 9)) {
		sscanf(value, "%d", &BODY_VECT);
		return true;
	}
	else if(!_strnicmp(keyword, "P_ANGLE", 7)) {
		sscanf(value, "%lf", &P);
		return true;
	}
	else if(!_strnicmp(keyword, "Y_ANGLE", 7)) {
		sscanf(value, "%lf", &Y);
		return true;
	}
	else if(!_strnicmp(keyword, "OM_ANGLE", 8)) {
		sscanf(value, "%lf", &OM);
		return true;
	}
	else if(!_strnicmp(keyword, "ROLL", 4)) {
		sscanf(value, "%lf", &MNVR_OPTION.data[ROLL]);
		return true;
	}
	else if(!_strnicmp(keyword, "PITCH", 5)) {
		sscanf(value, "%lf", &MNVR_OPTION.data[PITCH]);
		return true;
	}
	else if(!_strnicmp(keyword, "YAW", 3)) {
		sscanf(value, "%lf", &MNVR_OPTION.data[YAW]);
		return true;
	}
	else if(!_strnicmp(keyword, "DAP_MODE", 8)) {
		int nTemp1, nTemp2;
		sscanf(value, "%d %d", &nTemp1, &nTemp2);
		DAPSelect = static_cast<DAP_SELECT>(nTemp1);
		DAPMode = static_cast<DAP_MODE>(nTemp2);
		return true;
	}
	else if(!_strnicmp(keyword, "ROT_MODE", 8)) {
		int nTemp[3];
		sscanf(value, "%d %d %d", &nTemp[0], &nTemp[1], &nTemp[2]);
		for(int i=0;i<3;i++) RotMode[i] = static_cast<ROT_MODE>(nTemp[i]);
		return true;
	}
	else if(!_strnicmp(keyword, "TRANS_MODE", 10)) {
		int nTemp[3];
		sscanf(value, "%d %d %d", &nTemp[0], &nTemp[1], &nTemp[2]);
		for(int i=0;i<3;i++) TransMode[i] = static_cast<TRANS_MODE>(nTemp[i]);
		return true;
	}
	else if(!_strnicmp(keyword, "CONTROL_MODE", 12)) {
		//sscanf(value, "%d", &ControlMode);
		int nTemp;
		sscanf(value, "%d", &nTemp);
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
		sscanf(value, "%lf", &FutMnvrStartTime);
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
		//InitializeControlMode();
		break;
	case 3: // INRTL
		DAPControlMode = INRTL;
		StartINRTLManeuver(radCurrentOrbiterAtt);
		//InitializeControlMode();
		break;
	case 4: // LVLH
		DAPControlMode = LVLH;
		StartLVLHManeuver(GetCurrentLVLHAttitude());
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
	sprintf_s(oapiDebugString(), 255, "Rate: %f AttDb %f RateDb: %f", degRotRate, degAttDeadband, degRateDeadband);
}

double OrbitDAP::CalcManeuverCompletionTime(const VECTOR3& radTargetAttOrbiter, const VECTOR3& radNullRatesOrbiter) const
{
	double mnvrTime = 0.0;
	double lastMnvrTime = 0.0;
	int counter = 0;
	VECTOR3 radFinalTargetAtt, Axis;
	MATRIX3 PYR;
	do {
		counter++;
		lastMnvrTime=mnvrTime;
		if(mnvrTime < 0) {
			mnvrTime=0.0;
		}
		radFinalTargetAtt = radTargetAttOrbiter+radNullRatesOrbiter*mnvrTime; //check TargetAtt frame
		PYR=CalcPitchYawRollRotMatrix(radFinalTargetAtt);
		double Angle=CalcEulerAngle(IdentityMatrix, PYR, Axis);
		mnvrTime=(Angle*DEG)/degRotRate;

		//sprintf(oapiDebugString(), "Iterating: %d %f", counter, MNVR_TIME);
		//sprintf(oapiDebugString(), "Iterating: %d %f %f %f", counter, MNVR_TIME, NullRates.data[YAW]*DEG, TargetAttOrbiter.data[ROLL]*DEG);
		//oapiWriteLog(oapiDebugString());
	} while(abs(mnvrTime-lastMnvrTime)>0.05 && counter<50);
	return max(mnvrTime, 0.0);
}

VECTOR3 OrbitDAP::GetCurrentLVLHAttitude() const
{
	VESSELSTATUS Status;
	STS()->GetStatus(Status);

	RefPoints GlobalPts, LocalPts;
	MATRIX3 LocalToGlobal;
	VECTOR3 LocVel, HVel;
	VECTOR3 PitchUnit = {0, 0, 1.0}, YawRollUnit = {1.0, 0, 0};
	VECTOR3 H = crossp(Status.rpos, Status.rvel);
	VECTOR3 RefAttitude = GetPYR2(Status.rvel, H);

	RotateVector(PitchUnit, RefAttitude, GlobalPts.Pitch);
	RotateVector(YawRollUnit, RefAttitude, GlobalPts.Yaw);
	GlobalPts.Pitch = GlobalPos + GlobalPts.Pitch;
	GlobalPts.Yaw = GlobalPos + GlobalPts.Yaw;	
	STS()->Global2Local(GlobalPts.Pitch, LocalPts.Pitch);
	STS()->Global2Local(GlobalPts.Yaw, LocalPts.Yaw);
	
	VECTOR3 radLVLHAngles=GetPYR(LocalPts.Pitch, LocalPts.Yaw);

	STS()->GetRotationMatrix(LocalToGlobal);
	LocVel=tmul(LocalToGlobal, Status.rvel); //multiply rvel by transpose(inverse) of rotation matrix
	STS()->HorizonRot(LocVel, HVel);
	double VVAngle=asin(HVel.y/length(HVel));
	radLVLHAngles.data[PITCH]+=VVAngle;
	return radLVLHAngles;
}

VECTOR3 OrbitDAP::ConvertOrbiterAnglesToLocal(const VECTOR3 &radAngles) const
{
	VECTOR3 Output=_V(0, 0, 0);
	MATRIX3 RotMatrixX, RotMatrixY, RotMatrixZ, LocalToGlobal;
	//MATRIX3 M50, RotMatrixM50, LocalToGlobal;

	GetRotMatrixX(radAngles.x, RotMatrixX);
	GetRotMatrixY(radAngles.y, RotMatrixY);
	GetRotMatrixZ(radAngles.z, RotMatrixZ);

	STS()->GetRotationMatrix(LocalToGlobal);
	//transpose matrix
	LocalToGlobal=_M(LocalToGlobal.m11, LocalToGlobal.m21, LocalToGlobal.m31,
					 LocalToGlobal.m12, LocalToGlobal.m22, LocalToGlobal.m32,
					 LocalToGlobal.m13, LocalToGlobal.m23, LocalToGlobal.m33);
	MATRIX3 Temp=mul(RotMatrixX, RotMatrixY);
	MATRIX3 RotMatrix=mul(Temp,RotMatrixZ);
	RotMatrix=mul(LocalToGlobal, RotMatrix);

	//get angles
	Output.data[PITCH]=atan2(RotMatrix.m23, RotMatrix.m33);
	Output.data[YAW]=-asin(RotMatrix.m13);
	Output.data[ROLL]=atan2(RotMatrix.m12, RotMatrix.m11);
	return Output;
}

MATRIX3 OrbitDAP::ConvertLVLHAnglesToM50Matrix(const VECTOR3 &radAngles) const
{
	// calculate matrix to convert from LVLH to Orbiter inertial frame
	VECTOR3 GPos, GVel;
	STS()->GetRelativePos(STS()->GetSurfaceRef(), GPos);
	STS()->GetRelativeVel(STS()->GetSurfaceRef(), GVel);
	return ::ConvertLVLHAnglesToM50Matrix(radAngles, GPos, GVel);
	/*VECTOR3 norm_GVel = GVel/length(GVel); // almost z-axis
	VECTOR3 y_axis = GPos/length(GPos);
	VECTOR3 x_axis = crossp(y_axis, norm_GVel);
	x_axis = x_axis/length(x_axis);
	VECTOR3 z_axis = crossp(x_axis, y_axis);
	MATRIX3 TFMatrix = _M(x_axis.x, y_axis.x, z_axis.x,
						x_axis.y, y_axis.y, z_axis.y,	
						x_axis.z, y_axis.z, z_axis.z);

	// convert LVLH angles to rotation matrix
	VECTOR3 HorizonX, HorizonY, HorizonZ;
	RotateVectorPYR(_V(1, 0, 0), radAngles, HorizonX);
	RotateVectorPYR(_V(0, 1, 0), radAngles, HorizonY);
	RotateVectorPYR(_V(0, 0, 1), radAngles, HorizonZ);
	MATRIX3 LVLHMatrix = _M(HorizonX.x, HorizonY.x, HorizonZ.x,
							HorizonX.y, HorizonY.y, HorizonZ.y,
							HorizonX.z, HorizonY.z, HorizonZ.z);
	MATRIX3 RotMatrix=mul(TFMatrix, LVLHMatrix);

	RotMatrix=ConvertMatrixBetweenM50AndOrbiter(RotMatrix);
	return RotMatrix;*/
}

MATRIX3 OrbitDAP::CalcPitchYawRollRotMatrix(const VECTOR3& radTargetAttOrbiter) const
{
	//uses angles in orbiter coordinate-frame
	RefPoints GlobalPts, LocalPts;
	VECTOR3 PitchUnit = {0, 0, 1.0}, YawRollUnit = {1.0, 0, 0};
	//RotateVector(PitchUnit, RelAttitude, PitchUnit);
	//RotateVector(YawRollUnit, RelAttitude, YawRollUnit);
	RotateVector(PitchUnit, radTargetAttOrbiter, GlobalPts.Pitch);
	RotateVector(YawRollUnit, radTargetAttOrbiter, GlobalPts.Yaw);
	GlobalPts.Pitch = GlobalPos + GlobalPts.Pitch;
	GlobalPts.Yaw = GlobalPos + GlobalPts.Yaw;	
	STS()->Global2Local(GlobalPts.Pitch, LocalPts.Pitch);
	STS()->Global2Local(GlobalPts.Yaw, LocalPts.Yaw);
	//return GetPYR(LocalPts.Pitch, LocalPts.Yaw);

	VECTOR3 Pitch = Normalize(LocalPts.Pitch);
	VECTOR3 YawRoll = Normalize(LocalPts.Yaw);
	VECTOR3 H = Normalize(crossp(Pitch, YawRoll));
	return _M(YawRoll.x, YawRoll.y, YawRoll.z,
			  H.x, H.y, H.z,
			  Pitch.x, Pitch.y, Pitch.z);
}

};