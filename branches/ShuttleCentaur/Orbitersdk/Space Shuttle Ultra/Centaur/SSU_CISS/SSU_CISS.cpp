#define ORBITER_MODULE


#include "SSU_CISS.h"


DLLCLBK VESSEL *ovcInit( OBJHANDLE hVessel, int flightmodel )
{
	return new SSU_CISS( hVessel );
}

DLLCLBK void ovcExit( VESSEL *Vessel )
{
	if (Vessel) delete (SSU_CISS*)Vessel;
}

DLLCLBK void ovcSetState( VESSEL *Vessel, const VESSELSTATUS *Status )
{
	Vessel->DefSetState( Status );
}


SSU_CISS::SSU_CISS( OBJHANDLE hVessel ):VESSEL2( hVessel )
{
	return;
}

SSU_CISS::~SSU_CISS( void )
{
	return;
}

void SSU_CISS::clbkSetClassCaps( FILEHANDLE cfg )
{
	char pszBuffer[255];
	bool bFoundData = oapiReadItem_string( cfg, "Type", pszBuffer );

	if (!bFoundData || !_strnicmp( pszBuffer, "GPrime", 6 ))
	{
		SetEmptyMass( GPRIME_EMPTY_MASS );
		hMesh = oapiLoadMeshGlobal( GPRIME_MESHNAME );
		ahToOV = CreateAttachment( true, _V( 0, -2.35, 0.5 ), _V( 0, -1, 0 ), _V( 0, 0, 1 ), "SSU_OVCISS" );
		ahToCentaur = CreateAttachment( false, _V( 0, -0.03, -1.16 ), _V( 0, -1, 0 ), _V( 0, 0, -1 ), "SSU_CISSCENTAUR" );
	}
	else if (!_strnicmp( pszBuffer, "G", 1 ))
	{
		SetEmptyMass( G_EMPTY_MASS );
		hMesh = oapiLoadMeshGlobal( G_MESHNAME );
		ahToOV = CreateAttachment( true, _V( 0, -2.3, -2.3 ), _V( 0, -1, 0 ), _V( 0, 0, 1 ), "SSU_OVCISS" );
		ahToCentaur = CreateAttachment( false, _V( 0, 0, 5 ), _V( 0, -1, 0 ), _V( 0, 0, -1 ), "SSU_CISSCENTAUR" );
	}

	mesh_idx = AddMesh( hMesh );

	SetMaxThrust( ENGINE_MAIN, 0 );
	SetMaxThrust( ENGINE_RETRO, 0 );
	SetMaxThrust( ENGINE_HOVER, 0 );
	SetMaxThrust( ENGINE_ATTITUDE, 0 );

	SetSize( 30 );
	SetCrossSections( _V( 30.30, 31.63, 14.41 ) );
	SetPMI( _V( 6.56, 6.86, 2.44 ) );
	return;
}

int SSU_CISS::clbkConsumeBufferedKey( DWORD key, bool down, char* keystate )
{
	if (!down) return 0;

	switch (key)
	{
		case OAPI_KEY_G:
			// TODO raise table
			return 1;
		case OAPI_KEY_L:
			// TODO lower table
			return 1;
		case OAPI_KEY_J:
			// sep centaur
			// TODO add table position checks
			DetachChild( ahToCentaur, 0.3 );
			return 1;
	}
	return 0;
}
