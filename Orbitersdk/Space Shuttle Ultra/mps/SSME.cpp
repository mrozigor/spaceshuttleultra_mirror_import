#include "SSME.h"


namespace mps
{
	SSME::SSME( SubsystemDirector* _director, const string& _ident, int nID, VESSEL* Vessel, PROPELLANT_HANDLE phET, VECTOR3 pos, VECTOR3 dir, double ISP0, double ISP1, double FPL_THRUST ):AtlantisSubsystem( _director, _ident )
	{
		ID = nID;
		OV = Vessel;

		thSSME = OV->CreateThruster( pos, dir, FPL_THRUST, phET, ISP0, ISP1 );
		activeDCU = DCU_A;

		SSME_DATA_TABLE = new DATA_TABLE;

		return;
	}

	SSME::~SSME( void )
	{
		delete SSME_DATA_TABLE;
		// the end
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
		// TODO PCT psi
		return ((pcPCT * 3000) / 109);
	}

	void SSME::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		switch (SSME_DATA_TABLE->STATUS)
		{
			// CHECKOUT
			case CHECKOUT_STANDBY:
				break;
			case CHECKOUT_COMPONENTCHECKOUT:
				break;
			// START PREPARATION
			case STARTPREPARATION_PURGESEQUENCE1:
				switch (cmdSTATUS)
				{
					case CHECKOUT_STANDBY:
						SSME_DATA_TABLE->STATUS = CHECKOUT_STANDBY;
						break;
					case STARTPREPARATION_PURGESEQUENCE2:
						SSME_DATA_TABLE->STATUS = STARTPREPARATION_PURGESEQUENCE2;
						break;
					case POSTSHUTDOWN_TERMINATESEQUENCE:
						SSME_DATA_TABLE->STATUS = POSTSHUTDOWN_TERMINATESEQUENCE;
						break;
				}
				break;
			case STARTPREPARATION_PURGESEQUENCE2:
				switch (cmdSTATUS)
				{
					case CHECKOUT_STANDBY:
						SSME_DATA_TABLE->STATUS = CHECKOUT_STANDBY;
						break;
					case STARTPREPARATION_PURGESEQUENCE1:
						SSME_DATA_TABLE->STATUS = STARTPREPARATION_PURGESEQUENCE1;
						break;
					case STARTPREPARATION_PURGESEQUENCE3:
						SSME_DATA_TABLE->STATUS = STARTPREPARATION_PURGESEQUENCE3;
						break;
					case POSTSHUTDOWN_TERMINATESEQUENCE:
						SSME_DATA_TABLE->STATUS = POSTSHUTDOWN_TERMINATESEQUENCE;
						break;
				}
				break;
			case STARTPREPARATION_PURGESEQUENCE3:
				switch (cmdSTATUS)
				{
					case CHECKOUT_STANDBY:
						SSME_DATA_TABLE->STATUS = CHECKOUT_STANDBY;
						break;
					case STARTPREPARATION_PURGESEQUENCE4:
						PSN4time = fSimT;// temp
						SSME_DATA_TABLE->STATUS = STARTPREPARATION_PURGESEQUENCE4;
						break;
					case POSTSHUTDOWN_TERMINATESEQUENCE:
						SSME_DATA_TABLE->STATUS = POSTSHUTDOWN_TERMINATESEQUENCE;
						break;
				}
				break;
			case STARTPREPARATION_PURGESEQUENCE4:
				// from T-235 to T-117 ???
				if ((fSimT - PSN4time) > 118)
				{
					cmdSTATUS = STARTPREPARATION_ENGINEREADY;
					SSME_DATA_TABLE->STATUS = STARTPREPARATION_ENGINEREADY;
				}
				switch (cmdSTATUS)
				{
					case CHECKOUT_STANDBY:
						SSME_DATA_TABLE->STATUS = CHECKOUT_STANDBY;
						break;
					case STARTPREPARATION_PURGESEQUENCE3:
						SSME_DATA_TABLE->STATUS = STARTPREPARATION_PURGESEQUENCE3;
						break;
					case POSTSHUTDOWN_TERMINATESEQUENCE:
						SSME_DATA_TABLE->STATUS = POSTSHUTDOWN_TERMINATESEQUENCE;
						break;
				}
				break;
			case STARTPREPARATION_ENGINEREADY:
				SSME_DATA_TABLE->posCCV = 1;// TODO put vlv motion
				// TODO close bleed vlvs
				switch (cmdSTATUS)
				{
					case CHECKOUT_STANDBY:
						SSME_DATA_TABLE->STATUS = CHECKOUT_STANDBY;
						break;
					case STARTPREPARATION_PURGESEQUENCE4:
						PSN4time = fSimT;// temp
						SSME_DATA_TABLE->STATUS = STARTPREPARATION_PURGESEQUENCE4;
						break;
					case STARTMAINSTAGE_STARTPHASESTARTINITIATION:
						SSME_DATA_TABLE->cmdPC = 100;
						SSME_DATA_TABLE->timeESC = fSimT;
						SSME_DATA_TABLE->STATUS = STARTMAINSTAGE_STARTPHASESTARTINITIATION;
						break;
					case POSTSHUTDOWN_TERMINATESEQUENCE:
						SSME_DATA_TABLE->STATUS = POSTSHUTDOWN_TERMINATESEQUENCE;
						break;
				}
				break;
			// START/MAINSTAGE
			case STARTMAINSTAGE_STARTPHASESTARTINITIATION:
				Ignition( fSimT );
				if ((fSimT - SSME_DATA_TABLE->timeESC) > 2.4)
				{
					cmdSTATUS = STARTMAINSTAGE_STARTPHASETHRUSTBUILDUP;
					SSME_DATA_TABLE->STATUS = STARTMAINSTAGE_STARTPHASETHRUSTBUILDUP;
				}
				switch (cmdSTATUS)
				{
					case SHUTDOWN_THROTTLINGTOZEROTHRUST:
						SSME_DATA_TABLE->cmdPC = 0;
						SetCOTime();
						OPOV_pos_CO = SSME_DATA_TABLE->posOPOV * 100;
						FPOV_pos_CO = SSME_DATA_TABLE->posFPOV * 100;
						SSME_DATA_TABLE->timeCO = fSimT;
						SSME_DATA_TABLE->STATUS = SHUTDOWN_THROTTLINGTOZEROTHRUST;
						break;
				}
				break;
			case STARTMAINSTAGE_STARTPHASETHRUSTBUILDUP:
				Ignition( fSimT );
				switch (cmdSTATUS)
				{
					case SHUTDOWN_THROTTLINGTOZEROTHRUST:
						SSME_DATA_TABLE->cmdPC = 0;
						SetCOTime();
						OPOV_pos_CO = SSME_DATA_TABLE->posOPOV * 100;
						FPOV_pos_CO = SSME_DATA_TABLE->posFPOV * 100;
						SSME_DATA_TABLE->timeCO = fSimT;
						SSME_DATA_TABLE->STATUS = SHUTDOWN_THROTTLINGTOZEROTHRUST;
						break;
				}
				break;
			case STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL:// enter at ESC+3.8 ????
				if (SSME_DATA_TABLE->cmdPC != SSME_DATA_TABLE->powerlevel)// use diff instead???
				{
					Throttling( fDeltaT );
				}
				else
				{
					// keep same pc as before
					OV->SetThrusterLevel( thSSME, PCfromSTStoOSFS( SSME_DATA_TABLE->powerlevel ) );
				}
				switch (cmdSTATUS)
				{
					case SHUTDOWN_THROTTLINGTOZEROTHRUST:
						SSME_DATA_TABLE->cmdPC = 0;
						SetCOTime();
						OPOV_pos_CO = SSME_DATA_TABLE->posOPOV * 100;
						FPOV_pos_CO = SSME_DATA_TABLE->posFPOV * 100;
						SSME_DATA_TABLE->timeCO = fSimT;
						SSME_DATA_TABLE->STATUS = SHUTDOWN_THROTTLINGTOZEROTHRUST;
						break;
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
				Shutdown( fSimT );
				break;
			case SHUTDOWN_PROPELLANTVALVESCLOSED:
				// keep running down TPs...
				if ((fSimT - SSME_DATA_TABLE->timeCO) > 7)// ???time???
				{
					cmdSTATUS = POSTSHUTDOWN_STANDBY;
					SSME_DATA_TABLE->STATUS = POSTSHUTDOWN_STANDBY;
				}
				break;
			case SHUTDOWN_FAILSAFEPNEUMATIC:
				break;
			// POST SHUTDOWN
			case POSTSHUTDOWN_STANDBY:
				switch (cmdSTATUS)
				{
					case POSTSHUTDOWN_OXIDIZERDUMP:
						SSME_DATA_TABLE->STATUS = POSTSHUTDOWN_OXIDIZERDUMP;
						break;
				}
				break;
			case POSTSHUTDOWN_OXIDIZERDUMP:
				SSME_DATA_TABLE->posMOV = 1;// TODO put vlv motion
				switch (cmdSTATUS)
				{
					case POSTSHUTDOWN_TERMINATESEQUENCE:
						SSME_DATA_TABLE->STATUS = POSTSHUTDOWN_TERMINATESEQUENCE;
						break;
				}
				break;
			case POSTSHUTDOWN_TERMINATESEQUENCE:
				SSME_DATA_TABLE->posMOV = 0;// TODO put vlv motion
				SSME_DATA_TABLE->posCCV = 0;// TODO put vlv motion
				SSME_DATA_TABLE->posMFV = 0;// TODO put vlv motion
				SSME_DATA_TABLE->posOPOV = 0;// TODO put vlv motion
				SSME_DATA_TABLE->posFPOV = 0;// TODO put vlv motion
				SSME_DATA_TABLE->STATUS = POSTSHUTDOWN_STANDBY;
				cmdSTATUS = POSTSHUTDOWN_STANDBY;
				break;
			// PROM
			case PROM_STANDBY:
				break;
		}
		// update table time tag
		SSME_DATA_TABLE->timeTAG = fSimT;
		return;
	}

