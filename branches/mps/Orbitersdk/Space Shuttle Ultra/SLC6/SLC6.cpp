#define ORBITER_MODULE
#include "../Atlantis.h"
#include "SLC6.h"
#include "UltraMath.h"
#include "meshres_Tower.h"
#include "meshres_Pad.h"
#include "meshres_SAB.h"
#include "resource.h"
#include <DlgCtrl.h>

HINSTANCE hModule;

const unsigned int SLC6_LIGHT_COUNT = 10;
const VECTOR3 SLC6_LIGHT_OFFSET = _V(0.0, -13.5, -99.75);
const VECTOR3 SLC6_LIGHT_POS[SLC6_LIGHT_COUNT] = {
	_V(-56.546, 33.334, 66.734) + SLC6_LIGHT_OFFSET, // Cylinder01
	_V(-45.87, 33.334, 75.326) + SLC6_LIGHT_OFFSET, // Cylinder03
	_V(-56.546, 33.334, 31.012) + SLC6_LIGHT_OFFSET, // Cylinder04
	_V(-56.546, 33.334, 3.683) + SLC6_LIGHT_OFFSET, // Cylinder05
	_V(-56.546, 33.334, -25.767) + SLC6_LIGHT_OFFSET, // Cylinder06
	_V(-45.87, 33.334, 117.66) + SLC6_LIGHT_OFFSET, // Cylinder07
	_V(30.134, 33.334, 133.743) + SLC6_LIGHT_OFFSET, // Cylinder08
	_V(46.991, 33.334, 72.154) + SLC6_LIGHT_OFFSET, // Cylinder09
	_V(39.889, 33.334, 66.295) + SLC6_LIGHT_OFFSET, // Cylinder10
	_V(30.134, 33.334, 181.533) + SLC6_LIGHT_OFFSET // Cylinder11
};
const VECTOR3 SLC6_LIGHT_DIR[SLC6_LIGHT_COUNT] = {
	_V(1, 0, 0),
	_V(1, 0, 0),
	_V(1, 0, 0),
	_V(1, 0, 0),
	_V(1, 0, 0),
	_V(1, 0, 0),
	_V(-1, 0, 0),
	_V(-1, 0, 0),
	_V(-1, 0, 0),
	_V(-1, 0, 0)
};

SLC6::SLC6(OBJHANDLE hVessel, int flightmodel)
	: BaseSSUPad(hVessel, flightmodel), ROFILevel(0.0), ROFIStartTime(0.0),
	  SSSLevel(0.0), SSS_SSMESteam(0.0), SSS_SRBSteam(0.0),
	  bFirstStep(true), bGLSAutoSeq(false), timeToLaunch(31.0),
	  pSTS(NULL)
{
	hPadMesh = oapiLoadMeshGlobal(DEFAULT_MESHNAME_PAD);
	hTowerMesh = oapiLoadMeshGlobal(DEFAULT_MESHNAME_TOWER);
	hPCRMesh = oapiLoadMeshGlobal(DEFAULT_MESHNAME_PCR);
	hSABMesh = oapiLoadMeshGlobal(DEFAULT_MESHNAME_SAB);
	hMSTMesh = oapiLoadMeshGlobal(DEFAULT_MESHNAME_MST);

	goxVentPos[0] = GOXVENT_LEFT;
	goxVentPos[1] = GOXVENT_RIGHT;
	goxVentPos[2] = GOXVENT_DIRREF;

	phGOXVent = NULL;
	thGOXVent[0] = thGOXVent[1] = NULL;

	AccessArmState.Set(AnimState::CLOSED, 0.0);
	VentArmState.Set(AnimState::CLOSED, 0.0);
	VentHoodState.Set(AnimState::OPEN, 1.0);
	GH2VentlineState.Set(AnimState::CLOSED, 0.0);
	T0UmbilicalState.Set(AnimState::CLOSED, 0.0);
	IAAState.Set(AnimState::CLOSED, 0.0);
	PCRState.Set(AnimState::CLOSED, 0.0);
	SABState.Set(AnimState::CLOSED, 0.0);
	MSTState.Set(AnimState::CLOSED, 0.0);
	SABDoorState.Set(AnimState::CLOSED, 0.0);
}

