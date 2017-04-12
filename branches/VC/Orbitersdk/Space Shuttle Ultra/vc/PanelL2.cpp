#include "PanelL2.h"
#include "..\Atlantis.h"
#include "..\Atlantis_defs.h"
#include "..\CommonDefs.h"
#include "..\meshres_vc_l2.h"


namespace vc
{
	PanelL2::PanelL2( Atlantis* _sts ):AtlantisPanel( _sts, "L2" )
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELL2 );
		mesh_index = MESH_UNDEFINED;
	}

	PanelL2::~PanelL2()
	{
	}

	void PanelL2::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelL2::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelL2::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelL2::DefineVC()
	{
		AddAIDToMouseEventList( AID_L2 );
		return;
	}

	void PanelL2::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_L2, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_L2, 
			_V( -1.12764, 1.88993, 14.66 ) + ofs, _V( -0.957655, 1.83991, 14.6465 ) + ofs, 
			_V( -1.45253, 1.87544, 13.8055 ) + ofs, _V( -0.957266, 1.72869, 13.8047 ) + ofs );
		return;
	}

	void PanelL2::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelL2::Realize()
	{
		AtlantisPanel::Realize();
		return;
	}
};