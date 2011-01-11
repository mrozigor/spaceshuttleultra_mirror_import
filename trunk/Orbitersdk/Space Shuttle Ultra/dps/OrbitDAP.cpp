#include "OrbitDAP.h"
#include <UltraMath.h>
#include "../ParameterValues.h"

namespace dps
{

OrbitDAP::OrbitDAP(SimpleGPCSystem* pGPC)
: SimpleGPCSoftware(pGPC, "OrbitDAP"),
OMSTVCControlP(3.5, 0.0, 0.75), OMSTVCControlY(4.0, 0.0, 0.75), OMSTVCControlR(3.5, 0.0, 0.75),
ControlMode(RCS)
{
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

		STS()->BurnInProg = true; // temp code to disable GPC_old DAP
	}
}

void OrbitDAP::UseRCS()
{
	ControlMode = RCS;
	STS()->BurnInProg = false; // temp code to enable GPC_old DAP
}

void OrbitDAP::ManeuverToLVLHAttitude(const VECTOR3& degLVLHAtt)
{
	MATRIX3 RotMatrix;
	
	MATRIX3 RotMatrixR, RotMatrixP, RotMatrixY, Temp;
	GetRotMatrixZ(degLVLHAtt.data[ROLL]*RAD, RotMatrixR);
	GetRotMatrixX(-degLVLHAtt.data[PITCH]*RAD, RotMatrixP);
	GetRotMatrixY(degLVLHAtt.data[YAW]*RAD, RotMatrixY);
	Temp=mul(RotMatrixR, RotMatrixP);
	Temp=mul(Temp, RotMatrixY);
	RotMatrix=_M(Temp.m11, Temp.m21, Temp.m31,
		Temp.m12, Temp.m22, Temp.m32,
		Temp.m13, Temp.m23, Temp.m33);

	LoadCurLVLHManeuver(RotMatrix);

	STS()->LoadBurnAttManeuver(degLVLHAtt);
}

void OrbitDAP::LoadCurLVLHManeuver(const MATRIX3& RotMatrix)
{
	CurManeuver.bValid = true;
	CurManeuver.LVLHTgtOrientationMatrix = RotMatrix;
	CurManeuver.Type = AttManeuver::TRK;
}

void OrbitDAP::OMSTVC(const VECTOR3 &Rates, double SimDT)
{
	VECTOR3 CurrentRates=radAngularVelocity*DEG;
	double pitchDelta=Rates.data[PITCH]-CurrentRates.data[PITCH]; //if positive, vessel is pitching down
	double yawDelta=Rates.data[YAW]-CurrentRates.data[YAW]; //if positive, vessel is rotating to right
	double rollDelta=Rates.data[ROLL]-CurrentRates.data[ROLL]; //if positive, vessel is rolling to left
	bool RCSWraparound=(abs(rollDelta)>0.5 || abs(pitchDelta)>0.25 || abs(yawDelta)>0.25);

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

	if(RCSWraparound) STS()->SetRates(Rates);
	else if(ControlMode!=BOTH_OMS) STS()->SetRates(_V(0.0, 0.0, Rates.data[ROLL])); //for single-engine burns, use RCS for roll control
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
	//GetGlobalOrientation(InertialOrientationRad);
	//CurrentAttitude=ConvertAnglesBetweenM50AndOrbiter(InertialOrientationRad);
	//GetGlobalPos(GVesselPos);	
	//Mass=GetMass();
	//GetPMI(PMI);
}

void OrbitDAP::Realize()
{
	DiscreteBundle* pBundle = BundleManager()->CreateBundle("DAP_PBIS1", 16);
	for(unsigned int i=0;i<4;i++) DAPMode[i].Connect(pBundle, i+2);

	pBundle = BundleManager()->CreateBundle("LOMS", 5);
	POMSGimbalCommand[LEFT].Connect(pBundle, 3);
	YOMSGimbalCommand[LEFT].Connect(pBundle, 4);
	pBundle = BundleManager()->CreateBundle("ROMS", 5);
	POMSGimbalCommand[RIGHT].Connect(pBundle, 3);
	YOMSGimbalCommand[RIGHT].Connect(pBundle, 4);
}

void OrbitDAP::OnPreStep(double SimT, double DeltaT, double MJD)
{
	GetAttitudeData();

	if(ControlMode != RCS) {
		OMSTVC(STS()->ReqdRates, DeltaT); // for the moment, use data calculated by GPC_old code
	}
}

bool OrbitDAP::OnMajorModeChange(unsigned int newMajorMode)
{
	if(newMajorMode >= 104 && newMajorMode <= 303) {
		// perform initialization
		return true;
	}
	return false;
}

};