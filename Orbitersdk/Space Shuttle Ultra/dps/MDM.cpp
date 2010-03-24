// MDM.cpp: Implementierung der Klasse MDM.
//
//////////////////////////////////////////////////////////////////////

#include "../Atlantis.h"
#include "MDM.h"
#include "BIU.h"
#include "dps_defs.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace dps {

MDM::MDM(SubsystemDirector* _director, const string& _ident,
		 unsigned short MIA1_addr, unsigned short MIA2_addr)
: AtlantisSubsystem(_director, _ident)
{
	/*
	void Init(AtlantisSubsystem* pSubsys,
			IConnectedToBus* owner,
			const string& _label, 
			unsigned short usTermID);
	*/
	this->mia1.Init(this, this, "MIA1", MIA1_addr);
	this->mia2.Init(this, this, "MIA2", MIA2_addr);
	for(unsigned int i = 0; i<16; i++)
	{
		m_modules[i] = NULL;	
		SCU_PROM[i] = (word16)0xFFFF;
	}
}

MDM::~MDM()
{

}

void MDM::busCommandPhase(BusController* biu)
{
}

void MDM::busReadPhase(BusController* biu)
{
}
BUS_COMMAND_WORD MDM::busCommand(BusTerminal* biu, BUS_COMMAND_WORD cw, 
			unsigned long num_data, word16 *cdw)
{
	word16 tmp_data[16];

	switch(cw.mode_ctrl)
	{
	case 0:
		break;
	case 1:
		for(unsigned int i = 0; i<cw.word_count; i++)
		{
			tmp_data[i] = SCU_PROM[cw.prom_addr + i];
		}
		biu->WriteData(cw.word_count, tmp_data);
		break;
	case 2:
		ExecuteProm(cw.prom_addr, cw.word_count);
		break;
	case 3:
		break;
	case 4:
		//BITE 1 (SCU)
		break;
	case 5:
		//BITE 2 (A/D)
		break;
	case 6:
		//BITE 3 (Power supply)
		break;
	case 7:
		//BITE 4 (IOM)
		break;
	case 8:
		//Write data
		break;
	case 9:
		//Read data
		break;
	case 10:
		//Send and reset BITE
		break;
	case 11:
		//Master reset
		break;
	case 12:
		biu->WriteData(2, reinterpret_cast<word16*>(&cw));
		break;
	case 13:
		//Spare
		break;
	case 14:
		//load BITE register
		break;
	case 15:
		//spare
		break;
	}

	return cw;
}

void MDM::ExecuteProm(unsigned int start, unsigned int number_of_words)
{
	for(unsigned int i = 0; i<number_of_words; i++)
	{

		start++;
	}
}

void MDM::InstallDOHModule(unsigned int module_id)
{
	SCU_PROM[module_id] = 0x0300;
}

void MDM::InstallDOLModule(unsigned int module_id)
{
	SCU_PROM[module_id] = 0x0300;
}

void MDM::InstallDIHModule(unsigned int module_id)
{
	SCU_PROM[module_id] = 0x0100;
}

void MDM::InstallDILModule(unsigned int module_id)
{
	SCU_PROM[module_id] = 0x0100;
}

void MDM::InstallAIDModule(unsigned int module_id)
{
	SCU_PROM[module_id] = 0x0200;
}

void MDM::InstallAISModule(unsigned int module_id)
{
	SCU_PROM[module_id] = 0x0200;
}


void MDM::InstallAODModule(unsigned int module_id)
{
	SCU_PROM[module_id] = 0x0600;
}

void MDM::InstallSIOModule(unsigned int module_id)
{
	SCU_PROM[module_id] = 0x0400;
}

void MDM::InstallTacanModule(unsigned int module_id)
{
	SCU_PROM[module_id] = 0x0500;
}



};
