#include "RSLS.h"

namespace dps
{
	static char RSLSAbortCauses[2][255]={
	  "T -%f - Main engine %d failure",
	  "T -%f - Hydraulic system failure (APUs not on?)"
	};
	RSLS::RSLS()
		: GPCSoftware("G1PL1SEQ")
	{
		launch_mjd=-1.0;
		timeToLaunch=-1.0;
		Active=false;
		Aborted=false;
	}

	RSLS::~RSLS()
	{
	}

	void RSLS::OnPostStep(double simT, double dT, double mjd) {
		if(!Active) return;
		timeToLaunch=(launch_mjd-mjd)*86400.0;

		
		/*
    	if(timeToLaunch<=10.0 && lastTTL>=10.0)	sts->StartROFIs();
		if(timeToLaunch<=8.0 && lastTTL>=8.0) sts->SignalGSEStart();
		if(timeToLaunch<=6.60 && lastTTL>=6.60)	sts->SetSSMEThrustLevel(3, 1.0);
		if(timeToLaunch<=6.48 && lastTTL>=6.48)	sts->SetSSMEThrustLevel(2, 1.0);
		if(timeToLaunch<=6.36 && lastTTL>=6.36)	sts->SetSSMEThrustLevel(1, 1.0);
		*/

		bool RSLSAbort=false;
		if(timeToLaunch<=2.0) {
			/*
		  if(sts->GetSSMEThrustLevel(1)<0.9) {
			RSLSAbort=true;
			RSLSAbortTime=timeToLaunch;
			RSLSAbortCause=&RSLSAbortCauses[0][0];
			RSLSAbortData=1;
		  }
    	  if(sts->GetSSMEThrustLevel(2)<0.9) {
			RSLSAbort=true;
			RSLSAbortTime=timeToLaunch;
			RSLSAbortCause=&RSLSAbortCauses[0][0];
			RSLSAbortData=2;
		  }
		  if(sts->GetSSMEThrustLevel(3)<0.9)  {
			RSLSAbort=true;
			RSLSAbortTime=timeToLaunch;
			RSLSAbortCause=&RSLSAbortCauses[0][0];
			RSLSAbortData=3;
		  }
		  if(!sts->HydraulicsOK())  {
			RSLSAbort=true;
			RSLSAbortTime=timeToLaunch;
			RSLSAbortCause=&RSLSAbortCauses[1][0];
			RSLSAbortData=1;
		  }
		  */
		}

		if(RSLSAbort) {
			Active=false;
			/*
			sts->SetSSMEThrustLevel(1, 0);
			sts->SetSSMEThrustLevel(2, 0);
			sts->SetSSMEThrustLevel(3, 0);
			*/
            Aborted=true;
//				NOTEHANDLE nhAbort = oapiCreateAnnotation(true, 1.5, _V(1.0, 1.0, 0.0));
//             	oapiAnnotationSetPos(nhAbort, 0.05, 0.05, 0.75, 0.1);
//				char buf[255];
			sprintf(oapiDebugString(),RSLSAbortCause,RSLSAbortTime,RSLSAbortData);
//              oapiAnnotationSetText(nhAbort, buf);
			
		} else {
			//sprintf(oapiDebugString(),"RSLS T -%f",-timeToLaunch);
		}

		//launch
		/*
		if(timeToLaunch<=0.0 && !sts->GetLiftOffFlag() && !Aborted) {
			
			sts->SignalGSEBreakHDP();
			sts->IgniteSRBs();
			sts->TriggerLiftOff();
			
			Active=false;
		}
		*/

		lastTTL=timeToLaunch;
	}

	bool RSLS::SychronizeCountdown(double mjd)
	{
		launch_mjd=mjd;
		return true;
	}

	void RSLS::StartRSLSSequence()
	{
		Active=true;
		Aborted=false;
	}
};
