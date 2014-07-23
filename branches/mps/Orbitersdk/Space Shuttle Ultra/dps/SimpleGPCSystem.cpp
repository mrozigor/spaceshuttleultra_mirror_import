#include <cassert>
#include "SimpleGPCSystem.h"
#include "SimpleGPCSoftware.h"
#include "AscentGuidance.h"
#include "OrbitDAP.h"
#include "OMSBurnSoftware.h"
#include "StateVectorSoftware.h"
#include "OrbitTgtSoftware.h"
#include "AerojetDAP.h"
#include "SSME_SOP.h"
#include "SSME_Operations.h"
#include "RSLS_old.h"
#include "MPS_Dedicated_Display_Driver.h"
#include "MPS_Dump.h"
#include "MM801.h"
#include "IO_Control.h"
#include "TransitionDAP.h"
#include "ETSepSequence.h"
#include "SRBSepSequence.h"
#include "../Atlantis.h"

namespace dps
{

SimpleGPCSystem::SimpleGPCSystem(AtlantisSubsystemDirector* _director)
: AtlantisSubsystem(_director, "SimpleGPCSystem"), majorMode(101), newMajorMode(0)
{
	vSoftware.push_back( new MPS_Dump( this ) );
	vSoftware.push_back( new MPS_Dedicated_Display_Driver( this ) );
	vSoftware.push_back( new SSME_Operations( this ) );
	vSoftware.push_back( new SSME_SOP( this ) );
	vSoftware.push_back( new RSLS_old( this ) );
	vSoftware.push_back(new AscentGuidance(this));
	vSoftware.push_back(new OrbitDAP(this));
	vSoftware.push_back(new StateVectorSoftware(this));
	vSoftware.push_back(new OMSBurnSoftware(this));
	vSoftware.push_back(new OrbitTgtSoftware(this));
	vSoftware.push_back(new AerojetDAP(this));
	vSoftware.push_back(new MM801(this));
	vSoftware.push_back( new IO_Control( this ) );
	vSoftware.push_back( new TransitionDAP( this ) );
	vSoftware.push_back( new ETSepSequence( this ) );
	vSoftware.push_back( new SRBSepSequence( this ) );
}

SimpleGPCSystem::~SimpleGPCSystem()
{
	for(unsigned int i=0;i<vSoftware.size();i++)
		delete vSoftware[i];
}

void SimpleGPCSystem::SetMajorMode(unsigned int newMM)
{
	newMajorMode = newMM;
}

bool SimpleGPCSystem::IsValidMajorModeTransition(unsigned int newMajorMode) const
{
	switch(newMajorMode) {
	case 104:
		return majorMode == 103;
	case 105:
		return majorMode == 104;
	case 106:
		return majorMode == 105;
	case 201:
		return (majorMode == 106 || majorMode == 202 || majorMode == 301 || majorMode == 801);
	case 202:
		return majorMode == 201;
	case 301:
		return (majorMode == 104 || majorMode == 105 || majorMode == 106 || majorMode == 201 || majorMode == 302);
	case 302:
		return majorMode == 301;
	case 303:
		return majorMode == 302;
	case 304:
		return majorMode == 303;
	case 305:
		return majorMode == 304;
	case 801:
		return majorMode == 201;
	default:
		return false;
	}
}

void SimpleGPCSystem::Realize()
{
	for(unsigned int i=0;i<vSoftware.size();i++)
		vSoftware[i]->Realize();
}

void SimpleGPCSystem::OnPreStep(double SimT, double DeltaT, double MJD)
{
	// if major mode changed sometime in the last timestep, update major mode
	if(newMajorMode != 0) {
		vActiveSoftware.clear();
		for(unsigned int i=0;i<vSoftware.size();i++) {
			if(vSoftware[i]->OnMajorModeChange(newMajorMode))
				vActiveSoftware.push_back(vSoftware[i]);
		}
		majorMode = newMajorMode;
		newMajorMode = 0;
	}

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
	/*for(unsigned int i=0;i<vSoftware.size();i++) {
		oapiWriteScenario_string(scn, "@BEGINSOFTWARE", const_cast<char*>(vSoftware[i]->GetIdentifier().c_str()));
		vSoftware[i]->OnSaveState(scn);
		oapiWriteScenario_string(scn, "@ENDSOFTWARE", "");
	}*/
	for(unsigned int i=0;i<vActiveSoftware.size();i++) {
		oapiWriteScenario_string(scn, "@BEGINSOFTWARE", const_cast<char*>(vActiveSoftware[i]->GetIdentifier().c_str()));
		vActiveSoftware[i]->OnSaveState(scn);
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
