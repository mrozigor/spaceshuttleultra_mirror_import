#include "GPC.h"

namespace dps {

	GPC::GPC(SubsystemDirector* _direct, const string& _ident) 
		: AtlantisSubsystem(_direct, _ident)
	{
	}

	GPC::~GPC() {

	}

	void GPC::IPL() {

	}

	void GPC::LoadMemoryConfiguration(unsigned short usMemConfig) {

	}

	void GPC::OnIOPPrestep(double fSimT, double fDeltaT, double fMJD) {
	}

	void GPC::OnPreStep(double fSimT, double fDeltaT, double fMJD) {

	}

	void GPC::OnPostStep(double fSimT, double fDeltaT, double fMJD) {

	}
};