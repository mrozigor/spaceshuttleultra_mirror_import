// MDM.h: Schnittstelle für die Klasse MDM.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDM_H__BC1B13B9_53D9_4DC9_A6D6_2E7EA9F155A6__INCLUDED_)
#define AFX_MDM_H__BC1B13B9_53D9_4DC9_A6D6_2E7EA9F155A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../AtlantisSubsystem.h"
#include "dps_defs.h"
#include "BIU.h"

namespace dps {

	struct PROM_COMMAND_WORD {
		unsigned parity:1;
		unsigned mode_ctrl:2;
		unsigned channel_addr:5;
		unsigned module_addr:4;
		unsigned word_count:4;
	};

	struct PROM_IOM_INFO {
		unsigned test_table:8;
		unsigned iom_class:4;
		unsigned reserved:3;
		unsigned parity:1;
	};

class IOModule;

/**
 * @brief Implementation of the MDM subsystems
 * 
 * Uses Shuttle bus serial I/O for communication with the GPCs and 
 * connects to other subsystems over serial, analog or digital lines.
 * Contains a small programmable read-only memory module of 512 words
 * length, that permits extended data transfers.
 * 
 * Default behavior is to create the I/O modules by the data encoded 
 * in the first 16 words of the PROM.
 *
 * Possible modules per MDM (maximal 16 modules):
 *    
 * - Discrete output high (DOH), 28 volts, three l6-bit channels
 * - Discrete output low (DOL), 5 volts, three l6-bit channels
 * - Discrete input high (DIH), 28 volts, three l6-bit channels
 * - Discrete input low (DIL), 5 volts, three l6-bit channels
 * - Analog output differential (AOD), 16 channels
 * - Analog input differential (AID), 16 channels
 * - Analog input single-ended (AIS), 32 channels
 * - Serial input/output (SIO), four channels
 * - Tacan/radar altimeter (special-purpose I/O)
 * 
 * FF MDM:
 * Modules 5  and 13 connect to the RJDF - likely DOH/DIL modules
 *
 */
class MDM : public AtlantisSubsystem, public IConnectedToBus  
{
protected:
	word16 m_bite_status;
	word16 SCU_PROM[512];
	IOModule* m_modules[16];
	void MasterReset(void);
public:
	/**
	 * @brief standard constructor
	 * 
	 * @param _director Reference to the subsystem director
	 * object of the vessel
	 * @param _ident identification of the subsystem
	 * @param MIA1_addr numerical address of the MIA1 in 
	 * the Shuttle Bus network
	 * @param MIA2_addr numerical address of the MIA2 in 
	 * the Shuttle Bus network
	 * @sa ShuttleBus
	 */
	MDM(AtlantisSubsystemDirector* _director, const string& _ident,
		unsigned short MIA1_addr, unsigned short MIA2_addr);
	/**
	 * /brief destructor
	 */
	virtual ~MDM();

	/**
	 * Load a PROM image from a file without creating I/O modules.
	 * /param prom_file name of the PROM image file
	 * 
	 * /callgraph
	 * /callergraph
	 */
	void LoadPROM(const std::string& prom_file);
	/**
	 * Load a PROM image from a file and create the required
	 * I/O modules
	 * 
	 * /param prom_file name of the PROM image file
	 * 
	 * /callgraph
	 * /callergraph
	 */
	void LoadPROMAndCreate(const std::string& prom_file);
	/**
	 * Create and install a digital output, high voltage 
	 * module in the MDM.
	 * 
	 * @param module_id Module address, 0 - 15
	 */
	void InstallDOHModule(unsigned int module_id);
	void InstallDIHModule(unsigned int module_id);
	void InstallDOLModule(unsigned int module_id);
	void InstallDILModule(unsigned int module_id);
	void InstallAODModule(unsigned int module_id);
	void InstallAIDModule(unsigned int module_id);
	void InstallAISModule(unsigned int module_id);
	void InstallSIOModule(unsigned int module_id);
	void InstallTacanModule(unsigned int module_id);

	/**
	 * /brief Execute a series of instructions from the programmable 
	 * read-only memory. 
	 * 
	 * /param start Starting address of the sequence in the PROM
	 * /param number_of_words Number of instructions in the sequence
	 * /param biu Reference to the Bus Terminal that received the request
	 * /param cw Bus command word that initiated the execution
	 * /param num_data Number of command data words following the command.
	 * /param cdw Command data words that followed the command word.
	 * 
	 * /callgraph
	 * /callergraph
	 * /sa dps::PROM_COMMAND_WORD
	 */
	void ExecuteProm(unsigned int start, unsigned int number_of_words,
		BusTerminal* biu, BUS_COMMAND_WORD cw, 
			unsigned long num_data, word16 *cdw);
	
	/**
	 * Call back for the shuttle bus system.
	 * Doesn't do anything
	 */
	virtual void busCommandPhase(BusController* biu);
	/**
	 * Call back for the shuttle bus system
	 * Doesn't do anything.
	 */
	virtual void busReadPhase(BusController* biu);
	/**
	 * /brief call back to react on shuttle bus command words
	 * 
	 * Takes the command word and processes it, writing command data
	 * words to I/O module channels or by reading data from I/O module
	 * channels and writing the data back as response data words.
	 * 
	 * /param biu Reference to the Bus terminal that received
	 * the command word
	 * /param cw command word of the message
	 * /param num_data number of command data words in the message
	 * /param cdw array of command data words
	 * /return The executed bus command word
	 * 
	 * /callgraph
	 * /callergraph
	 * /sa BUS_COMMAND_WORD
	 */
	virtual BUS_COMMAND_WORD busCommand(BusTerminal* biu, 
			BUS_COMMAND_WORD cw, 
			unsigned long num_data, word16 *cdw);

	/**
	 * interfaces to the ShuttleBus network
	 */
	BusTerminal mia1, mia2;
};

};

#endif // !defined(AFX_MDM_H__BC1B13B9_53D9_4DC9_A6D6_2E7EA9F155A6__INCLUDED_)
