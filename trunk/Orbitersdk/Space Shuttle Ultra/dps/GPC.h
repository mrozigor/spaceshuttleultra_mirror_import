#pragma once 

#include "../AtlantisSubsystem.h"
#include "../discsignals/DiscInPort.h"
#include "../discsignals/DiscOutPort.h"
#include "BIU.h"

namespace dps {

	using class discsignals::DiscDemultiplex16;
	using class discsignals::DiscMultiplex16;

	class GPCSoftware;

	/**
	 * base class for all GPC subsystem units. Must implement IOP and CPU 
	 * behavior. 
	 */
	class GPC:public AtlantisSubsystem {
	protected:
		unsigned short usGPCID;
		virtual void OnIOPPrestep(double fSimT, double fDeltaT, double fMJD);
	public:
		GPC(SubsystemDirector* _direct, const string& _ident, unsigned short usGPCID);
		virtual ~GPC();
		virtual void IPL();
		virtual void LoadMemoryConfiguration(unsigned short usMemConfig);
	
		virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
	public:
		//@todo: Number is not unknown, needs research!
		DiscDemultiplex16 disc_in[4];
		DiscMultiplex16 disc_out;
		BIU channel[24];
	};

};