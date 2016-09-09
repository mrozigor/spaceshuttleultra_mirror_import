#include "PanelA7A8ODS.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"
#include "DiscreteBundleManager.h"
#include "../meshres_odsvc.h"

extern GDIParams g_Param;

namespace vc {

	using class discsignals::DiscreteBundleManager;
	using class discsignals::DiscreteBundle;

	PanelA7A8ODS::PanelA7A8ODS(Atlantis* _sts)
		: AtlantisPanel(_sts, "A7A3/A8A3")
	{

		mesh_odspanel = oapiLoadMeshGlobal(ODSPANEL_MESHNAME);

		Add(plADS				= new StandardLight(_sts, "ADS"));
		Add(plBDS				= new StandardLight(_sts, "BDS"));
		Add(plCDS				= new StandardLight(_sts, "CDS"));

		Add(plPowerOn			= new StandardLight(_sts, "POWER ON(light)"));
		Add(plAPDSProtectCircOff= new StandardLight(_sts, "APDS CIRCUIT PROTECT OFF(light)"));
		Add(plRingAligned		= new StandardLight(_sts, "RING ALIGNED"));
		Add(plRingInitialPos	= new StandardLight(_sts, "RING INITIAL POSITION"));
		Add(plFixersOff			= new StandardLight(_sts, "FIXERS OFF"));
		Add(plHooks1Open		= new StandardLight(_sts, "HOOKS 1 OPEN"));
		Add(plHooks2Open		= new StandardLight(_sts, "HOOKS 2 OPEN"));
		Add(plLatchesClosed		= new StandardLight(_sts, "LATCHES CLOSED"));
		Add(plUndockComplete	= new StandardLight(_sts, "UNDOCK COMPLET"));

		Add(plInitialContact	= new StandardLight(_sts, "INITIAL CONTACT"));
		Add(plCapture			= new StandardLight(_sts, "CAPTURE CAPTURE"));
		Add(plRingForward		= new StandardLight(_sts, "RING FORWARD"));
		Add(plReadyToHook		= new StandardLight(_sts, "READY TO HOOK"));
		Add(plInterpSealed		= new StandardLight(_sts, "INTERP SEALED"));
		Add(plHooks1Closed		= new StandardLight(_sts, "HOOKS 1 CLOSED"));
		Add(plHooks2Closed		= new StandardLight(_sts, "HOOKS 2 CLOSED"));
		Add(plLatchesOpen		= new StandardLight(_sts, "LATCHES OPEN"));
		Add(plRingFinal			= new StandardLight(_sts, "RING FINAL POSITION"));

		Add(pSystemPowerMNA		= new StdSwitch3(_sts, "SYSTEM POWER MNA"));
		Add(pSystemPowerMNB		= new StdSwitch3(_sts, "SYSTEM POWER MNB"));
		Add(pPyroPowerMNA		= new StdSwitch2(_sts, "PYRO POWER MNA"));
		Add(pPyroPowerMNC		= new StdSwitch2(_sts, "PYRO POWER MNC"));
		Add(pSys1VentIsol		= new StdSwitch3(_sts, "SYS1 VENT ISOL"));
		Add(pSys1Vent			= new StdSwitch3(_sts, "SYS1 VENT"));
		Add(pSys2VentIsol		= new StdSwitch3(_sts, "SYS2 VENT ISOL"));
		Add(pSys2Vent			= new StdSwitch3(_sts, "SYS2 VENT"));
		Add(pPSUPowerMNA		= new StdSwitch2(_sts, "PSU POWER MNA"));
		Add(pPSUPowerMNB		= new StdSwitch2(_sts, "PSU POWER MNB"));
		Add(pLightsAirlock14	= new StdSwitch2(_sts, "LIGHTS AIRLOCK 1-4"));
		Add(pLightsAirlock23	= new StdSwitch2(_sts, "LIGHTS AIRLOCK 2-3"));
		Add(pLightsDockingFWD	= new StdSwitch2(_sts, "LIGHTS DOCKING TRUSS FWD"));
		Add(pLightsDockingAFT	= new StdSwitch2(_sts, "LIGHTS DOCKING TRUSS AFT"));
		Add(pARLKTNLFanA		= new StdSwitch2(_sts, "ARLK/TNL FAN A"));
		Add(pARLKTNLFanB		= new StdSwitch2(_sts, "ARLK/TNL FAN B"));
		Add(pLightsCLVestPORT	= new StdSwitch2(_sts, "LIGHTS C/L VESTIBULE PORT"));
		Add(pLightsCLVestSTBD	= new StdSwitch2(_sts, "LIGHTS C/L VESTIBULE STBD"));


		Add(pControlPanelPowerA = new StdSwitch2(_sts, "CNTL PNL PWR A"));
		Add(pControlPanelPowerB = new StdSwitch2(_sts, "CNTL PNL PWR B"));
		Add(pControlPanelPowerC = new StdSwitch2(_sts, "CNTL PNL PWR C"));
		Add(pHeatersDCUPowerH1 = new StdSwitch2(_sts, "HTRS/DCU PWR H1"));
		Add(pHeatersDCUPowerH2 = new StdSwitch2(_sts, "HTRS/DCU PWR H2/DCU"));
		Add(pHeatersDCUPowerH3 = new StdSwitch2(_sts, "HTRS/DCU PWR H3/DCU"));
		Add(pAPDSPowerA = new StdSwitch2(_sts, "APDS PWR A"));
		Add(pAPDSPowerB = new StdSwitch2(_sts, "APDS PWR B"));
		Add(pAPDSPowerC = new StdSwitch2(_sts, "APDS PWR C"));
		
		
		Add(pPyrosAp			= new StdSwitch2(_sts, "PYROS Ap"));
		Add(pPyrosBp			= new StdSwitch2(_sts, "PYROS Bp"));
		Add(pPyrosCp			= new StdSwitch2(_sts, "PYROS Cp"));

		Add(pLampTest			= new PushButton(_sts, "LAMP TEST"));
		Add(pPowerOn			= new PushButton(_sts, "POWER ON"));
		Add(pPowerOff			= new PushButton(_sts, "POWER OFF"));
		Add(pRingOut			= new PushButton(_sts, "RING OUT"));
		Add(pRingIn				= new PushButton(_sts, "RING IN"));
		Add(pAPDSCircProtOff	= new PushButton(_sts, "APDS CIRC PROT OFF"));
		Add(pCloseHooks			= new PushButton(_sts, "CLOSE HOOKS"));
		Add(pCloseLatches		= new PushButton(_sts, "CLOSE LATCHES"));
		Add(pFixerOff			= new PushButton(_sts, "FIXER OFF"));


		pControlPanelPowerA->SetLabel(0, "OFF");
		pControlPanelPowerA->SetLabel(1, "ON");
		
		pControlPanelPowerB->SetLabel(0, "OFF");
		pControlPanelPowerB->SetLabel(1, "ON");

		pControlPanelPowerC->SetLabel(0, "OFF");
		pControlPanelPowerC->SetLabel(1, "ON");


		pHeatersDCUPowerH1->SetLabel(0, "OFF");
		pHeatersDCUPowerH1->SetLabel(1, "ON");
		pHeatersDCUPowerH2->SetLabel(0, "OFF");
		pHeatersDCUPowerH2->SetLabel(1, "ON");
		pHeatersDCUPowerH3->SetLabel(0, "OFF");
		pHeatersDCUPowerH3->SetLabel(1, "ON");

		pAPDSPowerA->SetLabel(0, "OFF");
		pAPDSPowerA->SetLabel(1, "ON");
		pAPDSPowerB->SetLabel(0, "OFF");
		pAPDSPowerB->SetLabel(1, "ON");
		pAPDSPowerC->SetLabel(0, "OFF");
		pAPDSPowerC->SetLabel(1, "ON");

		
	}

