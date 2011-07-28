#include "SSME.h"
#include "..\Atlantis.h"


namespace mps
{
	SSME::SSME( AtlantisSubsystemDirector* _director, const string& _ident, int nID ):AtlantisSubsystem( _director, _ident )
	{
		ID = nID;
		VDT = new VDT_128;
		activeDCU = DCU_A;
		//thSSME = NULL;

		STATUSWORD = STARTPREPARATION_PURGESEQUENCE3;
		PSN4time = -1;
		ESCtime = -1;
		COtime = -1;
		StartEnable = false;
		ShutdownEnable = false;

		ThrottleCmdTme = -1;

		PC_REF = 0;
		PC_CMD = 0;

		//dir = _V( 0.0, 0.0, 1.0 );

		/*switch (ID)
		{
			case 1:
				pos = SSMET_REF;
				break;
			case 2: 
				pos = SSMEL_REF;
				break;
			case 3:
				pos = SSMER_REF;
		}*/

		ptrCCV = new BasicValve( 0, 120 );// DONO max rate
		ptrMOV = new BasicValve( 0, 120 );// DONO max rate
		ptrMFV = new BasicValve( 0, 150 );// DONO max rate
		ptrFPOV = new BasicValve( 0, 170 );// DONO max rate
		ptrOPOV = new BasicValve( 0, 210 );// DONO max rate
		ptrOBV = new ValveTypeBool( 1, 50 );// DONO max rate
		ptrFBV = new ValveTypeBool( 1, 50 );// DONO max rate
		return;
	}

	SSME::~SSME( void )
	{
		delete ptrCCV;
		delete ptrMOV;
		delete ptrMFV;
		delete ptrFPOV;
		delete ptrOPOV;
		delete ptrOBV;
		delete ptrFBV;

		delete VDT;
		// the end
	}

	void SSME::Realize()
	{
		STS()->SetSSMEParams( ID, FPL_THRUST, ISP0, ISP1 );
	}

	double SSME::PCfromOSFStoSTS( double pcOSFS )
	{
		return (pcOSFS * FPL);
	}

	double SSME::PCfromSTStoOSFS( double pcSTS )
	{
		return (pcSTS / FPL);
	}

	double SSME::PCfromPCTtoPSI( double pcPCT )
	{
		return ((pcPCT * RPL_PC_PRESS) / 100);
	}

	double SSME::PCfromPSItoPCT( double pcPSI )
	{
		return ((pcPSI * 100) / RPL_PC_PRESS);
	}

	void SSME::ReadSensors( void )
	{
		// MCC press
		// TODO cookup variations (within redlines... for now...)
		double pcread = STS()->GetSSMEThrustLevel( ID );
		pcread = PCfromPCTtoPSI( PCfromOSFStoSTS( pcread ) );
		PRESS_MCC_A1 = pcread;
		PRESS_MCC_A2 = pcread;
		PRESS_MCC_B1 = pcread;
		PRESS_MCC_B2 = pcread;
		return;
	}

	void SSME::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		// valves first
		ptrCCV->tmestp( fDeltaT );
		ptrMOV->tmestp( fDeltaT );
		ptrMFV->tmestp( fDeltaT );
		ptrFPOV->tmestp( fDeltaT );
		ptrOPOV->tmestp( fDeltaT );
		ptrOBV->tmestp( fDeltaT );
		ptrFBV->tmestp( fDeltaT );

		ReadSensors();

