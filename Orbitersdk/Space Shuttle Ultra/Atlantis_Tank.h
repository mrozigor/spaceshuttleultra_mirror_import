#ifndef ATLANTIS_TANK_H
#define ATLANTIS_TANK_H
#pragma once

#include "orbitersdk.h"

const static char* DEFAULT_MESHNAME_ET = "SSU/ET129";

const static char* DEFAULT_NORMAL_ET_TEXTURE = "SSU/ET-129_exterior_launch.dds";
const static char* DEFAULT_SCORCHED_ET_TEXTURE = "SSU/ET-129_exterior_scorched.dds";

const double TANK_MAX_PROPELLANT_MASS = 719115.0;
// Main tank propellant mass [kg]

const double TANK_EMPTY_MASS = 26535; //Super light weight tank, 58500lb, STS-91 on
                                      //STS-1 to STS-5 tank 76000lb 35425.0kg
                                      //Light weight tank 66000lb 29937kg, STS-6 to STS-90
// Main tank empty mass

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

	virtual void TurnOnPadLights() const;
	virtual void TurnOffPadLights() const;

private:
	VISHANDLE hVis;

	MESHHANDLE hTankMesh;
	UINT mesh_idx;
	bool bUseBurntTexture;

	ATTACHMENTHANDLE ahToOrbiter;

	//////////////////////// ET vent ////////////////////////
	PROPELLANT_HANDLE phLOXtank;
	THRUSTER_HANDLE thLOXvent;
	double timer;
	short counter;
	double pos;
	//////////////////////// ET vent ////////////////////////
};

#endif