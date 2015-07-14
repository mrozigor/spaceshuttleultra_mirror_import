#include "PanelF3.h"
#include "../meshres_vc_additions.h"
#include "../Atlantis_defs.h"


extern GDIParams g_Param;


namespace vc
{
	PanelF3::PanelF3( Atlantis* _sts ):AtlantisPanel( _sts, "F3" )
	{
		Add( pDragChuteARMCover = new StandardSwitchCover( _sts, "Drag Chute ARM Cover" ) );
		Add( pDragChuteDPYCover = new StandardSwitchCover( _sts, "Drag Chute DPY Cover" ) );
		Add( pDragChuteJETTCover = new StandardSwitchCover( _sts, "Drag Chute JETT Cover" ) );

		Add( pDragChuteARM = new PushButtonIndicator( _sts, "Drag Chute ARM", true ) );
		Add( pDragChuteDPY = new PushButtonIndicator( _sts, "Drag Chute DPY", true ) );
		Add( pDragChuteJETT = new PushButtonIndicator( _sts, "Drag Chute JETT", true ) );
	}

	PanelF3::~PanelF3()
	{
	}

	void PanelF3::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "DRAG_CHUTE", 16 );
		pDragChuteARM->input.Connect( pBundle, 0 );// arm light
		pDragChuteDPY->input.Connect( pBundle, 1 );// dpy light
		pDragChuteJETT->input.Connect( pBundle, 2 );// jett light

		//pDragChuteARM->output.Connect( pBundle, 3 );// arm pb (F2)
		pDragChuteARM->output.Connect( pBundle, 4 );// arm pb (F3)
		//pDragChuteDPY->output.Connect( pBundle, 5 );// dpy pb (F2)
		pDragChuteDPY->output.Connect( pBundle, 6 );// dpy pb (F3)
		pDragChuteJETT->output.Connect( pBundle, 7 );// jett pb (F3)
		//pDragChuteJETT->output.Connect( pBundle, 8 );// jett pb (F4)
		
		AtlantisPanel::Realize();
	}

	void PanelF3::DefineVC()
	{
		AddAIDToMouseEventList( AID_F3 );

		pDragChuteARM->AddAIDToRedrawEventList( AID_F3_DC_ARM );
		pDragChuteARM->SetSourceImage( g_Param.pbi_lights );
		pDragChuteARM->SetBase( 0, 0 );
		pDragChuteARM->SetSourceCoords( true, 46, 28 );
		pDragChuteARM->SetSourceCoords( false, 0, 28 );
		pDragChuteARM->SetDimensions( 46, 36 );
		pDragChuteARM->SetMouseRegion( 0.944215f, 0.175093f, 0.959694f, 0.332207f );

		pDragChuteDPY->AddAIDToRedrawEventList( AID_F3_DC_DPY );
		pDragChuteDPY->SetSourceImage( g_Param.pbi_lights );
		pDragChuteDPY->SetBase( 0, 0 );
		pDragChuteDPY->SetSourceCoords( true, 46, 64 );
		pDragChuteDPY->SetSourceCoords( false, 0, 64 );
		pDragChuteDPY->SetDimensions( 46, 36 );
		pDragChuteDPY->SetMouseRegion( 0.972908f, 0.169789f, 0.988385f, 0.328097f );

		pDragChuteJETT->AddAIDToRedrawEventList( AID_F3_DC_JETT );
		pDragChuteJETT->SetSourceImage( g_Param.pbi_lights );
		pDragChuteJETT->SetBase( 0, 0 );
		pDragChuteJETT->SetSourceCoords( true, 0, 0 );
		pDragChuteJETT->SetSourceCoords( false, 0, 14 );
		pDragChuteJETT->SetDimensions( 42, 14 );
		pDragChuteJETT->SetMouseRegion( 0.030069f, 0.170118f, 0.047526f, 0.337567f );

		pDragChuteARMCover->SetMouseRegion( 0, 0.938748f, 0.117915f, 0.963932f, 0.419314f );
		pDragChuteARMCover->SetMouseRegion( 1, 0.933402f, 0.0f, 0.952428f, 0.066717f );
		pDragChuteARMCover->SetReference( _V( -0.5320, 2.5288, 14.6101 ), _V( 1, 0, 0 ) );
		pDragChuteARMCover->DefineCoverGroup( GRP_F3COVERS8_VC );

		pDragChuteDPYCover->SetMouseRegion( 0, 0.968342f, 0.117915f, 0.992407f, 0.419314f );
		pDragChuteDPYCover->SetMouseRegion( 1, 0.961976f, 0.0f, 0.986786f, 0.066717f );
		pDragChuteDPYCover->SetReference( _V( -0.5320, 2.5288, 14.6101 ), _V( 1, 0, 0 ) );
		pDragChuteDPYCover->DefineCoverGroup( GRP_F3COVERS9_VC );

		pDragChuteJETTCover->SetMouseRegion( 0, 0.025776f, 0.120962f, 0.051179f, 0.428174f );
		pDragChuteJETTCover->SetMouseRegion( 1, 0.025775f, 0.0f, 0.052357f, 0.073427f );
		pDragChuteJETTCover->SetReference( _V( -0.5320, 2.5288, 14.6101 ), _V( 1, 0, 0 ) );
		pDragChuteJETTCover->DefineCoverGroup( GRP_F3COVERS10_VC );
	}

	void PanelF3::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs=STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_F3, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP );
		oapiVCSetAreaClickmode_Quadrilateral( AID_F3, 
			_V( -0.5758, 2.5361, 14.6138 ) + ofs, _V( 0.5480, 2.5361, 14.6138 ) + ofs, 
			_V( -0.5758, 2.4282, 14.5819 ) + ofs, _V( 0.5480, 2.4282, 14.5819 ) + ofs );

		SURFHANDLE tex = oapiGetTextureHandle( STS()->hOrbiterVCMesh, TEX_FPANELS_VC );
		oapiVCRegisterArea( AID_F3_DC_ARM, _R( 1731, 168, 1777, 204 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex );		
		oapiVCRegisterArea( AID_F3_DC_DPY, _R( 1861, 242, 1907, 278 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex );
		oapiVCRegisterArea( AID_F3_DC_JETT, _R( 150, 757, 192, 771 ), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex );
	}
};
