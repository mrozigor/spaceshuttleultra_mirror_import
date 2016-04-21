#include "PanelF2.h"
#include "../meshres_vc_additions.h"
#include "../Atlantis_defs.h"

extern GDIParams g_Param;

namespace vc
{
	PanelF2::PanelF2(Atlantis* _sts)
		: AtlantisPanel(_sts, "F2")
	{
		Add(pSbdbkThrot = new PushButtonIndicator(_sts, "F2_SPDBKTHROT_AUTO", true));
		Add(pSbdbkThrotMan = new StandardLight(_sts, "F2_SPDBKTHROT_MAN"));
		Add(pBodyFlap = new PushButtonIndicator(_sts, "F2_BODYFLAP_AUTO", true));
		Add(pBodyFlapMan = new StandardLight(_sts, "F2_BODYFLAP_MAN"));
		
		Add(pPitchAuto = new PushButtonIndicator(_sts, "F2_PITCH_AUTO", true));
		Add(pPitchCSS = new PushButtonIndicator(_sts, "F2_PITCH_CSS", true));
		Add(pRollYawAuto = new PushButtonIndicator(_sts, "F2_RY_AUTO", true));
		Add(pRollYawCSS = new PushButtonIndicator(_sts, "F2_RY_CSS", true));

		Add( pDragChuteARMCover = new StandardSwitchCover( _sts, "Drag Chute ARM Cover" ) );
		Add( pDragChuteDPYCover = new StandardSwitchCover( _sts, "Drag Chute DPY Cover" ) );

		Add( pDragChuteARM = new PushButtonIndicator( _sts, "Drag Chute ARM", true ) );
		Add( pDragChuteDPY = new PushButtonIndicator( _sts, "Drag Chute DPY", true ) );

		Add( pHUDMode = new StdSwitch3( _sts, "HUD Mode" ) );// HACK this should be located on panel F6, but is here due to panel click areas

		pHUDMode->SetLabel( 0, "DCLT" );
		pHUDMode->SetLabel( 1, "NORM" );
		pHUDMode->SetLabel( 2, "TEST" );
		pHUDMode->SetSpringLoaded( true, 0 );
	}

	PanelF2::~PanelF2()
	{
	}

	void PanelF2::Realize()
	{
		DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("SPDBKTHROT_CONTROLS", 16);
		pSbdbkThrot->ConnectAll(pBundle, 0); //AUTO light; common to F2 and F4 PBIs
		pSbdbkThrotMan->input.Connect(pBundle, 1); //MAN light; F2 PBI only
		pSbdbkThrotMan->test.Connect(pBundle, 1); //MAN light; F2 PBI only

		pBundle=STS()->BundleManager()->CreateBundle("BODYFLAP_CONTROLS", 16);
		pBodyFlap->ConnectAll(pBundle, 0); //AUTO light; common to F2 and F4 PBIs
		pBodyFlapMan->input.Connect(pBundle, 1); //MAN light; common to F2 and F4 PBIs
		pBodyFlapMan->test.Connect(pBundle, 1); //MAN light; common to F2 and F4 PBIs
		
		pBundle=STS()->BundleManager()->CreateBundle("CSS_CONTROLS", 4);
		pPitchAuto->ConnectAll(pBundle, 0);
		pPitchCSS->ConnectAll(pBundle, 1);
		PitchPortGroup.AddPorts(pBundle, 0, 1);
		pRollYawAuto->ConnectAll(pBundle, 2);
		pRollYawCSS->ConnectAll(pBundle, 3);
		RollYawPortGroup.AddPorts(pBundle, 2, 3);

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

		pBundle = STS()->BundleManager()->CreateBundle( "HUD", 16 );
		//pHUDPower[0]->output.Connect( pBundle, 0 );// power cdr (F3)
		//pHUDPower[1]->output.Connect( pBundle, 1 );// power plt (F3)
		pHUDMode->outputA.Connect( pBundle, 2 );// mode dclt cdr (F6)
		//pHUDMode->outputB.Connect( pBundle, 3 );// mode test cdr (F6)
		//pHUDMode->outputA.Connect( pBundle, 4 );// mode dclt plt (F8)
		//pHUDMode->outputB.Connect( pBundle, 5 );// mode test plt (F8)
		
		AtlantisPanel::Realize();
	}

	void PanelF2::OnPreStep(double SimT, double DeltaT, double MJD)
	{
		PitchPortGroup.OnPreStep();
		RollYawPortGroup.OnPreStep();

		AtlantisPanel::OnPreStep(SimT, DeltaT, MJD);
	}

	void PanelF2::DefineVC()
	{
		AddAIDToMouseEventList(AID_F2);

		pSbdbkThrot->AddAIDToRedrawEventList(AID_F2_ST_AUTO);
		SetCommonPBIParameters(pSbdbkThrot);
		pSbdbkThrot->SetMouseRegion(0.181085f, 0.579283f, 0.235243f, 0.687911f);
		pSbdbkThrot->AllowReset(false);

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

		pPitchCSS->AddAIDToRedrawEventList(AID_F2_P_CSS);
		SetCommonPBIParameters(pPitchCSS);
		pPitchCSS->SetMouseRegion(0.349629f, 0.374445f, 0.404706f, 0.487884f);

		pRollYawAuto->AddAIDToRedrawEventList(AID_F2_RY_AUTO);		
		SetCommonPBIParameters(pRollYawAuto);
		pRollYawAuto->SetMouseRegion(0.445804f, 0.409961f, 0.499280f, 0.520422f);

		pRollYawCSS->AddAIDToRedrawEventList(AID_F2_RY_CSS);
		SetCommonPBIParameters(pRollYawCSS);
		pRollYawCSS->SetMouseRegion(0.383109f, 0.310907f, 0.436190f, 0.592650f);

		pDragChuteARM->AddAIDToRedrawEventList( AID_F2_DC_ARM );
		pDragChuteARM->SetSourceImage( g_Param.pbi_lights );
		pDragChuteARM->SetBase( 0, 0 );
		pDragChuteARM->SetSourceCoords( true, 46, 28 );
		pDragChuteARM->SetSourceCoords( false, 0, 28 );
		pDragChuteARM->SetDimensions( 46, 36 );
		pDragChuteARM->SetMouseRegion( 0.598273f, 0.060357f, 0.626919f, 0.122262f );

		pDragChuteDPY->AddAIDToRedrawEventList( AID_F2_DC_DPY );
		pDragChuteDPY->SetSourceImage( g_Param.pbi_lights );
		pDragChuteDPY->SetBase( 0, 0 );
		pDragChuteDPY->SetSourceCoords( true, 46, 64 );
		pDragChuteDPY->SetSourceCoords( false, 0, 64 );
		pDragChuteDPY->SetDimensions( 46, 36 );
		pDragChuteDPY->SetMouseRegion( 0.653209f, 0.062580f, 0.681769f, 0.117574f );

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
