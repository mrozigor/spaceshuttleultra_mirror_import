// AP101S.cpp: Implementierung der Klasse AP101S.
//
//////////////////////////////////////////////////////////////////////

#include "AP101S.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace dps {

	typedef struct {
		unsigned opcode:8;
		unsigned r1:4;
		unsigned r2:4;
	} RR_INSTRUCTION;

	AP101S::AP101S(AtlantisSubsystemDirector* _director, const string& _ident, unsigned short usGPCID)
	:GPC(_director, _ident, usGPCID)
	{

	}

	AP101S::~AP101S()
	{

	}

	void AP101S::GeneratePROMData() {

	}

	void AP101S::OnPropagate(double fSimT, double fDeltaT, double fMJD)
	{
		//Do thermal and power calculations here
	}

	void AP101S::OnSubPropagate(double fSimT, double fDeltaT, double fMJD)
	{

	}
	/*{
		
		//instructions per second: 1 million
		//Execute code
		if(fDeltaT > 4000.0)
			//Crash computer, too long timestep
			return;

		if(!halt) {
			
			unsigned long ulIPS = 1000000 * fDeltaT;
			word16 inst_buffer;

			RR_INSTRUCTION *rr_instr = (RR_INSTRUCTION*)(&inst_buffer);
			

			while(ulIPS != 0)
			{
				inst_buffer = memory.GetWord(psw.uIA);
			
				switch(rr_instr->opcode/64)
				{
				case 0:
					//RR instruction format
					break;
				case 1:
					//RX instruction format
					break;
				case 2:
					//RS instruction format
					break;
				case 3:
					//SS instruction format
					break;
				}
				ulIPS--;
			}
		}

	}*/

};