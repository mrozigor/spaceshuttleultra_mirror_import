#define ORBITER_MODULE
#include "SSUPad.h"
#include "../Atlantis.h"
#include "Pad_Resource.h"
#include "meshres_FSS.h"
#include "meshres_RSS.h"
#include <dlgctrl.h>
#include <OrbiterSoundSDK35.h>

//global dll handle
HINSTANCE hPad_DLL;


VECTOR3 FSS_POS_LIGHT[FSS_NUM_LIGHTS] = {
	_V(0,0,0),
};

VECTOR3 STADIUM_LIGHT_POS[STADIUM_LIGHT_COUNT] = {
	_V(-58.2, -0.75, 1.3) + _V(4.126, 32, -41.503), //POS of SE light
	_V(-58.2, -0.75, 1.3) + _V(55.574, 27, -43.662), // POS of E light
	_V(-58.2, -0.75, 1.3) + _V(107.217, 32, -41.615),  // POS of NE light
	_V(-58.2, -0.75, 1.3) + _V(117.091, 32, 30.015),  // POS of NW light
	_V(-58.2, -0.75, 1.3) + _V(-10.551, 32, 23.754) // POS of SW light
};

VECTOR3 STADIUM_LIGHT_DIR[STADIUM_LIGHT_COUNT] = {
	_V(0.7071, 0.0, 0.7071), // DIR of SE light
	_V(0.0, 0.0, -1.0), // DIR of E light
	_V(-0.7071, 0.0, 0.7071), // DIR of NE light
	_V(-0.7071, 0.0, -0.7071), // DIR of NW light
	_V(0.7071, 0.0, -0.7071) // DIR of SW light
};

//global functions
DLLCLBK void InitModule(HINSTANCE hDLL)
{
	hPad_DLL=hDLL;
	oapiRegisterCustomControls(hDLL);
}

DLLCLBK void ExitModule(HINSTANCE hDLL)
{
	oapiUnregisterCustomControls(hDLL);
}

DLLCLBK VESSEL* ovcInit(OBJHANDLE hVessel, int iFlightModel)
{
	return new SSUPad(hVessel, iFlightModel);
}

DLLCLBK void ovcExit(VESSEL* pVessel)
{
	delete static_cast<SSUPad*>(pVessel);
}

BOOL CALLBACK SSUPad_DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg==WM_COMMAND) {
		SSUPad* pad=static_cast<SSUPad*>(oapiGetDialogContext(hWnd));
		switch(LOWORD(wParam)) {
			case IDCANCEL:
				oapiCloseDialog(hWnd);
				return TRUE;
			case IDC_RSS_OPEN:
				pad->MoveRSS(AnimState::OPENING);
				return TRUE;
			case IDC_RSS_STOP:
				pad->MoveRSS(AnimState::STOPPED);
				return TRUE;
			case IDC_RSS_CLOSE:
				pad->MoveRSS(AnimState::CLOSING);
				return TRUE;
			case IDC_FSSOWP_OPEN:
				pad->MoveFSS_OWP(AnimState::OPENING);
				return TRUE;
			case IDC_FSSOWP_CLOSE:
				pad->MoveFSS_OWP(AnimState::CLOSING);
				return TRUE;
			case IDC_RSSOWP_OPEN:
				pad->MoveRSS_OWP(AnimState::OPENING);
				return TRUE;
			case IDC_RSSOWP_CLOSE:
				pad->MoveRSS_OWP(AnimState::CLOSING);
				return TRUE;
			case IDC_IAA_DEPLOY:
				pad->DeployIAA();
				return TRUE;
			case IDC_IAA_HALT:
				pad->HaltIAA();
				return TRUE;
			case IDC_IAA_RETRACT:
				pad->RetractIAA();
				return TRUE;
			case IDC_CREWARM_EXTEND:
				pad->MoveOrbiterAccessArm(AnimState::OPENING);
				return TRUE;
			case IDC_CREWARM_RETRACT:
				pad->MoveOrbiterAccessArm(AnimState::CLOSING);
				return TRUE;
			case IDC_CREWARM_STOP:
				pad->MoveOrbiterAccessArm(AnimState::STOPPED);
				return TRUE;
			case IDC_GH2_EXTEND:
				pad->MoveGH2Pipe(AnimState::CLOSING);
				return TRUE;
			case IDC_GH2_RETRACT:
				pad->MoveGH2Pipe(AnimState::OPENING);
				return TRUE;
			case IDC_VHOOD_DEPLOY:
				pad->MoveVentHood(AnimState::OPENING);
				return TRUE;
			case IDC_VHOOD_RETRACT:
				pad->MoveVentHood(AnimState::CLOSING);
				return TRUE;
			case IDC_VHOOD_HALT:
				pad->MoveVentHood(AnimState::STOPPED);
				return TRUE;
			case IDC_VARM_EXTEND:
				pad->MoveLOXArm(AnimState::OPENING);
				return TRUE;
			case IDC_VARM_RETRACT:
				pad->MoveLOXArm(AnimState::CLOSING);
				return TRUE;
			case IDC_VARM_HALT:
				pad->MoveLOXArm(AnimState::STOPPED);
				return TRUE;
		}
	}

	//if message has not been handled in this function, perform default action
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}

