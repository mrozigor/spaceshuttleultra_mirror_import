#define ORBITER_MODULE
#include "SLC6.h"
#include "UltraMath.h"
#include "meshres_Tower.h"
#include "resource.h"
#include <DlgCtrl.h>

HINSTANCE hModule;

SLC6::SLC6(OBJHANDLE hVessel, int flightmodel)
	: VESSEL3(hVessel, flightmodel), ROFILevel(0.0), ROFIStartTime(0.0),
	SSSLevel(0.0), SSS_SSMESteam(0.0), SSS_SRBSteam(0.0),
	bGLSAutoSeq(false), timeToLaunch(31.0)
{
	hPadMesh = oapiLoadMeshGlobal(DEFAULT_MESHNAME_PAD);
	hTowerMesh = oapiLoadMeshGlobal(DEFAULT_MESHNAME_TOWER);

	AccessArmState.Set(AnimState::CLOSED, 0.0);
	VentArmState.Set(AnimState::CLOSED, 0.0);
	VentHoodState.Set(AnimState::OPEN, 1.0);
	GH2VentlineState.Set(AnimState::CLOSED, 0.0);
	IAAState.Set(AnimState::CLOSED, 0.0);
}

SLC6::~SLC6()
{
	for(unsigned int i=0;i<vpAnimations.size();i++) delete vpAnimations.at(i);
}

void SLC6::clbkSetClassCaps(FILEHANDLE cfg)
{
	pad_mesh_idx = AddMesh(hPadMesh, &PAD_MESH_OFFSET);
	tower_mesh_idx = AddMesh(hTowerMesh, &TOWER_MESH_OFFSET);

	ahHDP = CreateAttachment(false, _V(0, 6, -1.9), _V(0.0, 1.0, 0.0), _V(0.0, 0.0, 1.0), "XHDP");

	DefineAnimations();
	DefineROFIs();
	DefineSSS();
}

void SLC6::clbkPostCreation()
{
}

void SLC6::clbkPreStep(double simt, double simdt, double mjd)
{
	VESSEL3::clbkPreStep(simt, simdt, mjd);

	if(bGLSAutoSeq) {
		timeToLaunch -= simdt;
		if(timeToLaunch < -5.0) {
			SSS_SSMESteam = 0.0;
			SSS_SRBSteam = 0.0;
			SSSLevel = 0.0;
			bGLSAutoSeq = false;
		}
		else {
			if(timeToLaunch < 16.0) SSSLevel = 1.0;
			if(timeToLaunch < 6.6) SSS_SSMESteam = 1.0;
			if(timeToLaunch < 0.0) SSS_SRBSteam = 1.0;
		}
	}

	if(AccessArmState.Moving()) {
		AccessArmState.Move(simdt*SLC6_ORBITER_ACCESS_ARM_RATE);
		SetAnimation(anim_AccessArm, AccessArmState.pos);
	}

	// only allow vent hood to move when vent arm is fully extended or stopped
	// only allow arm to move when hood is closed (retracted)
	if(VentHoodState.Moving() && (Eq(VentArmState.pos, 1.0, 0.001) || VentArmState.Static())) {
		VentHoodState.Move(simdt*SLC6_VENT_HOOD_RATE);
		SetAnimation(anim_VentHood, VentHoodState.pos);
	}
	else if(VentArmState.Moving() && Eq(VentHoodState.pos, 0.0, 0.001)) {
		VentArmState.Move(simdt*SLC6_VENT_ARM_RATE);
		SetAnimation(anim_VentArm, VentArmState.pos);
	}

	//sprintf_s(oapiDebugString(), 255, "VentArm: %d %f VentHood: %d %f", VentArmState.action, VentArmState.pos, VentHoodState.action, VentHoodState.pos);

	if(GH2VentlineState.Moving()) {
		GH2VentlineState.Move(simdt*SLC6_GH2_ARM_RATE);
		SetAnimation(anim_GH2Ventline, GH2VentlineState.pos);
	}

	if(IAAState.Moving()) {
		IAAState.Move(simdt*SLC6_IAA_RATE);
		SetAnimation(anim_IAA, IAAState.pos);
	}

	if(ROFILevel>0.01 && (simt-ROFIStartTime)>10.0) ROFILevel=0.0;
}

