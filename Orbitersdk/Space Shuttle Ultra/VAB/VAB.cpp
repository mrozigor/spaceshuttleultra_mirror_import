#define ORBITER_MODULE
#include "VAB.h"

DLLCLBK VESSEL* ovcInit(OBJHANDLE hVessel, int iFlightModel)
{
	return new VAB(hVessel, iFlightModel);
}

DLLCLBK void ovcExit(VESSEL* pVessel)
{
	delete static_cast<VAB*>(pVessel);
}

VAB::VAB(OBJHANDLE hVessel, int iFlightModel)
: VESSEL3(hVessel, iFlightModel), midxExterior((UINT)(-1)),
	midxInterior((UINT)(-1))
{
	
}

VAB::~VAB()
{
}

int VAB::clbkGeneric(int msgid, int prm, void *context)
{
	switch(msgid)
	{
	case VMSG_LUAINTERPRETER:
		return Lua_InitInterpreter(context);
	case VMSG_LUAINSTANCE:
		return Lua_InitInstance(context);
	}
	return 0;
}

void VAB::clbkLoadStateEx(FILEHANDLE scn, void *status)
{
	char* line;
	while(oapiReadScenario_nextline(scn, line))
	{
		ParseScenarioLineEx(line, status);
	}
}

void VAB::clbkPostStep(double fSimT, double fDeltaT, double fMJD)
{
}

void VAB::clbkPreStep(double fSimT, double fDeltaT, double fMJD)
{
}

void VAB::clbkSaveState(FILEHANDLE scn)
{
	VESSEL3::clbkSaveState(scn);
}

void VAB::clbkSetClassCaps(FILEHANDLE cfg)
{
	//General parameters
	SetSize(50.0);
	SetEmptyMass(16500000);
	SetTouchdownPoints(_V(0.0, BUILDING_COG_BASE, 50.0), _V(-50.0, BUILDING_COG_BASE, -50.0),
		_V(50.0, BUILDING_COG_BASE, -50.0));

	//Create visual
	midxExterior = AddMesh(MESHNAME_EXTERIOR_1980);
	midxInterior = AddMesh(MESHNAME_INTERIOR);
	
	DefineDoorAnimations();
}

void VAB::DefineDoorAnimations(void)
{
}

void VAB::DefineLightBeacons(void)
{
}

void VAB::DefineLights(void)
{
}