#pragma once 

#include "../AtlantisSubsystem.h"

namespace dps {

	class GPCSoftware;

	/**
	 * base class for all GPC subsystem units. Must implement IOP and CPU 
	 * behavior. 
	 */
	class GPC:public AtlantisSubsystem {
	protected:
		virtual void OnIOPPrestep(double fSimT, double fDeltaT, double fMJD);
	public:
		GPC(SubsystemDirector* _direct, const string& _ident);
		virtual ~GPC();
		virtual void IPL();
		virtual void LoadMemoryConfiguration(unsigned short usMemConfig);
	
		virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
	};

};