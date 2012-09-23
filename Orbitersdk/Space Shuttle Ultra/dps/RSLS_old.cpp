#include "RSLS_old.h"
#include "SSME_SOP.h"


namespace dps
{
	static char RSLSAbortCauses[4][255]={
	  "T -%f - MPS Engine %d Percent Ch Press",
	  "T -%f - Hydraulic system failure (APUs not on?)",
	  "T -%f - MPS ME-%d Shutdown Phase",
	  "T -%f - MPS E-%d Post-Shutdown Phase"
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
	}

	RSLS_old::~RSLS_old()
	{
	}

	void RSLS_old::OnPostStep( double simT, double dT, double mjd )
	{
		if(!Active) return;
		timeToLaunch=(launch_mjd-mjd)*86400.0;

		if(timeToLaunch<=31.0 && lastTTL>=31.0) STS()->GLSAutoSeqStart();
    		if(timeToLaunch<=10.0 && lastTTL>=10.0)	STS()->StartROFIs();// should be in GLS
		// engine start enable
		if (timeToLaunch <= 9.5 && lastTTL >= 9.5)
		{
			pSSME_SOP->SetStartEnableCommandFlag( 1 );
			pSSME_SOP->SetStartEnableCommandFlag( 2 );
			pSSME_SOP->SetStartEnableCommandFlag( 3 );
		}
		// engine start
		if(timeToLaunch<=6.60 && lastTTL>=6.60)	pSSME_SOP->SetEngineStartCommandFlag( 3 );
		if(timeToLaunch<=6.48 && lastTTL>=6.48)	pSSME_SOP->SetEngineStartCommandFlag( 2 );
		if(timeToLaunch<=6.36 && lastTTL>=6.36)	pSSME_SOP->SetEngineStartCommandFlag( 1 );

		// check shutdown phase
		if (pSSME_SOP->GetShutdownPhaseFlag( 1 ) == true)
		{
			RSLSAbort=true;
			RSLSAbortTime=timeToLaunch;
			RSLSAbortCause=&RSLSAbortCauses[2][0];
			RSLSAbortData=1;
		}
		if (pSSME_SOP->GetShutdownPhaseFlag( 2 ) == true)
		{
			RSLSAbort=true;
			RSLSAbortTime=timeToLaunch;
			RSLSAbortCause=&RSLSAbortCauses[2][0];
			RSLSAbortData=2;
		}
		if (pSSME_SOP->GetShutdownPhaseFlag( 3 ) == true)
		{
			RSLSAbort=true;
			RSLSAbortTime=timeToLaunch;
			RSLSAbortCause=&RSLSAbortCauses[2][0];
			RSLSAbortData=3;
		}

		// check post-shutdown phase
		if (pSSME_SOP->GetPostShutdownPhaseFlag( 1 ) == true)
		{
			RSLSAbort=true;
			RSLSAbortTime=timeToLaunch;
			RSLSAbortCause=&RSLSAbortCauses[3][0];
			RSLSAbortData=1;
		}
		if (pSSME_SOP->GetPostShutdownPhaseFlag( 2 ) == true)
		{
			RSLSAbort=true;
			RSLSAbortTime=timeToLaunch;
			RSLSAbortCause=&RSLSAbortCauses[3][0];
			RSLSAbortData=2;
		}
		if (pSSME_SOP->GetPostShutdownPhaseFlag( 3 ) == true)
		{
			RSLSAbort=true;
			RSLSAbortTime=timeToLaunch;
			RSLSAbortCause=&RSLSAbortCauses[3][0];
			RSLSAbortData=3;
		}

		if(timeToLaunch<=2.0)
		{
			if (pSSME_SOP->GetPercentChamberPressVal( 1 ) < 90.0)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[0][0];
				RSLSAbortData=1;
			}
			if (pSSME_SOP->GetPercentChamberPressVal( 2 ) < 90.0)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[0][0];
				RSLSAbortData=2;
			}
			if (pSSME_SOP->GetPercentChamberPressVal( 3 ) < 90.0)
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[0][0];
				RSLSAbortData=3;
			}
		  
			if(!STS()->HydraulicsOK())
			{
				RSLSAbort=true;
				RSLSAbortTime=timeToLaunch;
				RSLSAbortCause=&RSLSAbortCauses[1][0];
				RSLSAbortData=4;
			}
		}

		if(RSLSAbort)
		{
			if (abortfirstrun == true)
			{
				// setup
				Aborted = true;
				abortfirstrun = false;
				abortend = 0;
				switch (RSLSAbortData)// HACK no clue about engine shutdown delays
				{
					case 1:// don't command shutdown on problem engine if already in shutdown
						eng1SDtime = lastTTL;
						if (pSSME_SOP->GetShutdownPhaseFlag( 1 ) == true || pSSME_SOP->GetPostShutdownPhaseFlag( 1 ) == true)
						{
							eng1SDtime++;
							abortend++;
						}
						eng2SDtime = timeToLaunch - 1.2;
						eng3SDtime = timeToLaunch - 1.2;
						break;
					case 2:
						eng1SDtime = timeToLaunch - 2.4;
						eng2SDtime = lastTTL;
						if (pSSME_SOP->GetShutdownPhaseFlag( 2 ) == true || pSSME_SOP->GetPostShutdownPhaseFlag( 2 ) == true)
						{
							eng2SDtime++;
							abortend++;
						}
						eng3SDtime = timeToLaunch - 1.2;
						break;
					case 3:
						eng1SDtime = timeToLaunch - 2.4;
						eng2SDtime = timeToLaunch - 1.2;
						eng3SDtime = lastTTL;
						if (pSSME_SOP->GetShutdownPhaseFlag( 3 ) == true || pSSME_SOP->GetPostShutdownPhaseFlag( 3 ) == true)
						{
							eng3SDtime = 10;
							abortend++;
						}
						break;
					case 4:// hyd
						eng1SDtime = timeToLaunch - 1.2;
						eng2SDtime = timeToLaunch;
						eng3SDtime = timeToLaunch;
						break;
				}
				//				NOTEHANDLE nhAbort = oapiCreateAnnotation(true, 1.5, _V(1.0, 1.0, 0.0));
//             	oapiAnnotationSetPos(nhAbort, 0.05, 0.05, 0.75, 0.1);
//				char buf[255];
				sprintf(oapiDebugString(),RSLSAbortCause,RSLSAbortTime,RSLSAbortData);

				char buffer[100];
				sprintf_s( buffer, 100, RSLSAbortCause, RSLSAbortTime, RSLSAbortData );
				oapiWriteLog( buffer );
//              oapiAnnotationSetText(nhAbort, buf);
			}

			// engine shutdown
			if ((timeToLaunch <= eng1SDtime) && (lastTTL >= eng1SDtime))
			{
				pSSME_SOP->SetShutdownEnableCommandFlag( 1 );// both cmds now?
				pSSME_SOP->SetShutdownCommandFlag( 1 );
				abortend++;
				oapiWriteLog( "RSLS Abort SSME 1 shutdown" );
			}
			if ((timeToLaunch <= eng2SDtime) && (lastTTL >= eng2SDtime))
			{
				pSSME_SOP->SetShutdownEnableCommandFlag( 2 );// both cmds now?
				pSSME_SOP->SetShutdownCommandFlag( 2 );
				abortend++;
				oapiWriteLog( "RSLS Abort SSME 2 shutdown" );
			}
			if ((timeToLaunch <= eng3SDtime) && (lastTTL >= eng3SDtime))
			{
				pSSME_SOP->SetShutdownEnableCommandFlag( 3 );// both cmds now?
				pSSME_SOP->SetShutdownCommandFlag( 3 );
				abortend++;
				oapiWriteLog( "RSLS Abort SSME 3 shutdown" );
			}

			if (abortend == 3) Active = false;
			
		}
		else
		{
			//sprintf(oapiDebugString(),"RSLS T -%f",-timeToLaunch);
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
		return;
	}

	bool RSLS_old::OnMajorModeChange( unsigned int newMajorMode )
	{
		if (newMajorMode <= 102) return true;// HACK should only run on 101, but needed on 102 due to current liftoff sequence
		return false;
	}
};
