// MDM.cpp: Implementierung der Klasse MDM.
//
//////////////////////////////////////////////////////////////////////

#include "../Atlantis.h"
#include "MDM.h"
#include "BIU.h"
#include "dps_defs.h"
#include "../SSUOptions.h"
#include "assert.h"
#include "IOModule.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace dps {

MDM::MDM(AtlantisSubsystemDirector* _director, const string& _ident,
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
	assert(cdw != NULL);
	word16 tmp_data[32];

	switch(cw.mode_ctrl)
	{
	case 0:
		break;
	case 1:
		for(unsigned int i = 0; i<=cw.word_count; i++)
		{
			tmp_data[i] = SCU_PROM[cw.prom_addr + i];
		}
		biu->WriteData(cw.word_count, tmp_data);
		break;
	case 2:
		ExecuteProm(cw.prom_addr, cw.word_count,biu, cw, num_data, cdw);
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
		if(m_modules[cw.module_addr] != 0)
		{
			for(unsigned int i = 0; i<cw.word_count+1; i++)
			{	
				m_modules[cw.module_addr]->WriteToChannel(cw.channel_addr, cdw[i]);
			}
		}
		break;
	case 9:
		//Read data
		if(m_modules[cw.module_addr] != 0)
		{
			for(unsigned int i = 0; i<cw.word_count+1; i++)
			{
				m_modules[cw.module_addr]->ReadFromChannel(cw.channel_addr, tmp_data[i]);	
			}
			biu->WriteData(cw.word_count + 1, tmp_data);
		}
		
		break;
	case 10:
		//Send and reset BITE
		biu->WriteData(1, &m_bite_status);
		m_bite_status = 0;
		break;
	case 11:
		//Master reset
		MasterReset();
		break;
	case 12:
		//Send last command word
		biu->WriteData(1, reinterpret_cast<word16*>(&cw));
		break;
	case 13:
		//Spare
		break;
	case 14:
		//load BITE register
		m_bite_status = cdw[0];
		break;
	case 15:
		//spare
		break;
	}

	return cw;
}