	PanelA7A8ODS::~PanelA7A8ODS()
	{
	}

	void PanelA7A8ODS::AddMeshes(const VECTOR3& ofs)
	{
		SetHasOwnVCMesh();
		
		midx_odspanel = STS()->AddMesh(mesh_odspanel, &ofs);
		STS()->SetMeshVisibilityMode(midx_odspanel, MESHVIS_VC);
	}

	void PanelA7A8ODS::SetMeshVisibility(bool visible)
	{
		if(visible) STS()->SetMeshVisibilityMode(midx_odspanel, MESHVIS_VC);
		else STS()->SetMeshVisibilityMode(midx_odspanel, MESHVIS_NEVER);
	}

	UINT PanelA7A8ODS::GetVCMeshIndex() const
	{
		return midx_odspanel;
	}

	void PanelA7A8ODS::Realize()
	{	
		//Define switch connections

		oapiWriteLog("Panel A7/A8:\tRealize()");

		DiscreteBundle* pInternalA7 = 
			STS()->BundleManager()->CreateBundle("PANELA7", 16);

		if(!pInternalA7)
		{
			oapiWriteLog("Panel A7/A8:\tError creating internal cables.");
			exit(0);
		}

		DiscreteBundle* pBundle = 
			STS()->BundleManager()->CreateBundle("DSCU_TO_PANELA8A3_A", 16);

		plPowerOn->input.Connect(pBundle, 0);
		plPowerOn->test.Connect(pInternalA7, 0);

		plAPDSProtectCircOff->input.Connect(pBundle, 1);
		plAPDSProtectCircOff->test.Connect(pInternalA7, 0);

		plRingAligned->input.Connect(pBundle, 2);
		plRingAligned->test.Connect(pInternalA7, 0);

		plRingInitialPos->input.Connect(pBundle, 3);
		plRingInitialPos->test.Connect(pInternalA7, 0);

		plFixersOff->input.Connect(pBundle, 4);
		plFixersOff->test.Connect(pInternalA7, 0);

		plHooks1Open->input.Connect(pBundle, 5);
		plHooks1Open->test.Connect(pInternalA7, 0);

		plHooks2Open->input.Connect(pBundle, 6);
		plHooks2Open->test.Connect(pInternalA7, 0);

		plLatchesClosed->input.Connect(pBundle, 7);
		plLatchesClosed->test.Connect(pInternalA7, 0);

		plUndockComplete->input.Connect(pBundle, 8);
		plUndockComplete->test.Connect(pInternalA7, 0);


		plADS->input.Connect(pBundle, 9);
		plADS->test.Connect(pInternalA7, 0);

		plBDS->input.Connect(pBundle, 10);
		plBDS->test.Connect(pInternalA7, 0);

		plCDS->input.Connect(pBundle, 11);
		plCDS->test.Connect(pInternalA7, 0);

		pBundle = 
			STS()->BundleManager()->CreateBundle("DSCU_TO_PANELA8A3_B", 16);

		plInitialContact->input.Connect(pBundle, 0);
		plInitialContact->test.Connect(pInternalA7, 0);

		plCapture->input.Connect(pBundle, 1);
		plCapture->test.Connect(pInternalA7, 0);

		plRingForward->input.Connect(pBundle, 2);
		plRingForward->test.Connect(pInternalA7, 0);

		plReadyToHook->input.Connect(pBundle, 3);
		plReadyToHook->test.Connect(pInternalA7, 0);

		plInterpSealed->input.Connect(pBundle, 4);
		plInterpSealed->test.Connect(pInternalA7, 0);

		plHooks1Closed->input.Connect(pBundle, 5);
		plHooks1Closed->test.Connect(pInternalA7, 0);

		plHooks2Closed->input.Connect(pBundle, 6);
		plHooks2Closed->test.Connect(pInternalA7, 0);

		plLatchesOpen->input.Connect(pBundle, 7);
		plLatchesOpen->test.Connect(pInternalA7, 0);

		plRingFinal->input.Connect(pBundle, 8);
		plRingFinal->test.Connect(pInternalA7, 0);

		pLampTest->output.Connect(pInternalA7, 0);
		
		pBundle = STS()->BundleManager()->CreateBundle("PANELA8A3_TO_DSCU_A", 16);

		pPowerOn->output.Connect(pBundle, 0);
		pPowerOff->output.Connect(pBundle, 1);
		pRingOut->output.Connect(pBundle, 2);
		pRingIn->output.Connect(pBundle, 3);
		pAPDSCircProtOff->output.Connect(pBundle, 4);
		pCloseHooks->output.Connect(pBundle, 5);
		pCloseLatches->output.Connect(pBundle, 6);
		pFixerOff->output.Connect(pBundle, 7);
		

		pAPDSPowerA->output.Connect(pBundle, 9);		
		pAPDSPowerB->output.Connect(pBundle, 10);
		pAPDSPowerC->output.Connect(pBundle, 11);

		AtlantisPanel::Realize();

		oapiWriteLog("Panel A7/A8:\tRealize done.");
	}

