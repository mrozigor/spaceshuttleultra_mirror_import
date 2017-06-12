#include "PanelF4.h"
#include "..\Atlantis.h"
#include "../Atlantis_defs.h"
#include "..\meshres_vc.h"


namespace vc
{
	PanelF4::PanelF4(Atlantis* _sts)
		: AtlantisPanel(_sts, "F4")
	{
		Add( pMasterAlarm = new PushButtonIndicatorSingleLight( _sts, "Master Alarm" ) );

		Add( pPitchAuto = new PushButtonIndicatorSingleLight( _sts, "F4_PITCH_AUTO" ) );
		Add( pPitchCSS = new PushButtonIndicatorSingleLight( _sts, "F4_PITCH_CSS" ) );
		Add( pRollYawAuto = new PushButtonIndicatorSingleLight( _sts, "F4_RY_AUTO" ) );
		Add( pRollYawCSS = new PushButtonIndicatorSingleLight( _sts, "F4_RY_CSS" ) );

		Add( pSbdbkThrot = new PushButtonIndicatorDoubleLight( _sts, "F4_SPDBKTHROT" ) );
		Add( pBodyFlap = new PushButtonIndicatorDoubleLight( _sts, "F4_BODYFLAP" ) );

		Add( pDragChuteJETTCover = new StandardSwitchCover( _sts, "Drag Chute JETT Cover" ) );

		Add( pDragChuteJETT = new PushButtonIndicatorDoubleLight( _sts, "Drag Chute JETT" ) );

		Add( pBFC = new StandardSingleLight( _sts, "BFC" ) );
	}

	PanelF4::~PanelF4()
	{
	}

