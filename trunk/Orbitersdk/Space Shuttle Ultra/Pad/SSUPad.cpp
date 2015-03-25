#define ORBITER_MODULE
#include "SSUPad.h"
#include "../Atlantis.h"
#include "Pad_Resource.h"
#include "meshres_FSS.h"
#include "meshres_RSS.h"
#include "meshres_FSS_1985.h"
#include "meshres_RSS_1985.h"
#include <dlgctrl.h>
#include <OrbiterSoundSDK40.h>

//global dll handle
HINSTANCE hPad_DLL;


VECTOR3 FSS_POS_LIGHT[FSS_NUM_LIGHTS] = {
	_V(-58.2, -0.75, 1.3) + _V(185, 93, -104),
};

const VECTOR3 LC39_LIGHT_POS[LC39_LIGHT_COUNT] = {
	_V(-58.2, -0.75, 1.3) + _V(4.126, 32, -41.503), //POS of SE light
	_V(-58.2, -0.75, 1.3) + _V(55.574, 27, -43.662), // POS of E light
	_V(-58.2, -0.75, 1.3) + _V(107.217, 32, -41.615),  // POS of NE light
	_V(-58.2, -0.75, 1.3) + _V(117.091, 32, 30.015),  // POS of NW light
	_V(-58.2, -0.75, 1.3) + _V(-10.551, 32, 23.754) // POS of SW light
};

const VECTOR3 LC39_LIGHT_DIR[LC39_LIGHT_COUNT] = {
	_V(0.7071, 0.0, 0.7071), // DIR of SE light
	_V(0.0, 0.0, -1.0), // DIR of E light
	_V(-0.7071, 0.0, 0.7071), // DIR of NE light
	_V(-0.7071, 0.0, -0.7071), // DIR of NW light
	_V(0.7071, 0.0, -0.7071) // DIR of SW light
};
const COLOUR4 LC39_LIGHT_DIFFUSE = {0.95f, 1.0f, 0.95f, 1.0f};//{0.95f, 1.0f, 0.95f, 1.0f};
const COLOUR4 LC39_LIGHT_SPECULAR = {0,0,0,0};
const COLOUR4 LC39_LIGHT_AMBIENT = {0.1f, 0.125f, 0.1f, 0.0f};
const double LC39_LIGHT_RANGE = 100.0;
const double LC39_LIGHT_ATT0 = 1e-3;
const double LC39_LIGHT_ATT1 = 0;
const double LC39_LIGHT_ATT2 = 1e-3;
const double LC39_LIGHT_UMBRA = 45.0*RAD;
const double LC39_LIGHT_PENUMBRA = 180.0*RAD;

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
				pad->ExtendOrbiterAccessArm( OAA_RATE_NORMAL );
				return TRUE;
			case IDC_CREWARM_RETRACT:
				pad->RetractOrbiterAccessArm();
				return TRUE;
			case IDC_CREWARM_STOP:
				pad->HaltOrbiterAccessArm();
				return TRUE;
			case IDC_GH2_EXTEND:
				pad->AttachGH2Ventline();
				return TRUE;
			case IDC_GH2_RETRACT:
				pad->DetachGH2Ventline();
				return TRUE;
			case IDC_VHOOD_DEPLOY:
				pad->LowerVentHood();
				return TRUE;
			case IDC_VHOOD_RETRACT:
				pad->RaiseVentHood();
				return TRUE;
			case IDC_VHOOD_HALT:
				pad->HaltVentHood();
				return TRUE;
			case IDC_VARM_EXTEND:
				pad->ExtendGOXArm();
				return TRUE;
			case IDC_VARM_RETRACT:
				pad->RetractGOXArm();
				return TRUE;
			case IDC_VARM_HALT:
				pad->HaltGOXArm();
				return TRUE;
			case IDC_RBUS_EXTEND:
				pad->MoveRBUS(AnimState::CLOSING);
				return TRUE;
			case IDC_RBUS_RETRACT:
				pad->MoveRBUS(AnimState::OPENING);
				return TRUE;
		}
	}
	//else if(uMsg==WM_CREATE) {
	else if(uMsg==WM_SHOWWINDOW) {
		SSUPad* pad=static_cast<SSUPad*>(oapiGetDialogContext(hWnd));
		//oapiWriteLog("WM_CREATE message");
		if(pad->bPad1985) {
			//oapiWriteLog("Changing dialog");
			// change title of OWP group box
			SendDlgItemMessage(hWnd, IDC_GROUP_OWP, WM_SETTEXT, 0, (LPARAM)("RBUS"));
			// hide OWP buttons and labels
			//SendDlgItemMessage(hWnd, IDC_LABEL_FSSOWP, WM_SHOWWINDOW, FALSE, 0);
			ShowWindow(GetDlgItem(hWnd, IDC_LABEL_FSSOWP), SW_HIDE);
			ShowWindow(GetDlgItem(hWnd, IDC_LABEL_RSSOWP), SW_HIDE);
			ShowWindow(GetDlgItem(hWnd, IDC_RSSOWP_OPEN), SW_HIDE);
			ShowWindow(GetDlgItem(hWnd, IDC_RSSOWP_CLOSE), SW_HIDE);
			ShowWindow(GetDlgItem(hWnd, IDC_FSSOWP_OPEN), SW_HIDE);
			ShowWindow(GetDlgItem(hWnd, IDC_FSSOWP_CLOSE), SW_HIDE);
		}
		else {
			// hide RBUS buttons
			ShowWindow(GetDlgItem(hWnd, IDC_RBUS_EXTEND), SW_HIDE);
			ShowWindow(GetDlgItem(hWnd, IDC_RBUS_RETRACT), SW_HIDE);
		}
		// let Orbiter perform default actions
		return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
	}

	//if message has not been handled in this function, perform default action
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}

