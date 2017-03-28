#include "PanelF2.h"
#include "..\Atlantis.h"
#include "../Atlantis_defs.h"


namespace vc
{
	PanelF2::PanelF2(Atlantis* _sts)
		: AtlantisPanel(_sts, "F2")
	{
		Add( pMasterAlarm = new PushButtonIndicatorSingleLight( _sts, "Master Alarm" ) );

		Add( pPitchAuto = new PushButtonIndicatorSingleLight( _sts, "F2_PITCH_AUTO" ) );
		Add( pPitchCSS = new PushButtonIndicatorSingleLight( _sts, "F2_PITCH_CSS" ) );
		Add( pRollYawAuto = new PushButtonIndicatorSingleLight( _sts, "F2_RY_AUTO" ) );
		Add( pRollYawCSS = new PushButtonIndicatorSingleLight( _sts, "F2_RY_CSS" ) );
		
		Add( pSbdbkThrot = new PushButtonIndicatorDoubleLight( _sts, "F2_SPDBKTHROT" ) );
		Add( pBodyFlap = new PushButtonIndicatorDoubleLight( _sts, "F2_BODYFLAP" ) );

		Add( pDragChuteARMCover = new StandardSwitchCover( _sts, "Drag Chute ARM Cover" ) );
		Add( pDragChuteDPYCover = new StandardSwitchCover( _sts, "Drag Chute DPY Cover" ) );

		Add( pDragChuteARM = new PushButtonIndicatorDoubleLight( _sts, "Drag Chute ARM" ) );
		Add( pDragChuteDPY = new PushButtonIndicatorDoubleLight( _sts, "Drag Chute DPY" ) );

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

		Add( pBFC = new StandardSingleLight( _sts, "BFC" ) );
	}

	PanelF2::~PanelF2()
	{
	}

