#include "SBTC_RM.h"
#include "UltraMath.h"
#include "assert.h"


namespace dps
{
	SBTC_RM::SBTC_RM( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "SBTC_RM" )
	{
		SBTC_L = 0.0;
		SBTC_L_DG = true;
		SBTC_TO_L = true;

		SBTC_R = 0.0;
		SBTC_R_DG = true;
		SBTC_TO_R = true;
		return;
	}

	SBTC_RM::~SBTC_RM( void )
	{
		return;
	}

	void SBTC_RM::Realize( void )
	{
		DiscreteBundle* pBundle = BundleManager()->CreateBundle( "LeftSBTC", 16 );
		for (int i = 0; i < 6; i++) LeftSBTC[i].Connect( pBundle, i );
		pBundle = BundleManager()->CreateBundle( "RightSBTC", 16 );
		for (int i = 0; i < 6; i++) RightSBTC[i].Connect( pBundle, i );
		return;
	}

	void SBTC_RM::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		SBTC_L = midval( LeftSBTC[0].GetVoltage(), LeftSBTC[1].GetVoltage(), LeftSBTC[2].GetVoltage() );
		SBTC_L_DG = true;
		SBTC_TO_L = (LeftSBTC[3].IsSet( 0.9f ) && LeftSBTC[4].IsSet( 0.9f )) || (LeftSBTC[3].IsSet( 0.9f ) && LeftSBTC[5].IsSet( 0.9f )) || (LeftSBTC[4].IsSet( 0.9f ) && LeftSBTC[5].IsSet( 0.9f ));

		SBTC_R = midval( RightSBTC[0].GetVoltage(), RightSBTC[1].GetVoltage(), RightSBTC[2].GetVoltage() );
		SBTC_R_DG = true;
		SBTC_TO_R = (RightSBTC[3].IsSet( 0.9f ) && RightSBTC[4].IsSet( 0.9f )) || (RightSBTC[3].IsSet( 0.9f ) && RightSBTC[5].IsSet( 0.9f )) || (RightSBTC[4].IsSet( 0.9f ) && RightSBTC[5].IsSet( 0.9f ));
		return;
	}

	bool SBTC_RM::OnParseLine( const char* keyword, const char* value )
	{
		return false;
	}

	void SBTC_RM::OnSaveState( FILEHANDLE scn ) const
	{
		return;
	}

	bool SBTC_RM::OnMajorModeChange( unsigned int newMajorMode )
	{
		switch (newMajorMode)
		{
			case 102:
			case 103:
			case 304:
			case 305:
			case 601:
			case 602:
			case 603:
			case 801:
				return true;
			default:
				return false;
		}
	}

	void SBTC_RM::GetSBTCData_L( double &sbtc, bool &DG, bool &to ) const
	{
		sbtc = SBTC_L;
		DG = SBTC_L_DG;
		to = SBTC_TO_L;
		return;
	}

	void SBTC_RM::GetSBTCData_R( double &sbtc, bool &DG, bool &to ) const
	{
		sbtc = SBTC_R;
		DG = SBTC_R_DG;
		to = SBTC_TO_R;
		return;
	}
}