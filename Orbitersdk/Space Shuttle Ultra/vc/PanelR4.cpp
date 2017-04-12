#include "PanelR4.h"
#include "..\Atlantis.h"
#include "..\Atlantis_defs.h"
#include "..\CommonDefs.h"
#include "..\meshres_vc_r4.h"


namespace vc
{
	PanelR4::PanelR4( Atlantis* _sts ):AtlantisPanel( _sts, "R4" )
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELR4 );
		mesh_index = MESH_UNDEFINED;
	}

	PanelR4::~PanelR4()
	{
	}

	void PanelR4::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelR4::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelR4::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelR4::DefineVC()
	{
		AddAIDToMouseEventList( AID_R4 );
		return;
	}

	void PanelR4::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_R4, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_R4, 
			_V( 1.4982, 1.87836, 13.8054 ) + ofs, _V( 1.4982, 2.22704, 13.8054 ) + ofs, 
			_V( 1.14952, 1.87836, 13.8054 ) + ofs, _V( 1.14952, 2.22704, 13.8054 ) + ofs );
		return;
	}

	void PanelR4::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelR4::Realize()
	{
		AtlantisPanel::Realize();
		return;
	}
};