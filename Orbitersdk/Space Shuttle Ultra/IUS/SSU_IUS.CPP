#define ORBITER_MODULE


#include "SSU_IUS.h"
#include "meshres_IUS_SRM2.h"
#include <UltraMath.h>


DLLCLBK VESSEL *ovcInit( OBJHANDLE hvessel, int flightmodel )
{
	return new SSU_IUS( hvessel );
}

DLLCLBK void ovcExit( VESSEL *vessel )
{
	if (vessel) delete (SSU_IUS*)vessel;
}


SSU_IUS::SSU_IUS( OBJHANDLE hObj ):VESSEL3( hObj )
{
	MainExternalMeshVisual = NULL;

	propload[0] = 1;
	propload[1] = 1;

	ctrlSRM_P.SetGains( 0.1, 0.01, 0 );
	ctrlSRM_Y.SetGains( 0.1, 0.01, 0 );
	SRM_P = 0;
	SRM_Y = 0;
	manP = 0;
	manY = 0;
	
	separated = false;
	STAGE1separated = false;
	timer_RCS_ENA = -1;
	RCS_ENA = false;
	timer_SRM_ENA = -1;
	SRM_ENA = false;
	rollRCSonly = false;

	AdapterOffset = 0;
	AdapterMass = 0;
}

