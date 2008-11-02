#include "IDP.h"
#include "IDPSoftware.h"

namespace dps {
	
	IDPSoftware::IDPSoftware(IDP *hardware) 
		: pIDP (hardware) {
	}

	IDPSoftware::~IDPSoftware() {
	}


	IDP_OTP::IDP_OTP(IDP *hardware) 
		: IDPSoftware(hardware) {
		usFormat = 0;
	}

	IDP_OTP::~IDP_OTP() {
	}

	void IDP_OTP::Run(double fSimT, double fDeltaT) {
	}

	void IDP_OTP::PaintMDU(vc::MDU* mdu) {

		
	}

};