#include "MPS_Dedicated_Display_Driver.h"
#include "..\Atlantis.h"
#include "SSME_SOP.h"


namespace dps
{
	MPS_Dedicated_Display_Driver::MPS_Dedicated_Display_Driver( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "MPS_Dedicated_Display_Driver" )
	{
		AmberStatusLight[0] = false;
		AmberStatusLight[1] = false;
		AmberStatusLight[2] = false;

		RedStatusLight[0] = false;
		RedStatusLight[1] = false;
		RedStatusLight[2] = false;
		return;
	}

	MPS_Dedicated_Display_Driver::~MPS_Dedicated_Display_Driver( void )
	{
		return;
	}

	void MPS_Dedicated_Display_Driver::OnPreStep( double SimT, double DeltaT, double MJD )
	{
		return;
	}

	void MPS_Dedicated_Display_Driver::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		if (STS()->HasTank() == false)
		{
			// all lights off
			AmberStatusLight[0] = false;
			AmberStatusLight[1] = false;
			AmberStatusLight[2] = false;

			RedStatusLight[0] = false;
			RedStatusLight[1] = false;
			RedStatusLight[2] = false;
		}
		else
		{
			for (int i = 1; i <= 3; i++)// red lights
			{
				if (//(i limits exceeded flag == true) || 
					(pSSME_SOP->GetShutdownPhaseFlag( i ) == true) || 
					(pSSME_SOP->GetPostShutdownPhaseFlag( i ) == true))
				{
					// red light on
					RedStatusLight[i - 1] = true;
				}
				else
				{
					// red light off
					RedStatusLight[i - 1] = false;
				}
			}

			for (int i = 1; i <= 3; i++)// amber lights
			{
				if ((pSSME_SOP->GetElectricalLockupFlag( i ) == true) ||
					(pSSME_SOP->GetHydraulicLockupFlag( i ) == true) || 
					(pSSME_SOP->GetDataPathFailureFlag( i ) == true) || 
					(pSSME_SOP->GetCommandPathFailureFlag( i ) == true))
				{
					// amber light on
					AmberStatusLight[i - 1] = true;
				}
				else
				{
					// amber light off
					AmberStatusLight[i - 1] = false;
				}
			}
		}

		for (int i = 0; i < 3; i++)
		{
			if (AmberStatusLight[i] == true)
			{
				dspAmberStatusLight[i].SetLine();
			}
			else
			{
				dspAmberStatusLight[i].ResetLine();
			}

			if (RedStatusLight[i] == true)
			{
				dpsRedStatusLight[i].SetLine();
			}
			else
			{
				dpsRedStatusLight[i].ResetLine();
			}
		}
		return;
	}

	bool MPS_Dedicated_Display_Driver::OnParseLine( const char* keyword, const char* value )
	{
		return false;
	}

	void MPS_Dedicated_Display_Driver::OnSaveState( FILEHANDLE scn ) const
	{
		return;
	}

	void MPS_Dedicated_Display_Driver::Realize( void )
	{
		pSSME_SOP = static_cast<SSME_SOP*> (FindSoftware( "SSME_SOP" ));

		DiscreteBundle *pBundle = STS()->BundleManager()->CreateBundle( "MPS_STATUS_LIGHTS", 6);

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