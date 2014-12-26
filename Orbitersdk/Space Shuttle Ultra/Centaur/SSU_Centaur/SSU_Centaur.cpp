#define ORBITER_MODULE


#include "SSU_Centaur.h"



DLLCLBK VESSEL *ovcInit( OBJHANDLE hVessel, int flightmodel )
{
	return new SSU_Centaur( hVessel );
}

DLLCLBK void ovcExit( VESSEL *Vessel )
{
	if (Vessel) delete (SSU_Centaur*)Vessel;
}

DLLCLBK void ovcSetState( VESSEL *Vessel, const VESSELSTATUS *Status )
{
	Vessel->DefSetState( Status );
}


SSU_Centaur::SSU_Centaur( OBJHANDLE hVessel ):VESSEL2( hVessel )
{
	enginesENA = false;
	return;
}

SSU_Centaur::~SSU_Centaur( void )
{
	return;
}

void SSU_Centaur::clbkSetClassCaps( FILEHANDLE cfg )
{
	char pszBuffer[255];
	bool bFoundData = oapiReadItem_string( cfg, "Type", pszBuffer );

	if (!bFoundData || !_strnicmp( pszBuffer, "GPrime", 6 ))
	{
		SetEmptyMass( GPRIME_EMPTY_MASS );
		phTank = CreatePropellantResource( GPRIME_RL10_PROPELLANT_MASS );
		hMesh = oapiLoadMeshGlobal( GPRIME_MESHNAME );
		SetSize( 25 );
		SetCrossSections( _V( 26.07, 25.9, 14.52 ) );
		SetPMI( _V( 6.74, 6.75, 2.20 ) );
		ahToPayload = CreateAttachment( false, _V( 0, 0, 3.38 ), _V( 0, 1, 0 ), _V( 0, 0, -1 ), "SSU_CENTAURPAYLOAD" );
		ahToCISS = CreateAttachment( true, _V( 0, 0, -2.5 ), _V( 0, 1, 0 ), _V( 0, 0, -1 ), "SSU_CISSCENTAUR" );
	}
	else if (!_strnicmp( pszBuffer, "G", 1 ))
	{
		SetEmptyMass( G_EMPTY_MASS );
		phTank = CreatePropellantResource( G_RL10_PROPELLANT_MASS );
		hMesh = oapiLoadMeshGlobal( G_MESHNAME );
		SetSize( 20 );
		SetCrossSections( _V( 19.09, 20.21, 14.23 ) );
		SetPMI( _V( 4.55, 5.04, 2.68 ) );
		ahToPayload = CreateAttachment( false, _V( 0, 0, 1.95 ), _V( 0, 1, 0 ), _V( 0, 0, -1 ), "SSU_CENTAURPAYLOAD" );
		ahToCISS = CreateAttachment( true, _V( 0, 0, -2.659 ), _V( 0, 1, 0 ), _V( 0, 0, -1 ), "SSU_CISSCENTAUR" );
	}

	mesh_idx = AddMesh( hMesh );

	phACS = CreatePropellantResource( ACS_PROPELLANT_MASS );
	thACS[0] = CreateThruster( ACS_1_POS, ACS_1_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
	thACS[1] = CreateThruster( ACS_2_POS, ACS_2_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
	thACS[2] = CreateThruster( ACS_3_POS, ACS_3_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
	thACS[3] = CreateThruster( ACS_4_POS, ACS_4_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
	thACS[4] = CreateThruster( ACS_5_POS, ACS_5_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
	thACS[5] = CreateThruster( ACS_6_POS, ACS_6_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
	thACS[6] = CreateThruster( ACS_7_POS, ACS_7_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
	thACS[7] = CreateThruster( ACS_8_POS, ACS_8_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
	thACS[8] = CreateThruster( ACS_9_POS, ACS_9_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
	thACS[9] = CreateThruster( ACS_10_POS, ACS_10_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
	thACS[10] = CreateThruster( ACS_11_POS, ACS_11_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
	thACS[11] = CreateThruster( ACS_12_POS, ACS_12_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );

	PARTICLESTREAMSPEC psACS = {
		0,
		0.05,
		25,
		25,
		0,
		0.1,
		5,
		10,
		PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_SQRT,
		0, 1,
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0};

	AddExhaustStream( thACS[0], &psACS );
	AddExhaustStream( thACS[1], &psACS );
	AddExhaustStream( thACS[2], &psACS );
	AddExhaustStream( thACS[3], &psACS );
	AddExhaustStream( thACS[4], &psACS );
	AddExhaustStream( thACS[5], &psACS );
	AddExhaustStream( thACS[6], &psACS );
	AddExhaustStream( thACS[7], &psACS );
	AddExhaustStream( thACS[8], &psACS );
	AddExhaustStream( thACS[9], &psACS );
	AddExhaustStream( thACS[10], &psACS );
	AddExhaustStream( thACS[11], &psACS );

	thRL10[0] = CreateThruster( RL10_L_POS, RL10_L_DIR, RL10_THRUST_VAC, phTank, RL10_ISP_VAC );
	thRL10[1] = CreateThruster( RL10_R_POS, RL10_R_DIR, RL10_THRUST_VAC, phTank, RL10_ISP_VAC );

	PARTICLESTREAMSPEC psRL10 = {
		0,
		0.3,
		25,
		50,
		0,
		0.2,
		20,
		5,
		PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_SQRT,
		0, 1,
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0};

	AddExhaustStream( thRL10[0], &psRL10 );
	AddExhaustStream( thRL10[1], &psRL10 );

	SetMaxThrust( ENGINE_RETRO, 0 );
	SetMaxThrust( ENGINE_HOVER, 0 );
	return;
}

void SSU_Centaur::clbkPreStep( double simt, double simdt, double mjd )
{
	if ((GetAttachmentStatus( ahToCISS ) == NULL) && (enginesENA == false))
	{
		// enable engines only after deployment
		CreateThrusterGroup( thRL10, 2, THGROUP_MAIN );

		THRUSTER_HANDLE thTEMP[4];
		thTEMP[0] = thACS[1];
		thTEMP[1] = thACS[2];
		CreateThrusterGroup( thTEMP, 2, THGROUP_ATT_PITCHUP );
		thTEMP[0] = thACS[7];
		thTEMP[1] = thACS[8];
		CreateThrusterGroup( thTEMP, 2, THGROUP_ATT_PITCHDOWN );
		thTEMP[0] = thACS[0];
		thTEMP[1] = thACS[11];
		CreateThrusterGroup( thTEMP, 2, THGROUP_ATT_YAWLEFT );
		thTEMP[0] = thACS[5];
		thTEMP[1] = thACS[6];
		CreateThrusterGroup( thTEMP, 2, THGROUP_ATT_YAWRIGHT );
		thTEMP[0] = thACS[1];
		thTEMP[1] = thACS[7];
		CreateThrusterGroup( thTEMP, 2, THGROUP_ATT_BANKLEFT );
		thTEMP[0] = thACS[2];
		thTEMP[1] = thACS[8];
		CreateThrusterGroup( thTEMP, 2, THGROUP_ATT_BANKRIGHT );
		thTEMP[0] = thACS[3];
		thTEMP[1] = thACS[4];
		thTEMP[2] = thACS[9];
		thTEMP[3] = thACS[10];
		CreateThrusterGroup( thTEMP, 4, THGROUP_ATT_FORWARD );

		enginesENA = true;
	}
	return;
}

int SSU_Centaur::clbkConsumeBufferedKey( DWORD key, bool down, char* keystate )
{
	if (!down) return 0;

	if ((key == OAPI_KEY_J) && (GetAttachmentStatus( ahToCISS ) == NULL))
	{
		// payload sep only after deployment
		DetachChild( ahToPayload, 0.3 );
		return 1;
	}
	return 0;
}
