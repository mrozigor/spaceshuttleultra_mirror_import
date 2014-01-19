/****************************************************************************
  This file is part of Space Shuttle Ultra

  Output Electronics definition



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
#ifndef _mps_OE_H_
#define _mps_OE_H_


#include <orbitersdk.h>
#include "SSMEController.h"


namespace mps
{
	class SSME;
	class ComputerInterfaceElectronics;

	class OutputElectronics
	{
		protected:
			int ch;
			SSMEController* Controller;
			SSME* eng;
			double time;

			unsigned short StorageRegister;
			unsigned short ONOFFCommandRegister[2];// only 12 MSBs used

			double SH[5];
			double POS[5];

		public:
			OutputElectronics( int ch, SSME* eng, SSMEController* Controller );
			virtual ~OutputElectronics( void );

			void OnSaveState( FILEHANDLE scn ) const;
			bool OnParseLine( const char* line );

			// for use by the derived class
			virtual void __OnSaveState( FILEHANDLE scn ) const = 0;
			virtual bool __OnParseLine( const char* line ) = 0;

			virtual void tmestp( double time ) = 0;

			virtual void StorageRegister_write( unsigned short data, int ch ) = 0;
			virtual unsigned short StorageRegister_read( void ) const = 0;
			virtual unsigned short ONOFFCommandRegister_read( int num ) const = 0;
			virtual void CommandDecoder( void ) = 0;

			virtual void GetSH( double* data ) = 0;
			virtual void GetPOS( double* data ) = 0;
	};
}


#endif// _mps_OE_H_
