#include "SSME_SOP.h"
#include "..\Atlantis.h"
#include <UltraMath.h>


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

			PrimaryDataFail[i] = false;
			SecondaryDataFail[i] = false;

			PrimaryFailCounter[i] = 0;
			SecondaryFailCounter[i] = 0;
			DataFailCounter[i] = 0;
		}

		MECOCommand = false;

		MECOConfirmed = false;

		LOXDumptimeA = 99999;
		LOXDumptimeB = 99999;
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

			PadDataPathFailure[i] = false;

			if (!Eq( pridata[i][0] + ((double)pridata[i][1] / 10000), SimT - DeltaT, 0.0001 ))
			{
				PrimaryFailCounter[i]++;

				if (PrimaryFailCounter[i] < 2)
				{
					PrimaryDataFail[i] = true;
				}
				else
				{
					PadDataPathFailure[i] = true;
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
					SecondaryFailCounter[i]++;

					if (SecondaryFailCounter[i] < 2)
					{
						SecondaryDataFail[i] = true;
					}
					else
					{
						PadDataPathFailure[i] = true;
					}

					if (PrimaryDataFail[i] == true)
					{
						if (FlightDataPathFailure[i] == false)
						{
							DataFailCounter[i]++;

							if (DataFailCounter[i] == DATA_FAIL)
							{
								FlightDataPathFailure[i] = true;
							}
						}
					}
					else
					{
						ProcessPriData( i );
					}
				}
				else
				{
					SecondaryDataFail[i] = false;
					SecondaryFailCounter[i] = 0;

					if (PrimaryDataFail[i] == true)
					{
						ProcessSecData( i );
					}
					else
					{
						ProcessPriData( i );
					}
				}
			}
			else
			{
				ProcessPriData( i );
			}
			
			HydraulicLockupMode[i] = false;
			ElectricalLockupMode[i] = false;
			switch (Phase[i])
			{
				case 4:// Mainstage
					switch (Mode[i] & 0x7000)
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
			if (SelfTestStatus[i] == 1)
			{
				MajorComponentFailure[i] = true;
			}

			// TODO finish logic below
			//CommandPathFailure[i] = false;
			if (CommandStatus[i] == 1)
			{
				CommandPathFailure[i] = true;
			}
		}

		if (GetMajorMode() >= 102)
		{
			if ((MECOConfirmed == false) && (
				((PercentChamberPress[0] < 30) && (PercentChamberPress[1] < 30) && (PercentChamberPress[2] < 30)) ||
				((FlightDataPathFailure[0] == true) && (PercentChamberPress[1] < 30) && (PercentChamberPress[2] < 30)) ||
				((PercentChamberPress[0] < 30) && (FlightDataPathFailure[1] == true) && (PercentChamberPress[2] < 30)) ||
				((PercentChamberPress[0] < 30) && (PercentChamberPress[1] < 30) && (FlightDataPathFailure[2] == true))))
			{
				MECOConfirmed = true;
				oapiWriteLog( "MECO Confirmed" );
				// HACK LOX dump here for now
				LOXDumptimeA = SimT;
			}

			if ((SimT - LOXDumptimeA) > 120)// wait 120s for dump start
			{
				OxidizerDumpStartCommand[0] = true;
				OxidizerDumpStartCommand[1] = true;
				OxidizerDumpStartCommand[2] = true;
				LOXDumptimeA = 99999;
				LOXDumptimeB = SimT;
			}

			if ((SimT - LOXDumptimeB) > 120)// stop dump after 120s
			{
				DumpStopCommand[0] = true;
				DumpStopCommand[1] = true;
				DumpStopCommand[2] = true;
				LOXDumptimeB = 99999;
			}

			// HACK ECO sensors
			if ((STS()->GetETPropellant_B() <= 0.2) && (MECOCommand == false))// 0.2% is enough to shutdown the engines
			{
				SetMECOCommandFlag();
				oapiWriteLog( "MECO Low Level Cutoff" );
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
					else if (ThrottleCommand[i] == true)
					{
						STS()->pEIU[i]->command( THRT + (unsigned short)round( (CommandedThrottle * 1023) / 109 ) );
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

	bool SSME_SOP::OnParseLine( const char* keyword, const char* value )
	{
		return false;
	}

	void SSME_SOP::OnSaveState( FILEHANDLE scn ) const
	{
		return;
	}

	void SSME_SOP::Realize( void )
	{
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
			case 105:
				return true;
			default:
				return false;
		}
	}

	void SSME_SOP::ProcessPriData( int eng )
	{
		PercentChamberPress[eng] = pridata[eng][5] / 27.46789;
		Phase[eng] = (pridata[eng][3] & 0x0E00) >> 9;
		Mode[eng] = (pridata[eng][3] & 0x7000) >> 12;
		SelfTestStatus[eng] = (pridata[eng][3] & 0x8000) >> 15;
		CommandStatus[eng] = (pridata[eng][3] & 0x0004) >> 2;
		return;
	}

	void SSME_SOP::ProcessSecData( int eng )
	{
		PercentChamberPress[eng] = secdata[eng][5] / 27.46789;
		Phase[eng] = (secdata[eng][3] & 0x0E00) >> 9;
		Mode[eng] = (secdata[eng][3] & 0x7000) >> 12;
		SelfTestStatus[eng] = (secdata[eng][3] & 0x8000) >> 15;
		CommandStatus[eng] = (secdata[eng][3] & 0x0004) >> 2;
		return;
	}

	void SSME_SOP::SetStartEnableCommandFlag( int eng )
	{
		StartEnableCommand[eng - 1] = true;
		return;
	}

	void SSME_SOP::SetEngineStartCommandFlag( int eng )
	{
		EngineStartCommand[eng - 1] = true;
		return;
	}

	void SSME_SOP::SetShutdownEnableCommandFlag( int eng )
	{
		ShutdownEnableCommand[eng - 1] = true;
		return;
	}

	void SSME_SOP::SetShutdownCommandFlag( int eng )
	{
		ShutdownCommand[eng - 1] = true;
		return;
	}

	void SSME_SOP::SetThrottlePercent( double pct )
	{
		CommandedThrottle = pct;
		ThrottleCommand[0] = true;
		ThrottleCommand[1] = true;
		ThrottleCommand[2] = true;
		return;
	}

	void SSME_SOP::SetMECOCommandFlag( void )
	{
		ShutdownEnableCommand[0] = true;
		ShutdownEnableCommand[1] = true;
		ShutdownEnableCommand[2] = true;
		ShutdownCommand[0] = true;
		ShutdownCommand[1] = true;
		ShutdownCommand[2] = true;

		MECOCommand = true;
		return;
	}

	/*void SSME_SOP::SetOxidizerDumpStartCommandFlag( int eng )
	{
		OxidizerDumpStartCommand[eng - 1] = true;
		return;
	}

	void SSME_SOP::SetDumpStopCommandFlag( int eng )
	{
		DumpStopCommand[eng - 1] = true;
		return;
	}*/

	bool SSME_SOP::GetMECOCommandFlag( void ) const
	{
		return MECOCommand;
	}

	bool SSME_SOP::GetMECOConfirmedFlag( void ) const
	{
		return MECOConfirmed;
	}

	bool SSME_SOP::GetShutdownEnableCommandIssuedFlag( int eng ) const
	{
		return ShutdownEnableCommandIssued[eng - 1];
	}

	bool SSME_SOP::GetShutdownPhaseFlag( int eng ) const
	{
		return ShutdownPhase[eng - 1];
	}

	bool SSME_SOP::GetPostShutdownPhaseFlag( int eng ) const
	{
		return PostShutdownPhase[eng - 1];
	}

	bool SSME_SOP::GetHydraulicLockupModeFlag( int eng ) const
	{
		return HydraulicLockupMode[eng - 1];
	}

	bool SSME_SOP::GetElectricalLockupModeFlag( int eng ) const
	{
		return ElectricalLockupMode[eng - 1];
	}

	bool SSME_SOP::GetEngineReadyModeFlag( int eng ) const
	{
		return EngineReadyMode[eng - 1];
	}

	bool SSME_SOP::GetPadDataPathFailureFlag( int eng ) const
	{
		return PadDataPathFailure[eng - 1];
	}

	bool SSME_SOP::GetFlightDataPathFailureFlag( int eng ) const
	{
		return FlightDataPathFailure[eng - 1];
	}

	bool SSME_SOP::GetCommandPathFailureFlag( int eng ) const
	{
		return CommandPathFailure[eng - 1];
	}

	bool SSME_SOP::GetMajorComponentFailureFlag( int eng ) const
	{
		return MajorComponentFailure[eng - 1];
	}

	double SSME_SOP::GetPercentChamberPressVal( int eng ) const
	{
		return PercentChamberPress[eng - 1];
	}
}