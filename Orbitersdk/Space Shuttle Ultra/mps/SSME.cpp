#include "SSME.h"

namespace mps
{
	SSME::SSME( SubsystemDirector* _director, VESSEL* Vessel, PROPELLANT_HANDLE phET ):AtlantisSubsystem( _director, "mpsSSME" )
	{
		OV = Vessel;

		SSME_DATA_TABLE = new DATA_TABLE[3];

		/////////////////// SSME 1 ///////////////////
		MPL[0] = 67;
		FPL[0] = 109;
		model[0] = BLOCK_II;
		thSSME[0] = OV->CreateThruster( OFS_LAUNCH_ORBITER + SSME1_POS, SSME1_DIR, FPL_THRUST, phET, ISP0, ISP1 );
		activeDCU[0] = DCU_A;
		cmdSTATUS[0] = STARTPREPARATION_PURGESEQUENCE3;

		SSME_DATA_TABLE[0].cmdPC = 0;
		SSME_DATA_TABLE[0].StartEnable = false;
		SSME_DATA_TABLE[0].ShutdownEnable = false;
		SSME_DATA_TABLE[0].powerlevel = 0;
		SSME_DATA_TABLE[0].timeESC = -1;
		SSME_DATA_TABLE[0].timeCO = -1;
		SSME_DATA_TABLE[0].STATUS = STARTPREPARATION_PURGESEQUENCE3;
		SSME_DATA_TABLE[0].posOPOV = 0;
		SSME_DATA_TABLE[0].posFPOV = 0;
		SSME_DATA_TABLE[0].posMFV = 0;
		SSME_DATA_TABLE[0].posMOV = 0;
		SSME_DATA_TABLE[0].posCCV = 0;
		/*SSME_DATA_TABLE[0].MR = 0;
		SSME_DATA_TABLE[0].rpmLPFTP = 0;
		SSME_DATA_TABLE[0].rpmHPFTP = 0;
		SSME_DATA_TABLE[0].rpmLPOTP = 0;
		SSME_DATA_TABLE[0].rpmHPOTP = 0;
		SSME_DATA_TABLE[0].tempHPFTdis_chA = 520;// ????
		SSME_DATA_TABLE[0].tempHPOTdis_chA = 520;// ????
		SSME_DATA_TABLE[0].pressHPOTPintseal = 0;// ????
		SSME_DATA_TABLE[0].pressHPOTPsecseal = 0;// ????*/


		/////////////////// SSME 2 ///////////////////
		MPL[1] = 67;
		FPL[1] = 109;
		model[1] = BLOCK_II;
		thSSME[1] = OV->CreateThruster( OFS_LAUNCH_ORBITER + SSME2_POS, SSME2_DIR, FPL_THRUST, phET, ISP0, ISP1 );
		activeDCU[1] = DCU_A;
		cmdSTATUS[1] = STARTPREPARATION_PURGESEQUENCE3;

		SSME_DATA_TABLE[1].cmdPC = 0;
		SSME_DATA_TABLE[1].StartEnable = false;
		SSME_DATA_TABLE[1].ShutdownEnable = false;
		SSME_DATA_TABLE[1].powerlevel = 0;
		SSME_DATA_TABLE[1].timeESC = -1;
		SSME_DATA_TABLE[1].timeCO = -1;
		SSME_DATA_TABLE[1].STATUS = STARTPREPARATION_PURGESEQUENCE3;
		SSME_DATA_TABLE[1].posOPOV = 0;
		SSME_DATA_TABLE[1].posFPOV = 0;
		SSME_DATA_TABLE[1].posMFV = 0;
		SSME_DATA_TABLE[1].posMOV = 0;
		SSME_DATA_TABLE[1].posCCV = 0;
		/*SSME_DATA_TABLE[1].MR = 0;
		SSME_DATA_TABLE[1].rpmLPFTP = 0;
		SSME_DATA_TABLE[1].rpmHPFTP = 0;
		SSME_DATA_TABLE[1].rpmLPOTP = 0;
		SSME_DATA_TABLE[1].rpmHPOTP = 0;
		SSME_DATA_TABLE[1].tempHPFTdis_chA = 520;// ????
		SSME_DATA_TABLE[1].tempHPOTdis_chA = 520;// ????
		SSME_DATA_TABLE[1].pressHPOTPintseal = 0;// ????
		SSME_DATA_TABLE[1].pressHPOTPsecseal = 0;// ????*/


		/////////////////// SSME 3 ///////////////////
		MPL[2] = 67;
		FPL[2] = 109;
		model[2] = BLOCK_II;
		thSSME[2] = OV->CreateThruster( OFS_LAUNCH_ORBITER + SSME3_POS, SSME3_DIR, FPL_THRUST, phET, ISP0, ISP1 );
		activeDCU[2] = DCU_A;
		cmdSTATUS[2] = STARTPREPARATION_PURGESEQUENCE3;

		SSME_DATA_TABLE[2].cmdPC = 0;
		SSME_DATA_TABLE[2].StartEnable = false;
		SSME_DATA_TABLE[2].ShutdownEnable = false;
		SSME_DATA_TABLE[2].powerlevel = 0;
		SSME_DATA_TABLE[2].timeESC = -1;
		SSME_DATA_TABLE[2].timeCO = -1;
		SSME_DATA_TABLE[2].STATUS = STARTPREPARATION_PURGESEQUENCE3;
		SSME_DATA_TABLE[2].posOPOV = 0;
		SSME_DATA_TABLE[2].posFPOV = 0;
		SSME_DATA_TABLE[2].posMFV = 0;
		SSME_DATA_TABLE[2].posMOV = 0;
		SSME_DATA_TABLE[2].posCCV = 0;
		/*SSME_DATA_TABLE[2].MR = 0;
		SSME_DATA_TABLE[2].rpmLPFTP = 0;
		SSME_DATA_TABLE[2].rpmHPFTP = 0;
		SSME_DATA_TABLE[2].rpmLPOTP = 0;
		SSME_DATA_TABLE[2].rpmHPOTP = 0;
		SSME_DATA_TABLE[2].tempHPFTdis_chA = 520;// ????
		SSME_DATA_TABLE[2].tempHPOTdis_chA = 520;// ????
		SSME_DATA_TABLE[2].pressHPOTPintseal = 0;// ????
		SSME_DATA_TABLE[2].pressHPOTPsecseal = 0;// ????*/
		return;
	}