		switch (STATUSWORD)
		{
			// CHECKOUT
			case CHECKOUT_STANDBY:
				break;
			case CHECKOUT_COMPONENTCHECKOUT:
				break;
			// START PREPARATION
			case STARTPREPARATION_PURGESEQUENCE1:
				break;
			case STARTPREPARATION_PURGESEQUENCE2:
				break;
			case STARTPREPARATION_PURGESEQUENCE3:
				ptrOBV->Open();
				ptrFBV->Open();
				break;
			case STARTPREPARATION_PURGESEQUENCE4:
				// from T-235 to T-117 ???
				if ((fSimT - PSN4time) > 118) STATUSWORD = STARTPREPARATION_ENGINEREADY;
				break;
			case STARTPREPARATION_ENGINEREADY:
				ptrCCV->Open();// DONO rate
				ptrOBV->Close();
				ptrFBV->Close();
				break;
			// START/MAINSTAGE
			case STARTMAINSTAGE_STARTPHASESTARTINITIATION:
				if (ESCtime == -1)// first run, setup
				{
					PC_CMD = PCfromPCTtoPSI( 100 );
					ESCtime = fSimT;
					break;
				}
				Ignition( fSimT );
				if ((fSimT - ESCtime) > 2.4) STATUSWORD = STARTMAINSTAGE_STARTPHASETHRUSTBUILDUP;
				break;
			case STARTMAINSTAGE_STARTPHASETHRUSTBUILDUP:
				Ignition( fSimT );
				if (PCfromPSItoPCT( PRESS_MCC_A1 ) == 100) STATUSWORD = STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL;
				break;
			case STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL:
				// enter at ESC+3.8 ????
				if (PC_CMD != PRESS_MCC_A1)// TODO think about a margin in here....
				{
					Throttling( fDeltaT, fSimT );
				}
				else
				{
					// keep same pc as before
					ThrottleCmdTme = -1;
					STS()->SetSSMEThrustLevel( ID, PCfromSTStoOSFS( PCfromPSItoPCT( PC_CMD ) ) );
				}
				break;
			case STARTMAINSTAGE_FIXEDDENSITY:
				break;
			case STARTMAINSTAGE_THRUSTLIMITING:
				break;
			case STARTMAINSTAGE_ELECTRICALLOCKUP:
				break;
			case STARTMAINSTAGE_HYDRAULICLOCKUP:
				break;
			// SHUTDOWN
			case SHUTDOWN_THROTTLINGTOZEROTHRUST:
				if (COtime == -1)// first run, setup
				{
					PC_CMD = PCfromPCTtoPSI( 0 );
					SetCOTime();
					COtime = fSimT;
					break;
				}
				Shutdown( fSimT );
				// after all vlvs are closed move on
				if ((ptrCCV->GetPos() + ptrMOV->GetPos() + ptrMFV->GetPos() + ptrFPOV->GetPos() + ptrOPOV->GetPos()) == 0) STATUSWORD = SHUTDOWN_PROPELLANTVALVESCLOSED;
				break;
			case SHUTDOWN_PROPELLANTVALVESCLOSED:
				// TODO keep running down TPs...
				Shutdown( fSimT );
				if ((fSimT - COtime) > 8)// ???time???
				{
					STATUSWORD = POSTSHUTDOWN_STANDBY;
				}
				break;
			case SHUTDOWN_FAILSAFEPNEUMATIC:
				// TODO kill the engine
				STATUSWORD = POSTSHUTDOWN_STANDBY;
				break;
			// POST SHUTDOWN
			case POSTSHUTDOWN_STANDBY:
				break;
			case POSTSHUTDOWN_OXIDIZERDUMP:
				ptrMOV->Open();// DONO rate
				break;
			case POSTSHUTDOWN_TERMINATESEQUENCE:// DONO wait until all vlvs are closed???
				ptrCCV->Close();// DONO rate
				ptrMOV->Close();// DONO rate
				ptrMFV->Close();// DONO rate
				ptrFPOV->Close();// DONO rate
				ptrOPOV->Close();// DONO rate
				ptrOBV->Close();// DONO close this too???
				ptrFBV->Close();// DONO close this too???
				STATUSWORD = POSTSHUTDOWN_STANDBY;
				break;
			// PROM
			case PROM_STANDBY:
				break;
		}

