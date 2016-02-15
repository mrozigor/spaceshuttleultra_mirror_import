#include "SimpleGPC.h"
#include <sstream>

namespace dps {

	SimpleGPC::SimpleGPC(AtlantisSubsystemDirector* _direct, const string& _ident, unsigned short usGPCID) 
		: GPC(_direct, _ident, usGPCID)
	{
		
	}

	SimpleGPC::~SimpleGPC() {

	}

	void* SimpleGPC::AllocateGPCMemory(unsigned long usSize) {
		return NULL;
	}

	void SimpleGPC::ClearGPCMemory() {
	}

	word24 SimpleGPC::fcosClose()
	{
		return __word24(0);
	}

	word24 SimpleGPC::fcosSchedule()
	{
		return __word24(0);
	}

	word24 SimpleGPC::fcosWait(long delay)
	{
		return __word24(0);
	}

	void SimpleGPC::FreeApplicationMemory() {
		//Reset the memory management tables
	}

	void SimpleGPC::FreeGPCMemory(void* pMemBlock) {
	}

	

	void SimpleGPC::IPL() {
			//Load the first 24 bytes of memory from MMU1 or MMU2
		if(ipl_source1.IsSet()) {
			
		} else if(ipl_source2.IsSet()) {

		}
		//Load GPCIPL software
	}

	void SimpleGPC::LoadMemoryConfiguration(unsigned short usMemConfig) {

	}

	void SimpleGPC::MinorCycle(double fSimT, double fDeltaT, double fMJD) {
	}
	
	void SimpleGPC::MajorCycle(double fSimT, double fDeltaT, double fMJD) {
	}
	

	void SimpleGPC::OnIOPPrestep(double fSimT, double fDeltaT, double fMJD) {
	}

	void SimpleGPC::OnPreStep(double fSimT, double fDeltaT, double fMJD) {
		GPC::OnPreStep(fSimT, fDeltaT, fMJD);
	}

	void SimpleGPC::OnPropagate(double fSimT, double fDeltaT, double fMJD) {
		GPC::OnPropagate(fSimT, fDeltaT, fMJD);
	}

	void SimpleGPC::OnPostStep(double fSimT, double fDeltaT, double fMJD) {
		//sprintf_s(oapiDebugString(), 255, "sizeof(__psw) = %d", sizeof(AP101PSW));
	}

	word24 SimpleGPC::SupervisorCall(char function)
	{
		return __word24(0);
	}
};