	SSME::~SSME( void )
	{
		delete SSME_DATA_TABLE;
		// the end
	}

	bool SSME::OnParseLine( const char* line )// TODO val checks
	{
		int idata1 = 0;
		int idata2 = 0;
		float fdata1 = 0;
		float fdata2 = 0;
		float fdata3 = 0;
		float fdata4 = 0;
		float fdata5 = 0;

		if (!_strnicmp( line, "SSME_1sw", 8 ))
		{
			sscanf( line + 8, "%d %d", &idata1, &idata2 );
			SSME_DATA_TABLE[0].STATUS = (ENGINE_STATUS)idata1;
			cmdSTATUS[0] = (ENGINE_STATUS)idata2;
			return true;
		}
		else if (!_strnicmp( line, "SSME_2sw", 8 ))
		{
			sscanf( line + 8, "%d %d", &idata1, &idata2 );
			SSME_DATA_TABLE[1].STATUS = (ENGINE_STATUS)idata1;
			cmdSTATUS[1] = (ENGINE_STATUS)idata2;
			return true;
		}
		else if (!_strnicmp( line, "SSME_3sw", 8 ))
		{
			sscanf( line + 8, "%d %d", &idata1, &idata2 );
			SSME_DATA_TABLE[2].STATUS = (ENGINE_STATUS)idata1;
			cmdSTATUS[2] = (ENGINE_STATUS)idata2;
			return true;
		}
		else if (!_strnicmp( line, "SSME_1pl", 8 ))
		{
			sscanf( line + 8, "%f", &fdata1 );
			SSME_DATA_TABLE[0].powerlevel = fdata1;
			return true;
		}
		else if (!_strnicmp( line, "SSME_2pl", 8 ))
		{
			sscanf( line + 8, "%f", &fdata1 );
			SSME_DATA_TABLE[1].powerlevel = fdata1;
			return true;
		}
		else if (!_strnicmp( line, "SSME_3pl", 8 ))
		{
			sscanf( line + 8, "%f", &fdata1 );
			SSME_DATA_TABLE[2].powerlevel = fdata1;
			return true;
		}
		else if (!_strnicmp( line, "SSME_1ShE", 8 ))
		{
			sscanf( line + 8, "%d", &idata1 );
			if (idata1 == 1) SSME_DATA_TABLE[0].ShutdownEnable = true;
			return true;
		}
		else if (!_strnicmp( line, "SSME_2ShE", 8 ))
		{
			sscanf( line + 8, "%d", &idata1 );
			if (idata1 == 1) SSME_DATA_TABLE[1].ShutdownEnable = true;
			return true;
		}
		else if (!_strnicmp( line, "SSME_3ShE", 8 ))
		{
			sscanf( line + 8, "%d", &idata1 );
			if (idata1 == 1) SSME_DATA_TABLE[2].ShutdownEnable = true;
			return true;
		}
		else if (!_strnicmp( line, "SSME_1StE", 8 ))
		{
			sscanf( line + 8, "%d", &idata1 );
			if (idata1 == 1) SSME_DATA_TABLE[0].StartEnable = true;
			return true;
		}
		else if (!_strnicmp( line, "SSME_2StE", 8 ))
		{
			sscanf( line + 8, "%d", &idata1 );
			if (idata1 == 1) SSME_DATA_TABLE[1].StartEnable = true;
			return true;
		}
		else if (!_strnicmp( line, "SSME_3StE", 8 ))
		{
			sscanf( line + 8, "%d", &idata1 );
			if (idata1 == 1) SSME_DATA_TABLE[2].StartEnable = true;
			return true;
		}
		else if (!_strnicmp( line, "SSME_1DCU", 9 ))
		{
			sscanf( line + 9, "%d", &idata1 );
			activeDCU[0] = (DCU)idata1;
			return true;
		}
		else if (!_strnicmp( line, "SSME_2DCU", 9 ))
		{
			sscanf( line + 9, "%d", &idata1 );
			activeDCU[1] = (DCU)idata1;
			return true;
		}
		else if (!_strnicmp( line, "SSME_3DCU", 9 ))
		{
			sscanf( line + 9, "%d", &idata1 );
			activeDCU[2] = (DCU)idata1;
			return true;
		}
		else if (!_strnicmp( line, "SSME_1tme", 9 ))
		{
			sscanf( line + 9, "%f %f", &fdata1, &fdata2 );
			SSME_DATA_TABLE[0].timeESC = fdata1;
			SSME_DATA_TABLE[0].timeCO = fdata2;
			return true;
		}
		else if (!_strnicmp( line, "SSME_2tme", 9 ))
		{
			sscanf( line + 9, "%f %f", &fdata1, &fdata2 );
			SSME_DATA_TABLE[1].timeESC = fdata1;
			SSME_DATA_TABLE[1].timeCO = fdata2;
			return true;
		}
		else if (!_strnicmp( line, "SSME_3tme", 9 ))
		{
			sscanf( line + 9, "%f %f", &fdata1, &fdata2 );
			SSME_DATA_TABLE[2].timeESC = fdata1;
			SSME_DATA_TABLE[2].timeCO = fdata2;
			return true;
		}
		else if (!_strnicmp( line, "SSME_1pos", 9 ))
		{
			sscanf( line + 9, "%f %f %f %f %f", &fdata1, &fdata2, &fdata3, &fdata4, &fdata5 );
			SSME_DATA_TABLE[0].posOPOV = fdata1;
			SSME_DATA_TABLE[0].posFPOV = fdata2;
			SSME_DATA_TABLE[0].posMFV = fdata3;
			SSME_DATA_TABLE[0].posMOV = fdata4;
			SSME_DATA_TABLE[0].posCCV = fdata5;
			return true;
		}
		else if (!_strnicmp( line, "SSME_2pos", 9 ))
		{
			sscanf( line + 9, "%f %f %f %f %f", &fdata1, &fdata2, &fdata3, &fdata4, &fdata5 );
			SSME_DATA_TABLE[1].posOPOV = fdata1;
			SSME_DATA_TABLE[1].posFPOV = fdata2;
			SSME_DATA_TABLE[1].posMFV = fdata3;
			SSME_DATA_TABLE[1].posMOV = fdata4;
			SSME_DATA_TABLE[1].posCCV = fdata5;
			return true;
		}
		else if (!_strnicmp( line, "SSME_3pos", 9 ))
		{
			sscanf( line + 9, "%f %f %f %f %f", &fdata1, &fdata2, &fdata3, &fdata4, &fdata5 );
			SSME_DATA_TABLE[2].posOPOV = fdata1;
			SSME_DATA_TABLE[2].posFPOV = fdata2;
			SSME_DATA_TABLE[2].posMFV = fdata3;
			SSME_DATA_TABLE[2].posMOV = fdata4;
			SSME_DATA_TABLE[2].posCCV = fdata5;
			return true;
		}
		else if (!_strnicmp( line, "SSME_1", 6 ))
		{
			sscanf( line + 6, "%d %f %f", &idata1, &fdata1, &fdata2 );
			model[0] = (ENGINE_MODEL)idata1;
			MPL[0] = fdata1;
			FPL[0] = fdata2;
			return true;
		}
		else if (!_strnicmp( line, "SSME_2", 6 ))
		{
			sscanf( line + 6, "%d %f %f", &idata1, &fdata1, &fdata2 );
			model[1] = (ENGINE_MODEL)idata1;
			MPL[1] = fdata1;
			FPL[1] = fdata2;
			return true;
		}
		else if (!_strnicmp( line, "SSME_3", 6 ))
		{
			sscanf( line + 6, "%d %f %f", &idata1, &fdata1, &fdata2 );
			model[2] = (ENGINE_MODEL)idata1;
			MPL[2] = fdata1;
			FPL[2] = fdata2;
			return true;
		}
		else if (!_strnicmp( line, "SSME_1tt", 8 ))
		{
			sscanf( line + 8, "%f", &fdata1 );
			SSME_DATA_TABLE[0].timeTAG = fdata1;
			return true;
		}
		else if (!_strnicmp( line, "SSME_2tt", 8 ))
		{
			sscanf( line + 8, "%f", &fdata1 );
			SSME_DATA_TABLE[1].timeTAG = fdata1;
			return true;
		}
		else if (!_strnicmp( line, "SSME_3tt", 8 ))
		{
			sscanf( line + 8, "%f", &fdata1 );
			SSME_DATA_TABLE[2].timeTAG = fdata1;
			return true;
		}
		return false;
	}

