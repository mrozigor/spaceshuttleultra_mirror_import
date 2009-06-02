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

bool SubsystemDirector::ExistsSubsystem(const std::string& name) const
{
	std::vector<AtlantisSubsystem*>::const_iterator iter = subsystems.begin();
	while(iter != subsystems.end())
	{
		if((*iter)->GetIdentifier() == name) {
			return true;
		}
		iter++;
	}
	return false;
}

AtlantisSubsystem* SubsystemDirector::GetSubsystemByName(const std::string& name) const
{
	std::vector<AtlantisSubsystem*>::const_iterator iter = subsystems.begin();
	while(iter != subsystems.end())
	{
		if((*iter)->GetIdentifier() == name) {
			return (*iter);
		}
		iter++;
	}
	return NULL;
}

DiscreteBundleManager* SubsystemDirector::BundleManager() const {
	return psts->BundleManager();
}

ShuttleBusManager* SubsystemDirector::BusManager() const {
	return psts->BusManager();
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
		subsystems[i]->OnSetClassCaps();
	}
}

bool SubsystemDirector::ParseScenarioLine(FILEHANDLE scn, char* line)
{
	unsigned long i;
	char pszBuffer[400];
	if(!_strnicmp(line, "@SUBSYSTEM", 10)) {
		oapiWriteLog(line);
		line += 11;
		unsigned long i = 0;
		bool bStringFlag = false;
		while((*line != '\0' && *line != ' ') || bStringFlag) {
			if(*line == '\"') {
				bStringFlag = !bStringFlag;
			} else {
				pszBuffer[i] = *line;
				i++;
			}
			line++;
		}

		pszBuffer[i] = '\0';

		if(ExistsSubsystem(pszBuffer)) {
			AtlantisSubsystem* pT = GetSubsystemByName(pszBuffer);
			pT->OnReadState(scn);
		} else {
			//skip block
			//oapiWriteLog(pszBuffer);
			oapiWriteLog("Skip block");
			while(oapiReadScenario_nextline(scn, line)) {
				if(!_strnicmp(line, "@ENDSUBSYSTEM", 13))
					break;
			}
		}
	} else {
		for(i = 0; i<subsystems.size(); i++)
		{
			//
			if(subsystems[i]->OnParseLine(line))
			{
				return true;
			}
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
	char pszBuffer[400];
	for(i = 0; i<subsystems.size(); i++)
	{
		sprintf_s(pszBuffer,"@SUBSYSTEM %s", subsystems[i]->GetQualifiedIdentifier().c_str());
		oapiWriteLog(pszBuffer);
		oapiWriteLine(scn, pszBuffer);

		subsystems[i]->OnSaveState(scn);

		sprintf_s(pszBuffer,"@ENDSUBSYSTEM\t\t;%s", subsystems[i]->GetQualifiedIdentifier().c_str());
		oapiWriteLog(pszBuffer);
		oapiWriteLine(scn, pszBuffer);
	}
	return true;
}

bool SubsystemDirector::PostStep(double fSimT, double fDeltaT, double fMJD)
{
	unsigned long i;
	//const double SUBSAMPLING_DELTAT = 0.0005;	//0.5 ms
	const double SUBSAMPLING_DELTAT = 0.04;	//40 ms
	

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