	// EIU cmd

	bool SSME::cmdPurgeSequence1( void )
	{
		switch (SSME_DATA_TABLE->STATUS)
		{
			case CHECKOUT_STANDBY:
			case STARTPREPARATION_PURGESEQUENCE2:
				cmdSTATUS = STARTPREPARATION_PURGESEQUENCE1;
				return true;
		}
		return false;
	}

	bool SSME::cmdPurgeSequence2( void )
	{
		if (SSME_DATA_TABLE->STATUS == STARTPREPARATION_PURGESEQUENCE1)
		{
			cmdSTATUS = STARTPREPARATION_PURGESEQUENCE2;
			return true;
		}
		return false;
	}

	bool SSME::cmdPurgeSequence3( void )
	{
		switch (SSME_DATA_TABLE->STATUS)
		{
			case STARTPREPARATION_PURGESEQUENCE2:
			case STARTPREPARATION_PURGESEQUENCE4:
				cmdSTATUS = STARTPREPARATION_PURGESEQUENCE3;
				return true;
		}
		return false;
	}

	bool SSME::cmdPurgeSequence4( void )
	{
		switch (SSME_DATA_TABLE->STATUS)
		{
			case STARTPREPARATION_PURGESEQUENCE3:
			case STARTPREPARATION_ENGINEREADY:
				cmdSTATUS = STARTPREPARATION_PURGESEQUENCE4;
				return true;
		}
		return false;
	}

