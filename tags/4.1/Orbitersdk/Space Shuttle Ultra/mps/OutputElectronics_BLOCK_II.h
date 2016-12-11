/****************************************************************************
  This file is part of Space Shuttle Ultra

  Output Electronics Block II definition



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
#ifndef _mps_OE_BLOCK_II_H_
#define _mps_OE_BLOCK_II_H_


#include "OutputElectronics.h"


namespace mps
{
	// command decoder
	const unsigned short CD_ONOFF_1 = 0x0001;
	const unsigned short CD_ONOFF_2 = 0x0002;
	const unsigned short CD_CCV = 0x0003;
	const unsigned short CD_MFV = 0x0004;
	const unsigned short CD_MOV = 0x0005;
	const unsigned short CD_FPOV = 0x0006;
	const unsigned short CD_OPOV = 0x0007;

	// ON/OFF 1
	const unsigned short SV_HPOTPISPURGE = 0x0010;
	const unsigned short SV_EMERGENCYSHUTDOWN = 0x0020;
	const unsigned short SV_SHUTDOWNPURGE = 0x0040;
	const unsigned short SV_FUELSYSTEMPURGE = 0x0100;
	const unsigned short SV_BLEEDVALVESCONTROL = 0x0200;
	const unsigned short AFV = 0x0400;
	const unsigned short HPV = 0x0800;
	const unsigned short IGNITER_MCC = 0x2000;
	const unsigned short IGNITER_OPB = 0x4000;
	const unsigned short IGNITER_FPB = 0x8000;

	// ON/OFF 2
	const unsigned short SS_CCV_FO = 0x0010;
	const unsigned short SS_MFV_FO = 0x0020;
	const unsigned short SS_MOV_FO = 0x0040;
	const unsigned short SS_FPOV_FO = 0x0080;
	const unsigned short SS_OPOV_FO = 0x0100;
	const unsigned short SS_CCV_FS = 0x0200;
	const unsigned short SS_MFV_FS = 0x0400;
	const unsigned short SS_MOV_FS = 0x0800;
	const unsigned short SS_FPOV_FS = 0x1000;
	const unsigned short SS_OPOV_FS = 0x2000;


	class SSME;
	class SSMEController;


	class OutputElectronics_BLOCK_II:public OutputElectronics
	{
		public:
			OutputElectronics_BLOCK_II( int ch, SSME* eng, SSMEController* Controller );
			~OutputElectronics_BLOCK_II( void );

			void __OnSaveState( FILEHANDLE scn ) const;
			bool __OnParseLine( const char* line );

			void tmestp( double time, double tmestp );

			void StorageRegister_write( unsigned short data, int ch );
			unsigned short StorageRegister_read( void ) const;
			unsigned short ONOFFCommandRegister_read( int num ) const;
			void CommandDecoder( void );

			void GetSH( double* data );
			void GetPOS( double* data );
	};
}


#endif// _mps_OE_BLOCK_II_H_