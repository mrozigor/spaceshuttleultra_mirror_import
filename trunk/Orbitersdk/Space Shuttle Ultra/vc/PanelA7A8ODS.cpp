#include "PanelA7A8ODS.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"
#include "DiscreteBundleManager.h"
#include "../meshres_odsvc.h"


namespace vc {

	using namespace discsignals;

	PanelA7A8ODS::PanelA7A8ODS(Atlantis* _sts)
		: AtlantisPanel(_sts, "A7A3/A8A3")
	{

		mesh_odspanel = oapiLoadMeshGlobal(ODSPANEL_MESHNAME);

		Add(plADS				= new StandardSingleLight(_sts, "ADS"));
		Add(plBDS				= new StandardSingleLight(_sts, "BDS"));
		Add(plCDS				= new StandardSingleLight(_sts, "CDS"));

		Add(plPowerOn			= new StandardSingleLight(_sts, "POWER ON(light)"));
		Add(plAPDSProtectCircOff= new StandardSingleLight(_sts, "APDS CIRCUIT PROTECT OFF(light)"));
		Add(plRingAligned		= new StandardSingleLight(_sts, "RING ALIGNED"));
		Add(plRingInitialPos	= new StandardSingleLight(_sts, "RING INITIAL POSITION"));
		Add(plFixersOff			= new StandardSingleLight(_sts, "FIXERS OFF"));
		Add(plHooks1Open		= new StandardSingleLight(_sts, "HOOKS 1 OPEN"));
		Add(plHooks2Open		= new StandardSingleLight(_sts, "HOOKS 2 OPEN"));
		Add(plLatchesClosed		= new StandardSingleLight(_sts, "LATCHES CLOSED"));
		Add(plUndockComplete	= new StandardSingleLight(_sts, "UNDOCK COMPLET"));

		Add(plInitialContact	= new StandardSingleLight(_sts, "INITIAL CONTACT"));
		Add(plCapture			= new StandardSingleLight(_sts, "CAPTURE CAPTURE"));
		Add(plRingForward		= new StandardSingleLight(_sts, "RING FORWARD"));
		Add(plReadyToHook		= new StandardSingleLight(_sts, "READY TO HOOK"));
		Add(plInterfSealed		= new StandardSingleLight(_sts, "INTERF SEALED"));
		Add(plHooks1Closed		= new StandardSingleLight(_sts, "HOOKS 1 CLOSED"));
		Add(plHooks2Closed		= new StandardSingleLight(_sts, "HOOKS 2 CLOSED"));
		Add(plLatchesOpen		= new StandardSingleLight(_sts, "LATCHES OPEN"));
		Add(plRingFinal			= new StandardSingleLight(_sts, "RING FINAL POSITION"));

		Add( plAp = new StandardSingleLight( _sts, "Ap" ) );
		Add( plBp = new StandardSingleLight( _sts, "Bp" ) );
		Add( plCp = new StandardSingleLight( _sts, "Cp" ) );

		Add( pPyroProtectCircuitOff = new StandardSingleLight( _sts, "Pyro Protect Circuit Off" ) );

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

		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "PANELA7_A", 16 );
		for (int i = 0; i < 16; i++) lightoutput[i].Connect( pBundle, i );
		plPowerOn->ConnectLight( 0, pBundle, 0 );
		plAPDSProtectCircOff->ConnectLight( 0, pBundle, 1 );
		plRingAligned->ConnectLight( 0, pBundle, 2 );
		plRingInitialPos->ConnectLight( 0, pBundle, 3 );
		plFixersOff->ConnectLight( 0, pBundle, 4 );
		plHooks1Open->ConnectLight( 0, pBundle, 5 );
		plHooks2Open->ConnectLight( 0, pBundle, 6 );
		plLatchesClosed->ConnectLight( 0, pBundle, 7 );
		plUndockComplete->ConnectLight( 0, pBundle, 8 );
		plInitialContact->ConnectLight( 0, pBundle, 9 );
		plCapture->ConnectLight( 0, pBundle, 10 );
		plRingForward->ConnectLight( 0, pBundle, 11 );
		plReadyToHook->ConnectLight( 0, pBundle, 12 );
		plInterfSealed->ConnectLight( 0, pBundle, 13 );
		plHooks1Closed->ConnectLight( 0, pBundle, 14 );
		plHooks2Closed->ConnectLight( 0, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "PANELA7_B", 16 );
		for (int i = 16; i < 19; i++) lightoutput[i].Connect( pBundle, i - 16 );
		lighttest.Connect( pBundle, 3 );
		plLatchesOpen->ConnectLight( 0, pBundle, 0 );
		plRingFinal->ConnectLight( 0, pBundle, 1 );
		pPyroProtectCircuitOff->ConnectLight( 0, pBundle, 2 );
		pLampTest->output.Connect( pBundle, 3 );