	void SSME::OnSaveState(FILEHANDLE scn)
	{
		char sdata[64];
		int count = 0;

		while (count < 3)
		{
			sprintf( sdata, "  SSME_%dtt %f", count + 1, SSME_DATA_TABLE[count].timeTAG );
			oapiWriteLine( scn, sdata );

			sprintf( sdata, "  SSME_%dsw %d %d", count + 1, SSME_DATA_TABLE[count].STATUS, cmdSTATUS[count] );
			oapiWriteLine( scn, sdata );

			sprintf( sdata, "  SSME_%dpl %f", count + 1, SSME_DATA_TABLE[count].powerlevel );
			oapiWriteLine( scn, sdata );

			sprintf( sdata, "  SSME_%dShE %d", count + 1, SSME_DATA_TABLE[count].ShutdownEnable );
			oapiWriteLine( scn, sdata );

			sprintf( sdata, "  SSME_%dStE %d", count + 1, SSME_DATA_TABLE[count].StartEnable );
			oapiWriteLine( scn, sdata );

			sprintf( sdata, "  SSME_%dDCU %d", count + 1, activeDCU[count] );
			oapiWriteLine( scn, sdata );

			sprintf( sdata, "  SSME_%dtme %f %f", count + 1, SSME_DATA_TABLE[count].timeESC, SSME_DATA_TABLE[count].timeCO );
			oapiWriteLine( scn, sdata );

			sprintf( sdata, "  SSME_%dpos %f %f %f %f %f", count + 1, SSME_DATA_TABLE[count].posOPOV, SSME_DATA_TABLE[count].posFPOV, SSME_DATA_TABLE[count].posMFV, SSME_DATA_TABLE[count].posMOV, SSME_DATA_TABLE[count].posCCV );
			oapiWriteLine( scn, sdata );

			sprintf( sdata, "  SSME_%d %d %f %f", count + 1, model[count], MPL[count], FPL[count] );
			oapiWriteLine( scn, sdata );

			count++;
		}
		return;
	}



	/*double SSME::PCfromOSFStoSTS( double pcOSFS, int ME )
	{
		return (pcOSFS * FPL[ME]);
	}*/


