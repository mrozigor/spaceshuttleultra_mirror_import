#include "SubsystemDirector.h"
#include "Atlantis.h"
#include "AtlantisSubsystem.h"

SubsystemDirector::SubsystemDirector(Atlantis* _sts)
: psts(_sts)
{
}

SubsystemDirector::~SubsystemDirector()
{
	unsigned long i;
	for(i = 0; i<subsystems.size(); i++)
	{
		delete subsystems[i];
	}
}

bool SubsystemDirector::AddSubsystem(AtlantisSubsystem* pSubsys)
{
	subsystems.push_back(pSubsys);
	return true;
}

void SubsystemDirector::SetClassCaps(FILEHANDLE cfg)
{
	unsigned long i;
	for(i = 0; i<subsystems.size(); i++)
	{
		//
		//subsystems[i]->OnSetClassCaps();
	}
}

bool SubsystemDirector::ParseScenarioLine(char* line)
{
	unsigned long i;
	for(i = 0; i<subsystems.size(); i++)
	{
		//
		if(subsystems[i]->OnParseLine(line))
		{
			return true;
		}
	}
	return false;
}

bool SubsystemDirector::PlaybackEvent(double fSimT, double fEventT, const char* event_t, const char* event)
{
	return false;
}

bool SubsystemDirector::SaveState(FILEHANDLE scn)
{
	unsigned long i;
	for(i = 0; i<subsystems.size(); i++)
	{
		//
		subsystems[i]->OnSaveState(scn);
	}
	return true;
}

bool SubsystemDirector::PostStep(double fSimT, double fDeltaT, double fMJD)
{
	unsigned long i;
	//Propagate subsystem states to the end of the discrete timestep
	for(i = 0; i<subsystems.size(); i++)
	{
		//
		subsystems[i]->OnPropagate(fSimT, fDeltaT, fMJD);
	}
	for(i = 0; i<subsystems.size(); i++)
	{
		//
		subsystems[i]->OnPostStep(fSimT, fDeltaT, fMJD);
	}
	return true;
}

bool SubsystemDirector::PreStep(double fSimT, double fDeltaT, double fMJD)
{
	unsigned long i;
	for(i = 0; i<subsystems.size(); i++)
	{
		//
		subsystems[i]->OnPreStep(fSimT, fDeltaT, fMJD);
	}
	return true;
}

bool SubsystemDirector::WriteLog(const AtlantisSubsystem* src, char* message)
{
	return true;
}

Atlantis* SubsystemDirector::STS()
{
	return psts;
}
