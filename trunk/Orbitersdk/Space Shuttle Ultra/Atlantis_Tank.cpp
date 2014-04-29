// ==============================================================
//                 ORBITER MODULE: Atlantis
//                  Part of the ORBITER SDK
//          Copyright (C) 2001-2003 Martin Schweiger
//                   All rights reserved
//
// Atlantis_Tank.cpp
// Reference implementation of Atlantis Tank vessel class module
// Note: The propellant tanks associated with the ET are defined
// by the Atlantis class. This module is attached to the orbiter
// during launch and takes control of the tank after separation
// from the orbiter.
// ==============================================================

#define ORBITER_MODULE
//#include "Atlantis.h"
#include "Atlantis_Tank.h"
#include "CommonDefs.h"
#include "math.h"
#include <string>

// ==============================================================
// Specialised vessel class Atlantis_Tank
// ==============================================================

// Constructor
Atlantis_Tank::Atlantis_Tank (OBJHANDLE hObj)
: VESSEL2(hObj), bUseBurntTexture(false), scenarioMass(-1.0)
{
	// preload mesh
	mesh_idx = MESH_UNDEFINED;
	//hTankMesh = oapiLoadMeshGlobal (DEFAULT_MESHNAME_ET);

	//////////////////////// ET vent ////////////////////////
	ventTimer = -1;
	ventCounter = 50;
	pos = 0;
	//////////////////////// ET vent ////////////////////////
}

void Atlantis_Tank::UseBurntETTexture()
{
	bUseBurntTexture = true;
	if(strScenarioBurnTex.empty()) UseETTexture(DEFAULT_SCORCHED_ET_TEXTURE);
	else UseETTexture(strScenarioBurnTex.c_str());
}

void Atlantis_Tank::UseETTexture(const char* pszTexName)
{
	SURFHANDLE texture = oapiLoadTexture(pszTexName);
	if(!oapiSetTexture(hDevTankMesh, 1, texture))
		oapiWriteLog("(Atlantis_Tank) ERROR: Could not update texture");
}

// ==============================================================
// Callback functions
// ==============================================================

// Set Tank class specs
void Atlantis_Tank::clbkSetClassCaps (FILEHANDLE cfg)
{
	SetEnableFocus (false);
	// Tank cannot receive input focus

	SetSize (24.0);

	// see what type of tank this is
	char pszBuffer[255];
	bool bFoundData = oapiReadItem_string(cfg, "Type", pszBuffer);
	if(!bFoundData || !_strnicmp(pszBuffer, "SLWT", 4)) { // default to SLWT tank
		SetEmptyMass (SLWT_EMPTY_MASS);
		hTankMesh = oapiLoadMeshGlobal (SLWT_MESHNAME_ET);
	}
	else if(!_strnicmp(pszBuffer, "LWT", 3)) {
		SetEmptyMass (LWT_EMPTY_MASS);
		hTankMesh = oapiLoadMeshGlobal (LWT_MESHNAME_ET);
	}
	else if(!_strnicmp(pszBuffer, "SWT", 3)) {
		SetEmptyMass (SWT_EMPTY_MASS);
		hTankMesh = oapiLoadMeshGlobal (SWT_MESHNAME_ET);
	}
	//SetEmptyMass (TANK_EMPTY_MASS);
	//SetEmptyMass (SLWT_EMPTY_MASS);

	//SetMaxFuelMass (TANK_MAX_PROPELLANT_MASS);
	// Note that the Tank instance is only created after separation from
	// the orbiter, so the actual fuel mass will always be much smaller

	SetISP (5000.0);

	SetMaxThrust (ENGINE_MAIN, 0);
	SetMaxThrust (ENGINE_RETRO, 0);
	SetMaxThrust (ENGINE_HOVER, 0);
	SetMaxThrust (ENGINE_ATTITUDE, 0);
	// Tank has no engines of its own

	SetCW (0.2, 0.3, 1.2, 1.2);
	VECTOR3 cs = {412.1,411.8,72.7};
	SetCrossSections (cs);
	VECTOR3 rd = {0.5,0.5,0.1};
	SetRotDrag (rd);
	VECTOR3 pmi = {145.6,145.6,10.5};
	SetPMI (pmi);
	SetPitchMomentScale (1e-4);
	SetBankMomentScale (1e-4);

	VECTOR3 co = {0,0,0};
	SetCameraOffset (co);
	// Note that the camera offset should not be required
	// since the Tank doesn't define a 'cockpit'

	SetCOG_elev (-5.0);
	SetTouchdownPoints (_V(0,9,3), _V(-1,1,-3), _V(1,1,-3));
	SetLiftCoeffFunc (0);

	//////////////////////// ET vent ////////////////////////
	phLOXtank = CreatePropellantResource( 400 );
	thLOXvent = CreateThruster( _V( -3.12321, -2.36732, 13.4955 ), _V( 0.788011, 0.615661, 0 ), 600, phLOXtank, 300, 300 );
	PARTICLESTREAMSPEC psLOXvent = {
		0,
		0.1,
		2,
		50,
		0,
		1,
		25,
		1,
		PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_PLIN,
		0.75, 1,// match the #1 value with the pulse % below...
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0};
	
	AddExhaustStream( thLOXvent, &psLOXvent );
	//////////////////////// ET vent ////////////////////////

	mesh_idx = AddMesh (hTankMesh);

	//ahToOrbiter = CreateAttachment(true, _V(0, -0.25, 7.5), _V(0, -1, 0), _V(0, 0, 1), "SSU_ET");
	ahToOrbiter = CreateAttachment(true, _V(0, 0.0, 0.0), _V(0, -1, 0), _V(0, 0, 1), "SSU_ET");
}

