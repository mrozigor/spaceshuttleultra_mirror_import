/****************************************************************************
  This file is part of Space Shuttle Ultra

  Computer Interface Electronics definition



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
#ifndef _mps_CIE_H_
#define _mps_CIE_H_


#include <orbitersdk.h>


namespace mps
{
	// must be the same values as in SW

	const unsigned short DIO_DEV_INPUT = 0x0000;// 0xxx xxxx xxxx xxxx
	const unsigned short DIO_DEV_OUTPUT = 0x8000;// 1xxx xxxx xxxx xxxx

	const unsigned short DIO_DEV_OEchA = 0x0000;// x000 xxxx xxxx xxxx
	const unsigned short DIO_DEV_OEchB = 0x1000;// x001 xxxx xxxx xxxx
	const unsigned short DIO_DEV_VIE = 0x2000;// x010 xxxx xxxx xxxx

	const unsigned short DIO_DEV_OE_STORAGE_REG = 0x0000;// xxxx 0000 xxxx xxxx
	const unsigned short DIO_DEV_OE_ONOFF_REG_1 = 0x0100;// xxxx 0001 xxxx xxxx
	const unsigned short DIO_DEV_OE_ONOFF_REG_2 = 0x0200;// xxxx 0010 xxxx xxxx

	const unsigned short DIO_DEV_VIE_CMD1 = 0x0000;// xxxx 0000 xxxx xxxx
	const unsigned short DIO_DEV_VIE_CMD2 = 0x0100;// xxxx 0001 xxxx xxxx
	const unsigned short DIO_DEV_VIE_CMD3 = 0x0200;// xxxx 0010 xxxx xxxx

	class DigitalComputerUnit;
	class InputElectronics;
	class SSMEController;

	class ComputerInterfaceElectronics
	{
		protected:
			int ch;
			DigitalComputerUnit* DCU;
			InputElectronics* IE[2];
			ComputerInterfaceElectronics* CIEOpposite;
			SSMEController* Controller;

			double dWDT[2];
			bool bWDT[2];// timed out

			double time;
			double dt;

		public:
			ComputerInterfaceElectronics( int ch, SSMEController* Controller );
			virtual ~ComputerInterfaceElectronics( void );

			void OnSaveState( FILEHANDLE scn ) const;
			bool OnParseLine( const char* line );

			// for use by the derived class
			virtual void __OnSaveState( FILEHANDLE scn ) const = 0;
			virtual bool __OnParseLine( const char* line ) = 0;

			void tmestp( double time, double dt );

			void GiveRefs( DigitalComputerUnit* DCU, InputElectronics* IEchA, InputElectronics* IEchB, ComputerInterfaceElectronics* CIEOpposite );

			void DMA_ch1( unsigned short selectcontrol, unsigned short readaddress, unsigned short readlength );
			void DMA_ch2( unsigned short selectcontrol, unsigned short readaddress, unsigned short readlength );
			void DMA_ch3( unsigned short selectcontrol, unsigned short writeaddress, unsigned short writelength );

			unsigned short DIO( unsigned short control, unsigned short data );

			void InitializeWDT( int nWDT );
			void ResetWDT( int nWDT );
			bool CheckWDTOwn( int nWDT );
			bool CheckWDTOpposite( int nWDT );

			void SwitchVRC( void );
			void RestoreVRC( void );
	};

}


#endif// _mps_CIE_H_
