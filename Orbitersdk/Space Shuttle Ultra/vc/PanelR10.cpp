#include "PanelR10.h"
#include "..\Atlantis.h"
#include "..\Atlantis_defs.h"
#include "..\CommonDefs.h"
#include "..\meshres_vc_r10.h"


namespace vc
{
	PanelR10::PanelR10( Atlantis* _sts ):AtlantisPanel( _sts, "R10" )
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELR10 );
		mesh_index = MESH_UNDEFINED;
	}

	PanelR10::~PanelR10()
	{
	}

	void PanelR10::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelR10::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelR10::GetVCMeshIndex( void ) const
	{
		return mesh_index;
	}

	void PanelR10::DefineVC()
	{
		AddAIDToMouseEventList( AID_R10 );
		return;
	}

	void PanelR10::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea( AID_R10, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral( AID_R10, 
			_V( 1.65494, 2.50958, 12.8038 ) + ofs, _V( 1.69245, 2.53394, 12.2564 ) + ofs, 
			_V( 1.53864, 2.43406, 12.8038 ) + ofs, _V( 1.53913, 2.43437, 12.2564 ) + ofs );
		return;
	}

	void PanelR10::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelR10::Realize()
	{
		AtlantisPanel::Realize();
		return;
	}
};