SSUPad::SSUPad(OBJHANDLE hVessel, int flightmodel)
	: VESSEL3(hVessel, flightmodel),
	fLightsOn(false)
{
	//add meshes
	FSSMesh=oapiLoadMeshGlobal(DEFAULT_MESHNAME_FSS);
	fss_mesh_idx=AddMesh(FSSMesh);

	VECTOR3 rss_ofs=_V(13, 15, 1);
	RSSMesh=oapiLoadMeshGlobal(DEFAULT_MESHNAME_RSS);
	rss_mesh_idx=AddMesh(RSSMesh, &rss_ofs);

	VECTOR3 hs_ofs=_V(-58.2, -0.75, 1.3);
	HardStandMesh=oapiLoadMeshGlobal(DEFAULT_MESHNAME_HARDSTAND);
	hs_mesh_idx=AddMesh(HardStandMesh, &hs_ofs);
	VECTOR3 wt_ofs=_V(100, 45, -66);
	WaterTowerMesh=oapiLoadMeshGlobal(DEFAULT_MESHNAME_WATERTOWER);
	wt_mesh_idx=AddMesh(WaterTowerMesh, &wt_ofs);

	SetTouchdownPoints(_V(1.0, -1.0, 0.0), _V(-1.0, -1.0, 1.0), _V(-1.0, -1.0, -1.0));

	phGOXVent = NULL;
	fNextLightUpdate = -20.0;

	
	GOXArmAction=AnimState::STOPPED;
	vtx_goxvent[0] = FSS_POS_GOXVENTL;
	vtx_goxvent[1] = FSS_POS_GOXVENTR;
	vtx_goxvent[2] = FSS_POS_GOXVENTDIR;

	SoundID=-1;

	DefineAnimations();

	//GETTING HANDLE FOR STS
	//OBJHANDLE hSTS = oapiGetVesselByName();

	pSTS = NULL;
}

SSUPad::~SSUPad()
{
}