		pBundle = STS()->BundleManager()->CreateBundle("DSCU_TO_PANELA8A3_A", 16);
		for (int i = 0; i < 16; i++) lightcmd[i].Connect( pBundle, i );

		pBundle = STS()->BundleManager()->CreateBundle("DSCU_TO_PANELA8A3_B", 16);
		for (int i = 16; i < 19; i++) lightcmd[i].Connect( pBundle, i - 16 );
		plADS->ConnectLight( 0, pBundle, 3 );
		plBDS->ConnectLight( 0, pBundle, 4 );
		plCDS->ConnectLight( 0, pBundle, 5 );
		plAp->ConnectLight( 0, pBundle, 6 );
		plBp->ConnectLight( 0, pBundle, 7 );
		plCp->ConnectLight( 0, pBundle, 8 );
		
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
		pControlPanelPowerA->output.Connect( pBundle, 0 );
		pControlPanelPowerB->output.Connect( pBundle, 1 );
		pControlPanelPowerC->output.Connect( pBundle, 2 );
		pHeatersDCUPowerH1->output.Connect( pBundle, 3 );
		pHeatersDCUPowerH2DCU->output.Connect( pBundle, 4 );
		pHeatersDCUPowerH3DCU->output.Connect( pBundle, 5 );
		pAPDSPowerA->output.Connect( pBundle, 6 );
		pAPDSPowerB->output.Connect( pBundle, 7 );
		pAPDSPowerC->output.Connect( pBundle, 8 );
		pPyrosAp->output.Connect( pBundle, 9 );
		pPyrosBp->output.Connect( pBundle, 10 );
		pPyrosCp->output.Connect( pBundle, 11 );
		CNTL_PNL_A.Connect( pBundle, 0 );
		CNTL_PNL_V.Connect( pBundle, 2 );

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

		plADS->DefineMeshGroup( midx_odspanel, GRP_ADS_ODSVC );
		plADS->SetStateOffset( 1, -0.342773f, 0.0f );
		plADS->SetDefaultState( 1 );

		plBDS->DefineMeshGroup( midx_odspanel, GRP_BDS_ODSVC );
		plBDS->SetStateOffset( 1, -0.342773f, 0.0f );
		plBDS->SetDefaultState( 1 );

		plCDS->DefineMeshGroup( midx_odspanel, GRP_CDS_ODSVC );
		plCDS->SetStateOffset( 1, -0.342773f, 0.0f );
		plCDS->SetDefaultState( 1 );


		plPowerOn->DefineMeshGroup( midx_odspanel, GRP_POWER_ON_ODSVC );
		plPowerOn->SetStateOffset( 1, -0.342773f, 0.0f );
		plPowerOn->SetDefaultState( 1 );

		plAPDSProtectCircOff->DefineMeshGroup( midx_odspanel, GRP_APDS_CIRC_PROT_OFF_ODSVC );
		plAPDSProtectCircOff->SetStateOffset( 1, -0.342773f, 0.0f );
		plAPDSProtectCircOff->SetDefaultState( 1 );

		plRingAligned->DefineMeshGroup( midx_odspanel, GRP_RING_ALIGNED_ODSVC );
		plRingAligned->SetStateOffset( 1, -0.342773f, 0.0f );
		plRingAligned->SetDefaultState( 1 );

		plRingInitialPos->DefineMeshGroup( midx_odspanel, GRP_RING_INITIAL_POS_ODSVC );
		plRingInitialPos->SetStateOffset( 1, -0.342773f, 0.0f );
		plRingInitialPos->SetDefaultState( 1 );

