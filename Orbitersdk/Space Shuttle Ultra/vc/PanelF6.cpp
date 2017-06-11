#include "PanelF6.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "../Atlantis_defs.h"


namespace vc {

	PanelF6::PanelF6(Atlantis* _sts)
		: AtlantisPanel(_sts, "F6")
	{
		Add(pCDR1 = new MDU(_sts, "CDR1", MDUID_CDR1, true));
		Add(pCDR2 = new MDU(_sts, "CDR2", MDUID_CDR2, true));
		Add(pFltCntlrPower = new StdSwitch2(_sts, "Cdr Flt Cntlr Pwr"));
		Add( pADIAttitude = new StdSwitch3( _sts, "ADI Attitude" ) );
		Add( pADIError = new StdSwitch3( _sts, "ADI Error" ) );
		Add( pADIRate = new StdSwitch3( _sts, "ADI Rate" ) );

		pFltCntlrPower->SetLabel(0, "OFF");
		pFltCntlrPower->SetLabel(1, "ON");
		pADIAttitude->SetLabel( 0, "REF" );
		pADIAttitude->SetLabel( 1, "LVLH" );
		pADIAttitude->SetLabel( 2, "INRTL" );
		pADIError->SetLabel( 0, "LOW" );
		pADIError->SetLabel( 1, "MED" );
		pADIError->SetLabel( 2, "HIGH" );
		pADIRate->SetLabel( 0, "LOW" );
		pADIRate->SetLabel( 1, "MED" );
		pADIRate->SetLabel( 2, "HIGH" );

		Add( pLandingGearTB[0] = new StandardTalkback3( _sts, "Landing Gear Nose" ) );
		Add( pLandingGearTB[1] = new StandardTalkback3( _sts, "Landing Gear Left" ) );
		Add( pLandingGearTB[2] = new StandardTalkback3( _sts, "Landing Gear Right" ) );
		
		Add( pLandingGearArmDeployCover[0] = new StandardSwitchCover( _sts, "Landing Gear ARM Cover" ) );
		Add( pLandingGearArmDeployCover[1] = new StandardSwitchCover( _sts, "Landing Gear DN Cover" ) );
		
		Add( pLandingGearArmDeploy[0] = new PushButtonIndicatorSingleLight( _sts, "Landing Gear ARM" ) );
		Add( pLandingGearArmDeploy[1] = new PushButtonIndicatorSingleLight( _sts, "Landing Gear DN" ) );

		Add( pRCSCommand[0] = new StandardDoubleLight( _sts, "RCS COMMAND ROLL" ) );
		Add( pRCSCommand[1] = new StandardDoubleLight( _sts, "RCS COMMAND PITCH" ) );
		Add( pRCSCommand[2] = new StandardDoubleLight( _sts, "RCS COMMAND YAW" ) );
		
		Add( pRangeSafeArm = new StandardDoubleLight( _sts, "RANGE SAFE ARM" ) );// using double light because of 4 inputs

		Add( pAbort = new PushButtonIndicatorSingleLight( _sts, "ABORT" ) );

		Add( pHUDMode = new StdSwitch3( _sts, "HUD Mode" ) );
		Add( pHUDBrightness = new RotaryDemuxSwitch( _sts, "HUD Brightness", 5 ) );
		Add( pHUDBright = new StdSwitch3( _sts, "HUD Bright" ) );

		pHUDMode->SetLabel( 0, "DCLT" );
		pHUDMode->SetLabel( 1, "NORM" );
		pHUDMode->SetLabel( 2, "TEST" );
		pHUDMode->SetSpringLoaded( true, 0 );

		pHUDBright->SetLabel( 0, "MAN NIGHT" );
		pHUDBright->SetLabel( 1, "AUTO" );
		pHUDBright->SetLabel( 2, "MAN DAY" );
	}

	PanelF6::~PanelF6()
	{
	}

