#include "SSUPad.h"
#include "meshres_FSS.h"

#define ORBITER_MODULE

SSUPad::SSUPad(OBJHANDLE hVessel, int flightmodel)
	: VESSEL2(hVessel, flightmodel)
{
	//add mesh
	VECTOR3 mesh_ofs=_V(0, 0, 0);
	mesh=oapiLoadMeshGlobal(DEFAULT_MESHNAME_FSS);
	mesh_idx=AddMesh(mesh, &mesh_ofs);
	//SetTouchdownPoints(_V(0.0, 1.0, 1.0), _V(-1.0, 1.0, -1.0), _V(1.0, 1.0, -1.0));
	SetTouchdownPoints(_V(1.0, 1.0, 0.0), _V(-1.0, 1.0, 1.0), _V(-1.0, 1.0, -1.0));

	GOXArmAction=AnimState::STOPPED;
	DefineAnimations();
}

SSUPad::~SSUPad()
{
}

void SSUPad::DefineAnimations()
{
	//orbiter access arm
	static UINT AccessArmGrp[3] = {GRP_Orbiter_Access_Arm, GRP_White_Room_Mat, GRP_White_Room};
	static MGROUP_ROTATE AccessArm(mesh_idx, AccessArmGrp, 3,
		_V(-3.722, 63.144, 21.516), _V(0, -1, 0), (float)(72.0*RAD));
	AccessArmState.Set(AnimState::OPEN, 1.0);
	anim_accessarm=CreateAnimation(1.0);
	AddAnimationComponent(anim_accessarm, 0.0, 1.0, &AccessArm);

	//GOX arm
	GVAState.Set(AnimState::CLOSED, 0.0);
	anim_gva=CreateAnimation(0.0);
	static UINT GVAGrp[5] = {GRP_GOX_Cylinder06, GRP_GOX_Cylinder342,
		GRP_GOX_vent_arm_truss, GRP_North_GN2_vent_pipe, GRP_South_GN2_vent_pipe};
	static MGROUP_ROTATE GVA(mesh_idx, GVAGrp, 5,
		_V(3.743, -6.87, 21.359), _V(0, -1, 0), (float)(74.5*RAD));
	ANIMATIONCOMPONENT_HANDLE parent=AddAnimationComponent(anim_gva, 0.0, 1.0, &GVA);

	//GOX hood
	VentHoodState.Set(AnimState::OPEN, 1.0);
	anim_venthood=CreateAnimation(1.0);
	static UINT VentHoodGrp[1] = {GRP_Vent_hood};
	static MGROUP_ROTATE VentHood(mesh_idx, VentHoodGrp, 1,
		_V(-17.19, 83.344, 21.278), _V(0, 0, 1), (float)(41.0*RAD));
	parent=AddAnimationComponent(anim_venthood, 0.0, 1.0, &VentHood, parent);
}

void SSUPad::GOXArmSequence()
{
	if(GOXArmAction==AnimState::OPENING) {
		if(!Eq(GVAState.pos, 1.0)) {
			if(!Eq(VentHoodState.pos, 0.0)) {
				VentHoodState.action=AnimState::CLOSING;
				GVAState.action=AnimState::STOPPED;
			}
			else {
				GVAState.action=AnimState::OPENING;
				VentHoodState.action=AnimState::CLOSED;
			}
		}
		else if(!Eq(VentHoodState.pos, 1.0)) {
				VentHoodState.action=AnimState::OPENING;
				GVAState.action=AnimState::OPEN;
		}
		else GOXArmAction=AnimState::OPEN;
	}
	else if(GOXArmAction==AnimState::CLOSING) {
		if(!Eq(VentHoodState.pos, 0.0)) {
			VentHoodState.action=AnimState::CLOSING;
			GVAState.action=AnimState::STOPPED;
		}
		else if(!Eq(GVAState.pos, 0.0)) {
			GVAState.action=AnimState::CLOSING;
			VentHoodState.action=AnimState::CLOSED;
		}
		else GOXArmAction=AnimState::CLOSED;
	}
}

void SSUPad::clbkPreStep(double simt, double simdt, double mjd)
{
	if(AccessArmState.Moving()) {
		double dp=simdt*ORBITER_ACCESS_ARM_RATE;
		AccessArmState.Move(dp);
		SetAnimation(anim_accessarm, AccessArmState.pos);
	}
	
	if(GVAState.Moving()) {
		double dp=simdt*GVA_RATE;
		GVAState.Move(dp);
		SetAnimation(anim_gva, GVAState.pos);
		if(GOXArmAction>=AnimState::CLOSING && !GVAState.Moving()) GOXArmSequence();
	}

	if(VentHoodState.Moving()) {
		double dp=simdt*VENT_HOOD_RATE;
		VentHoodState.Move(dp);
		SetAnimation(anim_venthood, VentHoodState.pos);
		if(GOXArmAction>=AnimState::CLOSING && !VentHoodState.Moving()) GOXArmSequence();
	}
}

void SSUPad::clbkSaveState(FILEHANDLE scn)
{
	VESSEL2::clbkSaveState(scn); //default parameters

	WriteScenario_state(scn, "ACCESS_ARM", AccessArmState);
	WriteScenario_state(scn, "GVA", GVAState);
	WriteScenario_state(scn, "VENTHOOD", VentHoodState);
	oapiWriteScenario_int(scn, "GOX_SEQUENCE", GOXArmAction);
}

void SSUPad::clbkLoadStateEx(FILEHANDLE scn, void *status)
{
	char* line;

	while(oapiReadScenario_nextline(scn, line)) {
		if(!_strnicmp(line, "ACCESS_ARM", 10)) {
			sscan_state(line+10, AccessArmState);
			SetAnimation(anim_accessarm, AccessArmState.pos);
		}
		else if(!_strnicmp(line, "GVA", 3)) {
			sscan_state(line+3, GVAState);
			SetAnimation(anim_gva, GVAState.pos);
		}
		else if(!_strnicmp(line, "VENTHOOD", 8)) {
			sscan_state(line+8, VentHoodState);
			SetAnimation(anim_venthood, VentHoodState.pos);
		}
		else if(!_strnicmp(line, "GOX_SEQUENCE", 12)) {
			sscanf(line+12, "%d", &GOXArmAction);
		}
		else ParseScenarioLineEx(line, status);
	}
}

int SSUPad::clbkConsumeBufferedKey(DWORD key, bool down, char *keystate)
{
	if(!down) return 0; //only handle keydown events

	switch(key) {
		case OAPI_KEY_K:
			if(AccessArmState.Open() || AccessArmState.Opening()) AccessArmState.action=AnimState::CLOSING;
			else AccessArmState.action=AnimState::OPENING;
			return 1;
			break;
		case OAPI_KEY_G:
			//if(GVAState.Open() || GVAState.Opening()) GVAState.action=AnimState::CLOSING;
			//else GVAState.action=AnimState::OPENING;
			if(GOXArmAction==AnimState::CLOSED || GOXArmAction==AnimState::CLOSING)
				GOXArmAction=AnimState::OPENING;
			else GOXArmAction=AnimState::CLOSING;
			GOXArmSequence();
			return 1;
			break;
	}
	return 0;
}

DLLCLBK void InitModule(HINSTANCE hDLL)
{
}

DLLCLBK void ExitModule(HINSTANCE hDLL)
{
}

DLLCLBK VESSEL* ovcInit(OBJHANDLE hVessel, int iFlightModel)
{
	return new SSUPad(hVessel, iFlightModel);
}

DLLCLBK void ovcExit(VESSEL* pVessel)
{
	delete static_cast<SSUPad*>(pVessel);
}