	bool SSME::cmdStart( void )
	{
		if ((SSME_DATA_TABLE->STATUS == STARTPREPARATION_ENGINEREADY) && (SSME_DATA_TABLE->StartEnable == true))
		{
			cmdSTATUS = STARTMAINSTAGE_STARTPHASESTARTINITIATION;
			return true;
		}
		return false;
	}

	/*bool SSME::cmdThrottle( double itgtPC )
	{
		if ((SSME_DATA_TABLE->STATUS == STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL) && (itgtPC <= FPL) && (itgtPC >= MPL))
		{
			SSME_DATA_TABLE->cmdPC = itgtPC;
			return true;
		}
		return false;
	}*/

	bool SSME::cmdShutdown( void )
	{
		if (SSME_DATA_TABLE->ShutdownEnable == false) return false;

		switch (SSME_DATA_TABLE->STATUS)
		{
			case STARTMAINSTAGE_STARTPHASESTARTINITIATION:
			case STARTMAINSTAGE_STARTPHASETHRUSTBUILDUP:
			case STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL:
				cmdSTATUS = SHUTDOWN_THROTTLINGTOZEROTHRUST;
				return true;
			default:
				return false;
		}
	}


	bool SSME::cmdStartEnable( void )
	{
		SSME_DATA_TABLE->StartEnable = true;
		return true;
	}