void SSUPad::CreateLights() {
	static VECTOR3& light_color = _V(1.0, 0.25, 0.25);
	for(unsigned int i = 0; i<FSS_NUM_LIGHTS; i++) {
		lights[i].duration = 0;
		lights[i].period = 0;
		lights[i].pos = &FSS_POS_LIGHT[i];
		lights[i].col = &light_color;
		lights[i].size = 0.2;
		lights[i].shape = BEACONSHAPE_DIFFUSE;
		lights[i].falloff = 0.4;
		lights[i].active = false;

		AddBeacon(&lights[i]);
	}

	const COLOUR4 STADIUM_LIGHT_DIFFUSE = {0.95, 1, 0.95, 1};//{0.925, 1, 0.925, 1};
	const COLOUR4 STADIUM_LIGHT_SPECULAR = {0,0,0,0};
	const COLOUR4 STADIUM_LIGHT_AMBIENT = {0.1, 0.125, 0.1, 0};
	const double STADIUM_LIGHT_RANGE = 300.0;
	const double STADIUM_LIGHT_ATT0 = 1e-3;
	const double STADIUM_LIGHT_ATT1 = 0;
	const double STADIUM_LIGHT_ATT2 = 1e-3;

	phLights = CreatePropellantResource(1.0, 1.0);
	for(int i=0;i<STADIUM_LIGHT_COUNT;i++) {
		pStadiumLights[i] = AddSpotLight(STADIUM_LIGHT_POS[i], STADIUM_LIGHT_DIR[i],
			STADIUM_LIGHT_RANGE, STADIUM_LIGHT_ATT0, STADIUM_LIGHT_ATT1, STADIUM_LIGHT_ATT2, 45*RAD, 180*RAD,
			STADIUM_LIGHT_DIFFUSE, STADIUM_LIGHT_SPECULAR, STADIUM_LIGHT_AMBIENT);
		
		// create fake thruster to simulate glare from lights
		thStadiumLights[i] = CreateThruster(STADIUM_LIGHT_POS[i], STADIUM_LIGHT_DIR[i], 0.0, phLights,
			1.0, 1.0);
		AddExhaust(thStadiumLights[i], 0.0, 1.5);
	}
}

