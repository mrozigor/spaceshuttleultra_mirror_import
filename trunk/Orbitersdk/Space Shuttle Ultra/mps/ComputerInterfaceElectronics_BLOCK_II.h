/****************************************************************************
  This file is part of Space Shuttle Ultra

  Computer Interface Electronics Block II definition



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
#ifndef _mps_CIE_BLOCK_II_H_
#define _mps_CIE_BLOCK_II_H_


#include "ComputerInterfaceElectronics.h"


namespace mps
{
	class SSMEController;

	class ComputerInterfaceElectronics_BLOCK_II:public ComputerInterfaceElectronics
	{
		public:
			ComputerInterfaceElectronics_BLOCK_II( int ch, SSMEController* Controller );
			~ComputerInterfaceElectronics_BLOCK_II( void );

			void __OnSaveState( FILEHANDLE scn ) const;
			bool __OnParseLine( const char* line );
	};

}


#endif// _mps_CIE_BLOCK_II_H_

// TODO FDR
/*
For failure analysis each Computer Interface Electronics contains a Failure
Data Recorder, (FDR), that continually records the address bus data, memory
bus data, and control line data of the DCU/CIE (Digital Computer Unit /
Computer Interface Electronics) monitor microprocessor.  The FDR consists
of a 2048 by 48-bit RAM, an address counter and control logic.  The FDR
memory can only be read by the cross channel DCU and it must be in PROM
with FDR recording inhibited.  It logs the last 68,000 bus cycles.  "A"
side reads "B" side memory, and "B" side reads "A" side memory for dump
requirements. 
*/