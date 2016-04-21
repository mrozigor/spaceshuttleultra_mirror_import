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

		Add(pPitchAuto = new PushButtonIndicator(_sts, "F4_PITCH_AUTO", true));
		Add(pPitchCSS = new PushButtonIndicator(_sts, "F4_PITCH_CSS", true));
		Add(pRollYawAuto = new PushButtonIndicator(_sts, "F4_RY_AUTO", true));
		Add(pRollYawCSS = new PushButtonIndicator(_sts, "F4_RY_CSS", true));

		Add( pDragChuteJETTCover = new StandardSwitchCover( _sts, "Drag Chute JETT Cover" ) );

		Add( pDragChuteJETT = new PushButtonIndicator( _sts, "Drag Chute JETT", true ) );

		Add( pHUDMode = new StdSwitch3( _sts, "HUD Mode" ) );// HACK this should be located on panel F8, but is here due to panel click areas

		pHUDMode->SetLabel( 0, "DCLT" );
		pHUDMode->SetLabel( 1, "NORM" );
		pHUDMode->SetLabel( 2, "TEST" );
		pHUDMode->SetSpringLoaded( true, 0 );
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

		pBundle=STS()->BundleManager()->CreateBundle("CSS_CONTROLS", 4);
		pPitchAuto->ConnectAll(pBundle, 0);
		pPitchCSS->ConnectAll(pBundle, 1);
		PitchPortGroup.AddPorts(pBundle, 0, 1);
		pRollYawAuto->ConnectAll(pBundle, 2);
		pRollYawCSS->ConnectAll(pBundle, 3);
		RollYawPortGroup.AddPorts(pBundle, 2, 3);

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

		pBundle = STS()->BundleManager()->CreateBundle( "HUD", 16 );
		//pHUDPower[0]->output.Connect( pBundle, 0 );// power cdr (F3)
		//pHUDPower[1]->output.Connect( pBundle, 1 );// power plt (F3)
		//pHUDMode->outputA.Connect( pBundle, 2 );// mode dclt cdr (F6)
		//pHUDMode->outputB.Connect( pBundle, 3 );// mode test cdr (F6)
		pHUDMode->outputA.Connect( pBundle, 4 );// mode dclt plt (F8)
		//pHUDMode->outputB.Connect( pBundle, 5 );// mode test plt (F8)
		
		AtlantisPanel::Realize();
	}

	void PanelF4::OnPreStep(double SimT, double DeltaT, double MJD)
	{
		PitchPortGroup.OnPreStep();
		RollYawPortGroup.OnPreStep();

		AtlantisPanel::OnPreStep(SimT, DeltaT, MJD);
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

		pPitchCSS->AddAIDToRedrawEventList(AID_F4_P_CSS);
		pPitchCSS->SetSourceImage(g_Param.pbi_lights);
		pPitchCSS->SetBase(0, 0);
		pPitchCSS->SetSourceCoords(true, 0, 0);
		pPitchCSS->SetSourceCoords(false, 0, 14);
		pPitchCSS->SetDimensions(42, 14);
		pPitchCSS->SetMouseRegion(0.591548f, 0.358571f, 0.644673f, 0.468767f);

		pRollYawAuto->AddAIDToRedrawEventList(AID_F4_RY_AUTO);
		SetCommonPBIParameters(pRollYawAuto);
		pRollYawAuto->SetMouseRegion(0.503044f, 0.389149f, 0.554739f, 0.494769f);

		pRollYawCSS->AddAIDToRedrawEventList(AID_F4_RY_CSS);
		SetCommonPBIParameters(pRollYawCSS);
		pRollYawCSS->SetMouseRegion(0.561273f, 0.459885f, 0.614834f, 0.573177f);

		pDragChuteJETT->AddAIDToRedrawEventList( AID_F4_DC_JETT );
		pDragChuteJETT->SetSourceImage( g_Param.pbi_lights );
		pDragChuteJETT->SetBase( 0, 0 );
		pDragChuteJETT->SetSourceCoords( true, 0, 0 );
		pDragChuteJETT->SetSourceCoords( false, 0, 14 );
		pDragChuteJETT->SetDimensions( 42, 14 );
		pDragChuteJETT->SetMouseRegion( 0.361020f, 0.060915f, 0.393703f, 0.122957f );

		pDragChuteJETTCover->SetMouseRegion( 0, 0.352893f, 0.043177f, 0.399339f, 0.154601f );
		pDragChuteJETTCover->SetMouseRegion( 1, 0.356096f, 0.0f, 0.402268f, 0.031398f );
		pDragChuteJETTCover->SetReference( _V( -0.5320, 2.5288, 14.6101 ), _V( 1, 0, 0 ) );
		pDragChuteJETTCover->DefineCoverGroup( GRP_F4COVERS10_VC );

		pHUDMode->DefineSwitchGroup( GRP_F8HUDTEST_VC );
		pHUDMode->SetInitialAnimState( 0.5 );
		pHUDMode->SetReference( _V( -0.5996, 2.4685, 14.5712 ), _V( 1, 0, 0 ) );
		pHUDMode->SetMouseRegion( 0.048451f, 0.220342f, 0.080458f, 0.288456f );
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
