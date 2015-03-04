#define ORBITER_MODULE


#include "SSU_CISS.h"
#include "meshres_GPrime.h"


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
	bFirstStep = true;
}

SSU_CISS::~SSU_CISS( void )
{
	return;
}

void SSU_CISS::clbkSetClassCaps( FILEHANDLE cfg )
{
	char pszBuffer[255];
	bool bFoundData = oapiReadItem_string( cfg, "Type", pszBuffer );
	bool isGPrime = true;

	if ((bFoundData == true) && (_strnicmp( pszBuffer, "G", 2 ) == 0))
	{
		// set G
		isGPrime = false;
		SetEmptyMass( G_EMPTY_MASS );
		hMesh = oapiLoadMeshGlobal( G_MESHNAME );
		ahToOV = CreateAttachment( true, _V( 0, -2.9, 0.75 ), _V( 0, -1, 0 ), _V( 0, 0, 1 ), "SSU_OVC" );
		centaurAttachment[0] = _V( 0, -0.25, -1.3409 );
		ahToCentaur = CreateAttachment( false, centaurAttachment[0], _V( 0, 0, 1 ), _V( 0, -1, 0 ), "SSU_CG" );
	}
	else// pszBuffer = "GPrime"
	{
		// set GPrime
		// default
		SetEmptyMass( GPRIME_EMPTY_MASS );
		hMesh = oapiLoadMeshGlobal( GPRIME_MESHNAME );
		ahToOV = CreateAttachment( true, _V( 0, -2.9, 0.75 ), _V( 0, -1, 0 ), _V( 0, 0, 1 ), "SSU_OVC" );
		centaurAttachment[0] = _V( 0, -0.25, -1.3409 );
		ahToCentaur = CreateAttachment( false, centaurAttachment[0], _V( 0, 0, 1 ), _V( 0, -1, 0 ), "SSU_CGP" );
	}
	centaurAttachment[1] = centaurAttachment[0] + _V(0, 0, 1);
	centaurAttachment[2] = centaurAttachment[0] + _V(1, 0, 0);

	mesh_idx = AddMesh( hMesh );

	SetMaxThrust( ENGINE_MAIN, 0 );
	SetMaxThrust( ENGINE_RETRO, 0 );
	SetMaxThrust( ENGINE_HOVER, 0 );
	SetMaxThrust( ENGINE_ATTITUDE, 0 );

	SetSize( 30 );
	SetCrossSections( _V( 30.30, 31.63, 14.41 ) );
	SetPMI( _V( 6.56, 6.86, 2.44 ) );

	// define rotation animation
	if(isGPrime) DefineCentaurGPrimeAnimations();
	else DefineCentaurGAnimations();
	return;
}

