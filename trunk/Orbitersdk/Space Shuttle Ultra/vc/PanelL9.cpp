#include "PanelL9.h"
#include "..\Atlantis.h"
#include "..\Atlantis_defs.h"
#include "..\CommonDefs.h"
#include "..\meshres_vc_l9.h"


namespace vc
{
	PanelL9::PanelL9( Atlantis* _sts ):AtlantisPanel( _sts, "L9" )
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELL9 );
		mesh_index = MESH_UNDEFINED;
	}

	PanelL9::~PanelL9()
	{
	}

	void PanelL9::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelL9::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelL9::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelL9::DefineVC()
	{
		AddAIDToMouseEventList( AID_L9 );
		return;
	}

	void PanelL9::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_L9, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_L9, 
			_V( -1.6965, 2.53376, 12.2204 ) + ofs, _V( -1.66009, 2.51012, 12.8038 ) + ofs, 
			_V( -1.54444, 2.43502, 12.2204 ) + ofs, _V( -1.5438, 2.4346, 12.8038 ) + ofs );
		return;
	}

	void PanelL9::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelL9::Realize()
	{
		AtlantisPanel::Realize();
		return;
	}
};