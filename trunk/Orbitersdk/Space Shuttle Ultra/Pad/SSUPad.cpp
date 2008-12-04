#include "SSUPad.h"
#include "meshres_FSS.h"

#define ORBITER_MODULE



VECTOR3 FSS_POS_LIGHT[FSS_NUM_LIGHTS] = {
	_V(-6.0, 20.06, 31.55),
	_V(-6.0, 26.25, 31.55),
	_V(-6.0, 32.65, 31.55),
	_V(-6.0, 37.05, 31.55),
	_V(-6.0, 43.45, 31.55),

	_V(-6.0, 50.70, 31.55),
	_V(-6.0, 56.70, 31.55),
	_V(-6.0, 62.70, 31.55),
	_V(-6.0, 68.70, 31.55),
	_V(-6.0, 74.70, 31.55),

	_V(-6.0, 80.70, 31.55),
	_V(-2.33, 20.06, 31.55),
	_V(-2.33, 26.25, 31.55),
	_V(-2.33, 32.65, 31.55),
	_V(-2.33, 37.05, 31.55),

	_V(-2.33, 43.45, 31.55),
	_V(-2.33, 50.70, 31.55),
	_V(-2.33, 56.70, 31.55),
	_V(-2.33, 62.70, 31.55),
	_V(-2.33, 68.70, 31.55),

	_V(-2.33, 74.70, 31.55),
	_V(-2.33, 80.70, 31.55),
	_V(0.0, 30.0, 0.0),
	_V(0.0, 40.0, 0.0),
	_V(0.0, 50.0, 0.0),

	_V(0.0, 10.0, 0.0),
	_V(0.0, 20.0, 0.0),
	_V(0.0, 30.0, 0.0),
	_V(0.0, 40.0, 0.0),
	_V(0.0, 50.0, 0.0)};

SSUPad::SSUPad(OBJHANDLE hVessel, int flightmodel)
	: VESSEL2(hVessel, flightmodel),
	fLightsOn(false)
{
	//add mesh
	VECTOR3 mesh_ofs=_V(0, 0, 0);
	mesh=oapiLoadMeshGlobal(DEFAULT_MESHNAME_FSS);
	mesh_idx=AddMesh(mesh, &mesh_ofs);
	//SetTouchdownPoints(_V(0.0, 1.0, 1.0), _V(-1.0, 1.0, -1.0), _V(1.0, 1.0, -1.0));
	SetTouchdownPoints(_V(1.0, -1.0, 0.0), _V(-1.0, -1.0, 1.0), _V(-1.0, -1.0, -1.0));

	phGOXVent = NULL;
	fNextLightUpdate = -20.0;

	GOXArmAction=AnimState::STOPPED;
	vtx_goxvent[0] = FSS_POS_GOXVENTL;
	vtx_goxvent[1] = FSS_POS_GOXVENTR;
	vtx_goxvent[2] = FSS_POS_GOXVENTDIR;
	DefineAnimations();
}

SSUPad::~SSUPad()
{
}

void SSUPad::CreateLights() {
	static VECTOR3& light_color = _V(1.0, 1.0, 1.0);
	for(unsigned int i = 0; i<FSS_NUM_LIGHTS; i++) {
		lights[i].duration = 0;
		lights[i].period = 0;
		lights[i].pos = &FSS_POS_LIGHT[i];
		lights[i].col = &light_color;
		lights[i].size = 0.05;
		lights[i].shape = BEACONSHAPE_COMPACT;
		lights[i].falloff = 0.8;
		lights[i].active = false;

		AddBeacon(&lights[i]);
	}
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

	static MGROUP_ROTATE GVA_VTX(LOCALVERTEXLIST, MAKEGROUPARRAY(vtx_goxvent), 3,
		_V(3.743, -6.87, 21.359), _V(0, -1, 0), (float)(74.5*RAD));

	ANIMATIONCOMPONENT_HANDLE parent=AddAnimationComponent(anim_gva, 0.0, 1.0, &GVA);
	AddAnimationComponent(anim_gva, 0.0, 1.0, &GVA_VTX);

	//GOX hood
	VentHoodState.Set(AnimState::OPEN, 1.0);
	anim_venthood=CreateAnimation(1.0);
	static UINT VentHoodGrp[1] = {GRP_Vent_hood};
	static MGROUP_ROTATE VentHood(mesh_idx, VentHoodGrp, 1,
		_V(-17.19, 83.344, 21.278), _V(0, 0, 1), (float)(41.0*RAD));
	parent=AddAnimationComponent(anim_venthood, 0.0, 1.0, &VentHood, parent);
}

void SSUPad::DisableLights() {
	fLightsOn = true;
	for(unsigned int i = 0; i<FSS_NUM_LIGHTS; i++) {
		lights[i].active = false;

		fLightsOn = fLightsOn && lights[i].active;
	}
}

