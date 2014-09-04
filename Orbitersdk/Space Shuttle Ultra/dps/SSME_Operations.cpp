#include "SSME_Operations.h"
#include "..\Atlantis.h"
#include "SSME_SOP.h"
#include "IO_Control.h"
#include "assert.h"


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

		LowLevelSensorArm = false;
		LO2LowLevelSensorDryFlag[0] = false;
		LO2LowLevelSensorDryFlag[1] = false;
		LO2LowLevelSensorDryFlag[2] = false;
		LO2LowLevelSensorDryFlag[3] = false;
		LH2LowLevelSensorDryFlag[0] = false;
		LH2LowLevelSensorDryFlag[1] = false;
		LH2LowLevelSensorDryFlag[2] = false;
		LH2LowLevelSensorDryFlag[3] = false;
		LO2LowLevelSensorDsblFlag[0] = false;
		LO2LowLevelSensorDsblFlag[1] = false;
		LO2LowLevelSensorDsblFlag[2] = false;
		LO2LowLevelSensorDsblFlag[3] = false;
		LH2LowLevelSensorDsblFlag[0] = false;
		LH2LowLevelSensorDsblFlag[1] = false;
		LH2LowLevelSensorDsblFlag[2] = false;
		LH2LowLevelSensorDsblFlag[3] = false;
		LowLevel1stRun = true;
		LowLevelLO2timer = -1;
		LowLevelLH2timer = -1;

		MECOCommand = false;

		MECOConfirmed = false;

		ZeroThrust = false;

		timerMECOConfirmed = 999999;

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
		// INFO check 20030005845_2002110399 for new vlv timing data
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
				// TODO missing safing cmd (is it really needed?)
				if (((pSSME_SOP->GetFlightDataPathFailureFlag( i + 1 ) == true) && (ShutdownCommandIssued[i] == true)) ||
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

			if (dipSSMESDPB[i].IsSet() == true)
			{
				if (ManualShutdownFlag[i] == false)// only log first PB use
				{
					char buffer[64];
					sprintf_s( buffer, 64, "ME-%d PB Pressed @ MET %.2f", i + 1, STS()->GetMET() );
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
			if ((dipLimitSwitchInhibit.IsSet() == false) && (dipLimitSwitchEnable.IsSet() == false))
			{
				// AUTO
				counter_A = 0;
				counter_B = 0;
				for (int i = 0; i < 3; i++)
				{
					// HACK replaced line below so it also works on 2EO
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
				if (dipLimitSwitchEnable.IsSet() == true)
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

		if ((LowLevelSensorArm == true) && (MECOCommand == false))
		{
			if (LowLevel1stRun == false)
			{
				// 2 dry -> MECO
				int LO2count = 0;
				int LH2count = 0;

				for (int i = 0; i < 4; i++)
				{
					// LOX
					if (dipLO2LowLevelSensor[i].IsSet( 0.3 ) == false) LO2LowLevelSensorDryFlag[i] = true;
					if ((LO2LowLevelSensorDryFlag[i] == true) && (LO2LowLevelSensorDsblFlag[i] == false)) LO2count++;

					// LH2
					if (dipLH2LowLevelSensor[i].IsSet( 0.3 ) == false) LH2LowLevelSensorDryFlag[i] = true;
					if ((LH2LowLevelSensorDryFlag[i] == true) && (LH2LowLevelSensorDsblFlag[i] == false)) LH2count++;
				}

				if ((LO2count >= 2) && (LowLevelLO2timer == -1))
				{
					switch ((int)FailFlag[0] + (int)FailFlag[1] + (int)FailFlag[2])
					{
						case 0:
							LowLevelLO2timer = SimT + NOM_LO2_LOW_LVL_TIME_DELAY_L;
							break;
						case 1:
							LowLevelLO2timer = SimT + TWOENG_LO2_LOW_LVL_TIME_DELAY_L;
							break;
						case 2:
							LowLevelLO2timer = SimT + ONEENG_LO2_LOW_LVL_TIME_DELAY_L;
							break;
					}
				}
				if ((LowLevelLO2timer <= SimT) && (LowLevelLO2timer != -1))
				{
					MECOCommand = true;

					char buffer[64];
					sprintf_s( buffer, 64, "MECO LOX Low Level Cutoff @ MET %.2f", STS()->GetMET() );
					oapiWriteLog( buffer );
				}

				if ((LH2count >= 2) && (LowLevelLH2timer == -1))
				{
					switch ((int)FailFlag[0] + (int)FailFlag[1] + (int)FailFlag[2])
					{
						case 0:
							LowLevelLH2timer = SimT + NOM_LH2_LOW_LVL_TIME_DELAY_L;
							break;
						case 1:
							LowLevelLH2timer = SimT + TWOENG_LH2_LOW_LVL_TIME_DELAY_L;
							break;
						case 2:
							LowLevelLH2timer = SimT + ONEENG_LH2_LOW_LVL_TIME_DELAY_L;
							break;
					}
				}
				if ((LowLevelLH2timer <= SimT) && (LowLevelLH2timer != -1))
				{
					MECOCommand = true;

					char buffer[64];
					sprintf_s( buffer, 64, "MECO LH2 Low Level Cutoff @ MET %.2f", STS()->GetMET() );
					oapiWriteLog( buffer );
				}
			}
			else
			{
				// 1st run, disable 1 dry (failed) sensor (if any)
				char buffer[64];
				sprintf_s( buffer, 64, "Low Level Sensors Arm @ MET %.2f", STS()->GetMET() );
				oapiWriteLog( buffer );

				// LOX
				for (int i = 0; i < 4; i++)
				{
					if (dipLO2LowLevelSensor[i].IsSet( 0.3 ) == false)
					{
						LO2LowLevelSensorDsblFlag[i] = true;
						
						sprintf_s( buffer, 64, "LOX Low Level Sensor %d disabled", i + 1 );
						oapiWriteLog( buffer );
						break;
					}
				}

				// LH2
				for (int i = 0; i < 4; i++)
				{
					if (dipLH2LowLevelSensor[i].IsSet( 0.3 ) == false)
					{
						LH2LowLevelSensorDsblFlag[i] = true;

						sprintf_s( buffer, 64, "LH2 Low Level Sensor %d disabled", i + 1 );
						oapiWriteLog( buffer );
						break;
					}
				}

				LowLevel1stRun = false;
			}
		}

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

		// step 30
		if ((MECOConfirmed == false) && (ManualShutdownFlag[0] == true) && (ManualShutdownFlag[1] == true) && (ManualShutdownFlag[2] == true))
		{
			MECOCommand = true;
			MECOConfirmed = true;
			timerMECOConfirmed = SimT + TIME_TO_ZERO_THRUST;

			pIO_Control->SetCommand( PNEU_L_HE_XOVR_OP, true );// open LV10

			char buffer[64];
			sprintf_s( buffer, 64, "MECO Confirmed (PB) @ MET %.2f @ %.2f%% Prop", STS()->GetMET(), STS()->GetETPropellant_B() );
			oapiWriteLog( buffer );
		}

		// K
		if ((MECOConfirmed == false) && (
			((pSSME_SOP->GetPercentChamberPressVal( 1 ) < 30) && (pSSME_SOP->GetPercentChamberPressVal( 2 ) < 30) && (pSSME_SOP->GetPercentChamberPressVal( 3 ) < 30) && (pSSME_SOP->GetFlightDataPathFailureFlag( 1 ) == false) && (pSSME_SOP->GetFlightDataPathFailureFlag( 2 ) == false) && (pSSME_SOP->GetFlightDataPathFailureFlag( 3 ) == false)) ||
			((pSSME_SOP->GetFlightDataPathFailureFlag( 1 ) == true) && (pSSME_SOP->GetPercentChamberPressVal( 2 ) < 30) && (pSSME_SOP->GetPercentChamberPressVal( 3 ) < 30) && (pSSME_SOP->GetFlightDataPathFailureFlag( 2 ) == false) && (pSSME_SOP->GetFlightDataPathFailureFlag( 3 ) == false)) ||
			((pSSME_SOP->GetPercentChamberPressVal( 1 ) < 30) && (pSSME_SOP->GetFlightDataPathFailureFlag( 2 ) == true) && (pSSME_SOP->GetPercentChamberPressVal( 3 ) < 30) && (pSSME_SOP->GetFlightDataPathFailureFlag( 1 ) == false) && (pSSME_SOP->GetFlightDataPathFailureFlag( 3 ) == false)) ||
			((pSSME_SOP->GetPercentChamberPressVal( 1 ) < 30) && (pSSME_SOP->GetPercentChamberPressVal( 2 ) < 30) && (pSSME_SOP->GetFlightDataPathFailureFlag( 3 ) == true) && (pSSME_SOP->GetFlightDataPathFailureFlag( 1 ) == false) && (pSSME_SOP->GetFlightDataPathFailureFlag( 2 ) == false)) ||
			(GetMajorMode() == 104)))
		{
			MECOCommand = true;
			MECOConfirmed = true;
			timerMECOConfirmed = SimT + TIME_TO_ZERO_THRUST;

			pIO_Control->SetCommand( PNEU_L_HE_XOVR_OP, true );// open LV10

			char buffer[64];
			sprintf_s( buffer, 64, "MECO Confirmed @ MET %.2f @ %.2f%% Prop", STS()->GetMET(), STS()->GetETPropellant_B() );
			oapiWriteLog( buffer );
		}

		if ((timerMECOConfirmed <= SimT) && (ZeroThrust == false))
		{
			ZeroThrust = true;

			char buffer[64];
			sprintf_s( buffer, 64, "Zero Thrust @ MET %.2f", STS()->GetMET() );
			oapiWriteLog( buffer );
		}

		// HACK makeshift DSC for FCV operation (no capability yet to reasign ullage sensors)
		if ((dipLO2UllagePressureSensor[0].GetVoltage() * 10) < 34.7) pIO_Control->SetCommand( GOX_FCV_1, true );
		else if ((dipLO2UllagePressureSensor[0].GetVoltage() * 10) > 39.7) pIO_Control->SetCommand( GOX_FCV_1, false );

		if ((dipLO2UllagePressureSensor[1].GetVoltage() * 10) < 34.7) pIO_Control->SetCommand( GOX_FCV_2, true );
		else if ((dipLO2UllagePressureSensor[1].GetVoltage() * 10) > 39.7) pIO_Control->SetCommand( GOX_FCV_2, false );

		if ((dipLO2UllagePressureSensor[2].GetVoltage() * 10) < 34.7) pIO_Control->SetCommand( GOX_FCV_3, true );
		else if ((dipLO2UllagePressureSensor[2].GetVoltage() * 10) > 39.7) pIO_Control->SetCommand( GOX_FCV_3, false );

		if (((dipLH2UllagePressureSensor[0].GetVoltage() * 8) + 12) < 32.6) pIO_Control->SetCommand( GH2_FCV_1, true );
		else if ((dipLH2UllagePressureSensor[0].GetVoltage() * 10) > 33.4) pIO_Control->SetCommand( GH2_FCV_1, false );

		if (((dipLH2UllagePressureSensor[1].GetVoltage() * 8) + 12) < 32.6) pIO_Control->SetCommand( GH2_FCV_2, true );
		else if ((dipLH2UllagePressureSensor[1].GetVoltage() * 10) > 33.4) pIO_Control->SetCommand( GH2_FCV_2, false );

		if (((dipLH2UllagePressureSensor[2].GetVoltage() * 8) + 12) < 32.6) pIO_Control->SetCommand( GH2_FCV_3, true );
		else if ((dipLH2UllagePressureSensor[2].GetVoltage() * 10) > 33.4) pIO_Control->SetCommand( GH2_FCV_3, false );

		return;
	}

	void SSME_Operations::Realize( void )
	{
		DiscreteBundle *pBundle = BundleManager()->CreateBundle( "C3_LIMITS_SSMEPB", 5 );
		dipLimitSwitchInhibit.Connect( pBundle, 0 );
		dipLimitSwitchEnable.Connect( pBundle, 1 );
		dipSSMESDPB[1].Connect( pBundle, 2 );// L
		dipSSMESDPB[0].Connect( pBundle, 3 );// C
		dipSSMESDPB[2].Connect( pBundle, 4 );// R

		pBundle = BundleManager()->CreateBundle( "ET_LOX_SENSORS", 16 );
		dipLO2LowLevelSensor[0].Connect( pBundle, 0 );
		dipLO2LowLevelSensor[1].Connect( pBundle, 1 );
		dipLO2LowLevelSensor[2].Connect( pBundle, 2 );
		dipLO2LowLevelSensor[3].Connect( pBundle, 3 );
		dipLO2UllagePressureSensor[0].Connect( pBundle, 12 );
		dipLO2UllagePressureSensor[1].Connect( pBundle, 13 );
		dipLO2UllagePressureSensor[2].Connect( pBundle, 14 );
		dipLO2UllagePressureSensor[3].Connect( pBundle, 15 );

		pBundle = BundleManager()->CreateBundle( "ET_LH2_SENSORS", 16 );
		dipLH2LowLevelSensor[0].Connect( pBundle, 0 );
		dipLH2LowLevelSensor[1].Connect( pBundle, 1 );
		dipLH2LowLevelSensor[2].Connect( pBundle, 2 );
		dipLH2LowLevelSensor[3].Connect( pBundle, 3 );
		dipLH2UllagePressureSensor[0].Connect( pBundle, 12 );
		dipLH2UllagePressureSensor[1].Connect( pBundle, 13 );
		dipLH2UllagePressureSensor[2].Connect( pBundle, 14 );
		dipLH2UllagePressureSensor[3].Connect( pBundle, 15 );

		pSSME_SOP = static_cast<SSME_SOP*> (FindSoftware( "SSME_SOP" ));
		assert( (pSSME_SOP != NULL) && "SSME_Operations::Realize.pSSME_SOP" );
		pIO_Control = static_cast<IO_Control*> (FindSoftware( "IO_Control" ));
		assert( (pIO_Control != NULL) && "SSME_Operations::Realize.pIO_Control" );
		return;
	}

	bool SSME_Operations::OnMajorModeChange( unsigned int newMajorMode )
	{
		switch (newMajorMode)
		{
			case 102:
			case 103:
			case 104:
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


	void SSME_Operations::SetLowLevelSensorArmFlag( void )
	{
		LowLevelSensorArm = true;
		return;
	}

	void SSME_Operations::SetLO2LowLevelSensorDsblFlag( int num )
	{
		assert( (num >= 1) && (num <= 4) && "SSME_Operations::SetLO2LowLevelSensorDsblFlag.num" );
		LO2LowLevelSensorDsblFlag[num - 1] = true;
		return;
	}

	void SSME_Operations::SetLH2LowLevelSensorDsblFlag( int num )
	{
		assert( (num >= 1) && (num <= 4) && "SSME_Operations::SetLH2LowLevelSensorDsblFlag.num" );
		LH2LowLevelSensorDsblFlag[num - 1] = true;
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

	bool SSME_Operations::GetZeroThrustFlag( void ) const
	{
		return ZeroThrust;
	}

	bool SSME_Operations::GetFailFlag( int eng ) const
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_Operations::GetFailFlag.eng" );
		return FailFlag[eng - 1];
	}
}