	void PanelF4::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "DAP_CH_CONTROLS", 16 );
		pPitchAuto->ConnectPushButton( pBundle, 1 );
		pPitchCSS->ConnectPushButton( pBundle, 3 );
		pRollYawAuto->ConnectPushButton( pBundle, 5 );
		pRollYawCSS->ConnectPushButton( pBundle, 7 );
		pSbdbkThrot->ConnectPushButton( pBundle, 9 );
		pBodyFlap->ConnectPushButton( pBundle, 11 );

		pBundle = STS()->BundleManager()->CreateBundle( "DRAG_CHUTE", 16 );
		//pDragChuteARM->output.Connect( pBundle, 0 );// arm pb (F2)
		//pDragChuteARM->output.Connect( pBundle, 1 );// arm pb (F3)
		//pDragChuteDPY->output.Connect( pBundle, 2 );// dpy pb (F2)
		//pDragChuteDPY->output.Connect( pBundle, 3 );// dpy pb (F3)
		//pDragChuteJETT->output.Connect( pBundle, 4 );// jett pb (F3)
		pDragChuteJETT->output.Connect( pBundle, 5 );// jett pb (F4)

		pBundle = STS()->BundleManager()->CreateBundle( "ACA1_4", 16 );
		pMasterAlarm->ConnectLight( 0, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_1", 16 );
		pDragChuteJETT->ConnectLight( 2, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_2", 16 );
		pPitchAuto->ConnectLight( 0, pBundle, 11 );
		pPitchCSS->ConnectLight( 0, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_3", 16 );
		pRollYawAuto->ConnectLight( 0, pBundle, 7 );
		pRollYawCSS->ConnectLight( 0, pBundle, 11 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_4", 16 );
		pSbdbkThrot->ConnectLight( 0, pBundle, 3 );
		pSbdbkThrot->ConnectLight( 2, pBundle, 7 );
		pBodyFlap->ConnectLight( 0, pBundle, 11 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_5", 16 );
		pBodyFlap->ConnectLight( 2, pBundle, 3 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_3", 16 );
		pBFC->ConnectLight( 0, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_4", 16 );
		pMasterAlarm->ConnectLight( 1, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_5", 16 );
		pDragChuteJETT->ConnectLight( 0, pBundle, 11 );
		
		AtlantisPanel::Realize();
	}

	void PanelF4::DefineVC()
	{
		VECTOR3 push_dir = _V( 0.0, -0.269911, 0.962885 );

		AddAIDToMouseEventList(AID_F4);

		pMasterAlarm->SetStateOffset( 1, 0.139648f, 0.0f );
		pMasterAlarm->SetDirection( push_dir );
		pMasterAlarm->SetMouseRegion( AID_F4, 0.404892f, 0.258904f, 0.454918f, 0.331408f );
		pMasterAlarm->DefineMeshGroup( STS()->mesh_vc, GRP_F4_S1_VC );

		pPitchAuto->SetStateOffset( 1, 0.0f, 0.488281f );
		pPitchAuto->SetDirection( push_dir );
		pPitchAuto->SetMouseRegion( AID_F4, 0.573131f, 0.348409f, 0.606497f, 0.413077f );
		pPitchAuto->DefineMeshGroup( STS()->mesh_vc, GRP_F4_S2_VC );

		pPitchCSS->SetStateOffset( 1, 0.0f, 0.488281f );
		pPitchCSS->SetDirection( push_dir );
		pPitchCSS->SetMouseRegion( AID_F4, 0.621720f, 0.4071805f, 0.656692f, 0.474488f );
		pPitchCSS->DefineMeshGroup( STS()->mesh_vc, GRP_F4_S3_VC );

		pRollYawAuto->SetStateOffset( 1, 0.0f, 0.488281f );
		pRollYawAuto->SetDirection( push_dir );
		pRollYawAuto->SetMouseRegion( AID_F4, 0.548946f, 0.430385f, 0.581996f, 0.496719f );
		pRollYawAuto->DefineMeshGroup( STS()->mesh_vc, GRP_F4_S5_VC );

		pRollYawCSS->SetStateOffset( 1, 0.0f, 0.488281f );
		pRollYawCSS->SetDirection( push_dir );
		pRollYawCSS->SetMouseRegion( AID_F4, 0.599243f, 0.489802f, 0.633905f, 0.558635f );
		pRollYawCSS->DefineMeshGroup( STS()->mesh_vc, GRP_F4_S6_VC );

		pSbdbkThrot->SetStateOffset( 1, 0.0f, 0.488281f );// AUTO
		pSbdbkThrot->SetStateOffset( 2, 0.146484f, 0.488281f );// MAN
		pSbdbkThrot->SetStateOffset( 3, 0.146484f, 0.0f );// AUTO/MAN
		pSbdbkThrot->SetDirection( push_dir );
		pSbdbkThrot->SetMouseRegion( AID_F4, 0.798881f, 0.620206f, 0.833964f, 0.684788f );
		pSbdbkThrot->DefineMeshGroup( STS()->mesh_vc, GRP_F4_S8_VC );

		pBodyFlap->SetStateOffset( 1, 0.0f, 0.488281f );// AUTO
		pBodyFlap->SetStateOffset( 2, 0.146484f, 0.488281f );// MAN
		pBodyFlap->SetStateOffset( 3, 0.146484f, 0.0f );// AUTO/MAN
		pBodyFlap->SetDirection( push_dir );
		pBodyFlap->SetMouseRegion( AID_F4, 0.849432f, 0.681700f, 0.885129f, 0.744627f );
		pBodyFlap->DefineMeshGroup( STS()->mesh_vc, GRP_F4_S9_VC );

		pDragChuteJETT->SetStateOffset( 1, 0.0f, 0.488281f );// 1
		pDragChuteJETT->SetStateOffset( 2, 0.146484f, 0.488281f );// 2
		pDragChuteJETT->SetStateOffset( 3, 0.146484f, 0.0f );// 12
		pDragChuteJETT->SetDirection( push_dir );
		pDragChuteJETT->SetMouseRegion( AID_F4, 0.358708f, 0.054155f, 0.396472f, 0.128148f );
		pDragChuteJETT->DefineMeshGroup( STS()->mesh_vc, GRP_F4_S10_VC );

		pDragChuteJETTCover->SetMouseRegion( AID_F4, 0, 0.352893f, 0.043177f, 0.399339f, 0.154601f );
		pDragChuteJETTCover->SetMouseRegion( AID_F4, 1, 0.356096f, 0.0f, 0.402268f, 0.031398f );
		pDragChuteJETTCover->SetReference( _V( -0.5320, 2.5288, 14.6101 ), _V( 1, 0, 0 ) );
		pDragChuteJETTCover->DefineCoverGroup( GRP_F4COVERS10_VC );

		pBFC->DefineMeshGroup( STS()->mesh_vc, GRP_F4_XDS3_VC );
		pBFC->SetStateOffset( 1, 0.139648f, 0.0f );
	}

	void PanelF4::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs=STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea(AID_F4, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP);
		oapiVCSetAreaClickmode_Quadrilateral(AID_F4,
			_V(0.5530, 2.535, 14.612)+ofs, _V(1.147, 2.535, 14.612)+ofs,
			_V(0.5530, 2.25, 14.533)+ofs, _V(1.147, 2.25, 14.533)+ofs);

		return;
	}
};
