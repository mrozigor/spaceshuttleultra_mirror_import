#include "PanelF3.h"
#include "..\Atlantis.h"
#include "../Atlantis_defs.h"
#include "..\meshres_vc.h"


namespace vc
{
	PanelF3::PanelF3( Atlantis* _sts ):AtlantisPanel( _sts, "F3" )
	{
		Add( pDragChuteARMCover = new StandardSwitchCover( _sts, "Drag Chute ARM Cover" ) );
		Add( pDragChuteDPYCover = new StandardSwitchCover( _sts, "Drag Chute DPY Cover" ) );
		Add( pDragChuteJETTCover = new StandardSwitchCover( _sts, "Drag Chute JETT Cover" ) );

		Add( pDragChuteARM = new PushButtonIndicatorDoubleLight( _sts, "Drag Chute ARM" ) );
		Add( pDragChuteDPY = new PushButtonIndicatorDoubleLight( _sts, "Drag Chute DPY" ) );
		Add( pDragChuteJETT = new PushButtonIndicatorDoubleLight( _sts, "Drag Chute JETT" ) );

		Add( pHUDPower[0] = new StdSwitch2( _sts, "HUD Power CDR" ) );
		Add( pHUDPower[1] = new StdSwitch2( _sts, "HUD Power PLT" ) );

		pHUDPower[0]->SetLabel( 0, "OFF" );
		pHUDPower[0]->SetLabel( 1, "ON" );
		pHUDPower[1]->SetLabel( 0, "OFF" );
		pHUDPower[1]->SetLabel( 1, "ON" );

		Add( pNWSFail = new StandardSingleLight( _sts, "NWS Fail" ) );
		Add( pAntiSkidFail = new StandardSingleLight( _sts, "Anti Skid Fail" ) );
	}

	PanelF3::~PanelF3()
	{
	}

