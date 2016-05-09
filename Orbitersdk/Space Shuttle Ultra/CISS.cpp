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
	motorcrankAnimState.Set( AnimState::CLOSED, 0.0 );
	linkAnimState.Set( AnimState::CLOSED, 0.0 );
	lox_aAnimState.Set( AnimState::CLOSED, 0.0 );
	lox_a2AnimState.Set( AnimState::CLOSED, 0.0 );
	lox_bAnimState.Set( AnimState::CLOSED, 0.0 );
	lox_b2AnimState.Set( AnimState::CLOSED, 0.0 );
	gox_aAnimState.Set( AnimState::CLOSED, 0.0 );
	gox_a2AnimState.Set( AnimState::CLOSED, 0.0 );
	gox_bAnimState.Set( AnimState::CLOSED, 0.0 );
	gox_b2AnimState.Set( AnimState::CLOSED, 0.0 );
	lh2_aAnimState.Set( AnimState::CLOSED, 0.0 );
	lh2_a2AnimState.Set( AnimState::CLOSED, 0.0 );
	lh2_bAnimState.Set( AnimState::CLOSED, 0.0 );
	lh2_b2AnimState.Set( AnimState::CLOSED, 0.0 );
	gh2_aAnimState.Set( AnimState::CLOSED, 0.0 );
	gh2_a2AnimState.Set( AnimState::CLOSED, 0.0 );
	gh2_bAnimState.Set( AnimState::CLOSED, 0.0 );
	gh2_b2AnimState.Set( AnimState::CLOSED, 0.0 );

	hMesh_MK = oapiLoadMeshGlobal( CISS_MISSION_KIT_MESHNAME );

	if (isGPrime)
	{
		hMesh = oapiLoadMeshGlobal( CISS_GPRIME_MESHNAME );
		CISSoffset = CISS_GPRIME_OFFSET;
		Centaurattachoffset = CENTAUR_GPRIME_ATTACHMENT_OFFSET;

		double r1 = sqrt( pow( CISS_GPRIME_P4.z - CISS_GPRIME_P1.z, 2 ) + pow( CISS_GPRIME_P4.y - CISS_GPRIME_P1.y, 2 ) );
		double theta1 = atan2( CISS_GPRIME_P4.y - CISS_GPRIME_P1.y, CISS_GPRIME_P1.z - CISS_GPRIME_P4.z );
		x1 = r1 * cos( theta1 );
		y1 = r1 * sin( theta1 );
		r2 = sqrt( pow( CISS_GPRIME_P2.z - CISS_GPRIME_P1.z, 2 ) + pow( CISS_GPRIME_P2.y - CISS_GPRIME_P1.y, 2 ) );
		theta2o = atan2( CISS_GPRIME_P2.y - CISS_GPRIME_P1.y, CISS_GPRIME_P1.z - CISS_GPRIME_P2.z );
		r3 = sqrt( pow( CISS_GPRIME_P3.z - CISS_GPRIME_P2.z, 2 ) + pow( CISS_GPRIME_P3.y - CISS_GPRIME_P2.y, 2 ) );
		theta3o = atan2( CISS_GPRIME_P3.y - CISS_GPRIME_P2.y, CISS_GPRIME_P2.z - CISS_GPRIME_P3.z );
		r4 = sqrt( pow( CISS_GPRIME_P4.z - CISS_GPRIME_P3.z, 2 ) + pow( CISS_GPRIME_P4.y - CISS_GPRIME_P3.y, 2 ) );
		theta4o = atan2( CISS_GPRIME_P4.y - CISS_GPRIME_P3.y, CISS_GPRIME_P3.z - CISS_GPRIME_P4.z ) + PI;

		double r7 = sqrt( pow( CISS_GPRIME_P7.z - CISS_GPRIME_P1.z, 2 ) + pow( CISS_GPRIME_P7.y - CISS_GPRIME_P1.y, 2 ) );
		double theta7 = atan2( CISS_GPRIME_P7.y - CISS_GPRIME_P1.y, CISS_GPRIME_P1.z - CISS_GPRIME_P7.z );
		x7 = r7 * cos( theta7 );
		y7 = r7 * sin( theta7 );
		r2_5 = sqrt( pow( CISS_GPRIME_P5.z - CISS_GPRIME_P1.z, 2 ) + pow( CISS_GPRIME_P5.y - CISS_GPRIME_P1.y, 2 ) );
		theta2_5o = atan2( CISS_GPRIME_P5.y - CISS_GPRIME_P1.y, CISS_GPRIME_P1.z - CISS_GPRIME_P5.z );
		r5 = sqrt( pow( CISS_GPRIME_P6.z - CISS_GPRIME_P5.z, 2 ) + pow( CISS_GPRIME_P6.y - CISS_GPRIME_P5.y, 2 ) );
		theta5o = atan2( CISS_GPRIME_P6.y - CISS_GPRIME_P5.y, CISS_GPRIME_P5.z - CISS_GPRIME_P6.z );
		r6 = sqrt( pow( CISS_GPRIME_P6.z - CISS_GPRIME_P7.z, 2 ) + pow( CISS_GPRIME_P6.y - CISS_GPRIME_P7.y, 2 ) );
		theta6o = atan2( CISS_GPRIME_P6.y - CISS_GPRIME_P7.y, CISS_GPRIME_P7.z - CISS_GPRIME_P6.z );

		double r10 = sqrt( pow( CISS_GPRIME_P10.z - CISS_GPRIME_P1.z, 2 ) + pow( CISS_GPRIME_P10.y - CISS_GPRIME_P1.y, 2 ) );
		double theta10 = atan2( CISS_GPRIME_P10.y - CISS_GPRIME_P1.y, CISS_GPRIME_P1.z - CISS_GPRIME_P10.z );
		x10 = r10 * cos( theta10 );
		y10 = r10 * sin( theta10 );
		r2_8 = sqrt( pow( CISS_GPRIME_P8.z - CISS_GPRIME_P1.z, 2 ) + pow( CISS_GPRIME_P8.y - CISS_GPRIME_P1.y, 2 ) );
		theta2_8o = atan2( CISS_GPRIME_P8.y - CISS_GPRIME_P1.y, CISS_GPRIME_P1.z - CISS_GPRIME_P8.z );
		r8 = sqrt( pow( CISS_GPRIME_P9.z - CISS_GPRIME_P8.z, 2 ) + pow( CISS_GPRIME_P9.y - CISS_GPRIME_P8.y, 2 ) );
		theta8o = atan2( CISS_GPRIME_P9.y - CISS_GPRIME_P8.y, CISS_GPRIME_P8.z - CISS_GPRIME_P9.z );
		r9 = sqrt( pow( CISS_GPRIME_P9.z - CISS_GPRIME_P10.z, 2 ) + pow( CISS_GPRIME_P9.y - CISS_GPRIME_P10.y, 2 ) );
		theta9o = atan2( CISS_GPRIME_P9.y - CISS_GPRIME_P10.y, CISS_GPRIME_P10.z - CISS_GPRIME_P9.z );

		double r13 = sqrt( pow( CISS_GPRIME_P13.z - CISS_GPRIME_P1.z, 2 ) + pow( CISS_GPRIME_P13.y - CISS_GPRIME_P1.y, 2 ) );
		double theta13 = atan2( CISS_GPRIME_P13.y - CISS_GPRIME_P1.y, CISS_GPRIME_P1.z - CISS_GPRIME_P13.z );
		x13 = r13 * cos( theta13 );
		y13 = r13 * sin( theta13 );
		r2_11 = sqrt( pow( CISS_GPRIME_P11.z - CISS_GPRIME_P1.z, 2 ) + pow( CISS_GPRIME_P11.y - CISS_GPRIME_P1.y, 2 ) );
		theta2_11o = atan2( CISS_GPRIME_P11.y - CISS_GPRIME_P1.y, CISS_GPRIME_P1.z - CISS_GPRIME_P11.z );
		r11 = sqrt( pow( CISS_GPRIME_P12.z - CISS_GPRIME_P11.z, 2 ) + pow( CISS_GPRIME_P12.y - CISS_GPRIME_P11.y, 2 ) );
		theta11o = atan2( CISS_GPRIME_P12.y - CISS_GPRIME_P11.y, CISS_GPRIME_P11.z - CISS_GPRIME_P12.z );
		r12 = sqrt( pow( CISS_GPRIME_P12.z - CISS_GPRIME_P13.z, 2 ) + pow( CISS_GPRIME_P12.y - CISS_GPRIME_P13.y, 2 ) );
		theta12o = atan2( CISS_GPRIME_P12.y - CISS_GPRIME_P13.y, CISS_GPRIME_P13.z - CISS_GPRIME_P12.z );

		double r16 = sqrt( pow( CISS_GPRIME_P16.z - CISS_GPRIME_P1.z, 2 ) + pow( CISS_GPRIME_P16.y - CISS_GPRIME_P1.y, 2 ) );
		double theta16 = atan2( CISS_GPRIME_P16.y - CISS_GPRIME_P1.y, CISS_GPRIME_P1.z - CISS_GPRIME_P16.z );
		x16 = r16 * cos( theta16 );
		y16 = r16 * sin( theta16 );
		r2_14 = sqrt( pow( CISS_GPRIME_P14.z - CISS_GPRIME_P1.z, 2 ) + pow( CISS_GPRIME_P14.y - CISS_GPRIME_P1.y, 2 ) );
		theta2_14o = atan2( CISS_GPRIME_P14.y - CISS_GPRIME_P1.y, CISS_GPRIME_P1.z - CISS_GPRIME_P14.z );
		r14 = sqrt( pow( CISS_GPRIME_P15.z - CISS_GPRIME_P14.z, 2 ) + pow( CISS_GPRIME_P15.y - CISS_GPRIME_P14.y, 2 ) );
		theta14o = atan2( CISS_GPRIME_P15.y - CISS_GPRIME_P14.y, CISS_GPRIME_P14.z - CISS_GPRIME_P15.z );
		r15 = sqrt( pow( CISS_GPRIME_P15.z - CISS_GPRIME_P16.z, 2 ) + pow( CISS_GPRIME_P15.y - CISS_GPRIME_P16.y, 2 ) );
		theta15o = atan2( CISS_GPRIME_P15.y - CISS_GPRIME_P16.y, CISS_GPRIME_P16.z - CISS_GPRIME_P15.z );
	}
	else
	{
		hMesh = oapiLoadMeshGlobal( CISS_G_MESHNAME );
		CISSoffset = CISS_G_OFFSET;
		Centaurattachoffset = CENTAUR_G_ATTACHMENT_OFFSET;

		double r1 = sqrt( pow( CISS_G_P4.z - CISS_G_P1.z, 2 ) + pow( CISS_G_P4.y - CISS_G_P1.y, 2 ) );
		double theta1 = atan2( CISS_G_P4.y - CISS_G_P1.y, CISS_G_P1.z - CISS_G_P4.z );
		x1 = r1 * cos( theta1 );
		y1 = r1 * sin( theta1 );
		r2 = sqrt( pow( CISS_G_P2.z - CISS_G_P1.z, 2 ) + pow( CISS_G_P2.y - CISS_G_P1.y, 2 ) );
		theta2o = atan2( CISS_G_P2.y - CISS_G_P1.y, CISS_G_P1.z - CISS_G_P2.z );
		r3 = sqrt( pow( CISS_G_P3.z - CISS_G_P2.z, 2 ) + pow( CISS_G_P3.y - CISS_G_P2.y, 2 ) );
		theta3o = atan2( CISS_G_P3.y - CISS_G_P2.y, CISS_G_P2.z - CISS_G_P3.z );
		r4 = sqrt( pow( CISS_G_P4.z - CISS_G_P3.z, 2 ) + pow( CISS_G_P4.y - CISS_G_P3.y, 2 ) );
		theta4o = atan2( CISS_G_P4.y - CISS_G_P3.y, CISS_G_P3.z - CISS_G_P4.z ) + PI;

		double r7 = sqrt( pow( CISS_G_P7.z - CISS_G_P1.z, 2 ) + pow( CISS_G_P7.y - CISS_G_P1.y, 2 ) );
		double theta7 = atan2( CISS_G_P7.y - CISS_G_P1.y, CISS_G_P1.z - CISS_G_P7.z );
		x7 = r7 * cos( theta7 );
		y7 = r7 * sin( theta7 );
		r2_5 = sqrt( pow( CISS_G_P5.z - CISS_G_P1.z, 2 ) + pow( CISS_G_P5.y - CISS_G_P1.y, 2 ) );
		theta2_5o = atan2( CISS_G_P5.y - CISS_G_P1.y, CISS_G_P1.z - CISS_G_P5.z );
		r5 = sqrt( pow( CISS_G_P6.z - CISS_G_P5.z, 2 ) + pow( CISS_G_P6.y - CISS_G_P5.y, 2 ) );
		theta5o = atan2( CISS_G_P6.y - CISS_G_P5.y, CISS_G_P5.z - CISS_G_P6.z );
		r6 = sqrt( pow( CISS_G_P6.z - CISS_G_P7.z, 2 ) + pow( CISS_G_P6.y - CISS_G_P7.y, 2 ) );
		theta6o = atan2( CISS_G_P6.y - CISS_G_P7.y, CISS_G_P7.z - CISS_G_P6.z );

		double r10 = sqrt( pow( CISS_G_P10.z - CISS_G_P1.z, 2 ) + pow( CISS_G_P10.y - CISS_G_P1.y, 2 ) );
		double theta10 = atan2( CISS_G_P10.y - CISS_G_P1.y, CISS_G_P1.z - CISS_G_P10.z );
		x10 = r10 * cos( theta10 );
		y10 = r10 * sin( theta10 );
		r2_8 = sqrt( pow( CISS_G_P8.z - CISS_G_P1.z, 2 ) + pow( CISS_G_P8.y - CISS_G_P1.y, 2 ) );
		theta2_8o = atan2( CISS_G_P8.y - CISS_G_P1.y, CISS_G_P1.z - CISS_G_P8.z );
		r8 = sqrt( pow( CISS_G_P9.z - CISS_G_P8.z, 2 ) + pow( CISS_G_P9.y - CISS_G_P8.y, 2 ) );
		theta8o = atan2( CISS_G_P9.y - CISS_G_P8.y, CISS_G_P8.z - CISS_G_P9.z );
		r9 = sqrt( pow( CISS_G_P9.z - CISS_G_P10.z, 2 ) + pow( CISS_G_P9.y - CISS_G_P10.y, 2 ) );
		theta9o = atan2( CISS_G_P9.y - CISS_G_P10.y, CISS_G_P10.z - CISS_G_P9.z );

		double r13 = sqrt( pow( CISS_G_P13.z - CISS_G_P1.z, 2 ) + pow( CISS_G_P13.y - CISS_G_P1.y, 2 ) );
		double theta13 = atan2( CISS_G_P13.y - CISS_G_P1.y, CISS_G_P1.z - CISS_G_P13.z );
		x13 = r13 * cos( theta13 );
		y13 = r13 * sin( theta13 );
		r2_11 = sqrt( pow( CISS_G_P11.z - CISS_G_P1.z, 2 ) + pow( CISS_G_P11.y - CISS_G_P1.y, 2 ) );
		theta2_11o = atan2( CISS_G_P11.y - CISS_G_P1.y, CISS_G_P1.z - CISS_G_P11.z );
		r11 = sqrt( pow( CISS_G_P12.z - CISS_G_P11.z, 2 ) + pow( CISS_G_P12.y - CISS_G_P11.y, 2 ) );
		theta11o = atan2( CISS_G_P12.y - CISS_G_P11.y, CISS_G_P11.z - CISS_G_P12.z );
		r12 = sqrt( pow( CISS_G_P12.z - CISS_G_P13.z, 2 ) + pow( CISS_G_P12.y - CISS_G_P13.y, 2 ) );
		theta12o = atan2( CISS_G_P12.y - CISS_G_P13.y, CISS_G_P13.z - CISS_G_P12.z );

		double r16 = sqrt( pow( CISS_G_P16.z - CISS_G_P1.z, 2 ) + pow( CISS_G_P16.y - CISS_G_P1.y, 2 ) );
		double theta16 = atan2( CISS_G_P16.y - CISS_G_P1.y, CISS_G_P1.z - CISS_G_P16.z );
		x16 = r16 * cos( theta16 );
		y16 = r16 * sin( theta16 );
		r2_14 = sqrt( pow( CISS_G_P14.z - CISS_G_P1.z, 2 ) + pow( CISS_G_P14.y - CISS_G_P1.y, 2 ) );
		theta2_14o = atan2( CISS_G_P14.y - CISS_G_P1.y, CISS_G_P1.z - CISS_G_P14.z );
		r14 = sqrt( pow( CISS_G_P15.z - CISS_G_P14.z, 2 ) + pow( CISS_G_P15.y - CISS_G_P14.y, 2 ) );
		theta14o = atan2( CISS_G_P15.y - CISS_G_P14.y, CISS_G_P14.z - CISS_G_P15.z );
		r15 = sqrt( pow( CISS_G_P15.z - CISS_G_P16.z, 2 ) + pow( CISS_G_P15.y - CISS_G_P16.y, 2 ) );
		theta15o = atan2( CISS_G_P15.y - CISS_G_P16.y, CISS_G_P16.z - CISS_G_P15.z );
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
	double da = DeltaT * CISS_MOTOR_ROTATION_RATE * ((int)(bDA_PRI_UP | bDA_BKUP_UP) - (int)(bDA_PRI_DN | bDA_BKUP_DN));
	if (da > 0)
	{
		motorcrankAnimState.action = AnimState::OPENING;
		motorcrankAnimState.Move( da );
		CalcAnimation();
		RunAnimation();

		if (motorcrankAnimState.pos == 1)
		{
			bDA_PRI_UP = false;
			bDA_BKUP_UP = false;
		}
	}
	else if (da < 0)
	{
		motorcrankAnimState.action = AnimState::CLOSING;
		motorcrankAnimState.Move( -da );
		CalcAnimation();
		RunAnimation();

		if (motorcrankAnimState.pos == 0)
		{
			bDA_PRI_DN = false;
			bDA_BKUP_DN = false;
		}
	}
	else motorcrankAnimState.action = AnimState::STOPPED;

	// release
	if (bSUPER_ZIP_FIRE) STS()->DetachChild( ahToCentaur, 0.3 );

	// outputs
	if (pSSP_PRI_PWR_ON.IsSet())
	{
		pSUPER_ZIP_PRI_ARM_TB.SetLine( (int)pSUPER_ZIP_PRI_ARM_ARM * 5.0f );
		if ((motorcrankAnimState.pos == 1) || (motorcrankAnimState.pos == 0)) pPosition_PRI_TB.SetLine();
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
		if ((motorcrankAnimState.pos == 1) || (motorcrankAnimState.pos == 0)) pPosition_BKUP_TB.SetLine();
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
		CalcAnimation();
		RunAnimation();
	}
}