SSUPad::SSUPad(OBJHANDLE hVessel, int flightmodel)
	: BaseSSUPad(hVessel, flightmodel),
	bPad1985(false), fLightsOn(false)
{
	phGOXVent = NULL;
	fNextLightUpdate = -20.0;

	
	vtx_goxvent[0] = FSS_POS_GOXVENTL;
	vtx_goxvent[1] = FSS_POS_GOXVENTR;
	vtx_goxvent[2] = FSS_POS_GOXVENTDIR;

	SoundID=-1;

	//GETTING HANDLE FOR STS
	//OBJHANDLE hSTS = oapiGetVesselByName();

	pSTS = NULL;
}

SSUPad::~SSUPad()
{
}

void SSUPad::DefineAnimations()
{
	//orbiter access arm
	static UINT AccessArmGrp[2] = {GRP_Orbiter_Access_Arm_FSS, GRP_White_Room_FSS};
	static UINT AccessArmGrp_1985[2] = {GRP_ORBITER_ACCESS_ARM_FSS_1985, GRP_WHITE_ROOM_FSS_1985};
	/*static MGROUP_ROTATE AccessArm(fss_mesh_idx, AccessArmGrp, 2,
		_V(-3, 64.791, 22), _V(0, -1, 0), (float)(70.0*RAD));*/
	MGROUP_ROTATE* AccessArm = DefineRotation(fss_mesh_idx, bPad1985 ? AccessArmGrp_1985 : AccessArmGrp, 2,
		_V(-3, 64.791, 22), _V(0, -1, 0), (float)(70.0*RAD));
	AccessArmState.Set(AnimState::CLOSED, 0.0);
	anim_AccessArm=CreateAnimation(0.0);
	AddAnimationComponent(anim_AccessArm, 0.0, 1.0, AccessArm);

	//GOX arm
	VentArmState.Set(AnimState::CLOSED, 0.0);
	anim_VentArm=CreateAnimation(0.0);
	static UINT GVAGrp[3] = {GRP_GVA_swing_arm_fences_FSS, GRP_GVA_swing_arm_FSS,  GRP_GOX_vent_pipes_FSS};
	static UINT GVAGrp_1985[3] = {GRP_GVA_SWING_ARM_FENCES_FSS_1985, GRP_GVA_SWING_ARM_FSS_1985,  GRP_GOX_VENT_PIPES_FSS_1985};
	MGROUP_ROTATE* GVA = DefineRotation(fss_mesh_idx, bPad1985 ? GVAGrp_1985 : GVAGrp, 3,
		_V(3, -6.87, 21.709), _V(0, -1, 0), (float)(73*RAD));
	static MGROUP_ROTATE GVA_VTX(LOCALVERTEXLIST, MAKEGROUPARRAY(vtx_goxvent), 3,
		_V(3, -6.87, 21.709), _V(0, -1, 0), (float)(73*RAD));

	ANIMATIONCOMPONENT_HANDLE parent=AddAnimationComponent(anim_VentArm, 0.0, 1.0, GVA);
	AddAnimationComponent(anim_VentArm, 0.0, 1.0, &GVA_VTX);

	//GOX hood
	VentHoodState.Set(AnimState::OPEN, 1.0);
	anim_VentHood=CreateAnimation(1.0);
	static UINT VentHoodGrp[9] = {GRP_GOX_Vent_Hood_FSS, GRP_North_GOX_dockseal_FSS, GRP_South_GOX_dockseal_FSS, GRP_South_GOX_vent_cylinder_01_FSS, GRP_South_GOX_vent_cylinder_02_FSS, GRP_South_GOX_vent_cylinder_03_FSS, GRP_North_GOX_vent_cylinder_01_FSS, GRP_North_GOX_vent_cylinder_02_FSS, GRP_North_GOX_vent_cylinder_03_FSS};
	static UINT VentHoodGrp_1985[9] = {GRP_GOX_VENT_HOOD_FSS_1985, GRP_NORTH_GOX_DOCKSEAL_FSS_1985, GRP_SOUTH_GOX_DOCKSEAL_FSS_1985, GRP_SOUTH_GOX_VENT_CYLINDER_01_FSS_1985, GRP_SOUTH_GOX_VENT_CYLINDER_02_FSS_1985, GRP_SOUTH_GOX_VENT_CYLINDER_03_FSS_1985, GRP_NORTH_GOX_VENT_CYLINDER_01_FSS_1985, GRP_NORTH_GOX_VENT_CYLINDER_02_FSS_1985, GRP_NORTH_GOX_VENT_CYLINDER_03_FSS_1985};
	MGROUP_ROTATE* VentHood = DefineRotation(fss_mesh_idx, bPad1985 ? VentHoodGrp_1985 : VentHoodGrp, 9,
		_V(-16, 82, 21.709), _V(0, 0, 1), (float)(48.0*RAD));
	parent=AddAnimationComponent(anim_VentHood, 0.0, 1.0, VentHood, parent);

	//GH2 Vent Arm
	GH2VentlineState.Set(AnimState::CLOSED, 0.0);
	static UINT FSS_GH2_Arm[3] = {GRP_GH2_fwd_vent_flex_line_FSS, GRP_GH2_vent_hard_line_FSS, GRP_GUCP_FSS};
	static UINT FSS_GH2_Arm_1985[3] = {GRP_GH2_FWD_VENT_FLEX_LINE_FSS_1985, GRP_GH2_VENT_HARD_LINE_FSS_1985, GRP_GUCP_FSS_1985};
	MGROUP_ROTATE* FSS_GH2_ArmRot = DefineRotation(fss_mesh_idx, bPad1985 ? FSS_GH2_Arm_1985 : FSS_GH2_Arm, 3,
		_V(5.07, 65.5287, 11.6944), _V(-0.843, 0.0, 0.536), (float)(90.0*RAD));
	anim_GH2Ventline=CreateAnimation(0.0);
	AddAnimationComponent(anim_GH2Ventline, 0.0, 1.0, FSS_GH2_ArmRot);

	////IAA rotation
	IAAState.Set(AnimState::CLOSED, 0.0);
	static UINT IAAGrp[2] = {GRP_Intertank_Access_Arm_FSS, GRP_IAA_extensible_platform_FSS};
	static UINT IAAGrp_1985[2] = {GRP_INTERTANK_ACCESS_ARM_FSS_1985, GRP_IAA_EXTENSIBLE_PLATFORM_FSS_1985};
	MGROUP_ROTATE* IAA_Deploy = DefineRotation(fss_mesh_idx, bPad1985 ? IAAGrp_1985 : IAAGrp, 2, _V(8.98, 63.7142, 16), 
		_V(0.0, 1.0, 0.0), static_cast<float>(210.0 * RAD));
	anim_IAA = CreateAnimation(0.0);
	AddAnimationComponent(anim_IAA, 0.0, 1.0, IAA_Deploy);

	if(bPad1985) {
		//RBUS
		FSS_RBUS_UmbilicalState.Set(AnimState::CLOSED, 0.0);
		static UINT RBUS_grp[2] = {GRP_RBUS_UMBILICAL_BEAM_FSS_1985, GRP_RBUS_CARRIER_PLATE_FSS_1985};
		static MGROUP_TRANSLATE rbus_translate = MGROUP_TRANSLATE(fss_mesh_idx,RBUS_grp,2,_V(0,-1.665,15.42));
		anim_fss_rbus = CreateAnimation(0.0);
		AddAnimationComponent(anim_fss_rbus,0,1,&rbus_translate,0);
	}
	else {
		//FSS OWP
		FSS_OWP_State.Set(AnimState::CLOSED, 0.0);
		static UINT FSS_Y_OWPRotGrp[2] = {GRP_Outer_OWP_Curtain_Wall_panel_FSS, 
			GRP_Outer_OWP_Curtain_Wall_struts_FSS};
		static MGROUP_ROTATE FSS_Y_OWPRot(fss_mesh_idx, FSS_Y_OWPRotGrp, 2,
			_V(-6.37, 0.0, 22), _V(0, 1.0, 0.0), (float)(PI/2));
		anim_fss_y_owp=CreateAnimation(0.0);
		parent=AddAnimationComponent(anim_fss_y_owp, 0.0, 0.769, &FSS_Y_OWPRot);
		static UINT FSS_Y_OWPTransGrp[2] = {GRP_Inner_OWP_Curtain_Wall_structure_FSS, GRP_Inner_OWP_Curtain_Wall_panel_FSS};
		static MGROUP_TRANSLATE FSS_Y_OWPTrans(fss_mesh_idx, FSS_Y_OWPTransGrp, 2, _V(7.2, 0.0, 0.0));
		AddAnimationComponent(anim_fss_y_owp, 0.769, 1.0, &FSS_Y_OWPTrans, parent);
		static UINT FSS_Y_OWPStrutGrp[1] = {GRP_North_Cutrain_Wall_struts_FSS};
		static MGROUP_ROTATE FSS_Y_OWPStrut(fss_mesh_idx, FSS_Y_OWPStrutGrp, 1,
			_V(5.524, 0.0, 22.468), _V(0.0, 1.0, 0.0), (float)(PI));
		anim_fss_y_owp_strut=CreateAnimation(0.5);
		AddAnimationComponent(anim_fss_y_owp_strut, 0.0, 1.0, &FSS_Y_OWPStrut, parent);
		//RSS OWP
		RSS_OWP_State.Set(AnimState::CLOSED, 0.0);
		static UINT RSS_Y_LOWPGrp[2] = {GRP_OWP_Curtain_Wall, GRP_SRB_IEA_platform};
		static MGROUP_TRANSLATE RSS_Y_LOWP(rss_mesh_idx, RSS_Y_LOWPGrp, 2, _V(0.0, 0.0, 12.1));
		static UINT RSS_Y_UOWPGrp[2] = {GRP_Metal_Panel_flip_right, GRP_Metal_Panel_flip_right_lower};
		static MGROUP_ROTATE RSS_Y_UOWP(rss_mesh_idx, RSS_Y_UOWPGrp, 2,
			_V(0, 34.94, -4.57), _V(-1, 0, 0), (float)(33.0*RAD));
		static UINT RSS_flip_upperGrp[1] = {GRP_Metal_Panel_flip_upper_left};
		static MGROUP_ROTATE RSS_flip_upper(rss_mesh_idx, RSS_flip_upperGrp, 1,
			_V(0, 45.71, 4.515), _V(1, 0, 0), (float)(90.0*RAD));
		static UINT RSS_flip_lowerGrp[2] = {GRP_Metal_Panel_flip_lower_left, GRP_Line06};
		static MGROUP_ROTATE RSS_flip_lower(rss_mesh_idx, RSS_flip_lowerGrp, 2,
			_V(-20.75, 34.40, 6.06), _V(0, 1, 0), (float)(105.0*RAD));
		anim_rss_y_owp=CreateAnimation(0.0);
		AddAnimationComponent(anim_rss_y_owp, 0, 0.35, &RSS_Y_UOWP);
		AddAnimationComponent(anim_rss_y_owp, 0, 0.35, &RSS_flip_upper);
		AddAnimationComponent(anim_rss_y_owp, 0.05, 0.35, &RSS_flip_lower);
		AddAnimationComponent(anim_rss_y_owp, 0.38, 1.0, &RSS_Y_LOWP);
	}

	//RSS rotation
	RSS_State.Set(AnimState::CLOSED, 0.0);
	static MGROUP_ROTATE RSS_Retract(rss_mesh_idx, NULL, 0,
		_V(-28.838, 0, 23.024), _V(0.0, 1.0, 0.0), (float)(120.0*RAD));
	static UINT RSS_DoorGrp[1] = {GRP_FRCS_Room_door};
	static UINT RSS_DoorGrp_1985[1] = {GRP_FRCS_Room_door_1985};
	MGROUP_SCALE* RSS_door1 = DefineScale(rss_mesh_idx, bPad1985 ? RSS_DoorGrp_1985 : RSS_DoorGrp, 1, _V(-20.894, 50.993, 0.654), _V(1,0.01,1));
	static UINT RSS_DoorGrp2[1] = {GRP_FRCS_Room_door};
	static UINT RSS_DoorGrp2_1985[1] = {GRP_FRCS_Room_door_1985};
	MGROUP_SCALE* RSS_door2 = DefineScale(rss_mesh_idx, bPad1985 ? RSS_DoorGrp2_1985 : RSS_DoorGrp2, 1, _V(-20.894, 50.993, 0.654), _V(1,100,1));
	anim_rss=CreateAnimation(0.0);
	AddAnimationComponent(anim_rss, 0, 0.05, RSS_door1);
	AddAnimationComponent(anim_rss, 0.06, 0.95, &RSS_Retract);
	AddAnimationComponent(anim_rss, 0.96, 1.00, RSS_door2);
	//SetAnimation(anim_rss, 1.0);
}

