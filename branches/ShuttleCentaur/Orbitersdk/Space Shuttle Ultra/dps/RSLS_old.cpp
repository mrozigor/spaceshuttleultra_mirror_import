#include "RSLS_old.h"
#include "SSME_SOP.h"
#include "IO_Control.h"
#include "ATVC_SOP.h"
#include "assert.h"


namespace dps
{
	static char RSLSAbortCauses[14][255]={
	  "T%+f - MPS Engine %d Percent Ch Press",
	  "T%+f - Hydraulic system failure (APUs not on?)",
	  "T%+f - MPS E-%d in Shutdown Phase",
	  "T%+f - MPS E-%d in Post-Shutdown Phase",
	  "T%+f - MPS E-%d in Electrical Lockup Mode",
	  "T%+f - MPS E-%d in Hydraulic Lockup Mode",
	  "T%+f - MPS E-%d Pad Data Path Failure",
	  "T%+f - MPS E-%d Engine Ready Indication Not Set",
	  "T%+f - MPS E-%d MCF",
	  "T%+f - MPS E-%d Limit Exceeded",
	  "T%+f - MPS LO2 Overboard Bleed Valve Closed Indication Not Set",
	  "T%+f - MPS E-%d LH2 Prevalve Open Indication Not Set",
	  "T%+f - MPS LOX POGO Recirculation Valves Open Indication Not Set",
	  "T%+f - MPS E-%d Channel Fail"
	};
	
	RSLS_old::RSLS_old( SimpleGPCSystem *_gpc ):SimpleGPCSoftware( _gpc, "RSLS_old" )
	{
		launch_mjd=-1.0;
		timeToLaunch=-1.0;
		lastTTL = 100.0;
		Active=false;
		Aborted=false;
		RSLSAbort = false;
		abortfirstrun = true;
		eng1SDtime = 0;
		eng2SDtime = 0;
		eng3SDtime = 0;
		launchconfiggimbal = false;
	}

	RSLS_old::~RSLS_old()
	{
	}