bool CISS::OnParseLine( const char* line )
{
	if (!_strnicmp(line, "TABLE", 5))
	{
		sscan_state((char*)(line+5),rotateAnimState);
		return true;
	}
	else if (!_strnicmp(line, "MOTOR", 5))
	{
		sscan_state((char*)(line+5), motorcrankAnimState);
		return true;
	}
	else return false;
}

void CISS::OnSaveState( FILEHANDLE scn ) const
{
	WriteScenario_state(scn, "TABLE", rotateAnimState);
	WriteScenario_state(scn, "MOTOR", motorcrankAnimState);
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
	/////// tilt table ///////
	static UINT DA_GRP[10] = {GRP_DEPLOYMENT_ADAPTER_GPRIME,
		GRP_PROPELLANT_DISCONNECT_PANELS_GPRIME,
		GRP_LH2_PROPELLANT_DISCONNECT_GPRIME,
		GRP_GH2_PROPELLANT_DISCONNECT_GPRIME,
		GRP_LOX_PROPELLANT_DISCONNECT_GPRIME,
		GRP_GOX_PROPELLANT_DISCONNECT_GPRIME,
		GRP_LH2_FILL_DRAIN_SEGMENT3_GPRIME,
		GRP_GH2_VENT_SEGMENT3_GPRIME,
		GRP_LOX_FILL_DRAIN_SEGMENT3_GPRIME,
		GRP_GOX_VENT_SEGMENT3_GPRIME};
	anim_rotate = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE DA( mesh_idx, DA_GRP, 10, CISS_GPRIME_P1, _V( -1, 0, 0 ), static_cast<float>(360.0*RAD) );
	ANIMATIONCOMPONENT_HANDLE parent = STS()->AddAnimationComponent( anim_rotate, 0, 1, &DA );

	/////// attach points ///////
	static MGROUP_ROTATE AttachmentTrack( LOCALVERTEXLIST, MAKEGROUPARRAY(centaurAttachment), 3, CISS_GPRIME_P1 + CISSoffset, _V( -1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_rotate, 0, 1, &AttachmentTrack );
	STS()->SetAnimation( anim_rotate, rotateAnimState.pos );

	/////// motor crank ///////
	static UINT MOTORCRANK_GRP[1] = {GRP_DA_ROTATION_ARM_GPRIME};
	anim_motorcrank = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE MOTORCRANK( mesh_idx, MOTORCRANK_GRP, 1, CISS_GPRIME_P2, _V( 1, 0, 0 ), static_cast<float>(CISS_MOTOR_ROTATION_RANGE) );
	STS()->AddAnimationComponent( anim_motorcrank, 0, 1, &MOTORCRANK, parent );
	STS()->SetAnimation( anim_motorcrank, motorcrankAnimState.pos );

	/////// link ///////
	static UINT LINK_GRP[1] = {GRP_DA_ROTATION_LINK_GPRIME};
	anim_link = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LINK( mesh_idx, LINK_GRP, 1, CISS_GPRIME_P4, _V( -1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_link, 0, 1, &LINK );
	STS()->SetAnimation( anim_link, linkAnimState.pos );

	/////// LOX A ///////
	static UINT LOX_A_GRP[2] = {GRP_LOX_FILL_DRAIN_SEGMENT2_GPRIME, GRP_LOX_DUCT_BELLOW2_GPRIME};
	anim_lox_a = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LOX_A( mesh_idx, LOX_A_GRP, 2, CISS_GPRIME_P5, _V( 1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_lox_a, 0, 1, &LOX_A, parent );
	STS()->SetAnimation( anim_lox_a, lox_aAnimState.pos );

	/////// LOX A2 ///////
	static UINT LOX_A2_GRP[1] = {GRP_LOX_DUCT_BELLOW3_GPRIME};
	anim_lox_a2 = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LOX_A2( mesh_idx, LOX_A2_GRP, 1, CISS_GPRIME_P5, _V( 1, 0, 0 ), static_cast<float>(180.0*RAD) );
	STS()->AddAnimationComponent( anim_lox_a2, 0, 1, &LOX_A2, parent );
	STS()->SetAnimation( anim_lox_a2, lox_a2AnimState.pos );

	/////// LOX B ///////
	static UINT LOX_B_GRP[1] = {GRP_LOX_FILL_DRAIN_SEGMENT1_GPRIME};
	anim_lox_b = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LOX_B( mesh_idx, LOX_B_GRP, 1, CISS_GPRIME_P7, _V( -1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_lox_b, 0, 1, &LOX_B );
	STS()->SetAnimation( anim_lox_b, lox_bAnimState.pos );

	/////// LOX B2 ///////
	static UINT LOX_B2_GRP[1] = {GRP_LOX_DUCT_BELLOW1_GPRIME};
	anim_lox_b2 = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LOX_B2( mesh_idx, LOX_B2_GRP, 1, CISS_GPRIME_P7, _V( -1, 0, 0 ), static_cast<float>(180.0*RAD) );
	STS()->AddAnimationComponent( anim_lox_b2, 0, 1, &LOX_B2 );
	STS()->SetAnimation( anim_lox_b2, lox_b2AnimState.pos );

	/////// GOX A ///////
	static UINT GOX_A_GRP[2] = {GRP_GOX_VENT_SEGMENT2_GPRIME, GRP_GOX_DUCT_BELLOW2_GPRIME};
	anim_gox_a = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE GOX_A( mesh_idx, GOX_A_GRP, 2, CISS_GPRIME_P8, _V( 1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_gox_a, 0, 1, &GOX_A, parent );
	STS()->SetAnimation( anim_gox_a, gox_aAnimState.pos );

	/////// GOX A2 ///////
	static UINT GOX_A2_GRP[1] = {GRP_GOX_DUCT_BELLOW3_GPRIME};
	anim_gox_a2 = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE GOX_A2( mesh_idx, GOX_A2_GRP, 1, CISS_GPRIME_P8, _V( 1, 0, 0 ), static_cast<float>(180.0*RAD) );
	STS()->AddAnimationComponent( anim_gox_a2, 0, 1, &GOX_A2, parent );
	STS()->SetAnimation( anim_gox_a2, gox_a2AnimState.pos );

	/////// GOX B ///////
	static UINT GOX_B_GRP[1] = {GRP_GOX_VENT_SEGMENT1_GPRIME};
	anim_gox_b = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE GOX_B( mesh_idx, GOX_B_GRP, 1, CISS_GPRIME_P10, _V( -1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_gox_b, 0, 1, &GOX_B );
	STS()->SetAnimation( anim_gox_b, gox_bAnimState.pos );

	/////// GOX B2 ///////
	static UINT GOX_B2_GRP[1] = {GRP_GOX_DUCT_BELLOW1_GPRIME};
	anim_gox_b2 = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE GOX_B2( mesh_idx, GOX_B2_GRP, 1, CISS_GPRIME_P10, _V( -1, 0, 0 ), static_cast<float>(180.0*RAD) );
	STS()->AddAnimationComponent( anim_gox_b2, 0, 1, &GOX_B2 );
	STS()->SetAnimation( anim_gox_b2, gox_b2AnimState.pos );

	/////// LH2 A ///////
	static UINT LH2_A_GRP[2] = {GRP_LH2_FILL_DRAIN_SEGMENT2_GPRIME, GRP_LH2_DUCT_BELLOW2_GPRIME};
	anim_lh2_a = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LH2_A( mesh_idx, LH2_A_GRP, 2, CISS_GPRIME_P11, _V( 1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_lh2_a, 0, 1, &LH2_A, parent );
	STS()->SetAnimation( anim_lh2_a, lh2_aAnimState.pos );

	/////// LH2 A2 ///////
	static UINT LH2_A2_GRP[1] = {GRP_LH2_DUCT_BELLOW3_GPRIME};
	anim_lh2_a2 = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LH2_A2( mesh_idx, LH2_A2_GRP, 1, CISS_GPRIME_P11, _V( 1, 0, 0 ), static_cast<float>(180.0*RAD) );
	STS()->AddAnimationComponent( anim_lh2_a2, 0, 1, &LH2_A2, parent );
	STS()->SetAnimation( anim_lh2_a2, lh2_a2AnimState.pos );

	/////// LH2 B ///////
	static UINT LH2_B_GRP[1] = {GRP_LH2_FILL_DRAIN_SEGMENT1_GPRIME};
	anim_lh2_b = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LH2_B( mesh_idx, LH2_B_GRP, 1, CISS_GPRIME_P13, _V( -1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_lh2_b, 0, 1, &LH2_B );
	STS()->SetAnimation( anim_lh2_b, lh2_bAnimState.pos );

	/////// LH2 B2 ///////
	static UINT LH2_B2_GRP[1] = {GRP_LH2_DUCT_BELLOW1_GPRIME};
	anim_lh2_b2 = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LH2_B2( mesh_idx, LH2_B2_GRP, 1, CISS_GPRIME_P13, _V( -1, 0, 0 ), static_cast<float>(180.0*RAD) );
	STS()->AddAnimationComponent( anim_lh2_b2, 0, 1, &LH2_B2 );
	STS()->SetAnimation( anim_lh2_b2, lh2_b2AnimState.pos );

	/////// GH2 A ///////
	static UINT GH2_A_GRP[2] = {GRP_GH2_VENT_SEGMENT2_GPRIME, GRP_GH2_DUCT_BELLOW2_GPRIME};
	anim_gh2_a = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE GH2_A( mesh_idx, GH2_A_GRP, 2, CISS_GPRIME_P14, _V( 1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_gh2_a, 0, 1, &GH2_A, parent );
	STS()->SetAnimation( anim_gh2_a, gh2_aAnimState.pos );

	/////// GH2 A2 ///////
	static UINT GH2_A2_GRP[1] = {GRP_GH2_DUCT_BELLOW3_GPRIME};
	anim_gh2_a2 = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE GH2_A2( mesh_idx, GH2_A2_GRP, 1, CISS_GPRIME_P14, _V( 1, 0, 0 ), static_cast<float>(180.0*RAD) );
	STS()->AddAnimationComponent( anim_gh2_a2, 0, 1, &GH2_A2, parent );
	STS()->SetAnimation( anim_gh2_a2, gh2_a2AnimState.pos );

	/////// GH2 B ///////
	static UINT GH2_B_GRP[1] = {GRP_GH2_VENT_SEGMENT1_GPRIME};
	anim_gh2_b = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE GH2_B( mesh_idx, GH2_B_GRP, 1, CISS_GPRIME_P16, _V( -1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_gh2_b, 0, 1, &GH2_B );
	STS()->SetAnimation( anim_gh2_b, gh2_bAnimState.pos );

	/////// GH2 B2 ///////
	static UINT GH2_B2_GRP[1] = {GRP_GH2_DUCT_BELLOW1_GPRIME};
	anim_gh2_b2 = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE GH2_B2( mesh_idx, GH2_B2_GRP, 1, CISS_GPRIME_P16, _V( -1, 0, 0 ), static_cast<float>(180.0*RAD) );
	STS()->AddAnimationComponent( anim_gh2_b2, 0, 1, &GH2_B2 );
	STS()->SetAnimation( anim_gh2_b2, gh2_b2AnimState.pos );
}

void CISS::DefineCISSGAnimations()
{
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
	anim_rotate = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE DA( mesh_idx, DA_GRP, 10, CISS_G_P1, _V( -1, 0, 0 ), static_cast<float>(360.0*RAD) );
	ANIMATIONCOMPONENT_HANDLE parent = STS()->AddAnimationComponent( anim_rotate, 0, 1, &DA );

	/////// attach points ///////
	static MGROUP_ROTATE AttachmentTrack( LOCALVERTEXLIST, MAKEGROUPARRAY(centaurAttachment), 3, CISS_G_P1 + CISSoffset, _V( -1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_rotate, 0, 1, &AttachmentTrack );
	STS()->SetAnimation( anim_rotate, rotateAnimState.pos );

	/////// motor crank ///////
	static UINT MOTORCRANK_GRP[1] = {GRP_DA_ROTATION_ARM_G};
	anim_motorcrank = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE MOTORCRANK( mesh_idx, MOTORCRANK_GRP, 1, CISS_G_P2, _V( 1, 0, 0 ), static_cast<float>(CISS_MOTOR_ROTATION_RANGE) );
	STS()->AddAnimationComponent( anim_motorcrank, 0, 1, &MOTORCRANK, parent );
	STS()->SetAnimation( anim_motorcrank, motorcrankAnimState.pos );

	/////// link ///////
	static UINT LINK_GRP[1] = {GRP_DA_ROTATION_LINK_G};
	anim_link = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LINK( mesh_idx, LINK_GRP, 1, CISS_G_P4, _V( -1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_link, 0, 1, &LINK );
	STS()->SetAnimation( anim_link, linkAnimState.pos );

	/////// LOX A ///////
	static UINT LOX_A_GRP[2] = {GRP_LOX_FILL_DRAIN_SEGMENT2_G, GRP_LOX_DUCT_BELLOW2_G};
	anim_lox_a = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LOX_A( mesh_idx, LOX_A_GRP, 2, CISS_G_P5, _V( 1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_lox_a, 0, 1, &LOX_A, parent );
	STS()->SetAnimation( anim_lox_a, lox_aAnimState.pos );

	/////// LOX A2 ///////
	static UINT LOX_A2_GRP[1] = {GRP_LOX_DUCT_BELLOW3_G};
	anim_lox_a2 = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LOX_A2( mesh_idx, LOX_A2_GRP, 1, CISS_G_P5, _V( 1, 0, 0 ), static_cast<float>(180.0*RAD) );
	STS()->AddAnimationComponent( anim_lox_a2, 0, 1, &LOX_A2, parent );
	STS()->SetAnimation( anim_lox_a2, lox_a2AnimState.pos );

	/////// LOX B ///////
	static UINT LOX_B_GRP[1] = {GRP_LOX_FILL_DRAIN_SEGMENT1_G};
	anim_lox_b = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LOX_B( mesh_idx, LOX_B_GRP, 1, CISS_G_P7, _V( -1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_lox_b, 0, 1, &LOX_B );
	STS()->SetAnimation( anim_lox_b, lox_bAnimState.pos );

	/////// LOX B2 ///////
	static UINT LOX_B2_GRP[1] = {GRP_LOX_DUCT_BELLOW1_G};
	anim_lox_b2 = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LOX_B2( mesh_idx, LOX_B2_GRP, 1, CISS_G_P7, _V( -1, 0, 0 ), static_cast<float>(180.0*RAD) );
	STS()->AddAnimationComponent( anim_lox_b2, 0, 1, &LOX_B2 );
	STS()->SetAnimation( anim_lox_b2, lox_b2AnimState.pos );

	/////// GOX A ///////
	static UINT GOX_A_GRP[2] = {GRP_GOX_VENT_SEGMENT2_G, GRP_GOX_DUCT_BELLOW2_G};
	anim_gox_a = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE GOX_A( mesh_idx, GOX_A_GRP, 2, CISS_G_P8, _V( 1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_gox_a, 0, 1, &GOX_A, parent );
	STS()->SetAnimation( anim_gox_a, gox_aAnimState.pos );

	/////// GOX A2 ///////
	static UINT GOX_A2_GRP[1] = {GRP_GOX_DUCT_BELLOW3_G};
	anim_gox_a2 = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE GOX_A2( mesh_idx, GOX_A2_GRP, 1, CISS_G_P8, _V( 1, 0, 0 ), static_cast<float>(180.0*RAD) );
	STS()->AddAnimationComponent( anim_gox_a2, 0, 1, &GOX_A2, parent );
	STS()->SetAnimation( anim_gox_a2, gox_a2AnimState.pos );

	/////// GOX B ///////
	static UINT GOX_B_GRP[1] = {GRP_GOX_VENT_SEGMENT1_G};
	anim_gox_b = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE GOX_B( mesh_idx, GOX_B_GRP, 1, CISS_G_P10, _V( -1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_gox_b, 0, 1, &GOX_B );
	STS()->SetAnimation( anim_gox_b, gox_bAnimState.pos );

	/////// GOX B2 ///////
	static UINT GOX_B2_GRP[1] = {GRP_GOX_DUCT_BELLOW1_G};
	anim_gox_b2 = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE GOX_B2( mesh_idx, GOX_B2_GRP, 1, CISS_G_P10, _V( -1, 0, 0 ), static_cast<float>(180.0*RAD) );
	STS()->AddAnimationComponent( anim_gox_b2, 0, 1, &GOX_B2 );
	STS()->SetAnimation( anim_gox_b2, gox_b2AnimState.pos );

	/////// LH2 A ///////
	static UINT LH2_A_GRP[2] = {GRP_LH2_FILL_DRAIN_SEGMENT2_G, GRP_LH2_DUCT_BELLOW2_G};
	anim_lh2_a = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LH2_A( mesh_idx, LH2_A_GRP, 2, CISS_G_P11, _V( 1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_lh2_a, 0, 1, &LH2_A, parent );
	STS()->SetAnimation( anim_lh2_a, lh2_aAnimState.pos );

	/////// LH2 A2 ///////
	static UINT LH2_A2_GRP[1] = {GRP_LH2_DUCT_BELLOW3_G};
	anim_lh2_a2 = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LH2_A2( mesh_idx, LH2_A2_GRP, 1, CISS_G_P11, _V( 1, 0, 0 ), static_cast<float>(180.0*RAD) );
	STS()->AddAnimationComponent( anim_lh2_a2, 0, 1, &LH2_A2, parent );
	STS()->SetAnimation( anim_lh2_a2, lh2_a2AnimState.pos );

	/////// LH2 B ///////
	static UINT LH2_B_GRP[1] = {GRP_LH2_FILL_DRAIN_SEGMENT1_G};
	anim_lh2_b = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LH2_B( mesh_idx, LH2_B_GRP, 1, CISS_G_P13, _V( -1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_lh2_b, 0, 1, &LH2_B );
	STS()->SetAnimation( anim_lh2_b, lh2_bAnimState.pos );

	/////// LH2 B2 ///////
	static UINT LH2_B2_GRP[1] = {GRP_LH2_DUCT_BELLOW1_G};
	anim_lh2_b2 = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE LH2_B2( mesh_idx, LH2_B2_GRP, 1, CISS_G_P13, _V( -1, 0, 0 ), static_cast<float>(180.0*RAD) );
	STS()->AddAnimationComponent( anim_lh2_b2, 0, 1, &LH2_B2 );
	STS()->SetAnimation( anim_lh2_b2, lh2_b2AnimState.pos );

	/////// GH2 A ///////
	static UINT GH2_A_GRP[2] = {GRP_GH2_VENT_SEGMENT2_G, GRP_GH2_DUCT_BELLOW2_G};
	anim_gh2_a = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE GH2_A( mesh_idx, GH2_A_GRP, 2, CISS_G_P14, _V( 1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_gh2_a, 0, 1, &GH2_A, parent );
	STS()->SetAnimation( anim_gh2_a, gh2_aAnimState.pos );

	/////// GH2 A2 ///////
	static UINT GH2_A2_GRP[1] = {GRP_GH2_DUCT_BELLOW3_G};
	anim_gh2_a2 = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE GH2_A2( mesh_idx, GH2_A2_GRP, 1, CISS_G_P14, _V( 1, 0, 0 ), static_cast<float>(180.0*RAD) );
	STS()->AddAnimationComponent( anim_gh2_a2, 0, 1, &GH2_A2, parent );
	STS()->SetAnimation( anim_gh2_a2, gh2_a2AnimState.pos );

	/////// GH2 B ///////
	static UINT GH2_B_GRP[1] = {GRP_GH2_VENT_SEGMENT1_G};
	anim_gh2_b = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE GH2_B( mesh_idx, GH2_B_GRP, 1, CISS_G_P16, _V( -1, 0, 0 ), static_cast<float>(360.0*RAD) );
	STS()->AddAnimationComponent( anim_gh2_b, 0, 1, &GH2_B );
	STS()->SetAnimation( anim_gh2_b, gh2_bAnimState.pos );

	/////// GH2 B2 ///////
	static UINT GH2_B2_GRP[1] = {GRP_GH2_DUCT_BELLOW1_G};
	anim_gh2_b2 = STS()->CreateAnimation( 0.0 );
	static MGROUP_ROTATE GH2_B2( mesh_idx, GH2_B2_GRP, 1, CISS_G_P16, _V( -1, 0, 0 ), static_cast<float>(180.0*RAD) );
	STS()->AddAnimationComponent( anim_gh2_b2, 0, 1, &GH2_B2 );
	STS()->SetAnimation( anim_gh2_b2, gh2_b2AnimState.pos );
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
	STS()->SetAnimation( anim_motorcrank, motorcrankAnimState.pos );
	STS()->SetAnimation( anim_link, linkAnimState.pos );
	STS()->SetAnimation( anim_lox_a, lox_aAnimState.pos );
	STS()->SetAnimation( anim_lox_a2, lox_aAnimState.pos );
	STS()->SetAnimation( anim_lox_b, lox_bAnimState.pos );
	STS()->SetAnimation( anim_lox_b2, lox_bAnimState.pos );
	STS()->SetAnimation( anim_gox_a, gox_aAnimState.pos );
	STS()->SetAnimation( anim_gox_a2, gox_aAnimState.pos );
	STS()->SetAnimation( anim_gox_b, gox_bAnimState.pos );
	STS()->SetAnimation( anim_gox_b2, gox_bAnimState.pos );
	STS()->SetAnimation( anim_lh2_a, lh2_aAnimState.pos );
	STS()->SetAnimation( anim_lh2_a2, lh2_a2AnimState.pos );
	STS()->SetAnimation( anim_lh2_b, lh2_bAnimState.pos );
	STS()->SetAnimation( anim_lh2_b2, lh2_bAnimState.pos );
	STS()->SetAnimation( anim_gh2_a, gh2_aAnimState.pos );
	STS()->SetAnimation( anim_gh2_a2, gh2_aAnimState.pos );
	STS()->SetAnimation( anim_gh2_b, gh2_bAnimState.pos );
	STS()->SetAnimation( anim_gh2_b2, gh2_bAnimState.pos );
	STS()->SetAttachmentParams( ahToCentaur, centaurAttachment[0] + STS()->GetOrbiterCoGOffset(), centaurAttachment[1] - centaurAttachment[0], centaurAttachment[2] - centaurAttachment[0] );
	return;
}

void CISS::CalcAnimation( void )
{
	// motor crank/link/DA
	double theta3 = (motorcrankAnimState.pos * CISS_MOTOR_ROTATION_RANGE) + theta3o;
	double theta3b = theta3 - (rotateAnimState.pos * 2 * PI);
	double xa = x1 - r3 * cos( theta3b );
	double ya = y1 - r3 * sin( theta3b );
	double theta4 = atan2( ya, xa ) + acos( (pow( xa, 2 ) + pow( ya, 2 ) + pow( r4, 2 ) - pow( r2, 2 )) / (2 * (-r4) * sqrt( pow( xa, 2 ) + pow( ya, 2 ) )) );
	double theta2 = atan2( (ya + (r4 * sin( theta4 ))) / r2, (xa + (r4 * cos( theta4 ))) / r2 );
	rotateAnimState.pos = range( 0, (theta2o - theta2) / (2 * PI), 1 );
	motorcrankAnimState.pos = (theta3 - theta3o) / CISS_MOTOR_ROTATION_RANGE;
	linkAnimState.pos = (theta4o - theta4) / (2 * PI);
	if (linkAnimState.pos < 0) linkAnimState.pos += 1;

	// LOX
	double theta2_5 = theta2_5o - (rotateAnimState.pos * 2 * PI);
	xa = x7 - r2_5 * cos( theta2_5 );
	ya = y7 - r2_5 * sin( theta2_5 );
	double theta6 = atan2( ya, xa ) + acos( (pow( xa, 2 ) + pow( ya, 2 ) + pow( r6, 2 ) - pow( r5, 2 )) / (2 * (-r6) * sqrt( pow( xa, 2 ) + pow( ya, 2 ) )) );
	double theta5 = atan2( (ya + (r6 * sin( theta6 ))) / r5, (xa + (r6 * cos( theta6 ))) / r5 );
	lox_aAnimState.pos = (theta5 - theta5o + (theta2_5o - theta2_5)) / (2 * PI);
	lox_a2AnimState.pos = lox_aAnimState.pos;
	lox_bAnimState.pos = (theta6o - theta6) / (2 * PI);
	lox_b2AnimState.pos = lox_bAnimState.pos;

	// GOX
	double theta2_8 = theta2_8o - (rotateAnimState.pos * 2 * PI);
	xa = x10 - r2_8 * cos( theta2_8 );
	ya = y10 - r2_8 * sin( theta2_8 );
	double theta9 = atan2( ya, xa ) + acos( (pow( xa, 2 ) + pow( ya, 2 ) + pow( r9, 2 ) - pow( r8, 2 )) / (2 * (-r9) * sqrt( pow( xa, 2 ) + pow( ya, 2 ) )) );
	double theta8 = atan2( (ya + (r9 * sin( theta9 ))) / r8, (xa + (r9 * cos( theta9 ))) / r8 );
	gox_aAnimState.pos = (theta8 - theta8o + (theta2_8o - theta2_8)) / (2 * PI);
	gox_a2AnimState.pos = gox_aAnimState.pos;
	gox_bAnimState.pos = (theta9o - theta9) / (2 * PI);
	gox_b2AnimState.pos = gox_bAnimState.pos;

	// LH2
	double theta2_11 = theta2_11o - (rotateAnimState.pos * 2 * PI);
	xa = x13 - r2_11 * cos( theta2_11 );
	ya = y13 - r2_11 * sin( theta2_11 );
	double theta12 = atan2( ya, xa ) + acos( (pow( xa, 2 ) + pow( ya, 2 ) + pow( r12, 2 ) - pow( r11, 2 )) / (2 * (-r12) * sqrt( pow( xa, 2 ) + pow( ya, 2 ) )) );
	double theta11 = atan2( (ya + (r12 * sin( theta12 ))) / r11, (xa + (r12 * cos( theta12 ))) / r11 );
	lh2_aAnimState.pos = (theta11 - theta11o + (theta2_11o - theta2_11)) / (2 * PI);
	lh2_a2AnimState.pos = lh2_aAnimState.pos;
	lh2_bAnimState.pos = (theta12o - theta12) / (2 * PI);
	lh2_b2AnimState.pos = lh2_bAnimState.pos;

	// GH2
	double theta2_14 = theta2_14o - (rotateAnimState.pos * 2 * PI);
	xa = x16 - r2_14 * cos( theta2_14 );
	ya = y16 - r2_14 * sin( theta2_14 );
	double theta15 = atan2( ya, xa ) + acos( (pow( xa, 2 ) + pow( ya, 2 ) + pow( r15, 2 ) - pow( r14, 2 )) / (2 * (-r15) * sqrt( pow( xa, 2 ) + pow( ya, 2 ) )) );
	double theta14 = atan2( (ya + (r15 * sin( theta15 ))) / r14, (xa + (r15 * cos( theta15 ))) / r14 );
	gh2_aAnimState.pos = (theta14 - theta14o + (theta2_14o - theta2_14)) / (2 * PI);
	gh2_a2AnimState.pos = gh2_aAnimState.pos;
	gh2_bAnimState.pos = (theta15o - theta15) / (2 * PI);
	gh2_b2AnimState.pos = gh2_bAnimState.pos;
	return;
}
