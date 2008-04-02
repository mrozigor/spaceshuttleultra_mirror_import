#include "KuBandSystem.h"

KuBandSystem::KuBandSystem(SubsystemDirector* _director)
: AtlantisSubsystem(_director, "KuBand")
{
}

KuBandSystem::~KuBandSystem()
{

}

bool KuBandSystem::ParseLine(const char* line)
{
	return false;
}

void KuBandSystem::SaveState(FILEHANDLE scn)
{
}

void KuBandSystem::PostStep(double fSimT, double fDeltaT)
{

}

void KuBandSystem::PreStep(double fSimT, double fDeltaT)
{

}

void KuBandSystem::Propagate(double fSimT, double fDeltaT)
{
}

double KuBandSystem::GetSignalStrength()
{
	return 0.0;
}

bool KuBandSystem::SetSlew(double fAzimuth, double fElevation)
{
	//Solve inverse kinematics of the deployed antenna for pointing the antenna.
	return false;
}

double KuBandSystem::GetAzimuth()
{
	return 0.0;
}
