#ifndef ATLANTIS_TANK_H
#define ATLANTIS_TANK_H
#pragma once

#include "orbitersdk.h"

//const static char* DEFAULT_MESHNAME_ET = "SSU/SLWT";
const static char* SWT_MESHNAME_ET = "SSU/SWT";
const static char* LWT_MESHNAME_ET = "SSU/SLWT";
const static char* SLWT_MESHNAME_ET = "SSU/SLWT";

//const static char* DEFAULT_NORMAL_ET_TEXTURE = "SSU/SLWT_ETtex.dds";
const static char* DEFAULT_SCORCHED_ET_TEXTURE = "SSU/ET-129_exterior_scorched.dds";

const double TANK_MAX_PROPELLANT_MASS = 719115.0;
// Main tank propellant mass [kg]

// Main tank empty mass
const double SWT_EMPTY_MASS = 35425.0;	//STS-1 to STS-5 tank 76000lb 35425.0kg
const double LWT_EMPTY_MASS = 29937.0;	//Light weight tank 66000lb 29937kg, STS-6 to STS-90
const double SLWT_EMPTY_MASS = 26535.0;	//Super light weight tank, 58500lb, STS-91 on

// ==========================================================
// Interface for derived vessel class: Atlantis_Tank
// ==========================================================

class Atlantis_Tank: public VESSEL2 {
public:
	Atlantis_Tank (OBJHANDLE hObj);
	// Construct interface from existing object

	// Overloaded callback functions
	void clbkVisualCreated(VISHANDLE vis, int refcount);
	void clbkVisualDestroyed(VISHANDLE vis, int refcount);
	void clbkSetClassCaps (FILEHANDLE cfg);
	void clbkPostStep (double simt, double simdt, double mjd);
	void clbkLoadStateEx(FILEHANDLE scn, void* status);	
	void clbkSaveState (FILEHANDLE scn);
	
	virtual void UseBurntETTexture();

private:
	virtual void UseETTexture(const char* pszTexName);

	VISHANDLE hVis;

	MESHHANDLE hTankMesh;
	UINT mesh_idx;
	bool bUseBurntTexture;

	// values specified in scenario
	double scenarioMass;
	char pszScenarioTexture[255];

	ATTACHMENTHANDLE ahToOrbiter;

	//////////////////////// ET vent ////////////////////////
	PROPELLANT_HANDLE phLOXtank;
	THRUSTER_HANDLE thLOXvent;
	double ventTimer;
	short ventCounter;
	double pos;
	//////////////////////// ET vent ////////////////////////
};

#endif