	void RSLS_old::OnPostStep( double simT, double dT, double mjd )
	{
		if(!Active) return;
		timeToLaunch=(launch_mjd-mjd)*86400.0;

		if (RSLSAbort == true)
		{
			Abort();
			lastTTL = timeToLaunch;
			return;
		}

		//// verifications ////

		if (timeToLaunch <= 1140)// from T-19min
		{
			if (pSSME_SOP->GetPadDataPathFailureFlag( 1 ) == true)// check pad data path failure
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[6][0];
				RSLSAbortData=1;
			}
			else if (pSSME_SOP->GetPadDataPathFailureFlag( 2 ) == true)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[6][0];
				RSLSAbortData=2;
			}
			else if (pSSME_SOP->GetPadDataPathFailureFlag( 3 ) == true)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[6][0];
				RSLSAbortData=3;
			}
			else if (pSSME_SOP->GetElectricalLockupModeFlag( 1 ) == true)// check electrical lockup mode
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[4][0];
				RSLSAbortData=1;
			}
			else if (pSSME_SOP->GetElectricalLockupModeFlag( 2 ) == true)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[4][0];
				RSLSAbortData=2;
			}
			else if (pSSME_SOP->GetElectricalLockupModeFlag( 3 ) == true)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[4][0];
				RSLSAbortData=3;
			}
			else if (pSSME_SOP->GetHydraulicLockupModeFlag( 1 ) == true)// check hydraulic lockup mode
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[5][0];
				RSLSAbortData=1;
			}
			else if (pSSME_SOP->GetHydraulicLockupModeFlag( 2 ) == true)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[5][0];
				RSLSAbortData=2;
			}
			else if (pSSME_SOP->GetHydraulicLockupModeFlag( 3 ) == true)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[5][0];
				RSLSAbortData=3;
			}
			else if (pSSME_SOP->GetMajorComponentFailureFlag( 1 ) == true)// check MCF
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[8][0];
				RSLSAbortData=1;
			}
			else if (pSSME_SOP->GetMajorComponentFailureFlag( 2 ) == true)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[8][0];
				RSLSAbortData=2;
			}
			else if (pSSME_SOP->GetMajorComponentFailureFlag( 3 ) == true)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[8][0];
				RSLSAbortData=3;
			}
			else if (pSSME_SOP->GetChannelFailureFlag( 1 ) == true)// check check channel fail
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[13][0];
				RSLSAbortData=1;
			}
			else if (pSSME_SOP->GetChannelFailureFlag( 2 ) == true)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[13][0];
				RSLSAbortData=2;
			}
			else if (pSSME_SOP->GetChannelFailureFlag( 3 ) == true)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[13][0];
				RSLSAbortData=3;
			}
			else if (pSSME_SOP->GetLimitExceededFlag( 1 ) == true)// check limits exceeded
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[9][0];
				RSLSAbortData=1;
			}
			else if (pSSME_SOP->GetLimitExceededFlag( 2 ) == true)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[9][0];
				RSLSAbortData=2;
			}
			else if (pSSME_SOP->GetLimitExceededFlag( 3 ) == true)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[9][0];
				RSLSAbortData=3;
			}
		}

		if ((timeToLaunch <= 9.5 && lastTTL > 9.5))
		{
			if (pSSME_SOP->GetEngineReadyModeFlag( 1 ) == false)// check engine ready
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[7][0];
				RSLSAbortData=1;
			}
			else if (pSSME_SOP->GetEngineReadyModeFlag( 2 ) == false)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[7][0];
				RSLSAbortData=2;
			}
			else if (pSSME_SOP->GetEngineReadyModeFlag( 3 ) == false)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[7][0];
				RSLSAbortData=3;
			}
			else if ((PV20_OPInd.IsSet() == false) || (PV21_OPInd.IsSet() == false))// check LOX POGO recirc vlvs open
			{
				// TODO Fault indication for two cycles results in a RSLS Hold
				RSLSAbort = true;
				RSLSAbortTime = timeToLaunch;
				RSLSAbortCause = &RSLSAbortCauses[12][0];
				RSLSAbortData = 4;
			}
		}
		else if (timeToLaunch <= 7 && timeToLaunch > 6.60)
		{
			if (pSSME_SOP->GetEngineReadyModeFlag( 1 ) == false)// check engine ready
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[7][0];
				RSLSAbortData=1;
			}
			else if (pSSME_SOP->GetEngineReadyModeFlag( 2 ) == false)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[7][0];
				RSLSAbortData=2;
			}
			else if (pSSME_SOP->GetEngineReadyModeFlag( 3 ) == false)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[7][0];
				RSLSAbortData=3;
			}
			else if ((PV19_CLInd[0].IsSet() == false) && (PV19_CLInd[1].IsSet() == false))// check LO2 overboard bleed closed
			{
				// INFO one source says single time check at T-7s, other cvfy from T-7s to T-6.6s
				RSLSAbort = true;
				RSLSAbortTime = timeToLaunch;
				RSLSAbortCause = &RSLSAbortCauses[10][0];
				RSLSAbortData = 4;
			}
			else if ((PV4_OPInd[0].IsSet() == false) && (PV4_OPInd[1].IsSet() == false))// check LH2 prevalves open
			{
				RSLSAbort = true;
				RSLSAbortTime = timeToLaunch;
				RSLSAbortCause = &RSLSAbortCauses[11][0];
				RSLSAbortData = 1;
			}
			else if ((PV5_OPInd[0].IsSet() == false) && (PV5_OPInd[1].IsSet() == false))
			{
				RSLSAbort = true;
				RSLSAbortTime = timeToLaunch;
				RSLSAbortCause = &RSLSAbortCauses[11][0];
				RSLSAbortData = 2;
			}
			else if ((PV6_OPInd[0].IsSet() == false) && (PV6_OPInd[1].IsSet() == false))
			{
				RSLSAbort = true;
				RSLSAbortTime = timeToLaunch;
				RSLSAbortCause = &RSLSAbortCauses[11][0];
				RSLSAbortData = 3;
			}
		}
		
		if (timeToLaunch < 6.60)
		{
			if (pSSME_SOP->GetShutdownPhaseFlag( 1 ) == true)// check shutdown phase
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[2][0];
				RSLSAbortData=1;
			}
			else if (pSSME_SOP->GetShutdownPhaseFlag( 2 ) == true)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[2][0];
				RSLSAbortData=2;
			}
			else if (pSSME_SOP->GetShutdownPhaseFlag( 3 ) == true)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[2][0];
				RSLSAbortData=3;
			}
			else if (pSSME_SOP->GetPostShutdownPhaseFlag( 1 ) == true)// check post-shutdown phase
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[3][0];
				RSLSAbortData=1;
			}
			else if (pSSME_SOP->GetPostShutdownPhaseFlag( 2 ) == true)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[3][0];
				RSLSAbortData=2;
			}
			else if (pSSME_SOP->GetPostShutdownPhaseFlag( 3 ) == true)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[3][0];
				RSLSAbortData=3;
			}
		}

		if(timeToLaunch<=2.0)
		{
			if (pSSME_SOP->GetPercentChamberPressVal( 1 ) <= 90.0)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[0][0];
				RSLSAbortData=1;
			}
			else if (pSSME_SOP->GetPercentChamberPressVal( 2 ) <= 90.0)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[0][0];
				RSLSAbortData=2;
			}
			else if (pSSME_SOP->GetPercentChamberPressVal( 3 ) <= 90.0)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[0][0];
				RSLSAbortData=3;
			}
		}

		if(!STS()->HydraulicsOK())
		{
			RSLSAbort=true;
			RSLSAbortTime=timeToLaunch;
			RSLSAbortCause=&RSLSAbortCauses[1][0];
			RSLSAbortData=4;
		}

		if (RSLSAbort == true)
		{
			Abort();
			lastTTL = timeToLaunch;
			return;
		}

		//// commands ////

		if(timeToLaunch<=31.0 && lastTTL>31.0) STS()->GLSAutoSeqStart();
    		if(timeToLaunch<=10.0 && lastTTL>10.0)	STS()->StartROFIs();// HACK should be in GLS

		if (timeToLaunch <= 12.5 && lastTTL > 12.5)
		{
			// HACK should be continuous cmd from T-12.5s to T-6.6s
			pIO_Control->SetCommand( LOX_POGO_RECIRC_1, false );
			pIO_Control->SetCommand( LOX_POGO_RECIRC_2, false );
			oapiWriteLog( "RSLS: Open LOX POGO Recirculation Valves" );
		}

		if (timeToLaunch <= 11 && lastTTL > 11)
		{
			pSSME_SOP->SetThrottlePercent( 100 );
			oapiWriteLog( "RSLS: Initialize MEs throttle to 100%" );
		}

		if (timeToLaunch <= 9.5 && lastTTL > 9.5)
		{
			pSSME_SOP->SetStartEnableCommandFlag( 1 );
			pSSME_SOP->SetStartEnableCommandFlag( 2 );
			pSSME_SOP->SetStartEnableCommandFlag( 3 );
			oapiWriteLog( "RSLS: ME Start Enable Command" );

			// HACK should be continuous cmd from T-9.5s to T-6.6s
			pIO_Control->SetCommand( ME1_LH2_PVLV_OP, true );
			pIO_Control->SetCommand( ME2_LH2_PVLV_OP, true );
			pIO_Control->SetCommand( ME3_LH2_PVLV_OP, true );
			pIO_Control->SetCommand( ME1_LH2_PVLV_CL, false );
			pIO_Control->SetCommand( ME2_LH2_PVLV_CL, false );
			pIO_Control->SetCommand( ME3_LH2_PVLV_CL, false );
			oapiWriteLog( "RSLS: Open LH2 Prevalves" );
		}

		if (timeToLaunch <= 9.4 && lastTTL > 9.4)
		{
			// HACK should be continuous cmd from T-9.4s to T-6.6s
			pIO_Control->SetCommand( LOX_OVBD_BV, true );
			oapiWriteLog( "RSLS: Close LO2 Overboard Bleed Valve" );
		}

		// engine start
		if(timeToLaunch<=6.60 && lastTTL>6.60)
		{
			pSSME_SOP->SetEngineStartCommandFlag( 3 );
			oapiWriteLog( "RSLS: ME-3 Ignition Command" );
		}
		if(timeToLaunch<=6.48 && lastTTL>6.48)
		{
			pSSME_SOP->SetEngineStartCommandFlag( 2 );
			oapiWriteLog( "RSLS: ME-2 Ignition Command" );
		}
		if(timeToLaunch<=6.36 && lastTTL>6.36)
		{
			pSSME_SOP->SetEngineStartCommandFlag( 1 );
			oapiWriteLog( "RSLS: ME-1 Ignition Command" );
		}

		// move 2Y and 3Y to launch config
		if ((launchconfiggimbal == false) && (pSSME_SOP->GetPercentChamberPressVal( 1 ) > 90.0) && (pSSME_SOP->GetPercentChamberPressVal( 2 ) > 90.0) && (pSSME_SOP->GetPercentChamberPressVal( 3 ) > 90.0))
		{
			pATVC_SOP->SetSSMEActPos( 2, LAUNCHCONFIG_2P, LAUNCHCONFIG_2Y );
			pATVC_SOP->SetSSMEActPos( 3, LAUNCHCONFIG_3P, LAUNCHCONFIG_3Y );

			oapiWriteLog( "RSLS: SSME gimbal to Launch Configuration Command" );
			launchconfiggimbal = true;
		}

		//launch
		if(timeToLaunch<=0.0 && !STS()->GetLiftOffFlag() && !Aborted)
		{
			STS()->SignalGSEBreakHDP();
			STS()->IgniteSRBs();
			STS()->TriggerLiftOff();
			Active=false;
		}

		lastTTL=timeToLaunch;
	}

	void RSLS_old::Abort( void )
	{
		if (abortfirstrun == true)
		{
			oapiWriteLog( "RSLS Abort" );
			Aborted = true;
			abortfirstrun = false;
			engineSD = 0;

			sprintf(oapiDebugString(),RSLSAbortCause,RSLSAbortTime,RSLSAbortData);

			char buffer[100];
			sprintf_s( buffer, 100, RSLSAbortCause, -RSLSAbortTime, RSLSAbortData );
			oapiWriteLog( buffer );

			// HACK no clue about engine shutdown delays
			if (timeToLaunch < 6.6)// check 3 ignited
			{
				pIO_Control->SetCommand( PNEU_L_HE_XOVR_OP, true );// at least one SSME on -> open LV10
				oapiWriteLog( "RSLS: LV10 open command" );

				// assume problem in this engine, and S/D immediately ("default" action)
				eng3SDtime = timeToLaunch;
				PV123CLtime[2] = timeToLaunch - 4.5;
				PV456CLtime[2] = timeToLaunch - 4.9;

				if ((pSSME_SOP->GetShutdownPhaseFlag( 3 ) == true) || (pSSME_SOP->GetPostShutdownPhaseFlag( 3 ) == true))
				{
					// already turning off, just set time to close PVs
					eng3SDtime = 1000;

					char buffer[100];
					sprintf_s( buffer, 100, "T%+f - RSLS Abort SSME 3 already in controller initiated shutdown", -timeToLaunch );
					oapiWriteLog( buffer );
				}
				else if (RSLSAbortData != 3)
				{
					// on and healty, delay S/D
					eng3SDtime -= 1.2;
					PV123CLtime[2] -= 1.2;
					PV456CLtime[2] -= 1.2;
				}

				if (timeToLaunch < 6.48)// check 2 ignited
				{
					// assume problem in this engine, and S/D immediately ("default" action)
					eng2SDtime = timeToLaunch;
					PV123CLtime[1] = timeToLaunch - 4.5;
					PV456CLtime[1] = timeToLaunch - 4.9;

					if ((pSSME_SOP->GetShutdownPhaseFlag( 2 ) == true) || (pSSME_SOP->GetPostShutdownPhaseFlag( 2 ) == true))
					{
						// already turning off, just set time to close PVs
						eng2SDtime = 1000;

						char buffer[100];
						sprintf_s( buffer, 100, "T%+f - RSLS Abort SSME 2 already in controller initiated shutdown", -timeToLaunch );
						oapiWriteLog( buffer );
					}
					else if (RSLSAbortData != 2)
					{
						// on and healty, delay S/D
						eng2SDtime -= 1.2;
						PV123CLtime[1] -= 1.2;
						PV456CLtime[1] -= 1.2;
					}

					if (timeToLaunch < 6.36)// check 1 ignited
					{
						// assume problem in this engine, and S/D immediately ("default" action)
						eng1SDtime = timeToLaunch;
						PV123CLtime[0] = timeToLaunch - 4.5;
						PV456CLtime[0] = timeToLaunch - 4.9;

						if ((pSSME_SOP->GetShutdownPhaseFlag( 1 ) == true) || (pSSME_SOP->GetPostShutdownPhaseFlag( 1 ) == true))
						{
							// already turning off, just set time to close PVs
							eng1SDtime = 1000;

							char buffer[100];
							sprintf_s( buffer, 100, "T%+f - RSLS Abort SSME 1 already in controller initiated shutdown", -timeToLaunch );
							oapiWriteLog( buffer );
						}
						else if (RSLSAbortData != 1)
						{
							// on and healty, delay S/D
							eng1SDtime -= 2.4;
							PV123CLtime[0] -= 2.4;
							PV456CLtime[0] -= 2.4;
						}
					}
					else engineSD = 1;// ME-1 in start prep
				}
				else engineSD = 2;// ME-2 and ME-3 in start prep
			}
			else engineSD = 3;// all in start prep

			if (launchconfiggimbal == true)
			{
				// gimbal back to start config for shutdown
				pATVC_SOP->SetSSMEActPos( 2, STARTCONFIG_2P, STARTCONFIG_2Y );
				pATVC_SOP->SetSSMEActPos( 3, STARTCONFIG_3P, STARTCONFIG_3Y );
			}
		}

		// engine shutdown
		if ((timeToLaunch <= eng1SDtime) && (lastTTL > eng1SDtime))
		{
			pSSME_SOP->SetShutdownEnableCommandFlag( 1 );// both cmds now?
			pSSME_SOP->SetShutdownCommandFlag( 1 );

			char buffer[100];
			sprintf_s( buffer, 100, "T%+f - RSLS Abort SSME 1 shutdown", -timeToLaunch );
			oapiWriteLog( buffer );
		}
		if ((timeToLaunch <= eng2SDtime) && (lastTTL > eng2SDtime))
		{
			pSSME_SOP->SetShutdownEnableCommandFlag( 2 );// both cmds now?
			pSSME_SOP->SetShutdownCommandFlag( 2 );
			
			char buffer[100];
			sprintf_s( buffer, 100, "T%+f - RSLS Abort SSME 2 shutdown", -timeToLaunch );
			oapiWriteLog( buffer );
		}
		if ((timeToLaunch <= eng3SDtime) && (lastTTL > eng3SDtime))
		{
			pSSME_SOP->SetShutdownEnableCommandFlag( 3 );// both cmds now?
			pSSME_SOP->SetShutdownCommandFlag( 3 );
			
			char buffer[100];
			sprintf_s( buffer, 100, "T%+f - RSLS Abort SSME 3 shutdown", -timeToLaunch );
			oapiWriteLog( buffer );
		}

		// SSME 1 PV closing
		if ((timeToLaunch <= PV123CLtime[0]) && (lastTTL > PV123CLtime[0]))
		{
			pIO_Control->SetCommand( ME1_LOX_PVLV_OP_A, false );
			pIO_Control->SetCommand( ME1_LOX_PVLV_OP_B, false );
			pIO_Control->SetCommand( ME1_LOX_PVLV_CL_A, true );
			pIO_Control->SetCommand( ME1_LOX_PVLV_CL_B, true );
			
			char buffer[100];
			sprintf_s( buffer, 100, "T%+f - RSLS Abort SSME 1 LOX PV close command", -timeToLaunch );
			oapiWriteLog( buffer );
		}
		if ((timeToLaunch <= PV456CLtime[0]) && (lastTTL > PV456CLtime[0]))
		{
			pIO_Control->SetCommand( ME1_LH2_PVLV_OP, false );
			pIO_Control->SetCommand( ME1_LH2_PVLV_CL, true );
			engineSD++;
			
			char buffer[100];
			sprintf_s( buffer, 100, "T%+f - RSLS Abort SSME 1 LH2 PV close command", -timeToLaunch );
			oapiWriteLog( buffer );
		}
		// SSME 2 PV closing
		if ((timeToLaunch <= PV123CLtime[1]) && (lastTTL > PV123CLtime[1]))
		{
			pIO_Control->SetCommand( ME2_LOX_PVLV_OP_A, false );
			pIO_Control->SetCommand( ME2_LOX_PVLV_OP_B, false );
			pIO_Control->SetCommand( ME2_LOX_PVLV_CL_A, true );
			pIO_Control->SetCommand( ME2_LOX_PVLV_CL_B, true );
			
			char buffer[100];
			sprintf_s( buffer, 100, "T%+f - RSLS Abort SSME 2 LOX PV close command", -timeToLaunch );
			oapiWriteLog( buffer );
		}
		if ((timeToLaunch <= PV456CLtime[1]) && (lastTTL > PV456CLtime[1]))
		{
			pIO_Control->SetCommand( ME2_LH2_PVLV_OP, false );
			pIO_Control->SetCommand( ME2_LH2_PVLV_CL, true );
			engineSD++;
			
			char buffer[100];
			sprintf_s( buffer, 100, "T%+f - RSLS Abort SSME 2 LH2 PV close command", -timeToLaunch );
			oapiWriteLog( buffer );
		}
		// SSME 3 PV closing
		if ((timeToLaunch <= PV123CLtime[2]) && (lastTTL > PV123CLtime[2]))
		{
			pIO_Control->SetCommand( ME3_LOX_PVLV_OP_A, false );
			pIO_Control->SetCommand( ME3_LOX_PVLV_OP_B, false );
			pIO_Control->SetCommand( ME3_LOX_PVLV_CL_A, true );
			pIO_Control->SetCommand( ME3_LOX_PVLV_CL_B, true );
			
			char buffer[100];
			sprintf_s( buffer, 100, "T%+f - RSLS Abort SSME 3 LOX PV close command", -timeToLaunch );
			oapiWriteLog( buffer );
		}
		if ((timeToLaunch <= PV456CLtime[2]) && (lastTTL > PV456CLtime[2]))
		{
			pIO_Control->SetCommand( ME3_LH2_PVLV_OP, false );
			pIO_Control->SetCommand( ME3_LH2_PVLV_CL, true );
			engineSD++;
			
			char buffer[100];
			sprintf_s( buffer, 100, "T%+f - RSLS Abort SSME 3 LH2 PV close command", -timeToLaunch );
			oapiWriteLog( buffer );
		}

		if (engineSD == 3) Active = false;// all engines safed
		return;
	}

	bool RSLS_old::SychronizeCountdown(double mjd)
	{
		launch_mjd=mjd;
		return true;
	}

	void RSLS_old::StartRSLSSequence()
	{
		Active=true;
		Aborted=false;
	}

	void RSLS_old::Realize()
	{
		pSSME_SOP = static_cast<SSME_SOP*> (FindSoftware( "SSME_SOP" ));
		assert( (pSSME_SOP != NULL) && "RSLS_old::Realize.pSSME_SOP" );
		pIO_Control = static_cast<IO_Control*> (FindSoftware( "IO_Control" ));
		assert( (pIO_Control != NULL) && "RSLS_old::Realize.pIO_Control" );
		pATVC_SOP = static_cast<ATVC_SOP*> (FindSoftware( "ATVC_SOP" ));
		assert( (pATVC_SOP != NULL) && "MPS_Dump::Realize.ATVC_SOP" );

		discsignals::DiscreteBundle* bundle = BundleManager()->CreateBundle( "MPS_CLInd_A", 16 );
		PV19_CLInd[0].Connect( bundle, 8 );
		PV19_CLInd[1].Connect( bundle, 9 );

		bundle = BundleManager()->CreateBundle( "MPS_OPInd_A", 16 );
		PV4_OPInd[0].Connect( bundle, 3 );
		PV4_OPInd[1].Connect( bundle, 4 );
		PV5_OPInd[0].Connect( bundle, 5 );
		PV5_OPInd[1].Connect( bundle, 6 );
		PV6_OPInd[0].Connect( bundle, 7 );
		PV6_OPInd[1].Connect( bundle, 8 );

		PV20_OPInd.Connect( bundle, 15 );

		bundle = BundleManager()->CreateBundle( "MPS_OPInd_B", 16 );
		PV21_OPInd.Connect( bundle, 0 );
		return;
	}

	bool RSLS_old::OnMajorModeChange( unsigned int newMajorMode )
	{
		if (newMajorMode <= 101) return true;
		return false;
	}

	bool RSLS_old::GetRSLSAbortFlag( void ) const
	{
		return RSLSAbort;
	}
};
