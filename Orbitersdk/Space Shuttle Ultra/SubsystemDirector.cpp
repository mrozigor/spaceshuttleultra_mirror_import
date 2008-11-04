#include "SubsystemDirector.h"
#include "Atlantis.h"
#include "AtlantisSubsystem.h"
#include <algorithm>

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
	char pszBuffer[256];
	subsystems.push_back(pSubsys);
	sprintf_s(pszBuffer, 256, "Added subsystem %s.", pSubsys->GetQualifiedIdentifier().c_str());
	oapiWriteLog(pszBuffer);
	return true;
}

AtlantisSubsystem* SubsystemDirector::ReplaceSubsystem(AtlantisSubsystem* pCurrentSubsys, AtlantisSubsystem* pBySubsys) {
	char pszBuffer[256];

	vector<AtlantisSubsystem*>::iterator iter = std::find(subsystems.begin(),
		subsystems.end(), pCurrentSubsys);

	if(iter != subsystems.end()) {

		pCurrentSubsys->UnloadSubsystem();
		*iter = pBySubsys;
		pBySubsys->AddMeshes(psts->orbiter_ofs);
	
		sprintf_s(pszBuffer, 256, "Replaced subsystem %s by subsystem %s.", 
			pCurrentSubsys->GetQualifiedIdentifier().c_str(),
			pBySubsys->GetQualifiedIdentifier().c_str());
		oapiWriteLog(pszBuffer);

		delete pCurrentSubsys;

		sprintf_s(pszBuffer, 256, "Finished clean up.");
		oapiWriteLog(pszBuffer);
		return pBySubsys;
	}
	return pCurrentSubsys;
}

DiscreteBundleManager* SubsystemDirector::BundleManager() const {
	return psts->BundleManager();
}

bool SubsystemDirector::RealizeAll() {
	unsigned long i;
	for(i = 0; i<subsystems.size(); i++)
	{
		//
		subsystems[i]->Realize();
	}
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
	const double SUBSAMPLING_DELTAT = 0.0005;	//0.5 ms
	

	//Subsampling pass
	double t0 = fSimT;
	long lSubCount = 0;
	while(t0 < fSimT + fDeltaT) {
		double dt = min(SUBSAMPLING_DELTAT, fSimT + fDeltaT - t0);
		for(i = 0; i<subsystems.size(); i++)
		{	
			subsystems[i]->OnSubPreStep(t0, dt, fMJD);
		}
		for(i = 0; i<subsystems.size(); i++)
		{
			//
			subsystems[i]->OnSubPropagate(t0, dt, fMJD);
		}

		for(i = 0; i<subsystems.size(); i++)
		{	
			subsystems[i]->OnSubPostStep(t0, dt, fMJD);
		}
		t0 += SUBSAMPLING_DELTAT;
		lSubCount ++;
	}

	//sprintf_s(oapiDebugString(), 255, "%d SUBSAMPLING STEPS", lSubCount);

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

bool SubsystemDirector::SetSSMEParams(unsigned short usMPSNo, double fThrust0, double fISP0, double fISP1)
{

	return psts->SetSSMEParams(usMPSNo, fThrust0, fISP0, fISP1);
}

bool SubsystemDirector::SetSSMEDir(unsigned short usMPSNo, const VECTOR3& dir)
{
	return psts->SetSSMEDir(usMPSNo, dir);
}

bool SubsystemDirector::WriteLog(const AtlantisSubsystem* src, char* message)
{
	return true;
}

Atlantis* SubsystemDirector::STS()
{
	return psts;
}
