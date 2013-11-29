/****************************************************************************
  This file is part of Space Shuttle Ultra

  Engine Interface Unit definition



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
#ifndef _mps_EIU_H_
#define _mps_EIU_H_


#include <orbitersdk.h>
#include "..\AtlantisSubsystem.h"
#include "..\dps\dps_defs.h"
#include "..\dps\BIU.h"
#include "DiscInPort.h"


//#define _EIU_DATA_RECORDER


namespace mps
{
	class SSME;

	class EIU:public AtlantisSubsystem, public dps::IConnectedToBus
	{
		private:
			int ID;
			SSME* eng;
			DiscInPort* ACchA;
			DiscInPort* ACchB;

			unsigned short DataPri[32];
			unsigned short DataSec[6];
#ifdef _EIU_DATA_RECORDER
			FILE* fp;

			void DataRecorder( unsigned short* data, char* type );
#endif// _EIU_DATA_RECORDER
		public:
			dps::BusTerminal mia[4];

			EIU( AtlantisSubsystemDirector* _director, const string& _ident, int ID, SSME* eng );
			~EIU( void );

			virtual void busCommandPhase( dps::BusController* biu );
			virtual void busReadPhase( dps::BusController* biu );
			virtual dps::BUS_COMMAND_WORD busCommand( dps::BusTerminal* biu, dps::BUS_COMMAND_WORD cw, unsigned long num_data, dps::word16 *cdw );

			/**
			 * Used to connect DiscInPort
			 * @sa AtlantisSubsystem::Realize
			 */
			void Realize( void );

			void OnPostStep( double, double, double );

			void CIA( int num, unsigned short* data );

			// HACK
			void readpri( unsigned short* data );
			void readsec( unsigned short* data );
			void command( unsigned short cmd );
	};
}


#endif// _mps_EIU_H_
