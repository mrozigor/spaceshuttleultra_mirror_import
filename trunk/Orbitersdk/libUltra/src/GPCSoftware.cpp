#include "GPCSoftware.h"
#include "GPC.h"

namespace dps
{
	ScheduledFunction::ScheduledFunction(GPC* pGPC, double _period, GPCSCHEDULEDFUNC _function)
		: m_gpc(pGPC), period(_period)
	{
		CallTime=0.0;
		function=_function;
	}
	void ScheduledFunction::Step(double simt)
	{
		if(CallTime<=simt) {
			function(m_gpc);
			CallTime=simt+period;
		}
	}

	GPCSoftware::GPCSoftware(GPC* pGPC)
		: m_gpc(pGPC)
	{
	}

	GPCSoftware::~GPCSoftware()
	{
		while (ScheduledFunctions.size()>0) {
			ScheduledFunction* func=ScheduledFunctions.back();
			if(func) 
				delete func;
			ScheduledFunctions.pop_back();
		}
	}

	void GPCSoftware::OnLoad()
	{
	}

	void GPCSoftware::OnPreStep(double SimT, double DeltaT, double MJD)
	{
	}

	void GPCSoftware::OnPostStep(double SimT, double DeltaT, double MJD)
	{
		for(unsigned int i=0;i<ScheduledFunctions.size();i++)
			ScheduledFunctions[i]->Step(SimT);
	}

	void GPCSoftware::CreateScheduledFunction(double fPeriod, GPCSCHEDULEDFUNC func)
	{
		ScheduledFunction* pFunc = new ScheduledFunction(m_gpc, fPeriod, func);
		ScheduledFunctions.push_back(pFunc);
	}
};