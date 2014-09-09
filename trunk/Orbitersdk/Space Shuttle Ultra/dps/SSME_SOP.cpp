#include "SSME_SOP.h"
#include "..\Atlantis.h"
#include <UltraMath.h>
#include "assert.h"


namespace dps
{
	SSME_SOP::SSME_SOP( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "SSME_SOP" )
	{
		for (int i = 0; i < 3; i++)
		{
			StartEnableCommand[i] = false;
			EngineStartCommand[i] = false;
			ShutdownEnableCommand[i] = false;
			ShutdownCommand[i] = false;
			ThrottleCommand[i] = false;
			OxidizerDumpStartCommand[i] = false;
			DumpStopCommand[i] = false;
			LimitInhibitCommand[i] = false;
			LimitEnableCommand[i] = false;
			DCUSwitchVDTCommand[i] = false;

			ShutdownEnableCommandIssued[i] = false;

			ShutdownPhase[i] = false;
			PostShutdownPhase[i] = false;
			HydraulicLockupMode[i] = false;
			ElectricalLockupMode[i] = false;
			EngineReadyMode[i] = false;

			PadDataPathFailure[i] = false;
			FlightDataPathFailure[i] = false;
			CommandPathFailure[i] = false;
			MajorComponentFailure[i] = false;
			LimitExceeded[i] = false;
			ChannelFailure[i] = false;

			PrimaryDataFail[i] = false;
			SecondaryDataFail[i] = false;
			DCUProcess[i] = false;

			PrimaryFailCounter[i] = 0;
			SecondaryFailCounter[i] = 0;
			DataFailCounter[i] = 0;
		}
		return;
	}

	SSME_SOP::~SSME_SOP( void )
	{
		return;
	}

	void SSME_SOP::OnPreStep( double SimT, double DeltaT, double MJD )
	{
		for (int i = 0; i < 3; i++)
		{
			// always gets previous time

			STS()->pEIU[i]->readpri( pridata[i] );
			STS()->pEIU[i]->readsec( secdata[i] );
			
			/*A data path failure occurs when the GPC’s (PASS or BFS) either do not see the main engine time reference
			word (TREF) updating or when the two main engine identification words (ID words 1 & 2) are not one’s
			complements.*/
			PadDataPathFailure[i] = false;

			if (!Eq( pridata[i][0] + ((double)pridata[i][1] / 10000), SimT - DeltaT, 0.0001 ))
			{
				if (PrimaryDataFail[i] == true)
				{
					PadDataPathFailure[i] = true;
				}
				else
				{
					PrimaryDataFail[i] = true;
				}
			}
			else
			{
				PrimaryDataFail[i] = false;
				PrimaryFailCounter[i] = 0;
			}

			if ((PrimaryDataFail[i] == true) || (GetMajorMode() == 101))// SRB not ignited
			{
				if (!Eq( secdata[i][0] + ((double)secdata[i][1] / 10000), SimT - DeltaT, 0.0001 ))
				{
					if (SecondaryDataFail[i] == true)
					{
						PadDataPathFailure[i] = true;
					}
					else
					{
						SecondaryDataFail[i] = true;
					}
				}
				else
				{
					SecondaryDataFail[i] = false;
					SecondaryFailCounter[i] = 0;
				}

				if (PrimaryDataFail[i] == true)
				{
					if (SecondaryDataFail[i] == true)
					{
						if (DCUProcess[i] == false)
						{
							DataFailCounter[i]++;

							if (DataFailCounter[i] == 2)
							{
								// set TVC servo bypass ovrd flag
							}
							else
							{
								if (DataFailCounter[i] == 4)
								{
									DCUSwitchVDTCommand[i] = true;
									DCUProcess[i] = true;
									DataFailCounter[i] = 0;
								}
							}
						}
						else
						{
							if (FlightDataPathFailure[i] == false)
							{
								DataFailCounter[i]++;

								if (DataFailCounter[i] == DATA_FAIL)
								{
									PercentChamberPress[i] = 0;// so it displays 0 on data path failure
									FlightDataPathFailure[i] = true;
								}
							}
						}
					}
					else
					{
						ProcessSecData( i );
					}
				}
				else
				{
					ProcessPriData( i );
				}
			}
			else
			{
				ProcessPriData( i );
			}

			if ((PrimaryDataFail[i] == false) || (SecondaryDataFail[i] == false))
			{
				// HACK command and channel status not really perfect
				if (CommandStatus[i] == 1)// || (CommandStatus[i] == 2))
				{
					CommandPathFailure[i] = true;
				}

				if (GetMajorMode() == 101)
				{
					if (CommandStatus[i] > 0)
					{
						if (ChannelStatus[i] != 0)
						{
							ChannelFailure[i] = true;
						}
					}
				}

				HydraulicLockupMode[i] = false;
				ElectricalLockupMode[i] = false;

				switch (Phase[i])
				{
					case 4:// Mainstage
						switch (Mode[i])
						{
							case 4:// Hydraulic Lockup
								HydraulicLockupMode[i] = true;
								break;
							case 5:// Electrical Lockup
								ElectricalLockupMode[i] = true;
								break;
						}
						break;
					case 5:// Shutdown
						ShutdownPhase[i] = true;
						break;
					case 6:// Post-Shutdown
						ShutdownPhase[i] = false;
						PostShutdownPhase[i] = true;
						break;
					case 2:// Start-Prep
						if (Mode[i] == 5)// Engine Ready
						{
							EngineReadyMode[i] = true;
						}
						else
						{
							EngineReadyMode[i] = false;
						}
						break;
				}

				MajorComponentFailure[i] = false;
				LimitExceeded[i] = false;

				if (SelfTestStatus[i] == 2)
				{
					MajorComponentFailure[i] = true;
				}
				else if (SelfTestStatus[i] == 3)
				{
					LimitExceeded[i] = true;
				}
			}
		}
		return;
	}

