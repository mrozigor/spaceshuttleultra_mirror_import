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


namespace mps
{
	// interrupts (BLOCK I): 17 external (+2?) (12 SV failure)
	const int INT_PFI = 1;// power failure
	const int INT_PRI = 2;// power recovery
	const int INT_PBDI = 3;// power bus down (opposite)
	const int INT_CCVSVAFI = 4;// CCV SV chA failure
	const int INT_CCVSVBFI = 5;// CCV SV chB failure
	const int INT_MFVSVAFI = 6;// MFV SV chA failure
	const int INT_MFVSVBFI = 7;// MFV SV chB failure
	const int INT_MOVSVAFI = 8;// MOV SV chA failure
	const int INT_MOVSVBFI = 9;// MOV SV chB failure
	const int INT_FPOVSVAFI = 10;// FPOV SV chA failure
	const int INT_FPOVSVBFI = 11;// FPOV SV chB failure
	const int INT_OPOVSVAFI = 12;// OPOV SV chA failure
	const int INT_OPOVSVBFI = 13;// OPOV SV chB failure
	const int INT_CMD_RCVD = 14;// command received


	class SSMEController;
	class PowerSupplyElectronics;

	class DigitalComputerUnit
	{
		friend class ComputerInterfaceElectronics;
		friend class SSMEControllerSW;
		friend class SSMEControllerSW_AD08;

		protected:
			int ch;
			int ramsize;
			PowerSupplyElectronics* PSE;
			ComputerInterfaceElectronics* CIE;
			SSMEController* Controller;
			SSMEControllerSW* sw;
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

			void Realize( void );

			virtual void tmestp( double time, double tmestp ) = 0;

			unsigned short DIO_in( unsigned short device );
			void DIO_out( unsigned short device, unsigned short data );

			void DMA_write( unsigned short selectcontrol, unsigned short writeaddress, unsigned short writelength );
			void DMA_read( unsigned short selectcontrol, unsigned short readaddress, unsigned short readlength );

			void PowerFailureSense( void );
			void Interrupt( int num );
	};
}


#endif// _mps_DCU_H_
