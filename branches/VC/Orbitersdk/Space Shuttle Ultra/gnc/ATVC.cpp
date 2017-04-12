#include "ATVC.h"
#include "..\Atlantis.h"
#include <UltraMath.h>


namespace gnc
{
	ATVC::ATVC( AtlantisSubsystemDirector* _director, const string& _ident, short nID ):AtlantisSubsystem( _director, _ident )
	{
		ID = nID;

		Ppos[0] = 0;
		Ppos[1] = 0;
		Ppos[2] = 0;
		Ypos[0] = 0;
		Ypos[1] = 0;
		Ypos[2] = 0;

		Rpos[0] = 0;
		Rpos[1] = 0;
		Tpos[0] = 0;
		Tpos[1] = 0;
		return;
	}

	ATVC::~ATVC( void )
	{
		return;
	}


	void ATVC::Realize( void )
	{
		DiscreteBundle* bundle = BundleManager()->CreateBundle( "ATVC", 10 );
		dipPpos[0].Connect( bundle, 0 );
		dipPpos[1].Connect( bundle, 1 );
		dipPpos[2].Connect( bundle, 2 );
		dipYpos[0].Connect( bundle, 3 );
		dipYpos[1].Connect( bundle, 4 );
		dipYpos[2].Connect( bundle, 5 );
		dipRpos[0].Connect( bundle, 6 );
		dipRpos[1].Connect( bundle, 7 );
		dipTpos[0].Connect( bundle, 8 );
		dipTpos[1].Connect( bundle, 9 );

		// HACK to have the nozzles in the correct position (only works if this Realize runs after ATVC_SOP's Realize)
		for (int i = 0; i < 3; i++)
		{
			Ppos[i] = dipPpos[i].GetVoltage();
			Ypos[i] = dipYpos[i].GetVoltage();

			STS()->SetSSMEGimbalAngles( i + 1, Ppos[i], Ypos[i] );
		}
		return;
	}

	void ATVC::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		double temp_Ppos;
		double temp_Ypos;

		for (int i = 0; i < 3; i++)
		{
			temp_Ppos = dipPpos[i].GetVoltage() - Ppos[i];
			if (fabs( temp_Ppos ) < fDeltaT * 15)// HACK 10-20º/s limit
			{
				Ppos[i] = dipPpos[i].GetVoltage();
			}
			else
			{
				Ppos[i] += sign( temp_Ppos ) * fDeltaT * 10;
			}

			temp_Ypos = dipYpos[i].GetVoltage() - Ypos[i];
			if (fabs( temp_Ypos ) < fDeltaT * 15)// HACK 10-20º/s limit
			{
				Ypos[i] = dipYpos[i].GetVoltage();
			}
			else
			{
				Ypos[i] += sign( temp_Ypos ) * fDeltaT * 10;
			}

			STS()->SetSSMEGimbalAngles( i + 1, Ppos[i], Ypos[i] );
		}

		//STS()->SetSRBGimbalAngles( static_cast<SIDE> (0), pitchGimbal+rollGimbal, yawGimbal );
		//STS()->SetSRBGimbalAngles( static_cast<SIDE> (1), pitchGimbal+rollGimbal, yawGimbal );
		return;
	}
}