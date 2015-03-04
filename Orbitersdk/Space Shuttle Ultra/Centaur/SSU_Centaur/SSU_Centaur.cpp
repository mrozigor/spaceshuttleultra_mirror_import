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
	separated = false;
	timer_ACS = 0;
	ENAtimer_ACS = false;
	timer_RL10 = 0;
	ENAtimer_RL10 = false;
	AdapterOffset = 0;
	AdapterMass = 0;
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

	if ((bFoundData == true) && (_strnicmp( pszBuffer, "G", 2 ) == 0))
	{
		// set G
		SetEmptyMass( G_EMPTY_MASS );
		phTank = CreatePropellantResource( G_RL10_PROPELLANT_MASS );
		// RL-10
		thRL10[0] = CreateThruster( RL10_C1_POS_G, RL10_C1_DIR, RL10_THRUST_VAC, phTank, RL10_ISP_VAC );
		thRL10[1] = CreateThruster( RL10_C2_POS_G, RL10_C2_DIR, RL10_THRUST_VAC, phTank, RL10_ISP_VAC );

		phACS = CreatePropellantResource( G_ACS_PROPELLANT_MASS );
		// quad I
		thACS[0] = CreateThruster( ACS_P1_POS_G, ACS_P1_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		thACS[1] = CreateThruster( ACS_Y1_POS_G, ACS_Y1_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		// quad II
		thACS[2] = CreateThruster( ACS_Y2_POS_G, ACS_Y2_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		thACS[3] = CreateThruster( ACS_S2A_POS_G, ACS_S2A_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		thACS[4] = CreateThruster( ACS_S2B_POS_G, ACS_S2B_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		thACS[5] = CreateThruster( ACS_P2_POS_G, ACS_P2_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		// quad III
		thACS[6] = CreateThruster( ACS_P3_POS_G, ACS_P3_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		thACS[7] = CreateThruster( ACS_Y3_POS_G, ACS_Y3_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		// quad IV
		thACS[8] = CreateThruster( ACS_Y4_POS_G, ACS_Y4_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		thACS[9] = CreateThruster( ACS_S4A_POS_G, ACS_S4A_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		thACS[10] = CreateThruster( ACS_S4B_POS_G, ACS_S4B_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		thACS[11] = CreateThruster( ACS_P4_POS_G, ACS_P4_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );

		hMesh = oapiLoadMeshGlobal( G_MESHNAME );
		SetSize( 20 );
		SetCrossSections( _V( 19.49, 18.88, 15.71 ) );
		SetPMI( _V( 4.32, 4.35, 2.69 ) );

		ahToPayload = CreateAttachment( false, _V( 0, 0, 2.8373 ), _V( 0, 0, 1 ), _V( 0, 1, 0 ), "SSU_CPL" );
		ahToCISS = CreateAttachment( true, _V( 0, 0, -0.5225 ), _V( 0, 0, -1 ), _V( 0, -1, 0 ), "SSU_CG" );
	}
	else// pszBuffer = "GPrime"
	{
		// set GPrime
		// default
		SetEmptyMass( GPRIME_EMPTY_MASS );
		phTank = CreatePropellantResource( GPRIME_RL10_PROPELLANT_MASS );
		// RL-10
		thRL10[0] = CreateThruster( RL10_C1_POS_GPRIME, RL10_C1_DIR, RL10_THRUST_VAC, phTank, RL10_ISP_VAC );
		thRL10[1] = CreateThruster( RL10_C2_POS_GPRIME, RL10_C2_DIR, RL10_THRUST_VAC, phTank, RL10_ISP_VAC );

		phACS = CreatePropellantResource( GPRIME_ACS_PROPELLANT_MASS );
		// quad I
		thACS[0] = CreateThruster( ACS_P1_POS_GPRIME, ACS_P1_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		thACS[1] = CreateThruster( ACS_Y1_POS_GPRIME, ACS_Y1_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		// quad II
		thACS[2] = CreateThruster( ACS_Y2_POS_GPRIME, ACS_Y2_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		thACS[3] = CreateThruster( ACS_S2A_POS_GPRIME, ACS_S2A_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		thACS[4] = CreateThruster( ACS_S2B_POS_GPRIME, ACS_S2B_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		thACS[5] = CreateThruster( ACS_P2_POS_GPRIME, ACS_P2_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		// quad III
		thACS[6] = CreateThruster( ACS_P3_POS_GPRIME, ACS_P3_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		thACS[7] = CreateThruster( ACS_Y3_POS_GPRIME, ACS_Y3_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		// quad IV
		thACS[8] = CreateThruster( ACS_Y4_POS_GPRIME, ACS_Y4_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		thACS[9] = CreateThruster( ACS_S4A_POS_GPRIME, ACS_S4A_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		thACS[10] = CreateThruster( ACS_S4B_POS_GPRIME, ACS_S4B_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );
		thACS[11] = CreateThruster( ACS_P4_POS_GPRIME, ACS_P4_DIR, ACS_THRUST_VAC, phACS, ACS_ISP_VAC );

		hMesh = oapiLoadMeshGlobal( GPRIME_MESHNAME );
		SetSize( 25 );
		SetCrossSections( _V( 30.84, 30.86, 15.74 ) );
		SetPMI( _V( 5.70, 5.66, 2.66 ) );

		ahToPayload = CreateAttachment( false, _V( 0, 0, 2.4542 ), _V( 0, 0, 1 ), _V( 0, 1, 0 ), "SSU_CPL" );
		ahToCISS = CreateAttachment( true, _V( 0, 0, -4.0439 ), _V( 0, 0, -1 ), _V( 0, -1, 0 ), "SSU_CGP" );
	}

	mesh_idx = AddMesh( hMesh );

	PARTICLESTREAMSPEC psRL10 = {
		0,
		0.3,
		25,
		50,
		0,
		0.12,
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

	PARTICLESTREAMSPEC psACS = {
		0,
		0.05,
		25,
		25,
		0,
		0.05,
		3,
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

	SetMaxThrust( ENGINE_RETRO, 0 );
	SetMaxThrust( ENGINE_HOVER, 0 );
	return;
}

void SSU_Centaur::clbkPreStep( double simt, double simdt, double mjd )
{
	if ((ENAtimer_ACS == true) && (simt >= timer_ACS))
	{
		THRUSTER_HANDLE thTEMP[4];
		thTEMP[0] = thACS[0];// P1
		thTEMP[1] = thACS[11];// P4
		CreateThrusterGroup( thTEMP, 2, THGROUP_ATT_PITCHUP );
		thTEMP[0] = thACS[5];// P2
		thTEMP[1] = thACS[6];// P3
		CreateThrusterGroup( thTEMP, 2, THGROUP_ATT_PITCHDOWN );
		thTEMP[0] = thACS[7];// Y3
		thTEMP[1] = thACS[8];// Y4
		CreateThrusterGroup( thTEMP, 2, THGROUP_ATT_YAWLEFT );
		thTEMP[0] = thACS[1];// Y1
		thTEMP[1] = thACS[2];// Y2
		CreateThrusterGroup( thTEMP, 2, THGROUP_ATT_YAWRIGHT );
		thTEMP[0] = thACS[1];// Y1
		thTEMP[1] = thACS[5];// P2
		thTEMP[2] = thACS[7];// Y3
		thTEMP[3] = thACS[11];// P4
		CreateThrusterGroup( thTEMP, 4, THGROUP_ATT_BANKLEFT );
		thTEMP[0] = thACS[0];// P1
		thTEMP[1] = thACS[2];// Y2
		thTEMP[2] = thACS[6];// P3
		thTEMP[3] = thACS[8];// Y4
		CreateThrusterGroup( thTEMP, 4, THGROUP_ATT_BANKRIGHT );
		thTEMP[0] = thACS[3];// S2A
		thTEMP[1] = thACS[4];// S2B
		thTEMP[2] = thACS[9];// S4A
		thTEMP[3] = thACS[10];// S4B
		CreateThrusterGroup( thTEMP, 4, THGROUP_ATT_FORWARD );

		ENAtimer_ACS = false;
	}

	if ((ENAtimer_RL10 == true) && (simt >= timer_RL10))
	{
		CreateThrusterGroup( thRL10, 2, THGROUP_MAIN );

		ENAtimer_RL10 = false;
	}

	if ((separated == false) && (GetAttachmentStatus( ahToCISS ) == NULL))
	{
		if (simt < 1)// makeshift first cycle check
		{
			// if "first" cycle and separated from CISS, assume it separated a long time ago and enable all engines without delay
			timer_ACS = simt;
			timer_RL10 = simt;
		}
		else
		{
			timer_ACS = simt + ACS_ENA_DELAY;
			timer_RL10 = simt + RL10_ENA_DELAY;
		}
		ENAtimer_ACS = true;
		ENAtimer_RL10 = true;
		separated = true;

		// add payload mass to centaur dry mass
		OBJHANDLE ohPL = GetAttachmentStatus( ahToPayload );
		if (ohPL)
		{
			VESSEL* vPL = oapiGetVesselInterface( ohPL );
			SetEmptyMass( GetEmptyMass() + vPL->GetMass() );
		}
	}
	return;
}

int SSU_Centaur::clbkConsumeBufferedKey( DWORD key, bool down, char* kstate )
{
	if (!down) return 0;

	if ((KEYMOD_SHIFT( kstate ) == false) && (KEYMOD_CONTROL( kstate ) == false) && (KEYMOD_ALT( kstate ) == false))// no key modifiers
	{
		if (key == OAPI_KEY_J)
		{
			if (GetAttachmentStatus( ahToCISS ) == NULL)// payload sep only after deployment
			{
				// remove payload mass from centaur dry mass
				OBJHANDLE ohPL = GetAttachmentStatus( ahToPayload );
				if (ohPL)
				{
					VESSEL* vPL = oapiGetVesselInterface( ohPL );
					SetEmptyMass( GetEmptyMass() - vPL->GetMass() );
				}
				// separate payload
				DetachChild( ahToPayload, 0.3 );
				return 1;
			}
		}
	}
	return 0;
}

void SSU_Centaur::clbkSaveState( FILEHANDLE scn )
{
	VESSEL2::clbkSaveState( scn );
	oapiWriteScenario_string( scn, "ADAPTER_MESH", (char*)AdapterMeshName.c_str() );
	oapiWriteScenario_float( scn, "ADAPTER_OFFSET", AdapterOffset );
	oapiWriteScenario_float( scn, "ADAPTER_MASS", AdapterMass );
	return;
}

void SSU_Centaur::clbkLoadStateEx( FILEHANDLE scn, void *status )
{
	char* line;

	while (oapiReadScenario_nextline( scn, line ))
	{
		if (!_strnicmp( line, "ADAPTER_MESH", 12 ))
		{
			AdapterMeshName = line + 13;
		}
		else if (!_strnicmp( line, "ADAPTER_OFFSET", 14 ))
		{
			sscanf( line + 14, "%lf", &AdapterOffset );
		}
		else if (!_strnicmp( line, "ADAPTER_MASS", 12 ))
		{
			sscanf( line + 12, "%lf", &AdapterMass );
		}
		else ParseScenarioLineEx( line, status );
	}

	if ((AdapterMeshName.length() > 0) && (AdapterOffset > 0) && (AdapterMass > 0))// small sanity checks
	{
		VECTOR3 pos;
		VECTOR3 dir;
		VECTOR3 rot;
		GetAttachmentParams( ahToPayload, pos, dir, rot );
		pos.z += AdapterOffset;

		// add mesh
		hAdapterMesh = oapiLoadMeshGlobal( AdapterMeshName.c_str() );
		mesh_Adapter_idx = AddMesh( hAdapterMesh, &pos );
		SetMeshVisibilityMode( mesh_Adapter_idx, MESHVIS_EXTERNAL | MESHVIS_VC | MESHVIS_EXTPASS );
		
		// correct spacecraft attachment
		SetAttachmentParams( ahToPayload, pos, dir, rot );

		// correct mass
		SetEmptyMass( GetEmptyMass() + AdapterMass );

		char buffer[128];
		sprintf_s( buffer, 128, "(Shuttle Centaur) added payload adapter: mesh:%s|height:%lf|mass:%lf", AdapterMeshName.c_str(), AdapterOffset, AdapterMass );
		oapiWriteLog( buffer );
	}
	return;
}