		plFixersOff->DefineMeshGroup( midx_odspanel, GRP_FIXERS_OFF_ODSVC );
		plFixersOff->SetStateOffset( 1, -0.342773f, 0.0f );
		plFixersOff->SetDefaultState( 1 );

		plHooks1Open->DefineMeshGroup( midx_odspanel, GRP_HOOKS1_OPEN_ODSVC );
		plHooks1Open->SetStateOffset( 1, -0.342773f, 0.0f );
		plHooks1Open->SetDefaultState( 1 );

		plHooks2Open->DefineMeshGroup( midx_odspanel, GRP_HOOKS2_OPEN_ODSVC );
		plHooks2Open->SetStateOffset( 1, -0.342773f, 0.0f );
		plHooks2Open->SetDefaultState( 1 );

		plLatchesClosed->DefineMeshGroup( midx_odspanel, GRP_LATCHES_CLOSED_ODSVC );
		plLatchesClosed->SetStateOffset( 1, -0.342773f, 0.0f );
		plLatchesClosed->SetDefaultState( 1 );

		plUndockComplete->DefineMeshGroup( midx_odspanel, GRP_UNDOCK_COMPLET_ODSVC );
		plUndockComplete->SetStateOffset( 1, -0.342773f, 0.0f );
		plUndockComplete->SetDefaultState( 1 );

		plInitialContact->DefineMeshGroup( midx_odspanel, GRP_INITIAL_CONTACT_ODSVC );
		plInitialContact->SetStateOffset( 1, -0.342773f, 0.0f );
		plInitialContact->SetDefaultState( 1 );

		plCapture->DefineMeshGroup( midx_odspanel, GRP_CAPTURE_ODSVC );
		plCapture->SetStateOffset( 1, -0.342773f, 0.0f );
		plCapture->SetDefaultState( 1 );

		plRingForward->DefineMeshGroup( midx_odspanel, GRP_RING_FORWARD_POS_ODSVC );
		plRingForward->SetStateOffset( 1, -0.342773f, 0.0f );
		plRingForward->SetDefaultState( 1 );

		plReadyToHook->DefineMeshGroup( midx_odspanel, GRP_READY_TO_HOOK_ODSVC );
		plReadyToHook->SetStateOffset( 1, -0.342773f, 0.0f );
		plReadyToHook->SetDefaultState( 1 );

		plInterfSealed->DefineMeshGroup( midx_odspanel, GRP_INTERF_SEALED_ODSVC );
		plInterfSealed->SetStateOffset( 1, -0.342773f, 0.0f );
		plInterfSealed->SetDefaultState( 1 );

		plHooks1Closed->DefineMeshGroup( midx_odspanel, GRP_HOOKS1_CLOSED_ODSVC );
		plHooks1Closed->SetStateOffset( 1, -0.342773f, 0.0f );
		plHooks1Closed->SetDefaultState( 1 );

		plHooks2Closed->DefineMeshGroup( midx_odspanel, GRP_HOOKS2_CLOSED_ODSVC );
		plHooks2Closed->SetStateOffset( 1, -0.342773f, 0.0f );
		plHooks2Closed->SetDefaultState( 1 );

		plLatchesOpen->DefineMeshGroup( midx_odspanel, GRP_LATCHES_OPEN_ODSVC );
		plLatchesOpen->SetStateOffset( 1, -0.342773f, 0.0f );
		plLatchesOpen->SetDefaultState( 1 );

		plRingFinal->DefineMeshGroup( midx_odspanel, GRP_RING_FINAL_POS_ODSVC );
		plRingFinal->SetStateOffset( 1, -0.342773f, 0.0f );
		plRingFinal->SetDefaultState( 1 );

		plAp->DefineMeshGroup( midx_odspanel, GRP_AP_ODSVC );
		plAp->SetStateOffset( 1, 0.0f, 0.199219f );

		plBp->DefineMeshGroup( midx_odspanel, GRP_BP_ODSVC );
		plBp->SetStateOffset( 1, 0.0f, 0.199219f );

