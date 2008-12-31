#ifndef _RSLS_H
#define _RSLS_H
#pragma once

#include "../Atlantis.h"
#include "dps_defs.h"
#include "GPCSoftware.h"

namespace dps
{
	class RSLS : public GPCSoftware
	{
	public:
		RSLS(Atlantis* _sts);
		~RSLS();

		virtual void OnPostStep(double simT, double dT, double mjd);

		//Communication with LCC
		bool SychronizeCountdown(double mjd);
		void StartRSLSSequence();
	private:
		double launch_mjd, timeToLaunch, lastTTL, RSLSAbortTime;
		bool Active,Aborted;
		char* RSLSAbortCause;
		int RSLSAbortData;
	};
};

#endif //_RSLS_H
