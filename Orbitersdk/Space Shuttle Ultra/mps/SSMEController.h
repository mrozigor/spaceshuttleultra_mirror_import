/****************************************************************************
  This file is part of Space Shuttle Ultra

  Space Shuttle Main Engine Controller definition



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
#ifndef _mps_SSMECONTROLLER_H_
#define _mps_SSMECONTROLLER_H_


#include <orbitersdk.h>
#include "DiscreteBundle.h"


namespace mps
{
	class InputElectronics;
	class SSME;
	class EIU;

	using namespace discsignals;


	class SSMEController
	{
		friend class OutputElectronics;
		friend class InputElectronics;
		friend class InputElectronics_BLOCK_II;
		friend class ComputerInterfaceElectronics;
		friend class ComputerInterfaceElectronics_BLOCK_II;
		friend class DigitalComputerUnit;
		friend class VehicleInterfaceElectronics;
		friend class PowerSupplyElectronics;

		private:
			SSME* eng;
			EIU* pEIU;
		protected:
			VehicleInterfaceElectronics* VIE;
			PowerSupplyElectronics* PSE[2];
			InputElectronics* IE[2];
			OutputElectronics* OE[2];
			DigitalComputerUnit* DCU[2];
			ComputerInterfaceElectronics* CIE[2];

		public:
			SSMEController( SSME* eng );
			virtual ~SSMEController( void );

			void __OnSaveState( FILEHANDLE scn ) const;
			bool __OnParseLine( const char* line );

			void Realize( DiscreteBundle* power, DiscreteBundle* OEout, DiscreteBundle* OEoutCCV, DiscreteBundle* OEoutMFV, DiscreteBundle* OEoutMOV, DiscreteBundle* OEoutFPOV, DiscreteBundle* OEoutOPOV, DiscreteBundle* IEchA_Press, DiscreteBundle* IEchB_Press, DiscreteBundle* IEchA_Temp, DiscreteBundle* IEchB_Temp, DiscreteBundle* IEchA_Flow, DiscreteBundle* IEchB_Flow, DiscreteBundle* IEchA_Speed, DiscreteBundle* IEchB_Speed );

			void GetEIURef( EIU* pEIU );

			virtual void tmestp( double time, double tmestp ) = 0;
			
			void VIE_CommandDataConverter_write( int ch, unsigned short cmd );

			void EIU_CIA( int num, unsigned short* data );
	};
}


#endif// _mps_SSMECONTROLLER_H_
