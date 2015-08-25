/****************************************************************************
  This file is part of Space Shuttle Ultra

  Power Supply Electronics definition



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
#ifndef _mps_PSE_H_
#define _mps_PSE_H_


#include "DiscInPort.h"
#include <orbitersdk.h>


namespace mps
{
	class SSMEController;
	class DigitalComputerUnit;
	class ComputerInterfaceElectronics;

	using class discsignals::DiscInPort;
	using class discsignals::DiscreteBundle;

	class PowerSupplyElectronics
	{
		protected:
			int ch;
			SSMEController* Controller;
			DigitalComputerUnit* DCU;
			ComputerInterfaceElectronics* CIEOpposite;

			bool PowerOn;
		public:
			DiscInPort AC;

			PowerSupplyElectronics( int ch, SSMEController* Controller );
			virtual ~PowerSupplyElectronics( void );

			void tmestp( double time );

			void OnSaveState( FILEHANDLE scn ) const;
			bool OnParseLine( const char* line );

			// for use by the derived class
			virtual void __OnSaveState( FILEHANDLE scn ) const = 0;
			virtual bool __OnParseLine( const char* line ) = 0;

			void Realize( void );

			void ConnectPower( DiscreteBundle* AC );

			bool Power( void ) const;
	};
}


#endif// _mps_PSE_H_
