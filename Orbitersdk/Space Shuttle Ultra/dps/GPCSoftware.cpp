#include "GPCSoftware.h"

namespace dps
{
	ScheduledFunction::ScheduledFunction(Atlantis* _sts, double _period, GPCSCHEDULEDFUNC _function)
		: sts(_sts), period(_period)
	{
		CallTime=0.0;
		function=_function;
	}
	void ScheduledFunction::Step(double simt)
	{
		if(CallTime<=simt) {
			function(sts);
			CallTime=simt+period;
		}
	}

	GPCSoftware::GPCSoftware(Atlantis* _sts)
		: sts(_sts)
	{
	}

	GPCSoftware::~GPCSoftware()
	{
		while (ScheduledFunctions.size()>0) {
			ScheduledFunction* func=ScheduledFunctions.back();
			if(func) delete func;
			ScheduledFunctions.pop_back();
		}
	}

	void GPCSoftware::OnPreStep(double SimT, double DeltaT, double MJD)
	{
	}

	void GPCSoftware::OnPostStep(double SimT, double DeltaT, double MJD)
	{
		for(unsigned int i=0;i<ScheduledFunctions.size();i++)
			ScheduledFunctions[i]->Step(SimT);
	}
};