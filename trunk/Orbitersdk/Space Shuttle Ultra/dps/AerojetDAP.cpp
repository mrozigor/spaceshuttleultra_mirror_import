#include "AerojetDAP.h"
#include "../Atlantis.h"
#include <UltraMath.h>
#include "../ParameterValues.h"

namespace dps
{

AerojetDAP::AerojetDAP(SimpleGPCSystem* _gpc)
: SimpleGPCSoftware(_gpc, "AerojetDAP"),
bFirstStep(true), OrbiterMass(1.0),
//AOA_ElevonPitch(0.25, 0.10, 0.01, -1.0, 1.0, -50.0, 50.0), //NOTE: may be better to reduce integral limits and increase i gain
//Rate_ElevonPitch(0.75, 0.001, 0.005, -0.75, 0.75, -60.0, 60.0),
//Pitch_ElevonPitch(0.25, 0.10, 0.01, -1.0, 1.0, -50.0, 50.0),
ElevonPitch(0.25, 0.10, 0.01, -1.0, 1.0, -50.0, 50.0), //NOTE: may be better to reduce integral limits and increase i gain
Roll_AileronRoll(0.15, 0.05, 0.00, -1.0, 1.0)
{
	PMI = _V(1.0, 1.0, 1.0);
	RCSTorque.data[PITCH] = 0.5*ORBITER_PITCH_TORQUE;
	RCSTorque.data[YAW] = 0.5*ORBITER_YAW_TORQUE;
	RCSTorque.data[ROLL] = 0.5*ORBITER_ROLL_TORQUE;

	for(unsigned short i=0;i<3;i++) {
		ThrustersActive[i]=true;
		AerosurfacesActive[i]=false;
		RotatingAxis[i]=false;
		CSSInitialized[i]=false;
	}

	degTargetAttitude = _V(0.0, 0.0, 0.0);
	degCurrentAttitude = _V(0.0, 0.0, 0.0);
	degCurrentRates = _V(0.0, 0.0, 0.0);
}

AerojetDAP::~AerojetDAP()
{
}

void AerojetDAP::Realize()
{
	DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("HC_INPUT", 16);
	for(int i=0;i<3;i++) {
		RHCInput[i].Connect(pBundle, i);
	}

	pBundle=STS()->BundleManager()->CreateBundle("AEROSURFACE_CMD", 16);
	//LeftElevonCommand.Connect(pBundle, 0);
	//RightElevonCommand.Connect(pBundle, 1);
	ElevonCommand.Connect(pBundle, 0);
	AileronCommand.Connect(pBundle, 1);

	pBundle=STS()->BundleManager()->CreateBundle("THRUSTER_CMD", 16);
	for(unsigned int i=0;i<3;i++) {
		ThrusterCommands[i].Connect(pBundle, i);
	}

	pBundle=BundleManager()->CreateBundle("CSS_CONTROLS", 4);
	PitchAuto.Connect(pBundle, 0);
	RollYawAuto.Connect(pBundle, 2);
}

void AerojetDAP::OnPreStep(double SimT, double DeltaT, double MJD)
{
	// on first step, Orbiter gives incorrect data, so ignore this step
	if(bFirstStep) {
		bFirstStep = false;
		return;
	}

	switch(GetMajorMode()) {
	case 304:
		UpdateOrbiterData();
		GetAttitudeData(DeltaT);

		CheckControlActivation();
		//SetThrusterLevels();

		// control yaw regardless of AUTO/CSS mode
		//if(ThrustersActive[YAW])
			//ThrusterCommands[YAW].SetLine(GetThrusterCommand(YAW));

		if(PitchAuto) {
			CSSInitialized[PITCH] = false;
			degTargetAttitude.data[PITCH] = CalculateTargetAOA(STS()->GetMachNumber());
			
			//if(ThrustersActive[PITCH])
				//ThrusterCommands[PITCH].SetLine(GetThrusterCommand(PITCH));

			/*double elevonPos = 0.0;
			double aileronPos = 0.0;
			//double rudderPos = 0.0;
			if(AerosurfacesActive[PITCH]) {
				elevonPos = range(-1.0, AOA_ElevonPitch.Step(degTargetAttitude.data[PITCH]-degCurrentAttitude.data[PITCH], DeltaT), 1.0);
			}
			if(AerosurfacesActive[ROLL]) {
				aileronPos = range(-1.0, Roll_AileronRoll.Step(degTargetAttitude.data[ROLL]-degCurrentAttitude.data[ROLL], DeltaT), 1.0);
			}
			ElevonCommand.SetLine(static_cast<float>(elevonPos));
			AileronCommand.SetLine(static_cast<float>(aileronPos));*/
						
			//sprintf_s(oapiDebugString(), 255, "TargetAOA: %f ElevonPos: %f error: %f", targetAOA, elevonPos, targetAOA-CurrentAttitude.data[PITCH]);
			//sprintf_s(oapiDebugString(), 255, "AOA: %f %f Beta: %f %f Bank: %f %f", degCurrentAttitude.data[PITCH], degCurrentRates.data[PITCH],
				//degCurrentAttitude.data[YAW], degCurrentRates.data[YAW], degCurrentAttitude.data[ROLL], degCurrentRates.data[ROLL]);
		}
		else {
			CSSPitchGuidance(DeltaT);
		}

		// roll AP isn't implemented yet, so just CSS guidance
		CSSRollGuidance(DeltaT);

		// set thruster and aerosurface commands
		for(int i=0;i<3;i++) {
			if(ThrustersActive[i])
				ThrusterCommands[i].SetLine(GetThrusterCommand(static_cast<AXIS>(i), DeltaT));
			else
				ThrusterCommands[i].SetLine(0.0f);
		}
		SetAerosurfaceCommands(DeltaT);

		/*for(int i=0;i<3;i++) {
			if(AerosurfacesActive[i]) {
				double Error = degTargetAttitude.data[i] - degCurrentAttitude.data[i];
				double elevonPos = Pitch_ElevonPitch.Step(PitchError, DeltaT);
				ElevonCommand.SetLine(static_cast<float>(elevonPos));
			}
		}*/
		break;
	case 305:
		UpdateOrbiterData();
		GetAttitudeData(DeltaT);

		CheckControlActivation();
		//SetThrusterLevels();

		CSSPitchGuidance(DeltaT);
		CSSRollGuidance(DeltaT);

		// only yaw thrusters should be active at this point
		if(ThrustersActive[YAW])
			ThrusterCommands[YAW].SetLine(GetThrusterCommand(YAW, DeltaT));
		SetAerosurfaceCommands(DeltaT);
		break;
	}
	
	bFirstStep = false;
}

bool AerojetDAP::OnMajorModeChange(unsigned int newMajorMode)
{
	if(newMajorMode ==304 || newMajorMode == 305) {
		// set Translation Commands to 0.0 during entry and landing
		DiscreteBundle* pBundle = BundleManager()->CreateBundle("THRUSTER_CMD", 16);
		DiscOutPort port;
		for(int i=0;i<3;i++) {
			port.Connect(pBundle, i+3);
			port.ResetLine();
		}
		return true;
	}
	return false;
}

void AerojetDAP::SetAerosurfaceCommands(double DeltaT)
{
	double elevonPos = 0.0;
	double aileronPos = 0.0;
	if(AerosurfacesActive[PITCH]) {
		elevonPos = range(-1.0, ElevonPitch.Step(degTargetAttitude.data[PITCH]-degCurrentAttitude.data[PITCH], DeltaT), 1.0);
	}
	if(AerosurfacesActive[ROLL]) {
		aileronPos = range(-1.0, Roll_AileronRoll.Step(degTargetAttitude.data[ROLL]-degCurrentAttitude.data[ROLL], DeltaT), 1.0);
	}
	ElevonCommand.SetLine(static_cast<float>(elevonPos));
	AileronCommand.SetLine(static_cast<float>(aileronPos));

	sprintf_s(oapiDebugString(), 255, "Roll: %f Target Roll: %f Roll Rate: %f Commanded Aileron: %f",
		degCurrentAttitude.data[ROLL], degTargetAttitude.data[ROLL], degCurrentRates.data[ROLL], aileronPos);
}

void AerojetDAP::SetThrusterLevels()
{
	//for the moment, use RHC input to control thruster firings
	for(unsigned short i=0;i<3;i++) {
		if(ThrustersActive[i]) {
			if(RHCInput[i].GetVoltage()>0.01) {
				ThrusterCommands[i].SetLine(1.0f);
			}
			else if(RHCInput[i].GetVoltage()<-0.01) {
				ThrusterCommands[i].SetLine(-1.0f);
			}
			else {
				ThrusterCommands[i].ResetLine();
			}
		}
		else {
			ThrusterCommands[i].ResetLine();
		}
	}
	/*if(PitchActive) {
		if(RHCInput[PITCH].GetVoltage()>0.01) {
			ThrusterCommands[PITCH].SetLine(1.0f);
		}
		else if(RHCInput[PITCH].GetVoltage()<-0.01) {
			ThrusterCommands[PITCH].SetLine(-1.0f);
		}
		else {
			ThrusterCommands[PITCH].ResetLine();
		}
	}
	else {
	}

	if(YawActive) {
		if(RHCInput.data[YAW]>0.01) {
		}
		else if(RHCInput.data[YAW]<-0.01) {
		}
		else {
		}
	}
	else {
	}

	if(RollActive) {
		if(RHCInput.data[ROLL]>0.01) {
		}
		else if(RHCInput.data[ROLL]<-0.01) {
		}
		else {
		}
	}
	else {
	}*/
}

double AerojetDAP::GetThrusterCommand(AXIS axis, double DeltaT)
{
	// values in degrees
	const double ATT_DEADBAND = 0.25;
	const double RATE_DEADBAND = max(0.05, RotationRateChange(OrbiterMass, PMI.data[axis], RCSTorque.data[axis], DeltaT));
	const double MIN_ROTATION_RATE = 0.2;
	const double MAX_ROTATION_RATE = 2.0;

	double attError = degTargetAttitude.data[axis]-degCurrentAttitude.data[axis];

	if(RotatingAxis[axis]) {
		if(abs(attError) < 0.10 && abs(degCurrentRates.data[axis]) < RotationRateChange(OrbiterMass, PMI.data[axis], RCSTorque.data[axis], DeltaT)) { // stopped at target attitude
			RotatingAxis[axis] = false;
			return 0.0;
		}
		else if(abs(attError) <= NullStartAngle(abs(degCurrentRates.data[axis]*RAD), OrbiterMass, PMI.data[axis], RCSTorque.data[axis])) { // null rates
			return -sign(degCurrentRates.data[axis]);
		}
		else { // set rotation rate
			double rotationRate = range(MIN_ROTATION_RATE, 4.0*(abs(attError) - 2.0*ATT_DEADBAND), MAX_ROTATION_RATE);
			double rateError = rotationRate*sign(attError)-degCurrentRates.data[axis];
			//sprintf_s(oapiDebugString(), 255, "AErr: %f RErr: %f Rate: %f", attError, rateError, degCurrentRates.data[axis]);
			if(abs(rateError) < 0.025) return 0.0;
			else return sign(rateError);
		}
	}
	else if(abs(attError) > ATT_DEADBAND) {
		RotatingAxis[axis] = true;
	}
	else if(abs(degCurrentRates.data[axis]) > RATE_DEADBAND) {
		//sprintf_s(oapiDebugString(), 255, "Rate Deadband: %f %f", attError, degCurrentRates.data[axis]);
		return -sign(degCurrentRates.data[axis]);
	}

	return 0.0;
}

void AerojetDAP::CheckControlActivation()
{
	const double ROLL_OFF_PRESS=10*47.880259;
	const double PITCH_OFF_PRESS=40*47.880259;
	const double YAW_OFF_MACH=1.0;

	const double ELEVON_ON_PRESS=2.0*47.880259;
	const double RUDDER_ON_MACH=5.0;

	double dynamicPressure = STS()->GetDynPressure();
	double mach = STS()->GetMachNumber();

	if(ThrustersActive[ROLL] && dynamicPressure>ROLL_OFF_PRESS) {
		ThrustersActive[ROLL]=false;
	}
	if(ThrustersActive[PITCH] && dynamicPressure>PITCH_OFF_PRESS) {
		ThrustersActive[PITCH]=false;
	}
	if(ThrustersActive[ROLL] && mach<YAW_OFF_MACH && dynamicPressure>1000.0) { // 2nd test is to make sure we're in atmosphere
		ThrustersActive[YAW]=false;
	}

	if(!AerosurfacesActive[PITCH] && dynamicPressure>ELEVON_ON_PRESS) {
		AerosurfacesActive[PITCH]=true;
		AerosurfacesActive[ROLL]=true;
	}
	if(!AerosurfacesActive[YAW] && mach<RUDDER_ON_MACH && dynamicPressure>1000.0) {
		AerosurfacesActive[YAW]=true;
	}
}

void AerojetDAP::CSSPitchGuidance(double DeltaT)
{
	if(!CSSInitialized[PITCH]) {
		degTargetAttitude.data[PITCH] = degCurrentAttitude.data[PITCH];
		CSSInitialized[PITCH] = true;
	}
	else {
		double PitchRateCommand = STS()->GetControlSurfaceLevel(AIRCTRL_ELEVATOR)*2.0 + STS()->GetControlSurfaceLevel(AIRCTRL_ELEVATORTRIM)*5.0;
		degTargetAttitude.data[PITCH]+=PitchRateCommand*DeltaT;
		/*double PitchError = degTargetAttitude.data[PITCH] - degCurrentAttitude.data[PITCH];

		double elevonPos = Pitch_ElevonPitch.Step(PitchError, DeltaT);
		ElevonCommand.SetLine(static_cast<float>(elevonPos));*/
		//LeftElevonCommand.SetLine(static_cast<float>(elevonPos));
		//RightElevonCommand.SetLine(static_cast<float>(elevonPos));

		//sprintf_s(oapiDebugString(), 255, "RHC Input: %f elevon: %f rates: %f error: %f",
			//RHCInput[PITCH].GetVoltage(), range(-33.0, elevonPos*-33.0, 18.0), PitchRateCommand, PitchError);
	}
}

void AerojetDAP::CSSRollGuidance(double DeltaT)
{
	if(!CSSInitialized[ROLL]) {
		degTargetAttitude.data[ROLL] = degCurrentAttitude.data[ROLL];
		CSSInitialized[ROLL] = true;
	}
	else {
		double RollRateCommand = -STS()->GetControlSurfaceLevel(AIRCTRL_AILERON)*5.0;
		degTargetAttitude.data[ROLL]+=RollRateCommand*DeltaT;
		
		double deltaLimit = 1.0;
		if(ThrustersActive[ROLL]) deltaLimit = 2.0;
		degTargetAttitude.data[ROLL] = range(degCurrentAttitude.data[ROLL]-deltaLimit, degTargetAttitude.data[ROLL], degCurrentAttitude.data[ROLL]+deltaLimit);
		/*double RollError = degTargetAttitude.data[ROLL] - degCurrentAttitude.data[ROLL];

		double aileronPos = Roll_AileronRoll.Step(RollError, DeltaT);
		AileronCommand.SetLine(static_cast<float>(aileronPos));*/
	}
}

void AerojetDAP::GetAttitudeData(double DeltaT)
{
	//double lastSideslip = degCurrentAttitude.data[YAW];
	VECTOR3 degLastAttitude = degCurrentAttitude;

	// get AOA, sideslip and bank
	if(STS()->GetGPCMajorMode()==304)
		degCurrentAttitude.data[PITCH] = STS()->GetAOA()*DEG;
	else
		degCurrentAttitude.data[PITCH] = STS()->GetPitch()*DEG;
	degCurrentAttitude.data[YAW] = STS()->GetSlipAngle()*DEG;
	degCurrentAttitude.data[ROLL] = STS()->GetBank()*DEG;

	STS()->GetAngularVel(degCurrentRates);
	degCurrentRates*=DEG;
	// rotating frame, so calculate rates by differentiating attitude changes
	if(!bFirstStep) {
		//degCurrentRates.data[YAW] = (degCurrentAttitude.data[YAW]-lastSideslip)/DeltaT;
		for(unsigned int i=0;i<3;i++) {
			degCurrentRates.data[i] = (degCurrentAttitude.data[i]-degLastAttitude.data[i])/DeltaT;
		}
	}
	else {
		//degCurrentRates.data[YAW] = 0.0;
		degCurrentRates = _V(0.0, 0.0, 0.0);
	}
}

void AerojetDAP::UpdateOrbiterData()
{
	OrbiterMass=STS()->GetMass();
	STS()->GetPMI(PMI);
}

double AerojetDAP::CalculateTargetAOA(double mach) const
{
	const double AOA_Lookup[24] = {
		40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 40.0, 39.0, 38.0, 36.0, 34.0, 30.0, 27.0, 23.0, 19.0, 16.0, 15.0};
	//  25,   24,   23,   22,   21,   20,   19,   18,   17,   16,   15,   14,   13,   12,   11,   10,   9,    8,    7,    6,    5,    4,    3,    2.5
	
	if(mach > 3.0) {
		int index = max(25 - static_cast<int>(mach), 1);
		return linterp(static_cast<int>(mach), AOA_Lookup[index], static_cast<int>(mach)+1, AOA_Lookup[index-1], mach);
	}
	else {
		return linterp(3.0, AOA_Lookup[22], 2.5, AOA_Lookup[23], mach);
	}
}

};