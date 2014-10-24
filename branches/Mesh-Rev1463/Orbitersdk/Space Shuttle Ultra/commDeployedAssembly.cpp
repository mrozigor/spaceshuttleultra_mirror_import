#include "DeployedAssembly.h"

namespace comm 
{

DeployedAssembly::DeployedAssembly(AtlantisSubsystemDirector* _director)
: AtlantisSubsystem(_director, "KuBand")
{
}

DeployedAssembly::~DeployedAssembly()
{

}

bool DeployedAssembly::ParseLine(const char* line)
{
	return false;
}

void DeployedAssembly::SaveState(FILEHANDLE scn)
{
}

void DeployedAssembly::PostStep(double fSimT, double fDeltaT)
{

}

void DeployedAssembly::PreStep(double fSimT, double fDeltaT)
{

}

void DeployedAssembly::Propagate(double fSimT, double fDeltaT)
{
}

double DeployedAssembly::GetSignalStrength()
{
	return 0.0;
}

bool DeployedAssembly::SetSlew(double fAzimuth, double fElevation)
{
	//Solve inverse kinematics of the deployed antenna for pointing the antenna.
	return false;
}

double DeployedAssembly::GetAzimuth()
{
	return 0.0;
}

int DeployedAssembly::GetAlphaResolverAngle() const
{
	return -1;
}

int DeployedAssembly::GetBetaResolverAngle() const
{
	return -1;
}

}