void SLC6::clbkSaveState(FILEHANDLE scn)
{
	VESSEL3::clbkSaveState(scn); //default parameters

	WriteScenario_state(scn, "ACCESS_ARM", AccessArmState);
	WriteScenario_state(scn, "VENT_ARM", VentArmState);
	WriteScenario_state(scn, "VENT_HOOD", VentHoodState);
	WriteScenario_state(scn, "GH2_VENTLINE", GH2VentlineState);
	WriteScenario_state(scn, "IAA", IAAState);
}

void SLC6::clbkLoadStateEx(FILEHANDLE scn, void * status)
{
	char* line;

	while(oapiReadScenario_nextline(scn, line)) {
		if(!_strnicmp(line, "ACCESS_ARM", 10)) {
			sscan_state(line+10, AccessArmState);
			SetAnimation(anim_AccessArm, AccessArmState.pos);
		}
		else if(!_strnicmp(line, "VENT_ARM", 8)) {
			sscan_state(line+8, VentArmState);
			SetAnimation(anim_VentArm, VentArmState.pos);
		}
		else if(!_strnicmp(line, "VENT_HOOD", 9)) {
			sscan_state(line+9, VentHoodState);
			SetAnimation(anim_VentHood, VentHoodState.pos);
		}
		else if(!_strnicmp(line, "GH2_VENTLINE", 12)) {
			sscan_state(line+12, GH2VentlineState);
			SetAnimation(anim_GH2Ventline, GH2VentlineState.pos);
		}
		else if(!_strnicmp(line, "IAA", 3)) {
			sscan_state(line+3, IAAState);
			SetAnimation(anim_IAA, IAAState.pos);
		}
		else ParseScenarioLineEx(line, status);
	}
}

int SLC6::clbkConsumeBufferedKey(DWORD key, bool down, char * keystate)
{
	if(!down) return 0; //only handle keydown events

	if(KEYMOD_CONTROL(keystate) && key == OAPI_KEY_SPACE) {
		if(oapiOpenDialogEx(hModule, IDD_DLG_ANIMCTRL, SLC6_DlgProc, DLG_CAPTIONCLOSE, this))
			oapiWriteLog("SSU Pad: Dialog opened");
		else if(hModule) sprintf_s(oapiDebugString(), 255, "Error: %d", GetLastError());
		return 1;
	}
	/*if(KEYMOD_CONTROL(keystate) && key == OAPI_KEY_R) {
		TriggerROFIs();
		return 1;
	}
	if(KEYMOD_CONTROL(keystate) && key == OAPI_KEY_L) {
		//ActivateSSS();
		SSSLevel = 1.0 - SSSLevel;
		SSS_SRBSteam = 1.0 - SSS_SRBSteam;
		SSS_SSMESteam = 1.0 - SSS_SSMESteam;
		return 1;
	}*/

	switch(key) {
		case OAPI_KEY_K:
			if(AccessArmState.Open()) RetractOrbiterAccessArm();
			else ExtendOrbiterAccessArm();
			return 1;
		case OAPI_KEY_G:
			if(VentHoodState.Closed()) ExtendGOXArmAndHood();
			else RetractGOXArmAndHood();
			return 1;
		case OAPI_KEY_V:
			DetachGH2Ventline();
			return 1;
		case OAPI_KEY_A:
			if(IAAState.Open()) RetractIAA();
			else DeployIAA();
			return 1;
	}

	return 0;
}

void SLC6::GLSAutoSeqStart()
{
	timeToLaunch = 31.0;
	bGLSAutoSeq = true;
}

void SLC6::OnT0()
{
	DetachGH2Ventline();
	DetachShuttle();
}

void SLC6::TriggerROFIs()
{
	ROFILevel = 1.0;
	ROFIStartTime = oapiGetSimTime();
}