SLC6::~SLC6()
{
}

void SLC6::clbkSetClassCaps(FILEHANDLE cfg)
{
	pad_mesh_idx = AddMesh(hPadMesh, &PAD_MESH_OFFSET);
	tower_mesh_idx = AddMesh(hTowerMesh, &TOWER_MESH_OFFSET);
	pcr_mesh_idx = AddMesh(hPCRMesh, &PCR_MESH_OFFSET);
	sab_mesh_idx = AddMesh(hSABMesh, &SAB_MESH_OFFSET);
	mst_mesh_idx = AddMesh(hMSTMesh, &MST_MESH_OFFSET);

	ahHDP = CreateAttachment(false, _V(0, 6, -1.9), _V(0.0, 1.0, 0.0), _V(0.0, 0.0, 1.0), "XHDP");

	DefineAnimations();
	DefineROFIs();
	DefineSSS();
	DefineGOXVents();
	
	SetOrbiterAccessArmRate(SLC6_ORBITER_ACCESS_ARM_RATE);
	SetGOXVentArmRate(SLC6_VENT_ARM_RATE);
	SetGOXVentHoodRate(SLC6_VENT_HOOD_RATE);
	SetGH2VentlineRate(SLC6_GH2_ARM_RATE);
	SetIntertankAccessArmRate(SLC6_IAA_RATE);
	
	CreateStadiumLights(SLC6_LIGHT_POS, SLC6_LIGHT_DIR, SLC6_LIGHT_COUNT);
}

void SLC6::clbkPostCreation()
{
	UpdateGOXVents();
	SetPropellantMass(phGOXVent, 0.01); // ensure that fake propellant tank for GOX venting has fuel
}

void SLC6::clbkPreStep(double simt, double simdt, double mjd)
{
	BaseSSUPad::clbkPreStep(simt, simdt, mjd);

	if(bFirstStep) {
		OBJHANDLE hSTS = GetAttachmentStatus(ahHDP);
		if(hSTS!=NULL)
		{
			VESSEL *pVessel = oapiGetVesselInterface(hSTS);
			if(pVessel && !_strnicmp(pVessel->GetClassName(), "SpaceShuttleUltra",17))
				pSTS = static_cast<Atlantis*>(pVessel);
			else
				pSTS = NULL;
		}

		//UpdateGOXVents();
		//SetPropellantMass(phGOXVent, 0.01); // ensure that fake propellant tank for GOX venting has fuel

		bFirstStep = false;
	}

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

	if(T0UmbilicalState.Moving()) {
		double dp=simdt*SLC6_TSM_UMBILICAL_RETRACT_SPEED;
		T0UmbilicalState.Move(dp);
		SetAnimation(anim_T0Umb, T0UmbilicalState.pos);
	}

	if(PCRState.Moving()) {
		PCRState.Move(simdt*SLC6_PCR_TRANSLATE_RATE);
		SetAnimation(anim_PCR, PCRState.pos);
	}

	if(SABState.Moving()) {
		SABState.Move(simdt*SLC6_SAB_TRANSLATE_RATE);
		SetAnimation(anim_SAB, SABState.pos);
	}
	if(SABDoorState.Moving()) {
		SABDoorState.Move(simdt*SLC6_SAB_DOOR_RATE);
		SetAnimation(anim_SABDoor, SABDoorState.pos);
	}

	if(MSTState.Moving()) {
		// only allow MST to move if IAA is retracted and GH2 ventline is detached
		if(IAAState.Closed() && GH2VentlineState.Open()) {
			MSTState.Move(simdt*SLC6_MST_TRANSLATE_RATE);
			SetAnimation(anim_MST, MSTState.pos);
		}
	}

	if(VentHoodState.Open() && pSTS && pSTS->GetETPropellant()>=60) {
		double fFlow = static_cast<double>(pSTS->GetETPropellant())/100.0;
		SetThrusterLevel(thGOXVent[0], fFlow/5.0);
		SetThrusterLevel(thGOXVent[1], fFlow/5.0);

		UpdateGOXVents();
	}
	else
	{
		SetThrusterLevel(thGOXVent[0], 0.0);
		SetThrusterLevel(thGOXVent[1], 0.0);
	}

	if(ROFILevel>0.01 && (simt-ROFIStartTime)>10.0) ROFILevel=0.0;
}