	void PanelF3::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "DRAG_CHUTE", 16 );
		//pDragChuteARM->output.Connect( pBundle, 0 );// arm pb (F2)
		pDragChuteARM->output.Connect( pBundle, 1 );// arm pb (F3)
		//pDragChuteDPY->output.Connect( pBundle, 2 );// dpy pb (F2)
		pDragChuteDPY->output.Connect( pBundle, 3 );// dpy pb (F3)
		pDragChuteJETT->output.Connect( pBundle, 4 );// jett pb (F3)
		//pDragChuteJETT->output.Connect( pBundle, 5 );// jett pb (F4)

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_1", 16 );
		pDragChuteDPY->ConnectLight( 2, pBundle, 7 );
		pDragChuteARM->ConnectLight( 2, pBundle, 11 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA2_2", 16 );
		pAntiSkidFail->ConnectLight( 0, pBundle, 7 );
		pDragChuteJETT->ConnectLight( 2, pBundle, 13 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_1", 16 );
		pDragChuteJETT->ConnectLight( 0, pBundle, 5 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA3_5", 16 );
		pDragChuteARM->ConnectLight( 0, pBundle, 3 );
		pNWSFail->ConnectLight( 0, pBundle, 5 );
		pDragChuteDPY->ConnectLight( 0, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "HUD_CDR", 16 );
		pHUDPower[0]->output.Connect( pBundle, 0 );// power cdr
		
		pBundle = STS()->BundleManager()->CreateBundle( "HUD_PLT", 16 );
		pHUDPower[1]->output.Connect( pBundle, 0 );// power plt

		AtlantisPanel::Realize();
	}

	void PanelF3::DefineVC()
	{
		VECTOR3 switch_rot = _V( 1, 0, 0 );
		VECTOR3 push_dir = _V( 0.0, -0.269911, 0.962885 );

		AddAIDToMouseEventList( AID_F3 );

		pDragChuteARM->SetStateOffset( 1, 0.0f, 0.488281f );// 1
		pDragChuteARM->SetStateOffset( 2, 0.146484f, 0.488281f );// 2
		pDragChuteARM->SetStateOffset( 3, 0.146484f, 0.0f );// 12
		pDragChuteARM->SetDirection( push_dir );
		pDragChuteARM->SetMouseRegion( 0.941485f, 0.155126f, 0.961229f, 0.349624f );
		pDragChuteARM->DefineMeshGroup( STS()->mesh_vc, GRP_F3_S8_VC );

		pDragChuteDPY->SetStateOffset( 1, 0.0f, 0.488281f );// 1
		pDragChuteDPY->SetStateOffset( 2, 0.146484f, 0.488281f );// 2
		pDragChuteDPY->SetStateOffset( 3, 0.146484f, 0.0f );// 12
		pDragChuteDPY->SetDirection( push_dir );
		pDragChuteDPY->SetMouseRegion( 0.970976f, 0.154359f, 0.990583f, 0.348370f );
		pDragChuteDPY->DefineMeshGroup( STS()->mesh_vc, GRP_F3_S9_VC );

		pDragChuteJETT->SetStateOffset( 1, 0.0f, 0.488281f );// 1
		pDragChuteJETT->SetStateOffset( 2, 0.146484f, 0.488281f );// 2
		pDragChuteJETT->SetStateOffset( 3, 0.146484f, 0.0f );// 12
		pDragChuteJETT->SetDirection( push_dir );
		pDragChuteJETT->SetMouseRegion( 0.028511f, 0.158667f, 0.048433f, 0.349924f );
		pDragChuteJETT->DefineMeshGroup( STS()->mesh_vc, GRP_F3_S7_VC );

		pDragChuteARMCover->SetMouseRegion( 0, 0.938748f, 0.117915f, 0.963932f, 0.419314f );
		pDragChuteARMCover->SetMouseRegion( 1, 0.933402f, 0.0f, 0.952428f, 0.066717f );
		pDragChuteARMCover->SetReference( _V( -0.5320, 2.5288, 14.6101 ), switch_rot );
		pDragChuteARMCover->DefineCoverGroup( GRP_F3COVERS8_VC );

		pDragChuteDPYCover->SetMouseRegion( 0, 0.968342f, 0.117915f, 0.992407f, 0.419314f );
		pDragChuteDPYCover->SetMouseRegion( 1, 0.961976f, 0.0f, 0.986786f, 0.066717f );
		pDragChuteDPYCover->SetReference( _V( -0.5320, 2.5288, 14.6101 ), switch_rot );
		pDragChuteDPYCover->DefineCoverGroup( GRP_F3COVERS9_VC );

		pDragChuteJETTCover->SetMouseRegion( 0, 0.025776f, 0.120962f, 0.051179f, 0.428174f );
		pDragChuteJETTCover->SetMouseRegion( 1, 0.025775f, 0.0f, 0.052357f, 0.073427f );
		pDragChuteJETTCover->SetReference( _V( -0.5320, 2.5288, 14.6101 ), switch_rot );
		pDragChuteJETTCover->DefineCoverGroup( GRP_F3COVERS7_VC );
		
		pHUDPower[0]->DefineSwitchGroup( GRP_F3S1_VC );
		pHUDPower[0]->SetInitialAnimState( 0.5 );
		pHUDPower[0]->SetReference( _V( 0.5358, 2.4529, 14.5890 ), switch_rot );
		pHUDPower[0]->SetMouseRegion( 0.021770f, 0.649764f, 0.044223f, 0.888632f );

		pHUDPower[1]->DefineSwitchGroup( GRP_F3S6_VC );
		pHUDPower[1]->SetInitialAnimState( 0.5 );
		pHUDPower[1]->SetReference( _V( -0.5374, 2.4530, 14.5890 ), switch_rot );
		pHUDPower[1]->SetMouseRegion( 0.976221f, 0.656113f, 0.997667f, 0.889501f );

		pNWSFail->DefineMeshGroup( STS()->mesh_vc, GRP_F3_XDS3_VC );
		pNWSFail->SetStateOffset( 1, 0.139648f, 0.0f );

		pAntiSkidFail->DefineMeshGroup( STS()->mesh_vc, GRP_F3_XDS4_VC );
		pAntiSkidFail->SetStateOffset( 1, 0.139648f, 0.0f );
	}

	void PanelF3::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs=STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_F3, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP );
		oapiVCSetAreaClickmode_Quadrilateral( AID_F3, 
			_V( -0.5758, 2.5361, 14.6138 ) + ofs, _V( 0.5480, 2.5361, 14.6138 ) + ofs, 
			_V( -0.5758, 2.4282, 14.5819 ) + ofs, _V( 0.5480, 2.4282, 14.5819 ) + ofs );

		return;
	}
};