void MDM::ExecuteProm(unsigned int start, unsigned int number_of_words,
	BusTerminal* biu, BUS_COMMAND_WORD cw, 
	unsigned long num_data, word16 *cdw)
{
	PROM_COMMAND_WORD* operation;
	PROM_IOM_INFO* module_info;
	IOModule* iom;
	word16 io_buffer[32];
	short incr = 1;
	unsigned short cdw_ptr = 0;

	for(unsigned int i = 0; i<number_of_words; i++)
	{
		if(start > 511)
			return;


		operation = reinterpret_cast<PROM_COMMAND_WORD*>(&SCU_PROM[start]);
		module_info = reinterpret_cast<PROM_IOM_INFO*>(&SCU_PROM[operation->module_addr]);
		iom = m_modules[operation->module_addr];
		unsigned short channel = operation->channel_addr;

		if(iom == NULL)
			continue;

		if(module_info->iom_class == 0x4)
		{
			incr = 0;
		} else
		{
			incr = 1;
		}

		switch(operation->mode_ctrl)
		{
		case 0:
			//initiate command data transfer
			for(unsigned int j = 0; j < operation->word_count; ++j)
			{
				iom->WriteToChannel(channel, cdw[cdw_ptr]);
				cdw_ptr++;
				channel += incr;
			}
			break;
		case 1:
			//initiate response data transfer
			for(unsigned int j = 0; j < operation->word_count; ++j)
			{
				iom->ReadFromChannel(channel, io_buffer[j]);
				channel += incr;
			}
			mia1.WriteData(operation->word_count, io_buffer);
			
			break;
		case 2:
			//send and reset BITE status register
			
			break;
		case 3:
			//initiate BITE
			iom->BITE();
			break;
		}
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

//1355


void MDM::InstallAODModule(unsigned int module_id)
{
	if(m_modules[module_id]) delete m_modules[module_id];
	m_modules[module_id] = new AODModule(this);

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

void MDM::LoadMDM(const std::string& prom_file)
{
	std::string filename;
	std::ifstream prom;
	char buffer[400];
	try {
		filename = prom_file;

		prom.open(filename.c_str());
		if (!prom)
		{
			filename = STS()->options->GetROMFilePath() + string("\\") + prom_file;
			prom.open(filename.c_str());
		}

		if (prom)
		{

			while (!prom.eof())
			{
				prom.getline(buffer, 400);
				//Comments start with *
				if (buffer[0] != '*' && strlen(buffer) > 0)
				{

					parseMDMFileLine(buffer);
				}
			}
			prom.close();
		}
		else
		{
			char buffer[400];
			sprintf_s(buffer, 400, "(SpaceShuttleUltra) [ERROR] Can't load PROM image '%s' for '%s'",
				filename.c_str(), GetQualifiedIdentifier().c_str());
			oapiWriteLog(buffer);
		}
	}
	catch (std::exception &e) 
	{
		char buffer[400];
		sprintf_s(buffer, 400, "(SpaceShuttleUltra) [ERROR] Can't load PROM image '%s' for '%s': Unknown exception : %s",
			filename.c_str(), GetQualifiedIdentifier().c_str(), e.what());
		oapiWriteLog(buffer);
	}
}

void MDM::MasterReset(void)
{
	m_bite_status = 0;
	//Reset all I/O modules
	for(unsigned int i = 0; i<16; i++)
	{
		if(m_modules[i] != NULL)
			m_modules[i]->Reset();
	}
}

void MDM::parseMDMFileLine(const char* line)
{
	char buffer[400];
	unsigned int memloc = 0;
	char command[10];
	unsigned int module = 0;
	unsigned int channel = 0;
	unsigned int wordcount = 0;

	command[0] = '\0';

	int fields = sscanf_s(line, "%d %s %d %d %d", &memloc, command, 10, &module, &channel, &wordcount);

	if (memloc < 16) {
		//Allow only module description fields
		if (!_strnicmp(command, "DIL", 3)) {
			InstallDILModule(memloc);
		}
		else if (!_strnicmp(command, "DIH", 3)) {
			InstallDIHModule(memloc);
		}
		else if (!_strnicmp(command, "DOL", 3)) {
			InstallDOLModule(memloc);
		}
		else if (!_strnicmp(command, "DOH", 3)) {
			InstallDOHModule(memloc);
		}
		else if (!_strnicmp(command, "SIO", 3)) {
			//InstallSIOModule(memloc);
		}
		else if (!_strnicmp(command, "AIS", 3)) {
			InstallAISModule(memloc);
		}
		else if (!_strnicmp(command, "AID", 3)) {
			InstallAIDModule(memloc);
		}
		else if (!_strnicmp(command, "AOD", 3)) {
			InstallAODModule(memloc);
		}
		else if (!_strnicmp(command, "TACAN", 3)) {
			//InstallTACANModule(memloc);
		}
		else {
			sprintf_s(buffer, 400, "(SpaceShuttleUltra) [ERROR] MDM: ILLEGAL MODULE TYPE %s IN LOCATION %d", command, memloc);
			oapiWriteLog(buffer);
		}
	}
	else if (memloc < 512) {
		//Allow only IO commands
		if (!_strnicmp(command, "BITE", 4))
		{
			//
		}
		else if (!_strnicmp(command, "BSEND", 5))
		{
			//
		} 
		else if (!_strnicmp(command, "CMD", 3))
		{
			//
		}
		else if (!_strnicmp(command, "RESP", 4))
		{
			//
		}
		else

		{
			sprintf_s(buffer, 400, "(SpaceShuttleUltra) [ERROR] MDM: ILLEGAL OPERATION %s IN LOCATION %d", command, memloc);
			oapiWriteLog(buffer);
		}
	}
	else {
		//ILLEGAL MEMORY ADDRESS
		//MAYBE USE THIS FOR SPECIAL COMMANDS?
		sprintf_s(buffer, 400, "(SpaceShuttleUltra) [ERROR] MDM: ILLEGAL MEMORY LOCATION %d", memloc);
		oapiWriteLog(buffer);
	}
}

};
