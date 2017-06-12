#include "PanelF8.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "../Atlantis_defs.h"


namespace vc {

	PanelF8::PanelF8(Atlantis* _sts)
		: AtlantisPanel(_sts, "F8")
	{
		Add(pPLT1 = new MDU(_sts, "PLT1", MDUID_PLT1, true));
		Add(pPLT2 = new MDU(_sts, "PLT2", MDUID_PLT2, true));
		Add(pFltCntlrPower = new StdSwitch2(_sts, "Plt Flt Cntlr Pwr"));
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

	PanelF8::~PanelF8()
	{
	}

	void PanelF8::DefineVC()
	{
		VECTOR3 switch_rot = _V(1, 0, 0);
		VECTOR3 push_dir = _V( 0.0, -0.258681, 0.965963 );

		AddAIDToMouseEventList( AID_F8 );
		AddAIDToMouseEventList( AID_F8_MDU );
		AddAIDToMouseEventList( AID_F8_HUD );

		pPLT1->SetMouseRegion( AID_F8_MDU, 0.0f, 0.0f, 0.489801f, 1.0f );
		pPLT2->SetMouseRegion( AID_F8_MDU, 0.509333f, 0.0f, 1.0f, 1.0f );
		
		pPLT1->DefineVCGroup( GRP_PLT1_VC );
		pPLT2->DefineVCGroup( GRP_PLT2_VC );

		pFltCntlrPower->DefineSwitchGroup(GRP_F8SWITCH5_VC);
		pFltCntlrPower->SetInitialAnimState(0.5);
		pFltCntlrPower->SetReference(_V(-0.734, 1.99, 14.685), switch_rot);
		pFltCntlrPower->SetMouseRegion( AID_F8, 0.379877f, 0.609586f, 0.422314f, 0.793425f );

		pADIAttitude->DefineSwitchGroup( GRP_F8SWITCH6_VC );
		pADIAttitude->SetInitialAnimState( 0.5 );
		pADIAttitude->SetReference( _V( -0.633, 2.045, 14.7004 ), switch_rot );
		pADIAttitude->SetMouseRegion( AID_F8, 0.573540f, 0.180482f, 0.616687f, 0.353228f );

		pADIError->DefineSwitchGroup( GRP_F8SWITCH7_VC );
		pADIError->SetInitialAnimState( 0.5 );
		pADIError->SetReference( _V( -0.5921, 2.0469, 14.7009 ), switch_rot );
		pADIError->SetMouseRegion( AID_F8, 0.652143f, 0.174853f, 0.699142f, 0.359075f );

		pADIRate->DefineSwitchGroup( GRP_F8SWITCH8_VC );
		pADIRate->SetInitialAnimState( 0.5 );
		pADIRate->SetReference( _V( -0.5627, 2.0463, 14.7008 ), switch_rot );
		pADIRate->SetMouseRegion( AID_F8, 0.708632f, 0.178725f, 0.750834f, 0.350277f );

		pLandingGearTB[0]->DefineMeshGroup( STS()->mesh_vc, GRP_F8TALKBACK2_VC );
		
		pLandingGearTB[1]->DefineMeshGroup( STS()->mesh_vc, GRP_F8TALKBACK1_VC );
		
		pLandingGearTB[2]->DefineMeshGroup( STS()->mesh_vc, GRP_F8TALKBACK3_VC );

		pLandingGearArmDeployCover[0]->SetMouseRegion( AID_F8, 0, 0.052979f, 0.513634f, 0.120368f, 0.740271f );
		pLandingGearArmDeployCover[0]->SetMouseRegion( AID_F8, 1, 0.049178f, 0.511376f, 0.065602f, 0.734235f );
		pLandingGearArmDeployCover[0]->SetReference( _V( 0.444, 1.99996, 14.68735 ), _V( 0, 0.965408, 0.260745 ) );
		pLandingGearArmDeployCover[0]->DefineCoverGroup( GRP_F8COVER1_VC );

		pLandingGearArmDeployCover[1]->SetMouseRegion( AID_F8, 0, 0.142077f, 0.513883f, 0.210356f, 0.740062f );
		pLandingGearArmDeployCover[1]->SetMouseRegion( AID_F8, 1, 0.139839f, 0.516023f, 0.154470f, 0.733397f );
		pLandingGearArmDeployCover[1]->SetReference( _V( 0.4913, 1.99996, 14.68735 ), _V( 0, 0.965408, 0.260745 ) );
		pLandingGearArmDeployCover[1]->DefineCoverGroup( GRP_F8COVER2_VC );

		pLandingGearArmDeploy[0]->SetStateOffset( 1, 0.0f, 0.488281f );
		pLandingGearArmDeploy[0]->SetDirection( push_dir );
		pLandingGearArmDeploy[0]->SetMouseRegion( AID_F8, 0.072426f, 0.551969f, 0.114127f, 0.720267f );
		pLandingGearArmDeploy[0]->DefineMeshGroup( STS()->mesh_vc, GRP_F8_S1_VC );

		pLandingGearArmDeploy[1]->SetStateOffset( 1, 0.0f, 0.488281f );
		pLandingGearArmDeploy[1]->SetDirection( push_dir );
		pLandingGearArmDeploy[1]->SetMouseRegion( AID_F8, 0.160673f, 0.552534f, 0.201274f, 0.716342f );
		pLandingGearArmDeploy[1]->DefineMeshGroup( STS()->mesh_vc, GRP_F8_S2_VC );

		pHUDMode->DefineSwitchGroup( GRP_F8HUDTEST_VC );
		pHUDMode->SetInitialAnimState( 0.5 );
		pHUDMode->SetReference( _V( -0.5996, 2.4685, 14.5712 ), _V( 1, 0, 0 ) );
		pHUDMode->SetMouseRegion( AID_F8_HUD, 0.042951f, 0.399852f, 0.177495f, 0.643278f );

		pHUDBrightness->DefineSwitchGroup( GRP_F8HUDDIM_VC );
		pHUDBrightness->SetInitialAnimState( 0.5 );
		pHUDBrightness->SetReference( _V( 0.6496, 2.4697, 14.5635 ), _V( 0, 0.275637, -0.961262 ) );
		pHUDBrightness->DefineRotationAngle( 180.0f );
		pHUDBrightness->SetOffset( -90.0f );
		pHUDBrightness->SetMouseRegion( AID_F8_HUD, 0.390848f, 0.343682f, 0.605650f, 0.723217f );

		pHUDBright->DefineSwitchGroup( GRP_F8HUDBRT_VC );
		pHUDBright->SetInitialAnimState( 0.5 );
		pHUDBright->SetReference( _V( -0.7070, 2.4680, 14.5712 ), _V( 1, 0, 0 ) );
		pHUDBright->SetMouseRegion( AID_F8_HUD, 0.810735f, 0.390512f, 0.943742f, 0.641940f );
	}

	void PanelF8::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		oapiVCRegisterArea( AID_F8, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_F8, 
			_V( 0.4187, 2.0814, 14.7091 ) + ofs, _V( 0.9412, 2.0814, 14.7091 ) + ofs, 
			_V( 0.4187, 1.9513, 14.6745 ) + ofs, _V( 0.9412, 1.9513, 14.6745 ) + ofs );

		oapiVCRegisterArea( AID_F8_MDU, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_F8_MDU, 
			_V( 0.4227, 2.3119, 14.7565 ) + ofs, _V( 0.9362, 2.3119, 14.7565 ) + ofs, 
			_V( 0.4227, 2.0984, 14.6968 ) + ofs, _V( 0.9362, 2.0984, 14.6968 ) + ofs );

		oapiVCRegisterArea( AID_F8_HUD, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_F8_HUD, 
			_V( 0.5799, 2.5083, 14.5803 ) + ofs, _V( 0.7200, 2.5083, 14.5803 ) + ofs, 
			_V( 0.5799, 2.4325, 14.5593 ) + ofs, _V( 0.7200, 2.4325, 14.5593 ) + ofs );

		return;
	}