	void PanelF6::DefineVC()
	{
		VECTOR3 switch_rot = _V(1, 0, 0);
		VECTOR3 push_dir = _V( 0.0, -0.258681, 0.965963 );

		AddAIDToMouseEventList( AID_F6 );
		AddAIDToMouseEventList( AID_F6_MDU );
		AddAIDToMouseEventList( AID_F6_HUD );

		pCDR1->SetMouseRegion( AID_F6_MDU, 0.0f, 0.0f, 0.490755f, 1.0f );
		pCDR2->SetMouseRegion( AID_F6_MDU, 0.514292f, 0.0f, 1.0f, 1.0f );
		
		pCDR1->DefineVCGroup( GRP_CDR1_VC );
		pCDR2->DefineVCGroup( GRP_CDR2_VC );

		pFltCntlrPower->DefineSwitchGroup(GRP_F6SWITCH_5_VC);
		pFltCntlrPower->SetInitialAnimState(0.5);
		pFltCntlrPower->SetReference(_V(-0.734, 1.99, 14.685), switch_rot);
		pFltCntlrPower->SetMouseRegion( AID_F6, 0.383447f, 0.622820f, 0.424394f, 0.795206f );

		pADIAttitude->DefineSwitchGroup( GRP_F6SWITCH_6_VC );
		pADIAttitude->SetInitialAnimState( 0.5 );
		pADIAttitude->SetReference( _V( 0.633, 2.045, 14.7004 ), switch_rot );
		pADIAttitude->SetMouseRegion( AID_F6, 0.576813f, 0.195323f, 0.617059f, 0.362889f );

		pADIError->DefineSwitchGroup( GRP_F6SWITCH_7_VC );
		pADIError->SetInitialAnimState( 0.5 );
		pADIError->SetReference( _V( 0.5921, 2.0469, 14.7009 ), switch_rot );
		pADIError->SetMouseRegion( AID_F6, 0.653861f, 0.186497f, 0.699455f, 0.366683f );

		pADIRate->DefineSwitchGroup( GRP_F6SWITCH_8_VC );
		pADIRate->SetInitialAnimState( 0.5 );
		pADIRate->SetReference( _V( 0.5627, 2.0463, 14.7008 ), switch_rot );
		pADIRate->SetMouseRegion( AID_F6, 0.709568f, 0.184330f, 0.752249f, 0.356840f );

		pLandingGearTB[0]->DefineMeshGroup( STS()->mesh_vc, GRP_F6TALKBACK2_VC );
		
		pLandingGearTB[1]->DefineMeshGroup( STS()->mesh_vc, GRP_F6TALKBACK1_VC );
		
		pLandingGearTB[2]->DefineMeshGroup( STS()->mesh_vc, GRP_F6TALKBACK3_VC );

		pLandingGearArmDeployCover[0]->SetMouseRegion( AID_F6, 0, 0.054526f, 0.528985f, 0.125257f, 0.754980f );
		pLandingGearArmDeployCover[0]->SetMouseRegion( AID_F6, 1, 0.054526f, 0.528985f, 0.070585f, 0.754980f );
		pLandingGearArmDeployCover[0]->SetReference( _V( -0.92, 1.99996, 14.68735 ), _V( 0, 0.965408, 0.260745 ) );
		pLandingGearArmDeployCover[0]->DefineCoverGroup( GRP_F6COVER1_VC );

		pLandingGearArmDeployCover[1]->SetMouseRegion( AID_F6, 0, 0.145214f, 0.527563f, 0.213629f, 0.753547f );
		pLandingGearArmDeployCover[1]->SetMouseRegion( AID_F6, 1, 0.145214f, 0.527563f, 0.159221f, 0.652332f );
		pLandingGearArmDeployCover[1]->SetReference( _V( -0.87370, 1.99996, 14.68735 ), _V( 0, 0.965408, 0.260745 ) );
		pLandingGearArmDeployCover[1]->DefineCoverGroup( GRP_F6COVER2_VC );

		pLandingGearArmDeploy[0]->SetStateOffset( 1, 0.0f, 0.488281f );
		pLandingGearArmDeploy[0]->SetDirection( push_dir );
		pLandingGearArmDeploy[0]->SetMouseRegion( AID_F6, 0.075215f, 0.560928f, 0.116564f, 0.730217f );
		pLandingGearArmDeploy[0]->DefineMeshGroup( STS()->mesh_vc, GRP_F6_A5_S1_VC );

		pLandingGearArmDeploy[1]->SetStateOffset( 1, 0.0f, 0.488281f );
		pLandingGearArmDeploy[1]->SetDirection( push_dir );
		pLandingGearArmDeploy[1]->SetMouseRegion( AID_F6, 0.162630f, 0.559619f, 0.204250f, 0.726978f );
		pLandingGearArmDeploy[1]->DefineMeshGroup( STS()->mesh_vc, GRP_F6_A5_S2_VC );

		pRCSCommand[0]->DefineMeshGroup( STS()->mesh_vc, GRP_F6_XDS1_R_VC );
		pRCSCommand[0]->SetStateOffset( 1, 0.0f, 0.126953f );// L
		pRCSCommand[0]->SetStateOffset( 2, 0.139648f, 0.126953f );// R
		pRCSCommand[0]->SetStateOffset( 3, 0.139648f, 0.0f );// LR

		pRCSCommand[1]->DefineMeshGroup( STS()->mesh_vc, GRP_F6_XDS1_P_VC );
		pRCSCommand[1]->SetStateOffset( 1, 0.0f, 0.126953f );// U
		pRCSCommand[1]->SetStateOffset( 2, 0.139648f, 0.126953f );// D
		pRCSCommand[1]->SetStateOffset( 3, 0.139648f, 0.0f );// UD

		pRCSCommand[2]->DefineMeshGroup( STS()->mesh_vc, GRP_F6_XDS1_Y_VC );
		pRCSCommand[2]->SetStateOffset( 1, 0.0f, 0.126953f );// L
		pRCSCommand[2]->SetStateOffset( 2, 0.139648f, 0.126953f );// R
		pRCSCommand[2]->SetStateOffset( 3, 0.139648f, 0.0f );// LR

		pRangeSafeArm->DefineMeshGroup( STS()->mesh_vc, GRP_F6_XDS3_VC );
		pRangeSafeArm->SetStateOffset( 1, 0.139648f, 0.0f );
		pRangeSafeArm->SetStateOffset( 2, 0.139648f, 0.0f );
		pRangeSafeArm->SetStateOffset( 3, 0.139648f, 0.0f );

		pAbort->SetStateOffset( 1, -0.139648f, 0.0f );
		pAbort->SetDirection( push_dir );
		pAbort->SetMouseRegion( AID_F6, 0.908141f, 0.214696f, 0.939683f, 0.348886f );
		pAbort->DefineMeshGroup( STS()->mesh_vc, GRP_F6_A8_S2_VC );

		pHUDMode->DefineSwitchGroup( GRP_F6HUDTEST_VC );
		pHUDMode->SetInitialAnimState( 0.5 );
		pHUDMode->SetReference( _V( 0.7052, 2.4685, 14.5712 ), _V( 1, 0, 0 ) );
		pHUDMode->SetMouseRegion( AID_F6_HUD, 0.046968f, 0.392141f, 0.177257f, 0.644709f );

		pHUDBrightness->DefineSwitchGroup( GRP_F6HUDDIM_VC );
		pHUDBrightness->SetInitialAnimState( 0.5 );
		pHUDBrightness->SetReference( _V( -0.6552, 2.4697, 14.5635 ), _V( 0, 0.275637, -0.961262 ) );
		pHUDBrightness->DefineRotationAngle( 180.0f );
		pHUDBrightness->SetOffset( -90.0f );
		pHUDBrightness->SetMouseRegion( AID_F6_HUD, 0.401231f, 0.353931f, 0.605132f, 0.718589f );

		pHUDBright->DefineSwitchGroup( GRP_F6HUDBRT_VC );
		pHUDBright->SetInitialAnimState( 0.5 );
		pHUDBright->SetReference( _V( 0.5978, 2.4679, 14.5712 ), _V( 1, 0, 0 ) );
		pHUDBright->SetMouseRegion( AID_F6_HUD, 0.814331f, 0.392763f, 0.946918f, 0.646512f );
		return;
	}

