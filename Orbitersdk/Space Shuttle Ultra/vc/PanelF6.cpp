#include "PanelF6.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "MDU.h"
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
	}

	PanelF6::~PanelF6()
	{
	}

	void PanelF6::DefineVC()
	{
		VECTOR3 switch_rot = _V(1, 0, 0);
		VECTOR3 push_dir = _V( 0.0, -0.258681, 0.965963 );

		AddAIDToMouseEventList(AID_F6);

		pCDR1->SetMouseRegion(0.0f, 0.0f, 0.487004f, 0.615168f);
		pCDR2->SetMouseRegion(0.508892f, 0.0f, 0.993170f, 0.613257f);
		
		pCDR1->DefineVCGroup( GRP_CDR1_VC );
		pCDR2->DefineVCGroup( GRP_CDR2_VC );

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

		pLandingGearTB[0]->DefineMeshGroup( STS()->mesh_vc, GRP_F6TALKBACK2_VC );
		
		pLandingGearTB[1]->DefineMeshGroup( STS()->mesh_vc, GRP_F6TALKBACK1_VC );
		
		pLandingGearTB[2]->DefineMeshGroup( STS()->mesh_vc, GRP_F6TALKBACK3_VC );

		pLandingGearArmDeployCover[0]->SetMouseRegion( 0, 0.066067f, 0.825664f, 0.130528f, 0.900261f );
		pLandingGearArmDeployCover[0]->SetMouseRegion( 1, 0.004029f, 0.889020f, 0.029792f, 0.960046f );
		pLandingGearArmDeployCover[0]->SetReference( _V( -0.92, 1.99996, 14.68735 ), _V( 0, 0.965408, 0.260745 ) );
		pLandingGearArmDeployCover[0]->DefineCoverGroup( GRP_F6COVER1_VC );

		pLandingGearArmDeployCover[1]->SetMouseRegion( 0, 0.153059f, 0.825691f, 0.217150f, 0.900504f );
		pLandingGearArmDeployCover[1]->SetMouseRegion( 1, 0.082762f, 0.910630f, 0.126002f, 0.935335f );
		pLandingGearArmDeployCover[1]->SetReference( _V( -0.87370, 1.99996, 14.68735 ), _V( 0, 0.965408, 0.260745 ) );
		pLandingGearArmDeployCover[1]->DefineCoverGroup( GRP_F6COVER2_VC );

		pLandingGearArmDeploy[0]->SetStateOffset( 1, 0.0f, 0.488281f );
		pLandingGearArmDeploy[0]->SetDirection( push_dir );
		pLandingGearArmDeploy[0]->SetMouseRegion( 0.077788f, 0.831051f, 0.119110f, 0.892631f );
		pLandingGearArmDeploy[0]->DefineMeshGroup( STS()->mesh_vc, GRP_F6_A5_S1_VC );

		pLandingGearArmDeploy[1]->SetStateOffset( 1, 0.0f, 0.488281f );
		pLandingGearArmDeploy[1]->SetDirection( push_dir );
		pLandingGearArmDeploy[1]->SetMouseRegion( 0.163805f, 0.830309f, 0.205588f, 0.892445f );
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
		pAbort->SetMouseRegion( 0.897943f, 0.704919f, 0.930596f, 0.752289f );
		pAbort->DefineMeshGroup( STS()->mesh_vc, GRP_F6_A8_S2_VC );
		return;
	}

	void PanelF6::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		
		oapiVCRegisterArea(AID_F6, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral (AID_F6, 
			_V(-0.940, 2.312, 14.756)+ofs, _V(-0.413, 2.312, 14.755)+ofs,
			_V(-0.947, 1.951, 14.674)+ofs, _V(-0.413, 1.949, 14.673)+ofs);
		
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

		AtlantisPanel::Realize();
	}
		
};