void SSUPad::EnableLights() {
	fLightsOn = true;

	for(unsigned int i = 0; i<FSS_NUM_LIGHTS; i++) {
		lights[i].active = true;

		fLightsOn = fLightsOn && lights[i].active;
	}
}



bool SSUPad::IsDawn() const {
	
	OBJHANDLE Sun=NULL;
	int count=(int)oapiGetGbodyCount();
	for(int i=0;i<count;i++) {
		Sun=oapiGetGbodyByIndex(i);
		if(oapiGetObjectType(Sun)==OBJTP_STAR) break;
	}
	if(Sun) {
		VECTOR3 SunPosGlobal, SunPos;
		oapiGetGlobalPos(Sun, &SunPosGlobal);
		Global2Local(SunPosGlobal, SunPos);
		double angle=acos(SunPos.y/length(SunPos))*DEG;
		if(angle>85.0)
			return true;
	}
	return false;
}

void SSUPad::MoveOrbiterAccessArm(AnimState::Action action)
{
	if(action==AnimState::OPENING || action==AnimState::CLOSING)
		AccessArmState.action=action;
}

void SSUPad::MoveGOXArm(AnimState::Action action)
{
	if(action==AnimState::OPENING || action==AnimState::CLOSING) {
		GOXArmAction=action;
		GOXArmSequence();
	}
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

AnimState::Action SSUPad::GetAccessArmState()
{
	return AccessArmState.action;
}

AnimState::Action SSUPad::GetGOXArmState()
{
	return GOXArmAction;
}

void SSUPad::clbkPreStep(double simt, double simdt, double mjd)
{
	VESSEL2::clbkPreStep(simt, simdt, mjd);

	if(simt > fNextLightUpdate) {
		fNextLightUpdate = simt + 300.0;

		if(fLightsOn && !IsDawn()) {
			DisableLights();
		} 
		else if(!fLightsOn && IsDawn()) {
			EnableLights();
		}
	}

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

	UpdateGOXVentThrusters();
	
	double fGOXMass = GetPropellantMass(phGOXVent);
	if(fGOXMass > 0.0) {
		//Make vent level lag behind 
		double fFlow = max(fGOXMass/5.0, 0.01);
		SetThrusterLevel(thGOXVent[0], fFlow);
		SetThrusterLevel(thGOXVent[1], fFlow);

		if(fGOXMass > 0.0005) {
			SetPropellantMass(phGOXVent, max(fGOXMass - 5.0 * fFlow * simdt, 0.0));
		} else {
			SetPropellantMass(phGOXVent, 0.0);
		}
		
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

	CreateGOXVentThrusters();
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
		case OAPI_KEY_O:
			//Recharge GOX_Vent .. test
			SetPropellantMass(phGOXVent, 5.0);
			return 1;
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

void SSUPad::CreateGOXVentThrusters() {

	static PARTICLESTREAMSPEC gox_stream = {
	  0, 0.6, 140, 10, 0.025, 1.0, 1.1, 1.25, PARTICLESTREAMSPEC::DIFFUSE, 
	  PARTICLESTREAMSPEC::LVL_PSQRT, 0, 1, 
	  PARTICLESTREAMSPEC::ATM_PLOG, 1e-1140, 1
	  };

	gox_stream.tex = oapiRegisterParticleTexture ("Contrail1");


	VECTOR3 dir = (vtx_goxvent[2] - vtx_goxvent[0]);
	dir = dir/length(dir);

	if(phGOXVent == NULL) {
		phGOXVent = CreatePropellantResource(5.0);
		thGOXVent[0] = CreateThruster(vtx_goxvent[0], dir, 0.0, phGOXVent, 10.0, 8.0);
		thGOXVent[1] = CreateThruster(vtx_goxvent[1], dir, 0.0, phGOXVent, 10.0, 8.0);
		AddExhaustStream(thGOXVent[0], &gox_stream);
		AddExhaustStream(thGOXVent[1], &gox_stream);
	} else {
		UpdateGOXVentThrusters();
	}
}

void SSUPad::clbkSetClassCaps(FILEHANDLE cfg) {
	SetEmptyMass(6500000.0);
	SetSize(39.0);
	CreateGOXVentThrusters();
	CreateLights();
}

void SSUPad::UpdateGOXVentThrusters() {
	VECTOR3 dir = (vtx_goxvent[2] - vtx_goxvent[0]);
	dir = dir/length(dir);
	
	for(int i = 0; i<2; i++) {
		SetThrusterRef(thGOXVent[i], vtx_goxvent[i]);
		SetThrusterDir(thGOXVent[i], dir);
	}

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