		plCp->DefineMeshGroup( midx_odspanel, GRP_CP_ODSVC );
		plCp->SetStateOffset( 1, 0.0f, 0.199219f );

		pPyroProtectCircuitOff->DefineMeshGroup( midx_odspanel, GRP_PYROPROTECTCIRCUITOFF_ODSVC );
		pPyroProtectCircuitOff->SetStateOffset( 1, 0.0f, 0.199219f );


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

		const VECTOR3 PB_VECTOR = _V( 0.0, -0.224951, -0.974370 );

		pLampTest->SetMouseRegion(0.143f, 0.792f, 0.225f, 0.853f);
		pLampTest->DefineGroup(GRP_LAMPTEST_ODSVC);
		pLampTest->SetDirection(PB_VECTOR);
		pLampTest->SetMotionLength( 0.004 );

		pPowerOn->SetMouseRegion(0.625f, 0.787f, 0.708f, 0.851f);
		pPowerOn->DefineGroup(GRP_APDSCC_POWERON_ODSVC);
		pPowerOn->SetDirection(PB_VECTOR);
		pPowerOn->SetMotionLength( 0.004 );

		pPowerOff->SetMouseRegion(0.708f, 0.787f, 0.7875f, 0.851f);
		pPowerOff->DefineGroup(GRP_APDSCC_POWEROFF_ODSVC);
		pPowerOff->SetDirection(PB_VECTOR);
		pPowerOff->SetMotionLength( 0.004 );

		pRingOut->SetMouseRegion(0.7875f, 0.787f, 0.8674f, 0.851f);
		pRingOut->DefineGroup(GRP_APDSCC_RINGOUT_ODSVC);
		pRingOut->SetDirection(PB_VECTOR);
		pRingOut->SetMotionLength( 0.004 );

		pRingIn->SetMouseRegion(0.8674f, 0.787f, 0.949f, 0.851f);
		pRingIn->DefineGroup(GRP_APDSCC_RINGIN_ODSVC);
		pRingIn->SetDirection(PB_VECTOR);
		pRingIn->SetMotionLength( 0.004 );
	
		pAPDSCircProtOff->SetMouseRegion(0.625f, 0.903f, 0.708f, 0.962f);
		pAPDSCircProtOff->DefineGroup(GRP_APDSCC_APDS_CIRC_PROT_OFF_ODSVC);
		pAPDSCircProtOff->SetDirection(PB_VECTOR);
		pAPDSCircProtOff->SetMotionLength( 0.004 );
		
		pCloseHooks->SetMouseRegion(0.708f, 0.903f, 0.7875f, 0.962f);
		pCloseHooks->DefineGroup(GRP_APDSCC_CLOSEHOOKS_ODSVC);
		pCloseHooks->SetDirection(PB_VECTOR);
		pCloseHooks->SetMotionLength( 0.004 );

		pCloseLatches->SetMouseRegion(0.7875f, 0.903f, 0.8674f, 0.962f);
		pCloseLatches->DefineGroup(GRP_APDSCC_CLOSELATCHES_ODSVC);
		pCloseLatches->SetDirection(PB_VECTOR);
		pCloseLatches->SetMotionLength( 0.004 );

		pFixerOff->SetMouseRegion(0.8674f, 0.903f, 0.949f, 0.962f);
		pFixerOff->DefineGroup(GRP_APDSCC_FIXEROFF_ODSVC);
		pFixerOff->SetDirection(PB_VECTOR);
		pFixerOff->SetMotionLength( 0.004 );

		pPyroCommands->SetMouseRegion( 0, 0.716620f, 0.422147f, 0.911504f, 0.555984f );
		pPyroCommands->SetMouseRegion( 1, 0.921100f, 0.432651f, 0.951956f, 0.548972f );
		pPyroCommands->SetReference( _V( -0.2320, 2.3437, 12.4556 ), _V( 0.0, -0.9499, 0.3126 ) );
		pPyroCommands->DefineCoverGroup( GRP_PLATEGUARD1_ODSVC );