	void SSME_SOP::OnPostStep( double SimT, double DeltaT, double MJD )
	{
		for (int i = 0; i < 3; i++)
		{
			if (LastCommand[i] == STEN)
			{
				if (EngineStartCommand[i] == true)
				{
					STS()->pEIU[i]->command( IGNT );
					LastCommand[i] = IGNT;
					EngineStartCommand[i] = false;
					StartEnableCommand[i] = false;
				}
			}
			else
			{
				if (LastCommand[i] == SDEN)
				{
					if (ShutdownCommand[i] == true)
					{
						STS()->pEIU[i]->command( STDN );
						LastCommand[i] = STDN;
						ShutdownCommand[i] = false;
						ShutdownEnableCommandIssued[i] = false;
					}
				}
				else
				{
					if (ShutdownEnableCommand[i] == true)
					{
						STS()->pEIU[i]->command( SDEN );
						LastCommand[i] = SDEN;
						ShutdownEnableCommand[i] = false;
						ShutdownEnableCommandIssued[i] = true;
					}
					else if (DCUSwitchVDTCommand[i] == true)
					{
						STS()->pEIU[i]->command( SVRC );
						LastCommand[i] = SVRC;
						DCUSwitchVDTCommand[i] = false;
					}
					else if (LimitInhibitCommand[i] == true)
					{
						STS()->pEIU[i]->command( INLS );
						LastCommand[i] = INLS;
						LimitInhibitCommand[i] = false;
					}
					else if (LimitEnableCommand[i] == true)
					{
						STS()->pEIU[i]->command( ENLS );
						LastCommand[i] = ENLS;
						LimitEnableCommand[i] = false;
					}
					else if (ThrottleCommand[i] == true)
					{
						STS()->pEIU[i]->command( THRT + (unsigned short)round( (CommandedThrottle - MPL) * 10 ) );
						ThrottleCommand[i] = false;
					}
					else if (StartEnableCommand[i] == true)
					{
						STS()->pEIU[i]->command( STEN );
						LastCommand[i] = STEN;
						StartEnableCommand[i] = false;
					}
					else if (OxidizerDumpStartCommand[i] == true)
					{
						STS()->pEIU[i]->command( LOXD );
						LastCommand[i] = LOXD;
						OxidizerDumpStartCommand[i] = false;
					}
					else if (DumpStopCommand[i] == true)
					{
						STS()->pEIU[i]->command( TMSQ );
						LastCommand[i] = TMSQ;
						DumpStopCommand[i] = false;
					}
				}
			}
		}
		return;
	}

	bool SSME_SOP::OnMajorModeChange( unsigned int newMajorMode )
	{
		switch (newMajorMode)
		{
			case 101:
			case 102:
			case 103:
			case 104:
			case 601:
			case 602:
			case 603:
				return true;
			default:
				return false;
		}
	}

	void SSME_SOP::ProcessPriData( int eng )
	{
		PercentChamberPress[eng] = round( pridata[eng][5] / 27.46789 );
		Phase[eng] = (pridata[eng][2] & 0x0700) >> 8;
		Mode[eng] = (pridata[eng][2] & 0x3800) >> 11;
		SelfTestStatus[eng] = (pridata[eng][2] & 0xC000) >> 14;
		ChannelStatus[eng] = (pridata[eng][2] & 0x0038) >> 3;
		CommandStatus[eng] = (pridata[eng][2] & 0x0006) >> 1;
		return;
	}

