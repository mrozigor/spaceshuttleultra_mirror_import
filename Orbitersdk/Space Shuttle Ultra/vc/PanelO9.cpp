#include "PanelO9.h"
#include "..\Atlantis.h"
#include "..\Atlantis_defs.h"
#include "..\CommonDefs.h"
#include "..\meshres_vc_o9.h"


namespace vc
{
	PanelO9::PanelO9( Atlantis* _sts ):AtlantisPanel( _sts, "O9" )
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELO9 );
		mesh_index = MESH_UNDEFINED;
	}

	PanelO9::~PanelO9()
	{
	}

	void PanelO9::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelO9::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelO9::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelO9::DefineVC()
	{
		AddAIDToMouseEventList( AID_O9 );
		return;
	}

	void PanelO9::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_O9, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_O9, 
			_V( 0.759093, 2.98093, 14.2904 ) + ofs, _V( 0.84038, 2.98599, 13.912 ) + ofs, 
			_V( 0.866101, 2.86819, 14.3114 ) + ofs, _V( 0.946387, 2.87325, 13.933 ) + ofs );
		return;
	}

	void PanelO9::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelO9::Realize()
	{
		AtlantisPanel::Realize();
		return;
	}
};