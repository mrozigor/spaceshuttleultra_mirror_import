#define ORBITER_MODULE
#include "VAB.h"
#include "meshres.h"

const UINT VAB_HIGHBAY1_GROUPS[11] = {
	GRP_HB1HORIZONTALDOORL1, GRP_HB1HORIZONTALDOORL2,
	GRP_HB1HORIZONTALDOORR1, GRP_HB1HORIZONTALDOORR2,
	GRP_HB1VLD1, GRP_HB1VLD2, GRP_HB1VLD3,
	GRP_HB1VLD4, GRP_HB1VLD5, GRP_HB1VLD6,
	GRP_HB1VLD7
	};

const UINT VAB_HIGHBAY2_GROUPS[11] = {
	GRP_HB2HORIZONTALDOORL1, GRP_HB2HORIZONTALDOORL2,
	GRP_HB2HORIZONTALDOORR1, GRP_HB2HORIZONTALDOORR2,
	GRP_HB2VLD1, GRP_HB2VLD2, GRP_HB2VLD3,
	GRP_HB2VLD4, GRP_HB2VLD5, GRP_HB2VLD6,
	GRP_HB2VLD7
	};


const UINT VAB_HIGHBAY3_GROUPS[11] = {
	GRP_HB3HORIZONTALDOORL1, GRP_HB3HORIZONTALDOORL2,
	GRP_HB3HORIZONTALDOORR1, GRP_HB3HORIZONTALDOORR2,
	GRP_HB3VLD1, GRP_HB3VLD2, GRP_HB3VLD3,
	GRP_HB3VLD4, GRP_HB3VLD5, GRP_HB3VLD6,
	GRP_HB3VLD7
	};

const UINT VAB_HIGHBAY4_GROUPS[11] = {
	GRP_HB4HORIZONTALDOORL1, GRP_HB4HORIZONTALDOORL2,
	GRP_HB4HORIZONTALDOORR1, GRP_HB4HORIZONTALDOORR2,
	GRP_HB4VLD1, GRP_HB4VLD2, GRP_HB4VLD3,
	GRP_HB4VLD4, GRP_HB4VLD5, GRP_HB4VLD6,
	GRP_HB4VLD7
	};


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
	for(unsigned int i = 0; i<4; i++)
		highbay[i].execute(fDeltaT);
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
	highbay[0].init(this, midxExterior, _V(1.0, 0.0, 0.0), VAB_HIGHBAY1_GROUPS);
	highbay[1].init(this, midxExterior, _V(1.0, 0.0, 0.0), VAB_HIGHBAY2_GROUPS);
	highbay[2].init(this, midxExterior, _V(-1.0, 0.0, 0.0), VAB_HIGHBAY3_GROUPS);
	highbay[3].init(this, midxExterior, _V(-1.0, 0.0, 0.0), VAB_HIGHBAY4_GROUPS);
}

void VAB::DefineLightBeacons(void)
{
}

void VAB::DefineLights(void)
{
}

bool VAB::openTo(unsigned int hb, unsigned int segpos)
{

	return false;
}