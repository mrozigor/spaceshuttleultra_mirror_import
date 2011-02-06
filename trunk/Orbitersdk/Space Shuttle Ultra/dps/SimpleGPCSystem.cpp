#include <cassert>
#include "SimpleGPCSystem.h"
#include "SimpleGPCSoftware.h"
#include "OrbitDAP.h"
#include "OMSBurnSoftware.h"
#include "AerojetDAP.h"
#include "../Atlantis.h"

namespace dps
{

SimpleGPCSystem::SimpleGPCSystem(AtlantisSubsystemDirector* _director)
: AtlantisSubsystem(_director, "SimpleGPCSystem")
{
	vSoftware.push_back(new OrbitDAP(this));
	vSoftware.push_back(new OMSBurnSoftware(this));
	vSoftware.push_back(new AerojetDAP(this));
}

SimpleGPCSystem::~SimpleGPCSystem()
{
	for(unsigned int i=0;i<vSoftware.size();i++)
		delete vSoftware[i];
}

void SimpleGPCSystem::SetMajorMode(unsigned int newMM)
{
	vActiveSoftware.clear();
	for(unsigned int i=0;i<vSoftware.size();i++) {
		if(vSoftware[i]->OnMajorModeChange(newMM))
			vActiveSoftware.push_back(vSoftware[i]);
	}
	majorMode = newMM;
}

void SimpleGPCSystem::Realize()
{
	for(unsigned int i=0;i<vSoftware.size();i++)
		vSoftware[i]->Realize();
}

void SimpleGPCSystem::OnPreStep(double SimT, double DeltaT, double MJD)
{
	assert(majorMode == STS()->GetGPCMajorMode()); // TODO: change major mode only in this class
	for(unsigned int i=0;i<vActiveSoftware.size();i++)
		vActiveSoftware[i]->OnPreStep(SimT, DeltaT, MJD);
}

void SimpleGPCSystem::OnPostStep(double SimT, double DeltaT, double MJD)
{
	for(unsigned int i=0;i<vActiveSoftware.size();i++)
		vActiveSoftware[i]->OnPostStep(SimT, DeltaT, MJD);
}

void SimpleGPCSystem::OnPropagate(double SimT, double DeltaT, double MJD)
{
	for(unsigned int i=0;i<vActiveSoftware.size();i++)
		vActiveSoftware[i]->OnPropagate(SimT, DeltaT, MJD);
}

bool SimpleGPCSystem::OnReadState(FILEHANDLE scn)
{
	char* line;
	char pszKey[256];
	SimpleGPCSoftware* pCurrentSoftware = NULL;
	while(oapiReadScenario_nextline(scn, line)) {
		if(!_strnicmp(line, "@ENDSUBSYSTEM", 13)) {
			return true;
		} else {
			if(!_strnicmp(line, "@ENDSOFTWARE", 12)) {
				pCurrentSoftware = NULL;
			}
			else {
				unsigned long i = 0;
				while(*line != ' ' && *line != '\0') {
					pszKey[i++] = *line;
					line++;
				}
				pszKey[i++] = '\0';
				if(*line == ' ') line++;
				if(!_strnicmp(pszKey, "@BEGINSOFTWARE", 14)) {
					pCurrentSoftware = FindSoftware(line);
				}
				else if(pCurrentSoftware) {
					if(*line != '\0') {
						pCurrentSoftware->OnParseLine(pszKey, line);
					} else {
						pCurrentSoftware->OnParseLine(pszKey, NULL);
					}
				}
				else {
					if(*line != '\0') {
						this->OnParseLine(pszKey, line);
					} else {
						this->OnParseLine(pszKey, NULL);
					}
				}
			}
		}
	}
	
	return false;
}

void SimpleGPCSystem::OnSaveState(FILEHANDLE scn) const
{
	for(unsigned int i=0;i<vSoftware.size();i++) {
		oapiWriteScenario_string(scn, "@BEGINSOFTWARE", const_cast<char*>(vSoftware[i]->GetIdentifier().c_str()));
		vSoftware[i]->OnSaveState(scn);
		oapiWriteScenario_string(scn, "@ENDSOFTWARE", "");
	}
}

bool SimpleGPCSystem::ItemInput(int spec, int item, const char* Data)
{
	for(unsigned int i=0;i<vActiveSoftware.size();i++) {
		if(vActiveSoftware[i]->ItemInput(spec, item, Data))
			return true;
	}
	return false;
}

bool SimpleGPCSystem::ExecPressed(int spec)
{
	for(unsigned int i=0;i<vActiveSoftware.size();i++) {
		if(vActiveSoftware[i]->ExecPressed(spec))
			return true;
	}
	return false;
}

bool SimpleGPCSystem::OnPaint(int spec, vc::MDU* pMDU) const
{
	for(unsigned int i=0;i<vActiveSoftware.size();i++) {
		if(vActiveSoftware[i]->OnPaint(spec, pMDU))
			return true;
	}
	return false;
}

bool SimpleGPCSystem::OnDrawHUD(const HUDPAINTSPEC* hps, oapi::Sketchpad* skp) const
{
	for(unsigned int i=0;i<vActiveSoftware.size();i++) {
		if(vActiveSoftware[i]->OnDrawHUD(hps, skp))
			return true;
	}
	return false;
}

SimpleGPCSoftware* SimpleGPCSystem::FindSoftware(const std::string& identifier) const
{
	for(unsigned int i=0;i<vSoftware.size();i++) {
		if(vSoftware[i]->GetIdentifier() == identifier) {
			return vSoftware[i];
		}
	}
	oapiWriteLog("ERROR: Could not find GPC software");
	oapiWriteLog(const_cast<char*>(identifier.c_str()));
	return NULL;
}

};