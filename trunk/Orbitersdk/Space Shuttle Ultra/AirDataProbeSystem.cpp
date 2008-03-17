// AirDataProbeSystem.cpp: Implementierung der Klasse AirDataProbeSystem.
//
//////////////////////////////////////////////////////////////////////

#include "Atlantis.h"
#include "SubsystemDirector.h"
#include "AirDataProbeSystem.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

AirDataProbeSystem::AirDataProbeSystem(SubsystemDirector* _director)
: AtlantisSubsystem(_director, "ADPS")
{
	left_deploy[0] = left_deploy[1] = 0.0;
	right_deploy[0] = right_deploy[1] = 0.0;

	left_mode[0] = left_mode[1] = ADPS_STOW;
	left_state[0] = left_state[1] = ADPS_STOWED;

	right_mode[0] = right_mode[1] = ADPS_STOW;
	right_state[0] = right_state[1] = ADPS_STOWED;
}

AirDataProbeSystem::~AirDataProbeSystem()
{

}

void AirDataProbeSystem::OnPreStep(double fSimT, double fDeltaT, double fMJD)
{
	double fDeploySpeed = 1.0/15.0;
	switch(left_state[0])
	{
	case ADPS_DEPLOYING:
		if(left_deploy[0] < 1.0)
		{
			left_deploy[1] = min(1.0, left_deploy[0] + fDeploySpeed * fDeltaT);
			if(left_deploy[1] == 1.0)
			{
				left_state[1] = ADPS_DEPLOYED;
			}
		}
		break;
	case ADPS_STOWING:
		if(left_deploy[0] > 0.0)
		{
			left_deploy[1] = max(0.0, left_deploy[0] - fDeploySpeed * fDeltaT);
			if(left_deploy[1] == 0.0)
			{
				left_state[1] = ADPS_STOWED;
			}
		}
		break;

	}

	switch(left_mode[0])
	{
	case ADPS_STOW:
		if(left_state[0] == ADPS_DEPLOYED)
			left_state[1] = ADPS_STOWING;
		break;
	case ADPS_DEPLOY:
		if(left_state[0] == ADPS_STOWED)
			left_state[1] = ADPS_DEPLOYING;
		break;
	}

	switch(right_state[0])
	{
	case ADPS_DEPLOYING:
		if(right_deploy[0] < 1.0)
		{
			right_deploy[1] = min(1.0, right_deploy[0] + fDeploySpeed * fDeltaT);
			if(right_deploy[1] == 1.0)
			{
				right_state[1] = ADPS_DEPLOYED;
			}
		}
		break;
	case ADPS_STOWING:
		if(right_deploy[0] > 0.0)
		{
			right_deploy[1] = max(0.0, right_deploy[0] - fDeploySpeed * fDeltaT);
			if(right_deploy[1] == 0.0)
			{
				right_state[1] = ADPS_STOWED;
			}
		}
		break;

	}

	switch(right_mode[0])
	{
	case ADPS_STOW:
		if(right_state[0] == ADPS_DEPLOYED)
			right_state[1] = ADPS_STOWING;
		break;
	case ADPS_DEPLOY:
		if(right_state[0] == ADPS_STOWED)
			right_state[1] = ADPS_DEPLOYING;
		break;
	}

//	sprintf(oapiDebugString(), "ADPS: %f  %d/%d %f %d/%d", left_deploy[0], left_state[0], left_mode[0], 
//		right_deploy[0], right_state[0], right_mode[0]);
}

void AirDataProbeSystem::SetDeployMode(int side, DEPLOY_MODE mode)
{
	switch(side)
	{
	case 0:
		left_mode[1] = mode;
		break;
	case 1:
		right_mode[1] = mode;
		break;
	}
}

bool AirDataProbeSystem::IsDeployed(int side) const
{
	if(side == 0 && left_deploy[0] < 1.0)
		return false;
	if(side == 1 && right_deploy[0] < 1.0)
		return false;
	return true;
}

void AirDataProbeSystem::OnPostStep(double fSimT, double fDeltaT, double fMJD)
{
	director->STS()->SetAirDataProbeDeployment(0, left_deploy[0]);
	director->STS()->SetAirDataProbeDeployment(1, right_deploy[0]);
}

double AirDataProbeSystem::GetStaticPressure(int side) const
{
	return 0.0;
}

double AirDataProbeSystem::GetDynamicPressure(int side) const
{
	return 0.0;
}

double AirDataProbeSystem::GetTotalTemperature(int side) const
{
	return 0.0;
}

void AirDataProbeSystem::OnPropagate(double fSimT, double fDeltaT, double fMJD)
{
	left_deploy[0] = left_deploy[1];
	left_state[0] = left_state[1];
	left_mode[0] = left_mode[1];

	right_deploy[0] = right_deploy[1];
	right_state[0] = right_state[1];
	right_mode[0] = right_mode[1];
}
