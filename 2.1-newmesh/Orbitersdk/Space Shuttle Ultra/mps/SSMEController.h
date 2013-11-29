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


namespace mps
{
	const int chA = 0;
	const int chB = 1;
	const int chC = 2;

	class VehicleInterfaceElectronics;
	class PowerSupplyElectronics;
	class InputElectronics;
	class OutputElectronics;
	class DigitalComputerUnit;
	class ComputerInterfaceElectronics;
	class SSME;
	class EIU;


	class SSMEController
	{
		friend class OutputElectronics_BLOCK_II;
		friend class InputElectronics_BLOCK_II;
		friend class ComputerInterfaceElectronics_BLOCK_II;

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

			void Realize( void );

			void GetEIURef( EIU* pEIU );

			virtual void tmestp( double time, double tmestp ) = 0;

			// "tunnel functions" between controller components
			bool PSE_Power( int ch );

			bool CIE_CheckWDTOwn( int CIEch, int nWDT );

			void DCU_PowerFailureSense( int DCUch );

			void OE_GetSH( int OEch, double* data );
			void OE_GetPOS( int OEch, double* data );
			void OE_StorageRegister_write( int OEch, unsigned short data, int ch );
			unsigned short OE_StorageRegister_read( int OEch );
			unsigned short OE_ONOFFCommandRegister_read( int OEch, int num );

			void VIE_CommandDataConverter_write( int ch, unsigned short cmd );
			unsigned short VIE_CommandDataConverter_read( int ch );
			void VIE_RecorderDataConverter_chA( unsigned short* data, int ch );
			void VIE_RecorderDataConverter_chB( unsigned short* data, int ch );
			void VIE_SwitchVRC( void );
			void VIE_RestoreVRC( void );

			void EIU_CIA( int num, unsigned short* data );

			void PCA( void );
	};
}


#endif// _mps_SSMECONTROLLER_H_
