#pragma once

#include "IDP.h"

namespace dps {

	/**
	 * Operational test program
	 */
	class IDP_OTP: public IDPSoftware {
		unsigned short usFormat;
	public:
		IDP_OTP(IDP* hardware);
		virtual ~IDP_OTP();

		virtual void Run(double fSimT, double fDeltaT);
		virtual void PaintMDU(vc::MDU* pmdu);
	};

}