/****************************************************************************
  This file is part of Space Shuttle Ultra

  Input Electronics definition



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
#ifndef _mps_IE_H_
#define _mps_IE_H_


#include <orbitersdk.h>
#include "DiscInPort.h"


namespace mps
{
	class PowerSupplyElectronics;
	class OutputElectronics;

	using class discsignals::DiscInPort;
	using class discsignals::DiscreteBundle;


	class InputElectronics
	{
		friend class SSMEController;

		protected:
			int ch;
			SSMEController* Controller;
			PowerSupplyElectronics* PSE;
			OutputElectronics* OE;

			// sensor input
			DiscInPort press[15];
			DiscInPort temp[8];
			DiscInPort speed[4];
			DiscInPort flow[3];
			double actpos[9];
			double samplehold[5];

		public:
			InputElectronics( int ch, SSMEController* Controller );
			virtual ~InputElectronics( void );

			void OnSaveState( FILEHANDLE scn ) const;
			bool OnParseLine( const char* line );

			// for use by the derived class
			virtual void __OnSaveState( FILEHANDLE scn ) const = 0;
			virtual bool __OnParseLine( const char* line ) = 0;

			void Realize( DiscreteBundle* Press, DiscreteBundle* Temp, DiscreteBundle* Flow, DiscreteBundle* Speed );

			virtual void tmestp( double time ) = 0;

			void GetData( unsigned short device, unsigned short* data );
	};
}


#endif// _mps_IE_H_