		VDTUpdate( fSimT );
		return;
	}

	// EIU cmd
	bool SSME::cmdPurgeSequence1( void )
	{
		/////// remove start/shutdown enable ///////
		StartEnable = false;
		ShutdownEnable = false;
		/////// remove start/shutdown enable ///////

		switch (STATUSWORD)
		{
			case CHECKOUT_STANDBY:
			case STARTPREPARATION_PURGESEQUENCE2:
				STATUSWORD = STARTPREPARATION_PURGESEQUENCE1;
				return true;
		}
		return false;
	}

	bool SSME::cmdPurgeSequence2( void )
	{
		/////// remove start/shutdown enable ///////
		StartEnable = false;
		ShutdownEnable = false;
		/////// remove start/shutdown enable ///////

		if (STATUSWORD == STARTPREPARATION_PURGESEQUENCE1)
		{
			STATUSWORD = STARTPREPARATION_PURGESEQUENCE2;
			return true;
		}
		return false;
	}

	bool SSME::cmdPurgeSequence3( void )
	{
		/////// remove start/shutdown enable ///////
		StartEnable = false;
		ShutdownEnable = false;
		/////// remove start/shutdown enable ///////

		switch (STATUSWORD)
		{
			case STARTPREPARATION_PURGESEQUENCE2:
			case STARTPREPARATION_PURGESEQUENCE4:
				STATUSWORD = STARTPREPARATION_PURGESEQUENCE3;
				return true;
		}
		return false;
	}

	bool SSME::cmdPurgeSequence4( void )
	{
		/////// remove start/shutdown enable ///////
		StartEnable = false;
		ShutdownEnable = false;
		/////// remove start/shutdown enable ///////

		switch (STATUSWORD)
		{
			case STARTPREPARATION_PURGESEQUENCE3:
			case STARTPREPARATION_ENGINEREADY:
				PSN4time = VDT->DW1;
				STATUSWORD = STARTPREPARATION_PURGESEQUENCE4;
				return true;
		}
		return false;
	}

	bool SSME::cmdStartEnable( void )// TODO checks like in bool SSME::cmdStart( void )
	{
		/////// remove shutdown enable ///////
		ShutdownEnable = false;
		/////// remove shutdown enable ///////

		StartEnable = true;
		return true;
	}

	bool SSME::cmdStart( void )
	{
		/////// remove shutdown enable ///////
		ShutdownEnable = false;
		/////// remove shutdown enable ///////

		if ((STATUSWORD == STARTPREPARATION_ENGINEREADY) && (StartEnable == true))
		{
			STATUSWORD = STARTMAINSTAGE_STARTPHASESTARTINITIATION;
			return true;
		}
		return false;
	}

	bool SSME::cmdControllerReset( void )
	{
		/////// remove start/shutdown enable ///////
		StartEnable = false;
		ShutdownEnable = false;
		/////// remove start/shutdown enable ///////

		switch (STATUSWORD)
		{
			case CHECKOUT_STANDBY:
			case CHECKOUT_COMPONENTCHECKOUT:
			case STARTPREPARATION_PURGESEQUENCE1:
			case STARTPREPARATION_PURGESEQUENCE2:
			case STARTPREPARATION_PURGESEQUENCE3:
			case STARTPREPARATION_PURGESEQUENCE4:
			case STARTPREPARATION_ENGINEREADY:
			case POSTSHUTDOWN_STANDBY:
			case POSTSHUTDOWN_OXIDIZERDUMP:
			case POSTSHUTDOWN_TERMINATESEQUENCE:
				STATUSWORD = CHECKOUT_STANDBY;
				return true;
			default:
				return false;
		}
		return true;
	}

	bool SSME::cmdCheckoutStandby( void )
	{
		/////// remove start/shutdown enable ///////
		StartEnable = false;
		ShutdownEnable = false;
		/////// remove start/shutdown enable ///////

		switch (STATUSWORD)
		{
			case CHECKOUT_STANDBY:
			case CHECKOUT_COMPONENTCHECKOUT:
			case STARTPREPARATION_PURGESEQUENCE1:
			case STARTPREPARATION_PURGESEQUENCE2:
			case STARTPREPARATION_PURGESEQUENCE3:
			case STARTPREPARATION_PURGESEQUENCE4:
			case STARTPREPARATION_ENGINEREADY:
			case POSTSHUTDOWN_STANDBY:
			case POSTSHUTDOWN_OXIDIZERDUMP:
			case POSTSHUTDOWN_TERMINATESEQUENCE:
				STATUSWORD = CHECKOUT_STANDBY;
				return true;
			default:
				return false;
		}
		return true;
	}

	bool SSME::cmdTerminateSequence( void )
	{
		/////// remove start/shutdown enable ///////
		StartEnable = false;
		ShutdownEnable = false;
		/////// remove start/shutdown enable ///////

		switch (STATUSWORD)
		{
			case CHECKOUT_STANDBY:
			case CHECKOUT_COMPONENTCHECKOUT:
			case STARTPREPARATION_PURGESEQUENCE1:
			case STARTPREPARATION_PURGESEQUENCE2:
			case STARTPREPARATION_PURGESEQUENCE3:
			case STARTPREPARATION_PURGESEQUENCE4:
			case STARTPREPARATION_ENGINEREADY:
			case POSTSHUTDOWN_STANDBY:
			case POSTSHUTDOWN_OXIDIZERDUMP:
				STATUSWORD = POSTSHUTDOWN_TERMINATESEQUENCE;
				return true;
			default:
				return false;
		}
		return true;
	}

	bool SSME::cmdShutdownEnable( void )// TODO checks like in bool SSME::cmdShutdown( void )
	{
		/////// remove start enable ///////
		StartEnable = false;
		/////// remove start enable ///////

		ShutdownEnable = true;
		return true;
	}

	bool SSME::cmdShutdown( void )
	{
		/////// remove start enable ///////
		StartEnable = false;
		/////// remove start enable ///////

		if (ShutdownEnable == false) return false;

		switch (STATUSWORD)
		{
			case STARTPREPARATION_PURGESEQUENCE1:
			case STARTPREPARATION_PURGESEQUENCE2:
			case STARTPREPARATION_PURGESEQUENCE3:
			case STARTPREPARATION_PURGESEQUENCE4:
			case STARTPREPARATION_ENGINEREADY:
			case STARTMAINSTAGE_THRUSTLIMITING:
			case STARTMAINSTAGE_HYDRAULICLOCKUP:
				STATUSWORD = SHUTDOWN_FAILSAFEPNEUMATIC;
				return true;
			case STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL:
				ValveShutdownTableUpdate( PCfromPSItoPCT( VDT->DW6 ) );
				STATUSWORD = SHUTDOWN_THROTTLINGTOZEROTHRUST;
				return true;
			case STARTMAINSTAGE_STARTPHASESTARTINITIATION:// DONO goto SHUTDOWN_FAILSAFEPNEUMATIC ????
			case STARTMAINSTAGE_STARTPHASETHRUSTBUILDUP:
				// TODO new valve schedule update for the 2 above????
			case STARTMAINSTAGE_FIXEDDENSITY:
				STATUSWORD = SHUTDOWN_THROTTLINGTOZEROTHRUST;
				return true;
			default:
				return false;
		}
	}

	bool SSME::cmdFRT1( void )
	{
		/////// remove start/shutdown enable ///////
		StartEnable = false;
		ShutdownEnable = false;
		/////// remove start/shutdown enable ///////

		switch (STATUSWORD)
		{
			case STARTMAINSTAGE_STARTPHASESTARTINITIATION:
			case STARTMAINSTAGE_STARTPHASETHRUSTBUILDUP:
			case STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL:
			case STARTMAINSTAGE_FIXEDDENSITY:
			case STARTMAINSTAGE_THRUSTLIMITING:
			case STARTMAINSTAGE_ELECTRICALLOCKUP:
			case STARTMAINSTAGE_HYDRAULICLOCKUP:
				STATUSWORD = SHUTDOWN_FAILSAFEPNEUMATIC;
				return true;
			default:
				return false;
		}
	}

	bool SSME::cmdOxidizerDump( void )
	{
		/////// remove start/shutdown enable ///////
		StartEnable = false;
		ShutdownEnable = false;
		/////// remove start/shutdown enable ///////

		if (STATUSWORD == POSTSHUTDOWN_STANDBY)
		{
			STATUSWORD = POSTSHUTDOWN_OXIDIZERDUMP;
			return true;
		}
		return false;
	}

	bool SSME::cmdExitPROM( void )
	{
		/////// remove start/shutdown enable ///////
		StartEnable = false;
		ShutdownEnable = false;
		/////// remove start/shutdown enable ///////

		if (STATUSWORD == PROM_STANDBY)
		{
			STATUSWORD = POSTSHUTDOWN_STANDBY;
			return true;
		}
		return false;
	}

	bool SSME::cmdPowerOn( void )// TODO check if dead...
	{
		/////// remove start/shutdown enable ///////
		StartEnable = false;
		ShutdownEnable = false;
		/////// remove start/shutdown enable ///////

		STATUSWORD = PROM_STANDBY;
		return true;
	}

	bool SSME::cmdChannelReset( void )
	{
		/////// remove start/shutdown enable ///////
		StartEnable = false;
		ShutdownEnable = false;
		/////// remove start/shutdown enable ///////

		STATUSWORD = PROM_STANDBY;
		return true;
	}

	bool SSME::cmdThrottle( double itgtPC )
	{
		/////// remove start/shutdown enable ///////
		StartEnable = false;
		ShutdownEnable = false;
		/////// remove start/shutdown enable ///////

		if ((STATUSWORD == STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL) && (itgtPC <= FPL) && (itgtPC >= MPL))
		{
			PC_CMD = PCfromPCTtoPSI( itgtPC );
			ThrottleCmdTme = VDT->DW1;
			return true;
		}
		return false;
	}

	bool SSME::cmdPowerOff( void )
	{
		/////// remove start/shutdown enable ///////
		StartEnable = false;
		ShutdownEnable = false;
		/////// remove start/shutdown enable ///////

		if (STATUSWORD == POSTSHUTDOWN_STANDBY)
		{
			// shutdown...
			return true;
		}
		return false;
	}

	/*bool SSME::cmdDeactivateAllValves( void )
	{
		/////// remove start/shutdown enable ///////
		StartEnable = false;
		ShutdownEnable = false;
		/////// remove start/shutdown enable ///////

		return true;
	}*/

	int SSME::dataGetPrimaryData( VDT_32* PrimaryData )
	{
		memcpy( PrimaryData, VDT, sizeof(VDT_32) );
		return 0;
	}

	int SSME::dataGetSecondaryData( VDT_6* SecondaryData )
	{
		memcpy( SecondaryData, VDT, sizeof(VDT_6) );
		return 0;
	}

	/*THRUSTER_HANDLE SSME::GetHandle( void ) const
	{
		return thSSME;
	}*/