void SLC6::ActivateSSS()
{
	//bSSSActive = true;
	SSSLevel = 1.0;
}

void SLC6::ExtendOrbiterAccessArm()
{
	AccessArmState.action=AnimState::OPENING;
}

void SLC6::RetractOrbiterAccessArm()
{
	AccessArmState.action=AnimState::CLOSING;
}

void SLC6::HaltOrbiterAccessArm()
{
	AccessArmState.action=AnimState::STOPPED;
}

void SLC6::ExtendGOXArm()
{
	VentArmState.action=AnimState::OPENING;
}

void SLC6::RetractGOXArm()
{
	VentArmState.action=AnimState::CLOSING;
}

void SLC6::HaltGOXArm()
{
	VentArmState.action=AnimState::STOPPED;
}

void SLC6::AttachGH2Ventline()
{
	GH2VentlineState.action=AnimState::CLOSING;
}

void SLC6::DetachGH2Ventline()
{
	GH2VentlineState.action=AnimState::OPENING;
}

void SLC6::RaiseVentHood()
{
	VentHoodState.action=AnimState::CLOSING;
}

void SLC6::LowerVentHood()
{
	VentHoodState.action=AnimState::OPENING;
}

void SLC6::HaltVentHood()
{
	VentHoodState.action=AnimState::STOPPED;
}

void SLC6::ExtendGOXArmAndHood()
{
	ExtendGOXArm();
	LowerVentHood();
}

void SLC6::RetractGOXArmAndHood()
{
	RetractGOXArm();
	RaiseVentHood();
}

void SLC6::HaltGOXArmAndHood()
{
	HaltGOXArm();
	HaltVentHood();
}

void SLC6::DeployIAA()
{
	IAAState.action=AnimState::OPENING;
}

void SLC6::HaltIAA()
{
	IAAState.action=AnimState::STOPPED;
}

void SLC6::RetractIAA()
{
	IAAState.action=AnimState::CLOSING;
}

void SLC6::DetachShuttle() const
{
	OBJHANDLE hShuttle = GetAttachmentStatus(ahHDP);
	if(hShuttle) {
		VESSELSTATUS2 st;
		st.version = 2;
		st.flag = 0;
		VESSELSTATUS2 st2;
		st2.version = 2;
		st2.flag = 0;
		GetStatusEx(&st2);

		DetachChild(ahHDP);

		VESSEL* pV = oapiGetVesselInterface(hShuttle);
		pV->GetStatusEx(&st);
		st.rbody = st2.rbody;
		st.rvel = st2.rvel;
		st.status = 0;
		pV->DefSetStateEx(&st);
	}
}

void SLC6::DefineROFIs()
{
	const VECTOR3 FWD_LEFT_ROFI_POS = _V(-4.09, 9.66, 7.59);
	const VECTOR3 FWD_RIGHT_ROFI_POS = _V(4.09, 9.66, 7.59);
	const VECTOR3 AFT_LEFT_ROFI_POS = _V(-3.82, 9.66, 4.42);
	const VECTOR3 AFT_RIGHT_ROFI_POS = _V(3.82, 9.66, 4.42);
	static PARTICLESTREAMSPEC ROFI_Stream = {
		0, 0.1, 300.0, 17.5, 0.1, 0.30, 0, 0.5, PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_FLAT, 1, 1,
		PARTICLESTREAMSPEC::ATM_FLAT, 1, 1
	};

	AddParticleStream(&ROFI_Stream, FWD_LEFT_ROFI_POS, _V(1, 0, 0), &ROFILevel);
	AddParticleStream(&ROFI_Stream, FWD_RIGHT_ROFI_POS, _V(-1, 0, 0), &ROFILevel);
	AddParticleStream(&ROFI_Stream, AFT_LEFT_ROFI_POS, _V(1, 0, 0), &ROFILevel);
	AddParticleStream(&ROFI_Stream, AFT_RIGHT_ROFI_POS, _V(-1, 0, 0), &ROFILevel);
}

