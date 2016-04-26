#include "CISS.h"
#include "meshres_CISS_G.h"
#include "meshres_CISS_GPrime.h"



CISS::CISS( AtlantisSubsystemDirector* _director, bool isGPrime ):AtlantisSubsystem( _director, "CISS" )
{
	this->isGPrime = isGPrime;
	bFirstStep = true;

	bMECH_PRI_PWR = false;
	bMECH_BKUP_PWR = false;
	bDA_PRI_UP = false;
	bDA_PRI_DN = false;
	bDA_BKUP_UP = false;
	bDA_BKUP_DN = false;
	bSUPER_ZIP_FIRE = false;

	rotateAnimState.Set( AnimState::CLOSED, 0.0 );

	hMesh_MK = oapiLoadMeshGlobal( CISS_MISSION_KIT_MESHNAME );

	if (isGPrime)
	{
		hMesh = oapiLoadMeshGlobal( CISS_GPRIME_MESHNAME );
		CISSoffset = CISS_GPRIME_OFFSET;
		Centaurattachoffset = CENTAUR_GPRIME_ATTACHMENT_OFFSET;
	}
	else
	{
		hMesh = oapiLoadMeshGlobal( CISS_G_MESHNAME );
		CISSoffset = CISS_G_OFFSET;
		Centaurattachoffset = CENTAUR_G_ATTACHMENT_OFFSET;
	}

	ahToCentaur = NULL;
}

CISS::~CISS( void )
{
	return;
}

void CISS::Realize()
{
	DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "L12UtoCISS_PRI", 16 );
	pSUPER_ZIP_PRI_FIRE.Connect( pBundle, 0 );
	pSUPER_ZIP_PRI_ARM_ARM.Connect( pBundle, 1 );
	pSUPER_ZIP_PRI_ARM_TB.Connect( pBundle, 2 );
	pDA_PRI_ROT_DN.Connect( pBundle, 3 );
	pDA_PRI_ROT_UP.Connect( pBundle, 4 );
	pPosition_PRI_TB.Connect( pBundle, 5 );
	pLOGIC_PRI_PWR_ON.Connect( pBundle, 6 );
	pSSP_PRI_PWR_ON.Connect( pBundle, 7 );
	pMECH_PRI_PWR_OFF.Connect( pBundle, 8 );
	pMECH_PRI_PWR_ON.Connect( pBundle, 9 );
	pMECH_PRI_PWR_TB.Connect( pBundle, 10 );

	pBundle = STS()->BundleManager()->CreateBundle( "L12UtoCISS_BKUP", 16 );
	pSUPER_ZIP_BKUP_FIRE.Connect( pBundle, 0 );
	pSUPER_ZIP_BKUP_ARM_ARM.Connect( pBundle, 1 );
	pSUPER_ZIP_BKUP_ARM_TB.Connect( pBundle, 2 );
	pDA_BKUP_ROT_DN.Connect( pBundle, 3 );
	pDA_BKUP_ROT_UP.Connect( pBundle, 4 );
	pPosition_BKUP_TB.Connect( pBundle, 5 );
	pLOGIC_BKUP_PWR_ON.Connect( pBundle, 6 );
	pSSP_BKUP_PWR_ON.Connect( pBundle, 7 );
	pMECH_BKUP_PWR_OFF.Connect( pBundle, 8 );
	pMECH_BKUP_PWR_ON.Connect( pBundle, 9 );
	pMECH_BKUP_PWR_TB.Connect( pBundle, 10 );
	
	// define rotation animation
	if (isGPrime) DefineCISSGPrimeAnimations();
	else DefineCISSGAnimations();
	return;
}


