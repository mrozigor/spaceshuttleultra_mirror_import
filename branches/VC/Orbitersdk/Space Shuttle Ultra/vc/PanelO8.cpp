#include "PanelO8.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"
#include "..\CommonDefs.h"
#include "..\meshres_vc_o8.h"


namespace vc
{
	PanelO8::PanelO8( Atlantis* psts ):AtlantisPanel( psts, "O8" )
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELO8 );
		mesh_index = MESH_UNDEFINED;

		Add( pRadarAltimeter[0] = new StdSwitch2( psts, "Radar Altimeter 1" ) );
		pRadarAltimeter[0]->SetLabel( 0, "OFF" );
		pRadarAltimeter[0]->SetLabel( 1, "ON" );

		Add( pRadarAltimeter[1] = new StdSwitch2( psts, "Radar Altimeter 2" ) );
		pRadarAltimeter[1]->SetLabel( 0, "OFF" );
		pRadarAltimeter[1]->SetLabel( 1, "ON" );

		Add( pAnnunciatorLampTest = new StdSwitch3( psts, "Annunciator Lamp Test" ) );
	}

	PanelO8::~PanelO8()
	{
	}

	void PanelO8::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelO8::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelO8::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelO8::DefineVC()
	{
		VECTOR3 switch_rot = _V( 1, 0, 0 );

		AddAIDToMouseEventList( AID_O8 );

		pRadarAltimeter[0]->SetInitialAnimState( 0.5f );
		pRadarAltimeter[0]->DefineSwitchGroup( GRP_S4_O8_VC );
		pRadarAltimeter[0]->SetReference( _V( 0.2651, 3.1652, 13.6946 ), switch_rot );
		pRadarAltimeter[0]->SetMouseRegion( 0.089737f, 0.106703f, 0.135184f, 0.140805f );

		pRadarAltimeter[1]->SetInitialAnimState( 0.5f );
		pRadarAltimeter[1]->DefineSwitchGroup( GRP_S5_O8_VC );
		pRadarAltimeter[1]->SetReference( _V( 0.2651, 3.1652, 13.6946 ), switch_rot );
		pRadarAltimeter[1]->SetMouseRegion( 0.157729f, 0.106310f, 0.201778f, 0.140387f );

		pAnnunciatorLampTest->SetInitialAnimState( 0.5f );
		pAnnunciatorLampTest->DefineSwitchGroup( GRP_S18_O8_VC );
		pAnnunciatorLampTest->SetReference( _V( 0.6000, 3.0978, 13.9709 ), switch_rot );
		pAnnunciatorLampTest->SetMouseRegion( 0.775565f, 0.498687f, 0.814904f, 0.530986f );
		pAnnunciatorLampTest->SetSpringLoaded( true, 0 );
		pAnnunciatorLampTest->SetSpringLoaded( true, 2 );
	}

	void PanelO8::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelO8::Realize()
	{
		discsignals::DiscreteBundle* pBundle = pBundle = STS()->BundleManager()->CreateBundle( "RDR_ALT", 16 );
		pRadarAltimeter[0]->output.Connect( pBundle, 0 );
		pRadarAltimeter[1]->output.Connect( pBundle, 1 );

		pBundle = STS()->BundleManager()->CreateBundle( "ACA", 16 );
		pAnnunciatorLampTest->ConnectPort( 2, pBundle, 8 );
		pAnnunciatorLampTest->ConnectPort( 1, pBundle, 9 );

		AtlantisPanel::Realize();
	}

	void PanelO8::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_O8, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_O8, 
			_V( 0.202423, 3.1858, 13.6083 ) + ofs, _V( 0.710853, 3.1858, 13.6083 ) + ofs, 
			_V( 0.202423, 3.01485, 14.3133 ) + ofs, _V( 0.710853, 3.01474, 14.3133 ) + ofs );
	}

};
