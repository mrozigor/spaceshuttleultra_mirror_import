#define ORBITER_MODULE


#include "SSU_Centaur.h"
#include "meshres_Centaur_G.h"
#include "meshres_Centaur_GPrime.h"
#include <UltraMath.h>


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
	ctrlRL10_P.SetGains( 0.1, 0.01, 0 );
	ctrlRL10_Y.SetGains( 0.1, 0.01, 0 );
	ctrlRL10_R.SetGains( 0.4, 0.05, 0 );
	RL10_P = 0;
	RL10_Y = 0;
	RL10_R = 0;
	manP = 0;
	manY = 0;
	manR = 0;

	separated = false;
	timer_ACS_ENA = -1;
	ACS_ENA = false;
	timer_RL10_ENA = -1;
	RL10_ENA = false;
	timer_RL10_startseq = -1;
	RL10_startseq = false;
	RL10_chill = 0;

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
	static PARTICLESTREAMSPEC pssRL10_chill = {
		0,
		0.4,
		50,
		5,
		0,
		0.5,
		1,
		0.5,
		PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_FLAT,
		1, 1,
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0
	};

	if ((bFoundData == true) && (_strnicmp( pszBuffer, "G", 2 ) == 0))
	{
		// set G
		SetEmptyMass( G_EMPTY_MASS );
		phTank = CreatePropellantResource( G_RL10_PROPELLANT_MASS );
		// RL-10
		thRL10[0] = CreateThruster( RL10_C1_POS_G, RL10_C1_DIR, RL10A33B_THRUST_VAC, phTank, RL10A33B_ISP_VAC );
		thRL10[1] = CreateThruster( RL10_C2_POS_G, RL10_C2_DIR, RL10A33B_THRUST_VAC, phTank, RL10A33B_ISP_VAC );

		AddParticleStream( &pssRL10_chill, RL10_C1_POS_G, -RL10_C1_DIR, &RL10_chill );
		AddParticleStream( &pssRL10_chill, RL10_C2_POS_G, -RL10_C2_DIR, &RL10_chill );

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
		SetMeshVisibilityMode( mesh_idx, MESHVIS_ALWAYS );
		SetSize( 20 );
		SetCrossSections( _V( 19.49, 18.88, 15.71 ) );
		SetPMI( _V( 4.32, 4.35, 2.69 ) );
		SetCameraOffset( _V( 0, 2.4, 1.9 ) );

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

		AddParticleStream( &pssRL10_chill, RL10_C1_POS_GPRIME, -RL10_C1_DIR, &RL10_chill );
		AddParticleStream( &pssRL10_chill, RL10_C2_POS_GPRIME, -RL10_C2_DIR, &RL10_chill );

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
		SetMeshVisibilityMode( mesh_idx, MESHVIS_ALWAYS );
		SetSize( 25 );
		SetCrossSections( _V( 30.84, 30.86, 15.74 ) );
		SetPMI( _V( 5.70, 5.66, 2.66 ) );
		SetCameraOffset( _V( 0, 2.4, 1.2 ) );

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
		PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_SQRT,
		0, 1,
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0};
	psRL10.tex = oapiRegisterParticleTexture( "Contrail1a" );
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
		0.1,
		1.5,
		10,
		PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_SQRT,
		0, 1,
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0};
	AddExhaust( thACS[0], 0.5, 0.05 );
	AddExhaustStream( thACS[0], &psACS );
	AddExhaust( thACS[1], 0.5, 0.05 );
	AddExhaustStream( thACS[1], &psACS );
	AddExhaust( thACS[2], 0.5, 0.05 );
	AddExhaustStream( thACS[2], &psACS );
	AddExhaust( thACS[3], 0.5, 0.05 );
	AddExhaustStream( thACS[3], &psACS );
	AddExhaust( thACS[4], 0.5, 0.05 );
	AddExhaustStream( thACS[4], &psACS );
	AddExhaust( thACS[5], 0.5, 0.05 );
	AddExhaustStream( thACS[5], &psACS );
	AddExhaust( thACS[6], 0.5, 0.05 );
	AddExhaustStream( thACS[6], &psACS );
	AddExhaust( thACS[7], 0.5, 0.05 );
	AddExhaustStream( thACS[7], &psACS );
	AddExhaust( thACS[8], 0.5, 0.05 );
	AddExhaustStream( thACS[8], &psACS );
	AddExhaust( thACS[9], 0.5, 0.05 );
	AddExhaustStream( thACS[9], &psACS );
	AddExhaust( thACS[10], 0.5, 0.05 );
	AddExhaustStream( thACS[10], &psACS );
	AddExhaust( thACS[11], 0.5, 0.05 );
	AddExhaustStream( thACS[11], &psACS );

	SetMaxThrust( ENGINE_RETRO, 0 );
	SetMaxThrust( ENGINE_HOVER, 0 );
	return;
}