void CISS::OnPreStep( double SimT, double DeltaT, double MJD )
{
	// inputs
	if (pSSP_PRI_PWR_ON.IsSet())
	{
		bMECH_PRI_PWR = pMECH_PRI_PWR_ON.IsSet() | (bMECH_PRI_PWR & !pMECH_PRI_PWR_OFF.IsSet());

		bDA_PRI_UP = (pDA_PRI_ROT_UP.IsSet() | bDA_PRI_UP) & (pLOGIC_PRI_PWR_ON.IsSet() & bMECH_PRI_PWR);
		bDA_PRI_DN = (pDA_PRI_ROT_DN.IsSet() | bDA_PRI_DN) & (pLOGIC_PRI_PWR_ON.IsSet() & bMECH_PRI_PWR);

		bSUPER_ZIP_FIRE = bSUPER_ZIP_FIRE | (pSUPER_ZIP_PRI_FIRE.IsSet() & pSUPER_ZIP_PRI_ARM_ARM.IsSet());
	}
	if (pSSP_BKUP_PWR_ON.IsSet())
	{
		bMECH_BKUP_PWR = pMECH_BKUP_PWR_ON.IsSet() | (bMECH_BKUP_PWR & !pMECH_BKUP_PWR_OFF.IsSet());

		bDA_BKUP_UP = (pDA_BKUP_ROT_UP.IsSet() | bDA_BKUP_UP) & (pLOGIC_BKUP_PWR_ON.IsSet() & bMECH_BKUP_PWR);
		bDA_BKUP_DN = (pDA_BKUP_ROT_DN.IsSet() | bDA_BKUP_DN) & (pLOGIC_BKUP_PWR_ON.IsSet() & bMECH_BKUP_PWR);

		bSUPER_ZIP_FIRE = bSUPER_ZIP_FIRE | (pSUPER_ZIP_BKUP_FIRE.IsSet() & pSUPER_ZIP_BKUP_ARM_ARM.IsSet());
	}

	// table rotation
	double da = DeltaT * CISS_ROTATION_RATE * ((int)(bDA_PRI_UP | bDA_BKUP_UP) - (int)(bDA_PRI_DN | bDA_BKUP_DN));

	if (da > 0)
	{
		rotateAnimState.action = AnimState::OPENING;
		rotateAnimState.Move( da );
		RunAnimation();

		if (rotateAnimState.pos == 1)
		{
			bDA_PRI_UP = false;
			bDA_BKUP_UP = false;
		}
	}
	else if (da < 0)
	{
		rotateAnimState.action = AnimState::CLOSING;
		rotateAnimState.Move( -da );
		RunAnimation();

		if (rotateAnimState.pos == 0)
		{
			bDA_PRI_DN = false;
			bDA_BKUP_DN = false;
		}
	}
	else rotateAnimState.action = AnimState::STOPPED;

	// release
	if (bSUPER_ZIP_FIRE) STS()->DetachChild( ahToCentaur, 0.3 );


	// outputs
	if (pSSP_PRI_PWR_ON.IsSet())
	{
		pSUPER_ZIP_PRI_ARM_TB.SetLine( (int)pSUPER_ZIP_PRI_ARM_ARM * 5.0f );
		if ((rotateAnimState.pos == 1) || (rotateAnimState.pos == 0)) pPosition_PRI_TB.SetLine();
		else pPosition_PRI_TB.ResetLine();
		pMECH_PRI_PWR_TB.SetLine( (int)bMECH_PRI_PWR * 5.0f );
	}
	else
	{
		pSUPER_ZIP_PRI_ARM_TB.ResetLine();
		pPosition_PRI_TB.ResetLine();
		pMECH_PRI_PWR_TB.ResetLine();
	}
	if (pSSP_BKUP_PWR_ON.IsSet())
	{
		pSUPER_ZIP_BKUP_ARM_TB.SetLine( (int)pSUPER_ZIP_BKUP_ARM_ARM * 5.0f );
		if ((rotateAnimState.pos == 1) || (rotateAnimState.pos == 0)) pPosition_BKUP_TB.SetLine();
		else pPosition_BKUP_TB.ResetLine();
		pMECH_BKUP_PWR_TB.SetLine( (int)bMECH_BKUP_PWR * 5.0f );
	}
	else
	{
		pSUPER_ZIP_BKUP_ARM_TB.ResetLine();
		pPosition_BKUP_TB.ResetLine();
		pMECH_BKUP_PWR_TB.ResetLine();
	}
	return;
}

void CISS::OnPostStep( double SimT, double DeltaT, double MJD )
{
	if(bFirstStep) { // required to make sure Centaur is positioned correctly
		bFirstStep = false;
		RunAnimation();
	}
}

bool CISS::OnParseLine( const char* line )
{
	if (!_strnicmp(line, "ROTATION", 8))
	{
		sscan_state((char*)(line+8), rotateAnimState);
		return true;
	}
	else return false;
}

void CISS::OnSaveState( FILEHANDLE scn ) const
{
	WriteScenario_state(scn, "ROTATION", rotateAnimState);
	return;
}

double CISS::GetSubsystemEmptyMass() const
{
	if (isGPrime) return CISS_GPRIME_EMPTY_MASS;
	else return CISS_G_EMPTY_MASS;
}

void CISS::AddMesh()
{
	VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + CISSoffset;
	mesh_idx = STS()->AddMesh( hMesh, &ofs );
	STS()->SetMeshVisibilityMode( mesh_idx, MESHVIS_EXTERNAL | MESHVIS_VC | MESHVIS_EXTPASS );

	ofs = STS()->GetOrbiterCoGOffset() + CISS_MISSION_KIT_OFFSET;
	mesh_idx_MK = STS()->AddMesh( hMesh_MK, &ofs );
	STS()->SetMeshVisibilityMode( mesh_idx_MK, MESHVIS_EXTERNAL | MESHVIS_VC | MESHVIS_EXTPASS );
	return;
}

