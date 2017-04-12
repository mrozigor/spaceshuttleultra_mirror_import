#include "PanelO13.h"
#include "..\Atlantis.h"
#include "..\Atlantis_defs.h"
#include "..\CommonDefs.h"
#include "..\meshres_vc_o13.h"


namespace vc
{
	PanelO13::PanelO13( Atlantis* _sts ):AtlantisPanel( _sts, "O13" )
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELO13 );
		mesh_index = MESH_UNDEFINED;
	}

	PanelO13::~PanelO13()
	{
	}

	void PanelO13::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelO13::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelO13::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelO13::DefineVC()
	{
		AddAIDToMouseEventList( AID_O13 );
		return;
	}

	void PanelO13::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_O13, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_O13, 
			_V( -0.9374, 3.023, 13.226 ) + ofs, _V( -0.8098, 3.122, 13.226 ) + ofs, 
			_V( -0.9374, 3.023, 13.579 ) + ofs, _V( -0.8098, 3.122, 13.579 ) + ofs );
		return;
	}

	void PanelO13::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelO13::Realize()
	{
		AtlantisPanel::Realize();
		return;
	}
};