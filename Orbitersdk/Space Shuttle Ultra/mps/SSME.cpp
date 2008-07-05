#include "SSME.h"
#include "../Atlantis.h"


namespace mps
{
	SSME::SSME( SubsystemDirector* _director, const string& _ident, 
		int nID /*, PROPELLANT_HANDLE _phET, 
		const VECTOR3& _pos, const VECTOR3& _dir, 
		double ISP0, double ISP1, double FPL_THRUST*/ )
		:AtlantisSubsystem( _director, _ident )
		/*,
		pos(_pos), dir(_dir), phET(_phET), fFPL_THRUST(FPL_THRUST), 
		fISP0(ISP0), fISP1(ISP1)*/
	{
		ID = nID;
		//OV = Vessel;
		thSSME = NULL;

		dir = _V(0.0, 0.0, 1.0);

		switch(ID)
		{
		case 0:
		case 1:
			pos = SSMEL_REF;
			break;
		case 2: 
			pos = SSMET_REF;
			break;
		case 3:
			pos = SSMER_REF;
		}
		
		activeDCU = DCU_A;

		SSME_DATA_TABLE = new DATA_TABLE;

		return;
	}

	SSME::~SSME( void )
	{
		delete SSME_DATA_TABLE;
		// the end
	}

	void SSME::Realize()
	{
		thSSME = STS()->CreateThruster( pos, dir, FPL_THRUST, phET, ISP0, ISP1);
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
				break;
			case STARTPREPARATION_PURGESEQUENCE2:
				break;
			case STARTPREPARATION_PURGESEQUENCE3:
				break;
			case STARTPREPARATION_PURGESEQUENCE4:
				if (PSN4time == -1)// first run, setup
				{
					PSN4time = fSimT;
					break;
				}
				// from T-235 to T-117 ???
				if ((fSimT - PSN4time) > 118) SSME_DATA_TABLE->STATUS = STARTPREPARATION_ENGINEREADY;
				break;
			case STARTPREPARATION_ENGINEREADY:
				SSME_DATA_TABLE->posCCV = 1;// TODO put vlv motion
				// TODO close bleed vlvs
				break;
			// START/MAINSTAGE
			case STARTMAINSTAGE_STARTPHASESTARTINITIATION:
				if (SSME_DATA_TABLE->timeESC == -1)// first run, setup
				{
					SSME_DATA_TABLE->cmdPC = 100;
					SSME_DATA_TABLE->timeESC = fSimT;
					break;
				}
				Ignition( fSimT );
				if ((fSimT - SSME_DATA_TABLE->timeESC) > 2.4) SSME_DATA_TABLE->STATUS = STARTMAINSTAGE_STARTPHASETHRUSTBUILDUP;
				break;
			case STARTMAINSTAGE_STARTPHASETHRUSTBUILDUP:
				Ignition( fSimT );
				if (SSME_DATA_TABLE->powerlevel == 100) SSME_DATA_TABLE->STATUS = STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL;
				break;
			case STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL:
				// enter at ESC+3.8 ????
				if (SSME_DATA_TABLE->cmdPC != SSME_DATA_TABLE->powerlevel)// use diff instead???
				{
					Throttling( fDeltaT );
				}
				else
				{
					// keep same pc as before
					STS()->SetThrusterLevel( thSSME, PCfromSTStoOSFS( SSME_DATA_TABLE->powerlevel ) );
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
				if (SSME_DATA_TABLE->timeCO == -1)// first run, setup
				{
					SSME_DATA_TABLE->cmdPC = 0;
					SetCOTime();
					OPOV_pos_CO = SSME_DATA_TABLE->posOPOV * 100;
					FPOV_pos_CO = SSME_DATA_TABLE->posFPOV * 100;
					SSME_DATA_TABLE->timeCO = fSimT;
					break;
				}
				Shutdown( fSimT );
				// TODO check when all vlvs are closed
				if (SSME_DATA_TABLE->powerlevel == 0) SSME_DATA_TABLE->STATUS = SHUTDOWN_PROPELLANTVALVESCLOSED;
				break;
			case SHUTDOWN_PROPELLANTVALVESCLOSED:
				// TODO keep running down TPs...
				if ((fSimT - SSME_DATA_TABLE->timeCO) > 7)// ???time???
				{
					SSME_DATA_TABLE->STATUS = POSTSHUTDOWN_STANDBY;
				}
				break;
			case SHUTDOWN_FAILSAFEPNEUMATIC:
				// TODO kill the engine
				SSME_DATA_TABLE->STATUS = POSTSHUTDOWN_STANDBY;
				break;
			// POST SHUTDOWN
			case POSTSHUTDOWN_STANDBY:
				break;
			case POSTSHUTDOWN_OXIDIZERDUMP:
				SSME_DATA_TABLE->posMOV = 1;// TODO put vlv motion
				break;
			case POSTSHUTDOWN_TERMINATESEQUENCE:
				SSME_DATA_TABLE->posMOV = 0;// TODO put vlv motion
				SSME_DATA_TABLE->posCCV = 0;// TODO put vlv motion
				SSME_DATA_TABLE->posMFV = 0;// TODO put vlv motion
				SSME_DATA_TABLE->posOPOV = 0;// TODO put vlv motion
				SSME_DATA_TABLE->posFPOV = 0;// TODO put vlv motion
				SSME_DATA_TABLE->STATUS = POSTSHUTDOWN_STANDBY;
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
				SSME_DATA_TABLE->STATUS = STARTPREPARATION_PURGESEQUENCE1;
				return true;
		}
		return false;
	}

