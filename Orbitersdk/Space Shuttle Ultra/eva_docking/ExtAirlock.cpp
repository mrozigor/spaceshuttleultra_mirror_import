#include "ExtAirlock.h"
#include "..\meshres_ExtAL.h"
#include "..\CommonDefs.h"


namespace eva_docking
{
	ExtAirlock::ExtAirlock(AtlantisSubsystemDirector *pdirect, const std::string &_ident, bool HideTopCover ):BasicExternalAirlock(pdirect, _ident) 
	{
		fHatchState = 0.0;
		this->HideTopCover = HideTopCover;

		mesh_extal = MESH_UNDEFINED;
		hExtALMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_EXTAL );
		oapiWriteLog( "ExtAL mesh loaded" );
	}

	ExtAirlock::~ExtAirlock()
	{
	}

	void ExtAirlock::AddMeshes( const VECTOR3 &ofs )
	{
		if (mesh_extal == MESH_UNDEFINED)
		{
			VECTOR3 pos = _V( EXTERNAL_AIRLOCK_POS.x, EXTERNAL_AIRLOCK_POS.y, ofs.z );
			mesh_extal = STS()->AddMesh( hExtALMesh, &pos );
			oapiWriteLog( "ExtAL mesh added" );
		}
		STS()->SetMeshVisibilityMode( mesh_extal, MESHVIS_EXTERNAL | MESHVIS_VC | MESHVIS_EXTPASS );
		return;
	}

	void ExtAirlock::DefineAnimations(const VECTOR3& ofs) {
			//No animations
	}

	void ExtAirlock::VisualCreated( VISHANDLE vis )
	{
		if (HideTopCover == true)
		{
			MESHHANDLE hExtALTemplateMesh = STS()->GetMeshTemplate( mesh_extal );
			MESHGROUPEX* mg = oapiMeshGroupEx( hExtALTemplateMesh, GRP_OUTER_HATCH_FLAP_ExtAL );
			DEVMESHHANDLE hExtALDevMesh = STS()->GetDevMesh( vis, mesh_extal );
			
			GROUPEDITSPEC grpSpec;
			grpSpec.flags = GRPEDIT_SETUSERFLAG;
			grpSpec.UsrFlag = 2;
			oapiEditMeshGroup( hExtALDevMesh, GRP_OUTER_HATCH_FLAP_ExtAL, &grpSpec );
		}
		return;
	}
};