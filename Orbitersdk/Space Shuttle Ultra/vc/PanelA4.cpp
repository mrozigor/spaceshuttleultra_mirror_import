#include "PanelA4.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"
#include "..\CommonDefs.h"
#include "..\meshres_vc_a4.h"


namespace vc
{
	PanelA4::PanelA4( Atlantis *_sts ):AtlantisPanel( _sts, "A4" )
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELA4 );
		mesh_index = MESH_UNDEFINED;

		Add( pMissionTimer = new StdSwitch3( _sts, "Mission Timer" ) );
		pMissionTimer->SetLabel( 0, "TEST" );
		pMissionTimer->SetLabel( 1, "MET" );
		pMissionTimer->SetLabel( 2, "GMT" );

		Add( pMissionTime = new _7SegDisp_MissionTime( _sts, "Mission Time" ) );
		Add( pEventTime = new _7SegDisp_EventTime( _sts, "Event Time" ) );
	}

	PanelA4::~PanelA4()
	{
	}

	void PanelA4::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelA4::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelA4::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelA4::DefineVC()
	{
		VECTOR3 switch_rot = _V( -0.96598, 0, 0.25860 );

		AddAIDToMouseEventList( AID_A4 );

		pMissionTimer->DefineSwitchGroup( GRP_S1_A4_VC );
		pMissionTimer->SetInitialAnimState( 0.5 );
		pMissionTimer->SetReference( _V( -0.7997, 2.9475, 12.3330 ), switch_rot );
		pMissionTimer->SetMouseRegion( 0.741337f, 0.766488f, 0.860226f, 0.890987f );
		pMissionTimer->SetSpringLoaded( true, 0 );

		pMissionTime->DefineMesh( mesh_index );
		pMissionTime->DefineComponent( GRP_M1_1_A4_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->DefineComponent( GRP_M1_2_A4_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->DefineComponent( GRP_M1_3_A4_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->DefineComponent( GRP_M1_4_A4_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->DefineComponent( GRP_M1_5_A4_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->DefineComponent( GRP_M1_6_A4_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->DefineComponent( GRP_M1_7_A4_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->DefineComponent( GRP_M1_8_A4_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->DefineComponent( GRP_M1_9_A4_VC, true, false, false, _7SD_STATE_NUM0 );
		pMissionTime->SetLocation( false );

		pEventTime->DefineMesh( mesh_index );
		pEventTime->DefineComponent( GRP_M2_1_A4_VC, true, false, false, _7SD_STATE_NUM0 );
		pEventTime->DefineComponent( GRP_M2_2_A4_VC, true, false, false, _7SD_STATE_NUM0 );
		pEventTime->DefineComponent( GRP_M2_3_A4_VC, true, false, false, _7SD_STATE_NUM0 );
		pEventTime->DefineComponent( GRP_M2_4_A4_VC, true, false, false, _7SD_STATE_NUM0 );
		pEventTime->SetLocation( false );
		return;
	}

	void PanelA4::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_A4, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_A4, 
			_V( -0.6351, 3.1074, 12.2873 ) + ofs, _V( -0.8416, 3.1074, 12.3431 ) + ofs, 
			_V( -0.6351, 2.9140, 12.2873 ) + ofs, _V( -0.8416, 2.9140, 12.3431 ) + ofs );
		return;
	}

	void PanelA4::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelA4::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "A4_Timer", 2 );
		pMissionTimer->ConnectPort( 1, pBundle, 0 );
		pMissionTimer->ConnectPort( 2, pBundle, 1 );

		AtlantisPanel::Realize();
		return;
	}
}
