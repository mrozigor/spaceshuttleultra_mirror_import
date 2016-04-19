#include "TunnelAdapterAssembly.h"
#include "..\Atlantis.h"
#include "..\CommonDefs.h"


namespace eva_docking
{
	TunnelAdapterAssembly::TunnelAdapterAssembly( AtlantisSubsystemDirector* _director, bool AftTAA ):AtlantisSubsystem( _director, "TunnelAdapterAssembly" )
	{
		mesh_idx = MESH_UNDEFINED;

		if (AftTAA)
		{
			hMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_TAA_AFT );
			oapiWriteLog( "aft TAA mesh loaded" );
		}
		else
		{
			hMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_TAA_FORWARD );
			oapiWriteLog( "forward TAA mesh loaded" );
		}
		return;
	}

	TunnelAdapterAssembly::~TunnelAdapterAssembly( void )
	{
	}

	void TunnelAdapterAssembly::AddMeshes( const VECTOR3 &ofs )
	{
		if (mesh_idx == MESH_UNDEFINED)
		{
			VECTOR3 pos = _V( TAA_POS.x, TAA_POS.y, ofs.z );
			mesh_idx = STS()->AddMesh( hMesh, &pos );
			oapiWriteLog( "TAA mesh added" );
		}
		STS()->SetMeshVisibilityMode( mesh_idx, MESHVIS_EXTERNAL | MESHVIS_VC | MESHVIS_EXTPASS );
		return;
	}

	void TunnelAdapterAssembly::DefineAnimations( const VECTOR3& ofs )
	{
		return;
	}
};