	bool SSME::cmdShutdownEnable( void )
	{
		SSME_DATA_TABLE->ShutdownEnable = true;
		return true;
	}

	bool SSME::cmdOxidizerDump( void )
	{
		if (SSME_DATA_TABLE->STATUS == POSTSHUTDOWN_STANDBY)
		{
			cmdSTATUS = POSTSHUTDOWN_OXIDIZERDUMP;
			return true;
		}
		return false;
	}

	bool SSME::cmdTerminateSequence( void )
	{
		cmdSTATUS = POSTSHUTDOWN_TERMINATESEQUENCE;
		return true;
	}


	int SSME::dataDataTable( DATA_TABLE* ptrDataTable )
	{
		memcpy( ptrDataTable, SSME_DATA_TABLE, sizeof(DATA_TABLE) );
		return 0;
	}

// PRIVATE

	void SSME::Ignition( double time )
	{
		double pc = dcPC_ESC( time - SSME_DATA_TABLE->timeESC );
		SSME_DATA_TABLE->posOPOV = dcOPOV_ESC( time - SSME_DATA_TABLE->timeESC );
		SSME_DATA_TABLE->posFPOV = dcFPOV_ESC( time - SSME_DATA_TABLE->timeESC );
		SSME_DATA_TABLE->posMOV = dcMOV_ESC( time - SSME_DATA_TABLE->timeESC );
		SSME_DATA_TABLE->posMFV = dcMFV_ESC( time - SSME_DATA_TABLE->timeESC );
		SSME_DATA_TABLE->posCCV = dcCCV_ESC( time - SSME_DATA_TABLE->timeESC );

		if (pc == 100) SSME_DATA_TABLE->STATUS = STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL;
		OV->SetThrusterLevel( thSSME, PCfromSTStoOSFS( pc ) );
		SSME_DATA_TABLE->powerlevel = pc;
		return;
	}

	void SSME::Shutdown( double time )
	{
		double pc = dcPC_CO( time - SSME_DATA_TABLE->timeCO + COtimecoef );
		SSME_DATA_TABLE->posOPOV = dcOPOV_CO( time - SSME_DATA_TABLE->timeCO, OPOV_pos_CO );
		SSME_DATA_TABLE->posFPOV = dcFPOV_CO( time - SSME_DATA_TABLE->timeCO, FPOV_pos_CO );
		SSME_DATA_TABLE->posMOV = dcMOV_CO( time - SSME_DATA_TABLE->timeCO );
		SSME_DATA_TABLE->posMFV = dcMFV_CO( time - SSME_DATA_TABLE->timeCO );
		SSME_DATA_TABLE->posCCV = dcCCV_CO( time - SSME_DATA_TABLE->timeCO + COtimecoef );

		if (pc == 0) SSME_DATA_TABLE->STATUS = SHUTDOWN_PROPELLANTVALVESCLOSED;
		OV->SetThrusterLevel( thSSME, PCfromSTStoOSFS( pc ) );
		SSME_DATA_TABLE->powerlevel = pc;
		return;
	}

	void SSME::Throttling( double dtime )
	{
		double pc = dcPC_MS( dtime );

		SSME_DATA_TABLE->posOPOV = dcOPOV_MS( pc );
		SSME_DATA_TABLE->posFPOV = dcFPOV_MS( pc );
		//SSME_DATA_TABLE->posMOV = 1;// redundant
		//SSME_DATA_TABLE->posMFV = 1;// redundant
		SSME_DATA_TABLE->posCCV = dcCCV_MS( pc );

		OV->SetThrusterLevel( thSSME, PCfromSTStoOSFS( pc ) );
		SSME_DATA_TABLE->powerlevel = pc;
		return;
	}

	void SSME::SetCOTime( void )
	{
		// pc > 100% -> < 0
		// pc = 100% -> = 0
		// pc < 100% -> > 0
		if (SSME_DATA_TABLE->powerlevel >= 39)
		{
			COtimecoef = (100 - SSME_DATA_TABLE->powerlevel) / 122;
		}
		else
		{
			if (SSME_DATA_TABLE->powerlevel >= 8)
			{
				COtimecoef = ((50.92 - SSME_DATA_TABLE->powerlevel) * 2.6) / 62;
			}
			else
			{
				COtimecoef = ((11.43 - SSME_DATA_TABLE->powerlevel) * 4.2) / 8;
			}
		}
		return;
	}
}