	double SSME::PCfromSTStoOSFS( double pcSTS, int ME )
	{
		return (pcSTS / FPL[ME]);
	}

	/*double SSME::PCfromPCTtoPSI( double pcPCT )
	{
		// TODO PCT psi
		return ((pcPCT * 3000) / 109);
	}*/

	void SSME::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		int ME = 0;
		while (ME <= 2)
		{
			switch (SSME_DATA_TABLE[ME].STATUS)
			{
				// CHECKOUT
				case CHECKOUT_STANDBY:
					break;
				case CHECKOUT_COMPONENTCHECKOUT:
					break;
				// START PREPARATION
				case STARTPREPARATION_PURGESEQUENCE1:
					switch (cmdSTATUS[ME])
					{
						case CHECKOUT_STANDBY:
							SSME_DATA_TABLE[ME].STATUS = CHECKOUT_STANDBY;
							break;
						case STARTPREPARATION_PURGESEQUENCE2:
							SSME_DATA_TABLE[ME].STATUS = STARTPREPARATION_PURGESEQUENCE2;
							break;
						case POSTSHUTDOWN_TERMINATESEQUENCE:
							SSME_DATA_TABLE[ME].STATUS = POSTSHUTDOWN_TERMINATESEQUENCE;
							break;
					}
					break;
				case STARTPREPARATION_PURGESEQUENCE2:
					switch (cmdSTATUS[ME])
					{
						case CHECKOUT_STANDBY:
							SSME_DATA_TABLE[ME].STATUS = CHECKOUT_STANDBY;
							break;
						case STARTPREPARATION_PURGESEQUENCE1:
							SSME_DATA_TABLE[ME].STATUS = STARTPREPARATION_PURGESEQUENCE1;
							break;
						case STARTPREPARATION_PURGESEQUENCE3:
							SSME_DATA_TABLE[ME].STATUS = STARTPREPARATION_PURGESEQUENCE3;
							break;
						case POSTSHUTDOWN_TERMINATESEQUENCE:
							SSME_DATA_TABLE[ME].STATUS = POSTSHUTDOWN_TERMINATESEQUENCE;
							break;
					}
					break;
				case STARTPREPARATION_PURGESEQUENCE3:
					switch (cmdSTATUS[ME])
					{
						case CHECKOUT_STANDBY:
							SSME_DATA_TABLE[ME].STATUS = CHECKOUT_STANDBY;
							break;
						case STARTPREPARATION_PURGESEQUENCE4:
							PSN4time[ME] = fSimT;// temp
							SSME_DATA_TABLE[ME].STATUS = STARTPREPARATION_PURGESEQUENCE4;
							break;
						case POSTSHUTDOWN_TERMINATESEQUENCE:
							SSME_DATA_TABLE[ME].STATUS = POSTSHUTDOWN_TERMINATESEQUENCE;
							break;
					}
					break;
				case STARTPREPARATION_PURGESEQUENCE4:
					// from T-235 to T-117 ???
					if ((fSimT - PSN4time[ME]) > 118)
					{
						cmdSTATUS[ME] = STARTPREPARATION_ENGINEREADY;
						SSME_DATA_TABLE[ME].STATUS = STARTPREPARATION_ENGINEREADY;
					}
					switch (cmdSTATUS[ME])
					{
						case CHECKOUT_STANDBY:
							SSME_DATA_TABLE[ME].STATUS = CHECKOUT_STANDBY;
							break;
						case STARTPREPARATION_PURGESEQUENCE3:
							SSME_DATA_TABLE[ME].STATUS = STARTPREPARATION_PURGESEQUENCE3;
							break;
						case POSTSHUTDOWN_TERMINATESEQUENCE:
							SSME_DATA_TABLE[ME].STATUS = POSTSHUTDOWN_TERMINATESEQUENCE;
							break;
					}
					break;
				case STARTPREPARATION_ENGINEREADY:
					SSME_DATA_TABLE[ME].posCCV = 1;// TODO put vlv motion
					// TODO close bleed vlvs
					switch (cmdSTATUS[ME])
					{
						case CHECKOUT_STANDBY:
							SSME_DATA_TABLE[ME].STATUS = CHECKOUT_STANDBY;
							break;
						case STARTPREPARATION_PURGESEQUENCE4:
							PSN4time[ME] = fSimT;// temp
							SSME_DATA_TABLE[ME].STATUS = STARTPREPARATION_PURGESEQUENCE4;
							break;
						case STARTMAINSTAGE_STARTPHASESTARTINITIATION:
							SSME_DATA_TABLE[ME].cmdPC = 100;
							SSME_DATA_TABLE[ME].timeESC = fSimT;
							SSME_DATA_TABLE[ME].STATUS = STARTMAINSTAGE_STARTPHASESTARTINITIATION;
							break;
						case POSTSHUTDOWN_TERMINATESEQUENCE:
							SSME_DATA_TABLE[ME].STATUS = POSTSHUTDOWN_TERMINATESEQUENCE;
							break;
					}
					break;
				// START/MAINSTAGE
				case STARTMAINSTAGE_STARTPHASESTARTINITIATION:
					Ignition( ME, fSimT );
					if ((fSimT - SSME_DATA_TABLE[ME].timeESC) > 2.4)
					{
						cmdSTATUS[ME] = STARTMAINSTAGE_STARTPHASETHRUSTBUILDUP;
						SSME_DATA_TABLE[ME].STATUS = STARTMAINSTAGE_STARTPHASETHRUSTBUILDUP;
					}
					switch (cmdSTATUS[ME])
					{
						case SHUTDOWN_THROTTLINGTOZEROTHRUST:
							SSME_DATA_TABLE[ME].cmdPC = 0;
							SetCOTime( ME );
							OPOV_pos_CO[ME] = SSME_DATA_TABLE[ME].posOPOV * 100;
							FPOV_pos_CO[ME] = SSME_DATA_TABLE[ME].posFPOV * 100;
							SSME_DATA_TABLE[ME].timeCO = fSimT;
							SSME_DATA_TABLE[ME].STATUS = SHUTDOWN_THROTTLINGTOZEROTHRUST;
							break;
					}
					break;
				case STARTMAINSTAGE_STARTPHASETHRUSTBUILDUP:
					Ignition( ME, fSimT );
					switch (cmdSTATUS[ME])
					{
						case SHUTDOWN_THROTTLINGTOZEROTHRUST:
							SSME_DATA_TABLE[ME].cmdPC = 0;
							SetCOTime( ME );
							OPOV_pos_CO[ME] = SSME_DATA_TABLE[ME].posOPOV * 100;
							FPOV_pos_CO[ME] = SSME_DATA_TABLE[ME].posFPOV * 100;
							SSME_DATA_TABLE[ME].timeCO = fSimT;
							SSME_DATA_TABLE[ME].STATUS = SHUTDOWN_THROTTLINGTOZEROTHRUST;
							break;
					}
					break;
				case STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL:// enter at ESC+3.8 ????
					if (SSME_DATA_TABLE[ME].cmdPC != SSME_DATA_TABLE[ME].powerlevel) Throttling( ME, fDeltaT );// use diff instead???
					switch (cmdSTATUS[ME])
					{
						case SHUTDOWN_THROTTLINGTOZEROTHRUST:
							SSME_DATA_TABLE[ME].cmdPC = 0;
							SetCOTime( ME );
							OPOV_pos_CO[ME] = SSME_DATA_TABLE[ME].posOPOV * 100;
							FPOV_pos_CO[ME] = SSME_DATA_TABLE[ME].posFPOV * 100;
							SSME_DATA_TABLE[ME].timeCO = fSimT;
							SSME_DATA_TABLE[ME].STATUS = SHUTDOWN_THROTTLINGTOZEROTHRUST;
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
					Shutdown( ME, fSimT );
					break;
				case SHUTDOWN_PROPELLANTVALVESCLOSED:
					// keep running down TPs...
					if ((fSimT - SSME_DATA_TABLE[ME].timeCO) > 5)// ???time???
					{
						cmdSTATUS[ME] = POSTSHUTDOWN_STANDBY;
						SSME_DATA_TABLE[ME].STATUS = POSTSHUTDOWN_STANDBY;
					}
					break;
				case SHUTDOWN_FAILSAFEPNEUMATIC:
					break;
				// POST SHUTDOWN
				case POSTSHUTDOWN_STANDBY:
					switch (cmdSTATUS[ME])
					{
						case POSTSHUTDOWN_OXIDIZERDUMP:
							SSME_DATA_TABLE[ME].STATUS = POSTSHUTDOWN_OXIDIZERDUMP;
							break;
					}
					break;
				case POSTSHUTDOWN_OXIDIZERDUMP:
					SSME_DATA_TABLE[ME].posMOV = 1;// TODO put vlv motion
					switch (cmdSTATUS[ME])
					{
						case POSTSHUTDOWN_TERMINATESEQUENCE:
							SSME_DATA_TABLE[ME].STATUS = POSTSHUTDOWN_TERMINATESEQUENCE;
							break;
					}
					break;
				case POSTSHUTDOWN_TERMINATESEQUENCE:
					SSME_DATA_TABLE[ME].posMOV = 0;// TODO put vlv motion
					SSME_DATA_TABLE[ME].posCCV = 0;// TODO put vlv motion
					SSME_DATA_TABLE[ME].posMFV = 0;// TODO put vlv motion
					SSME_DATA_TABLE[ME].posOPOV = 0;// TODO put vlv motion
					SSME_DATA_TABLE[ME].posFPOV = 0;// TODO put vlv motion
					SSME_DATA_TABLE[ME].STATUS = POSTSHUTDOWN_STANDBY;// TODO fix cmdSTATUS
					break;
				// PROM
				case PROM_STANDBY:
					break;
			}
			// update table time tag
			SSME_DATA_TABLE[ME].timeTAG = fSimT;
			ME++;
		}
		return;
	}