	void PanelF8::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle("Controllers", 16);
		pFltCntlrPower->ConnectPort(1, pBundle, 2);

		pBundle = STS()->BundleManager()->CreateBundle( "ADI_Switches_F6_F8", 12 );
		pADIAttitude->outputA.Connect( pBundle, 6 );// REF
		pADIAttitude->outputB.Connect( pBundle, 7 );// INRTL
		pADIError->outputA.Connect( pBundle, 8 );// LOW
		pADIError->outputB.Connect( pBundle, 9 );// HIGH
		pADIRate->outputA.Connect( pBundle, 10 );// LOW
		pADIRate->outputB.Connect( pBundle, 11 );// HIGH

		pBundle = STS()->BundleManager()->CreateBundle( "LANDING_GEAR", 16 );
		pLandingGearTB[0]->SetInput( 0, pBundle, 0, TB_UP );
		pLandingGearTB[0]->SetInput( 1, pBundle, 1, TB_DN );
		pLandingGearTB[1]->SetInput( 0, pBundle, 2, TB_UP );
		pLandingGearTB[1]->SetInput( 1, pBundle, 3, TB_DN );
		pLandingGearTB[2]->SetInput( 0, pBundle, 4, TB_UP );
		pLandingGearTB[2]->SetInput( 1, pBundle, 5, TB_DN );
		pLandingGearArmDeploy[0]->output.Connect( pBundle, 7 );// arm pb
		pLandingGearArmDeploy[1]->output.Connect( pBundle, 9 );// dn pb

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_5", 16 );
		pLandingGearArmDeploy[0]->ConnectLight( 0, pBundle, 7 );// arm light
		pLandingGearArmDeploy[1]->ConnectLight( 0, pBundle, 15 );// dn light

		pBundle = STS()->BundleManager()->CreateBundle( "HUD_PLT", 16 );
		pHUDMode->outputA.Connect( pBundle, 1 );// mode dclt plt
		pHUDMode->outputB.Connect( pBundle, 2 );// mode test plt
		pHUDBrightness->ConnectOutputSignal( 0, pBundle, 3 );// brightness lvl 1 plt
		pHUDBrightness->ConnectOutputSignal( 1, pBundle, 4 );// brightness lvl 2 plt
		pHUDBrightness->ConnectOutputSignal( 2, pBundle, 5 );// brightness lvl 3 plt
		pHUDBrightness->ConnectOutputSignal( 3, pBundle, 6 );// brightness lvl 4 plt
		pHUDBrightness->ConnectOutputSignal( 4, pBundle, 7 );// brightness lvl 5 plt
		pHUDBright->outputA.Connect( pBundle, 8 );// bright man night plt
		pHUDBright->outputB.Connect( pBundle, 9 );// bright man day plt
		
		AtlantisPanel::Realize();
	}
		
};
