#include "SSME_Operations.h"
#include "..\Atlantis.h"
#include "SSME_SOP.h"
#include "IO_Control.h"


namespace dps
{
	SSME_Operations::SSME_Operations( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "SSME_Operations" )
	{
		for (int i = 0; i < 3; i++)
		{
			FailFlag[i] = false;
			ManualShutdownFlag[i] = false;
			ShutdownCommandIssued[i] = false;
			ShutdownFlag_A[i] = false;
			PVLV_CL_CMD[i] = false;
			PVLV_CL_CMD_removed[i] = false;

			counter_CEG[i] = 3;
			counter_DFH[i] = 0;

			timerADG[i] = -1;
			timerCFI[i] = -1;
			timerLH2PVLV[i] = -1;
		}

		counter_A = 0;
		counter_B = 0;

		MECOCommand = false;

		MECOConfirmed = false;

		LimitSwitch = 1;

		otherSSMEs[0][0] = 1;
		otherSSMEs[0][1] = 2;

		otherSSMEs[1][0] = 0;
		otherSSMEs[1][1] = 2;

		otherSSMEs[2][0] = 0;
		otherSSMEs[2][1] = 1;
		return;
	}

	SSME_Operations::~SSME_Operations( void )
	{
		return;
	}

	void SSME_Operations::OnPreStep( double SimT, double DeltaT, double MJD )
	{
		for (int i = 0; i < 3; i++)
		{
			if (PVLV_CL_CMD[i] == true)
			{
				if (PVLV_CL_CMD_removed[i] == false)
				{
					if (timerCFI[i] <= SimT)
					{
						pIO_Control->SetCommand( ME1_LOX_PVLV_CL_A + i, false );
						pIO_Control->SetCommand( ME1_LOX_PVLV_CL_B + i, false );
						pIO_Control->SetCommand( ME1_LH2_PVLV_CL + i, false );

						PVLV_CL_CMD_removed[i] = true;
					}
				}
			}
			else
			{
				if (((pSSME_SOP->GetFlightDataPathFailureFlag( i + 1 ) == true) && (ShutdownCommandIssued[i] == true)) || // TODO missing safing cmd
					((pSSME_SOP->GetFlightDataPathFailureFlag( i + 1 ) == false) && ((pSSME_SOP->GetShutdownPhaseFlag( i + 1 ) == true) || (pSSME_SOP->GetPostShutdownPhaseFlag( i + 1 ) == true))))
				{
					FailFlag[i] = true;
				
					if (timerADG[i] != -1)
					{
						if (timerADG[i] <= SimT)
						{
							pIO_Control->SetCommand( ME1_LOX_PVLV_OP_A + i, false );
							pIO_Control->SetCommand( ME1_LOX_PVLV_OP_B + i, false );
							pIO_Control->SetCommand( ME1_LOX_PVLV_CL_A + i, true );
							pIO_Control->SetCommand( ME1_LOX_PVLV_CL_B + i, true );

							if (timerLH2PVLV[i] == -1)
							{
								timerLH2PVLV[i] = SimT + TIME_DELAY_LH2_PVLV_CL;
							}

							if (timerLH2PVLV[i] <= SimT)
							{
								pIO_Control->SetCommand( ME1_LH2_PVLV_OP + i, false );
								pIO_Control->SetCommand( ME1_LH2_PVLV_CL + i, true );

								PVLV_CL_CMD[i] = true;

								timerCFI[i] = SimT + TIME_DELAY_CFI;
							}
						}
					}
					else
					{
						timerADG[i] = SimT + TIME_DELAY_ADG;
					}
				}
			}

			if (dspSSMESDPB[i].IsSet() == true)
			{
				if (ManualShutdownFlag[i] == false)// only log first PB use
				{
					char buffer[64];
					sprintf_s( buffer, 64, "ME%d PB Pressed @ MET %.2f", i + 1, STS()->GetMET() );
					oapiWriteLog( buffer );
				}
				ManualShutdownFlag[i] = true;
			}
		}

		if (MECOCommand == true)
		{
			// ...
			// go to G
		}
		else
		{
			if ((dspLimitSwitchInhibit.IsSet() == false) && (dspLimitSwitchEnable.IsSet() == false))
			{
				// AUTO
				counter_A = 0;
				counter_B = 0;
				for (int i = 0; i < 3; i++)
				{
					// TODO still not working for 1EO and 2EO
					if (ManualShutdownFlag[i] == false)
					//if ((ManualShutdownFlag[otherSSMEs[i][0]] == false) && (ManualShutdownFlag[otherSSMEs[i][1]] == false))
					{
						if ((pSSME_SOP->GetShutdownPhaseFlag( i + 1 ) == true) || (pSSME_SOP->GetPostShutdownPhaseFlag( i + 1 ) == true) || (pSSME_SOP->GetFlightDataPathFailureFlag( i + 1 ) == true))
						{
							if (counter_DFH[i] < 3)
							{
								// INH all others
								pSSME_SOP->SetLimitInhibitCommandFlag( otherSSMEs[i][0] + 1 );
								pSSME_SOP->SetLimitInhibitCommandFlag( otherSSMEs[i][1] + 1 );
							
								counter_DFH[i]++;
							}
						}
						else
						{
							if (counter_CEG[i] < 3)
							{
								// ENA me
								pSSME_SOP->SetLimitEnableCommandFlag( i + 1 );

								counter_CEG[i]++;
							}
						}
					}
				}
			}
			else
			{
				counter_CEG[0] = 0;
				counter_CEG[1] = 0;
				counter_CEG[2] = 0;
				counter_DFH[0] = 0;
				counter_DFH[1] = 0;
				counter_DFH[2] = 0;
				if (dspLimitSwitchEnable.IsSet() == true)
				{
					// ENA
					counter_B = 0;

					if (counter_A < 3)
					{
						// ENA all
						pSSME_SOP->SetLimitEnableCommandFlag( 1 );
						pSSME_SOP->SetLimitEnableCommandFlag( 2 );
						pSSME_SOP->SetLimitEnableCommandFlag( 3 );

						counter_A++;
					}
				}
				else
				{
					// INH
					counter_A = 0;

					if (counter_B < 3)
					{
						if (ManualShutdownFlag[0] == false) pSSME_SOP->SetLimitInhibitCommandFlag( 1 );
						if (ManualShutdownFlag[1] == false) pSSME_SOP->SetLimitInhibitCommandFlag( 2 );
						if (ManualShutdownFlag[2] == false) pSSME_SOP->SetLimitInhibitCommandFlag( 3 );

						counter_B++;
					}
				}
			}

			// F
		}

		// G

		// H
		for (int i = 0; i < 3; i++)
		{
			if ((ManualShutdownFlag[i] == true) || (MECOCommand == true))
			{
				if ((pSSME_SOP->GetShutdownPhaseFlag( i + 1 ) == true) || (pSSME_SOP->GetPostShutdownPhaseFlag( i + 1 ) == true))
				{
					ShutdownCommandIssued[i] = true;
				}
				else
				{
					if (ShutdownFlag_A[i] == true)
					{
						// HACK added if and commented below to compensate delays in getting engine data
						if (ShutdownCommandIssued[i] == false)
						{
							pSSME_SOP->SetShutdownCommandFlag( i + 1 );
							//ShutdownFlag_A[i] = false;
							ShutdownCommandIssued[i] = true;
						}
					}
					else
					{
						pSSME_SOP->SetShutdownEnableCommandFlag( i + 1 );
						ShutdownFlag_A[i] = true;
					}
				}
			}
		}

		if ((ManualShutdownFlag[0] == true) && (ManualShutdownFlag[1] == true) && (ManualShutdownFlag[2] == true)) MECOCommand = true;

		// K
		if ((MECOConfirmed == false) && (
			((pSSME_SOP->GetPercentChamberPressVal( 1 ) < 30) && (pSSME_SOP->GetPercentChamberPressVal( 2 ) < 30) && (pSSME_SOP->GetPercentChamberPressVal( 3 ) < 30)) ||
			((pSSME_SOP->GetFlightDataPathFailureFlag( 1 ) == true) && (pSSME_SOP->GetPercentChamberPressVal( 2 ) < 30) && (pSSME_SOP->GetPercentChamberPressVal( 3 ) < 30)) ||
			((pSSME_SOP->GetPercentChamberPressVal( 1 ) < 30) && (pSSME_SOP->GetFlightDataPathFailureFlag( 2 ) == true) && (pSSME_SOP->GetPercentChamberPressVal( 3 ) < 30)) ||
			((pSSME_SOP->GetPercentChamberPressVal( 1 ) < 30) && (pSSME_SOP->GetPercentChamberPressVal( 2 ) < 30) && (pSSME_SOP->GetFlightDataPathFailureFlag( 3 ) == true))))
		{
			MECOCommand = true;
			MECOConfirmed = true;
			//oapiWriteLog( "MECO Confirmed" );
			char buffer[64];
			sprintf_s( buffer, 64, "MECO Confirmed @ MET%.2f @ %.2f%% Prop", STS()->GetMET(), STS()->GetETPropellant_B() );
			oapiWriteLog( buffer );
		}

		// HACK ECO sensors
		if ((STS()->GetETPropellant_B() <= 0.2) && (MECOCommand == false))// 0.2% is enough to shutdown the engines
		{
			MECOCommand = true;
			//oapiWriteLog( "MECO Low Level Cutoff" );
			char buffer[64];
			sprintf_s( buffer, 64, "MECO Low Level Cutoff @ MET %.2f @ %.2f%% Prop", STS()->GetMET(), STS()->GetETPropellant_B() );
			oapiWriteLog( buffer );
		}
		return;
	}