	// EIU cmd

	bool SSME::cmdPurgeSequence1( int ME )
	{
		switch (SSME_DATA_TABLE[ME].STATUS)
		{
			case CHECKOUT_STANDBY:
			case STARTPREPARATION_PURGESEQUENCE2:
				cmdSTATUS[ME] = STARTPREPARATION_PURGESEQUENCE1;
				return true;
		}
		return false;
	}

	bool SSME::cmdPurgeSequence2( int ME )
	{
		if (SSME_DATA_TABLE[ME].STATUS == STARTPREPARATION_PURGESEQUENCE1)
		{
			cmdSTATUS[ME] = STARTPREPARATION_PURGESEQUENCE2;
			return true;
		}
		return false;
	}

	bool SSME::cmdPurgeSequence3( int ME )
	{
		switch (SSME_DATA_TABLE[ME].STATUS)
		{
			case STARTPREPARATION_PURGESEQUENCE2:
			case STARTPREPARATION_PURGESEQUENCE4:
				cmdSTATUS[ME] = STARTPREPARATION_PURGESEQUENCE3;
				return true;
		}
		return false;
	}

	bool SSME::cmdPurgeSequence4( int ME )
	{
		switch (SSME_DATA_TABLE[ME].STATUS)
		{
			case STARTPREPARATION_PURGESEQUENCE3:
			case STARTPREPARATION_ENGINEREADY:
				cmdSTATUS[ME] = STARTPREPARATION_PURGESEQUENCE4;
				return true;
		}
		return false;
	}

