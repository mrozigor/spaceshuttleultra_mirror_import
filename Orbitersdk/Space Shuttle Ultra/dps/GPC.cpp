#include "GPC.h"
#include <sstream>

namespace dps {

	GPC::GPC(SubsystemDirector* _direct, const string& _ident, unsigned short usGPCID) 
		: AtlantisSubsystem(_direct, _ident)
	{
		this->usGPCID = usGPCID;

		ostringstream sbuf;

		for(int i = 0; i<20; i++) {
			sbuf.str("");
			sbuf << "MIA" << i + 1;
			channel[i].Init(this, NULL, sbuf.str(), usGPCID);
		}
		ICC[0].Init(this, NULL, "ICCA", (usGPCID + 1)%5 + 1);
		ICC[1].Init(this, NULL, "ICCB", (usGPCID + 2)%5 + 1);
		ICC[2].Init(this, NULL, "ICCC", (usGPCID + 3)%5 + 1);
		ICC[3].Init(this, NULL, "ICCD", (usGPCID + 4)%5 + 1);
	}

	GPC::~GPC() {

	}

	void GPC::OnPreStep(double fSimT, double fDeltaT, double fMJD) {

	}

	void GPC::OnPropagate(double fSimT, double fDeltaT, double fMJD) {
		disc_in[0].Propagate(fSimT, fDeltaT, fMJD);
		disc_in[1].Propagate(fSimT, fDeltaT, fMJD);
		disc_in[2].Propagate(fSimT, fDeltaT, fMJD);
		disc_in[3].Propagate(fSimT, fDeltaT, fMJD);
		disc_out.Propagate(fSimT, fDeltaT, fMJD);
	}

	void GPC::OnPostStep(double fSimT, double fDeltaT, double fMJD) {
		//sprintf_s(oapiDebugString(), 255, "sizeof(__psw) = %d", sizeof(AP101PSW));
	}
};