void SSU_CISS::DefineCentaurGPrimeAnimations()
{
	anim_rotate = CreateAnimation(0.0);
	rotateAnimState.Set(AnimState::CLOSED, 0.0);

	static UINT DA_GRP[25] = {GRP_DEPLOYMENT_ADAPTER_GPRIME,
							  GRP_GOX_DUCT_BELLOW_1_GPRIME,
							  GRP_GOX_DUCT_BELLOW_2_GPRIME,
							  GRP_GOX_DUCT_BELLOW_3_GPRIME,
							  GRP_GOX_DUCT_SEGMENT_1_GPRIME,
							  GRP_GOX_DUCT_SEGMENT_2_GPRIME,
							  GRP_GOX_DUCT_SEGMENT_3_GPRIME,
							  GRP_LOX_DUCT_BELLOW_1_GPRIME,
							  GRP_LOX_DUCT_BELLOW_2_GPRIME,
							  GRP_LOX_DUCT_BELLOW_3_GPRIME,
							  GRP_LOX_DUCT_SEGMENT_1_GPRIME,
							  GRP_LOX_DUCT_SEGMENT_2_GPRIME,
							  GRP_LOX_DUCT_SEGMENT_3_GPRIME,
							  GRP_LH2_DUCT_BELLOW_1_GPRIME,
							  GRP_LH2_DUCT_BELLOW_2_GPRIME,
							  GRP_LH2_DUCT_BELLOW_3_GPRIME,
							  GRP_LH2_DUCT_SEGMENT_1_GPRIME,
							  GRP_LH2_DUCT_SEGMENT_2_GPRIME,
							  GRP_LH2_DUCT_SEGMENT_3_GPRIME,
							  GRP_GH2_DUCT_BELLOW_1_GPRIME,
							  GRP_GH2_DUCT_BELLOW_2_GPRIME,
							  GRP_GH2_DUCT_BELLOW_3_GPRIME,
							  GRP_GH2_DUCT_SEGMENT_1_GPRIME,
							  GRP_GH2_DUCT_SEGMENT_2_GPRIME,
							  GRP_GH2_DUCT_SEGMENT_3_GPRIME};
	static MGROUP_ROTATE DA (mesh_idx, DA_GRP, 25, _V(0,-0.04,-2.12), _V(-1,0,0), static_cast<float>(45.0*RAD));
	AddAnimationComponent(mesh_idx, 0, 1, &DA);

	/*static UINT GOX_VentLine_Seg1_Grp[6] = {GRP_GOX_DUCT_BELLOW_1_GPRIME,
											GRP_GOX_DUCT_BELLOW_2_GPRIME,
											GRP_GOX_DUCT_BELLOW_3_GPRIME,
											GRP_GOX_DUCT_SEGMENT_1_GPRIME,
											GRP_GOX_DUCT_SEGMENT_2_GPRIME,
											GRP_GOX_DUCT_SEGMENT_3_GPRIME};
	static MGROUP_ROTATE GOX_VentLine_Seg1 (mesh_idx, GOX_VentLine_Seg1_Grp, 6, _V(-1.77,-0.47,-2.12), _V(-1, 0, 0), static_cast<float>(45.0*RAD));
	AddAnimationComponent(mesh_idx, 0, 1, &GOX_VentLine_Seg1);

	static UINT LOX_VentLine_Seg1_Grp[6] = {GRP_LOX_DUCT_BELLOW_1_GPRIME,
											GRP_LOX_DUCT_BELLOW_2_GPRIME,
											GRP_LOX_DUCT_BELLOW_3_GPRIME,
											GRP_LOX_DUCT_SEGMENT_1_GPRIME,
											GRP_LOX_DUCT_SEGMENT_2_GPRIME,
											GRP_LOX_DUCT_SEGMENT_3_GPRIME};
	static MGROUP_ROTATE LOX_VentLine_Seg1 (mesh_idx, LOX_VentLine_Seg1_Grp, 6, _V(-1.58,-0.66,-2.17), _V(-1, 0, 0), static_cast<float>(45.0*RAD));
	AddAnimationComponent(mesh_idx, 0, 1, &LOX_VentLine_Seg1);

	static UINT GH2_VentLine_Seg1_Grp[6] = {GRP_GH2_DUCT_BELLOW_1_GPRIME,
											GRP_GH2_DUCT_BELLOW_2_GPRIME,
											GRP_GH2_DUCT_BELLOW_3_GPRIME,
											GRP_GH2_DUCT_SEGMENT_1_GPRIME,
											GRP_GH2_DUCT_SEGMENT_2_GPRIME,
											GRP_GH2_DUCT_SEGMENT_3_GPRIME};
	static MGROUP_ROTATE GH2_VentLine_Seg1 (mesh_idx, GH2_VentLine_Seg1_Grp, 6, _V(1.77,-0.47,-2.12), _V(-1, 0, 0), static_cast<float>(45.0*RAD));
	AddAnimationComponent(mesh_idx, 0, 1, &GH2_VentLine_Seg1);

	static UINT LH2_VentLine_Seg1_Grp[6] = {GRP_LH2_DUCT_BELLOW_1_GPRIME,
											GRP_LH2_DUCT_BELLOW_2_GPRIME,
											GRP_LH2_DUCT_BELLOW_3_GPRIME,
											GRP_LH2_DUCT_SEGMENT_1_GPRIME,
											GRP_LH2_DUCT_SEGMENT_2_GPRIME,
											GRP_LH2_DUCT_SEGMENT_3_GPRIME};
	static MGROUP_ROTATE LH2_VentLine_Seg1 (mesh_idx, LH2_VentLine_Seg1_Grp, 6, _V(1.58,-0.66,-2.17), _V(-1, 0, 0), static_cast<float>(45.0*RAD));
	AddAnimationComponent(mesh_idx, 0, 1, &LH2_VentLine_Seg1);*/

	static MGROUP_ROTATE AttachmentTrack (LOCALVERTEXLIST, MAKEGROUPARRAY(centaurAttachment), 3, _V(0,-0.04,-2.12), _V(-1, 0, 0), static_cast<float>(45.0*RAD));
	AddAnimationComponent(mesh_idx, 0, 1, &AttachmentTrack);
}