	bool SSME::cmdStart( int ME )
	{
		if ((SSME_DATA_TABLE[ME].STATUS == STARTPREPARATION_ENGINEREADY) && (SSME_DATA_TABLE[ME].StartEnable == true))
		{
			cmdSTATUS[ME] = STARTMAINSTAGE_STARTPHASESTARTINITIATION;
			return true;
		}
		return false;
	}

	bool SSME::cmdThrottle( int ME, double itgtPC )
	{
		if ((SSME_DATA_TABLE[ME].STATUS == STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL) && (itgtPC <= FPL[ME]) && (itgtPC >= MPL[ME]))
		{
			SSME_DATA_TABLE[ME].cmdPC = itgtPC;
			return true;
		}
		return false;
	}

	bool SSME::cmdShutdown( int ME )
	{
		if (SSME_DATA_TABLE[ME].ShutdownEnable == false) return false;

		switch (SSME_DATA_TABLE[ME].STATUS)
		{
			case STARTMAINSTAGE_STARTPHASESTARTINITIATION:
			case STARTMAINSTAGE_STARTPHASETHRUSTBUILDUP:
			case STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL:
				cmdSTATUS[ME] = SHUTDOWN_THROTTLINGTOZEROTHRUST;
				return true;
			default:
				return false;
		}
	}


	bool SSME::cmdStartEnable( int ME )
	{
		SSME_DATA_TABLE[ME].StartEnable = true;
		return true;
	}


	bool SSME::cmdShutdownEnable( int ME )
	{
		SSME_DATA_TABLE[ME].ShutdownEnable = true;
		return true;
	}

	bool SSME::cmdOxidizerDump( int ME )
	{
		if (SSME_DATA_TABLE[ME].STATUS == POSTSHUTDOWN_STANDBY)
		{
			cmdSTATUS[ME] = POSTSHUTDOWN_OXIDIZERDUMP;
			return true;
		}
		return false;
	}

	bool SSME::cmdTerminateSequence( int ME )
	{
		cmdSTATUS[ME] = POSTSHUTDOWN_TERMINATESEQUENCE;
		return true;
	}


	int SSME::dataDataTable( int ME, DATA_TABLE* ptrDataTable )
	{
		memcpy( ptrDataTable, &SSME_DATA_TABLE[ME], sizeof(DATA_TABLE) );
		return 0;
	}

// PRIVATE

	void SSME::Ignition( int ME, double time )
	{
		double pc = dcPC_ESC( time - SSME_DATA_TABLE[ME].timeESC );
		SSME_DATA_TABLE[ME].posOPOV = dcOPOV_ESC( time - SSME_DATA_TABLE[ME].timeESC );
		SSME_DATA_TABLE[ME].posFPOV = dcFPOV_ESC( time - SSME_DATA_TABLE[ME].timeESC );
		SSME_DATA_TABLE[ME].posMOV = dcMOV_ESC( time - SSME_DATA_TABLE[ME].timeESC );
		SSME_DATA_TABLE[ME].posMFV = dcMFV_ESC( time - SSME_DATA_TABLE[ME].timeESC );
		SSME_DATA_TABLE[ME].posCCV = dcCCV_ESC( time - SSME_DATA_TABLE[ME].timeESC );

		if (pc == 100) SSME_DATA_TABLE[ME].STATUS = STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL;
		OV->SetThrusterLevel( thSSME[ME], PCfromSTStoOSFS( pc, ME ) );
		SSME_DATA_TABLE[ME].powerlevel = pc;
		return;
	}

	void SSME::Shutdown( int ME, double time )
	{
		double pc = dcPC_CO( time - SSME_DATA_TABLE[ME].timeCO + COtimecoef[ME] );
		SSME_DATA_TABLE[ME].posOPOV = dcOPOV_CO( time - SSME_DATA_TABLE[ME].timeCO, OPOV_pos_CO[ME] );
		SSME_DATA_TABLE[ME].posFPOV = dcFPOV_CO( time - SSME_DATA_TABLE[ME].timeCO, FPOV_pos_CO[ME] );
		SSME_DATA_TABLE[ME].posMOV = dcMOV_CO( time - SSME_DATA_TABLE[ME].timeCO );
		SSME_DATA_TABLE[ME].posMFV = dcMFV_CO( time - SSME_DATA_TABLE[ME].timeCO );
		SSME_DATA_TABLE[ME].posCCV = dcCCV_CO( time - SSME_DATA_TABLE[ME].timeCO + COtimecoef[ME] );

		if (pc == 0) SSME_DATA_TABLE[ME].STATUS = SHUTDOWN_PROPELLANTVALVESCLOSED;
		OV->SetThrusterLevel( thSSME[ME], PCfromSTStoOSFS( pc, ME ) );
		SSME_DATA_TABLE[ME].powerlevel = pc;
		return;
	}

	void SSME::Throttling( int ME, double dtime )
	{
		double pc = dcPC_MS( dtime, ME );

		// TODO vlv pos
		SSME_DATA_TABLE[ME].posOPOV = dcOPOV_MS( pc );
		SSME_DATA_TABLE[ME].posFPOV = dcFPOV_MS( pc );
		//SSME_DATA_TABLE[ME].posMOV = 1;// redundant
		//SSME_DATA_TABLE[ME].posMFV = 1;// redundant
		SSME_DATA_TABLE[ME].posCCV = dcCCV_MS( pc );

		OV->SetThrusterLevel( thSSME[ME], PCfromSTStoOSFS( pc, ME ) );
		SSME_DATA_TABLE[ME].powerlevel = pc;
		return;
	}

