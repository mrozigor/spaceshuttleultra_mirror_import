#include "ElectronicsAssembly2.h"
#include "DeployedAssembly.h"


namespace comm 
{
	ElectronicsAssembly2::ElectronicsAssembly2( AtlantisSubsystemDirector* _director, DeployedAssembly* pDeployedAssembly ):AtlantisSubsystem( _director, "ElectronicsAssembly2" )
	{
		this->pDeployedAssembly = pDeployedAssembly;
	}

	ElectronicsAssembly2::~ElectronicsAssembly2()
	{
	}

	void ElectronicsAssembly2::Realize( void )
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "A1U_SWITCHES_B", 16 );
		pPower_Off.Connect( pBundle, 5 );
		pPower_On.Connect( pBundle, 6 );
		return;
	}

	void ElectronicsAssembly2::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		if (pPower_Off.IsSet()) return;

		double rng;
		double az;
		double el;

		if (FindTarget( rng, az, el ))
		{
			//sprintf_s( oapiDebugString(), 255, "(EA2) rng:%f az:%f el:%f", rng, az, el );
		}
		return;
	}

	bool ElectronicsAssembly2::FindTarget( double &rng, double &az, double &el )
	{
		VECTOR3 gpos;

		for (int i = oapiGetVesselCount() - 1; i >= 0; i--)
		{
			OBJHANDLE hVessel = oapiGetVesselByIndex( i );

			if (hVessel != STS()->GetHandle())
			{
				//STS()->GetRelativePos( hVessel, gpos );
				oapiGetGlobalPos( hVessel, &gpos );

				VECTOR3 lpos;
				STS()->Global2Local( gpos, lpos );

				double r = length( lpos );
				if ((r > 10) && (r < 10000))
				{
					rng = r;
					az = atan2( -lpos.x, lpos.y ) * DEG;
					el = atan2( lpos.z, fabs( lpos.y ) ) * DEG;
					return true;
				}
			}
		}
		return false;
	}
};