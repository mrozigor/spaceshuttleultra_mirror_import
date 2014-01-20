#include "MPS_Dedicated_Display_Driver.h"
#include "..\Atlantis.h"
#include "SSME_SOP.h"


namespace dps
{
	MPS_Dedicated_Display_Driver::MPS_Dedicated_Display_Driver( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "MPS_Dedicated_Display_Driver" )
	{
		return;
	}

	MPS_Dedicated_Display_Driver::~MPS_Dedicated_Display_Driver( void )
	{
		return;
	}

	void MPS_Dedicated_Display_Driver::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		if (STS()->HasTank() == false)
		{
			// all lights off
			dspAmberStatusLight[0].ResetLine();
			dspAmberStatusLight[1].ResetLine();
			dspAmberStatusLight[2].ResetLine();

			dpsRedStatusLight[0].ResetLine();
			dpsRedStatusLight[1].ResetLine();
			dpsRedStatusLight[2].ResetLine();
		}
		else
		{
			for (int i = 1; i <= 3; i++)// red lights
			{
				if ((pSSME_SOP->GetLimitExceededFlag( i ) == true) || 
					(pSSME_SOP->GetShutdownPhaseFlag( i ) == true) || 
					(pSSME_SOP->GetPostShutdownPhaseFlag( i ) == true))
				{
					// red light on
					dpsRedStatusLight[i - 1].SetLine();
				}
				else
				{
					// red light off
					dpsRedStatusLight[i - 1].ResetLine();
				}
			}

			for (int i = 1; i <= 3; i++)// amber lights
			{
				if ((pSSME_SOP->GetElectricalLockupModeFlag( i ) == true) ||
					(pSSME_SOP->GetHydraulicLockupModeFlag( i ) == true) || 
					(pSSME_SOP->GetFlightDataPathFailureFlag( i ) == true) || 
					(pSSME_SOP->GetCommandPathFailureFlag( i ) == true))
				{
					// amber light on
					dspAmberStatusLight[i - 1].SetLine();
				}
				else
				{
					// amber light off
					dspAmberStatusLight[i - 1].ResetLine();
				}
			}
		}
		return;
	}

	void MPS_Dedicated_Display_Driver::Realize( void )
	{
		pSSME_SOP = static_cast<SSME_SOP*> (FindSoftware( "SSME_SOP" ));

		DiscreteBundle *pBundle = STS()->BundleManager()->CreateBundle( "MPS_STATUS_LIGHTS", 6 );

		for (int i = 0; i < 3; i++)
		{
			dspAmberStatusLight[i].Connect( pBundle, i + 3 );
			dpsRedStatusLight[i].Connect( pBundle, i );
		}
		return;
	}

	bool MPS_Dedicated_Display_Driver::OnMajorModeChange( unsigned int newMajorMode )
	{
		switch (newMajorMode)
		{
			case 101:
			case 102:
			case 103:
			case 601:
				return true;
			default:
				return false;
		}
	}
}