		pAPDSControlCommands->SetMouseRegion( 0, 0.716260f, 0.572989f, 0.901355f, 0.712548f );
		pAPDSControlCommands->SetMouseRegion( 1, 0.924516f, 0.581548f, 0.950181f, 0.694610f );
		pAPDSControlCommands->SetReference( _V( -0.2320, 2.3437, 12.4556 ), _V( 0.0, -0.9499, 0.3126 ) );
		pAPDSControlCommands->DefineCoverGroup( GRP_PLATEGUARD2_ODSVC );

		pPyroCircProtOff->SetMouseRegion( 0.753635f, 0.449135f, 0.806399f, 0.484442f );
		pPyroCircProtOff->DefineGroup( GRP_PYRO_CIRC_PROT_OFF_ODSVC );
		pPyroCircProtOff->SetDirection( PB_VECTOR );
		pPyroCircProtOff->SetMotionLength( 0.004 );

		pPyroCircProtOn->SetMouseRegion( 0.816807f, 0.448902f, 0.868185f, 0.483662f );
		pPyroCircProtOn->DefineGroup( GRP_PYRO_CIRC_PROT_ON_ODSVC );
		pPyroCircProtOn->SetDirection( PB_VECTOR );
		pPyroCircProtOn->SetMotionLength( 0.004 );

		pActHooksFiring->SetMouseRegion( 0.753999f, 0.497091f, 0.805913f, 0.530741f );
		pActHooksFiring->DefineGroup( GRP_ACT_HOOKS_FIRING_ODSVC );
		pActHooksFiring->SetDirection( PB_VECTOR );
		pActHooksFiring->SetMotionLength( 0.004 );

		pPasHooksFiring->SetMouseRegion( 0.816745f, 0.497108f, 0.868219f, 0.530819f );
		pPasHooksFiring->DefineGroup( GRP_PAS_HOOKS_FIRING_ODSVC );
		pPasHooksFiring->SetDirection( PB_VECTOR );
		pPasHooksFiring->SetMotionLength( 0.004 );

		pOpenHooks->SetMouseRegion( 0.754024f, 0.600301f, 0.806527f, 0.634483f );
		pOpenHooks->DefineGroup( GRP_OPEN_HOOKS_ODSVC );
		pOpenHooks->SetDirection( PB_VECTOR );
		pOpenHooks->SetMotionLength( 0.004 );

		pOpenLatches->SetMouseRegion( 0.816430f, 0.600107f, 0.868482f, 0.634369f );
		pOpenLatches->DefineGroup( GRP_OPEN_LATCHES_ODSVC );
		pOpenLatches->SetDirection( PB_VECTOR );
		pOpenLatches->SetMotionLength( 0.004 );

		pUndocking->SetMouseRegion( 0.815996f, 0.647695f, 0.868637f, 0.683079f );
		pUndocking->DefineGroup( GRP_UNDOCKING_ODSVC );
		pUndocking->SetDirection( PB_VECTOR );
		pUndocking->SetMotionLength( 0.004 );
	}

	void PanelA7A8ODS::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		oapiVCRegisterArea(AID_A8A3, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP);
		oapiVCSetAreaClickmode_Quadrilateral (AID_A8A3, 
			_V(0.254716, 2.5048, 12.399)+ofs, _V(-0.265916, 2.5048, 12.399)+ofs,
			_V(0.254716, 2.12746, 12.5175)+ofs, _V(-0.265916, 2.12746, 12.5175) + ofs);

		return;
	}

	void PanelA7A8ODS::DefineVCAnimations(UINT vc_idx)
	{
		oapiWriteLog("PANEL A7/A8:\tDefine VC Animations");
		AtlantisPanel::DefineVCAnimations(midx_odspanel);
	}
	
	void PanelA7A8ODS::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		AtlantisPanel::OnPostStep( SimT, DeltaT, MJD );

		if (lighttest.IsSet() && (CNTL_PNL_A.IsSet() || CNTL_PNL_V.IsSet()))
		{
			for (int i = 0; i < 19; i++) lightoutput[i].SetLine();
		}
		else
		{
			for (int i = 0; i < 19; i++) lightoutput[i].SetLine( (int)lightcmd[i].IsSet() * 5.0f );
		}
		return;
	}
};