	void PanelF2::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "DAP_CH_CONTROLS", 16 );
		pPitchAuto->ConnectPushButton( pBundle, 0 );
		pPitchCSS->ConnectPushButton( pBundle, 2 );
		pRollYawAuto->ConnectPushButton( pBundle, 4 );
		pRollYawCSS->ConnectPushButton( pBundle, 6 );
		pSbdbkThrot->ConnectPushButton( pBundle, 8 );
		pBodyFlap->ConnectPushButton( pBundle, 10 );

		pBundle = STS()->BundleManager()->CreateBundle( "DRAG_CHUTE", 16 );
		pDragChuteARM->output.Connect( pBundle, 0 );// arm pb (F2)
		//pDragChuteARM->output.Connect( pBundle, 1 );// arm pb (F3)
		pDragChuteDPY->output.Connect( pBundle, 2 );// dpy pb (F2)
		//pDragChuteDPY->output.Connect( pBundle, 3 );// dpy pb (F3)
		//pDragChuteJETT->output.Connect( pBundle, 4 );// jett pb (F3)
		//pDragChuteJETT->output.Connect( pBundle, 5 );// jett pb (F4)

		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_2", 16 );
		pBodyFlap->ConnectLight( 2, pBundle, 5 );
		pPitchAuto->ConnectLight( 0, pBundle, 9 );
		pPitchCSS->ConnectLight( 0, pBundle, 13 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_3", 16 );
		pRollYawAuto->ConnectLight( 0, pBundle, 5 );
		pRollYawCSS->ConnectLight( 0, pBundle, 9 );
		
		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_4", 16 );
		pSbdbkThrot->ConnectLight( 0, pBundle, 1 );
		pSbdbkThrot->ConnectLight( 2, pBundle, 5 );
		pBodyFlap->ConnectLight( 0, pBundle, 9 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_5", 16 );
		pMasterAlarm->ConnectLight( 0, pBundle, 1 );
		pBFC->ConnectLight( 0, pBundle, 9 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_2", 16 );
		pDragChuteARM->ConnectLight( 2, pBundle, 5 );
		pDragChuteDPY->ConnectLight( 2, pBundle, 9 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_5", 16 );
		pMasterAlarm->ConnectLight( 1, pBundle, 1 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_1", 16 );
		pDragChuteDPY->ConnectLight( 0, pBundle, 9 );
		pDragChuteARM->ConnectLight( 0, pBundle, 13 );

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
		VECTOR3 push_dir = _V( 0.0, -0.269911, 0.962885 );

		AddAIDToMouseEventList(AID_F2);

		pMasterAlarm->SetStateOffset( 1, 0.139648f, 0.0f );
		pMasterAlarm->SetDirection( push_dir );
		pMasterAlarm->SetMouseRegion( 0.547658f, 0.269709f, 0.599607f, 0.342875f );
		pMasterAlarm->DefineMeshGroup( STS()->mesh_vc, GRP_F2_S1_VC );

		pPitchAuto->SetStateOffset( 1, 0.0f, 0.488281f );
		pPitchAuto->SetDirection( push_dir );
		pPitchAuto->SetMouseRegion( 0.423229f, 0.345204f, 0.455615f, 0.413250f );
		pPitchAuto->DefineMeshGroup( STS()->mesh_vc, GRP_F2_S2_VC );

		pPitchCSS->SetStateOffset( 1, 0.0f, 0.488281f );
		pPitchCSS->SetDirection( push_dir );
		pPitchCSS->SetMouseRegion( 0.371251f, 0.406804f, 0.405170f, 0.469908f );
		pPitchCSS->DefineMeshGroup( STS()->mesh_vc, GRP_F2_S3_VC );

		pRollYawAuto->SetStateOffset( 1, 0.0f, 0.488281f );
		pRollYawAuto->SetDirection( push_dir );
		pRollYawAuto->SetMouseRegion( 0.447410f, 0.430849f, 0.481545f, 0.497045f );
		pRollYawAuto->DefineMeshGroup( STS()->mesh_vc, GRP_F2_S5_VC );

		pRollYawCSS->SetStateOffset( 1, 0.0f, 0.488281f );
		pRollYawCSS->SetDirection( push_dir );
		pRollYawCSS->SetMouseRegion( 0.394850f, 0.489301f, 0.428580f, 0.556178f );
		pRollYawCSS->DefineMeshGroup( STS()->mesh_vc, GRP_F2_S6_VC );

		pSbdbkThrot->SetStateOffset( 1, 0.0f, 0.488281f );// AUTO
		pSbdbkThrot->SetStateOffset( 2, 0.146484f, 0.488281f );// MAN
		pSbdbkThrot->SetStateOffset( 3, 0.146484f, 0.0f );// AUTO/MAN
		pSbdbkThrot->SetDirection( push_dir );
		pSbdbkThrot->SetMouseRegion( 0.187106f, 0.618308f, 0.221076f, 0.681222f );
		pSbdbkThrot->DefineMeshGroup( STS()->mesh_vc, GRP_F2_S8_VC );

		pBodyFlap->SetStateOffset( 1, 0.0f, 0.488281f );// AUTO
		pBodyFlap->SetStateOffset( 2, 0.146484f, 0.488281f );// MAN
		pBodyFlap->SetStateOffset( 3, 0.146484f, 0.0f );// AUTO/MAN
		pBodyFlap->SetDirection( push_dir );
		pBodyFlap->SetMouseRegion( 0.133054f, 0.680736f, 0.167659f, 0.742256f );
		pBodyFlap->DefineMeshGroup( STS()->mesh_vc, GRP_F2_S9_VC );

		pDragChuteARM->SetStateOffset( 1, 0.0f, 0.488281f );// 1
		pDragChuteARM->SetStateOffset( 2, 0.146484f, 0.488281f );// 2
		pDragChuteARM->SetStateOffset( 3, 0.146484f, 0.0f );// 12
		pDragChuteARM->SetDirection( push_dir );
		pDragChuteARM->SetMouseRegion( 0.591878f, 0.054978f, 0.630076f, 0.128218f );
		pDragChuteARM->DefineMeshGroup( STS()->mesh_vc, GRP_F2_S10_VC );

		pDragChuteDPY->SetStateOffset( 1, 0.0f, 0.488281f );// 1
		pDragChuteDPY->SetStateOffset( 2, 0.146484f, 0.488281f );// 2
		pDragChuteDPY->SetStateOffset( 3, 0.146484f, 0.0f );// 12
		pDragChuteDPY->SetDirection( push_dir );
		pDragChuteDPY->SetMouseRegion( 0.648582f, 0.054299f, 0.686353f, 0.127821f );
		pDragChuteDPY->DefineMeshGroup( STS()->mesh_vc, GRP_F2_S11_VC );

		pDragChuteARMCover->SetMouseRegion( 0, 0.585063f, 0.040947f, 0.633282f, 0.150854f );
		pDragChuteARMCover->SetMouseRegion( 1, 0.583068f, 0.0f, 0.629692f, 0.027156f );
		pDragChuteARMCover->SetReference( _V( -0.5320, 2.5288, 14.6101 ), _V( 1, 0, 0 ) );
		pDragChuteARMCover->DefineCoverGroup( GRP_F2COVERS10_VC );

		pDragChuteDPYCover->SetMouseRegion( 0, 0.642587f, 0.041293f, 0.690531f, 0.154314f );
		pDragChuteDPYCover->SetMouseRegion( 1, 0.642025f, 0.0f, 0.689314f, 0.027156f );
		pDragChuteDPYCover->SetReference( _V( -0.5320, 2.5288, 14.6101 ), _V( 1, 0, 0 ) );
		pDragChuteDPYCover->DefineCoverGroup( GRP_F2COVERS11_VC );

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

		pBFC->DefineMeshGroup( STS()->mesh_vc, GRP_F2_XDS3_VC );
		pBFC->SetStateOffset( 1, 0.139648f, 0.0f );
	}

	void PanelF2::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs=STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea(AID_F2, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP);
		oapiVCSetAreaClickmode_Quadrilateral(AID_F2,
			_V(-1.154, 2.535, 14.612)+ofs, _V(-0.5808, 2.535, 14.612)+ofs,
			_V(-1.154, 2.25, 14.533)+ofs, _V(-0.5808, 2.25, 14.533)+ofs);

		return;
	}
};