void SLC6::DefineSSS()
{
	static PARTICLESTREAMSPEC sss_steam = {
		0, 8, 20, 250.0, 0.3, 10, 3, 5, PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_PSQRT, 0, 0.1,
		PARTICLESTREAMSPEC::ATM_PLOG, 1e-6, 1.0};
	sss_steam.tex = oapiRegisterParticleTexture("contrail4");
	static PARTICLESTREAMSPEC sss_water = {
		0, 0.05, 100.0, 12.0, 0.1, 0.30, 3, 2, PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_FLAT, 1, 1,
		PARTICLESTREAMSPEC::ATM_FLAT, 1, 1
	};

	for(int i=0;i<=19;i++) {
		double zpos = 4+0.4*i;
		AddParticleStream(&sss_water, _V(-4.11, 6.48, zpos), _V(1, 0, 0), &SSSLevel);
		AddParticleStream(&sss_water, _V(4.11, 6.48, zpos), _V(-1, 0, 0), &SSSLevel);
	}
	for(int i=0;i<=10;i++) {
		double zpos = -2.73+0.3*i;
		AddParticleStream(&sss_water, _V(-9.52, 6.48, zpos), _V(1, 0, 0), &SSSLevel);
		AddParticleStream(&sss_water, _V(-1.75, 6.48, zpos), _V(-1, 0, 0), &SSSLevel);
		AddParticleStream(&sss_water, _V(9.52, 6.48, zpos), _V(-1, 0, 0), &SSSLevel);
		AddParticleStream(&sss_water, _V(1.75, 6.48, zpos), _V(1, 0, 0), &SSSLevel);
	}
	AddParticleStream(&sss_steam, _V(-63.0, -7.0, 50.0), _V(-cos(10.0*RAD), sin(10.0*RAD), 0), &SSS_SSMESteam);
	AddParticleStream(&sss_steam, _V(-75.0, -5.0, -10.0), _V(-cos(10.0*RAD), sin(10.0*RAD), 0), &SSS_SRBSteam);
	AddParticleStream(&sss_steam, _V(50.0, -5.0, -10.0), _V(cos(10.0*RAD), sin(10.0*RAD), 0), &SSS_SRBSteam);
}

void SLC6::DefineAnimations()
{
	static UINT AccessArmGrp[1] = {GRP_OAA};
	MGROUP_TRANSLATE* pAccessArm = DefineTranslation(tower_mesh_idx, AccessArmGrp, 1, _V(-19.75, 0, 3.476));
	anim_AccessArm = CreateAnimation(0.0);
	AddAnimationComponent(anim_AccessArm, 0.0, 1.0, pAccessArm);

	static UINT VentArmGrp[4] = {GRP_GOX_VENT_PIPES, GRP_GVA_SWING_ARM, GRP_NORTH_GOX_VENT_CYLINDER_01, GRP_SOUTH_GOX_VENT_CYLINDER_01};
	MGROUP_ROTATE* pVentArm = DefineRotation(tower_mesh_idx, VentArmGrp, 4, _V(3.3, 60.876, -23.148), _V(0, -1, 0), static_cast<float>(77.5*RAD));
	anim_VentArm = CreateAnimation(0.0);
	ANIMATIONCOMPONENT_HANDLE parent = AddAnimationComponent(anim_VentArm, 0.0, 1.0, pVentArm);

	static UINT VentHoodGrp[5] = {GRP_GOX_VENT_HOOD, GRP_NORTH_GOX_VENT_CYLINDER_02, GRP_NORTH_GOX_VENT_CYLINDER_03,
		GRP_SOUTH_GOX_VENT_CYLINDER_02, GRP_SOUTH_GOX_VENT_CYLINDER_03};
	MGROUP_ROTATE* pVentHood = DefineRotation(tower_mesh_idx, VentHoodGrp, 5, _V(3.327, 62.599, 1.888), _V(1, 0, 0), static_cast<float>(48.0*RAD));
	anim_VentHood = CreateAnimation(1.0);
	AddAnimationComponent(anim_VentHood, 0.0, 1.0, pVentHood, parent);

	static UINT GH2LineGrp[3] = {GRP_GH2_FWD_VENT_FLEX_LINE, GRP_GH2_VENT_HARD_LINE, GRP_GUCP};
	MGROUP_ROTATE* pGH2Line = DefineRotation(tower_mesh_idx, GH2LineGrp, 3, _V(-11.928, 45.48, -24.215), _V(0.495629, 0, 0.868534), static_cast<float>(90.0*RAD));
	anim_GH2Ventline = CreateAnimation(0.0);
	AddAnimationComponent(anim_GH2Ventline, 0.0, 1.0, pGH2Line);

	static UINT IAAGrp[1] = {GRP_INTERTANK_ACCESS_ARM};
	MGROUP_ROTATE* pIAA = DefineRotation(tower_mesh_idx, IAAGrp, 1, _V(-7.725, 43.15, -28.065), _V(0, 1, 0), static_cast<float>(210.0*RAD));
	anim_IAA = CreateAnimation(0.0);
	AddAnimationComponent(anim_IAA, 0.0, 1.0, pIAA);
}

