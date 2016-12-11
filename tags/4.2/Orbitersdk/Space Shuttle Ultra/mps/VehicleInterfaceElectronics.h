/****************************************************************************
  This file is part of Space Shuttle Ultra

  Vehicle Interface Electronics definition



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
#ifndef _mps_VIE_H_
#define _mps_VIE_H_


#include <orbitersdk.h>


namespace mps
{
	class SSMEController;
	class PowerSupplyElectronics;
	class ComputerInterfaceElectronics;

	class VehicleInterfaceElectronics
	{
		protected:
			SSMEController* Controller;
			PowerSupplyElectronics* PSE[2];
			ComputerInterfaceElectronics* CIE[2];

			short Command[3];

			bool VehicleDataSwitch;// true -> chA | false -> chB
			bool WDTstate;// for use in Vehicle Data Switch

		public:
			VehicleInterfaceElectronics( SSMEController* Controller );
			virtual ~VehicleInterfaceElectronics( void );

			void OnSaveState( FILEHANDLE scn ) const;
			bool OnParseLine( const char* line );

			// for use by the derived class
			virtual void __OnSaveState( FILEHANDLE scn ) const = 0;
			virtual bool __OnParseLine( const char* line ) = 0;

			void Realize( void );

			virtual void tmestp( double time ) = 0;

			void CommandDataConverter_write( int ch, unsigned short cmd );// cmds from EIU
			unsigned short CommandDataConverter_read( int ch ) const;// cmds to DCUs

			void RecorderDataConverter_chA( unsigned short* data, int ch );
			void RecorderDataConverter_chB( unsigned short* data, int ch );

			virtual void SwitchVRC( void ) = 0;
			virtual void RestoreVRC( void ) = 0;
	};
}


#endif// _mps_VIE_H_
