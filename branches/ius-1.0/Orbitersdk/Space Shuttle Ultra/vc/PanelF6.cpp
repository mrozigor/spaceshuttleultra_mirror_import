#include "PanelF6.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "MDU.h"
#include "../Atlantis_defs.h"
#include "../meshres_vc_additions.h"

extern GDIParams g_Param;

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

		Add( pLandingGearTB[0] = new StandardTalkback( _sts, "Landing Gear Nose", 2 ) );
		Add( pLandingGearTB[1] = new StandardTalkback( _sts, "Landing Gear Left", 2 ) );
		Add( pLandingGearTB[2] = new StandardTalkback( _sts, "Landing Gear Right", 2 ) );
		
		Add( pLandingGearArmDeployCover[0] = new StandardSwitchCover( _sts, "Landing Gear ARM Cover" ) );
		Add( pLandingGearArmDeployCover[1] = new StandardSwitchCover( _sts, "Landing Gear DN Cover" ) );
		
		Add( pLandingGearArmDeploy[0] = new PushButtonIndicator( _sts, "Landing Gear ARM", true ) );
		Add( pLandingGearArmDeploy[1] = new PushButtonIndicator( _sts, "Landing Gear DN", true ) );
	}

	PanelF6::~PanelF6()
	{
	}

	void PanelF6::DefineVC()
	{
		VECTOR3 switch_rot = _V(1, 0, 0);

		AddAIDToMouseEventList(AID_F6);

		pCDR1->SetMouseRegion(0.0f, 0.0f, 0.487004f, 0.615168f);
		pCDR2->SetMouseRegion(0.508892f, 0.0f, 0.993170f, 0.613257f);
		
		pCDR1->DefineVCGroup(MFDGROUPS[MDUID_CDR1]);
		pCDR2->DefineVCGroup(MFDGROUPS[MDUID_CDR2]);

		pFltCntlrPower->DefineSwitchGroup(GRP_F6SWITCH_5_VC);
		pFltCntlrPower->SetInitialAnimState(0.5);
		pFltCntlrPower->SetReference(_V(-0.734, 1.99, 14.685), switch_rot);
		pFltCntlrPower->SetMouseRegion(0.372206f, 0.851179f, 0.428169f, 0.928952f);

		pADIAttitude->DefineSwitchGroup( GRP_F6SWITCH_6_VC );
		pADIAttitude->SetInitialAnimState( 0.5 );
		pADIAttitude->SetReference( _V( 0.633, 2.045, 14.7004 ), switch_rot );
		pADIAttitude->SetMouseRegion( 0.571952f, 0.695254f, 0.614206f, 0.759335f );

		pADIError->DefineSwitchGroup( GRP_F6SWITCH_7_VC );
		pADIError->SetInitialAnimState( 0.5 );
		pADIError->SetReference( _V( 0.5921, 2.0469, 14.7009 ), switch_rot );
		pADIError->SetMouseRegion( 0.649129f, 0.690659f, 0.693501f, 0.759623f );

		pADIRate->DefineSwitchGroup( GRP_F6SWITCH_8_VC );
		pADIRate->SetInitialAnimState( 0.5 );
		pADIRate->SetReference( _V( 0.5627, 2.0463, 14.7008 ), switch_rot );
		pADIRate->SetMouseRegion( 0.704403f, 0.693751f, 0.747005f, 0.759008f );

		pLandingGearTB[0]->AddAIDToRedrawEventList( AID_F6_TKBK1 );
		pLandingGearTB[0]->SetDimensions( 57, 34 );
		pLandingGearTB[0]->SetTalkbackLocation( 0, 0 );

		pLandingGearTB[1]->AddAIDToRedrawEventList( AID_F6_TKBK2 );
		pLandingGearTB[1]->SetDimensions( 57, 34 );
		pLandingGearTB[1]->SetTalkbackLocation( 0, 0 );

		pLandingGearTB[2]->AddAIDToRedrawEventList( AID_F6_TKBK3 );
		pLandingGearTB[2]->SetDimensions( 57, 34 );
		pLandingGearTB[2]->SetTalkbackLocation( 0, 0 );

		pLandingGearArmDeployCover[0]->SetMouseRegion( 0, 0.066067f, 0.825664f, 0.130528f, 0.900261f );
		pLandingGearArmDeployCover[0]->SetMouseRegion( 1, 0.004029f, 0.889020f, 0.029792f, 0.960046f );
		pLandingGearArmDeployCover[0]->SetReference( _V( -0.92, 1.99996, 14.68735 ), _V( 0, 0.965408, 0.260745 ) );
		pLandingGearArmDeployCover[0]->DefineCoverGroup( GRP_F6COVER1_VC );

		pLandingGearArmDeployCover[1]->SetMouseRegion( 0, 0.153059f, 0.825691f, 0.217150f, 0.900504f );
		pLandingGearArmDeployCover[1]->SetMouseRegion( 1, 0.082762f, 0.910630f, 0.126002f, 0.935335f );
		pLandingGearArmDeployCover[1]->SetReference( _V( -0.87370, 1.99996, 14.68735 ), _V( 0, 0.965408, 0.260745 ) );
		pLandingGearArmDeployCover[1]->DefineCoverGroup( GRP_F6COVER2_VC );

		pLandingGearArmDeploy[0]->AddAIDToRedrawEventList( AID_F6_PB1 );
		pLandingGearArmDeploy[0]->SetSourceImage( g_Param.pbi_lights );
		pLandingGearArmDeploy[0]->SetBase( 0, 0 );
		pLandingGearArmDeploy[0]->SetSourceCoords( true, 0, 0 );
		pLandingGearArmDeploy[0]->SetSourceCoords( false, 0, 14 );
		pLandingGearArmDeploy[0]->SetDimensions( 42, 14 );
		pLandingGearArmDeploy[0]->SetMouseRegion( 0.078388f, 0.840748f, 0.112708f, 0.888240f );

		pLandingGearArmDeploy[1]->AddAIDToRedrawEventList( AID_F6_PB2 );
		pLandingGearArmDeploy[1]->SetSourceImage( g_Param.pbi_lights );
		pLandingGearArmDeploy[1]->SetBase( 0, 0 );
		pLandingGearArmDeploy[1]->SetSourceCoords( true, 42, 0 );
		pLandingGearArmDeploy[1]->SetSourceCoords( false, 0, 14 );
		pLandingGearArmDeploy[1]->SetDimensions( 42, 14 );
		pLandingGearArmDeploy[1]->SetMouseRegion( 0.168069f, 0.840792f, 0.201960f, 0.887350f );
	}

	void PanelF6::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		
		oapiVCRegisterArea(AID_F6, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral (AID_F6, 
			_V(-0.940, 2.312, 14.756)+ofs, _V(-0.413, 2.312, 14.755)+ofs,
			_V(-0.947, 1.951, 14.674)+ofs, _V(-0.413, 1.949, 14.673)+ofs);
		
		SURFHANDLE panel_tex = oapiGetTextureHandle( STS()->hOrbiterVCMesh, TEX_PANELF6_VC );
		oapiVCRegisterArea( AID_F6_TKBK1, _R( 256, 121, 313, 155 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panel_tex );
		oapiVCRegisterArea( AID_F6_TKBK2, _R( 123, 147, 180, 181 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panel_tex );
		oapiVCRegisterArea( AID_F6_TKBK3, _R( 385, 143, 442, 177 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panel_tex );

		oapiVCRegisterArea( AID_F6_PB1, _R( 185, 323, 227, 337 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panel_tex );
		oapiVCRegisterArea( AID_F6_PB2, _R( 357, 322, 399, 336 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panel_tex );
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

		pLandingGearArmDeploy[0]->input.Connect( pBundle, 6 );// arm light
		pLandingGearArmDeploy[1]->input.Connect( pBundle, 7 );// dn light

		pLandingGearArmDeploy[0]->output.Connect( pBundle, 8 );// arm pb (F6)
		pLandingGearArmDeploy[1]->output.Connect( pBundle, 9 );// dn pb (F6)

		AtlantisPanel::Realize();
	}
		
};
