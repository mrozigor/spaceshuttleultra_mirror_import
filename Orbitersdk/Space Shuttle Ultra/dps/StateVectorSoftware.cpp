#include "StateVectorSoftware.h"
#include "../Atlantis.h"
#include <UltraMath.h>

namespace dps
{

void GetStateVectors(VESSEL* v, OBJHANDLE hEarth, VECTOR3& pos, VECTOR3& vel)
{
	MATRIX3 obliquityMat;
	oapiGetPlanetObliquityMatrix(hEarth, &obliquityMat);
	v->GetRelativePos(hEarth, pos);
	v->GetRelativeVel(hEarth, vel);
	pos=tmul(obliquityMat, pos);
	vel=tmul(obliquityMat, vel);
}

StateVectorSoftware::StateVectorSoftware(SimpleGPCSystem * _gpc)
: SimpleGPCSoftware(_gpc, "StateVectorSoftware"),
lastUpdateSimTime(-10000.0), propagator(0.2, 50, 7200.0), targetPropagator(0.2, 50, 7200.0), pTargetVessel(NULL)
{
}

StateVectorSoftware::~StateVectorSoftware()
{
}

void StateVectorSoftware::Realize()
{
	hEarth = STS()->GetGravityRef();
	if(!targetVesselName.empty()) SetTargetVessel(const_cast<char*>(targetVesselName.c_str()));

	double J2 = 0;
	if(STS()->NonsphericalGravityEnabled()) J2 = oapiGetPlanetJCoeff(hEarth, 0);
	propagator.SetParameters(STS()->GetMass(), oapiGetMass(hEarth), oapiGetSize(hEarth), J2);
	targetPropagator.SetParameters(1.0, oapiGetMass(hEarth), oapiGetSize(hEarth), J2); // we may not have valid target vessel pointer, so use placeholder mass

	UpdatePropagatorStateVectors();
	if(pTargetVessel) UpdateTargetStateVectors();
}

void StateVectorSoftware::OnPreStep(double SimT, double DeltaT, double MJD)
{
	double timeBetweenUpdates = max(4.0, 4.0*oapiGetTimeAcceleration());
	if((SimT-lastUpdateSimTime) > timeBetweenUpdates) {
		UpdatePropagatorStateVectors();
		propagator.UpdateVesselMass(STS()->GetMass());
		if(pTargetVessel) {
			UpdateTargetStateVectors();
			targetPropagator.UpdateVesselMass(pTargetVessel->GetMass());
		}

		lastUpdateSimTime = SimT;
	}

	propagator.Step(STS()->GetMET(), DeltaT);
	if(pTargetVessel) targetPropagator.Step(STS()->GetMET(), DeltaT);
}

bool StateVectorSoftware::OnMajorModeChange(unsigned int newMajorMode)
{
	if(newMajorMode == 102) { // liftoff - get position at T0
		VECTOR3 vel;
		GetStateVectors(STS(), hEarth, t0Pos, vel);
	}
	if(newMajorMode >= 104 && newMajorMode <= 303) return true;
	return false;
}

bool StateVectorSoftware::ItemInput(int spec, int item, const char * Data)
{
	return false;
}

bool StateVectorSoftware::OnPaint(int spec, vc::MDU * pMDU) const
{
	return false;
}

bool StateVectorSoftware::OnParseLine(const char* keyword, const char* value)
{
	if(!_strnicmp(keyword, "TARGET_VESSEL", 13)) {
		targetVesselName = value;
		return true;
	}
	else if(!_strnicmp(keyword, "T0_POS", 6)) {
		sscanf_s(value, "%lf%lf%lf", &t0Pos.x, &t0Pos.y, &t0Pos.z);
		return true;
	}
	return false;
}

void StateVectorSoftware::OnSaveState(FILEHANDLE scn) const
{
	if(!targetVesselName.empty()) oapiWriteScenario_string(scn, "TARGET_VESSEL", const_cast<char*>(targetVesselName.c_str()));
	oapiWriteScenario_vec(scn, "T0_POS", t0Pos);
}

void StateVectorSoftware::SetTargetVessel(char* vesselName)
{
	OBJHANDLE hTarget = oapiGetVesselByName(vesselName);
	if(hTarget)	{
		pTargetVessel = oapiGetVesselInterface(hTarget);
		 targetVesselName = vesselName;
	}
	else {
		pTargetVessel = NULL;
		targetVesselName = "";
	}
}

double StateVectorSoftware::GetMETAtAltitude(double altitude) const
{
	return propagator.GetMETAtAltitude(STS()->GetMET(), altitude);
}

void StateVectorSoftware::GetPropagatedStateVectors(double met, VECTOR3 & pos, VECTOR3 & vel) const
{
	propagator.GetStateVectors(met, pos, vel);
}

void StateVectorSoftware::GetCurrentStateVectorsM50(VECTOR3& pos, VECTOR3& vel) const
{
	GetStateVectors(STS(), hEarth, pos, vel);
	// GetStateVectors already converts to equatorial (LH M50) frame, so we just need to swap Z and Y axes
	pos = ConvertBetweenLHAndRHFrames(pos);
	vel = ConvertBetweenLHAndRHFrames(vel);
}
void StateVectorSoftware::GetApogeeData(double& ApD, double& ApT) const
{
	propagator.GetApogeeData(STS()->GetMET(), ApD, ApT);
}

void StateVectorSoftware::GetPerigeeData(double& PeD, double& PeT) const
{
	propagator.GetPerigeeData(STS()->GetMET(), PeD, PeT);
}

VECTOR3 StateVectorSoftware::GetPositionAtT0() const
{
	return t0Pos;
}

void StateVectorSoftware::GetTargetStateVectors(double met, VECTOR3 & pos, VECTOR3 & vel) const
{
	targetPropagator.GetStateVectors(met, pos, vel);
}

bool StateVectorSoftware::UpdatePropagatorStateVectors()
{
	VECTOR3 pos, vel;
	GetStateVectors(STS(), hEarth, pos, vel);
	char cbuf[255];
	sprintf_s(cbuf, 255, "Pos: %f %f %f Vel: %f %f %f", pos.x, pos.y, pos.z, vel.x, vel.y, vel.z);
	oapiWriteLog(cbuf);
	return propagator.UpdateStateVector(pos, vel, STS()->GetMET());
}

void StateVectorSoftware::UpdateTargetStateVectors()
{
	VECTOR3 pos, vel;
	GetStateVectors(pTargetVessel, hEarth, pos, vel);
	targetPropagator.UpdateStateVector(pos, vel, STS()->GetMET());
}

};
