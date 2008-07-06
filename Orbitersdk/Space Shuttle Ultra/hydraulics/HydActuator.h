#pragma once

#include "../discsignals/DiscInPort.h"

namespace hydraulics {
	using namespace discsignals;

	class HydActuator {
		/**
		 * Command input port A
		 */
		DiscInPort InA;
		DiscInPort InB;
		DiscInPort InC;
		DiscInPort InD;
	public:
		HydActuator();
		virtual ~HydActuator();

		double GetActPos() const;
	};
};