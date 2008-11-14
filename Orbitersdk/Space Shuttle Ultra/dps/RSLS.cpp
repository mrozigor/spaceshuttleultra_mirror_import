#include "RSLS.h"

namespace dps
{
	RSLS::RSLS(Atlantis* _sts)
		: GPCSoftware(_sts)
	{
		launch_mjd=-1.0;
		timeToLaunch=-1.0;
		Active=false;
	}

	RSLS::~RSLS()
	{
	}

	void RSLS::OnPostStep(double simT, double dT, double mjd)
	{
		timeToLaunch=(launch_mjd-mjd)*86400.0;

		if(Active) {
			if(timeToLaunch<=10.0 && lastTTL>=10.0)
				sts->StartROFIs();

			if(timeToLaunch<=6.60 && lastTTL>=6.60)
				sts->SetSSMEThrustLevel(3, 1.0);
			if(timeToLaunch<=6.48 && lastTTL>=6.48)
				sts->SetSSMEThrustLevel(2, 1.0);
			if(timeToLaunch<=6.36 && lastTTL>=6.36)
				sts->SetSSMEThrustLevel(1, 1.0);

			//launch
			if(timeToLaunch<=0.0 && !sts->GetLiftOffFlag()) {
				sts->SignalGSEBreakHDP();
				sts->IgniteSRBs();
				sts->TriggerLiftOff();
			}
		}

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
	}
};
