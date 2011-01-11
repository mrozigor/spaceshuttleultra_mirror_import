#pragma once

#include <OrbiterAPI.h>
#include <VesselAPI.h>

const char MESHNAME_EXTERIOR_1980[] = "SSU\\VAB_exterior_1980";
const char MESHNAME_INTERIOR[] = "SSU\\VAB_interior_structure_low-res";
const double BUILDING_COG_BASE = -70.0;

class VerticalDoorState {
	int iSegmentPos0;
	int iSegmentPos1;
	
public:
	VerticalDoorState(int s0, int s1);
	void Move(double fDeltaT); 
};

class VAB: public VESSEL3
{
	UINT midxExterior;
	UINT midxInterior;
	AnimState ldoor_state;
	AnimState rdoor_state;

	void DefineDoorAnimations(void);
	void DefineLightBeacons(void);
	void DefineLights(void);
	
	int Lua_InitInterpreter (void *context);
	int Lua_InitInstance (void *context);
public:
	VAB(OBJHANDLE hVessel, int iFlightModel);
	virtual ~VAB();
	virtual int clbkGeneric(int msgid = 0, int prm = 0, void* context = 0);
	virtual void clbkLoadStateEx(FILEHANDLE scn, void* status);
	virtual void clbkPostStep(double fSimT, double fDeltaT, double fMJD);
	virtual void clbkPreStep(double fSimT, double fDeltaT, double fMJD);
	virtual void clbkSaveState(FILEHANDLE scn);
	virtual void clbkSetClassCaps(FILEHANDLE cfg);
};