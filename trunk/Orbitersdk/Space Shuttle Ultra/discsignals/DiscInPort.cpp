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
		else
			return 0.0;
	}

	bool DiscInPort::IsSet() const {
		return IsSet(4.7f);
	}

	bool DiscInPort::IsSet(float fThreshold) const {
		if(pBundle)
		{
			return (pBundle->GetVoltage(usLine) > fThreshold);
		}
		else {
			return false;
		}
	}

	DiscDemultiplex16::DiscDemultiplex16() {

	}

	DiscDemultiplex16::~DiscDemultiplex16() {
	}

	void DiscDemultiplex16::Propagate(double fSimT, double fDeltaT, double fMJD) {
	}

	

};