MGROUP_ROTATE* SLC6::DefineRotation(UINT mesh, UINT * grp, UINT ngrp, const VECTOR3 & ref, const VECTOR3 & axis, float angle)
{
	MGROUP_ROTATE* mgrp = new MGROUP_ROTATE(mesh, grp, ngrp, ref, axis, angle);
	vpAnimations.push_back(mgrp);
	return mgrp;
}

MGROUP_TRANSLATE* SLC6::DefineTranslation(UINT mesh, UINT* grp, UINT ngrp, const VECTOR3& shift)
{
	MGROUP_TRANSLATE* mgrp = new MGROUP_TRANSLATE(mesh, grp, ngrp, shift);
	vpAnimations.push_back(mgrp);
	return mgrp;
}

//global functions
DLLCLBK void InitModule(HINSTANCE hDLL)
{
	hModule=hDLL;
	oapiRegisterCustomControls(hDLL);
}

DLLCLBK void ExitModule(HINSTANCE hDLL)
{
	oapiUnregisterCustomControls(hDLL);
}

DLLCLBK VESSEL* ovcInit(OBJHANDLE hVessel, int iFlightModel)
{
	return new SLC6(hVessel, iFlightModel);
}

DLLCLBK void ovcExit(VESSEL* pVessel)
{
	delete static_cast<SLC6*>(pVessel);
}

BOOL CALLBACK SLC6_DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg==WM_COMMAND) {
		SLC6* pad=static_cast<SLC6*>(oapiGetDialogContext(hWnd));
		switch(LOWORD(wParam)) {
			case IDCANCEL:
				oapiCloseDialog(hWnd);
				return TRUE;
			case IDC_OAA_DEPLOY:
				pad->ExtendOrbiterAccessArm();
				return TRUE;
			case IDC_OAA_HALT:
				pad->HaltOrbiterAccessArm();
				return TRUE;
			case IDC_OAA_RETRACT:
				pad->RetractOrbiterAccessArm();
				return TRUE;
			case IDC_GVA_EXTEND:
				pad->ExtendGOXArm();
				return TRUE;
			case IDC_GVA_HALT:
				pad->HaltGOXArm();
				return TRUE;
			case IDC_GVA_RETRACT:
				pad->RetractGOXArm();
				return TRUE;
			case IDC_HOOD_LOWER:
				pad->LowerVentHood();
				return TRUE;
			case IDC_HOOD_HALT:
				pad->HaltVentHood();
				return TRUE;
			case IDC_HOOD_RAISE:
				pad->RaiseVentHood();
				return TRUE;
			case IDC_GH2_DETACH:
				pad->DetachGH2Ventline();
				return TRUE;
			case IDC_GH2_ATTACH:
				pad->AttachGH2Ventline();
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
		}
	}
	//if message has not been handled in this function, perform default action
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}