void SSUPad::OnT0()
{
	DetachGH2Ventline();
	if(bPad1985) MoveRBUS(AnimState::OPENING);
}

void SSUPad::MoveFSS_OWP(AnimState::Action action)
{
	if(bPad1985) return; // 1985 pad; no OWP
	if(action==AnimState::OPENING || action==AnimState::CLOSING)
		FSS_OWP_State.action=action;
}

void SSUPad::MoveRSS_OWP(AnimState::Action action)
{
	if(bPad1985) return; // 1985 pad; no OWP
	if(RSS_State.Closed() && (action==AnimState::OPENING || action==AnimState::CLOSING))
		RSS_OWP_State.action=action;
}

void SSUPad::MoveRSS(AnimState::Action action)
{
	if(bPad1985 || RSS_OWP_State.Closed())
		RSS_State.action=action;
}

/*void SSUPad::GOXArmSequence()
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
}*/


void SSUPad::MoveRBUS(AnimState::Action action)
{
	if(!bPad1985) return; // RBUS is only on 1985 pad
	if(action == AnimState::CLOSING || action == AnimState::OPENING)
		FSS_RBUS_UmbilicalState.action = action;
}

void SSUPad::AnimateFSSOWPStrut()
{
	if(bPad1985) return; // no OWP on 1985 pad

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
	SoundID=ConnectToOrbiterSoundDLL(GetHandle());
	if(SoundID!=-1) {
		SetMyDefaultWaveDirectory(const_cast<char*>(SOUND_DIRECTORY)); // this is defined in Atlantis.h, but is same for SSU Pad
		RequestLoadVesselWave(SoundID, RSS_ROTATE_SOUND, (char*)RSS_ROTATE_SOUND_FILE, BOTHVIEW_FADED_FAR);
		RequestLoadVesselWave(SoundID, CRYO_HISS, const_cast<char*>(CRYO_HISS_SOUND_FILE), BOTHVIEW_FADED_MEDIUM);
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
	BaseSSUPad::clbkPreStep(simt, simdt, mjd);

	/*if(simt > fNextLightUpdate) {
		fNextLightUpdate = simt + 300.0;

		if(fLightsOn && !IsNight()) {
			DisableLights();
		} 
		else if(!fLightsOn && IsNight()) {
			EnableLights();
		}
	}*/

	if(!bPad1985 && RSS_OWP_State.Moving()) {
		double dp=simdt*RSS_OWP_RATE;
		RSS_OWP_State.Move(dp);
		SetAnimation(anim_rss_y_owp, RSS_OWP_State.pos);
	}
	if(!bPad1985 && FSS_OWP_State.Moving()) {
		double dp=simdt*FSS_OWP_RATE;
		FSS_OWP_State.Move(dp);
		SetAnimation(anim_fss_y_owp, FSS_OWP_State.pos);

		if(FSS_OWP_State.pos<=0.5) AnimateFSSOWPStrut();
	}
	if(RSS_State.Moving()) {
		double dp=simdt*RSS_RATE;
		RSS_State.Move(dp);
		SetAnimation(anim_rss, RSS_State.pos);
		PlayVesselWave(SoundID, RSS_ROTATE_SOUND, LOOP);
	}
	else StopVesselWave(SoundID, RSS_ROTATE_SOUND);

	if(bPad1985 && FSS_RBUS_UmbilicalState.Moving())
	{
		double dp = simdt*FSS_RBUS_RATE;
		FSS_RBUS_UmbilicalState.Move(dp);
		SetAnimation(anim_fss_rbus,FSS_RBUS_UmbilicalState.pos);
	}

	UpdateGOXVents();
	
	
	if(VentHoodState.Open() && pSTS) {
		short sGOXLevel = pSTS->GetETPropellant();
		if(sGOXLevel >= 60) {
			SetPropellantMass(phGOXVent, 100.0); // we don't care what the actual mass is, just make sure it's nonzero
			// Make vent level lag behind 
			double fFlow = static_cast<double>(sGOXLevel)/100.0;
			SetThrusterLevel(thGOXVent[0], fFlow/5.0);
			SetThrusterLevel(thGOXVent[1], fFlow/5.0);
		}
		else
		{
			SetThrusterLevel(thGOXVent[0], 0.0);
			SetThrusterLevel(thGOXVent[1], 0.0);
		}
	}
	else
	{
		SetThrusterLevel(thGOXVent[0], 0.0);
		SetThrusterLevel(thGOXVent[1], 0.0);
	}

	PlayVesselWave(SoundID, CRYO_HISS, LOOP);
	
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
	BaseSSUPad::clbkSaveState(scn); //default parameters

	WriteScenario_state(scn, "ACCESS_ARM", AccessArmState);
	WriteScenario_state(scn, "GVA", VentArmState);
	WriteScenario_state(scn, "VENTHOOD", VentHoodState);
	if(!bPad1985) {
		WriteScenario_state(scn, "FSS_OWP", FSS_OWP_State);
		WriteScenario_state(scn, "RSS_OWP", RSS_OWP_State);
	}
	WriteScenario_state(scn, "RSS", RSS_State);
	WriteScenario_state(scn, "FSS_GH2", GH2VentlineState);
	WriteScenario_state(scn, "FSS_IAA", IAAState);
	if(bPad1985) WriteScenario_state(scn, "RBUS", FSS_RBUS_UmbilicalState);
	//oapiWriteScenario_int(scn, "GOX_SEQUENCE", GOXArmAction);
	oapiWriteScenario_string(scn,"SHUTTLE", (char*)ShuttleName.c_str());
}

void SSUPad::clbkLoadStateEx(FILEHANDLE scn, void *status)
{
	char* line;

	while(oapiReadScenario_nextline(scn, line)) {
		if(!_strnicmp(line, "ACCESS_ARM", 10)) {
			sscan_state(line+10, AccessArmState);
			SetAnimation(anim_AccessArm, AccessArmState.pos);
		}
		else if(!_strnicmp(line, "GVA", 3)) {
			sscan_state(line+3, VentArmState);
			SetAnimation(anim_VentArm, VentArmState.pos);
		}
		else if(!_strnicmp(line, "VENTHOOD", 8)) {
			sscan_state(line+8, VentHoodState);
			SetAnimation(anim_VentHood, VentHoodState.pos);
		}
		/*else if(!_strnicmp(line, "GOX_SEQUENCE", 12)) {
			sscanf(line+12, "%d", &GOXArmAction);
		}
		else*/ if (!bPad1985 && !_strnicmp(line, "FSS_OWP", 7)) {
			sscan_state(line+7, FSS_OWP_State);
			SetAnimation(anim_fss_y_owp, FSS_OWP_State.pos);
			AnimateFSSOWPStrut();
		}
		else if (!bPad1985 && !_strnicmp(line, "RSS_OWP", 7)) {
			sscan_state(line+7, RSS_OWP_State);
			SetAnimation(anim_rss_y_owp, RSS_OWP_State.pos);
		}
		else if (!_strnicmp(line, "RSS", 3)) {
			sscan_state(line+3, RSS_State);
			SetAnimation(anim_rss, RSS_State.pos);
		}
		else if (!_strnicmp(line, "FSS_IAA", 7)) {
			sscan_state(line+7, IAAState);
			SetAnimation(anim_IAA, IAAState.pos);
		}
		else if (!_strnicmp(line, "FSS_GH2", 7)) {
			sscan_state(line+7, GH2VentlineState);
			SetAnimation(anim_GH2Ventline, GH2VentlineState.pos);
		}
		else if (!_strnicmp(line, "SHUTTLE", 7)) {
			//sscanf_s(line+7,"%s",&ShuttleName,sizeof(ShuttleName));
			ShuttleName = line+8; // read char array into std::string
			char cnam[256];
			sprintf(cnam,"%s%s","Shuttle name: ",ShuttleName.c_str());
			oapiWriteLog(cnam);
		}
		else if(bPad1985 && !_strnicmp(line,"RBUS",4)){
			sscan_state(line+4, FSS_RBUS_UmbilicalState);
			SetAnimation(anim_fss_rbus, FSS_RBUS_UmbilicalState.pos);
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
				if(bPad1985) return 0; // 1985 pad; no OWP
				if(RSS_State.Closed()) {
					if(RSS_OWP_State.Closing() || RSS_OWP_State.Closed()) RSS_OWP_State.action=AnimState::OPENING;
					else RSS_OWP_State.action=AnimState::CLOSING;
				}
				return 1;
			case OAPI_KEY_Y:
				if(bPad1985) return 0; // 1985 pad; no OWP
				if(FSS_OWP_State.Closing() || FSS_OWP_State.Closed()) FSS_OWP_State.action=AnimState::OPENING;
				else FSS_OWP_State.action=AnimState::CLOSING;
				return 1;
		}
	}

	/*switch(key) {
		case OAPI_KEY_O:
			//Recharge GOX_Vent .. test
			SetPropellantMass(phGOXVent, 8000000000000000.0);
			return 1;
	}*/
	return BaseSSUPad::clbkConsumeBufferedKey(key, down, keystate);
}

void SSUPad::CreateGOXVentThrusters() {

	static PARTICLESTREAMSPEC gox_stream = {
	  0, 0.8, 15, 7, 0, 1.2, 1, 3.0, PARTICLESTREAMSPEC::DIFFUSE, 
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
		UpdateGOXVents();
	}
}

void SSUPad::clbkSetClassCaps(FILEHANDLE cfg) {
	// check which version of pad is being used
	if(!oapiReadItem_bool(cfg, "PAD_1985", bPad1985)) bPad1985 = false;

	SetEmptyMass(2.000001);
	SetSize(392.5);
	
	//add meshes
	if(bPad1985) {
		FSSMesh=oapiLoadMeshGlobal(DEFAULT_MESHNAME_FSS_1985);
		RSSMesh=oapiLoadMeshGlobal(DEFAULT_MESHNAME_RSS_1985);
	}
	else {
		FSSMesh=oapiLoadMeshGlobal(DEFAULT_MESHNAME_FSS);
		RSSMesh=oapiLoadMeshGlobal(DEFAULT_MESHNAME_RSS);
	}
	HardStandMesh=oapiLoadMeshGlobal(DEFAULT_MESHNAME_HARDSTAND);
	WaterTowerMesh=oapiLoadMeshGlobal(DEFAULT_MESHNAME_WATERTOWER);
	const VECTOR3 rss_ofs=_V(14, 15.5, 1);
	const VECTOR3 hs_ofs=_V(-57.5, -1.75, 1.3);
	const VECTOR3 wt_ofs=_V(100, 45, -63);
	fss_mesh_idx=AddMesh(FSSMesh);
	rss_mesh_idx=AddMesh(RSSMesh, &rss_ofs);
	hs_mesh_idx=AddMesh(HardStandMesh, &hs_ofs);
	wt_mesh_idx=AddMesh(WaterTowerMesh, &wt_ofs);

	SetTouchdownPoints(_V(1.0, -2.0, 0.0), _V(-1.0, -2.0, 1.0), _V(-1.0, -2.0, -1.0));

	if (bPad1985 == true)// update GOX vents position, as it appears there's an offset between the "regular" pad and the "1985" pad
	{
		vtx_goxvent[0] = FSS_POS_GOXVENTL_1985;
		vtx_goxvent[1] = FSS_POS_GOXVENTR_1985;
	}

	//if(bPad1985) DefineAnimations1985();
	//else DefineAnimations();
	DefineAnimations();

	CreateGOXVentThrusters();
	
	SetOrbiterAccessArmRate(ORBITER_ACCESS_ARM_RATE_NORMAL, OAA_RATE_NORMAL);
	SetOrbiterAccessArmRate(ORBITER_ACCESS_ARM_RATE_EMERGENCY, OAA_RATE_EMERGENCY);
	SetGOXVentArmRate(GVA_RATE);
	SetGOXVentHoodRate(VENT_HOOD_RATE);
	SetGH2VentlineRate(FSS_GH2_ARM_RATE);
	SetIntertankAccessArmRate(FSS_IAA_RATE);

	CreateLights(FSS_POS_LIGHT, FSS_NUM_LIGHTS);
	CreateStadiumLights(LC39_LIGHT_POS, LC39_LIGHT_DIR, LC39_LIGHT_COUNT, LC39_LIGHT_RANGE, LC39_LIGHT_ATT0, LC39_LIGHT_ATT1, LC39_LIGHT_ATT2, LC39_LIGHT_UMBRA, LC39_LIGHT_PENUMBRA, LC39_LIGHT_DIFFUSE, LC39_LIGHT_SPECULAR, LC39_LIGHT_AMBIENT);

	//if(bPad1985) CreateAttachment(false, _V(4.45, 20, 1.25), _V(0, -1, 0), _V(1, 0, 0), "XMLP");
	//else CreateAttachment(false, _V(2.00, 21.50, -0.95), _V(0, -1, 0), _V(1, 0, 0), "XMLP");
	CreateAttachment(false, _V(5.15, 20, 1.25), _V(0, -1, 0), _V(1, 0, 0), "XMLP");
}

void SSUPad::UpdateGOXVents() {
	VECTOR3 dir = (vtx_goxvent[2] - vtx_goxvent[0]);
	dir = dir/length(dir);
	
	for(int i = 0; i<2; i++) {
		SetThrusterRef(thGOXVent[i], vtx_goxvent[i]);
		SetThrusterDir(thGOXVent[i], dir);
	}

}
