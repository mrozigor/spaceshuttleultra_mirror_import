#include "AerojetDAP.h"
#include "../Atlantis.h"
#include "../SSUMath.h"

namespace dps
{

AerojetDAP::AerojetDAP(AtlantisSubsystemDirector* _director)
: AtlantisSubsystem(_director, "AerojetDAP"),
AOA_ElevonPitch(0.25, 0.10, 0.01, -1.0, 1.0, -50.0, 50.0), //NOTE: may be better to reduce integral limits and increase i gain
Rate_ElevonPitch(0.25, 0.001, 0.10, -1.0, 1.0, -5.0, 5.0)
{
	for(unsigned short i=0;i<3;i++) ThrustersActive[i]=true;
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
	LeftElevonCommand.Connect(pBundle, 0);
	RightElevonCommand.Connect(pBundle, 1);

	pBundle=STS()->BundleManager()->CreateBundle("THRUSTER_CMD", 16);
	for(unsigned int i=0;i<3;i++) {
		ThrusterCommands[i].Connect(pBundle, i);
	}

	pBundle=BundleManager()->CreateBundle("CSS_CONTROLS", 4);
	PitchAuto.Connect(pBundle, 0);
	RollYawAuto.Connect(pBundle, 2);
}

void AerojetDAP::OnPostStep(double SimT, double DeltaT, double MJD)
{
	switch(STS()->GetGPCMajorMode()) {
	case 304:
		CheckThrusterActivation();
		SetThrusterLevels();

		if(STS()->GetAltitude()<100.0*1000.0 && PitchAuto) {
			double targetAOA = CalculateTargetAOA(STS()->GetMachNumber());
			
			double elevonPos = AOA_ElevonPitch.Step(targetAOA-STS()->GetAOA()*DEG, DeltaT);
			LeftElevonCommand.SetLine(static_cast<float>(elevonPos));
			RightElevonCommand.SetLine(static_cast<float>(elevonPos));
			
			sprintf_s(oapiDebugString(), 255, "TargetAOA: %f ElevonPos: %f error: %f", targetAOA, elevonPos, targetAOA-STS()->GetAOA()*DEG);
		}
		else {
			ControllerInputGuidance(DeltaT);
		}
		break;
	case 305:
		CheckThrusterActivation();
		SetThrusterLevels();

		ControllerInputGuidance(DeltaT);
		break;
	}
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

void AerojetDAP::CheckThrusterActivation()
{
	const double ROLL_OFF_PRESS=10*47.880259;
	const double PITCH_OFF_PRESS=40*47.880259;
	const double YAW_OFF_MACH=1.0;

	double dynamicPressure = STS()->GetDynPressure();
	if(ThrustersActive[ROLL] && dynamicPressure>ROLL_OFF_PRESS) {
		ThrustersActive[ROLL]=false;
	}
	if(ThrustersActive[PITCH] && dynamicPressure>PITCH_OFF_PRESS) {
		ThrustersActive[PITCH]=false;
	}
	if(ThrustersActive[ROLL] && STS()->GetMachNumber()<YAW_OFF_MACH && dynamicPressure>1000.0) { // 2nd test is to make sure we're in atmosphere
		ThrustersActive[YAW]=false;
	}
}

void AerojetDAP::ControllerInputGuidance(double DeltaT)
{
	VECTOR3 AngularVelocity;
	STS()->GetAngularVel(AngularVelocity);
	double PitchRateCommand = STS()->GetControlSurfaceLevel(AIRCTRL_ELEVATOR)*2.0 + STS()->GetControlSurfaceLevel(AIRCTRL_ELEVATORTRIM)*5.0;
	double PitchError = PitchRateCommand-AngularVelocity.data[PITCH]*DEG;

	double elevonPos = Rate_ElevonPitch.Step(PitchError, DeltaT);
	LeftElevonCommand.SetLine(static_cast<float>(elevonPos));
	RightElevonCommand.SetLine(static_cast<float>(elevonPos));

	sprintf_s(oapiDebugString(), 255, "RHC Input: %f elevon: %f rates: %f error: %f",
		RHCInput[PITCH].GetVoltage(), range(-33.0, elevonPos*-33.0, 18.0), PitchRateCommand, PitchError);
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