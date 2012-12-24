#include "SSME_SOP.h"
#include "..\Atlantis.h"
#include <UltraMath.h>


namespace dps
{
	SSME_SOP::SSME_SOP( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "SSME_SOP" )
	{
		StartEnableCommand[0] = false;
		StartEnableCommand[1] = false;
		StartEnableCommand[2] = false;

		EngineStartCommand[0] = false;
		EngineStartCommand[1] = false;
		EngineStartCommand[2] = false;

		ShutdownEnableCommand[0] = false;
		ShutdownEnableCommand[1] = false;
		ShutdownEnableCommand[2] = false;

		ShutdownCommand[0] = false;
		ShutdownCommand[1] = false;
		ShutdownCommand[2] = false;

		Throttle[0] = false;
		Throttle[1] = false;
		Throttle[2] = false;

		OxidizerDumpStart[0] = false;
		OxidizerDumpStart[1] = false;
		OxidizerDumpStart[2] = false;

		DumpStop[0] = false;
		DumpStop[1] = false;
		DumpStop[2] = false;

		MECOCommand = false;

		MECOConfirmed = false;

		ShutdownEnableCommandIssued[0] = false;
		ShutdownEnableCommandIssued[1] = false;
		ShutdownEnableCommandIssued[2] = false;

		ShutdownPhase[0] = false;
		ShutdownPhase[1] = false;
		ShutdownPhase[2] = false;

		PostShutdownPhase[0] = false;
		PostShutdownPhase[1] = false;
		PostShutdownPhase[2] = false;

		DataPathFailure[0] = false;
		DataPathFailure[1] = false;
		DataPathFailure[2] = false;

		CommandPathFailure[0] = false;
		CommandPathFailure[1] = false;
		CommandPathFailure[2] = false;

		HydraulicLockup[0] = false;
		HydraulicLockup[1] = false;
		HydraulicLockup[2] = false;

		ElectricalLockup[0] = false;
		ElectricalLockup[1] = false;
		ElectricalLockup[2] = false;

		PrimaryDataFail[0] = false;
		PrimaryDataFail[1] = false;
		PrimaryDataFail[2] = false;

		SecondaryDataFail[0] = false;
		SecondaryDataFail[1] = false;
		SecondaryDataFail[2] = false;

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

			if (!Eq( pridata[i][0] + ((double)pridata[i][1] / 10000), SimT - DeltaT, 0.0001 ))
			{
				PrimaryDataFail[i] = true;
			}

			if (PrimaryDataFail[i] == true)
			{
				if (!Eq( secdata[i][0] + ((double)secdata[i][1] / 10000), SimT - DeltaT, 0.0001 ))
				{
					SecondaryDataFail[i] = true;
				}

				if (SecondaryDataFail[i] == true)
				{
					DataPathFailure[i] = true;
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

		if (GetMajorMode() >= 102)
		{
			if ((MECOConfirmed == false) && (
				((PercentChamberPress[0] < 30) && (PercentChamberPress[1] < 30) && (PercentChamberPress[2] < 30)) ||
				((DataPathFailure[0] == true) && (PercentChamberPress[1] < 30) && (PercentChamberPress[2] < 30)) ||
				((PercentChamberPress[0] < 30) && (DataPathFailure[1] == true) && (PercentChamberPress[2] < 30)) ||
				((PercentChamberPress[0] < 30) && (PercentChamberPress[1] < 30) && (DataPathFailure[2] == true))))
			{
				MECOConfirmed = true;
				oapiWriteLog( "MECO Confirmed" );
				// HACK LOX dump here for now
				LOXDumptimeA = SimT;
			}

			if ((SimT - LOXDumptimeA) > 120)// wait 120s for dump start
			{
				OxidizerDumpStart[0] = true;
				OxidizerDumpStart[1] = true;
				OxidizerDumpStart[2] = true;
				LOXDumptimeA = 99999;
				LOXDumptimeB = SimT;
			}

			if ((SimT - LOXDumptimeB) > 120)// stop dump after 120s
			{
				DumpStop[0] = true;
				DumpStop[1] = true;
				DumpStop[2] = true;
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
					else if (Throttle[i] == true)
					{
						STS()->pEIU[i]->command( THRT + (unsigned short)round( (CommandedThrottle * 1023) / 109 ) );
						Throttle[i] = false;
					}
					else if (StartEnableCommand[i] == true)
					{
						STS()->pEIU[i]->command( STEN );
						LastCommand[i] = STEN;
						StartEnableCommand[i] = false;
					}
					else if (OxidizerDumpStart[i] == true)
					{
						STS()->pEIU[i]->command( LOXD );
						LastCommand[i] = LOXD;
						OxidizerDumpStart[i] = false;
					}
					else if (DumpStop[i] == true)
					{
						STS()->pEIU[i]->command( TMSQ );
						LastCommand[i] = TMSQ;
						OxidizerDumpStart[i] = false;
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

		// phase
		switch (pridata[eng][3] & 0x0E00)
		{
			case 0x0800:// Mainstage
				// mode
				switch (pridata[eng][3] & 0x7000)
				{
					case 0x4000:// Hydraulic Lockup
						HydraulicLockup[eng] = true;
						ElectricalLockup[eng] = false;
						break;
					case 0x5000:// Electrical Lockup
						HydraulicLockup[eng] = false;
						ElectricalLockup[eng] = true;
						break;
					default:
						HydraulicLockup[eng] = false;
						ElectricalLockup[eng] = false;
						break;
				}
				break;
			case 0x0A00:// Shutdown
				ShutdownPhase[eng] = true;
				PostShutdownPhase[eng] = false;
				break;
			case 0x0C00:// Post-Shutdown
				ShutdownPhase[eng] = false;
				PostShutdownPhase[eng] = true;
				break;
			default:
				ShutdownPhase[eng] = false;
				PostShutdownPhase[eng] = false;
				break;
		}
		return;
	}

	void SSME_SOP::ProcessSecData( int eng )
	{
		PercentChamberPress[eng] = secdata[eng][5] / 27.46789;

		// phase
		switch (secdata[eng][3] & 0x0E00)
		{
			case 0x0800:// Mainstage
				// mode
				switch (secdata[eng][3] & 0x7000)
				{
					case 0x4000:// Hydraulic Lockup
						HydraulicLockup[eng] = true;
						ElectricalLockup[eng] = false;
						break;
					case 0x5000:// Electrical Lockup
						HydraulicLockup[eng] = false;
						ElectricalLockup[eng] = true;
						break;
					default:
						HydraulicLockup[eng] = false;
						ElectricalLockup[eng] = false;
						break;
				}
				break;
			case 0x0A00:// Shutdown
				ShutdownPhase[eng] = true;
				PostShutdownPhase[eng] = false;
				break;
			case 0x0C00:// Post-Shutdown
				ShutdownPhase[eng] = false;
				PostShutdownPhase[eng] = true;
				break;
			default:
				ShutdownPhase[eng] = false;
				PostShutdownPhase[eng] = false;
				break;
		}
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
		Throttle[0] = true;
		Throttle[1] = true;
		Throttle[2] = true;
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

	/*void SSME_SOP::SetOxidizerDumpStartFlag( int eng )
	{
		OxidizerDumpStart[eng - 1] = true;
		return;
	}

	void SSME_SOP::SetDumpStopFlag( int eng )
	{
		DumpStop[eng - 1] = true;
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

	bool SSME_SOP::GetDataPathFailureFlag( int eng ) const
	{
		return DataPathFailure[eng - 1];
	}

	bool SSME_SOP::GetCommandPathFailureFlag( int eng ) const
	{
		return CommandPathFailure[eng - 1];
	}

	bool SSME_SOP::GetHydraulicLockupFlag( int eng ) const
	{
		return HydraulicLockup[eng - 1];
	}

	bool SSME_SOP::GetElectricalLockupFlag( int eng ) const
	{
		return ElectricalLockup[eng - 1];
	}

	double SSME_SOP::GetPercentChamberPressVal( int eng ) const
	{
		return PercentChamberPress[eng - 1];
	}
}