void SSU_IUS::clbkSetClassCaps(FILEHANDLE cfg)
{
	// default to full vehicle
	SetSize(5.0);
	SetEmptyMass( STAGE1_EMPTYMASS + STAGE2_EMPTYMASS );
	SetCrossSections(_V(13.73, 17.84, 10.71));
	SetPMI(_V(2.29, 2.4, 1.79));

	mesh_stage1 = AddMesh( oapiLoadMeshGlobal( STAGE_1_MESHNAME ), &STAGE1_OFFSET );// stage 1 mesh
	SetMeshVisibilityMode( mesh_stage1, MESHVIS_ALWAYS );

	mesh_stage2 = AddMesh( oapiLoadMeshGlobal( STAGE_2_MESHNAME ) );// Main ship mesh (stage 2)
	SetMeshVisibilityMode( mesh_stage2, MESHVIS_ALWAYS );

	SetCameraOffset( _V( 0, 1.8, 0.8 ) );

	ahPayload = CreateAttachment(false, PAYLOAD_ATTACH_OFFSET, _V(0, 0, 1), _V(0, 1, 0), "SSU_PLD");
	ahASE = CreateAttachment(true, ASE_ATTACH_OFFSET, _V(0, 0, -1), _V(0, -1, 0), "SSU_ASE");

	// SRM
	ph_main[0] = CreatePropellantResource( STAGE1_PROPELLANT_MAXMASS );    // main tank stage 1
	ph_main[1] = CreatePropellantResource( STAGE2_PROPELLANT_MAXMASS );    // main tank stage 2
	SetDefaultPropellantResource( ph_main[0] );

	PARTICLESTREAMSPEC psSRM = {
		0,
		0.6,
		25,
		50,
		0,
		0.12,
		40,
		5,
		PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_SQRT,
		0, 1,
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0};

	th_main[0] = CreateThruster( SRM1_POS, SRM1_DIR, STAGE1_THRUST_VAC, ph_main[0], STAGE1_ISP_VAC ); //THRUSTER FOR 1ST STAGE ALONE
	AddExhaust( th_main[0], 5, 0.5 ); //EXHAUST location OF 1ST STAGE
	AddExhaustStream( th_main[0], &psSRM );

	th_main[1] = CreateThruster( SRM2_POS, SRM2_DIR, STAGE2_THRUST_VAC, ph_main[1], STAGE2_EEC_ISP_VAC );//THRUSTER FOR 2ND STAGE ALONE
	AddExhaust( th_main[1], 5, 0.5 ); //EXHAUST location OF 2ND STAGE
	AddExhaustStream( th_main[1], &psSRM );

	// RCS
	ph_rcs = CreatePropellantResource( RCS_PROPELLANT_MASS );
	PARTICLESTREAMSPEC psRCS = {
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

	th_rcs[0] = CreateThruster( RCS1_POS, RCS1_DIR, RCS_THRUST_VAC, ph_rcs, RCS_ISP_VAC );
	AddExhaust( th_rcs[0], 0.5, 0.05 );
	AddExhaustStream( th_rcs[0], &psRCS );
	th_rcs[1] = CreateThruster( RCS2_POS, RCS2_DIR, RCS_THRUST_VAC, ph_rcs, RCS_ISP_VAC );
	AddExhaust( th_rcs[1], 0.5, 0.05 );
	AddExhaustStream( th_rcs[1], &psRCS );
	th_rcs[2] = CreateThruster( RCS3_POS, RCS3_DIR, RCS_THRUST_VAC, ph_rcs, RCS_ISP_VAC );
	AddExhaust( th_rcs[2], 0.5, 0.05 );
	AddExhaustStream( th_rcs[2], &psRCS );
	th_rcs[3] = CreateThruster( RCS4_POS, RCS4_DIR, RCS_THRUST_VAC, ph_rcs, RCS_ISP_VAC );
	AddExhaust( th_rcs[3], 0.5, 0.05 );
	AddExhaustStream( th_rcs[3], &psRCS );
	th_rcs[4] = CreateThruster( RCS5_POS, RCS5_DIR, RCS_THRUST_VAC, ph_rcs, RCS_ISP_VAC );
	AddExhaust( th_rcs[4], 0.5, 0.05 );
	AddExhaustStream( th_rcs[4], &psRCS );
	th_rcs[5] = CreateThruster( RCS6_POS, RCS6_DIR, RCS_THRUST_VAC, ph_rcs, RCS_ISP_VAC );
	AddExhaust( th_rcs[5], 0.5, 0.05 );
	AddExhaustStream( th_rcs[5], &psRCS );
	th_rcs[6] = CreateThruster( RCS7_POS, RCS7_DIR, RCS_THRUST_VAC, ph_rcs, RCS_ISP_VAC );
	AddExhaust( th_rcs[6], 0.5, 0.05 );
	AddExhaustStream( th_rcs[6], &psRCS );
	th_rcs[7] = CreateThruster( RCS8_POS, RCS8_DIR, RCS_THRUST_VAC, ph_rcs, RCS_ISP_VAC );
	AddExhaust( th_rcs[7], 0.5, 0.05 );
	AddExhaustStream( th_rcs[7], &psRCS );
	th_rcs[8] = CreateThruster( RCS9_POS, RCS9_DIR, RCS_THRUST_VAC, ph_rcs, RCS_ISP_VAC );
	AddExhaust( th_rcs[8], 0.5, 0.05 );
	AddExhaustStream( th_rcs[8], &psRCS );
	th_rcs[9] = CreateThruster( RCS10_POS, RCS10_DIR, RCS_THRUST_VAC, ph_rcs, RCS_ISP_VAC );
	AddExhaust( th_rcs[9], 0.5, 0.05 );
	AddExhaustStream( th_rcs[9], &psRCS );
	th_rcs[10] = CreateThruster( RCS11_POS, RCS11_DIR, RCS_THRUST_VAC, ph_rcs, RCS_ISP_VAC );
	AddExhaust( th_rcs[10], 0.5, 0.05 );
	AddExhaustStream( th_rcs[10], &psRCS );
	th_rcs[11] = CreateThruster( RCS12_POS, RCS12_DIR, RCS_THRUST_VAC, ph_rcs, RCS_ISP_VAC );
	AddExhaust( th_rcs[11], 0.5, 0.05 );
	AddExhaustStream( th_rcs[11], &psRCS );

	DefineAnimations();
}

void SSU_IUS::clbkPostStep(double simt, double simdt, double mjd)
{
	if ((!RCS_ENA) && (timer_RCS_ENA != -1))
	{
		timer_RCS_ENA += simdt;
		if (timer_RCS_ENA >= RCS_ENA_DELAY)
		{
			EnableRCS();
			RCS_ENA = true;
		}
	}

	if ((!SRM_ENA) && (timer_SRM_ENA != -1))
	{
		timer_SRM_ENA += simdt;
		if (timer_SRM_ENA >= SRM_ENA_DELAY)
		{
			SRM_ENA = true;
		}
	}

	if ((separated == false) && (GetAttachmentStatus( ahASE ) == NULL))
	{
		if (simt < 1)// makeshift first cycle check
		{
			// if "first" cycle and separated from ASE, assume it separated a long time ago and enable all engines without delay
			EnableRCS();
			RCS_ENA = true;
			SRM_ENA = true;
		}
		else
		{
			timer_RCS_ENA = 0;
			timer_SRM_ENA = 0;
		}
		separated = true;

		// add payload mass to IUS dry mass
		OBJHANDLE ohPL = GetAttachmentStatus( ahPayload );
		if (ohPL)
		{
			VESSEL* vPL = oapiGetVesselInterface( ohPL );
			SetEmptyMass( GetEmptyMass() + vPL->GetMass() );
		}
	}

	if (GetThrusterLevel( th_main[0] ) != 0)
	{
		// engine tvc
		VECTOR3 avel;
		GetAngularVel( avel );
		if (manP != 0) SRM_P += manP * simdt;// man
		else
		{
			double newP = ctrlSRM_P.Step( avel.x * DEG, simdt );// auto (drive to 0º/s)
			//nozzle rate limit
			if ((newP - SRM_P) > SRM_AUTO_GIMBAL_RATE) newP = SRM_P + SRM_AUTO_GIMBAL_RATE;
			else if ((newP - SRM_P) < -SRM_AUTO_GIMBAL_RATE) newP = SRM_P - SRM_AUTO_GIMBAL_RATE;
			SRM_P = newP;
		}
		if (manY != 0) SRM_Y += manY * simdt;// man
		else
		{
			double newY = ctrlSRM_Y.Step( avel.y * DEG, simdt );// auto (drive to 0º/s)
			//nozzle rate limit
			if ((newY - SRM_Y) > SRM_AUTO_GIMBAL_RATE) newY = SRM_Y + SRM_AUTO_GIMBAL_RATE;
			else if ((newY - SRM_Y) < -SRM_AUTO_GIMBAL_RATE) newY = SRM_Y - SRM_AUTO_GIMBAL_RATE;
			SRM_Y = newY;
		}
		// output
		VECTOR3 tv = _V( -sin( range( -SRM1_GIMBAL_RANGE, SRM_Y, SRM1_GIMBAL_RANGE ) * RAD ), sin( range( -SRM1_GIMBAL_RANGE, SRM_P, SRM1_GIMBAL_RANGE ) * RAD ), 1 );
		normalise( tv );
		SetThrusterDir( th_main[0], tv );
	}
	else if (GetThrusterLevel( th_main[1] ) != 0)
	{
		VECTOR3 avel;
		GetAngularVel( avel );
		if (manP != 0) SRM_P += manP * simdt;// man
		else
		{
			double newP = ctrlSRM_P.Step( avel.x * DEG, simdt );// auto (drive to 0º/s)
			//nozzle rate limit
			if ((newP - SRM_P) > SRM_AUTO_GIMBAL_RATE) newP = SRM_P + SRM_AUTO_GIMBAL_RATE;
			else if ((newP - SRM_P) < -SRM_AUTO_GIMBAL_RATE) newP = SRM_P - SRM_AUTO_GIMBAL_RATE;
			SRM_P = newP;
		}
		if (manY != 0) SRM_Y += manY * simdt;// man
		else
		{
			double newY = ctrlSRM_Y.Step( avel.y * DEG, simdt );// auto (drive to 0º/s)
			//nozzle rate limit
			if ((newY - SRM_Y) > SRM_AUTO_GIMBAL_RATE) newY = SRM_Y + SRM_AUTO_GIMBAL_RATE;
			else if ((newY - SRM_Y) < -SRM_AUTO_GIMBAL_RATE) newY = SRM_Y - SRM_AUTO_GIMBAL_RATE;
			SRM_Y = newY;
		}
		// output
		VECTOR3 tv = _V( -sin( range( -SRM2_GIMBAL_RANGE, SRM_Y, SRM2_GIMBAL_RANGE ) * RAD ), sin( range( -SRM2_GIMBAL_RANGE, SRM_P, SRM2_GIMBAL_RANGE ) * RAD ), 1 );
		normalise( tv );
		SetThrusterDir( th_main[1], tv );
	}
	else if (rollRCSonly)
	{
		EnablePitchYawRCS();
		rollRCSonly = false;
	}

	// animation
	if (asEXTENDNOZZLE.Opening() == true) asEXTENDNOZZLE.Move( simdt / (STAGE2_EEC_DEPLOY_DELAY + (1 / STAGE2_EEC_DEPLOY_RATE)) );
	return;
}

void SSU_IUS::DefineAnimations(void)
{
	double animstart = STAGE2_EEC_DEPLOY_DELAY / (STAGE2_EEC_DEPLOY_DELAY + (1 / STAGE2_EEC_DEPLOY_RATE));

	static UINT EECGrp[2] = {GRP_EXTENDABLE_EXITCONE_IUS_SRM2, GRP_EEC_STRUTS_BRACKETS_IUS_SRM2};
	static MGROUP_TRANSLATE NOZZLE( mesh_stage2, EECGrp, 2, _V( 0, 0, -0.5 ) );
	anim_EXTENDNOZZLE = CreateAnimation( 0.0 );
	AddAnimationComponent(anim_EXTENDNOZZLE, animstart, 1, &NOZZLE);

	static UINT EECStrutsGrp[1] = {GRP_EEC_STRUTS_IUS_SRM2};
	static MGROUP_SCALE NOZZLE_STRUTS( mesh_stage2, EECStrutsGrp, 1, _V( 0, 0, -0.2308 ), _V( 1, 1, 14 ) );
	AddAnimationComponent( anim_EXTENDNOZZLE, animstart, 1, &NOZZLE_STRUTS );

	asEXTENDNOZZLE.Set( AnimState::CLOSED, 0.0 );
	SetAnimation( anim_EXTENDNOZZLE, asEXTENDNOZZLE.pos );

	RegisterAnimation();
}

bool SSU_IUS::clbkDrawHUD( int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp )
{
	if (!separated) return true;

	char cbuf[64];
	sprintf_s( cbuf, 64, "RCS available: %.2fKg", GetPropellantMass( ph_rcs ) );
	skp->Text( (int)(hps->W * 0.01), (int)(hps->H * 0.3), cbuf, strlen( cbuf ) );

	if (!SRM_ENA)
	{
		sprintf_s( cbuf, 64, "SRM inhibits removed in: %.0fs", SRM_ENA_DELAY - timer_SRM_ENA );
		skp->Text( (int)(hps->W * 0.01), (int)(hps->H * 0.33), cbuf, strlen( cbuf ) );
	}

	if (!RCS_ENA)
	{
		sprintf_s( cbuf, 64, "RCS inhibits removed in: %.0fs", RCS_ENA_DELAY - timer_RCS_ENA );
		skp->Text( (int)(hps->W * 0.01), (int)(hps->H * 0.36), cbuf, strlen( cbuf ) );
	}
	return true;
}

int SSU_IUS::clbkConsumeBufferedKey(DWORD key, bool down, char *kstate)
{
	if (!down)
	{
		// man gimbal
		if ((manP == -SRM_MAN_GIMBAL_RATE) && (key == OAPI_KEY_NUMPAD2))
		{
			manP = 0;
			return 1;
		}
		else if ((manP == SRM_MAN_GIMBAL_RATE) && (key == OAPI_KEY_NUMPAD8))
		{
			manP = 0;
			return 1;
		}
		else if ((manY == -SRM_MAN_GIMBAL_RATE) && (key == OAPI_KEY_NUMPAD1))
		{
			manY = 0;
			return 1;
		}
		else if ((manY == SRM_MAN_GIMBAL_RATE) && (key == OAPI_KEY_NUMPAD3))
		{
			manY = 0;
			return 1;
		}
		return 0;	
	}

	if ((KEYMOD_SHIFT( kstate ) == false) && (KEYMOD_CONTROL( kstate ) == true) && (KEYMOD_ALT( kstate ) == false))// only CTRL key modifier
	{
		if ((GetThrusterLevel( th_main[0] ) != 0) || (GetThrusterLevel( th_main[1] ) != 0)) return 0;// no separations while engines are firing

		if (key == OAPI_KEY_J)
		{
			if (GetAttachmentStatus( ahASE ) == NULL)// payload sep only after deployment
			{
				// remove payload mass from IUS dry mass
				OBJHANDLE ohPL = GetAttachmentStatus( ahPayload );
				if (ohPL)
				{
					VESSEL* vPL = oapiGetVesselInterface( ohPL );
					SetEmptyMass( GetEmptyMass() - vPL->GetMass() );
				}
				// separate payload
				DetachChild( ahPayload, 0.3 );
				return 1;
			}
		}
		else if (key == OAPI_KEY_G)
		{
			if (STAGE1separated == false)
			{
				STAGE1separated = true;
				// separate stage 1 (code comes from drag chute jettison)
				VESSELSTATUS2 vs;
				memset(&vs, 0, sizeof(vs));
				vs.version=2;
				GetStatusEx(&vs);
				vs.status=0;
				VECTOR3 stage1_ofs;
				Local2Rel(STAGE1_OFFSET, stage1_ofs);
				vs.rpos=stage1_ofs;
				// add separation velocity
				GlobalRot( _V( 0, 0, -0.2 ), stage1_ofs );
				vs.rvel += stage1_ofs;
				char name[255];
				strcpy_s(name, GetName()); 
				strcat_s(name, "-STAGE1");
				oapiCreateVesselEx(name, "SSU_IUS_STAGE1", &vs);

				// update vehicle defs
				SetStage2Config();

				// start EEC deployment
				asEXTENDNOZZLE.action = AnimState::OPENING;
				return 1;
			}
		}
	}

	if ((KEYMOD_SHIFT( kstate ) == false) && (KEYMOD_CONTROL( kstate ) == false) && (KEYMOD_ALT( kstate ) == false))// no key modifiers
	{
		if (key == OAPI_KEY_ADD)// check for start
		{
			if (SRM_ENA)
			{
				if (!STAGE1separated)
				{
					SetThrusterLevel( th_main[0], 1 );
					rollRCSonly = true;
					InhibitPitchYawRCS();
					return 1;
				}
				else if (asEXTENDNOZZLE.Open())
				{
					SetThrusterLevel( th_main[1], 1 );
					rollRCSonly = true;
					InhibitPitchYawRCS();
					return 1;
				}
			}
		}

		// man gimbal
		if (manP == 0)
		{
			if (key == OAPI_KEY_NUMPAD2)
			{
				manP = -SRM_MAN_GIMBAL_RATE;
				return 1;
			}
			else if (key == OAPI_KEY_NUMPAD8)
			{
				manP = SRM_MAN_GIMBAL_RATE;
				return 1;
			}
		}
		if (manY == 0)
		{
			if (key == OAPI_KEY_NUMPAD1)
			{
				manY = -SRM_MAN_GIMBAL_RATE;
				return 1;
			}
			else if (key == OAPI_KEY_NUMPAD3)
			{
				manY = SRM_MAN_GIMBAL_RATE;
				return 1;
			}
		}
	}
	return 0;
}

void SSU_IUS::clbkAnimate( double simt )
{
	SetAnimation( anim_EXTENDNOZZLE, asEXTENDNOZZLE.pos );
}

void SSU_IUS::clbkVisualCreated(VISHANDLE vis, int refcount)
{
	MainExternalMeshVisual = GetMesh(vis, 0);

	SetAnimation( anim_EXTENDNOZZLE, asEXTENDNOZZLE.pos );
}

void SSU_IUS::clbkVisualDestroyed(VISHANDLE vis, int refcount)
{
	MainExternalMeshVisual = 0;
}

void SSU_IUS::clbkSaveState(FILEHANDLE scn)
{
	VESSEL2::clbkSaveState( scn );

	WriteScenario_state( scn, "EEC", asEXTENDNOZZLE );

	if (STAGE1separated == true) oapiWriteLine( scn, "  STAGE2" );

	if ((AdapterMeshName.length() > 0) && (AdapterOffset > 0) && (AdapterMass > 0))// only save if used
	{
		oapiWriteScenario_string( scn, "ADAPTER_MESH", (char*)AdapterMeshName.c_str() );
		oapiWriteScenario_float( scn, "ADAPTER_OFFSET", AdapterOffset );
		oapiWriteScenario_float( scn, "ADAPTER_MASS", AdapterMass );
	}
	if (propload[0] < 1) oapiWriteScenario_float( scn, "LOAD_STAGE1", propload[0] );
	if (propload[1] < 1) oapiWriteScenario_float( scn, "LOAD_STAGE2", propload[1] );
}

void SSU_IUS::clbkLoadStateEx(FILEHANDLE scn, void *status)
{
	char *line;
	bool adapterON = false;

	while (oapiReadScenario_nextline(scn, line))
	{
		if (!_strnicmp(line, "EEC", 3)) {
			sscan_state( line + 3, asEXTENDNOZZLE );
		}
		else if (!_strnicmp(line, "STAGE2", 6)) {
			STAGE1separated = true;
			SetStage2Config();
		}
		else if (!_strnicmp( line, "LOAD_STAGE1", 11 ))
		{
			double tmp = 0;
			sscanf( line + 11, "%lf", &tmp );
			if ((tmp >= 0.5) && (tmp <= 1))
			{
				propload[0] = tmp;
				SetPropellantMaxMass( ph_main[0], GetPropellantMaxMass( ph_main[0] ) * propload[0] );
			}
		}
		else if (!_strnicmp( line, "LOAD_STAGE2", 11 ))
		{
			double tmp = 0;
			sscanf( line + 11, "%lf", &tmp );
			if ((tmp >= 0.5) && (tmp <= 1))
			{
				propload[1] = tmp;
				SetPropellantMaxMass( ph_main[1], GetPropellantMaxMass( ph_main[1] ) * propload[1] );
			}
		}
		else if (!_strnicmp( line, "ADAPTER_MESH", 12 ))
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
		else ParseScenarioLineEx(line, status);
	}

	if (adapterON)
	{
		// small sanity checks
		if (AdapterMeshName.length() == 0)
		{
			oapiWriteLog( "(SSU IUS) ERROR: ADAPTER_MESH was not specified" );
			return;
		}

		if (AdapterOffset <= 0)
		{
			oapiWriteLog( "(SSU IUS) ERROR: ADAPTER_OFFSET was not specified or isn't greater than 0" );
			return;
		}

		if (AdapterMass <= 0)
		{
			oapiWriteLog( "(SSU IUS) ERROR: ADAPTER_MASS was not specified or isn't greater than 0" );
			return;
		}

		char buffer[128];
		VECTOR3 pos;
		VECTOR3 dir;
		VECTOR3 rot;
		GetAttachmentParams( ahPayload, pos, dir, rot );

		// add mesh
		MESHHANDLE hAdapterMesh = oapiLoadMeshGlobal( AdapterMeshName.c_str() );
		if (hAdapterMesh == NULL)
		{
			sprintf_s( buffer, 128, "(SSU IUS) ERROR: ADAPTER_MESH file not found" );
			oapiWriteLog( buffer );
			return;
		}
		SetMeshVisibilityMode( AddMesh( hAdapterMesh, &pos ), MESHVIS_ALWAYS );
		
		// correct spacecraft attachment
		pos.z += AdapterOffset;
		SetAttachmentParams( ahPayload, pos, dir, rot );

		// correct mass
		SetEmptyMass( GetEmptyMass() + AdapterMass );

		sprintf_s( buffer, 128, "(SSU IUS) added payload adapter: mesh:%s|height:%lf|mass:%lf", AdapterMeshName.c_str(), AdapterOffset, AdapterMass );
		oapiWriteLog( buffer );
	}

	SetAnimation( anim_EXTENDNOZZLE, asEXTENDNOZZLE.pos );
}

void SSU_IUS::SetStage2Config( void )
{
	SetMeshVisibilityMode( mesh_stage1, MESHVIS_NEVER );

	SetDefaultPropellantResource( ph_main[1] );
	SetSize(3.0);
	SetEmptyMass( GetEmptyMass() - STAGE1_EMPTYMASS );
	SetPropellantMass( ph_main[0], 0 );
	SetCrossSections(_V(4.29,8.18,4.18));
	SetPMI(_V(.88, .99, .84));
	return;
}

void SSU_IUS::EnableRCS( void )
{
	THRUSTER_HANDLE th_group[8];
	//  "BANK-RIGHT"
	th_group[0] = th_rcs[2];
	th_group[1] = th_rcs[8];
	CreateThrusterGroup(th_group, 2, THGROUP_ATT_BANKRIGHT);
	//  "BANK-LEFT"
	th_group[0] = th_rcs[5];
	th_group[1] = th_rcs[11];
	CreateThrusterGroup(th_group, 2, THGROUP_ATT_BANKLEFT);
	//FORWARD
	th_group[0] = th_rcs[0];
	th_group[1] = th_rcs[1];
	th_group[2] = th_rcs[3];
	th_group[3] = th_rcs[4];
	th_group[4] = th_rcs[6];
	th_group[5] = th_rcs[7];
	th_group[6] = th_rcs[9];
	th_group[7] = th_rcs[10];
	CreateThrusterGroup(th_group, 8, THGROUP_ATT_FORWARD);
	
	EnablePitchYawRCS();
	return;
}

void SSU_IUS::EnablePitchYawRCS( void )
{
	THRUSTER_HANDLE th_group[2];
	//  "PITCH-UP"
	th_group[0] = th_rcs[6];
	th_group[1] = th_rcs[7];
	CreateThrusterGroup(th_group, 2, THGROUP_ATT_PITCHUP);
	//  "PITCH-DOWN"
	th_group[0] = th_rcs[0];
	th_group[1] = th_rcs[1];
	CreateThrusterGroup(th_group, 2, THGROUP_ATT_PITCHDOWN);
	//  "YAW-RIGHT"
	th_group[0] = th_rcs[9];
	th_group[1] = th_rcs[10];
	CreateThrusterGroup(th_group, 2, THGROUP_ATT_YAWRIGHT);
	//  "YAW-LEFT"
	th_group[0] = th_rcs[3];
	th_group[1] = th_rcs[4];
	CreateThrusterGroup(th_group, 2, THGROUP_ATT_YAWLEFT);
	return;
}

void SSU_IUS::InhibitPitchYawRCS( void )
{
	DelThrusterGroup( THGROUP_ATT_PITCHUP );
	DelThrusterGroup( THGROUP_ATT_PITCHDOWN );
	DelThrusterGroup( THGROUP_ATT_YAWRIGHT );
	DelThrusterGroup( THGROUP_ATT_YAWLEFT );
	return;
}