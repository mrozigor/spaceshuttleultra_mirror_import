#include "PanelO5.h"
#include "..\Atlantis.h"
#include "..\Atlantis_defs.h"
#include "..\CommonDefs.h"
#include "..\meshres_vc_o5.h"


namespace vc
{
	PanelO5::PanelO5( Atlantis* _sts ):AtlantisPanel( _sts, "O5" )
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELO5 );
		mesh_index = MESH_UNDEFINED;
	}

	PanelO5::~PanelO5()
	{
	}

	void PanelO5::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelO5::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelO5::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelO5::DefineVC()
	{
		AddAIDToMouseEventList( AID_O5 );
		return;
	}

	void PanelO5::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_O5, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_O5, 
			_V( -0.8451, 2.98599, 13.912 ) + ofs, _V( -0.76482, 2.98093, 14.2904 ) + ofs, 
			_V( -0.95111, 2.87325, 13.933 ) + ofs, _V( -0.87083, 2.86819, 14.3114 ) + ofs );
		return;
	}

	void PanelO5::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelO5::Realize()
	{
		AtlantisPanel::Realize();
		return;
	}
};