	void PanelA7A8ODS::DefineVC()
	{
		oapiWriteLog("Panel A7/A8:\tDefine VC");
		//Define VC regions
		AddAIDToMouseEventList(AID_A7A3);
		AddAIDToMouseEventList(AID_A8A3);
		
		//first is A7A3, second region A8A3. 

		//Panel A8A3
		//-0.265916, 2.5048, 12.399
		//0.254716, 2.12746, 12.5175
		//register mouse event regions in BasicPanel

		//0.262413  2.443722  12.41595

		plADS->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plADS->SetSourceImage(g_Param.odslights);
		plADS->SetBase(17, 559);
		//plADS->SetBase(150, 150);
		plADS->SetSourceCoords(true, 17, 559);
		plADS->SetSourceCoords(false, 368, 559);
		plADS->SetDimensions(68, 43);

		plBDS->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plBDS->SetSourceImage(g_Param.odslights);
		plBDS->SetBase(93, 559);
		plBDS->SetSourceCoords(true, 93, 559);
		plBDS->SetSourceCoords(false, 445, 559);
		plBDS->SetDimensions(68, 43);

		plCDS->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plCDS->SetBase(172, 559);
		plCDS->SetSourceImage(g_Param.odslights);
		plCDS->SetSourceCoords(true, 172, 559);
		plCDS->SetSourceCoords(false, 524, 559);
		plCDS->SetDimensions(68, 43);



		plPowerOn->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plPowerOn->SetBase(17, 21);
		plPowerOn->SetSourceImage(g_Param.odslights);
		plPowerOn->SetSourceCoords(true, 17, 21);
		plPowerOn->SetSourceCoords(false, 367, 21);
		plPowerOn->SetDimensions(148, 46);

		plAPDSProtectCircOff->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plAPDSProtectCircOff->SetBase(17, 79);
		plAPDSProtectCircOff->SetSourceImage(g_Param.odslights);
		plAPDSProtectCircOff->SetSourceCoords(true, 17, 79);
		plAPDSProtectCircOff->SetSourceCoords(false, 367, 79);
		plAPDSProtectCircOff->SetDimensions(148, 46);

		//Y=138
		plRingAligned->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plRingAligned->SetBase(17, 138);
		plRingAligned->SetSourceImage(g_Param.odslights);
		plRingAligned->SetSourceCoords(true, 17, 138);
		plRingAligned->SetSourceCoords(false, 367, 138);
		plRingAligned->SetDimensions(148, 46);

		//plRingInitialPos
		plRingInitialPos->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plRingInitialPos->SetBase(17, 196);
		plRingInitialPos->SetSourceImage(g_Param.odslights);
		plRingInitialPos->SetSourceCoords(true, 17, 196);
		plRingInitialPos->SetSourceCoords(false, 367, 196);
		plRingInitialPos->SetDimensions(148, 46);

		plFixersOff->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plFixersOff->SetBase(17, 253);
		plFixersOff->SetSourceImage(g_Param.odslights);
		plFixersOff->SetSourceCoords(true, 17, 253);
		plFixersOff->SetSourceCoords(false, 367, 253);
		plFixersOff->SetDimensions(148, 46);

		plHooks1Open->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plHooks1Open->SetBase(17, 311);
		plHooks1Open->SetSourceImage(g_Param.odslights);
		plHooks1Open->SetSourceCoords(true, 17, 311);
		plHooks1Open->SetSourceCoords(false, 367, 311);
		plHooks1Open->SetDimensions(148, 46);

		plHooks2Open->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plHooks2Open->SetBase(17, 369);
		plHooks2Open->SetSourceImage(g_Param.odslights);
		plHooks2Open->SetSourceCoords(true, 17, 369);
		plHooks2Open->SetSourceCoords(false, 367, 369);
		plHooks2Open->SetDimensions(148, 46);

		plLatchesClosed->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plLatchesClosed->SetBase(17, 428);
		plLatchesClosed->SetSourceImage(g_Param.odslights);
		plLatchesClosed->SetSourceCoords(true, 17, 428);
		plLatchesClosed->SetSourceCoords(false, 367, 428);
		plLatchesClosed->SetDimensions(148, 46);

		plUndockComplete->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plUndockComplete->SetBase(17, 484);
		plUndockComplete->SetSourceImage(g_Param.odslights);
		plUndockComplete->SetSourceCoords(true, 17, 484);
		plUndockComplete->SetSourceCoords(false, 367, 484);
		plUndockComplete->SetDimensions(148, 46);

		//--------------------------------------------------
		// SECOND COLUMN OF LIGHTS
		//On: X=175
		//Off: X=527 
		//--------------------------------------------------

		plInitialContact->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plInitialContact->SetBase(175, 21);
		plInitialContact->SetSourceImage(g_Param.odslights);
		plInitialContact->SetSourceCoords(true, 175, 21);
		plInitialContact->SetSourceCoords(false, 527, 21);
		plInitialContact->SetDimensions(148, 46);

		plCapture->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plCapture->SetBase(175, 79);
		plCapture->SetSourceImage(g_Param.odslights);
		plCapture->SetSourceCoords(true, 175, 79);
		plCapture->SetSourceCoords(false, 527, 79);
		plCapture->SetDimensions(148, 46);

		//Y=138
		plRingForward->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plRingForward->SetBase(175, 138);
		plRingForward->SetSourceImage(g_Param.odslights);
		plRingForward->SetSourceCoords(true, 175, 138);
		plRingForward->SetSourceCoords(false, 527, 138);
		plRingForward->SetDimensions(148, 46);

		//plRingInitialPos
		plReadyToHook->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plReadyToHook->SetBase(175, 196);
		plReadyToHook->SetSourceImage(g_Param.odslights);
		plReadyToHook->SetSourceCoords(true, 175, 196);
		plReadyToHook->SetSourceCoords(false, 527, 196);
		plReadyToHook->SetDimensions(148, 46);

		plInterpSealed->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plInterpSealed->SetBase(175, 253);
		plInterpSealed->SetSourceImage(g_Param.odslights);
		plInterpSealed->SetSourceCoords(true, 175, 253);
		plInterpSealed->SetSourceCoords(false, 527, 253);
		plInterpSealed->SetDimensions(148, 46);

		plHooks1Closed->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plHooks1Closed->SetBase(175, 311);
		plHooks1Closed->SetSourceImage(g_Param.odslights);
		plHooks1Closed->SetSourceCoords(true, 175, 311);
		plHooks1Closed->SetSourceCoords(false, 527, 311);
		plHooks1Closed->SetDimensions(148, 46);

		plHooks2Closed->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plHooks2Closed->SetBase(175, 369);
		plHooks2Closed->SetSourceImage(g_Param.odslights);
		plHooks2Closed->SetSourceCoords(true, 175, 369);
		plHooks2Closed->SetSourceCoords(false, 527, 369);
		plHooks2Closed->SetDimensions(148, 46);

		plLatchesOpen->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plLatchesOpen->SetBase(175, 428);
		plLatchesOpen->SetSourceImage(g_Param.odslights);
		plLatchesOpen->SetSourceCoords(true, 175, 428);
		plLatchesOpen->SetSourceCoords(false, 527, 428);
		plLatchesOpen->SetDimensions(148, 46);

		plRingFinal->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plRingFinal->SetBase(175, 484);
		plRingFinal->SetSourceImage(g_Param.odslights);
		plRingFinal->SetSourceCoords(true, 175, 484);
		plRingFinal->SetSourceCoords(false, 527, 484);
		plRingFinal->SetDimensions(148, 46);

		pControlPanelPowerA->SetMouseRegion(0.081f, 0.124f, 0.094f, 0.197f);
		pControlPanelPowerA->SetReference(_V(0.0, 2.443722, 12.41595),
			_V(-1.0, 0.0, 0.0));
		pControlPanelPowerA->DefineSwitchGroup(GRP_A8A3S1_ODSVC);

		pControlPanelPowerB->SetMouseRegion(0.177f, 0.124f, 0.194f, 0.197f);
		pControlPanelPowerB->SetReference(_V(0.0, 2.443722, 12.41595), 
			_V(-1.0, 0.0, 0.0));
		pControlPanelPowerB->DefineSwitchGroup(GRP_A8A3S2_ODSVC);

		pControlPanelPowerC->SetMouseRegion(0.274f, 0.124f, 0.286f, 0.197f);
		pControlPanelPowerC->SetReference(_V(0.0, 2.443722, 12.41595), 
			_V(-1.0, 0.0, 0.0));
		pControlPanelPowerC->DefineSwitchGroup(GRP_A8A3S3_ODSVC);

		pHeatersDCUPowerH1->SetMouseRegion(0.076f, 0.337f, 0.095f, 0.379f);
		pHeatersDCUPowerH1->SetReference(_V(0.0, 2.370605, 12.43914), 
			_V(-1.0, 0.0, 0.0));
		pHeatersDCUPowerH1->DefineSwitchGroup(GRP_A8A3S4_ODSVC);

		pHeatersDCUPowerH2->SetMouseRegion(0.176f, 0.337f, 0.195f, 0.379f);
		pHeatersDCUPowerH2->SetReference(_V(0.0, 2.370605, 12.43914),
			_V(-1.0, 0.0, 0.0));
		pHeatersDCUPowerH2->DefineSwitchGroup(GRP_A8A3S5_ODSVC);

		pHeatersDCUPowerH3->SetMouseRegion(0.273f, 0.337f, 0.290f, 0.379f);
		pHeatersDCUPowerH3->SetReference(_V(0.0, 2.370605, 12.43914), 
			_V(-1.0, 0.0, 0.0));
		pHeatersDCUPowerH3->DefineSwitchGroup(GRP_A8A3S6_ODSVC);

		pAPDSPowerA->SetMouseRegion(0.076f, 0.519f, 0.095f, 0.590f);
		pAPDSPowerA->SetReference(_V(0.0,  2.297854, 12.46215), _V(-1.0, 0.0, 0.0));
		pAPDSPowerA->DefineSwitchGroup(GRP_A8A3S7_ODSVC);

		pAPDSPowerB->SetMouseRegion(0.176f, 0.519f, 0.195f, 0.590f);
		pAPDSPowerB->SetReference(_V(0.0,  2.297854, 12.46215), _V(-1.0, 0.0, 0.0));
		pAPDSPowerB->DefineSwitchGroup(GRP_A8A3S8_ODSVC);

		pAPDSPowerC->SetMouseRegion(0.273f, 0.519f, 0.290f, 0.590f);
		pAPDSPowerC->SetReference(_V(0.0,  2.297854, 12.46215), _V(-1.0, 0.0, 0.0));
		pAPDSPowerC->DefineSwitchGroup(GRP_A8A3S9_ODSVC);
		
		
		pPyrosAp->SetMouseRegion(0.709f, 0.124f, 0.725f, 0.197f);
		pPyrosAp->SetReference(_V(0.0, 2.443722, 12.41595),
			_V(-1.0, 0.0, 0.0));
		pPyrosAp->DefineSwitchGroup(GRP_A8A3S10_ODSVC);

		pPyrosBp->SetMouseRegion(0.803f, 0.124f, 0.825f, 0.197f);
		pPyrosBp->SetReference(_V(0.0, 2.443722, 12.41595), 
			_V(-1.0, 0.0, 0.0));
		pPyrosBp->DefineSwitchGroup(GRP_A8A3S11_ODSVC);

		pPyrosCp->SetMouseRegion(0.902f, 0.124f, 0.920f, 0.197f);
		pPyrosCp->SetReference(_V(0.0, 2.443722, 12.41595),
			_V(-1.0, 0.0, 0.0));
		pPyrosCp->DefineSwitchGroup(GRP_A8A3S12_ODSVC);

		const VECTOR3 PB_VECTOR = _V(0.0, -9.21E-04, -3.990E-03);

		pLampTest->SetMouseRegion(0.143f, 0.792f, 0.225f, 0.853f);
		pLampTest->DefineGroup(GRP_LampTest_ODSVC);
		pLampTest->SetDirection(PB_VECTOR);

		pPowerOn->SetMouseRegion(0.625f, 0.787f, 0.708f, 0.851f);
		pPowerOn->DefineGroup(GRP_APDSCC_PowerOn_ODSVC);
		pPowerOn->SetDirection(PB_VECTOR);

		pPowerOff->SetMouseRegion(0.708f, 0.787f, 0.7875f, 0.851f);
		pPowerOff->DefineGroup(GRP_APDSCC_PowerOff_ODSVC);
		pPowerOff->SetDirection(PB_VECTOR);

		pRingOut->SetMouseRegion(0.7875f, 0.787f, 0.8674f, 0.851f);
		pRingOut->DefineGroup(GRP_APDSCC_RingOut_ODSVC);
		pRingOut->SetDirection(PB_VECTOR);

		pRingIn->SetMouseRegion(0.8674f, 0.787f, 0.949f, 0.851f);
		pRingIn->DefineGroup(GRP_APDSCC_RingIn_ODSVC);
		pRingIn->SetDirection(PB_VECTOR);
	
		pAPDSCircProtOff->SetMouseRegion(0.625f, 0.903f, 0.708f, 0.962f);
		pAPDSCircProtOff->DefineGroup(GRP_APDSCC_APDS_Circ_Prot_OFF_ODSVC);
		pAPDSCircProtOff->SetDirection(PB_VECTOR);
		
		pCloseHooks->SetMouseRegion(0.708f, 0.903f, 0.7875f, 0.962f);
		pCloseHooks->DefineGroup(GRP_APDSCC_CloseHooks_ODSVC);
		pCloseHooks->SetDirection(PB_VECTOR);

		pCloseLatches->SetMouseRegion(0.7875f, 0.903f, 0.8674f, 0.962f);
		pCloseLatches->DefineGroup(GRP_APDSCC_CloseLatches_ODSVC);
		pCloseLatches->SetDirection(PB_VECTOR);

		pFixerOff->SetMouseRegion(0.8674f, 0.903f, 0.949f, 0.962f);
		pFixerOff->DefineGroup(GRP_APDSCC_FixerOff_ODSVC);
		pFixerOff->SetDirection(PB_VECTOR);
	}

	void PanelA7A8ODS::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		oapiVCRegisterArea(AID_A8A3, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP);
		oapiVCSetAreaClickmode_Quadrilateral (AID_A8A3, 
			_V(0.254716, 2.5048, 12.399)+ofs, _V(-0.265916, 2.5048, 12.399)+ofs,
			_V(0.254716, 2.12746, 12.5175)+ofs, _V(-0.265916, 2.12746, 12.5175) + ofs);


		SURFHANDLE shODSButtons = oapiGetTextureHandle(mesh_odspanel, 2);

		oapiVCRegisterArea(AID_ODSLIGHTS, 
			_R(0, 0, 334, 609), 
			PANEL_REDRAW_USER, 
			PANEL_MOUSE_IGNORE, 
			PANEL_MAP_BACKGROUND, 
			shODSButtons);

	}

	void PanelA7A8ODS::DefineVCAnimations(UINT vc_idx)
	{
		oapiWriteLog("PANEL A7/A8:\tDefine VC Animations");
		AtlantisPanel::DefineVCAnimations(midx_odspanel);
	}
		
};