// PRIVATE

	void SSME::Ignition( double time )
	{
		double pc = dcPC_ESC( time - ESCtime );
		
		ValveScheduleIgnition( time - ESCtime );

		STS()->SetSSMEThrustLevel( ID, PCfromSTStoOSFS( pc ) );
		//PC_REF = PCfromPCTtoPSI( pc );
		return;
	}

	void SSME::Shutdown( double time )
	{
		double pc = dcPC_CO( time - COtime + COtimecoef );
		
		ValveScheduleShutdown( time - COtime );

		STS()->SetSSMEThrustLevel( ID, PCfromSTStoOSFS( pc ) );
		//PC_REF = PCfromPCTtoPSI( pc );
		return;
	}

	void SSME::Throttling( double dtime, double time )
	{
		ValveScheduleThrottle( PCfromPSItoPCT( PC_CMD ) );

		if (ThrottleCmdTme == -1)
		{
			ThrottleCmdTme = time;
			return;
		}

		if ((time - ThrottleCmdTme) >= 0.2)// 200ms delay in pc change
		{
			double pc = dcPC_MS( dtime );
			STS()->SetSSMEThrustLevel( ID, PCfromSTStoOSFS( pc ) );
		}
		//PC_REF = PCfromPCTtoPSI( pc );
		return;
	}

	void SSME::SetCOTime( void )
	{
		// pc > 100% -> < 0
		// pc = 100% -> = 0
		// pc < 100% -> > 0
		if (PCfromPSItoPCT( PRESS_MCC_A1 ) >= 39)
		{
			COtimecoef = (100 - PCfromPSItoPCT( PRESS_MCC_A1 )) / 122;
		}
		else
		{
			if (PCfromPSItoPCT( PRESS_MCC_A1 ) >= 8)
			{
				COtimecoef = ((50.92 - PCfromPSItoPCT( PRESS_MCC_A1 )) * 2.6) / 62;
			}
			else
			{
				COtimecoef = ((11.43 - PCfromPSItoPCT( PRESS_MCC_A1 )) * 4.2) / 8;
			}
		}
		return;
	}

	// Valve Schedules
	void SSME::ValveScheduleIgnition( double time )// TODO act upon vlv cmd failure
	{
		short index = 0;

		// move CCV
		do
		{
			if ((CCVScheduleIgnition_Time[index] <= time) && (CCVScheduleIgnition_Time[index + 1] > time))
			{
				ptrCCV->Move( CCVScheduleIgnition_Position[index], CCVScheduleIgnition_Rate[index] );
				break;
			}
			index++;
		} while (index < *CCVSIindex);

		// move MOV
		index = 0;
		do
		{
			if ((MOVScheduleIgnition_Time[index] <= time) && (MOVScheduleIgnition_Time[index + 1] > time))
			{
				ptrMOV->Move( MOVScheduleIgnition_Position[index], MOVScheduleIgnition_Rate[index] );
				break;
			}
			index++;
		} while (index < *MOVSIindex);

		// move MFV
		index = 0;
		do
		{
			if ((MFVScheduleIgnition_Time[index] <= time) && (MFVScheduleIgnition_Time[index + 1] > time))
			{
				ptrMFV->Move( MFVScheduleIgnition_Position[index], MFVScheduleIgnition_Rate[index] );
				break;
			}
			index++;
		} while (index < *MFVSIindex);

		// move FPOV
		index = 0;
		do
		{
			if ((FPOVScheduleIgnition_Time[index] <= time) && (FPOVScheduleIgnition_Time[index + 1] > time))
			{
				ptrFPOV->Move( FPOVScheduleIgnition_Position[index], FPOVScheduleIgnition_Rate[index] );
				break;
			}
			index++;
		} while (index < *FPOVSIindex);

		// move OPOV
		index = 0;
		do
		{
			if ((OPOVScheduleIgnition_Time[index] <= time) && (OPOVScheduleIgnition_Time[index + 1] > time))
			{
				ptrOPOV->Move( OPOVScheduleIgnition_Position[index], OPOVScheduleIgnition_Rate[index] );
				break;
			}
			index++;
		} while (index < *OPOVSIindex);

		return;
	}

	void SSME::ValveScheduleShutdown( double time )// TODO act upon vlv cmd failure
	{
		short index = 0;

		// move CCV
		do
		{
			if ((CCVScheduleShutdown_Time[index] <= time) && (CCVScheduleShutdown_Time[index + 1] > time))
			{
				ptrCCV->Move( CCVScheduleShutdown_Position[index], CCVScheduleShutdown_Rate[index] );
				break;
			}
			index++;
		} while (index < *CCVSSindex);

		// move MOV
		index = 0;
		do
		{
			if ((MOVScheduleShutdown_Time[index] <= time) && (MOVScheduleShutdown_Time[index + 1] > time))
			{
				ptrMOV->Move( MOVScheduleShutdown_Position[index], MOVScheduleShutdown_Rate[index] );
				break;
			}
			index++;
		} while (index < *MOVSSindex);

		// move MFV
		index = 0;
		do
		{
			if ((MFVScheduleShutdown_Time[index] <= time) && (MFVScheduleShutdown_Time[index + 1] > time))
			{
				ptrMFV->Move( MFVScheduleShutdown_Position[index], MFVScheduleShutdown_Rate[index] );
				break;
			}
			index++;
		} while (index < *MFVSSindex);

		// move OPOV
		index = 0;
		do
		{
			if ((OPOVScheduleShutdown_Time[index] <= time) && (OPOVScheduleShutdown_Time[index + 1] > time))
			{
				ptrOPOV->Move( OPOVScheduleShutdown_Position[index], OPOVScheduleShutdown_Rate[index] );
				break;
			}
			index++;
		} while (index < *OPOVSSindex);

		// move FPOV
		index = 0;
		do
		{
			if ((FPOVScheduleShutdown_Time[index] <= time) && (FPOVScheduleShutdown_Time[index + 1] > time))
			{
				ptrFPOV->Move( FPOVScheduleShutdown_Position[index], FPOVScheduleShutdown_Rate[index] );
				break;
			}
			index++;
		} while (index < *FPOVSSindex);

		return;
	}
}