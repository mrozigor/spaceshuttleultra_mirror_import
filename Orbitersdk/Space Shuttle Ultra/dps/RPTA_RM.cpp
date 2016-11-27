#include "RPTA_RM.h"
#include "..\Atlantis.h"
#include "..\AtlantisSubsystemDirector.h"
#include "assert.h"


namespace dps
{
	RPTA_RM::RPTA_RM( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "RPTA_RM" )
	{
		RPTA_L = 0.0;
		RPTA_L_DG = true;

		RPTA_R = 0.0;
		RPTA_R_DG = true;
		return;
	}

	RPTA_RM::~RPTA_RM( void )
	{
		return;
	}

	void RPTA_RM::Realize( void )
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "RPTA", 16 );
		for (int i = 0; i < 3; i++)
		{
			LeftRPTA[i].Connect( pBundle, i );
			RightRPTA[i].Connect( pBundle, i + 3 );
		}
		return;
	}

	void RPTA_RM::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		RPTA_L = midval( LeftRPTA[0].GetVoltage(), LeftRPTA[1].GetVoltage(), LeftRPTA[2].GetVoltage() );
		RPTA_L_DG = true;

		RPTA_R = midval( RightRPTA[0].GetVoltage(), RightRPTA[1].GetVoltage(), RightRPTA[2].GetVoltage() );
		RPTA_R_DG = true;
		return;
	}

	bool RPTA_RM::OnParseLine( const char* keyword, const char* value )
	{
		return false;
	}

	void RPTA_RM::OnSaveState( FILEHANDLE scn ) const
	{
		return;
	}

	bool RPTA_RM::OnMajorModeChange( unsigned int newMajorMode )
	{
		switch (newMajorMode)
		{
			case 304:
			case 305:
			case 602:
			case 603:
				return true;
			default:
				return false;
		}
	}

	void RPTA_RM::GetRPTAData_L( double &rpta, bool &DG ) const
	{
		rpta = RPTA_L;
		DG = RPTA_L_DG;
		return;
	}
	
	void RPTA_RM::GetRPTAData_R( double &rpta, bool &DG ) const
	{
		rpta = RPTA_R;
		DG = RPTA_R_DG;
		return;
	}
}