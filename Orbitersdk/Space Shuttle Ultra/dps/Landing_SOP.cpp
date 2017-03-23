#include "Landing_SOP.h"
#include "AerojetDAP.h"
#include "..\Atlantis.h"
#include "assert.h"


namespace dps
{
	Landing_SOP::Landing_SOP( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "Landing_SOP" )
	{
		WOWLON = false;
		FLATTURN = false;
		WONG = false;
		GSENBL = false;
		return;
	}

	Landing_SOP::~Landing_SOP( void )
	{
		return;
	}

	void Landing_SOP::Realize( void )
	{
		DiscreteBundle* pBundle = BundleManager()->CreateBundle( "C3_SEP", 4 );
		SRBSEPSW.Connect( pBundle, 0 );
		SRBSEPPB.Connect( pBundle, 1 );
		ETSEPSW.Connect( pBundle, 2 );
		ETSEPPB.Connect( pBundle, 3 );

		pAerojetDAP = static_cast<AerojetDAP*>(FindSoftware( "AerojetDAP" ));
		assert( (pAerojetDAP != NULL) && "Landing_SOP::Realize.pAerojetDAP" );
		return;
	}

	void Landing_SOP::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		if (pAerojetDAP->GetApproachAndLandState() == false) return;// only run from A/L

		if (WOWLON == false)
		{
			if (((STS()->GroundContact()) && (STS()->GetGearState() == AnimState::OPEN)) ||// assume it's the rear wheels
				(SRBSEPSW.IsSet() && SRBSEPPB.IsSet()) || (ETSEPSW.IsSet() && ETSEPPB.IsSet()))
			{
				WOWLON = true;
				FLATTURN = true;
			}
		}
		else if (WONG == false)
		{
			if (((STS()->GroundContact()) && (STS()->GetGearState() == AnimState::OPEN) &&
				(STS()->GetPitch() < (-4.0 * RAD))) ||// assume pitch under -4º and ground contact is nose wheels down
				(SRBSEPSW.IsSet() && SRBSEPPB.IsSet()) || (ETSEPSW.IsSet() && ETSEPPB.IsSet()))
				WONG = true;
		}
		else if (GSENBL == false)
		{
			if (STS()->GetPitch() < 0.0) GSENBL = true;
		}
		return;
	}

	bool Landing_SOP::OnParseLine( const char* keyword, const char* value )
	{
		return false;
	}

	void Landing_SOP::OnSaveState( FILEHANDLE scn ) const
	{
		return;
	}

	bool Landing_SOP::OnMajorModeChange( unsigned int newMajorMode )
	{
		switch (newMajorMode)
		{
			case 305:
			case 603:
				return true;
			default:
				return false;
		}
	}

	bool Landing_SOP::GetWOWLON( void ) const
	{
		return WOWLON;
	}

	bool Landing_SOP::GetFLATTURN( void ) const
	{
		return FLATTURN;
	}

	bool Landing_SOP::GetWONG( void ) const
	{
		return WONG;
	}

	bool Landing_SOP::GetGSENBL( void ) const
	{
		return GSENBL;
	}
}