/****************************************************************************
  This file is part of Space Shuttle Ultra

  Digital Computer Unit definition



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
#ifndef _mps_DCU_H_
#define _mps_DCU_H_


#include <string>
#include <orbitersdk.h>
#include "SSMEController.h"
#include "SSMEControllerSW_AD08.h"


namespace mps
{
	class SSMEControllerSW;
	class ComputerInterfaceElectronics;

	class DigitalComputerUnit
	{
		friend class ComputerInterfaceElectronics;
		friend class SSMEControllerSW;
		friend class SSMEControllerSW_AD08;

		protected:
			int ch;
			int ramsize;
			SSMEControllerSW* sw;
			SSMEController* Controller;
			ComputerInterfaceElectronics* CIE;
			unsigned short* RAM;

			double time;
			double dt;

			void (SSMEControllerSW::*funct)( void );

			virtual void PROM( void ) = 0;
		public:
			DigitalComputerUnit( int ch, int ramsize, const std::string& sw, SSMEController* Controller );
			virtual ~DigitalComputerUnit( void );

			void OnSaveState( FILEHANDLE scn ) const;
			bool OnParseLine( const char* line );

			// for use by the derived class
			virtual void __OnSaveState( FILEHANDLE scn ) const = 0;
			virtual bool __OnParseLine( const char* line ) = 0;

			void GiveRefs( ComputerInterfaceElectronics* CIE );

			virtual void tmestp( double time, double tmestp ) = 0;

			unsigned short DIO_in( unsigned short device );
			void DIO_out( unsigned short device, unsigned short data );

			void DMA_write( unsigned short selectcontrol, unsigned short writeaddress, unsigned short writelength );
			void DMA_read( unsigned short selectcontrol, unsigned short readaddress, unsigned short readlength );

			void PowerFailureSense( void );
	};
}


#endif// _mps_DCU_H_
