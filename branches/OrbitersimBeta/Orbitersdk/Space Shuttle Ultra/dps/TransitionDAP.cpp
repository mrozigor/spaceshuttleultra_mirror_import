#include "TransitionDAP.h"
#include "..\Atlantis.h"
#include "SSME_Operations.h"
#include <UltraMath.h>
#include "../ParameterValues.h"


namespace dps
{
	TransitionDAP::TransitionDAP( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "TransitionDAP" )
	{
		MinusZTranslation = false;

		ETSepMinusZDV = 0;
		ETSepMinusZDT = -1;
		return;
	}

	TransitionDAP::~TransitionDAP( void )
	{
		return;
	}

	void TransitionDAP::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		if (pSSME_Operations->GetMECOConfirmedFlag() == true)
		{
			PBI[0].SetLine();
			PBI[1].SetLine();
			PBI[2].SetLine();
			PBI[3].SetLine();
		}
		// HACK conflicting info about which flag does what
		if (pSSME_Operations->GetZeroThrustFlag() == true)
		{
			// hold attitude
			NullRates( DeltaT );

			if (MinusZTranslation == true)
			{
				// fire cmd
				ZTransCommand.SetLine( -1.0f );

				if (GetMajorMode() == 601)
				{
					// RTLS
					if (ETSepMinusZDT == -1) ETSepMinusZDT = SimT + MINUSZ_TARGET_DT_RTLS;

					if (ETSepMinusZDT <= SimT)
					{
						ZTransCommand.ResetLine();
						MinusZTranslation = false;
						SetMajorMode( 602 );

						char buffer[64];
						sprintf_s( buffer, 64, "-Z Translation terminated" );
						oapiWriteLog( buffer );
					}
				}
				else if (0)// TODO add TAL abort flag
				{
					// TAL
					VECTOR3 ThrustVector;
					STS()->GetThrustVector( ThrustVector );
					ETSepMinusZDV += (ThrustVector.y / STS()->GetMass()) * DeltaT;

					if (ETSepMinusZDV >= MINUSZ_TARGET_DV_TAL)
					{
						ZTransCommand.ResetLine();
						MinusZTranslation = false;
						SetMajorMode( 104 );

						char buffer[64];
						sprintf_s( buffer, 64, "-Z Translation terminated" );
						oapiWriteLog( buffer );
					}
				}
				else
				{
					// NOM
					VECTOR3 ThrustVector;
					STS()->GetThrustVector( ThrustVector );
					ETSepMinusZDV += (ThrustVector.y / STS()->GetMass()) * DeltaT;

					if (ETSepMinusZDV >= MINUSZ_TARGET_DV_NOM)
					{
						ZTransCommand.ResetLine();
						MinusZTranslation = false;
						SetMajorMode( 104 );

						char buffer[64];
						sprintf_s( buffer, 64, "-Z Translation terminated" );
						oapiWriteLog( buffer );
					}
				}
			}
			else
			{
				// TODO man control?
			}
		}
		return;
	}

	void TransitionDAP::Realize( void )
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "THRUSTER_CMD", 16 );
		RotThrusterCommands[0].Connect( pBundle, 0 );
		RotThrusterCommands[1].Connect( pBundle, 1 );
		RotThrusterCommands[2].Connect( pBundle, 2 );
		ZTransCommand.Connect( pBundle, 5 );

		pBundle = BundleManager()->CreateBundle( "DAP_PBIS1", 16 );
		PBI[0].Connect( pBundle, 2 );
		PBI[1].Connect( pBundle, 15 );

		pBundle = BundleManager()->CreateBundle( "DAP_PBIS2", 16 );
		PBI[2].Connect( pBundle, 0 );
		PBI[3].Connect( pBundle, 1 );

		pSSME_Operations = static_cast<SSME_Operations*> (FindSoftware( "SSME_Operations" ));
		return;
	}

	bool TransitionDAP::OnMajorModeChange( unsigned int newMajorMode )
	{
		switch (newMajorMode)
		{
			//case 102:
			case 103:
			//case 104:// TAL only?
				return true;
			default:
				return false;
		}
	}

	void TransitionDAP::MinusZTranslationCommand()
	{
		MinusZTranslation = true;

		char buffer[64];
		sprintf_s( buffer, 64, "-Z Translation comanded" );
		oapiWriteLog( buffer );
		return;
	}

	void TransitionDAP::NullRates( double DeltaT )
	{
		const VECTOR3 PRI_LIMITS = _V(0.005, 0.005, 0.005);

		VECTOR3 Error;
		STS()->GetAngularVel( Error );
		Error = _V( Error.x, -Error.y, Error.z ); // convert from Orbitersim to body axis frame
		Error = -Error * DEG;

		VECTOR3 Limits;
		double timeAcc = max(1.0, oapiGetTimeAcceleration());
		double MaxThrusterLevel = 1.0/timeAcc;

		VECTOR3 Torque;// HACK probably should use other values because of the ET
		Torque.data[PITCH]=ORBITER_PITCH_TORQUE;
		Torque.data[YAW]=ORBITER_YAW_TORQUE;
		Torque.data[ROLL]=ORBITER_ROLL_TORQUE;
		VECTOR3 PMI;
		STS()->GetPMI(PMI);

		for(unsigned int i=0;i<3;i++)
		{
			Limits.data[i] = max(PRI_LIMITS.data[i], 0.5*RotationRateChange(STS()->GetMass(), PMI.data[i], Torque.data[i]/timeAcc, DeltaT));
		}

		for(unsigned int i=0;i<3;i++)
		{
			if(abs(Error.data[i])>Limits.data[i])
			{
				double thrusterLevel = MaxThrusterLevel;
				double scale = abs(Error.data[i])/Limits.data[i];
				if(scale < 2) thrusterLevel = MaxThrusterLevel/3.0;
				else if(scale < 5) thrusterLevel = MaxThrusterLevel*(0.667);
				RotThrusterCommands[i].SetLine(static_cast<float>(thrusterLevel*sign(Error.data[i])));
			}
			else
			{
				RotThrusterCommands[i].ResetLine();
			}
		}
	}
}