#include "PanelF2.h"
#include "../meshres_vc_additions.h"
#include "../Atlantis_defs.h"

extern GDIParams g_Param;

namespace vc
{
	PanelF2::PanelF2(Atlantis* _sts)
		: AtlantisPanel(_sts, "F2")
	{
		Add( pSbdbkThrot = new PushButtonIndicator( _sts, "F2_SPDBKTHROT_AUTO" ) );
		Add( pSbdbkThrotMan = new StandardLight( _sts, "F2_SPDBKTHROT_MAN" ) );
		Add( pBodyFlap = new PushButtonIndicator( _sts, "F2_BODYFLAP_AUTO" ) );
		Add( pBodyFlapMan = new StandardLight( _sts, "F2_BODYFLAP_MAN" ) );
		
		Add( pPitchAuto = new PushButtonIndicator( _sts, "F2_PITCH_AUTO" ) );
		Add( pPitchCSS = new PushButtonIndicator( _sts, "F2_PITCH_CSS" ) );
		Add( pRollYawAuto = new PushButtonIndicator( _sts, "F2_RY_AUTO" ) );
		Add( pRollYawCSS = new PushButtonIndicator( _sts, "F2_RY_CSS" ) );

		Add( pDragChuteARMCover = new StandardSwitchCover( _sts, "Drag Chute ARM Cover" ) );
		Add( pDragChuteDPYCover = new StandardSwitchCover( _sts, "Drag Chute DPY Cover" ) );

		Add( pDragChuteARM = new PushButtonIndicator( _sts, "Drag Chute ARM" ) );
		Add( pDragChuteDPY = new PushButtonIndicator( _sts, "Drag Chute DPY" ) );

		Add( pHUDMode = new StdSwitch3( _sts, "HUD Mode" ) );// HACK these 3 should be located on panel F6, but is here due to panel click areas
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

	PanelF2::~PanelF2()
	{
	}

	void PanelF2::Realize()
	{
		DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("SPDBKTHROT_CONTROLS", 16);
		pSbdbkThrot->output.Connect( pBundle, 0 );// CDR PBI
		pSbdbkThrot->input.Connect( pBundle, 1 );// CDR AUTO LT
		pSbdbkThrotMan->input.Connect( pBundle, 2 );// CDR MAN LT

		pBundle=STS()->BundleManager()->CreateBundle("BODYFLAP_CONTROLS", 16);
		pBodyFlap->ConnectAll(pBundle, 0); //AUTO light; common to F2 and F4 PBIs
		pBodyFlapMan->input.Connect(pBundle, 1); //MAN light; common to F2 and F4 PBIs
		pBodyFlapMan->test.Connect(pBundle, 1); //MAN light; common to F2 and F4 PBIs
		
		pBundle=STS()->BundleManager()->CreateBundle( "CSS_CONTROLS", 16 );
		pPitchAuto->output.Connect( pBundle, 0 );
		pPitchAuto->input.Connect( pBundle, 1 );
		pPitchCSS->output.Connect( pBundle, 2 );
		pPitchCSS->input.Connect( pBundle, 3 );
		pRollYawAuto->output.Connect( pBundle, 4 );
		pRollYawAuto->input.Connect( pBundle, 5 );
		pRollYawCSS->output.Connect( pBundle, 6 );
		pRollYawCSS->input.Connect( pBundle, 7 );

		pBundle = STS()->BundleManager()->CreateBundle( "DRAG_CHUTE", 16 );
		pDragChuteARM->input.Connect( pBundle, 0 );// arm light
		pDragChuteDPY->input.Connect( pBundle, 1 );// dpy light
		//pDragChuteJETT->input.Connect( pBundle, 2 );// jett light

		pDragChuteARM->output.Connect( pBundle, 3 );// arm pb (F2)
		//pDragChuteARM->output.Connect( pBundle, 4 );// arm pb (F3)
		pDragChuteDPY->output.Connect( pBundle, 5 );// dpy pb (F2)
		//pDragChuteDPY->output.Connect( pBundle, 6 );// dpy pb (F3)
		//pDragChuteJETT->output.Connect( pBundle, 7 );// jett pb (F3)
		//pDragChuteJETT->output.Connect( pBundle, 8 );// jett pb (F4)

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

	void PanelF2::DefineVC()
	{
		AddAIDToMouseEventList(AID_F2);

		pSbdbkThrot->AddAIDToRedrawEventList(AID_F2_ST_AUTO);
		SetCommonPBIParameters(pSbdbkThrot);
		pSbdbkThrot->SetMouseRegion(0.181085f, 0.579283f, 0.235243f, 0.687911f);
		pSbdbkThrot->SetMomentary( true );

		pSbdbkThrotMan->AddAIDToRedrawEventList(AID_F2_ST_MAN);
		pSbdbkThrotMan->SetSourceImage(g_Param.pbi_lights);
		pSbdbkThrotMan->SetBase(0, 0);
		pSbdbkThrotMan->SetSourceCoords(true, 0, 0);
		pSbdbkThrotMan->SetSourceCoords(false, 0, 14);
		pSbdbkThrotMan->SetDimensions(42, 14);

		pBodyFlap->AddAIDToRedrawEventList(AID_F2_BF_AUTO);
		SetCommonPBIParameters(pBodyFlap);
		pBodyFlap->SetMouseRegion(0.120885f, 0.6449937f, 0.175332f, 0.757229f);
		pBodyFlap->AllowReset(true);

		pBodyFlapMan->AddAIDToRedrawEventList(AID_F2_BF_MAN);
		pBodyFlapMan->SetSourceImage(g_Param.pbi_lights);
		pBodyFlapMan->SetBase(0, 0);
		pBodyFlapMan->SetSourceCoords(true, 0, 0);
		pBodyFlapMan->SetSourceCoords(false, 0, 14);
		pBodyFlapMan->SetDimensions(42, 14);

		pPitchAuto->AddAIDToRedrawEventList(AID_F2_P_AUTO);
		SetCommonPBIParameters(pPitchAuto);
		pPitchAuto->SetMouseRegion(0.414578f, 0.300987f, 0.468414f, 0.414815f);
		pPitchAuto->SetMomentary( true );

		pPitchCSS->AddAIDToRedrawEventList(AID_F2_P_CSS);
		SetCommonPBIParameters(pPitchCSS);
		pPitchCSS->SetMouseRegion(0.349629f, 0.374445f, 0.404706f, 0.487884f);
		pPitchCSS->SetMomentary( true );

		pRollYawAuto->AddAIDToRedrawEventList(AID_F2_RY_AUTO);		
		SetCommonPBIParameters(pRollYawAuto);
		pRollYawAuto->SetMouseRegion(0.445804f, 0.409961f, 0.499280f, 0.520422f);
		pRollYawAuto->SetMomentary( true );

		pRollYawCSS->AddAIDToRedrawEventList(AID_F2_RY_CSS);
		SetCommonPBIParameters(pRollYawCSS);
		pRollYawCSS->SetMouseRegion(0.383109f, 0.310907f, 0.436190f, 0.592650f);
		pRollYawCSS->SetMomentary( true );

		pDragChuteARM->AddAIDToRedrawEventList( AID_F2_DC_ARM );
		pDragChuteARM->SetSourceImage( g_Param.pbi_lights );
		pDragChuteARM->SetBase( 0, 0 );
		pDragChuteARM->SetSourceCoords( true, 46, 28 );
		pDragChuteARM->SetSourceCoords( false, 0, 28 );
		pDragChuteARM->SetDimensions( 46, 36 );
		pDragChuteARM->SetMouseRegion( 0.598273f, 0.060357f, 0.626919f, 0.122262f );
		pDragChuteARM->SetMomentary( true );

		pDragChuteDPY->AddAIDToRedrawEventList( AID_F2_DC_DPY );
		pDragChuteDPY->SetSourceImage( g_Param.pbi_lights );
		pDragChuteDPY->SetBase( 0, 0 );
		pDragChuteDPY->SetSourceCoords( true, 46, 64 );
		pDragChuteDPY->SetSourceCoords( false, 0, 64 );
		pDragChuteDPY->SetDimensions( 46, 36 );
		pDragChuteDPY->SetMouseRegion( 0.653209f, 0.062580f, 0.681769f, 0.117574f );
		pDragChuteDPY->SetMomentary( true );

		pDragChuteARMCover->SetMouseRegion( 0, 0.585063f, 0.040947f, 0.633282f, 0.150854f );
		pDragChuteARMCover->SetMouseRegion( 1, 0.583068f, 0.0f, 0.629692f, 0.027156f );
		pDragChuteARMCover->SetReference( _V( -0.5320, 2.5288, 14.6101 ), _V( 1, 0, 0 ) );
		pDragChuteARMCover->DefineCoverGroup( GRP_F2COVERS8_VC );

		pDragChuteDPYCover->SetMouseRegion( 0, 0.642587f, 0.041293f, 0.690531f, 0.154314f );
		pDragChuteDPYCover->SetMouseRegion( 1, 0.642025f, 0.0f, 0.689314f, 0.027156f );
		pDragChuteDPYCover->SetReference( _V( -0.5320, 2.5288, 14.6101 ), _V( 1, 0, 0 ) );
		pDragChuteDPYCover->DefineCoverGroup( GRP_F2COVERS9_VC );

		pHUDMode->DefineSwitchGroup( GRP_F6HUDTEST_VC );
		pHUDMode->SetInitialAnimState( 0.5 );
		pHUDMode->SetReference( _V( 0.7052, 2.4685, 14.5712 ), _V( 1, 0, 0 ) );
		pHUDMode->SetMouseRegion( 0.753794f, 0.219066f, 0.789329f, 0.290315f );

		pHUDBrightness->DefineSwitchGroup( GRP_F6HUDDIM_VC );
		pHUDBrightness->SetInitialAnimState( 0.5 );
		pHUDBrightness->SetReference( _V( -0.6552, 2.4697, 14.5635 ), _V( 0, 0.275637, -0.961262 ) );
		pHUDBrightness->DefineRotationAngle( 180.0f );
		pHUDBrightness->SetOffset( -90.0f );
		pHUDBrightness->SetMouseRegion( 0.846057f, 0.215747f, 0.897844f, 0.306673f );

		pHUDBright->DefineSwitchGroup( GRP_F6HUDBRT_VC );
		pHUDBright->SetInitialAnimState( 0.5 );
		pHUDBright->SetReference( _V( 0.5978, 2.4679, 14.5712 ), _V( 1, 0, 0 ) );
		pHUDBright->SetMouseRegion( 0.951320f, 0.219343f, 0.985137f, 0.289879f );
	}

	void PanelF2::SetCommonPBIParameters(PushButtonIndicator* pPBI)
	{
		pPBI->SetSourceImage(g_Param.pbi_lights);
		pPBI->SetBase(0, 0);
		pPBI->SetSourceCoords(true, 0, 0);
		pPBI->SetSourceCoords(false, 0, 14);
		pPBI->SetDimensions(42, 14);
	}

	void PanelF2::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		SURFHANDLE tex=oapiGetTextureHandle(STS()->hOrbiterVCMesh, TEX_FPANELS_VC);

		VECTOR3 ofs=STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea(AID_F2, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP);
		oapiVCSetAreaClickmode_Quadrilateral(AID_F2,
			_V(-1.154, 2.535, 14.612)+ofs, _V(-0.5808, 2.535, 14.612)+ofs,
			_V(-1.154, 2.25, 14.533)+ofs, _V(-0.5808, 2.25, 14.533)+ofs);

		oapiVCRegisterArea(AID_F2_ST_AUTO, _R(867, 255, 909, 269), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F2_ST_MAN, _R(867, 293, 909, 307), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F2_BF_AUTO, _R(737, 252, 779, 266), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F2_BF_MAN, _R(738, 294, 780, 308), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F2_P_AUTO, _R(1362, 272, 1404, 286), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F2_P_CSS, _R(1231, 275, 1273, 289), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);		
		oapiVCRegisterArea(AID_F2_RY_AUTO, _R(1364, 396, 1406, 410), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F2_RY_CSS, _R(1232, 399, 1274, 413), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea( AID_F2_DC_ARM, _R( 1731, 168, 1777, 204 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex );		
		oapiVCRegisterArea( AID_F2_DC_DPY, _R( 1861, 242, 1907, 278 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex );
	}
};
