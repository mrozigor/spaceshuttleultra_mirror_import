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
	};
};