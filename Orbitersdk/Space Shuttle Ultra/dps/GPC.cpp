#include "GPC.h"
#include <sstream>

namespace dps {

	GPC::GPC(SubsystemDirector* _direct, const string& _ident, unsigned short usGPCID) 
		: AtlantisSubsystem(_direct, _ident)
	{
		this->usGPCID = usGPCID;

		ostringstream sbuf;

		for(int i = 0; i<24; i++) {
			sbuf.str("");
			sbuf << "MIA" << i + 1;
			channel[i].Init(this, sbuf.str(), usGPCID, false);
		}
	}

	GPC::~GPC() {

	}

	void* GPC::AllocateGPCMemory(unsigned long usSize) {
		return NULL;
	}

	void GPC::ClearGPCMemory() {
	}

	void GPC::FreeApplicationMemory() {
		//Reset the memory management tables
	}

	void GPC::FreeGPCMemory(void* pMemBlock) {
	}

	

	void GPC::IPL() {
			//Load the first 24 bytes of memory from MMU1 or MMU2
		if(ipl_source1.IsSet()) {
			
		} else if(ipl_source2.IsSet()) {

		}
	}

	void GPC::LoadMemoryConfiguration(unsigned short usMemConfig) {

	}

	void GPC::MinorCycle(double fSimT, double fDeltaT, double fMJD) {
	}
	
	void GPC::MajorCycle(double fSimT, double fDeltaT, double fMJD) {
	}
	

	void GPC::OnIOPPrestep(double fSimT, double fDeltaT, double fMJD) {
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