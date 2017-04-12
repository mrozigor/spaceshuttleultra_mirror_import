#include "PanelF9.h"
#include "..\Atlantis.h"
#include "..\Atlantis_defs.h"
#include "..\CommonDefs.h"
#include "..\meshres_vc_f9.h"


namespace vc
{
	PanelF9::PanelF9( Atlantis* _sts ):AtlantisPanel( _sts, "F9" )
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELF9 );
		mesh_index = MESH_UNDEFINED;
	}

	PanelF9::~PanelF9()
	{
	}

	void PanelF9::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelF9::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelF9::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelF9::DefineVC()
	{
		AddAIDToMouseEventList( AID_F9 );
		return;
	}

	void PanelF9::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_F9, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_F9, 
			_V( 0.949898, 2.15454, 14.7274 ) + ofs, _V( 1.17864, 2.15454, 14.7274 ) + ofs, 
			_V( 0.951728, 1.83612, 14.6439 ) + ofs, _V( 1.12191, 1.88919, 14.6594 ) + ofs );
		return;
	}

	void PanelF9::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelF9::Realize()
	{
		AtlantisPanel::Realize();
		return;
	}
};