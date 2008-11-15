#pragma once

#include "DiscretePort.h"

namespace discsignals {

	class DiscInPort: public DiscretePort {
	public:
		DiscInPort();
		virtual ~DiscInPort();

		bool IsSet() const;
		bool IsSet(float fThreshold) const;

		float GetVoltage() const;

		inline operator bool() const {
			return IsSet();
		};
	};

	class DiscDemultiplex16: public DiscMultiplex {
	/*protected:
		unsigned short usLatch;*/
	public:
		DiscInPort r[16];
	public:
		DiscDemultiplex16();
		virtual ~DiscDemultiplex16();
		unsigned short GetValue() const;
		unsigned short GetValueBCD() const;
		virtual void Propagate(double fSimT, double fDeltaT, double fMJD);
	};
};