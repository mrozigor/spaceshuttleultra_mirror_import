#define ORBITER_MODULE


#include "SSU_Centaur.h"
#include "meshres_Centaur_G.h"
#include "meshres_Centaur_GPrime.h"



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


SSU_Centaur::SSU_Centaur( OBJHANDLE hVessel ):VESSEL3( hVessel )
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
		thRL10[0] = CreateThruster( RL10_C1_POS_G, RL10_C1_DIR, RL10A33B_THRUST_VAC, phTank, RL10A33B_ISP_VAC );
		thRL10[1] = CreateThruster( RL10_C2_POS_G, RL10_C2_DIR, RL10A33B_THRUST_VAC, phTank, RL10A33B_ISP_VAC );

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
		mesh_idx = AddMesh( hMesh );
		SetSize( 20 );
		SetCrossSections( _V( 19.49, 18.88, 15.71 ) );
		SetPMI( _V( 4.32, 4.35, 2.69 ) );

		ahToPayload = CreateAttachment( false, _V( 0, 0, 3.3602 ), _V( 0, 0, 1 ), _V( 0, 1, 0 ), "SSU_CPL" );
		ahToCISS = CreateAttachment( true, _V( 0, 0, -0.4703 ), _V( 0, 0, -1 ), _V( 1, 0, 0 ), "SSU_CG" );

		DefineGAnimations();
	}
	else// pszBuffer = "GPrime"
	{
		// set GPrime
		// default
		SetEmptyMass( GPRIME_EMPTY_MASS );
		phTank = CreatePropellantResource( GPRIME_RL10_PROPELLANT_MASS );
		// RL-10
		thRL10[0] = CreateThruster( RL10_C1_POS_GPRIME, RL10_C1_DIR, RL10A33A_THRUST_VAC, phTank, RL10A33A_ISP_VAC );
		thRL10[1] = CreateThruster( RL10_C2_POS_GPRIME, RL10_C2_DIR, RL10A33A_THRUST_VAC, phTank, RL10A33A_ISP_VAC );

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
		mesh_idx = AddMesh( hMesh );
		SetSize( 25 );
		SetCrossSections( _V( 30.84, 30.86, 15.74 ) );
		SetPMI( _V( 5.70, 5.66, 2.66 ) );

		ahToPayload = CreateAttachment( false, _V( 0, 0, 2.5617 ), _V( 0, 0, 1 ), _V( 0, 1, 0 ), "SSU_CPL" );
		ahToCISS = CreateAttachment( true, _V( 0, 0, -3.9864 ), _V( 0, 0, -1 ), _V( 1, 0, 0 ), "SSU_CGP" );

		DefineGPrimeAnimations();
	}

	PARTICLESTREAMSPEC psRL10 = {
		0,
		0.4,
		25,
		50,
		0.5,
		0.12,
		22,
		5,
		PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_SQRT,
		0, 1,
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0};
	AddExhaustStream( thRL10[0], &psRL10 );
	AddExhaust( thRL10[0], 3, 0.6, 0.5 );
	AddExhaustStream( thRL10[1], &psRL10 );
	AddExhaust( thRL10[1], 3, 0.6, 0.5 );

	PARTICLESTREAMSPEC psACS = {
		0,
		0.01,
		100,
		3,
		0,
		0.2,
		1.5,
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

			asANTENNA.pos = 1.0;
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

		// deploy antennas
		asANTENNA.action = AnimState::OPENING;
	}

	if (asANTENNA.Opening() == true) asANTENNA.Move( simdt / (ANTENNA_DEPLOY_DELAY + (1 / ANTENNA_DEPLOY_RATE)) );
	return;
}

void SSU_Centaur::DefineGAnimations( void )
{
	double animstart = ANTENNA_DEPLOY_DELAY / (ANTENNA_DEPLOY_DELAY + (1 / ANTENNA_DEPLOY_RATE));

	static UINT ANTENNA1Grp[1] = {GRP_QUAD1_ANTENNA_G};
	static MGROUP_ROTATE ANTENNA1( mesh_idx, ANTENNA1Grp, 1, _V( -0.0782, 2.1877, 2.1566 ), _V( 0, 0, 1 ), static_cast<float>(90.0*RAD) );
	anim_ANTENNA = CreateAnimation( 0.0 );
	AddAnimationComponent( anim_ANTENNA, animstart, 1, &ANTENNA1 );

	static UINT ANTENNA2Grp[1] = {GRP_QUAD2_ANTENNA_G};
	static MGROUP_ROTATE ANTENNA2( mesh_idx, ANTENNA2Grp, 1, _V( -0.0782, -2.1877, 2.1566 ), _V( 0, 0, -1 ), static_cast<float>(90.0*RAD) );
	AddAnimationComponent( anim_ANTENNA, animstart, 1, &ANTENNA2 );

	asANTENNA.Set( AnimState::CLOSED, 0.0 );
	SetAnimation( anim_ANTENNA, asANTENNA.pos );

	RegisterAnimation();
}

void SSU_Centaur::DefineGPrimeAnimations( void )
{
	double animstart = ANTENNA_DEPLOY_DELAY / (ANTENNA_DEPLOY_DELAY + (1 / ANTENNA_DEPLOY_RATE));

	static UINT ANTENNA1Grp[1] = {GRP_QUAD1_ANTENNA_GPRIME};
	static MGROUP_ROTATE ANTENNA1( mesh_idx, ANTENNA1Grp, 1, _V( -0.0782, 2.1877, 1.3591 ), _V( 0, 0, 1 ), static_cast<float>(90.0*RAD) );
	anim_ANTENNA = CreateAnimation( 0.0 );
	AddAnimationComponent( anim_ANTENNA, animstart, 1, &ANTENNA1 );

	static UINT ANTENNA2Grp[1] = {GRP_QUAD2_ANTENNA_GPRIME};
	static MGROUP_ROTATE ANTENNA2( mesh_idx, ANTENNA2Grp, 1, _V( -0.0782, -2.1877, 1.3591 ), _V( 0, 0, -1 ), static_cast<float>(90.0*RAD) );
	AddAnimationComponent( anim_ANTENNA, animstart, 1, &ANTENNA2 );

	asANTENNA.Set( AnimState::CLOSED, 0.0 );
	SetAnimation( anim_ANTENNA, asANTENNA.pos );

	RegisterAnimation();
}

