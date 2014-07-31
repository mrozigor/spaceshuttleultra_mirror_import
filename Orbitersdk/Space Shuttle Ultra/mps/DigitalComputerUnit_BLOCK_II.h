/****************************************************************************
  This file is part of Space Shuttle Ultra

  Digital Computer Unit Block II definition



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
#ifndef _mps_DCU_BLOCK_II_H_
#define _mps_DCU_BLOCK_II_H_


#include "DigitalComputerUnit.h"


namespace mps
{
	const int RAMSIZE_DCU_BII = 65536;


	// Self-Test Status
	const unsigned short ESW_EngineOK = 1;
	const unsigned short ESW_MCF = 2;

	// Phase/Mode
	const unsigned short ESW_PROM = 0x0700;
	const unsigned short ESW_Standby = 0x0800;

	// Command Status
	const unsigned short ESW_NoCommand = 0x0000;
	const unsigned short ESW_CommandRejected_A = 0x0002;
	const unsigned short ESW_CommandRejected_B = 0x0004;
	const unsigned short ESW_Accepted = 0x0006;


	const int RAM_PROM_BII_CH = 0;
	const int RAM_PROM_BII_TREF = 1;

	const int RAM_PROM_BII_ESW = 3;

	const int RAM_PROM_BII_VALIDCMD = 5;
	const int RAM_PROM_BII_CMD1 = 6;
	const int RAM_PROM_BII_CMD2 = 7;
	const int RAM_PROM_BII_CMD3 = 8;

	const int RAM_PROM_BII_CURCMD = 9;
	const int RAM_PROM_BII_PRVCMD = 10;

	const int RAM_PROM_BII_DVDT_1 = 21;
	const int RAM_PROM_BII_DVDT_2 = 22;
	const int RAM_PROM_BII_DVDT_3 = 23;
	const int RAM_PROM_BII_DVDT_5 = 25;
	const int RAM_PROM_BII_DVDT_90 = 110;
	const int RAM_PROM_BII_DVDT_98 = 118;
	const int RAM_PROM_BII_DVDT_99 = 119;
	const int RAM_PROM_BII_DVDT_100 = 120;
	const int RAM_PROM_BII_DVDT_103 = 123;

	// commands
	const unsigned short NOP = 0x0000;
	const unsigned short SDCA = 0x0400;
	const unsigned short SDCB = 0x0800;
	const unsigned short EGND = 0x0C00;
	const unsigned short EFLT = 0x1000;
	const unsigned short EFRT = 0x1400;
	const unsigned short ETWO = 0x1800;
	const unsigned short RVRC = 0x1C00;
	const unsigned short SVRC = 0x2000;
	const unsigned short AFRT = 0x2400;
	const unsigned short XFRT = 0x2800;
	const unsigned short HYDC = 0x2C00;
	const unsigned short ERCK = 0x3000;
	const unsigned short RSYS = 0x3400;
	const unsigned short RESM = 0x3800;
	const unsigned short RSCT = 0x3C00;
	const unsigned short MLDA = 0x4000;
	const unsigned short MLDB = 0x4400;
	const unsigned short RSCA = 0x4800;
	const unsigned short RSCB = 0x4C00;
	const unsigned short HELA = 0x5000;
	const unsigned short HELB = 0x5400;
	const unsigned short PSCA = 0x5800;
	const unsigned short PSCB = 0x5C00;
	const unsigned short IOHA = 0x6000;
	const unsigned short IOHB = 0x6400;
	const unsigned short IOLA = 0x6800;
	const unsigned short IOLB = 0x6C00;
	const unsigned short IOSA = 0x7000;
	const unsigned short IOSB = 0x7400;
	const unsigned short ENFA = 0x7800;
	const unsigned short ENFB = 0x7C00;
	const unsigned short ROFA = 0x8000;
	const unsigned short ROFB = 0x8400;
	const unsigned short RWRA = 0x8800;
	const unsigned short RWRB = 0x8C00;
	const unsigned short RSKA = 0x9000;
	const unsigned short RSKB = 0x9400;
	const unsigned short EXPM = 0x9800;
	const unsigned short MRC1 = 0x9C00;
	const unsigned short MRC2 = 0xA000;
	const unsigned short LOXD = 0xA400;
	const unsigned short APOW = 0xA800;
	const unsigned short XPOW = 0xAC00;
	const unsigned short PSN1 = 0xB000;
	const unsigned short PSN2 = 0xB400;
	const unsigned short PSN3 = 0xB800;
	const unsigned short PSN4 = 0xBC00;
	const unsigned short STEN = 0xC000;
	const unsigned short IGNT = 0xC400;
	const unsigned short SDEN = 0xC800;
	const unsigned short STDN = 0xCC00;
	const unsigned short TMSQ = 0xD000;
	const unsigned short THRT = 0xD400;
	const unsigned short COSY = 0xD800;
	const unsigned short DAVL = 0xDC00;
	const unsigned short ENLS = 0xE000;
	const unsigned short INLS = 0xE400;
	const unsigned short INFA = 0xE800;
	const unsigned short INFB = 0xEC00;


	class DigitalComputerUnit_BLOCK_II:public DigitalComputerUnit
	{
		private:
			void PROM( void );
		public:
			DigitalComputerUnit_BLOCK_II( int ch, const std::string& sw, SSMEController* Controller );
			~DigitalComputerUnit_BLOCK_II( void );

			void __OnSaveState( FILEHANDLE scn ) const;
			bool __OnParseLine( const char* line );

			void tmestp( double time, double tmestp );
	};
}


#endif// _mps_DCU_BLOCK_II_H_
