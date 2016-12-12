#include "THC_RM.h"
#include "assert.h"


namespace dps
{
	THC_RM::THC_RM( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "THC_RM" )
	{
		THC_L_pX = false;
		THC_L_mX = false;
		THC_L_pY = false;
		THC_L_mY = false;
		THC_L_pZ = false;
		THC_L_mZ = false;

		THC_A_pX = false;
		THC_A_mX = false;
		THC_A_pY = false;
		THC_A_mY = false;
		THC_A_pZ = false;
		THC_A_mZ = false;
		return;
	}

	THC_RM::~THC_RM( void )
	{
		return;
	}

	void THC_RM::Realize( void )
	{
		DiscreteBundle* pBundle = BundleManager()->CreateBundle( "LeftRHCTHC_A", 16 );
		for (int i = 9; i < 16; i++) LeftTHC[i - 9].Connect( pBundle, i );
		pBundle = BundleManager()->CreateBundle( "LeftRHCTHC_B", 16 );
		for (int i = 0; i < 11; i++) LeftTHC[i + 7].Connect( pBundle, i );
		pBundle = BundleManager()->CreateBundle( "AftRHCTHC_A", 16 );
		for (int i = 9; i < 16; i++) AftTHC[i - 9].Connect( pBundle, i );
		pBundle = BundleManager()->CreateBundle( "AftRHCTHC_B", 16 );
		for (int i = 0; i < 11; i++) AftTHC[i + 7].Connect( pBundle, i );
		return;
	}

	void THC_RM::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		THC_L_pX = (LeftTHC[0].IsSet( 0.9f ) && LeftTHC[1].IsSet( 0.9f )) || (LeftTHC[0].IsSet( 0.9f ) && LeftTHC[2].IsSet( 0.9f )) || (LeftTHC[1].IsSet( 0.9f ) && LeftTHC[2].IsSet( 0.9f ));
		THC_L_mX = (LeftTHC[3].IsSet( 0.9f ) && LeftTHC[4].IsSet( 0.9f )) || (LeftTHC[3].IsSet( 0.9f ) && LeftTHC[5].IsSet( 0.9f )) || (LeftTHC[4].IsSet( 0.9f ) && LeftTHC[5].IsSet( 0.9f ));
		THC_L_pY = (LeftTHC[6].IsSet( 0.9f ) && LeftTHC[7].IsSet( 0.9f )) || (LeftTHC[6].IsSet( 0.9f ) && LeftTHC[8].IsSet( 0.9f )) || (LeftTHC[7].IsSet( 0.9f ) && LeftTHC[8].IsSet( 0.9f ));
		THC_L_mY = (LeftTHC[9].IsSet( 0.9f ) && LeftTHC[10].IsSet( 0.9f )) || (LeftTHC[9].IsSet( 0.9f ) && LeftTHC[11].IsSet( 0.9f )) || (LeftTHC[10].IsSet( 0.9f ) && LeftTHC[11].IsSet( 0.9f ));
		THC_L_pZ = (LeftTHC[12].IsSet( 0.9f ) && LeftTHC[13].IsSet( 0.9f )) || (LeftTHC[12].IsSet( 0.9f ) && LeftTHC[14].IsSet( 0.9f )) || (LeftTHC[13].IsSet( 0.9f ) && LeftTHC[14].IsSet( 0.9f ));
		THC_L_mZ = (LeftTHC[15].IsSet( 0.9f ) && LeftTHC[16].IsSet( 0.9f )) || (LeftTHC[15].IsSet( 0.9f ) && LeftTHC[17].IsSet( 0.9f )) || (LeftTHC[16].IsSet( 0.9f ) && LeftTHC[17].IsSet( 0.9f ));
		
		THC_A_pX = (AftTHC[0].IsSet( 0.9f ) && AftTHC[1].IsSet( 0.9f )) || (AftTHC[0].IsSet( 0.9f ) && AftTHC[2].IsSet( 0.9f )) || (AftTHC[1].IsSet( 0.9f ) && AftTHC[2].IsSet( 0.9f ));
		THC_A_mX = (AftTHC[3].IsSet( 0.9f ) && AftTHC[4].IsSet( 0.9f )) || (AftTHC[3].IsSet( 0.9f ) && AftTHC[5].IsSet( 0.9f )) || (AftTHC[4].IsSet( 0.9f ) && AftTHC[5].IsSet( 0.9f ));
		THC_A_pY = (AftTHC[6].IsSet( 0.9f ) && AftTHC[7].IsSet( 0.9f )) || (AftTHC[6].IsSet( 0.9f ) && AftTHC[8].IsSet( 0.9f )) || (AftTHC[7].IsSet( 0.9f ) && AftTHC[8].IsSet( 0.9f ));
		THC_A_mY = (AftTHC[9].IsSet( 0.9f ) && AftTHC[10].IsSet( 0.9f )) || (AftTHC[9].IsSet( 0.9f ) && AftTHC[11].IsSet( 0.9f )) || (AftTHC[10].IsSet( 0.9f ) && AftTHC[11].IsSet( 0.9f ));
		THC_A_pZ = (AftTHC[12].IsSet( 0.9f ) && AftTHC[13].IsSet( 0.9f )) || (AftTHC[12].IsSet( 0.9f ) && AftTHC[14].IsSet( 0.9f )) || (AftTHC[13].IsSet( 0.9f ) && AftTHC[14].IsSet( 0.9f ));
		THC_A_mZ = (AftTHC[15].IsSet( 0.9f ) && AftTHC[16].IsSet( 0.9f )) || (AftTHC[15].IsSet( 0.9f ) && AftTHC[17].IsSet( 0.9f )) || (AftTHC[16].IsSet( 0.9f ) && AftTHC[17].IsSet( 0.9f ));
		return;
	}

	bool THC_RM::OnParseLine( const char* keyword, const char* value )
	{
		return false;
	}

	void THC_RM::OnSaveState( FILEHANDLE scn ) const
	{
		return;
	}

	bool THC_RM::OnMajorModeChange( unsigned int newMajorMode )
	{
		switch (newMajorMode)
		{
			case 103:
			case 104:
			case 105:
			case 106:
			case 201:
			case 202:
			case 301:
			case 302:
			case 303:
			case 601:
			case 801:
				return true;
			default:
				return false;
		}
	}

	void THC_RM::GetTHCData_L( bool &px, bool &mx, bool &py, bool &my, bool &pz, bool &mz ) const
	{
		px = THC_L_pX;
		mx = THC_L_mX;
		py = THC_L_pY;
		my = THC_L_mY;
		pz = THC_L_pZ;
		mz = THC_L_mZ;
		return;
	}

	void THC_RM::GetTHCData_A( bool &px, bool &mx, bool &py, bool &my, bool &pz, bool &mz ) const
	{
		px = THC_A_pX;
		mx = THC_A_mX;
		py = THC_A_pY;
		my = THC_A_mY;
		pz = THC_A_pZ;
		mz = THC_A_mZ;
		return;
	}
}