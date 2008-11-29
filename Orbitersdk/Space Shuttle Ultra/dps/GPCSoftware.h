#ifndef _GPCSOFTWARE_H
#define _GPCSOFTWARE_H

#include "../Atlantis.h"

typedef void (*GPCSCHEDULEDFUNC) (Atlantis*);

namespace dps
{	
	class ScheduledFunction
	{
	public:
		ScheduledFunction(Atlantis* _sts, double _period, GPCSCHEDULEDFUNC _function);
		void Step(double simt);
	private:
		GPCSCHEDULEDFUNC function;
		Atlantis* sts;

		/**
		 * Sim time at which to call function
		 */
		double CallTime;
		/**
		 * Period between function calls
		 */
		double period;
	};

	class GPCSoftware
	{
	public:
		GPCSoftware(Atlantis* _sts);
		~GPCSoftware();

		virtual void OnPreStep(double SimT, double DeltaT, double MJD);
		virtual void OnPostStep(double SimT, double DeltaT, double MJD);

		static GPCSoftware* CreateSoftware(const string& _softname);
	protected:
		Atlantis* sts;

		//scheduled GPC functions
		vector<ScheduledFunction*> ScheduledFunctions;
	};
};

#endif //!_GPSOFTWARE_H
