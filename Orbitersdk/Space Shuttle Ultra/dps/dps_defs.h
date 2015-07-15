/****************************************************************************
  This file is part of Space Shuttle Ultra

  Data Processing System definitions



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/
#pragma once

const char SSU_KEY_FAULTSUMM = 0;
const char SSU_KEY_SYSSUMM = 1;
const char SSU_KEY_MSGRESET = 2;
const char SSU_KEY_ACK = 3;
const char SSU_KEY_GPCIDP = 4;
const char SSU_KEY_A = 5;
const char SSU_KEY_B = 6;
const char SSU_KEY_C = 7;
const char SSU_KEY_IORESET = 8;
const char SSU_KEY_D = 9;
const char SSU_KEY_E = 10;
const char SSU_KEY_F = 11;
const char SSU_KEY_ITEM = 12;
const char SSU_KEY_1 = 13;
const char SSU_KEY_2 = 14;
const char SSU_KEY_3 = 15;
const char SSU_KEY_EXEC = 16;
const char SSU_KEY_4 = 17;
const char SSU_KEY_5 = 18;
const char SSU_KEY_6 = 19;
const char SSU_KEY_OPS = 20;
const char SSU_KEY_7 = 21;
const char SSU_KEY_8 = 22;
const char SSU_KEY_9 = 23;
const char SSU_KEY_SPEC = 24;
const char SSU_KEY_MINUS = 25;
const char SSU_KEY_0 = 26;
const char SSU_KEY_PLUS = 27;
const char SSU_KEY_RESUME = 28;
const char SSU_KEY_CLEAR = 29;
const char SSU_KEY_DOT = 30;
const char SSU_KEY_PRO = 31;


namespace dps {

	class IDP;
	
	class SimpleGPCSystem;
	class OrbitDAP; // temporary
	class RSLS;
	class RSLS_old;
	class MMU;
	class MDM;
	class MDMIOModule;
	class MasterTimingUnit;
	class ShuttleBus;
	class ShuttleBusManager;
	class TimeBus;
	class GPC;
	class AP101S;
	class SimpleGPC;
	class SimpleGPCSoftware;
	/**
	 * Connection between Shuttle Bus and subsystems.
	 */
	class BIU;
	class BusTerminal;
	class BusController;
	/**
	 * Class representing a random access memory with 32 bit words. 
	 */
	class RAM32;

	//typedef long word24;
	typedef long word32;
	typedef short word16;

	typedef union {
		unsigned int uaddr:24;
		char cd[3];
	} word24;

	inline word24 __word24(int x)
	{
		word24 y;
		y.uaddr = x;
		return y;
	}
	

	typedef float float32;
	typedef unsigned char gpcchar;

	typedef enum {
		GNC = 0,
		SM = 1,
		PL = 2
	} MAJORFUNCTION;

	struct DEUCHAR {
		char cSymbol;
		char cAttr;
	};

	struct DEU_LINE {
		int x0, y0;
		int x1, y1;
		char cAttr;
	};

	struct DEU_ELLIPSE {
		int xLeft, yTop, xRight, yBottom;
		char cAttr;
	};

	struct DEU_STATUS {
		word16 bite1;
		word16 bite2;
		word16 bite3;
		word16 bite4;
		word16 deu_mf;
		word16 cmd_len;

		gpcchar cmd_line[32];
	};

	/**
	 * @brief low-level HAL/S FC event type
	 * Event type as used in AP-101 computers. 
	 */
	struct EVENT {
		/**
		 * 
		 */
		int event_used:15;
		int event_state:1;
	};

	struct PSW {
		unsigned int uIA:24;
		unsigned int uPM:4;
		unsigned int uCC:2;
		unsigned int uILC:2;
		unsigned int uIntCode:16;
		unsigned int bProblemState:1;
		unsigned int bWaitState:1;
		unsigned int bMachineCheckMask:1;
		unsigned int bASCIIMode:1;
		unsigned int bProtKey:4;
		unsigned int SystemMask:8;
	};


	/**
	 * @brief IRIG-B compatible time frame
	 * 
	 * Format used for transmitting GMT and MET over serial lines in
	 * the shuttle. One such frame requires 1 second to be transmitted,
	 * so millisecond timing comes from other time sources, such as the 
	 * pulses used for transmitting this frame.
	 * It takes half a second to send to actual time in 5 pulse coded bytes
	 * The other 5 bytes of the time are not used but get transmitted anyway.
	 */
	struct IRIGB_FRAME {
		unsigned int uSeconds:7;
		unsigned int uMinutes:7;
		unsigned int uHours:6;
		unsigned int uDays:9;
		unsigned char uP5:8;
		unsigned char uP6:8;
		unsigned char uP7:8;
		unsigned char uP8:8;
		unsigned char uP9:8;
	};


	struct GPC_TIME_RECORD {
		unsigned int uSeconds:7;
		unsigned int uMinutes:7;
		unsigned int uHours:6;
		unsigned int uDays:9;
	};

	/*
	typedef struct {
		unsigned char ucGPCID;
		unsigned short ucMajorFunction;
		short sOPS;
		short sSPEC;
		short sDISP;
		GPC_TIME_RECORD gpc_time;
		GPC_TIME_RECORD crt_time;
		short sDataPage;
		long lData[16];
	} DEU_COMMAND;
	*/

	/*
	typedef struct {
		unsigned char ucMajorFunction;
		unsigned long ulKeyMap;
		unsigned short usDataSize;
		long lData[32];
	} DEU_DATA;
	*/

	struct BUS_COMMAND_WORD {
		unsigned int word_count:5;
		union {
			struct{
				unsigned int channel_addr:5;
				unsigned int module_addr:4;
			};
			unsigned int prom_addr:9;
		};
		unsigned int mode_ctrl:5;
		unsigned int mdm_no:5;
		unsigned int _spare_:8;
	};

	class IConnectedToBus {
	public:
		virtual void busCommandPhase(BusController* biu) = 0;
		virtual void busReadPhase(BusController* biu) = 0;
		virtual BUS_COMMAND_WORD busCommand(BusTerminal* biu, BUS_COMMAND_WORD cw, 
			unsigned long num_data, word16 *cdw) = 0;
	};	

	
	typedef union {
		float32 fl32;
		word32 dword;
		word16 word[2];
		BUS_COMMAND_WORD cw;
	} MEMORY_WORD;

	inline BUS_COMMAND_WORD _CW(unsigned int mdm, unsigned int mode_control, unsigned int data, unsigned int word_count) {
		BUS_COMMAND_WORD t;
		t.mdm_no = mdm;
		t.mode_ctrl = mode_control;
		t.prom_addr = data;
		t.word_count = (word_count-1);
		return t;
	};




};
