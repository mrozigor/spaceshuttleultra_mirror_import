#include "PanelF4.h"
#include "../meshres_vc_additions.h"
#include "../Atlantis_defs.h"

extern GDIParams g_Param;

namespace vc
{
	PanelF4::PanelF4(Atlantis* _sts)
		: AtlantisPanel(_sts, "F4")
	{
		Add(pSbdbkThrot = new PushButtonIndicator(_sts, "F4_SPDBKTHROT_AUTO", true));
		Add(pSbdbkThrotMan = new StandardLight(_sts, "F4_SPDBKTHROT_MAN"));
		Add(pBodyFlap = new PushButtonIndicator(_sts, "F4_BODYFLAP_AUTO", true));
		Add(pBodyFlapMan = new StandardLight(_sts, "F4_BODYFLAP_MAN"));

		Add( pPitchAuto = new PushButtonIndicator( _sts, "F4_PITCH_AUTO" ) );
		Add( pPitchCSS = new PushButtonIndicator( _sts, "F4_PITCH_CSS" ) );
		Add( pRollYawAuto = new PushButtonIndicator( _sts, "F4_RY_AUTO" ) );
		Add( pRollYawCSS = new PushButtonIndicator( _sts, "F4_RY_CSS" ) );

		Add( pDragChuteJETTCover = new StandardSwitchCover( _sts, "Drag Chute JETT Cover" ) );

		Add( pDragChuteJETT = new PushButtonIndicator( _sts, "Drag Chute JETT" ) );

		Add( pHUDMode = new StdSwitch3( _sts, "HUD Mode" ) );// HACK these 3 should be located on panel F8, but is here due to panel click areas
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

	PanelF4::~PanelF4()
	{
	}

	void PanelF4::Realize()
	{
		DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("SPDBKTHROT_CONTROLS", 16);
		pSbdbkThrot->ConnectAll(pBundle, 0); //AUTO light; common to F2 and F4 PBIs
		pSbdbkThrotMan->input.Connect(pBundle, 2); //MAN light; F4 PBI only
		pSbdbkThrotMan->test.Connect(pBundle, 2); //MAN light; F4 PBI only

		pBundle=STS()->BundleManager()->CreateBundle("BODYFLAP_CONTROLS", 16);
		pBodyFlap->ConnectAll(pBundle, 0); //AUTO light; common to F2 and F4 PBIs
		pBodyFlapMan->input.Connect(pBundle, 1); //MAN light; common to F2 and F4 PBIs
		pBodyFlapMan->test.Connect(pBundle, 1); //MAN light; common to F2 and F4 PBIs

		pBundle=STS()->BundleManager()->CreateBundle( "CSS_CONTROLS", 16 );
		pPitchAuto->output.Connect( pBundle, 8 );
		pPitchAuto->input.Connect( pBundle, 9 );
		pPitchCSS->output.Connect( pBundle, 10 );
		pPitchCSS->input.Connect( pBundle, 11 );
		pRollYawAuto->output.Connect( pBundle, 12 );
		pRollYawAuto->input.Connect( pBundle, 13 );
		pRollYawCSS->output.Connect( pBundle, 14 );
		pRollYawCSS->input.Connect( pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "DRAG_CHUTE", 16 );
		//pDragChuteARM->input.Connect( pBundle, 0 );// arm light
		//pDragChuteDPY->input.Connect( pBundle, 1 );// dpy light
		pDragChuteJETT->input.Connect( pBundle, 2 );// jett light

		//pDragChuteARM->output.Connect( pBundle, 3 );// arm pb (F2)
		//pDragChuteARM->output.Connect( pBundle, 4 );// arm pb (F3)
		//pDragChuteDPY->output.Connect( pBundle, 5 );// dpy pb (F2)
		//pDragChuteDPY->output.Connect( pBundle, 6 );// dpy pb (F3)
		//pDragChuteJETT->output.Connect( pBundle, 7 );// jett pb (F3)
		pDragChuteJETT->output.Connect( pBundle, 8 );// jett pb (F4)

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

	void PanelF4::DefineVC()
	{
		AddAIDToMouseEventList(AID_F4);

		pSbdbkThrot->AddAIDToRedrawEventList(AID_F4_ST_AUTO);
		pSbdbkThrot->SetSourceImage(g_Param.pbi_lights);
		pSbdbkThrot->SetBase(0, 0);
		pSbdbkThrot->SetSourceCoords(true, 0, 0);
		pSbdbkThrot->SetSourceCoords(false, 0, 14);
		pSbdbkThrot->SetDimensions(42, 14);
		pSbdbkThrot->SetMouseRegion(0.772716f, 0.583791f, 0.826997f, 0.697191f);
		pSbdbkThrot->AllowReset(false);

		pSbdbkThrotMan->AddAIDToRedrawEventList(AID_F4_ST_MAN);
		pSbdbkThrotMan->SetSourceImage(g_Param.pbi_lights);
		pSbdbkThrotMan->SetBase(0, 0);
		pSbdbkThrotMan->SetSourceCoords(true, 0, 0);
		pSbdbkThrotMan->SetSourceCoords(false, 0, 14);
		pSbdbkThrotMan->SetDimensions(42, 14);

		pBodyFlap->AddAIDToRedrawEventList(AID_F4_BF_AUTO);
		pBodyFlap->SetSourceImage(g_Param.pbi_lights);
		pBodyFlap->SetBase(0, 0);
		pBodyFlap->SetSourceCoords(true, 0, 0);
		pBodyFlap->SetSourceCoords(false, 0, 14);
		pBodyFlap->SetDimensions(42, 14);
		pBodyFlap->SetMouseRegion(0.838923f, 0.660717f, 0.891593f, 0.770096f);
		pBodyFlap->AllowReset(true);

		pBodyFlapMan->AddAIDToRedrawEventList(AID_F4_BF_MAN);
		pBodyFlapMan->SetSourceImage(g_Param.pbi_lights);
		pBodyFlapMan->SetBase(0, 0);
		pBodyFlapMan->SetSourceCoords(true, 0, 0);
		pBodyFlapMan->SetSourceCoords(false, 0, 14);
		pBodyFlapMan->SetDimensions(42, 14);

		pPitchAuto->AddAIDToRedrawEventList(AID_F4_P_AUTO);
		pPitchAuto->SetSourceImage(g_Param.pbi_lights);
		pPitchAuto->SetBase(0, 0);
		pPitchAuto->SetSourceCoords(true, 0, 0);
		pPitchAuto->SetSourceCoords(false, 0, 14);
		pPitchAuto->SetDimensions(42, 14);
		pPitchAuto->SetMouseRegion(0.531938f, 0.286244f, 0.583997f, 0.395611f);
		pPitchAuto->SetMomentary( true );

		pPitchCSS->AddAIDToRedrawEventList(AID_F4_P_CSS);
		pPitchCSS->SetSourceImage(g_Param.pbi_lights);
		pPitchCSS->SetBase(0, 0);
		pPitchCSS->SetSourceCoords(true, 0, 0);
		pPitchCSS->SetSourceCoords(false, 0, 14);
		pPitchCSS->SetDimensions(42, 14);
		pPitchCSS->SetMouseRegion(0.591548f, 0.358571f, 0.644673f, 0.468767f);
		pPitchCSS->SetMomentary( true );

		pRollYawAuto->AddAIDToRedrawEventList(AID_F4_RY_AUTO);
		SetCommonPBIParameters(pRollYawAuto);
		pRollYawAuto->SetMouseRegion(0.503044f, 0.389149f, 0.554739f, 0.494769f);
		pRollYawAuto->SetMomentary( true );

		pRollYawCSS->AddAIDToRedrawEventList(AID_F4_RY_CSS);
		SetCommonPBIParameters(pRollYawCSS);
		pRollYawCSS->SetMouseRegion(0.561273f, 0.459885f, 0.614834f, 0.573177f);
		pRollYawCSS->SetMomentary( true );

		pDragChuteJETT->AddAIDToRedrawEventList( AID_F4_DC_JETT );
		pDragChuteJETT->SetSourceImage( g_Param.pbi_lights );
		pDragChuteJETT->SetBase( 0, 0 );
		pDragChuteJETT->SetSourceCoords( true, 0, 0 );
		pDragChuteJETT->SetSourceCoords( false, 0, 14 );
		pDragChuteJETT->SetDimensions( 42, 14 );
		pDragChuteJETT->SetMouseRegion( 0.361020f, 0.060915f, 0.393703f, 0.122957f );
		pDragChuteJETT->SetMomentary( true );

		pDragChuteJETTCover->SetMouseRegion( 0, 0.352893f, 0.043177f, 0.399339f, 0.154601f );
		pDragChuteJETTCover->SetMouseRegion( 1, 0.356096f, 0.0f, 0.402268f, 0.031398f );
		pDragChuteJETTCover->SetReference( _V( -0.5320, 2.5288, 14.6101 ), _V( 1, 0, 0 ) );
		pDragChuteJETTCover->DefineCoverGroup( GRP_F4COVERS10_VC );

		pHUDMode->DefineSwitchGroup( GRP_F8HUDTEST_VC );
		pHUDMode->SetInitialAnimState( 0.5 );
		pHUDMode->SetReference( _V( -0.5996, 2.4685, 14.5712 ), _V( 1, 0, 0 ) );
		pHUDMode->SetMouseRegion( 0.048451f, 0.220342f, 0.080458f, 0.288456f );

		pHUDBrightness->DefineSwitchGroup( GRP_F8HUDDIM_VC );
		pHUDBrightness->SetInitialAnimState( 0.5 );
		pHUDBrightness->SetReference( _V( 0.6496, 2.4697, 14.5635 ), _V( 0, 0.275637, -0.961262 ) );
		pHUDBrightness->DefineRotationAngle( 180.0f );
		pHUDBrightness->SetOffset( -90.0f );
		pHUDBrightness->SetMouseRegion( 0.135890f, 0.217720f, 0.186902f, 0.309858f );

		pHUDBright->DefineSwitchGroup( GRP_F8HUDBRT_VC );
		pHUDBright->SetInitialAnimState( 0.5 );
		pHUDBright->SetReference( _V( -0.7070, 2.4680, 14.5712 ), _V( 1, 0, 0 ) );
		pHUDBright->SetMouseRegion( 0.239172f, 0.220413f, 0.270920f, 0.289949f );
	}

	void PanelF4::SetCommonPBIParameters(PushButtonIndicator* pPBI)
	{
		pPBI->SetSourceImage(g_Param.pbi_lights);
		pPBI->SetBase(0, 0);
		pPBI->SetSourceCoords(true, 0, 0);
		pPBI->SetSourceCoords(false, 0, 14);
		pPBI->SetDimensions(42, 14);
	}

	void PanelF4::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		SURFHANDLE tex=oapiGetTextureHandle(STS()->hOrbiterVCMesh, TEX_FPANELS_VC);

		VECTOR3 ofs=STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea(AID_F4, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP);
		oapiVCSetAreaClickmode_Quadrilateral(AID_F4,
			_V(0.5530, 2.535, 14.612)+ofs, _V(1.147, 2.535, 14.612)+ofs,
			_V(0.5530, 2.25, 14.533)+ofs, _V(1.147, 2.25, 14.533)+ofs);

		oapiVCRegisterArea(AID_F4_ST_AUTO, _R(1296, 548, 1338, 562), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F4_ST_MAN, _R(1296, 585, 1338, 599), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F4_BF_AUTO, _R(1436, 542, 1478, 556), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F4_BF_MAN, _R(1437, 583, 1479, 597), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F4_P_AUTO, _R(773, 562, 815, 576), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F4_P_CSS, _R(899, 565, 941, 579), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F4_RY_AUTO, _R(773, 678, 815, 692), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F4_RY_CSS, _R(901, 680, 943, 694), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea( AID_F4_DC_JETT, _R( 150, 757, 192, 771 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex );
	}
};