void CISS::DefineCISSGPrimeAnimations()
{
	anim_rotate = STS()->CreateAnimation( 0.0 );

	/////// tilt table ///////
	static UINT DA_GRP[11] = {GRP_DEPLOYMENT_ADAPTER_GPRIME,
		GRP_PROPELLANT_DISCONNECT_PANELS_GPRIME,
		GRP_LH2_PROPELLANT_DISCONNECT_GPRIME,
		GRP_GH2_PROPELLANT_DISCONNECT_GPRIME,
		GRP_LOX_PROPELLANT_DISCONNECT_GPRIME,
		GRP_GOX_PROPELLANT_DISCONNECT_GPRIME,
		GRP_LH2_FILL_DRAIN_SEGMENT3_GPRIME,
		GRP_GH2_VENT_SEGMENT3_GPRIME,
		GRP_LOX_FILL_DRAIN_SEGMENT3_GPRIME,
		GRP_GOX_VENT_SEGMENT3_GPRIME,
		GRP_LH2_DUCT_BELLOW1A_GPRIME};
	static MGROUP_ROTATE DA( mesh_idx, DA_GRP, 11, CISS_GPRIME_DA_ROTATION_AXIS, _V( -1, 0, 0 ), static_cast<float>(45.0*RAD) );
	ANIMATIONCOMPONENT_HANDLE parent = STS()->AddAnimationComponent( anim_rotate, 0, 1, &DA );

	/////// attach points ///////
	static MGROUP_ROTATE AttachmentTrack( LOCALVERTEXLIST, MAKEGROUPARRAY(centaurAttachment), 3, CISS_GPRIME_DA_ROTATION_AXIS + CISSoffset, _V( -1, 0, 0 ), static_cast<float>(45.0*RAD) );
	STS()->AddAnimationComponent( anim_rotate, 0, 1, &AttachmentTrack );

	STS()->SetAnimation( anim_rotate, rotateAnimState.pos );
}

void CISS::DefineCISSGAnimations()
{
	anim_rotate = STS()->CreateAnimation( 0.0 );

	/////// tilt table ///////
	static UINT DA_GRP[10] = {GRP_DEPLOYMENT_ADAPTER_G,
		GRP_PROPELLANT_DISCONNECT_PANELS_G,
		GRP_LH2_PROPELLANT_DISCONNECT_G,
		GRP_GH2_PROPELLANT_DISCONNECT_G,
		GRP_LOX_PROPELLANT_DISCONNECT_G,
		GRP_GOX_PROPELLANT_DISCONNECT_G,
		GRP_LH2_FILL_DRAIN_SEGMENT3_G,
		GRP_GH2_VENT_SEGMENT3_G,
		GRP_LOX_FILL_DRAIN_SEGMENT3_G,
		GRP_GOX_VENT_SEGMENT3_G};
	static MGROUP_ROTATE DA( mesh_idx, DA_GRP, 10, CISS_G_DA_ROTATION_AXIS, _V( -1, 0, 0 ), static_cast<float>(45.0*RAD) );
	STS()->AddAnimationComponent( anim_rotate, 0, 1, &DA );

	/////// attach points ///////
	static MGROUP_ROTATE AttachmentTrack( LOCALVERTEXLIST, MAKEGROUPARRAY(centaurAttachment), 3, CISS_G_DA_ROTATION_AXIS + CISSoffset, _V( -1, 0, 0 ), static_cast<float>(45.0*RAD) );
	STS()->AddAnimationComponent( anim_rotate, 0, 1, &AttachmentTrack );

	STS()->SetAnimation( anim_rotate, rotateAnimState.pos );
}

void CISS::CreateAttachment()
{
	if (ahToCentaur == NULL)
	{
		centaurAttachment[0] = STS()->GetOrbiterCoGOffset() + Centaurattachoffset;
		centaurAttachment[1] = centaurAttachment[0] + _V( 0, 0, 1 );
		centaurAttachment[2] = centaurAttachment[0] + _V( 0, 1, 0 );
		if (isGPrime) ahToCentaur = STS()->CreateAttachment( false, centaurAttachment[0], _V( 0, 0, 1 ), _V( 0, 1, 0 ), "SSU_CGP" );
		else ahToCentaur = STS()->CreateAttachment( false, centaurAttachment[0], _V( 0, 0, 1 ), _V( 0, 1, 0 ), "SSU_CG" );
	}
	return;
}

void CISS::RunAnimation()
{
	STS()->SetAnimation( anim_rotate, rotateAnimState.pos );
	STS()->SetAttachmentParams( ahToCentaur, centaurAttachment[0] + STS()->GetOrbiterCoGOffset(), centaurAttachment[1] - centaurAttachment[0], centaurAttachment[2] - centaurAttachment[0] );
	return;
}