void SSUPad::DefineAnimations()
{
	//orbiter access arm
	static UINT AccessArmGrp[2] = {GRP_Orbiter_Access_Arm_FSS, GRP_White_Room_FSS};
	static MGROUP_ROTATE AccessArm(fss_mesh_idx, AccessArmGrp, 2,
		_V(-3, 64.791, 22), _V(0, -1, 0), (float)(70.0*RAD));
	AccessArmState.Set(AnimState::CLOSED, 0.0);
	anim_accessarm=CreateAnimation(0.0);
	AddAnimationComponent(anim_accessarm, 0.0, 1.0, &AccessArm);

	//GOX arm
	GVAState.Set(AnimState::CLOSED, 0.0);
	anim_gva=CreateAnimation(0.0);
	static UINT GVAGrp[3] = {GRP_GVA_swing_arm_fences_FSS, GRP_GVA_swing_arm_FSS,  GRP_GOX_vent_pipes_FSS};
	static MGROUP_ROTATE GVA(fss_mesh_idx, GVAGrp, 3,
		_V(3, -6.87, 21.709), _V(0, -1, 0), (float)(73*RAD));

	static MGROUP_ROTATE GVA_VTX(LOCALVERTEXLIST, MAKEGROUPARRAY(vtx_goxvent), 3,
		_V(3, -6.87, 21.709), _V(0, -1, 0), (float)(73*RAD));

	ANIMATIONCOMPONENT_HANDLE parent=AddAnimationComponent(anim_gva, 0.0, 1.0, &GVA);
	AddAnimationComponent(anim_gva, 0.0, 1.0, &GVA_VTX);

	//GOX hood
	VentHoodState.Set(AnimState::OPEN, 1.0);
	anim_venthood=CreateAnimation(1.0);
	static UINT VentHoodGrp[9] = {GRP_GOX_Vent_Hood_FSS, GRP_North_GOX_dockseal_FSS, GRP_South_GOX_dockseal_FSS, GRP_South_GOX_vent_cylinder_01_FSS, GRP_South_GOX_vent_cylinder_02_FSS, GRP_South_GOX_vent_cylinder_03_FSS, GRP_North_GOX_vent_cylinder_01_FSS, GRP_North_GOX_vent_cylinder_02_FSS, GRP_North_GOX_vent_cylinder_03_FSS};
	static MGROUP_ROTATE VentHood(fss_mesh_idx, VentHoodGrp, 9,
		_V(-16, 82, 21.709), _V(0, 0, 1), (float)(48.0*RAD));
	parent=AddAnimationComponent(anim_venthood, 0.0, 1.0, &VentHood, parent);

	
	//FSS OWP
	FSS_OWP_State.Set(AnimState::CLOSED, 0.0);
	static UINT FSS_Y_OWPRotGrp[2] = {GRP_Outer_OWP_Curtain_Wall_panel_FSS, 
		GRP_Outer_OWP_Curtain_Wall_struts_FSS};
	static MGROUP_ROTATE FSS_Y_OWPRot(fss_mesh_idx, FSS_Y_OWPRotGrp, 2,
		_V(-6.688, 0.0, 22.614), _V(0, 1.0, 0.0), (float)(PI/2));
	anim_fss_y_owp=CreateAnimation(0.0);
	parent=AddAnimationComponent(anim_fss_y_owp, 0.0, 0.769, &FSS_Y_OWPRot);
	static UINT FSS_Y_OWPTransGrp[2] = {GRP_Inner_OWP_Curtain_Wall_structure_FSS, GRP_Inner_OWP_Curtain_Wall_panel_FSS};
	static MGROUP_TRANSLATE FSS_Y_OWPTrans(fss_mesh_idx, FSS_Y_OWPTransGrp, 2, _V(10.7, 0.0, 0.0));
	AddAnimationComponent(anim_fss_y_owp, 0.769, 1.0, &FSS_Y_OWPTrans, parent);
	static UINT FSS_Y_OWPStrutGrp[1] = {GRP_North_Cutrain_Wall_struts_FSS};
	static MGROUP_ROTATE FSS_Y_OWPStrut(fss_mesh_idx, FSS_Y_OWPStrutGrp, 1,
		_V(5.524, 0.0, 22.468), _V(0.0, 1.0, 0.0), (float)(PI));
	anim_fss_y_owp_strut=CreateAnimation(0.5);
	AddAnimationComponent(anim_fss_y_owp_strut, 0.0, 1.0, &FSS_Y_OWPStrut, parent);

	//GH2 Vent Arm
	FSS_GH2_VentArmState.Set(AnimState::CLOSED, 0.0);
	static UINT FSS_GH2_Arm[3] = {GRP_GH2_fwd_vent_flex_line_FSS, GRP_GH2_vent_hard_line_FSS, GRP_GUCP_FSS};
	static MGROUP_ROTATE FSS_GH2_ArmRot(fss_mesh_idx, FSS_GH2_Arm, 3,
		_V(5.123, 65.803, 9.541), _V(-0.80134, 0.0, 0.59821), (float)(85.0*RAD));
	anim_fss_gh2_ventarm=CreateAnimation(0.0);
	AddAnimationComponent(anim_fss_gh2_ventarm, 0.0, 1.0, &FSS_GH2_ArmRot);

	////IAA rotation
	IAA_State.Set(AnimState::CLOSED, 0.0);
	static UINT IAAGrp[2] = {GRP_Intertank_Access_Arm_FSS, GRP_IAA_extensible_platform_FSS};
	static MGROUP_ROTATE IAA_Deploy(fss_mesh_idx, IAAGrp, 2, _V(8.98, 63.7142, 16), 
		_V(0.0, 1.0, 0.0), static_cast<float>(210.0 * RAD));
	anim_iaa = CreateAnimation(0.0);
	AddAnimationComponent(anim_iaa, 0.0, 1.0, &IAA_Deploy);


	//RSS rotation
	RSS_State.Set(AnimState::CLOSED, 0.0);
	static MGROUP_ROTATE RSS_Retract(rss_mesh_idx, NULL, 0,
	_V(-28.838, 0, 23.024), _V(0.0, 1.0, 0.0), (float)(120.0*RAD));
	static UINT RSS_DoorGrp[1] = {GRP_FRCS_Room_door};
	static MGROUP_SCALE RSS_door1(rss_mesh_idx, RSS_DoorGrp, 1, _V(-20.894, 50.993, 0.654), _V(1,0.01,1));
	static UINT RSS_DoorGrp2[1] = {GRP_FRCS_Room_door};
	static MGROUP_SCALE RSS_door2(rss_mesh_idx, RSS_DoorGrp2, 1, _V(-20.894, 50.993, 0.654), _V(1,100,1));
	anim_rss=CreateAnimation(0.0);
	AddAnimationComponent(anim_rss, 0, 0.05, &RSS_door1);
	AddAnimationComponent(anim_rss, 0.06, 0.95, &RSS_Retract);
	AddAnimationComponent(anim_rss, 0.96, 1.00, &RSS_door2);
	//SetAnimation(anim_rss, 1.0);

	
	//RSS OWP
	RSS_OWP_State.Set(AnimState::CLOSED, 0.0);
	static UINT RSS_Y_LOWPGrp[2] = {GRP_OWP_Curtain_Wall, GRP_SRB_IEA_platform};
	static MGROUP_TRANSLATE RSS_Y_LOWP(rss_mesh_idx, RSS_Y_LOWPGrp, 2, _V(0.0, 0.0, 11.7));
	static UINT RSS_Y_UOWPGrp[3] = {GRP_Metal_Panel_flip_right, GRP_Metal_Panel_flip_right_lower};
	static MGROUP_ROTATE RSS_Y_UOWP(rss_mesh_idx, RSS_Y_UOWPGrp, 2,
		_V(-0, 49.85, -7), _V(-1, 0, 0), (float)(33.0*RAD));
	static UINT RSS_flip_upperGrp[1] = {GRP_Metal_Panel_flip_upper_left};
	static MGROUP_ROTATE RSS_flip_upper(rss_mesh_idx, RSS_flip_upperGrp, 1,
		_V(0, 62.5, 2.45), _V(1, 0, 0), (float)(90.0*RAD));
	static UINT RSS_flip_lowerGrp[1] = {GRP_Metal_Panel_flip_lower_left};
	static MGROUP_ROTATE RSS_flip_lower(rss_mesh_idx, RSS_flip_lowerGrp, 1,
		_V(-7.286, 62.5, 4.21), _V(0, 1, 0), (float)(108.0*RAD));
	anim_rss_y_owp=CreateAnimation(0.0);
	AddAnimationComponent(anim_rss_y_owp, 0, 0.35, &RSS_Y_UOWP);
	AddAnimationComponent(anim_rss_y_owp, 0, 0.35, &RSS_flip_upper);
	AddAnimationComponent(anim_rss_y_owp, 0.05, 0.35, &RSS_flip_lower);
	AddAnimationComponent(anim_rss_y_owp, 0.38, 1.0, &RSS_Y_LOWP);
	//SetAnimation(anim_rss_y_owp, 1.0);



}