	void SSME::SetCOTime( int ME )
	{
		// pc > 100% -> < 0
		// pc = 100% -> = 0
		// pc < 100% -> > 0
		if (SSME_DATA_TABLE[ME].powerlevel >= 39)
		{
			COtimecoef[ME] = (100 - SSME_DATA_TABLE[ME].powerlevel) / 122;
		}
		else
		{
			if (SSME_DATA_TABLE[ME].powerlevel >= 8)
			{
				COtimecoef[ME] = ((50.92 - SSME_DATA_TABLE[ME].powerlevel) * 2.6) / 62;
			}
			else
			{
				COtimecoef[ME] = ((11.43 - SSME_DATA_TABLE[ME].powerlevel) * 4.2) / 8;
			}
		}
		return;
	}

	// data cookup
	// ESC
	double SSME::dcPC_ESC( double tme )
	{
		double pc;

		if (tme <= 0.95)
		{
			pc = 0;
		}
		else if (tme <= 1.5)
		{
			pc = abs( (5.09 * tme) - 4.84 );// abs here due to double precision prob
		}
		else if (tme <= 1.62)
		{
			pc = (96.67 * tme) - 142.2;
		}
		else if (tme <= 2.14)
		{
			pc = (22.5 * tme) - 22.05;
		}
		else if (tme <= 2.46)
		{
			pc = 26.1;
		}
		else if (tme <= 3.8)
		{
			pc = (49.78 * tme) - 96.35;
		}
		else if (tme <= 5)
		{
			pc = (6 * tme) + 70;
		}
		else
		{
			pc = 100;
		}
		return pc;
	}

	double SSME::dcMOV_ESC( double tme )
	{
		double pos;

		if (tme <= 0.2)
		{
			pos = 0;
		}
		else
		{
			if (tme <= 1.29)
			{
				pos = ((tme * 59) - 11.8) / 1.09;
			}
			else
			{
				if (tme <= 2.49)
				{
					pos = 59;
				}
				else
				{
					if (tme <= 3.94)
					{
						pos = ((tme * 41) - 16.54) / 1.45;
					}
					else
					{
						pos = 100;
					}
				}
			}
		}
		return pos / 100;
	}

	double SSME::dcMFV_ESC( double tme )
	{
		double pos;

		if (tme <= 0.71)
		{
			pos = (100 * tme) / 0.71;
		}
		else
		{
			pos = 100;
		}
		return pos / 100;
	}

	double SSME::dcCCV_ESC( double tme )
	{
		double pos;

		if (tme <= 1.47)
		{
			pos = 100;
		}
		else
		{
			if (tme <= 1.78)
			{
				pos = (75.1 - (30 * tme)) / 0.31;
			}
			else
			{
				if (tme <= 2.4)
				{
					pos = 70;
				}
				else
				{
					if (tme <= 2.85)
					{
						pos = (77.1 - (19 * tme)) / 0.45;
					}
					else
					{
						if (tme <= 3.72)
						{
							pos = ((49 * tme) - 95.28) / 0.87;
						}
						else
						{
							pos = 100;
						}
					}
				}
			}
		}
		return pos / 100;
	}

	double SSME::dcFPOV_ESC( double tme )
	{
		double pos;

		if (tme <= 0.1)
		{
			pos = 0;
		}
		else if (tme <= 0.43)
		{
			pos = ((56 * tme) - 5.6) / 0.33;
		}
		else if (tme <= 0.72)
		{
			pos = 56;
		}
		else if (tme <= 0.79)
		{
			pos = (10.4 - (9 * tme)) / 0.07;
		}
		else if (tme <= 0.95)
		{
			pos = (43.75 * tme) + 12.4375;
		}
		else if (tme <= 1.15)
		{
			pos = 54;
		}
		else if (tme <= 1.21)
		{
			pos = 111.5 - (50 * tme);
		}
		else if (tme <= 1.52)
		{
			pos = 51;
		}
		else if (tme <= 1.68)
		{
			pos = 79.5 - (18.75 * tme);
		}
		else if (tme <= 1.77)
		{
			pos = ((5 * tme) - 4.08) / 0.09;
		}
		else if (tme <= 2.05)
		{
			pos = (20.15 - (3 * tme)) / 0.28;
		}
		else if (tme <= 2.42)
		{
			pos = 50;
		}
		else if (tme <= 2.48)
		{
			pos = ((6.5 * tme) - 12.73) / 0.06;
		}
		else if (tme <= 2.77)
		{
			pos = ((3 * tme) + 25.33) / 0.58;
		}
		else if (tme <= 3.38)
		{
			pos = ((9 * tme) + 10.45) / 0.61;
		}
		else if (tme <= 3.75)
		{
			pos = ((9 * tme) - 5.63) / 0.37;
		}
		else if (tme <= 4.3)
		{
			pos = ((2.298 * tme) + 33.1825) / 0.55;
		}
		else
		{
			pos = 78.298;
		}
		return pos / 100;
	}

