#include "PanelR13U.h"
#include "..\Atlantis.h"
#include "..\Atlantis_defs.h"
#include "..\CommonDefs.h"
#include "..\meshres_vc_r13u.h"


namespace vc
{
	PanelR13U::PanelR13U( Atlantis* _sts ):AtlantisPanel( _sts, "R13U" )
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELR13U );
		mesh_index = MESH_UNDEFINED;
	}

	PanelR13U::~PanelR13U()
	{
	}

	void PanelR13U::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelR13U::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelR13U::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelR13U::DefineVC()
	{
		AddAIDToMouseEventList( AID_R13U );
		return;
	}

	void PanelR13U::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_R13U, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_R13U, 
			_V( 1.54125, 2.43524, 12.561 ) + ofs, _V( 1.54125, 2.43524, 12.2564 ) + ofs, 
			_V( 1.30501, 2.28002, 12.561 ) + ofs, _V( 1.30501, 2.28002, 12.2564 ) + ofs );
		return;
	}

	void PanelR13U::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelR13U::Realize()
	{
		AtlantisPanel::Realize();
		return;
	}
};