	void PanelF6::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		
		oapiVCRegisterArea( AID_F6, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_F6, 
			_V( -0.9453, 2.0815, 14.7095 ) + ofs, _V( -0.4221, 2.0815, 14.7095 ) + ofs,
			_V( -0.9453, 1.9531, 14.6751 ) + ofs, _V( -0.4221, 1.9531, 14.6751 ) + ofs );

		oapiVCRegisterArea( AID_F6_MDU, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_F6_MDU, 
			_V( -0.9395, 2.3122, 14.7566 ) + ofs, _V( -0.4259, 2.3122, 14.7566 ) + ofs,
			_V( -0.9395, 2.0986, 14.6968 ) + ofs, _V( -0.4259, 2.0986, 14.6968 ) + ofs );

		oapiVCRegisterArea( AID_F6_HUD, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_F6_HUD, 
			_V( -0.7256, 2.5085, 14.5803 ) + ofs, _V( -0.5853, 2.5085, 14.5803 ) + ofs,
			_V( -0.7256, 2.4326, 14.5593 ) + ofs, _V( -0.5853, 2.4326, 14.5593 ) + ofs );
		
		return;
	}

	void PanelF6::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle("Controllers", 16);
		pFltCntlrPower->ConnectPort(1, pBundle, 1);

		pBundle = STS()->BundleManager()->CreateBundle( "ADI_Switches_F6_F8", 12 );
		pADIAttitude->outputA.Connect( pBundle, 0 );// REF
		pADIAttitude->outputB.Connect( pBundle, 1 );// INRTL
		pADIError->outputA.Connect( pBundle, 2 );// LOW
		pADIError->outputB.Connect( pBundle, 3 );// HIGH
		pADIRate->outputA.Connect( pBundle, 4 );// LOW
		pADIRate->outputB.Connect( pBundle, 5 );// HIGH

		pBundle = STS()->BundleManager()->CreateBundle( "LANDING_GEAR", 16 );
		pLandingGearTB[0]->SetInput( 0, pBundle, 0, TB_UP );
		pLandingGearTB[0]->SetInput( 1, pBundle, 1, TB_DN );
		pLandingGearTB[1]->SetInput( 0, pBundle, 2, TB_UP );
		pLandingGearTB[1]->SetInput( 1, pBundle, 3, TB_DN );
		pLandingGearTB[2]->SetInput( 0, pBundle, 4, TB_UP );
		pLandingGearTB[2]->SetInput( 1, pBundle, 5, TB_DN );
		pLandingGearArmDeploy[0]->ConnectPushButton( pBundle, 6 );// arm pb
		pLandingGearArmDeploy[1]->ConnectPushButton( pBundle, 8 );// dn pb

		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_4", 16 );
		pAbort->ConnectLight( 0, pBundle, 13 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_5", 16 );
		pRangeSafeArm->ConnectLight( 0, pBundle, 5 );
		pRangeSafeArm->ConnectLight( 1, pBundle, 13 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_3", 16 );
		pRCSCommand[0]->ConnectLight( 0, pBundle, 1 );
		pRCSCommand[0]->ConnectLight( 2, pBundle, 5 );
		pRCSCommand[1]->ConnectLight( 0, pBundle, 9 );
		pRCSCommand[1]->ConnectLight( 2, pBundle, 13 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_4", 16 );
		pRCSCommand[2]->ConnectLight( 0, pBundle, 1 );
		pRCSCommand[2]->ConnectLight( 2, pBundle, 5 );
		pAbort->ConnectLight( 1, pBundle, 13 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_5", 16 );
		pRangeSafeArm->ConnectLight( 2, pBundle, 13 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_3", 16 );
		pRCSCommand[0]->ConnectLight( 1, pBundle, 1 );
		pRCSCommand[0]->ConnectLight( 3, pBundle, 5 );
		pRCSCommand[1]->ConnectLight( 1, pBundle, 9 );
		pRCSCommand[1]->ConnectLight( 3, pBundle, 13 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_4", 16 );
		pRCSCommand[2]->ConnectLight( 1, pBundle, 1 );
		pRCSCommand[2]->ConnectLight( 3, pBundle, 5 );
		pLandingGearArmDeploy[0]->ConnectLight( 0, pBundle, 13 );// arm light

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_5", 16 );
		pLandingGearArmDeploy[1]->ConnectLight( 0, pBundle, 1 );// dn light
		pRangeSafeArm->ConnectLight( 1, pBundle, 13 );

		pBundle = STS()->BundleManager()->CreateBundle( "HUD_CDR", 16 );
		pHUDMode->outputA.Connect( pBundle, 1 );// mode dclt cdr
		pHUDMode->outputB.Connect( pBundle, 2 );// mode test cdr
		pHUDBrightness->ConnectOutputSignal( 0, pBundle, 3 );// brightness lvl 1 cdr
		pHUDBrightness->ConnectOutputSignal( 1, pBundle, 4 );// brightness lvl 2 cdr
		pHUDBrightness->ConnectOutputSignal( 2, pBundle, 5 );// brightness lvl 3 cdr
		pHUDBrightness->ConnectOutputSignal( 3, pBundle, 6 );// brightness lvl 4 cdr
		pHUDBrightness->ConnectOutputSignal( 4, pBundle, 7 );// brightness lvl 5 cdr
		pHUDBright->outputA.Connect( pBundle, 8 );// bright man night cdr
		pHUDBright->outputB.Connect( pBundle, 9 );// bright man day cdr

		AtlantisPanel::Realize();
	}
		
};