	double SSME::dcOPOV_ESC( double tme )
	{
		double pos;

		if (tme <= 0.12)
		{
			pos = 0;
		}
		else if (tme <= 0.3)
		{
			pos = ((14 * tme) - 1.68) / 0.09;
		}
		else if (tme <= 0.74)
		{
			pos = ((10 * tme) + 9.32) / 0.44;
		}
		else if (tme <= 0.9)
		{
			pos = ((14.5 * tme) - 4.65) / 0.16;
		}
		else if (tme <= 1.08)
		{
			pos = 52.5;
		}
		else if (tme <= 1.16)
		{
			pos = 194.25 - (131.25 * tme);
		}
		else if (tme <= 1.55)
		{
			pos = (18.7 - (2 * tme)) / 0.39;
		}
		else if (tme <= 1.6)
		{
			pos = 117.5 - (50 * tme);
		}
		else if (tme <= 1.77)
		{
			pos = 37.5;
		}
		else if (tme <= 2.09)
		{
			pos = (18.195 - (3.5 * tme)) / 0.32;
		}
		else if (tme <= 2.18)
		{
			pos = ((10 * tme) - 17.84) / 0.09;
		}
		else if (tme <= 2.4)
		{
			pos = 44;
		}
		else if (tme <= 2.46)
		{
			pos = ((4 * tme) - 6.96) / 0.06;
		}
		else if (tme <= 2.84)
		{
			pos = ((2 * tme) + 13.32) / 0.38;
		}
		else if (tme <= 3.37)
		{
			pos = ((7 * tme) + 6.62) / 0.53;
		}
		else if (tme <= 3.79)
		{
			pos = ((6 * tme) + 3.72) / 0.42;
		}
		else if (tme <= 4.29)
		{
			pos = ((0.797 * tme) + 28.47937) / 0.5;
		}
		else
		{
			pos = 63.797;
		}
		return pos / 100;
	}

	// CO
	double SSME::dcPC_CO( double tme )
	{
		double pc;

		if (tme <= 0.5)
		{
			pc = 100 - (122 * tme);
		}
		else
		{
			if (tme <= 1.8)
			{
				pc = 50.92 - (23.85 * tme);
			}
			else
			{
				if (tme <= 6)
				{
					pc = 11.43 - (1.9 * tme);
				}
				else
				{
					pc = 0;
				}
			}
		}
		return pc;
	}

	double SSME::dcMOV_CO( double tme )
	{
		double pos;

		if (tme <= 0.27)
		{
			pos = (27 - (3 * tme)) / 0.27;
		}
		else
		{
			if (tme <= 2.68)
			{
				pos = (259.96 - (97 * tme)) / 2.41;
			}
			else
			{
				pos = 0;
			}
		}
		return pos / 100;
	}

	double SSME::dcMFV_CO( double tme )
	{
		double pos;

		if (tme <= 1.3)
		{
			pos = 100;
		}
		else
		{
			if (tme <= 3.53)
			{
				pos = (272.4 - (38 * tme)) / 2.23;
			}
			else
			{
				if (tme <= 4.57)
				{
					pos = 62;
				}
				else
				{
					if (tme <= 5.4)
					{
						pos = (334.8 - (62 * tme)) / 0.83;
					}
					else
					{
						pos = 0;
					}
				}
			}
		}
		return pos / 100;
	}

	double SSME::dcCCV_CO( double tme )
	{
		double pos;

		if (tme <= 0)
		{
			pos = 100;
		}
		else if (tme <= 0.27)
		{
			pos = (27 - (31.7 * tme)) / 0.27;
		}
		else if (tme <= 0.58)
		{
			pos = 68.3;
		}
		else if (tme <= 0.71)
		{
			pos = ((6.7 * tme) + 4.993) / 0.13;
		}
		else if (tme <= 4.58)
		{
			pos = 75;
		}
		else if (tme <= 5.33)
		{
			pos = 533 - (100 * tme);
		}
		else
		{
			pos = 0;
		}
		return pos / 100;
	}

	double SSME::dcFPOV_CO( double tme, double POS1 )
	{
		double pos;

		double T1 = (POS1 - 60) / 26;

		if (tme <= T1)
		{
			pos = POS1 - (26 * tme);
		}
		else
		{
			if (tme <= (T1 + 1.15))
			{
				pos = (69 - (20 * (tme - T1))) / 1.15;
			}
			else
			{
				if (tme <= (T1 + 1.65))
				{
					pos = (40 * (T1 + 1.65 - tme)) / 0.5;
				}
				else
				{
					pos = 0;
				}
			}
		}
		return pos / 100;
	}

	double SSME::dcOPOV_CO( double tme, double POS1 )
	{
		double pos;

		double T1 = (52 - POS1) / 45;

		if (tme <= T1)
		{
			pos = POS1 - (45 * tme);
		}
		else
		{
			if (tme <= 1.4)
			{
				pos = (52 + (31.25 * T1)) - (31.25 * tme);
			}
			else
			{
				if (tme <= 1.5)
				{
					double POS2 = 8.25 + (31.25 * T1);
					pos = (POS2 * (1.5 - tme)) / 0.1;
				}
				else
				{
					pos = 0;
				}
			}
		}
		return pos / 100;
	}

	// Mainstage
	double SSME::dcPC_MS( double dtime, int ME )
	{
		double pc;

		if (SSME_DATA_TABLE[ME].cmdPC < SSME_DATA_TABLE[ME].powerlevel)
		{
			// ---
			pc = SSME_DATA_TABLE[ME].powerlevel - (10 * dtime);
			if (pc < SSME_DATA_TABLE[ME].cmdPC) pc = SSME_DATA_TABLE[ME].cmdPC;
		}
		else
		{
			// +++
			pc = SSME_DATA_TABLE[ME].powerlevel + (10 * dtime);
			if (pc > SSME_DATA_TABLE[ME].cmdPC) pc = SSME_DATA_TABLE[ME].cmdPC;
		}
		return pc;
	}

	double SSME::dcCCV_MS( double pc )
	{
		double pos;

		if (pc >= 100)
		{
			pos = 100;
		}
		else
		{
			pos = ((31.7 * pc) + 130) / 33;
		}
		return pos / 100;
	}

	double SSME::dcFPOV_MS( double pc )
	{
		return ((0.0035 * pc * pc) - (0.3168 * pc) + 74.978) / 100;
	}

	double SSME::dcOPOV_MS( double pc )
	{
		return ((0.0042 * pc * pc) - (0.4179 * pc) + 63.587) / 100;
	}

}