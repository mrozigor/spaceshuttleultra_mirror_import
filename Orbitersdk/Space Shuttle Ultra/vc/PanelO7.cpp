#include "PanelO7.h"
#include "..\Atlantis.h"
#include "..\Atlantis_defs.h"
#include "..\CommonDefs.h"
#include "..\meshres_vc_o7.h"


namespace vc
{
	PanelO7::PanelO7( Atlantis* _sts ):AtlantisPanel( _sts, "O7" )
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELO7 );
		mesh_index = MESH_UNDEFINED;
	}

	PanelO7::~PanelO7()
	{
	}

	void PanelO7::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelO7::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelO7::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelO7::DefineVC()
	{
		AddAIDToMouseEventList( AID_O7 );
		return;
	}

	void PanelO7::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_O7, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_O7, 
			_V( -0.217782, 3.1858, 13.6081 ) + ofs, _V( 0.202397, 3.1858, 13.6081 ) + ofs, 
			_V( -0.217782, 3.01494, 14.3132 ) + ofs, _V( 0.202397, 3.01485, 14.3131 ) + ofs );
		return;
	}

	void PanelO7::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelO7::Realize()
	{
		AtlantisPanel::Realize();
		return;
	}
};