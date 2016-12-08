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
		Add(plInterfSealed		= new StandardLight(_sts, "INTERF SEALED"));
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
		Add(pHeatersDCUPowerH2DCU = new StdSwitch2(_sts, "HTRS/DCU PWR H2/DCU"));
		Add(pHeatersDCUPowerH3DCU = new StdSwitch2(_sts, "HTRS/DCU PWR H3/DCU"));
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
		pHeatersDCUPowerH2DCU->SetLabel(0, "OFF");
		pHeatersDCUPowerH2DCU->SetLabel(1, "ON");
		pHeatersDCUPowerH3DCU->SetLabel(0, "OFF");
		pHeatersDCUPowerH3DCU->SetLabel(1, "ON");

		pAPDSPowerA->SetLabel(0, "OFF");
		pAPDSPowerA->SetLabel(1, "ON");
		pAPDSPowerB->SetLabel(0, "OFF");
		pAPDSPowerB->SetLabel(1, "ON");
		pAPDSPowerC->SetLabel(0, "OFF");
		pAPDSPowerC->SetLabel(1, "ON");

		pPyrosAp->SetLabel( 0, "OFF" );
		pPyrosAp->SetLabel( 1, "ON" );
		pPyrosBp->SetLabel( 0, "OFF" );
		pPyrosBp->SetLabel( 1, "ON" );
		pPyrosCp->SetLabel( 0, "OFF" );
		pPyrosCp->SetLabel( 1, "ON" );

		Add( pPyroCommands = new StandardSwitchCover( _sts, "Pyro Commands" ) );
		Add( pAPDSControlCommands = new StandardSwitchCover( _sts, "APDS Control Commands" ) );

		Add( pPyroCircProtOff = new PushButton( _sts, "Pyro Circ Prot Off" ) );
		Add( pPyroCircProtOn = new PushButton( _sts, "Pyro Circ Prot On" ) );
		Add( pActHooksFiring = new PushButton( _sts, "Act Hooks Firing" ) );
		Add( pPasHooksFiring = new PushButton( _sts, "Pas Hooks Firing" ) );
		Add( pOpenHooks = new PushButton( _sts, "Open Hooks" ) );
		Add( pOpenLatches = new PushButton( _sts, "Open Latches" ) );
		Add( pUndocking = new PushButton( _sts, "Undocking" ) );
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

		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle("DSCU_TO_PANELA8A3_A", 16);
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

		plInitialContact->input.Connect(pBundle, 9);
		plInitialContact->test.Connect(pInternalA7, 0);

		plCapture->input.Connect(pBundle, 10);
		plCapture->test.Connect(pInternalA7, 0);

		plRingForward->input.Connect(pBundle, 11);
		plRingForward->test.Connect(pInternalA7, 0);

		plReadyToHook->input.Connect(pBundle, 12);
		plReadyToHook->test.Connect(pInternalA7, 0);

		plInterfSealed->input.Connect(pBundle, 13);
		plInterfSealed->test.Connect(pInternalA7, 0);

		plHooks1Closed->input.Connect(pBundle, 14);
		plHooks1Closed->test.Connect(pInternalA7, 0);

		plHooks2Closed->input.Connect(pBundle, 15);
		plHooks2Closed->test.Connect(pInternalA7, 0);


		pBundle = STS()->BundleManager()->CreateBundle("DSCU_TO_PANELA8A3_B", 16);
		plLatchesOpen->input.Connect(pBundle, 0);
		plLatchesOpen->test.Connect(pInternalA7, 0);

		plRingFinal->input.Connect(pBundle, 1);
		plRingFinal->test.Connect(pInternalA7, 0);

		plADS->input.Connect(pBundle, 2);
		plADS->test.Connect(pInternalA7, 0);

		plBDS->input.Connect(pBundle, 3);
		plBDS->test.Connect(pInternalA7, 0);

		plCDS->input.Connect(pBundle, 4);
		plCDS->test.Connect(pInternalA7, 0);

		pLampTest->output.Connect(pInternalA7, 0);

		
		pBundle = STS()->BundleManager()->CreateBundle( "PANELA8A3_TO_DSCU_A", 16 );// PBs
		pPowerOn->output.Connect( pBundle, 0 );
		pPowerOff->output.Connect( pBundle, 1 );
		pRingOut->output.Connect( pBundle, 2 );
		pRingIn->output.Connect( pBundle, 3 );
		pAPDSCircProtOff->output.Connect( pBundle, 4 );
		pCloseHooks->output.Connect( pBundle, 5 );
		pCloseLatches->output.Connect( pBundle, 6 );
		pFixerOff->output.Connect( pBundle, 7 );
		pPyroCircProtOff->output.Connect( pBundle, 8 );
		pPyroCircProtOn->output.Connect( pBundle, 9 );
		pActHooksFiring->output.Connect( pBundle, 10 );
		pPasHooksFiring->output.Connect( pBundle, 11 );
		pOpenHooks->output.Connect( pBundle, 12 );
		pOpenLatches->output.Connect( pBundle, 13 );
		pUndocking->output.Connect( pBundle, 14 );

		pBundle = STS()->BundleManager()->CreateBundle( "PANELA8A3_TO_DSCU_B", 16 );// CBs
		pHeatersDCUPowerH1->output.Connect( pBundle, 0 );
		pHeatersDCUPowerH2DCU->output.Connect( pBundle, 1 );
		pHeatersDCUPowerH3DCU->output.Connect( pBundle, 2 );
		pAPDSPowerA->output.Connect( pBundle, 3 );
		pAPDSPowerB->output.Connect( pBundle, 4 );
		pAPDSPowerC->output.Connect( pBundle, 5 );
		pPyrosAp->output.Connect( pBundle, 6 );
		pPyrosBp->output.Connect( pBundle, 7 );
		pPyrosCp->output.Connect( pBundle, 8 );

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
		plADS->SetSourceCoords(true, 17, 559);
		plADS->SetSourceCoords(false, 368, 559);
		plADS->SetDimensions(68, 43);

		plBDS->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plBDS->SetSourceImage(g_Param.odslights);
		plBDS->SetBase(93, 559);
		plBDS->SetSourceCoords(true, 93, 559);
		plBDS->SetSourceCoords(false, 444, 559);
		plBDS->SetDimensions(68, 43);

		plCDS->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plCDS->SetBase(172, 559);
		plCDS->SetSourceImage(g_Param.odslights);
		plCDS->SetSourceCoords(true, 172, 559);
		plCDS->SetSourceCoords(false, 523, 559);
		plCDS->SetDimensions(68, 43);



		plPowerOn->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plPowerOn->SetBase(17, 21);
		plPowerOn->SetSourceImage(g_Param.odslights);
		plPowerOn->SetSourceCoords(true, 17, 21);
		plPowerOn->SetSourceCoords(false, 368, 21);
		plPowerOn->SetDimensions(148, 46);

		plAPDSProtectCircOff->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plAPDSProtectCircOff->SetBase(17, 79);
		plAPDSProtectCircOff->SetSourceImage(g_Param.odslights);
		plAPDSProtectCircOff->SetSourceCoords(true, 17, 79);
		plAPDSProtectCircOff->SetSourceCoords(false, 368, 79);
		plAPDSProtectCircOff->SetDimensions(148, 46);

		plRingAligned->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plRingAligned->SetBase(17, 138);
		plRingAligned->SetSourceImage(g_Param.odslights);
		plRingAligned->SetSourceCoords(true, 17, 138);
		plRingAligned->SetSourceCoords(false, 368, 138);
		plRingAligned->SetDimensions(148, 46);

		plRingInitialPos->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plRingInitialPos->SetBase(17, 196);
		plRingInitialPos->SetSourceImage(g_Param.odslights);
		plRingInitialPos->SetSourceCoords(true, 17, 196);
		plRingInitialPos->SetSourceCoords(false, 368, 196);
		plRingInitialPos->SetDimensions(148, 46);

		plFixersOff->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plFixersOff->SetBase(17, 253);
		plFixersOff->SetSourceImage(g_Param.odslights);
		plFixersOff->SetSourceCoords(true, 17, 253);
		plFixersOff->SetSourceCoords(false, 368, 253);
		plFixersOff->SetDimensions(148, 46);

		plHooks1Open->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plHooks1Open->SetBase(17, 311);
		plHooks1Open->SetSourceImage(g_Param.odslights);
		plHooks1Open->SetSourceCoords(true, 17, 311);
		plHooks1Open->SetSourceCoords(false, 368, 311);
		plHooks1Open->SetDimensions(148, 46);

		plHooks2Open->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plHooks2Open->SetBase(17, 369);
		plHooks2Open->SetSourceImage(g_Param.odslights);
		plHooks2Open->SetSourceCoords(true, 17, 369);
		plHooks2Open->SetSourceCoords(false, 368, 369);
		plHooks2Open->SetDimensions(148, 46);

		plLatchesClosed->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plLatchesClosed->SetBase(17, 428);
		plLatchesClosed->SetSourceImage(g_Param.odslights);
		plLatchesClosed->SetSourceCoords(true, 17, 428);
		plLatchesClosed->SetSourceCoords(false, 368, 428);
		plLatchesClosed->SetDimensions(148, 46);

		plUndockComplete->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plUndockComplete->SetBase(17, 484);
		plUndockComplete->SetSourceImage(g_Param.odslights);
		plUndockComplete->SetSourceCoords(true, 17, 484);
		plUndockComplete->SetSourceCoords(false, 368, 484);
		plUndockComplete->SetDimensions(148, 46);

		//--------------------------------------------------
		// SECOND COLUMN OF LIGHTS
		//On: X=176
		//Off: X=527 
		//--------------------------------------------------
		plInitialContact->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plInitialContact->SetBase(176, 21);
		plInitialContact->SetSourceImage(g_Param.odslights);
		plInitialContact->SetSourceCoords(true, 176, 21);
		plInitialContact->SetSourceCoords(false, 527, 21);
		plInitialContact->SetDimensions(148, 46);

		plCapture->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plCapture->SetBase(176, 79);
		plCapture->SetSourceImage(g_Param.odslights);
		plCapture->SetSourceCoords(true, 176, 79);
		plCapture->SetSourceCoords(false, 527, 79);
		plCapture->SetDimensions(148, 46);

		plRingForward->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plRingForward->SetBase(176, 138);
		plRingForward->SetSourceImage(g_Param.odslights);
		plRingForward->SetSourceCoords(true, 176, 138);
		plRingForward->SetSourceCoords(false, 527, 138);
		plRingForward->SetDimensions(148, 46);

		plReadyToHook->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plReadyToHook->SetBase(176, 196);
		plReadyToHook->SetSourceImage(g_Param.odslights);
		plReadyToHook->SetSourceCoords(true, 176, 196);
		plReadyToHook->SetSourceCoords(false, 527, 196);
		plReadyToHook->SetDimensions(148, 46);

		plInterfSealed->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plInterfSealed->SetBase(176, 253);
		plInterfSealed->SetSourceImage(g_Param.odslights);
		plInterfSealed->SetSourceCoords(true, 176, 253);
		plInterfSealed->SetSourceCoords(false, 527, 253);
		plInterfSealed->SetDimensions(148, 46);

		plHooks1Closed->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plHooks1Closed->SetBase(176, 311);
		plHooks1Closed->SetSourceImage(g_Param.odslights);
		plHooks1Closed->SetSourceCoords(true, 176, 311);
		plHooks1Closed->SetSourceCoords(false, 527, 311);
		plHooks1Closed->SetDimensions(148, 46);

		plHooks2Closed->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plHooks2Closed->SetBase(176, 369);
		plHooks2Closed->SetSourceImage(g_Param.odslights);
		plHooks2Closed->SetSourceCoords(true, 176, 369);
		plHooks2Closed->SetSourceCoords(false, 527, 369);
		plHooks2Closed->SetDimensions(148, 46);

		plLatchesOpen->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plLatchesOpen->SetBase(176, 428);
		plLatchesOpen->SetSourceImage(g_Param.odslights);
		plLatchesOpen->SetSourceCoords(true, 176, 428);
		plLatchesOpen->SetSourceCoords(false, 527, 428);
		plLatchesOpen->SetDimensions(148, 46);

		plRingFinal->AddAIDToRedrawEventList(AID_ODSLIGHTS);
		plRingFinal->SetBase(176, 484);
		plRingFinal->SetSourceImage(g_Param.odslights);
		plRingFinal->SetSourceCoords(true, 176, 484);
		plRingFinal->SetSourceCoords(false, 527, 484);
		plRingFinal->SetDimensions(148, 46);

		pControlPanelPowerA->SetMouseRegion( 0.078607f, 0.121607f, 0.097529f, 0.202774f );
		pControlPanelPowerA->SetReference(_V(0.0, 2.443722, 12.41595), _V(-1.0, 0.0, 0.0));
		pControlPanelPowerA->DefineSwitchGroup(GRP_A8A3S1_ODSVC);

		pControlPanelPowerB->SetMouseRegion( 0.175489f, 0.123035f, 0.194648f, 0.204244f );
		pControlPanelPowerB->SetReference(_V(0.0, 2.443722, 12.41595), _V(-1.0, 0.0, 0.0));
		pControlPanelPowerB->DefineSwitchGroup(GRP_A8A3S2_ODSVC);

		pControlPanelPowerC->SetMouseRegion( 0.271936f, 0.122135f, 0.291739f, 0.203266f );
		pControlPanelPowerC->SetReference(_V(0.0, 2.443722, 12.41595), _V(-1.0, 0.0, 0.0));
		pControlPanelPowerC->DefineSwitchGroup(GRP_A8A3S3_ODSVC);

		pHeatersDCUPowerH1->SetMouseRegion( 0.078266f, 0.315846f, 0.097893f, 0.396873f );
		pHeatersDCUPowerH1->SetReference(_V(0.0, 2.370605, 12.43914), _V(-1.0, 0.0, 0.0));
		pHeatersDCUPowerH1->DefineSwitchGroup(GRP_A8A3S4_ODSVC);

		pHeatersDCUPowerH2DCU->SetMouseRegion( 0.175297f, 0.316582f, 0.194602f, 0.397552f );
		pHeatersDCUPowerH2DCU->SetReference(_V(0.0, 2.370605, 12.43914),_V(-1.0, 0.0, 0.0));
		pHeatersDCUPowerH2DCU->DefineSwitchGroup(GRP_A8A3S5_ODSVC);

		pHeatersDCUPowerH3DCU->SetMouseRegion( 0.271984f, 0.315340f, 0.291396f, 0.397430f );
		pHeatersDCUPowerH3DCU->SetReference(_V(0.0, 2.370605, 12.43914), _V(-1.0, 0.0, 0.0));
		pHeatersDCUPowerH3DCU->DefineSwitchGroup(GRP_A8A3S6_ODSVC);

		pAPDSPowerA->SetMouseRegion( 0.078492f, 0.510377f, 0.097095f, 0.591183f );
		pAPDSPowerA->SetReference(_V(0.0,  2.297854, 12.46215), _V(-1.0, 0.0, 0.0));
		pAPDSPowerA->DefineSwitchGroup(GRP_A8A3S7_ODSVC);

		pAPDSPowerB->SetMouseRegion( 0.175082f, 0.510769f, 0.194313f, 0.590632f );
		pAPDSPowerB->SetReference(_V(0.0,  2.297854, 12.46215), _V(-1.0, 0.0, 0.0));
		pAPDSPowerB->DefineSwitchGroup(GRP_A8A3S8_ODSVC);

		pAPDSPowerC->SetMouseRegion( 0.271967f, 0.509268f, 0.291367f, 0.591272f );
		pAPDSPowerC->SetReference(_V(0.0,  2.297854, 12.46215), _V(-1.0, 0.0, 0.0));
		pAPDSPowerC->DefineSwitchGroup(GRP_A8A3S9_ODSVC);
		
		pPyrosAp->SetMouseRegion( 0.706286f, 0.120643f, 0.725018f, 0.203573f );
		pPyrosAp->SetReference(_V(0.0, 2.443722, 12.41595), _V(-1.0, 0.0, 0.0));
		pPyrosAp->DefineSwitchGroup(GRP_A8A3S10_ODSVC);

		pPyrosBp->SetMouseRegion( 0.802748f, 0.123284f, 0.821752f, 0.204392f );
		pPyrosBp->SetReference(_V(0.0, 2.443722, 12.41595), _V(-1.0, 0.0, 0.0));
		pPyrosBp->DefineSwitchGroup(GRP_A8A3S11_ODSVC);

		pPyrosCp->SetMouseRegion( 0.899503f, 0.122141f, 0.918277f, 0.203744f );
		pPyrosCp->SetReference(_V(0.0, 2.443722, 12.41595), _V(-1.0, 0.0, 0.0));
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

		pPyroCommands->SetMouseRegion( 0, 0.716620f, 0.422147f, 0.911504f, 0.555984f );
		pPyroCommands->SetMouseRegion( 1, 0.921100f, 0.432651f, 0.951956f, 0.548972f );
		pPyroCommands->SetReference( _V( -0.2320, 2.3437, 12.4556 ), _V( 0.0, -0.9499, 0.3126 ) );
		pPyroCommands->DefineCoverGroup( GRP_plateguard1_ODSVC );

		pAPDSControlCommands->SetMouseRegion( 0, 0.716260f, 0.572989f, 0.901355f, 0.712548f );
		pAPDSControlCommands->SetMouseRegion( 1, 0.924516f, 0.581548f, 0.950181f, 0.694610f );
		pAPDSControlCommands->SetReference( _V( -0.2320, 2.3437, 12.4556 ), _V( 0.0, -0.9499, 0.3126 ) );
		pAPDSControlCommands->DefineCoverGroup( GRP_plateguard2_ODSVC );

		pPyroCircProtOff->SetMouseRegion( 0.753635f, 0.449135f, 0.806399f, 0.484442f );
		pPyroCircProtOff->DefineGroup( GRP_Pyro_Circ_Prot_OFF_ODSVC );
		pPyroCircProtOff->SetDirection( PB_VECTOR );

		pPyroCircProtOn->SetMouseRegion( 0.816807f, 0.448902f, 0.868185f, 0.483662f );
		pPyroCircProtOn->DefineGroup( GRP_Pyro_Circ_Prot_ON_ODSVC );
		pPyroCircProtOn->SetDirection( PB_VECTOR );

		pActHooksFiring->SetMouseRegion( 0.753999f, 0.497091f, 0.805913f, 0.530741f );
		pActHooksFiring->DefineGroup( GRP_Act_Hooks_Firing_ODSVC );
		pActHooksFiring->SetDirection( PB_VECTOR );

		pPasHooksFiring->SetMouseRegion( 0.816745f, 0.497108f, 0.868219f, 0.530819f );
		pPasHooksFiring->DefineGroup( GRP_Pas_Hooks_Firing_ODSVC );
		pPasHooksFiring->SetDirection( PB_VECTOR );

		pOpenHooks->SetMouseRegion( 0.754024f, 0.600301f, 0.806527f, 0.634483f );
		pOpenHooks->DefineGroup( GRP_OPEN_Hooks_ODSVC );
		pOpenHooks->SetDirection( PB_VECTOR );

		pOpenLatches->SetMouseRegion( 0.816430f, 0.600107f, 0.868482f, 0.634369f );
		pOpenLatches->DefineGroup( GRP_OPEN_Latches_ODSVC );
		pOpenLatches->SetDirection( PB_VECTOR );

		pUndocking->SetMouseRegion( 0.815996f, 0.647695f, 0.868637f, 0.683079f );
		pUndocking->DefineGroup( GRP_Undocking_ODSVC );
		pUndocking->SetDirection( PB_VECTOR );
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