// Simulation time step
void Atlantis_Tank::clbkPostStep (double simt, double simdt, double mjd)
{
	if (GetAltitude() < 0.0) oapiDeleteVessel (GetHandle());

	//////////////////////// ET vent ////////////////////////
	if (ventTimer < 0.0 && !GetAttachmentStatus(ahToOrbiter)) {
		ventTimer = simt + 120;// start vent 120 sec after ET Sep
		SetPropellantMass(phLOXtank, 400.0); // make sure 'tank' is full for venting
	}

	if(ventTimer > 0.0) {
		if (ventCounter >= (int)((1 - pos) * 50))
		{
			pos = (simt * 10) - (int)(simt * 10);// new valve position
			ventCounter = 0;
		}

		if ((simt - ventTimer) > 0)
		{
			ventCounter++;
			if (pos <= 0.75)// close it if under 75% so it pulses... looks cool!
			{
				SetThrusterLevel( thLOXvent, 0 );
			}
			else
			{
				SetThrusterLevel( thLOXvent, pos );
			}
		}
		else
		{
			SetThrusterLevel( thLOXvent, 0 );
		}
	}
	//////////////////////// ET vent ////////////////////////
}

void Atlantis_Tank::clbkLoadStateEx(FILEHANDLE scn, void *status)
{
	char* line;

	while(oapiReadScenario_nextline(scn, line)) {
		if(!_strnicmp(line, "USE_BURNT_TEX", 13)) {
			bUseBurntTexture = true;
		}
		else if(!_strnicmp(line, "EMPTY_MASS", 10)) {
			sscanf_s(line+10, "%lf", scenarioMass);
			SetEmptyMass(scenarioMass);
		}
		else if(!_strnicmp(line, "ET_TEX_NAME", 11)) {
			strScenarioTextureName = line+12;
			strScenarioTexture = "SSU\\" + strScenarioTextureName + ".dds";
		}
		else if(!_strnicmp(line, "BURNT_TEX_NAME", 14)) {
			strScenarioBurnTexName = line+15;
			strScenarioBurnTex = "SSU\\" + strScenarioBurnTexName + ".dds";
		}
		else ParseScenarioLineEx(line, status);
	}
}

void Atlantis_Tank::clbkSaveState(FILEHANDLE scn)
{
	VESSEL2::clbkSaveState(scn);

	if(bUseBurntTexture) oapiWriteLine(scn, "  BURNT_TEX");
	if(scenarioMass>0.0) oapiWriteScenario_float(scn, "EMPTY_MASS", scenarioMass);
	if(!strScenarioTextureName.empty()) oapiWriteScenario_string(scn, "ET_TEX_NAME", const_cast<char*>(strScenarioTextureName.c_str()));
	if(!strScenarioBurnTexName.empty()) oapiWriteScenario_string(scn, "BURNT_TEX_NAME", const_cast<char*>(strScenarioBurnTexName.c_str()));
}

void Atlantis_Tank::clbkVisualCreated(VISHANDLE vis, int refcount)
{
	hVis = vis;

	hDevTankMesh = GetDevMesh(vis, mesh_idx);

	if(bUseBurntTexture) UseBurntETTexture();
	if(!strScenarioTexture.empty()) UseETTexture(strScenarioTexture.c_str());
}

void Atlantis_Tank::clbkVisualDestroyed(VISHANDLE vis, int refcount)
{
	if(vis == hVis) hVis = NULL;
}

// ==============================================================
// API interface
// ==============================================================

// Initialisation
DLLCLBK VESSEL *ovcInit (OBJHANDLE hvessel, int flightmodel)
{
	return new Atlantis_Tank (hvessel);
}

// Cleanup
DLLCLBK void ovcExit (VESSEL *vessel)
{
	if (vessel) delete (Atlantis_Tank*)vessel;
}

// Vessel status initialisation
DLLCLBK void ovcSetState (VESSEL *vessel, const VESSELSTATUS *status)
{
	vessel->DefSetState (status);	        // default initialisation
}