void SLC6::clbkSaveState(FILEHANDLE scn)
{
	BaseSSUPad::clbkSaveState(scn); //default parameters

	WriteScenario_state(scn, "ACCESS_ARM", AccessArmState);
	WriteScenario_state(scn, "VENT_ARM", VentArmState);
	WriteScenario_state(scn, "VENT_HOOD", VentHoodState);
	WriteScenario_state(scn, "GH2_VENTLINE", GH2VentlineState);
	WriteScenario_state(scn, "T0_UMB", T0UmbilicalState);
	WriteScenario_state(scn, "IAA", IAAState);
	WriteScenario_state(scn, "PCR", PCRState);
	WriteScenario_state(scn, "SAB", SABState);
	WriteScenario_state(scn, "MST", MSTState);
	WriteScenario_state(scn, "SABDoor", SABDoorState);
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
		else if(!_strnicmp(line, "T0_UMB", 6)) {
			sscan_state(line+6, T0UmbilicalState);
			SetAnimation(anim_T0Umb, T0UmbilicalState.pos);
		}
		else if(!_strnicmp(line, "IAA", 3)) {
			sscan_state(line+3, IAAState);
			SetAnimation(anim_IAA, IAAState.pos);
		}
		else if(!_strnicmp(line, "PCR", 3)) {
			sscan_state(line+3, PCRState);
			SetAnimation(anim_PCR, PCRState.pos);
		}
		else if(!_strnicmp(line, "SABDoor", 7)) {
			sscan_state(line+7, SABDoorState);
			SetAnimation(anim_SABDoor, SABDoorState.pos);
		}
		else if(!_strnicmp(line, "SAB", 3)) {
			sscan_state(line+3, SABState);
			SetAnimation(anim_SAB, SABState.pos);
		}
		else if(!_strnicmp(line, "MST", 3)) {
			sscan_state(line+3, MSTState);
			SetAnimation(anim_MST, MSTState.pos);
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
		case OAPI_KEY_C:
			if(PCRState.Closed()) ExtendPCR();
			else RetractPCR();
			return 1;
		case OAPI_KEY_Y:
			if(SABState.Closed()) ExtendSAB();
			else RetractSAB();
			return 1;
		case OAPI_KEY_M:
			if(MSTState.Closed()) ExtendMST();
			else RetractMST();
			return 1;
		case OAPI_KEY_D:
			if(SABDoorState.Closed()) OpenSABDoor();
			else CloseSABDoor();
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
	T0UmbilicalState.action=AnimState::CLOSING;
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

void SLC6::ExtendPCR()
{
	PCRState.action = AnimState::OPENING;
}

void SLC6::HaltPCR()
{
	PCRState.action = AnimState::STOPPED;
}

void SLC6::RetractPCR()
{
	PCRState.action = AnimState::CLOSING;
}

void SLC6::ExtendSAB()
{
	SABState.action = AnimState::OPENING;
}

void SLC6::HaltSAB()
{
	SABState.action = AnimState::STOPPED;
}

void SLC6::RetractSAB()
{
	SABState.action = AnimState::CLOSING;
}

void SLC6::OpenSABDoor()
{
	SABDoorState.action = AnimState::OPENING;
}

void SLC6::HaltSABDoor()
{
	SABDoorState.action = AnimState::STOPPED;
}

void SLC6::CloseSABDoor()
{
	SABDoorState.action = AnimState::CLOSING;
}

void SLC6::ExtendMST()
{
	// only extend MST if IAA is retracted and GH2 ventline is detached
	if(IAAState.Closed() && GH2VentlineState.Open())
		MSTState.action = AnimState::OPENING;
}

void SLC6::HaltMST()
{
	MSTState.action = AnimState::STOPPED;
}

void SLC6::RetractMST()
{
	MSTState.action = AnimState::CLOSING;
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

void SLC6::DefineGOXVents()
{
	static PARTICLESTREAMSPEC gox_stream = {
		0, 0.8, 15, 7, 0, 1.2, 1, 3.0, PARTICLESTREAMSPEC::DIFFUSE, 
		PARTICLESTREAMSPEC::LVL_PSQRT, 0, 1, 
		PARTICLESTREAMSPEC::ATM_PLOG, 1e-50, 1
	};

	gox_stream.tex = oapiRegisterParticleTexture ("SSU\\GOX_stream");

	//AddParticleStream(&gox_stream, 
	phGOXVent = CreatePropellantResource(0.01);
	VECTOR3 dir = Normalize(goxVentPos[2]-goxVentPos[0]);
	for(int i=0;i<2;i++) {
		thGOXVent[i] = CreateThruster(goxVentPos[i], dir, 0.0, phGOXVent);
		AddExhaustStream(thGOXVent[i], &gox_stream);
	}
}

void SLC6::UpdateGOXVents()
{
	VECTOR3 dir = Normalize(goxVentPos[2]-goxVentPos[0]);
	for(int i=0;i<2;i++) {
		SetThrusterRef(thGOXVent[i], goxVentPos[i]+TOWER_MESH_OFFSET);
		SetThrusterDir(thGOXVent[i], dir);
	}
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
	MGROUP_ROTATE* pVentVector = DefineRotation(LOCALVERTEXLIST, MAKEGROUPARRAY(goxVentPos), 3, _V(0.0, 0.0, 0.0), _V(0, 1, 0), static_cast<float>(0.0));
	AddAnimationComponent(anim_VentArm, 0.0, 1.0, pVentVector, parent);

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

	static UINT LeftT0UmbGrp[1] = {GRP_SLC6_PAD_LH_T0_UMBILICALS};
	MGROUP_ROTATE* pLeftT0Umb = DefineRotation(pad_mesh_idx, LeftT0UmbGrp, 1, _V(5.30725, 9.1715, 6.0545), _V(-0.0867, 0.0, 0.9962), (float)(17.0*RAD));
	static UINT RightT0UmbGrp[1] = {GRP_SLC6_PAD_RH_T0_UMBILICALS};
	MGROUP_ROTATE* pRightT0Umb = DefineRotation(pad_mesh_idx, RightT0UmbGrp, 1, _V(-5.30725, 9.1715, 6.0545), _V(-0.0867, 0.0, -0.9962), (float)(17.0*RAD));
	static UINT LeftT0UmbCoverGrp[1] = {GRP_SLC6_PAD_LH_TSM_BONNET};
	MGROUP_ROTATE* pLeftT0UmbCover = DefineRotation(pad_mesh_idx, LeftT0UmbCoverGrp, 1, _V(5.75, 16.36, 6.088), _V(0.0867, 0.0, -0.9962), (float)(90.0*RAD));
	static UINT RightT0UmbCoverGrp[1] = {GRP_SLC6_PAD_RH_TSM_BONNET};
	MGROUP_ROTATE* pRightT0UmbCover = DefineRotation(pad_mesh_idx, RightT0UmbCoverGrp, 1, _V(-5.75, 16.36, 6.088), _V(0.0867, 0.0, 0.9962), (float)(90.0*RAD));
	anim_T0Umb = CreateAnimation(0.0);
	AddAnimationComponent(anim_T0Umb, 0.5, 1, pLeftT0Umb);
	AddAnimationComponent(anim_T0Umb, 0.5, 1, pRightT0Umb);
	AddAnimationComponent(anim_T0Umb, 0, 0.55, pLeftT0UmbCover);
	AddAnimationComponent(anim_T0Umb, 0, 0.55, pRightT0UmbCover);

	MGROUP_TRANSLATE* pPCR = DefineTranslation(pcr_mesh_idx, NULL, 0, _V(0, 0, -223));
	anim_PCR = CreateAnimation(0.0);
	AddAnimationComponent(anim_PCR, 0.0, 1.0, pPCR);

	MGROUP_TRANSLATE* pSAB = DefineTranslation(sab_mesh_idx, NULL, 0, _V(0, 0, -104.5));
	anim_SAB = CreateAnimation(0.0);
	AddAnimationComponent(anim_SAB, 0.0, 1.0, pSAB);

	MGROUP_TRANSLATE* pMST = DefineTranslation(mst_mesh_idx, NULL, 0, _V(0, 0, 115));
	anim_MST = CreateAnimation(0.0);
	AddAnimationComponent(anim_MST, 0.0, 1.0, pMST);
	// rotate IAA structure out of way when MST is extended
	// in theory, the IAA and GH2 ventline animations should be children of this animation component; for the moment, we just prevent the MST from moving when the IAA is deployed or the GH2 ventline is attached
	static UINT IAAStructureGrp[9] = {GRP_IAA_STRUCTURE, GRP_GH2_AFT_VENT_FLEX_HOSE, GRP_GH2_AFT_VENT_HARD_LINE, GRP_GH2_PIVOT_POINT, GRP_GH2_VENT_LINE_HAUNCH, GRP_GH2_FWD_VENT_FLEX_LINE, GRP_GH2_VENT_HARD_LINE, GRP_GUCP, GRP_INTERTANK_ACCESS_ARM};
	MGROUP_ROTATE* pIAAStructure = DefineRotation(tower_mesh_idx, IAAStructureGrp, 9, _V(4.348, 0.0, -25.298), _V(0, 1, 0), static_cast<float>(-90.0*RAD)); // rotation angle is just a guess
	AddAnimationComponent(anim_MST, 0.0, 0.05, pIAAStructure);

	static UINT SABDoorGrp[7] = {GRP_Door_panel7_SAB, GRP_Door_panel6_SAB, GRP_Door_panel5_SAB, GRP_Door_panel4_SAB, GRP_Door_panel3_SAB, GRP_Door_panel2_SAB, GRP_Door_panel1_SAB};
	MGROUP_TRANSLATE* pSABDoor[7];
	anim_SABDoor = CreateAnimation(0.0);
	double start = 0.0;
	for(int i=0;i<7;i++) {
		pSABDoor[i] = DefineTranslation(sab_mesh_idx, &SABDoorGrp[i], 1, _V(0, (7-i)*7.5, 0));
		AddAnimationComponent(anim_SABDoor, start, 1.0, pSABDoor[i]);
		start += 1.0/7.0;
	}
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
			case IDC_PCR_EXTEND:
				pad->ExtendPCR();
				return TRUE;
			case IDC_PCR_HALT:
				pad->HaltPCR();
				return TRUE;
			case IDC_PCR_RETRACT:
				pad->RetractPCR();
				return TRUE;
			case IDC_SAB_EXTEND:
				pad->ExtendSAB();
				return TRUE;
			case IDC_SAB_HALT:
				pad->HaltSAB();
				return TRUE;
			case IDC_SAB_RETRACT:
				pad->RetractSAB();
				return TRUE;
			case IDC_SAB_DOOR_OPEN:
				pad->OpenSABDoor();
				return TRUE;
			case IDC_SAB_DOOR_HALT:
				pad->HaltSABDoor();
				return TRUE;
			case IDC_SAB_DOOR_CLOSE:
				pad->CloseSABDoor();
				return TRUE;
			case IDC_MST_EXTEND:
				pad->ExtendMST();
				return TRUE;
			case IDC_MST_HALT:
				pad->HaltMST();
				return TRUE;
			case IDC_MST_RETRACT:
				pad->RetractMST();
				return TRUE;
		}
	}
	//if message has not been handled in this function, perform default action
	return oapiDefDialogProc(hWnd, uMsg, wParam, lParam);
}