bool SSU_Centaur::clbkDrawHUD( int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp )
{
	char cbuf[64];
	sprintf_s( cbuf, 64, "ACS available: %.2fKg", GetPropellantMass( phACS ) );
	skp->Text( (int)(hps->W * 0.01), (int)(hps->H * 0.3), cbuf, strlen( cbuf ) );

	if (ENAtimer_RL10)
	{
		sprintf_s( cbuf, 64, "RL-10 inhibits removed in: %.0fs", timer_RL10 - oapiGetSimTime() );
		skp->Text( (int)(hps->W * 0.01), (int)(hps->H * 0.33), cbuf, strlen( cbuf ) );
	}

	if (ENAtimer_ACS)
	{
		sprintf_s( cbuf, 64, "ACS inhibits removed in: %.0fs", timer_ACS - oapiGetSimTime() );
		skp->Text( (int)(hps->W * 0.01), (int)(hps->H * 0.36), cbuf, strlen( cbuf ) );
	}
	return true;
}


int SSU_Centaur::clbkConsumeBufferedKey( DWORD key, bool down, char* kstate )
{
	if (!down) return 0;

	if ((KEYMOD_SHIFT( kstate ) == false) && (KEYMOD_CONTROL( kstate ) == true) && (KEYMOD_ALT( kstate ) == false))// only CTRL key modifier
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

void SSU_Centaur::clbkAnimate( double simt )
{
	SetAnimation( anim_ANTENNA, asANTENNA.pos );
}

void SSU_Centaur::clbkVisualCreated( VISHANDLE vis, int refcount )
{
	MainExternalMeshVisual = GetMesh(vis, 0);

	SetAnimation( anim_ANTENNA, asANTENNA.pos );
}

void SSU_Centaur::clbkVisualDestroyed( VISHANDLE vis, int refcount )
{
	MainExternalMeshVisual = 0;
}

void SSU_Centaur::clbkSaveState( FILEHANDLE scn )
{
	VESSEL2::clbkSaveState( scn );
	
	if ((AdapterMeshName.length() > 0) && (AdapterOffset > 0) && (AdapterMass > 0))// only save if used
	{
		oapiWriteScenario_string( scn, "ADAPTER_MESH", (char*)AdapterMeshName.c_str() );
		oapiWriteScenario_float( scn, "ADAPTER_OFFSET", AdapterOffset );
		oapiWriteScenario_float( scn, "ADAPTER_MASS", AdapterMass );
	}

	return;
}

void SSU_Centaur::clbkLoadStateEx( FILEHANDLE scn, void *status )
{
	char* line;
	bool adapterON = false;

	while (oapiReadScenario_nextline( scn, line ))
	{
		if (!_strnicmp( line, "ADAPTER_MESH", 12 ))
		{
			AdapterMeshName = line + 13;
			adapterON = true;
		}
		else if (!_strnicmp( line, "ADAPTER_OFFSET", 14 ))
		{
			sscanf( line + 14, "%lf", &AdapterOffset );
			adapterON = true;
		}
		else if (!_strnicmp( line, "ADAPTER_MASS", 12 ))
		{
			sscanf( line + 12, "%lf", &AdapterMass );
			adapterON = true;
		}
		else ParseScenarioLineEx( line, status );
	}


	if (adapterON)
	{
		// small sanity checks
		if (AdapterMeshName.length() == 0)
		{
			oapiWriteLog( "(Shuttle Centaur) ERROR: ADAPTER_MESH was not specified" );
			return;
		}

		if (AdapterOffset <= 0)
		{
			oapiWriteLog( "(Shuttle Centaur) ERROR: ADAPTER_OFFSET was not specified or isn't greater than 0" );
			return;
		}

		if (AdapterMass <= 0)
		{
			oapiWriteLog( "(Shuttle Centaur) ERROR: ADAPTER_MASS was not specified or isn't greater than 0" );
			return;
		}
		
		char buffer[128];
		VECTOR3 pos;
		VECTOR3 dir;
		VECTOR3 rot;
		GetAttachmentParams( ahToPayload, pos, dir, rot );

		// add mesh
		hAdapterMesh = oapiLoadMeshGlobal( AdapterMeshName.c_str() );
		if (hAdapterMesh == NULL)
		{
			sprintf_s( buffer, 128, "(Shuttle Centaur) ERROR: ADAPTER_MESH file not found" );
			oapiWriteLog( buffer );
			return;
		}
		SetMeshVisibilityMode( AddMesh( hAdapterMesh, &pos ), MESHVIS_EXTERNAL | MESHVIS_EXTPASS );
		
		// correct spacecraft attachment
		pos.z += AdapterOffset;
		SetAttachmentParams( ahToPayload, pos, dir, rot );

		// correct mass
		SetEmptyMass( GetEmptyMass() + AdapterMass );

		sprintf_s( buffer, 128, "(Shuttle Centaur) added payload adapter: mesh:%s|height:%lf|mass:%lf", AdapterMeshName.c_str(), AdapterOffset, AdapterMass );
		oapiWriteLog( buffer );
	}
	return;
}