	void SSME_SOP::ProcessSecData( int eng )
	{
		PercentChamberPress[eng] = round( secdata[eng][5] / 27.46789 );
		Phase[eng] = (secdata[eng][2] & 0x0700) >> 8;
		Mode[eng] = (secdata[eng][2] & 0x3800) >> 11;
		SelfTestStatus[eng] = (secdata[eng][2] & 0xC000) >> 14;
		ChannelStatus[eng] = (secdata[eng][2] & 0x0038) >> 3;
		CommandStatus[eng] = (secdata[eng][2] & 0x0006) >> 1;
		return;
	}

	void SSME_SOP::SetStartEnableCommandFlag( int eng )
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::SetStartEnableCommandFlag.eng" );
		StartEnableCommand[eng - 1] = true;
		return;
	}

	void SSME_SOP::SetEngineStartCommandFlag( int eng )
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::SetEngineStartCommandFlag.eng" );
		EngineStartCommand[eng - 1] = true;
		return;
	}

	void SSME_SOP::SetShutdownEnableCommandFlag( int eng )
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::SetShutdownEnableCommandFlag.eng" );
		ShutdownEnableCommand[eng - 1] = true;
		return;
	}

	void SSME_SOP::SetShutdownCommandFlag( int eng )
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::SetShutdownCommandFlag.eng" );
		ShutdownCommand[eng - 1] = true;
		return;
	}

	bool SSME_SOP::SetThrottlePercent( double pct )
	{
		if ((pct < MPL) || (pct > FPL)) return false;
		
		CommandedThrottle = pct;
		ThrottleCommand[0] = true;
		ThrottleCommand[1] = true;
		ThrottleCommand[2] = true;
		return true;
	}

	void SSME_SOP::SetLimitInhibitCommandFlag( int eng )
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::SetLimitInhibitCommandFlag.eng" );
		LimitInhibitCommand[eng - 1] = true;
		return;
	}

	void SSME_SOP::SetLimitEnableCommandFlag( int eng )
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::SetLimitEnableCommandFlag.eng" );
		LimitEnableCommand[eng - 1] = true;
		return;
	}

	void SSME_SOP::SetOxidizerDumpStartCommandFlag( int eng )
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::SetOxidizerDumpStartCommandFlag.eng" );
		OxidizerDumpStartCommand[eng - 1] = true;
		return;
	}

	void SSME_SOP::SetDumpStopCommandFlag( int eng )
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::SetDumpStopCommandFlag.eng" );
		DumpStopCommand[eng - 1] = true;
		return;
	}

	bool SSME_SOP::GetShutdownEnableCommandIssuedFlag( int eng ) const
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::GetShutdownEnableCommandIssuedFlag.eng" );
		return ShutdownEnableCommandIssued[eng - 1];
	}

	bool SSME_SOP::GetShutdownPhaseFlag( int eng ) const
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::GetShutdownPhaseFlag.eng" );
		return ShutdownPhase[eng - 1];
	}

	bool SSME_SOP::GetPostShutdownPhaseFlag( int eng ) const
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::GetPostShutdownPhaseFlag.eng" );
		return PostShutdownPhase[eng - 1];
	}

	bool SSME_SOP::GetHydraulicLockupModeFlag( int eng ) const
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::GetHydraulicLockupModeFlag.eng" );
		return HydraulicLockupMode[eng - 1];
	}

	bool SSME_SOP::GetElectricalLockupModeFlag( int eng ) const
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::GetElectricalLockupModeFlag.eng" );
		return ElectricalLockupMode[eng - 1];
	}

	bool SSME_SOP::GetEngineReadyModeFlag( int eng ) const
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::GetEngineReadyModeFlag.eng" );
		return EngineReadyMode[eng - 1];
	}

	bool SSME_SOP::GetPadDataPathFailureFlag( int eng ) const
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::GetPadDataPathFailureFlag.eng" );
		return PadDataPathFailure[eng - 1];
	}

	bool SSME_SOP::GetFlightDataPathFailureFlag( int eng ) const
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::GetFlightDataPathFailureFlag.eng" );
		return FlightDataPathFailure[eng - 1];
	}

	bool SSME_SOP::GetCommandPathFailureFlag( int eng ) const
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::GetCommandPathFailureFlag.eng" );
		return CommandPathFailure[eng - 1];
	}

	bool SSME_SOP::GetMajorComponentFailureFlag( int eng ) const
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::GetMajorComponentFailureFlag.eng" );
		return MajorComponentFailure[eng - 1];
	}

	bool SSME_SOP::GetLimitExceededFlag( int eng ) const
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::GetLimitExceededFlag.eng" );
		return LimitExceeded[eng - 1];
	}

	bool SSME_SOP::GetChannelFailureFlag( int eng ) const
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::GetChannelFailureFlag.eng" );
		return ChannelFailure[eng - 1];
	}

	double SSME_SOP::GetPercentChamberPressVal( int eng ) const
	{
		assert( (eng >= 1) && (eng <= 3) && "SSME_SOP::GetPercentChamberPressVal.eng" );
		return PercentChamberPress[eng - 1];
	}
}