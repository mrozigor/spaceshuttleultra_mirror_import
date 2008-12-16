#include "SSUPad.h"
#include "meshres_FSS.h"
#include "meshres_RSS.h"

#define ORBITER_MODULE



VECTOR3 FSS_POS_LIGHT[FSS_NUM_LIGHTS] = {
	_V(-4.5, 20.06, 32.55),
	_V(-4.5, 26.25, 32.55),
	_V(-4.5, 32.65, 32.55),
	_V(-4.5, 37.05, 32.55),
	_V(-4.5, 43.45, 32.55),

	_V(-4.5, 50.70, 32.55),
	_V(-4.5, 56.70, 32.55),
	_V(-4.5, 62.70, 32.55),
	_V(-4.5, 68.70, 32.55),
	_V(-4.5, 74.70, 32.55),

	_V(-4.5, 80.70, 32.55),
	_V(-0.85, 20.06, 32.55),
	_V(-0.85, 26.25, 32.55),
	_V(-0.85, 32.65, 32.55),
	_V(-0.85, 37.05, 32.55),

	_V(-0.85, 43.45, 32.55),
	_V(-0.85, 50.70, 32.55),
	_V(-0.85, 56.70, 32.55),
	_V(-0.85, 62.70, 32.55),
	_V(-0.85, 68.70, 32.55),

	_V(-0.85, 74.70, 32.55),
	_V(-0.85, 80.70, 32.55),
	_V(-4.5, 20.06, 27.55),
	_V(-4.5, 26.25, 27.55),
	_V(-4.5, 32.65, 27.55),

	_V(-4.5, 37.05, 27.55),
	_V(-4.5, 43.45, 27.55),
	_V(-4.5, 50.70, 27.55),
	_V(-4.5, 56.70, 27.55),
	_V(-4.5, 62.70, 27.55),
	_V(-4.5, 68.70, 27.55),
	_V(-4.5, 74.70, 27.55),
	_V(-4.5, 80.70, 27.55),
	
	_V(-0.85, 20.06, 27.55),
	_V(-0.85, 26.25, 27.55),
	_V(-0.85, 32.65, 27.55),

	_V(-0.85, 37.05, 27.55),
	_V(-0.85, 43.45, 27.55),
	_V(-0.85, 50.70, 27.55),
	_V(-0.85, 56.70, 27.55),
	_V(-0.85, 62.70, 27.55),
	_V(-0.85, 68.70, 27.55),
	_V(-0.85, 74.70, 27.55),
	_V(-0.85, 80.70, 27.55)};

SSUPad::SSUPad(OBJHANDLE hVessel, int flightmodel)
	: VESSEL2(hVessel, flightmodel),
	fLightsOn(false)
{
	//add meshes
	FSSMesh=oapiLoadMeshGlobal(DEFAULT_MESHNAME_FSS);
	fss_mesh_idx=AddMesh(FSSMesh);
	VECTOR3 rss_ofs=_V(0.0, 1.0, 1.5);
	RSSMesh=oapiLoadMeshGlobal(DEFAULT_MESHNAME_RSS);
	rss_mesh_idx=AddMesh(RSSMesh, &rss_ofs);

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
	static VECTOR3& light_color = _V(1.0, 0.972, 0.219);
	for(unsigned int i = 0; i<FSS_NUM_LIGHTS; i++) {
		lights[i].duration = 0;
		lights[i].period = 0;
		lights[i].pos = &FSS_POS_LIGHT[i];
		lights[i].col = &light_color;
		lights[i].size = 0.2;
		lights[i].shape = BEACONSHAPE_DIFFUSE;
		lights[i].falloff = 0.8;
		lights[i].active = false;

		AddBeacon(&lights[i]);
	}
}

