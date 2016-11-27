#include <cassert>
#include "SimpleGPCSystem.h"
#include "SimpleGPCSoftware.h"
#include "AscentDAP.h"
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
#include "ATVC_SOP.h"
#include "GeneralDisplays.h"
#include "MEC_SOP.h"
#include "RHC_RM.h"
#include "THC_RM.h"
#include "RPTA_RM.h"
#include "RHC_SOP.h"
#include "THC_SOP.h"
#include "RPTA_SOP.h"
#include "../Atlantis.h"

namespace dps
{

SimpleGPCSystem::SimpleGPCSystem(AtlantisSubsystemDirector* _director)
: AtlantisSubsystem(_director, "SimpleGPCSystem"), majorMode(101), newMajorMode(0)
{
	//TODO: Move this all to Partition model
	vSoftware.push_back( new MPS_Dump( this ) );
	vSoftware.push_back( new MPS_Dedicated_Display_Driver( this ) );
	vSoftware.push_back( new SSME_Operations( this ) );
	vSoftware.push_back( new SSME_SOP( this ) );
	vSoftware.push_back( new RSLS_old( this ) );
	vSoftware.push_back(new AscentDAP(this));
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
	vSoftware.push_back( new ATVC_SOP( this ) );
	vSoftware.push_back( new GeneralDisplays( this ) );
	vSoftware.push_back( new MEC_SOP( this ) );
	vSoftware.push_back( new RHC_RM( this ) );
	vSoftware.push_back( new THC_RM( this ) );
	vSoftware.push_back( new RPTA_RM( this ) );
	vSoftware.push_back( new RHC_SOP( this ) );
	vSoftware.push_back( new THC_SOP( this ) );
	vSoftware.push_back( new RPTA_SOP( this ) );
}

SimpleGPCSystem::~SimpleGPCSystem()
{
	for(unsigned int i=0;i<vSoftware.size();i++)
		delete vSoftware[i];
}

void SimpleGPCSystem::SetMajorMode(unsigned int newMM)
{
	//TODO: Move to Memory Configuration and Redundant Set (Partition)
	newMajorMode = newMM;
}

bool SimpleGPCSystem::IsValidMajorModeTransition(unsigned int newMajorMode) const
{
	//TODO: Replace by table in memory configuration
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
	case 901:
		return majorMode == 101;
	default:
		return false;
	}
}

bool SimpleGPCSystem::IsValidSPEC( int spec ) const
{
	switch (majorMode / 100)
	{
		case 0:
			switch (spec)
			{
				case 1:
				case 2:
				case 6:
				case 99:
					return true;
			}
			break;
		case 1:
			switch (spec)
			{
				case 0:
				case 1:
				case 6:
				case 18:
				case 19:// HACK BFS only
				case 23:
				case 50:
				case 51:
				case 53:
				case 55:
				case 99:
					return true;
				default:
					return false;
			}
			break;
		case 2:
			switch (spec)
			{
				case 0:
				case 1:
				case 2:
				case 6:
				case 18:
				case 19:
				case 20:
				case 21:
				case 22:
				case 23:
				case 25:
				case 33:
				case 34:
				case 55:
				case 99:
					return true;
				default:
					return false;
			}
			break;
		case 3:
			switch (spec)
			{
				case 0:
				case 1:
				case 6:
				case 18:
				case 19:// HACK BFS only
				case 21:
				case 22:
				case 23:
				case 50:
				case 51:
				case 53:
				case 55:
				case 99:
					return true;
				default:
					return false;
			}
			break;
		case 6:
			switch (spec)
			{
				case 0:
				case 1:
				case 6:
				case 18:
				case 19:// HACK BFS only
				case 23:
				case 50:
				case 51:
				case 53:
				case 55:
				case 99:
					return true;
				default:
					return false;
			}
			break;
		case 8:
			switch (spec)
			{
				case 0:
				case 1:
				case 2:
				case 6:
				case 18:
				case 19:
				case 23:
				case 40:
				case 41:
				case 42:
				case 43:
				case 44:
				case 45:
				case 55:
				case 99:
					return true;
				default:
					return false;
			}
		case 9:
			switch (spec)
			{
				case 0:
				case 1:
				case 2:
				case 6:
				case 55:
				case 62:
				case 99:
				case 100:
				case 101:
				case 102:
				case 104:
				case 105:
				case 106:
				case 112:
				case 113:
					return true;
				default:
					return false;
			}
			break;
	}
	return false;
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
	//TODO Save number of memory configuration

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
