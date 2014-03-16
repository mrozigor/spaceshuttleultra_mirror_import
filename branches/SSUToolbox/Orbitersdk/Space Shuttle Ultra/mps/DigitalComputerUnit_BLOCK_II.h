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
