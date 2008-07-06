#pragma once

#include "../discsignals/DiscInPort.h"
#include "../discsignals/DiscOutPort.h"

namespace hydraulics {
	using namespace discsignals;

	class HydActuator {
		/**
		 * Command input port A
		 */
	public:
		DiscInPort InA;
		DiscInPort InB;
		DiscInPort InC;
		DiscInPort InD;

		DiscOutPort OutA;
		DiscOutPort OutB;
		DiscOutPort OutC;
		DiscOutPort OutD;
	protected:

	public:
		HydActuator();
		virtual ~HydActuator();

		double GetActPos() const;
	};
};