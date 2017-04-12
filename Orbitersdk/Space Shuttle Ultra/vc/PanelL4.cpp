#include "PanelL4.h"
#include "..\Atlantis.h"
#include "..\Atlantis_defs.h"
#include "..\CommonDefs.h"
#include "..\meshres_vc_l4.h"


namespace vc
{
	PanelL4::PanelL4( Atlantis* _sts ):AtlantisPanel( _sts, "L4" )
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELL4 );
		mesh_index = MESH_UNDEFINED;
	}

	PanelL4::~PanelL4()
	{
	}

	void PanelL4::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelL4::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelL4::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelL4::DefineVC()
	{
		AddAIDToMouseEventList( AID_L4 );
		return;
	}

	void PanelL4::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_L4, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_L4, 
			_V( -1.44342, 2.60918, 13.8059 ) + ofs, _V( -1.44342, 1.95123, 13.8059 ) + ofs, 
			_V( -1.15403, 2.60918, 13.8059 ) + ofs, _V( -1.15403, 1.95123, 13.8059 ) + ofs );
		return;
	}

	void PanelL4::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelL4::Realize()
	{
		AtlantisPanel::Realize();
		return;
	}
};