void SSUPad::DisableLights() {
	fLightsOn = false;
	for(unsigned int i = 0; i<FSS_NUM_LIGHTS; i++) {
		lights[i].active = false;

		//fLightsOn = fLightsOn && lights[i].active;
	}

	for(unsigned int i=0;i<STADIUM_LIGHT_COUNT;i++) {
		pStadiumLights[i]->Activate(false);
		SetThrusterLevel(thStadiumLights[i], 0.0);
	}
}

void SSUPad::EnableLights() {
	fLightsOn = true;

	for(unsigned int i = 0; i<FSS_NUM_LIGHTS; i++) {
		lights[i].active = true;

		//fLightsOn = fLightsOn && lights[i].active;
	}

	for(unsigned int i=0;i<STADIUM_LIGHT_COUNT;i++) {
		pStadiumLights[i]->Activate(true);
		SetThrusterLevel(thStadiumLights[i], 1.0);
	}
}



bool SSUPad::IsNight() const {
	
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

void SSUPad::OnT0()
{
	FSS_GH2_VentArmState.action=AnimState::OPENING;
}

void SSUPad::MoveOrbiterAccessArm(AnimState::Action action)
{
	if(action==AnimState::OPENING || action==AnimState::CLOSING || action==AnimState::STOPPED)
		AccessArmState.action=action;
}

void SSUPad::MoveGOXArm(AnimState::Action action)
{
	if(action==AnimState::OPENING || action==AnimState::CLOSING || action==AnimState::STOPPED) {
		GOXArmAction=action;
		GOXArmSequence();
	}
}

void SSUPad::MoveFSS_OWP(AnimState::Action action)
{
	if(action==AnimState::OPENING || action==AnimState::CLOSING)
		FSS_OWP_State.action=action;
}

void SSUPad::MoveRSS_OWP(AnimState::Action action)
{
	if(RSS_State.Closed() && (action==AnimState::OPENING || action==AnimState::CLOSING))
		RSS_OWP_State.action=action;
}

void SSUPad::MoveRSS(AnimState::Action action)
{
	if(RSS_OWP_State.Closed())
		RSS_State.action=action;
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
	else if(GOXArmAction==AnimState::STOPPED)
		GVAState.action=AnimState::STOPPED;
}

void SSUPad::MoveGH2Pipe(AnimState::Action action)
{
	if(action==AnimState::CLOSING || action == AnimState::OPENING)
		FSS_GH2_VentArmState.action = action;
}

void SSUPad::MoveVentHood(AnimState::Action action)
{
	if(action == AnimState::CLOSING || action == AnimState::OPENING || action == AnimState::STOPPED)
		VentHoodState.action = action;
}

void SSUPad::MoveLOXArm(AnimState::Action action)
{
	if(action == AnimState::CLOSING || action == AnimState::OPENING || action == AnimState::STOPPED)
		GVAState.action = action;
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
	double angle=(PI/2)*(min(FSS_OWP_State.pos, 0.769)/0.769);
	//calculate horizontal distance between FSS strut attachment (to OWP bracket) and FSS bracket attachment (to FSS)
	double YPos=FSS_OWP_BRACKET_LENGTH*cos(angle);
	//calculate angle between struts and OWP bracket
	double StrutAngle=acos((FSS_OWP_STRUT_OFFSET-YPos)/FSS_OWP_STRUT_LENGTH)+angle;
	double pos=(FSS_OWP_STRUT_NULL_ANGLE-StrutAngle*DEG)/180.0 + 0.5;
	pos=min(1, max(0, pos)); //make sure pos value is within limits
	SetAnimation(anim_fss_y_owp_strut, pos);
}

void SSUPad::clbkPostCreation()
{
	SoundID=ConnectToOrbiterSoundDLL3(GetHandle());
	if(SoundID!=-1) {
		RequestLoadVesselWave3(SoundID, RSS_ROTATE_SOUND, (char*)RSS_ROTATE_SOUND_FILE, BOTHVIEW_FADED_FAR);
	}


	//GET POINTER TO ATLANTIS CLASS
	OBJHANDLE hSTS = oapiGetVesselByName((char*)ShuttleName.c_str());
	if(hSTS!=NULL)
	{
		VESSEL *pVessel = oapiGetVesselInterface(hSTS);
		if(pVessel && !_strnicmp(pVessel->GetClassName(), "SpaceShuttleUltra",17))
			pSTS = static_cast<Atlantis*>(pVessel);
		else
			pSTS = NULL;
	}

	//if(pSTS)
		//StsTank = pSTS->STSTankForPad;
		//SetPropellantLeve
	
}

void SSUPad::clbkPreStep(double simt, double simdt, double mjd)
{
	VESSEL3::clbkPreStep(simt, simdt, mjd);

	if(simt > fNextLightUpdate) {
		fNextLightUpdate = simt + 300.0;

		if(fLightsOn && !IsNight()) {
			DisableLights();
		} 
		else if(!fLightsOn && IsNight()) {
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
	if(RSS_OWP_State.Moving()) {
		double dp=simdt*RSS_OWP_RATE;
		RSS_OWP_State.Move(dp);
		SetAnimation(anim_rss_y_owp, RSS_OWP_State.pos);
	}
	if(FSS_OWP_State.Moving()) {
		double dp=simdt*FSS_OWP_RATE;
		FSS_OWP_State.Move(dp);
		SetAnimation(anim_fss_y_owp, FSS_OWP_State.pos);

		if(FSS_OWP_State.pos<=0.5) AnimateFSSOWPStrut();
	}
	if(IAA_State.Moving()) {
		double dp=simdt*FSS_IAA_RATE;
		IAA_State.Move(dp);
		SetAnimation(anim_iaa, IAA_State.pos);
	}
	if(FSS_GH2_VentArmState.Moving()) {
		double dp=simdt*FSS_GH2_ARM_RATE;
		FSS_GH2_VentArmState.Move(dp);
		SetAnimation(anim_fss_gh2_ventarm, FSS_GH2_VentArmState.pos);
	}
	if(RSS_State.Moving()) {
		double dp=simdt*RSS_RATE;
		RSS_State.Move(dp);
		SetAnimation(anim_rss, RSS_State.pos);
		PlayVesselWave3(SoundID, RSS_ROTATE_SOUND, LOOP);
	}
	else StopVesselWave3(SoundID, RSS_ROTATE_SOUND);

	UpdateGOXVentThrusters();
	
	
	if(VentHoodState.Open() && pSTS) {
		short sGOXLevel = pSTS->GetETPropellant();
		if(sGOXLevel >= 60) {
			SetPropellantMass(phGOXVent, 100.0); // we don't care what the actual mass is, just make sure it's nonzero
			// Make vent level lag behind 
			double fFlow = static_cast<double>(sGOXLevel)/100.0;
			SetThrusterLevel(thGOXVent[0], fFlow/5.0);
			SetThrusterLevel(thGOXVent[1], fFlow/5.0);
		}
	}
	else
	{
		SetThrusterLevel(thGOXVent[0], 0.0);
		SetThrusterLevel(thGOXVent[1], 0.0);
	}

	
	//NEW GOX VENT CODE
	/*double StsTank;
	if(pSTS)
	{
		StsTank = pSTS->STSTankForPad;
		SetPropellantMass(phGOXVent,StsTank);
	}
	else
		StsTank = 0;
	
	if(VentHoodState.Open())
	{
		double fGOXMass = StsTank;
		if((StsTank/TANK_MAX_PROPELLANT_MASS)>=0.6)
		{	
			double fFlow = 1*(StsTank/TANK_MAX_PROPELLANT_MASS);
			SetThrusterLevel(thGOXVent[0], fFlow/5);
			SetThrusterLevel(thGOXVent[1], fFlow/5);
		}
		else 
		{
			SetThrusterLevel(thGOXVent[0], 0);
			SetThrusterLevel(thGOXVent[1], 0);
		}
	}
	else
	{
		SetThrusterLevel(thGOXVent[0], 0);
		SetThrusterLevel(thGOXVent[1], 0);
	}*/
	
}

void SSUPad::clbkSaveState(FILEHANDLE scn)
{
	VESSEL3::clbkSaveState(scn); //default parameters

	WriteScenario_state(scn, "ACCESS_ARM", AccessArmState);
	WriteScenario_state(scn, "GVA", GVAState);
	WriteScenario_state(scn, "VENTHOOD", VentHoodState);
	WriteScenario_state(scn, "FSS_OWP", FSS_OWP_State);
	WriteScenario_state(scn, "RSS_OWP", RSS_OWP_State);
	WriteScenario_state(scn, "RSS", RSS_State);
	WriteScenario_state(scn, "FSS_GH2", FSS_GH2_VentArmState);
	WriteScenario_state(scn, "FSS_IAA", IAA_State);
	oapiWriteScenario_int(scn, "GOX_SEQUENCE", GOXArmAction);
	oapiWriteScenario_string(scn,"SHUTTLE", (char*)ShuttleName.c_str());
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
			sscan_state(line+7, FSS_OWP_State);
			SetAnimation(anim_fss_y_owp, FSS_OWP_State.pos);
			AnimateFSSOWPStrut();
		}
		else if (!_strnicmp(line, "RSS_OWP", 7)) {
			sscan_state(line+7, RSS_OWP_State);
			SetAnimation(anim_rss_y_owp, RSS_OWP_State.pos);
		}
		else if (!_strnicmp(line, "RSS", 3)) {
			sscan_state(line+3, RSS_State);
			SetAnimation(anim_rss, RSS_State.pos);
		}
		else if (!_strnicmp(line, "FSS_IAA", 7)) {
			sscan_state(line+7, IAA_State);
			SetAnimation(anim_iaa, IAA_State.pos);
		}
		else if (!_strnicmp(line, "FSS_GH2", 7)) {
			sscan_state(line+7, FSS_GH2_VentArmState);
			SetAnimation(anim_fss_gh2_ventarm, FSS_GH2_VentArmState.pos);
		}
		else if (!_strnicmp(line, "SHUTTLE", 7)) {
			//sscanf_s(line+7,"%s",&ShuttleName,sizeof(ShuttleName));
			ShuttleName = line+8; // read char array into string
			char cnam[256];
			sprintf(cnam,"%s%s","Shuttle name: ",ShuttleName);
			oapiWriteLog(cnam);
		}
		else ParseScenarioLineEx(line, status);
	}

	CreateGOXVentThrusters();
	//SetPropellantMass(phGOXVent,pSTS->STSTankForPad);
}

int SSUPad::clbkConsumeBufferedKey(DWORD key, bool down, char *keystate)
{
	if(!down) return 0; //only handle keydown events

	if(KEYMOD_CONTROL(keystate)) {
		switch(key) {
			case OAPI_KEY_SPACE:
				if(oapiOpenDialogEx(hPad_DLL, IDD_ANIMCTRL, SSUPad_DlgProc, DLG_CAPTIONCLOSE, this))
					oapiWriteLog("SSU Pad: Dialog opened");
				else if(hPad_DLL) sprintf_s(oapiDebugString(), 255, "Error: %d", GetLastError());
				return 1;
			case OAPI_KEY_K:
				if(RSS_State.Closing() || RSS_State.Closed()) RSS_State.action=AnimState::OPENING;
				else RSS_State.action=AnimState::CLOSING;
				return 1;
			case OAPI_KEY_X:
				if(RSS_State.Closed()) {
					if(RSS_OWP_State.Closing() || RSS_OWP_State.Closed()) RSS_OWP_State.action=AnimState::OPENING;
					else RSS_OWP_State.action=AnimState::CLOSING;
				}
				return 1;
			case OAPI_KEY_Y:
				if(FSS_OWP_State.Closing() || FSS_OWP_State.Closed()) FSS_OWP_State.action=AnimState::OPENING;
				else FSS_OWP_State.action=AnimState::CLOSING;
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
	  0, 0.6, 15, 1.15, 5, 1.03, 0.5, 6.0, PARTICLESTREAMSPEC::EMISSIVE, 
	  PARTICLESTREAMSPEC::LVL_PSQRT, 0, 1, 
	  PARTICLESTREAMSPEC::ATM_PLOG, 1e-50, 1
	  };

	gox_stream.tex = oapiRegisterParticleTexture ("SSU\\GOX_stream");


	VECTOR3 dir = (vtx_goxvent[2] - vtx_goxvent[0]);
	dir = dir/length(dir);

	if(phGOXVent == NULL) {
		phGOXVent = CreatePropellantResource(TANK_MAX_PROPELLANT_MASS);
		thGOXVent[0] = CreateThruster(vtx_goxvent[0], dir, 0, phGOXVent, 250, 100);
		thGOXVent[1] = CreateThruster(vtx_goxvent[1], dir, 0, phGOXVent, 250, 100);
		AddExhaustStream(thGOXVent[0], &gox_stream);
		AddExhaustStream(thGOXVent[1], &gox_stream);
	} else {
		UpdateGOXVentThrusters();
	}
}

void SSUPad::clbkSetClassCaps(FILEHANDLE cfg) {
	SetEmptyMass(2.000001);
	SetSize(392.5);
	CreateGOXVentThrusters();
	CreateLights();

	CreateAttachment(false, _V(4.45, 20, 1.25), _V(0, -1, 0), _V(1, 0, 0), "XMLP");
}

void SSUPad::UpdateGOXVentThrusters() {
	VECTOR3 dir = (vtx_goxvent[2] - vtx_goxvent[0]);
	dir = dir/length(dir);
	
	for(int i = 0; i<2; i++) {
		SetThrusterRef(thGOXVent[i], vtx_goxvent[i]);
		SetThrusterDir(thGOXVent[i], dir);
	}

}

void SSUPad::DeployIAA()
{
	IAA_State.action = AnimState::OPENING;
}

void SSUPad::HaltIAA()
{
	IAA_State.action = AnimState::STOPPED;
}

void SSUPad::RetractIAA()
{
	IAA_State.action = AnimState::CLOSING;
}