	void SSME_Operations::Realize( void )
	{
		DiscreteBundle *pBundle = BundleManager()->CreateBundle( "C3_LIMITS_SSMEPB", 5 );

		dspLimitSwitchInhibit.Connect( pBundle, 0 );
		dspLimitSwitchEnable.Connect( pBundle, 1 );
		dspSSMESDPB[1].Connect( pBundle, 2 );// L
		dspSSMESDPB[0].Connect( pBundle, 3 );// C
		dspSSMESDPB[2].Connect( pBundle, 4 );// R

		pSSME_SOP = static_cast<SSME_SOP*> (FindSoftware( "SSME_SOP" ));
		pIO_Control = static_cast<IO_Control*> (FindSoftware( "IO_Control" ));
		return;
	}

	bool SSME_Operations::OnMajorModeChange( unsigned int newMajorMode )
	{
		switch (newMajorMode)
		{
			case 102:
			case 103:
			case 601:
				return true;
			default:
				return false;
		}
	}

	void SSME_Operations::SetMECOCommandFlag( void )
	{
		MECOCommand = true;
		return;
	}

	bool SSME_Operations::GetMECOCommandFlag( void ) const
	{
		return MECOCommand;
	}

	bool SSME_Operations::GetMECOConfirmedFlag( void ) const
	{
		return MECOConfirmed;
	}
}