void SSUPad::DefineAnimations()
{
	//orbiter access arm
	static UINT AccessArmGrp[3] = {GRP_Orbiter_Access_Arm, GRP_White_Room_Mat, GRP_White_Room};
	static MGROUP_ROTATE AccessArm(fss_mesh_idx, AccessArmGrp, 3,
		_V(-3.722, 63.144, 21.516), _V(0, -1, 0), (float)(72.0*RAD));
	AccessArmState.Set(AnimState::CLOSED, 0.0);
	anim_accessarm=CreateAnimation(0.0);
	AddAnimationComponent(anim_accessarm, 0.0, 1.0, &AccessArm);

	//GOX arm
	GVAState.Set(AnimState::CLOSED, 0.0);
	anim_gva=CreateAnimation(0.0);
	static UINT GVAGrp[5] = {GRP_GOX_Cylinder06, GRP_GOX_Cylinder342,
		GRP_GOX_vent_arm_truss, GRP_North_GN2_vent_pipe, GRP_South_GN2_vent_pipe};
	static MGROUP_ROTATE GVA(fss_mesh_idx, GVAGrp, 5,
		_V(3.743, -6.87, 21.359), _V(0, -1, 0), (float)(74.5*RAD));

	static MGROUP_ROTATE GVA_VTX(LOCALVERTEXLIST, MAKEGROUPARRAY(vtx_goxvent), 3,
		_V(3.743, -6.87, 21.359), _V(0, -1, 0), (float)(74.5*RAD));

	ANIMATIONCOMPONENT_HANDLE parent=AddAnimationComponent(anim_gva, 0.0, 1.0, &GVA);
	AddAnimationComponent(anim_gva, 0.0, 1.0, &GVA_VTX);

	//GOX hood
	VentHoodState.Set(AnimState::OPEN, 1.0);
	anim_venthood=CreateAnimation(1.0);
	static UINT VentHoodGrp[1] = {GRP_Vent_hood};
	static MGROUP_ROTATE VentHood(fss_mesh_idx, VentHoodGrp, 1,
		_V(-17.19, 83.344, 21.278), _V(0, 0, 1), (float)(41.0*RAD));
	parent=AddAnimationComponent(anim_venthood, 0.0, 1.0, &VentHood, parent);

	//RSS rotation
	RSS_State.Set(AnimState::CLOSED, 0.0);
	static MGROUP_ROTATE RSS_Retract(rss_mesh_idx, NULL, 0,
		_V(-15.8, 0, 21.5), _V(0.0, 1.0, 0.0), (float)(120.0*RAD));
	anim_rss=CreateAnimation(0.0);
	AddAnimationComponent(anim_rss, 0.0, 1.0, &RSS_Retract);
	//SetAnimation(anim_rss, 1.0);

	//RSS OWP
	RSS_Y_OWP_State.Set(AnimState::CLOSED, 0.0);
	static UINT RSS_Y_OWPGrp[2] = {GRP_RSS_Plus_Y_OWP_Lower, GRP_Box73};
	static MGROUP_TRANSLATE RSS_Y_OWP(rss_mesh_idx, RSS_Y_OWPGrp, 2, _V(0.0, 0.0, 11.7));
	anim_rss_y_owp=CreateAnimation(0.0);
	AddAnimationComponent(anim_rss_y_owp, 0.0, 1.0, &RSS_Y_OWP);
	//SetAnimation(anim_rss_y_owp, 1.0);

	//FSS OWP
	FSS_Y_OWP_State.Set(AnimState::CLOSED, 0.0);
	static UINT FSS_Y_OWPRotGrp[3] = {GRP_Inner_FSS_WPS_panel_track, GRP_Outer_WPS_panel, GRP_Y_FSS_WPS_struts};
	static MGROUP_ROTATE FSS_Y_OWPRot(fss_mesh_idx, FSS_Y_OWPRotGrp, 3,
		_V(-6.645, 0.0, 22.463), _V(0, 1.0, 0.0), (float)(PI/2));
	anim_fss_y_owp=CreateAnimation(0.0);
	parent=AddAnimationComponent(anim_fss_y_owp, 0.0, 0.5, &FSS_Y_OWPRot);
	static UINT FSS_Y_OWPTransGrp[1] = {GRP_Inner_WPS_panel};
	static MGROUP_TRANSLATE FSS_Y_OWPTrans(fss_mesh_idx, FSS_Y_OWPTransGrp, 1, _V(10.7, 0.0, 0.0));
	AddAnimationComponent(anim_fss_y_owp, 0.5, 1.0, &FSS_Y_OWPTrans, parent);
	//SetAnimation(anim_fss_y_owp, 1.0);
	static UINT FSS_Y_OWPStrutGrp[1] = {GRP_FSS_WPS_Z_bracket};
	static MGROUP_ROTATE FSS_Y_OWPStrut(fss_mesh_idx, FSS_Y_OWPStrutGrp, 1,
		_V(5.524, 0.0, 22.468), _V(0.0, 1.0, 0.0), (float)(PI));
	anim_fss_y_owp_strut=CreateAnimation(0.5);
	AddAnimationComponent(anim_fss_y_owp_strut, 0.0, 1.0, &FSS_Y_OWPStrut, parent);
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

AnimState::Action SSUPad::GetAccessArmState() const
{
	return AccessArmState.action;
}

AnimState::Action SSUPad::GetGOXArmState() const
{
	return GOXArmAction;
}

void SSUPad::AnimateFSSOWPStrut()
{
	double angle=(PI/2)*(min(FSS_Y_OWP_State.pos, 0.5)/0.5);
	double YPos=FSS_OWP_BRACKET_LENGTH*cos(angle);
	double StrutAngle=acos((FSS_OWP_STRUT_OFFSET-YPos)/FSS_OWP_STRUT_LENGTH)+angle;
	double pos=(88.482-StrutAngle*DEG)/180.0 + 0.5;
	pos=min(1, max(0, pos)); //make sure pos value is within limits
	SetAnimation(anim_fss_y_owp_strut, pos);
	//sprintf_s(oapiDebugString(), 255, "Strut angle: %f %f %f %f", (pos-0.5)*180.0, acos((13.465-YPos)/FSS_OWP_STRUT_LENGTH)*DEG, angle*DEG, pos);
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
	if(RSS_Y_OWP_State.Moving()) {
		double dp=simdt*RSS_Y_OWP_RATE;
		RSS_Y_OWP_State.Move(dp);
		SetAnimation(anim_rss_y_owp, RSS_Y_OWP_State.pos);
	}
	if(FSS_Y_OWP_State.Moving()) {
		double dp=simdt*FSS_Y_OWP_RATE;
		FSS_Y_OWP_State.Move(dp);
		SetAnimation(anim_fss_y_owp, FSS_Y_OWP_State.pos);

		if(FSS_Y_OWP_State.pos<=0.5) AnimateFSSOWPStrut();
	}
	if(RSS_State.Moving()) {
		double dp=simdt*RSS_RATE;
		RSS_State.Move(dp);
		SetAnimation(anim_rss, RSS_State.pos);
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
	WriteScenario_state(scn, "FSS_OWP", FSS_Y_OWP_State);
	WriteScenario_state(scn, "RSS_OWP", RSS_Y_OWP_State);
	WriteScenario_state(scn, "RSS", RSS_State);
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
		else if (!_strnicmp(line, "FSS_OWP", 7)) {
			sscan_state(line+7, FSS_Y_OWP_State);
			SetAnimation(anim_fss_y_owp, FSS_Y_OWP_State.pos);
			AnimateFSSOWPStrut();
		}
		else if (!_strnicmp(line, "RSS_OWP", 7)) {
			sscan_state(line+7, RSS_Y_OWP_State);
			SetAnimation(anim_rss_y_owp, RSS_Y_OWP_State.pos);
		}
		else if (!_strnicmp(line, "RSS", 3)) {
			sscan_state(line+3, RSS_State);
			SetAnimation(anim_rss, RSS_State.pos);
		}
		else ParseScenarioLineEx(line, status);
	}

	CreateGOXVentThrusters();
}

int SSUPad::clbkConsumeBufferedKey(DWORD key, bool down, char *keystate)
{
	if(!down) return 0; //only handle keydown events

	if(KEYMOD_CONTROL(keystate)) {
		switch(key) {
			case OAPI_KEY_K:
				if(RSS_State.Closing() || RSS_State.Closed()) RSS_State.action=AnimState::OPENING;
				else RSS_State.action=AnimState::CLOSING;
				return 1;
			case OAPI_KEY_X:
				if(RSS_State.Closed()) {
					if(RSS_Y_OWP_State.Closing() || RSS_Y_OWP_State.Closed()) RSS_Y_OWP_State.action=AnimState::OPENING;
					else RSS_Y_OWP_State.action=AnimState::CLOSING;
				}
				return 1;
			case OAPI_KEY_Y:
				if(FSS_Y_OWP_State.Closing() || FSS_Y_OWP_State.Closed()) FSS_Y_OWP_State.action=AnimState::OPENING;
				else FSS_Y_OWP_State.action=AnimState::CLOSING;
				return 1;
		}
	}

	switch(key) {
		case OAPI_KEY_K:
			if(AccessArmState.Open() || AccessArmState.Opening()) AccessArmState.action=AnimState::CLOSING;
			else AccessArmState.action=AnimState::OPENING;
			return 1;
			break;
		case OAPI_KEY_O:
			//Recharge GOX_Vent .. test
			SetPropellantMass(phGOXVent, 8000000000000000.0);
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
	  0, 0.6, 140, 5, 0.7, 0.25, 1.6, 1.25, PARTICLESTREAMSPEC::DIFFUSE, 
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