void SSU_Centaur::clbkPreStep( double simt, double simdt, double mjd )
{
	if ((!ACS_ENA) && (timer_ACS_ENA != -1))
	{
		timer_ACS_ENA += simdt;
		if (timer_ACS_ENA >= ACS_ENA_DELAY)
		{
			EnableACS();
			ACS_ENA = true;
		}
	}

	if ((!RL10_ENA) && (timer_RL10_ENA != -1))
	{
		timer_RL10_ENA += simdt;
		if (timer_RL10_ENA >= RL10_ENA_DELAY)
		{
			RL10_ENA = true;
		}
	}

	if ((separated == false) && (GetAttachmentStatus( ahToCISS ) == NULL))
	{
		if (simt < 1)// makeshift first cycle check
		{
			// if "first" cycle and separated from CISS, assume it separated a long time ago and enable all engines without delay
			EnableACS();
			ACS_ENA = true;
			RL10_ENA = true;

			asANTENNA.pos = 1.0;
		}
		else
		{
			timer_ACS_ENA = 0;
			timer_RL10_ENA = 0;
		}
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

	// RL10 start sequence
	if (RL10_startseq)
	{
		if (GetThrusterLevel( thRL10[0] ) == 0)
		{
			if (GetPropellantMass( phTank ) == 0)
			{
				RL10_startseq = false;
				EnablePitchYawRollACS();
			}
			else
			{
				timer_RL10_startseq += simdt;
				if (timer_RL10_startseq > RL10_START_SEQ)// prop settling time
				{
					SetThrusterLevel( thRL10[0], 1 );
					SetThrusterLevel( thRL10[1], 1 );
					SetThrusterGroupLevel( THGROUP_ATT_FORWARD, 0 );
					RL10_chill = 0;
					InhibitPitchYawRollACS();
				}
			}
		}
		else
		{
			// engine tvc
			VECTOR3 avel;
			GetAngularVel( avel );
			if (manP != 0) RL10_P += manP * simdt;// man
			else
			{
				double newP = ctrlRL10_P.Step( avel.x * DEG, simdt );// auto (drive to 0º/s)
				//nozzle rate limit
				if ((newP - RL10_P) > RL10_AUTO_GIMBAL_RATE) newP = RL10_P + RL10_AUTO_GIMBAL_RATE;
				else if ((newP - RL10_P) < -RL10_AUTO_GIMBAL_RATE) newP = RL10_P - RL10_AUTO_GIMBAL_RATE;
				RL10_P = newP;
			}
			if (manY != 0) RL10_Y += manY * simdt;// man
			else
			{
				double newY = ctrlRL10_Y.Step( avel.y * DEG, simdt );// auto (drive to 0º/s)
				//nozzle rate limit
				if ((newY - RL10_Y) > RL10_AUTO_GIMBAL_RATE) newY = RL10_Y + RL10_AUTO_GIMBAL_RATE;
				else if ((newY - RL10_Y) < -RL10_AUTO_GIMBAL_RATE) newY = RL10_Y - RL10_AUTO_GIMBAL_RATE;
				RL10_Y = newY;
			}
			if (manR != 0) RL10_R += manR * simdt;// man
			else
			{
				double newR = ctrlRL10_R.Step( avel.z * DEG, simdt );// auto (drive to 0º/s)
				//nozzle rate limit
				if ((newR - RL10_R) > RL10_AUTO_GIMBAL_RATE) newR = RL10_R + RL10_AUTO_GIMBAL_RATE;
				else if ((newR - RL10_R) < -RL10_AUTO_GIMBAL_RATE) newR = RL10_R - RL10_AUTO_GIMBAL_RATE;
				RL10_R = newR;
			}
			// output
			VECTOR3 tv0 = _V( -sin( range( -RL10_GIMBAL_RANGE, RL10_Y + RL10_R, RL10_GIMBAL_RANGE ) * RAD ), sin( range( -RL10_GIMBAL_RANGE, RL10_P, RL10_GIMBAL_RANGE ) * RAD ), 1 );
			VECTOR3 tv1 = _V( -sin( range( -RL10_GIMBAL_RANGE, RL10_Y - RL10_R, RL10_GIMBAL_RANGE ) * RAD ), sin( range( -RL10_GIMBAL_RANGE, RL10_P, RL10_GIMBAL_RANGE ) * RAD ), 1 );
			normalise( tv0 );
			normalise( tv1 );
			SetThrusterDir( thRL10[0], tv0 );
			SetThrusterDir( thRL10[1], tv1 );
		}
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
	if (!separated) return true;

	char cbuf[64];
	double Ypos = 0.3;

	sprintf_s( cbuf, 64, "ACS available: %.2f Kg", GetPropellantMass( phACS ) );
	skp->Text( (int)(hps->W * 0.01), (int)(hps->H * Ypos), cbuf, strlen( cbuf ) );
	Ypos += 0.03;

	if (!RL10_ENA)
	{
		sprintf_s( cbuf, 64, "RL-10 inhibits removed in: %.0f s", RL10_ENA_DELAY - timer_RL10_ENA );
		skp->Text( (int)(hps->W * 0.01), (int)(hps->H * Ypos), cbuf, strlen( cbuf ) );
		Ypos += 0.03;
	}
	else if ((RL10_startseq) && (RL10_chill))
	{
		sprintf_s( cbuf, 64, "RL-10 ignition in: %.0f s", RL10_START_SEQ - timer_RL10_startseq );
		skp->Text( (int)(hps->W * 0.01), (int)(hps->H * Ypos), cbuf, strlen( cbuf ) );
		Ypos += 0.03;
	}

	if (!ACS_ENA)
	{
		sprintf_s( cbuf, 64, "ACS inhibits removed in: %.0f s", ACS_ENA_DELAY - timer_ACS_ENA );
		skp->Text( (int)(hps->W * 0.01), (int)(hps->H * Ypos), cbuf, strlen( cbuf ) );
		Ypos += 0.03;
	}

	OBJHANDLE earth = GetGravityRef();
	char str[8];
	oapiGetObjectName( earth, str, 8 ); 
	if (!_strnicmp( str, "Earth", 5 ))
	{
		VECTOR3 vel;
		VECTOR3 pos;
		GetRelativeVel( earth, vel );
		GetRelativePos( earth, pos );
		double v = length( vel );
		double r = length( pos );
		double mu = 398600439968871.19;
		double C3 = ((0.5 * v * v) - (mu / r)) * 0.000002;
		sprintf_s( cbuf, 64, "C3 energy: %.2f km^2/sec^2", C3 );
		skp->Text( (int)(hps->W * 0.01), (int)(hps->H * Ypos), cbuf, strlen( cbuf ) );
	}
	return true;
}


int SSU_Centaur::clbkConsumeBufferedKey( DWORD key, bool down, char* kstate )
{
	if (!down)
	{
		// man gimbal
		if ((manP == -RL10_MAN_GIMBAL_RATE) && (key == OAPI_KEY_NUMPAD2))
		{
			manP = 0;
			return 1;
		}
		else if ((manP == RL10_MAN_GIMBAL_RATE) && (key == OAPI_KEY_NUMPAD8))
		{
			manP = 0;
			return 1;
		}
		else if ((manY == -RL10_MAN_GIMBAL_RATE) && (key == OAPI_KEY_NUMPAD1))
		{
			manY = 0;
			return 1;
		}
		else if ((manY == RL10_MAN_GIMBAL_RATE) && (key == OAPI_KEY_NUMPAD3))
		{
			manY = 0;
			return 1;
		}
		else if ((manR == -RL10_MAN_GIMBAL_RATE) && (key == OAPI_KEY_NUMPAD6))
		{
			manR = 0;
			return 1;
		}
		else if ((manR == RL10_MAN_GIMBAL_RATE) && (key == OAPI_KEY_NUMPAD4))
		{
			manR = 0;
			return 1;
		}
		return 0;	
	}

	if ((KEYMOD_SHIFT( kstate ) == false) && (KEYMOD_CONTROL( kstate ) == true) && (KEYMOD_ALT( kstate ) == false))// only CTRL key modifier
	{
		if ((GetThrusterLevel( thRL10[0] ) != 0) || GetAttachmentStatus( ahToCISS ) || !RL10_ENA) return 0;// no separations while engines are firing or still attached to CISS or nearby OV

		if (key == OAPI_KEY_J)
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
	
	if ((KEYMOD_SHIFT( kstate ) == false) && (KEYMOD_CONTROL( kstate ) == false) && (KEYMOD_ALT( kstate ) == false))// no key modifiers
	{
		if (key == OAPI_KEY_ADD)// check for start
		{
			if (!RL10_startseq && RL10_ENA && (GetPropellantMass( phTank ) != 0))
			{
				timer_RL10_startseq = 0;
				RL10_startseq = true;
				// start prop settling
				SetThrusterGroupLevel( THGROUP_ATT_FORWARD, 1 );
				// start chilldown
				RL10_chill = 1;
				return 1;
			}
		}
		else if (key == OAPI_KEY_SUBTRACT)// check for shutdown
		{
			if (RL10_startseq)
			{
				timer_RL10_startseq = -1;
				RL10_startseq = false;
				// stop engines
				SetThrusterLevel( thRL10[0], 0 );
				SetThrusterLevel( thRL10[1], 0 );
				SetThrusterGroupLevel( THGROUP_ATT_FORWARD, 0 );
				RL10_chill = 0;
				EnablePitchYawRollACS();
				return 1;
			}
		}

		// man gimbal
		if (manP == 0)
		{
			if (key == OAPI_KEY_NUMPAD2)
			{
				manP = -RL10_MAN_GIMBAL_RATE;
				return 1;
			}
			else if (key == OAPI_KEY_NUMPAD8)
			{
				manP = RL10_MAN_GIMBAL_RATE;
				return 1;
			}
		}
		if (manY == 0)
		{
			if (key == OAPI_KEY_NUMPAD1)
			{
				manY = -RL10_MAN_GIMBAL_RATE;
				return 1;
			}
			else if (key == OAPI_KEY_NUMPAD3)
			{
				manY = RL10_MAN_GIMBAL_RATE;
				return 1;
			}
		}
		if (manR == 0)
		{
			if (key == OAPI_KEY_NUMPAD6)
			{
				manR = -RL10_MAN_GIMBAL_RATE;
				return 1;
			}
			else if (key == OAPI_KEY_NUMPAD4)
			{
				manR = RL10_MAN_GIMBAL_RATE;
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
		SetMeshVisibilityMode( AddMesh( hAdapterMesh, &pos ), MESHVIS_ALWAYS );
		
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

void SSU_Centaur::EnableACS( void )
{
	THRUSTER_HANDLE thTEMP[4];
	thTEMP[0] = thACS[3];// S2A
	thTEMP[1] = thACS[4];// S2B
	thTEMP[2] = thACS[9];// S4A
	thTEMP[3] = thACS[10];// S4B
	CreateThrusterGroup( thTEMP, 4, THGROUP_ATT_FORWARD );

	EnablePitchYawRollACS();
	return;
}

void SSU_Centaur::EnablePitchYawRollACS( void )
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
	return;
}

void SSU_Centaur::InhibitPitchYawRollACS( void )
{
	DelThrusterGroup( THGROUP_ATT_PITCHUP );
	DelThrusterGroup( THGROUP_ATT_PITCHDOWN );
	DelThrusterGroup( THGROUP_ATT_YAWLEFT );
	DelThrusterGroup( THGROUP_ATT_YAWRIGHT );
	DelThrusterGroup( THGROUP_ATT_BANKLEFT );
	DelThrusterGroup( THGROUP_ATT_BANKRIGHT );
	return;
}