	bool SSME::cmdPurgeSequence2( void )
	{
		if (SSME_DATA_TABLE->STATUS == STARTPREPARATION_PURGESEQUENCE1)
		{
			SSME_DATA_TABLE->STATUS = STARTPREPARATION_PURGESEQUENCE2;
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
				SSME_DATA_TABLE->STATUS = STARTPREPARATION_PURGESEQUENCE3;
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
				PSN4time = -1;
				SSME_DATA_TABLE->STATUS = STARTPREPARATION_PURGESEQUENCE4;
				return true;
		}
		return false;
	}

	bool SSME::cmdStartEnable( void )// TODO checks like in bool SSME::cmdStart( void )
	{
		SSME_DATA_TABLE->StartEnable = true;
		return true;
	}

	bool SSME::cmdStart( void )
	{
		if ((SSME_DATA_TABLE->STATUS == STARTPREPARATION_ENGINEREADY) && (SSME_DATA_TABLE->StartEnable == true))
		{
			SSME_DATA_TABLE->STATUS = STARTMAINSTAGE_STARTPHASESTARTINITIATION;
			return true;
		}
		return false;
	}

	bool SSME::cmdControllerReset( void )
	{
		switch (SSME_DATA_TABLE->STATUS)
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
				SSME_DATA_TABLE->STATUS = CHECKOUT_STANDBY;
				return true;
			default:
				return false;
		}
		return true;
	}

	bool SSME::cmdCheckoutStandby( void )
	{
		switch (SSME_DATA_TABLE->STATUS)
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
				SSME_DATA_TABLE->STATUS = CHECKOUT_STANDBY;
				return true;
			default:
				return false;
		}
		return true;
	}

	bool SSME::cmdTerminateSequence( void )
	{
		switch (SSME_DATA_TABLE->STATUS)
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
				SSME_DATA_TABLE->STATUS = POSTSHUTDOWN_TERMINATESEQUENCE;
				return true;
			default:
				return false;
		}
		return true;
	}

	bool SSME::cmdShutdownEnable( void )// TODO checks like in bool SSME::cmdShutdown( void )
	{
		SSME_DATA_TABLE->ShutdownEnable = true;
		return true;
	}

	bool SSME::cmdShutdown( void )
	{
		if (SSME_DATA_TABLE->ShutdownEnable == false) return false;

		switch (SSME_DATA_TABLE->STATUS)
		{
			case STARTPREPARATION_PURGESEQUENCE1:
			case STARTPREPARATION_PURGESEQUENCE2:
			case STARTPREPARATION_PURGESEQUENCE3:
			case STARTPREPARATION_PURGESEQUENCE4:
			case STARTPREPARATION_ENGINEREADY:
			case STARTMAINSTAGE_STARTPHASESTARTINITIATION:
			case STARTMAINSTAGE_THRUSTLIMITING:
			case STARTMAINSTAGE_HYDRAULICLOCKUP:
				SSME_DATA_TABLE->STATUS = SHUTDOWN_FAILSAFEPNEUMATIC;
				return true;
			case STARTMAINSTAGE_STARTPHASETHRUSTBUILDUP:
			case STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL:
			case STARTMAINSTAGE_FIXEDDENSITY:
				SSME_DATA_TABLE->STATUS = SHUTDOWN_THROTTLINGTOZEROTHRUST;
				return true;
			default:
				return false;
		}
	}

	bool SSME::cmdFRT1( void )
	{
		switch (SSME_DATA_TABLE->STATUS)
		{
			case STARTMAINSTAGE_STARTPHASESTARTINITIATION:
			case STARTMAINSTAGE_STARTPHASETHRUSTBUILDUP:
			case STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL:
			case STARTMAINSTAGE_FIXEDDENSITY:
			case STARTMAINSTAGE_THRUSTLIMITING:
			case STARTMAINSTAGE_ELECTRICALLOCKUP:
			case STARTMAINSTAGE_HYDRAULICLOCKUP:
				SSME_DATA_TABLE->STATUS = SHUTDOWN_FAILSAFEPNEUMATIC;
				return true;
			default:
				return false;
		}
	}

	bool SSME::cmdOxidizerDump( void )
	{
		if (SSME_DATA_TABLE->STATUS == POSTSHUTDOWN_STANDBY)
		{
			SSME_DATA_TABLE->STATUS = POSTSHUTDOWN_OXIDIZERDUMP;
			return true;
		}
		return false;
	}

	bool SSME::cmdExitPROM( void )
	{
		if (SSME_DATA_TABLE->STATUS == PROM_STANDBY)
		{
			SSME_DATA_TABLE->STATUS = POSTSHUTDOWN_STANDBY;
			return true;
		}
		return false;
	}

	bool SSME::cmdPowerOn( void )// TODO check if dead...
	{
		SSME_DATA_TABLE->STATUS = PROM_STANDBY;
		return true;
	}

	bool SSME::cmdChannelReset( void )
	{
		SSME_DATA_TABLE->STATUS = PROM_STANDBY;
		return true;
	}

	bool SSME::cmdThrottle( double itgtPC )
	{
		if ((SSME_DATA_TABLE->STATUS == STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL) && (itgtPC <= FPL) && (itgtPC >= MPL))
		{
			SSME_DATA_TABLE->cmdPC = itgtPC;
			return true;
		}
		return false;
	}

	bool SSME::cmdPowerOff( void )
	{
		if (SSME_DATA_TABLE->STATUS == POSTSHUTDOWN_STANDBY)
		{
			// shutdown...
			return true;
		}
		return false;
	}

	//bool cmdDeactivateAllValves( void );


	int SSME::dataDataTable( DATA_TABLE* ptrDataTable )
	{
		memcpy( ptrDataTable, SSME_DATA_TABLE, sizeof(DATA_TABLE) );
		return 0;
	}

	THRUSTER_HANDLE SSME::GetHandle() const
	{
		return thSSME;
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

		STS()->SetThrusterLevel( thSSME, PCfromSTStoOSFS( pc ) );
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

		STS()->SetThrusterLevel( thSSME, PCfromSTStoOSFS( pc ) );
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

		STS()->SetThrusterLevel( thSSME, PCfromSTStoOSFS( pc ) );
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