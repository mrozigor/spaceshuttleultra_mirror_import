#include "DiscInPort.h"
#include "DiscreteBundle.h"

namespace discsignals {

	DiscInPort::DiscInPort()
		:DiscretePort()
	{
	}

	DiscInPort::~DiscInPort()
	{
	}

	float DiscInPort::GetVoltage() const
	{
		if(pBundle) {
			return pBundle->GetVoltage(usLine);
		}
		return 0.0;
	}


	DiscDemultiplex16::DiscDemultiplex16() {

	}

	DiscDemultiplex16::~DiscDemultiplex16() {
	}

	void DiscDemultiplex16::Propagate(double fSimT, double fDeltaT, double fMJD) {
	}

};