void SSU_CISS::DefineCentaurGAnimations()
{
	anim_rotate = CreateAnimation(0.0);
	rotateAnimState.Set(AnimState::CLOSED, 0.0);
	// TODO: implement animation (once mesh has been created)
}

int SSU_CISS::clbkConsumeBufferedKey( DWORD key, bool down, char* kstate )
{
	if (!down) return 0;

	if ((KEYMOD_SHIFT( kstate ) == false) && (KEYMOD_CONTROL( kstate ) == false) && (KEYMOD_ALT( kstate ) == false))// no key modifiers
	{
		switch (key)
		{
			case OAPI_KEY_G:
				rotateAnimState.action = AnimState::OPENING;
				return 1;
			case OAPI_KEY_L:
				rotateAnimState.action = AnimState::CLOSING;
				return 1;
			case OAPI_KEY_J:
				// centaur sep only after ciss rotation complete
				if (rotateAnimState.Open()) DetachChild( ahToCentaur, 0.3 );
				return 1;
		}
	}
	return 0;
}

void SSU_CISS::clbkPostStep(double simT, double simDT, double mjd)
{
	VESSEL2::clbkPostStep(simT, simDT, mjd);

	if(bFirstStep) { // required to make sure Centaur is positioned correctly
		bFirstStep = false;
		SetAnimation(anim_rotate, rotateAnimState.pos);
		SetAttachmentParams(ahToCentaur, centaurAttachment[0], centaurAttachment[1]-centaurAttachment[0], centaurAttachment[2]-centaurAttachment[0]);
	}

	if(rotateAnimState.Moving()) {
		rotateAnimState.Move(simDT*CISS_ROTATION_RATE);
		SetAnimation(anim_rotate, rotateAnimState.pos);
		SetAttachmentParams(ahToCentaur, centaurAttachment[0], centaurAttachment[1]-centaurAttachment[0], centaurAttachment[2]-centaurAttachment[0]);
	}
}

void SSU_CISS::clbkSaveState(FILEHANDLE scn)
{
	VESSEL2::clbkSaveState(scn);
	WriteScenario_state(scn, "ROTATION", rotateAnimState);
}

void SSU_CISS::clbkLoadStateEx(FILEHANDLE scn, void *vs)
{
	char *line;
	while (oapiReadScenario_nextline (scn, line)) {
		if(!_strnicmp(line, "ROTATION", 8)) {
			sscan_state(line+8, rotateAnimState);
			SetAnimation(anim_rotate, rotateAnimState.pos);
			SetAttachmentParams(ahToCentaur, centaurAttachment[0], centaurAttachment[1]-centaurAttachment[0], centaurAttachment[2]-centaurAttachment[0]);
		}
		else {
			ParseScenarioLineEx (line, vs);
		}
	}
}
