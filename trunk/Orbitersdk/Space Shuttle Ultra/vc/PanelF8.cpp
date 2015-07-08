#include "PanelF8.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "MDU.h"
#include "../Atlantis_defs.h"
#include "../meshres_vc_additions.h"

extern GDIParams g_Param;

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

		Add( pLandingGearTB[0] = new StandardTalkback( _sts, "Landing Gear Nose", 2 ) );
		Add( pLandingGearTB[1] = new StandardTalkback( _sts, "Landing Gear Left", 2 ) );
		Add( pLandingGearTB[2] = new StandardTalkback( _sts, "Landing Gear Right", 2 ) );

		Add( pLandingGearArmDeployCover[0] = new StandardSwitchCover( _sts, "Landing Gear ARM Cover" ) );
		Add( pLandingGearArmDeployCover[1] = new StandardSwitchCover( _sts, "Landing Gear DN Cover" ) );

		Add( pLandingGearArmDeploy[0] = new PushButtonIndicator( _sts, "Landing Gear ARM", true ) );
		Add( pLandingGearArmDeploy[1] = new PushButtonIndicator( _sts, "Landing Gear DN", true ) );
	}

	PanelF8::~PanelF8()
	{
	}

	void PanelF8::DefineVC()
	{
		VECTOR3 switch_rot = _V(1, 0, 0);

		AddAIDToMouseEventList(AID_F8);

		pPLT1->SetMouseRegion(0.0f, 0.0f, 0.491582f, 0.609860f);
		pPLT1->SetEdgekeyArea(0.210163f, 0.915644f, 0.783473f, 0.973985f);
		pPLT2->SetMouseRegion(0.511129f, 0.0f, 1.0f, 0.611175f);
		pPLT2->SetEdgekeyArea(0.210163f, 0.915644f, 0.783473f, 0.973985f);
		
		pPLT1->DefineVCGroup(MFDGROUPS[MDUID_PLT1]);
		pPLT2->DefineVCGroup(MFDGROUPS[MDUID_PLT2]);

		pFltCntlrPower->DefineSwitchGroup(GRP_F8SWITCH5_VC);
		pFltCntlrPower->SetInitialAnimState(0.5);
		pFltCntlrPower->SetReference(_V(-0.734, 1.99, 14.685), switch_rot);
		pFltCntlrPower->SetMouseRegion(0.372206f, 0.851179f, 0.428169f, 0.928952f);

		pADIAttitude->DefineSwitchGroup( GRP_F8SWITCH6_VC );
		pADIAttitude->SetInitialAnimState( 0.5 );
		pADIAttitude->SetReference( _V( -0.633, 2.045, 14.7004 ), switch_rot );
		pADIAttitude->SetMouseRegion( 0.576194f, 0.689506f, 0.618179f, 0.755844f );

		pADIError->DefineSwitchGroup( GRP_F8SWITCH7_VC );
		pADIError->SetInitialAnimState( 0.5 );
		pADIError->SetReference( _V( -0.5921, 2.0469, 14.7009 ), switch_rot );
		pADIError->SetMouseRegion( 0.655636f, 0.686714f, 0.698151f, 0.754200f );

		pADIRate->DefineSwitchGroup( GRP_F8SWITCH8_VC );
		pADIRate->SetInitialAnimState( 0.5 );
		pADIRate->SetReference( _V( -0.5627, 2.0463, 14.7008 ), switch_rot );
		pADIRate->SetMouseRegion( 0.710745f, 0.687697f, 0.753016f, 0.750151f );

		pLandingGearTB[0]->AddAIDToRedrawEventList( AID_F8_TKBK1 );
		pLandingGearTB[0]->SetDimensions( 57, 34 );
		pLandingGearTB[0]->SetTalkbackLocation( 0, 0 );

		pLandingGearTB[1]->AddAIDToRedrawEventList( AID_F8_TKBK2 );
		pLandingGearTB[1]->SetDimensions( 57, 34 );
		pLandingGearTB[1]->SetTalkbackLocation( 0, 0 );

		pLandingGearTB[2]->AddAIDToRedrawEventList( AID_F8_TKBK3 );
		pLandingGearTB[2]->SetDimensions( 57, 34 );
		pLandingGearTB[2]->SetTalkbackLocation( 0, 0 );

		pLandingGearArmDeployCover[0]->SetMouseRegion( 0, 0.067760f, 0.823818f, 0.131444f, 0.893443f );
		pLandingGearArmDeployCover[0]->SetMouseRegion( 1, 0.004029f, 0.889020f, 0.029792f, 0.960046f );////////////
		pLandingGearArmDeployCover[0]->SetReference( _V( 0.92, 1.99996, 14.68735 ), _V( 0, 0.965408, 0.260745 ) );////////////////
		pLandingGearArmDeployCover[0]->DefineCoverGroup( GRP_F8COVER1_VC );

		pLandingGearArmDeployCover[1]->SetMouseRegion( 0, 0.152777f, 0.820686f, 0.213770f, 0.893461f );
		pLandingGearArmDeployCover[1]->SetMouseRegion( 1, 0.082762f, 0.910630f, 0.126002f, 0.935335f );//////////////
		pLandingGearArmDeployCover[1]->SetReference( _V( 0.87370, 1.99996, 14.68735 ), _V( 0, 0.965408, 0.260745 ) );/////////
		pLandingGearArmDeployCover[1]->DefineCoverGroup( GRP_F8COVER2_VC );

		pLandingGearArmDeploy[0]->AddAIDToRedrawEventList( AID_F8_PB1 );
		pLandingGearArmDeploy[0]->SetSourceImage( g_Param.pbi_lights );
		pLandingGearArmDeploy[0]->SetBase( 0, 0 );
		pLandingGearArmDeploy[0]->SetSourceCoords( true, 0, 0 );
		pLandingGearArmDeploy[0]->SetSourceCoords( false, 0, 14 );
		pLandingGearArmDeploy[0]->SetDimensions( 42, 14 );
		pLandingGearArmDeploy[0]->SetMouseRegion( 0.079704f, 0.835708f, 0.113657f, 0.881215f );

		pLandingGearArmDeploy[1]->AddAIDToRedrawEventList( AID_F8_PB2 );
		pLandingGearArmDeploy[1]->SetSourceImage( g_Param.pbi_lights );
		pLandingGearArmDeploy[1]->SetBase( 0, 0 );
		pLandingGearArmDeploy[1]->SetSourceCoords( true, 42, 0 );
		pLandingGearArmDeploy[1]->SetSourceCoords( false, 0, 14 );
		pLandingGearArmDeploy[1]->SetDimensions( 42, 14 );
		pLandingGearArmDeploy[1]->SetMouseRegion( 0.169614f, 0.835418f, 0.203512f, 0.881207f );
	}

	void PanelF8::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		oapiVCRegisterArea(AID_F8, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral (AID_F8, 
			_V(0.416, 2.312, 14.755)+ofs, _V(0.942, 2.312, 14.755)+ofs,
			_V(0.416, 1.949, 14.673)+ofs, _V(0.942, 1.949, 14.673) + ofs);

		SURFHANDLE panel_tex = oapiGetTextureHandle( STS()->hOrbiterVCMesh, TEX_PANELF8_VC );
		oapiVCRegisterArea( AID_F8_TKBK1, _R( 256, 121, 313, 155 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panel_tex );
		oapiVCRegisterArea( AID_F8_TKBK2, _R( 123, 147, 180, 181 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panel_tex );
		oapiVCRegisterArea( AID_F8_TKBK3, _R( 385, 143, 442, 177 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panel_tex );

		oapiVCRegisterArea( AID_F8_PB1, _R( 185, 323, 227, 337 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panel_tex );
		oapiVCRegisterArea( AID_F8_PB2, _R( 357, 322, 399, 336 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panel_tex );
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

		pLandingGearArmDeploy[0]->input.Connect( pBundle, 6 );// arm light
		pLandingGearArmDeploy[1]->input.Connect( pBundle, 7 );// dn light

		pLandingGearArmDeploy[0]->output.Connect( pBundle, 10 );// arm pb (F8)
		pLandingGearArmDeploy[1]->output.Connect( pBundle, 11 );// dn pb (F8)
		
		AtlantisPanel::Realize();
	}
		
};
