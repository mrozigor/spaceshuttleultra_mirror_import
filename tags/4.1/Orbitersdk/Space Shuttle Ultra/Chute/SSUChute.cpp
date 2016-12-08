#include "SSUChute.h"
#include "../Atlantis_defs.h"

#define ORBITER_MODULE

SSUChute::SSUChute(OBJHANDLE hVessel, int flightmodel)
	: VESSEL2(hVessel, flightmodel)
{
	SetEmptyMass(10.0);
	VECTOR3 mesh_ofs= _V(0, 0, 0);

	mesh=oapiLoadMeshGlobal(DEFAULT_MESHNAME_CHUTE);
	mesh_idx=AddMesh(mesh, &mesh_ofs);

	DefineAnimations();
	CreateVariableDragElement(&anim_collapse_state, 45, _V(0, 0, 0));
}

SSUChute::~SSUChute()
{
}

void SSUChute::DefineAnimations()
{
	anim_collapse_state=1.0;
	anim_collapse=CreateAnimation(1.0);
	static MGROUP_SCALE collapse (mesh_idx, NULL, 4, _V(0, 0, 0), _V(1, 100, 1));
	AddAnimationComponent(anim_collapse, 0, 1, &collapse);
}

void SSUChute::clbkPreStep(double simt, double simdt, double mjd)
{
	if(anim_collapse_state>0.0) {
		anim_collapse_state=max(anim_collapse_state-simdt*CHUTE_DEFLATE_TIME, 0.0);
		SetAnimation(anim_collapse, anim_collapse_state);
	}
}

void SSUChute::clbkLoadStateEx(FILEHANDLE scn, void *status)
{
	char* line;

	while(oapiReadScenario_nextline(scn, line)) {
		if(!_strnicmp(line, "CHUTE_INFLATION", 15)) {
			sscanf_s(line+15, "%lf", &anim_collapse_state);
			SetAnimation(anim_collapse, anim_collapse_state);
		}
		else ParseScenarioLineEx(line, status);
	}
}

void SSUChute::clbkSaveState(FILEHANDLE scn)
{
	VESSEL2::clbkSaveState(scn); //default parameters
	oapiWriteScenario_float(scn, "CHUTE_INFLATION", anim_collapse_state);
}

DLLCLBK void InitModule(HINSTANCE hDLL)
{
}

DLLCLBK void ExitModule(HINSTANCE hDLL)
{
}

DLLCLBK VESSEL* ovcInit(OBJHANDLE hVessel, int iFlightModel)
{
	return new SSUChute(hVessel, iFlightModel);
}

DLLCLBK void ovcExit(VESSEL* pVessel)
{
	delete static_cast<SSUChute*>(pVessel);
}
