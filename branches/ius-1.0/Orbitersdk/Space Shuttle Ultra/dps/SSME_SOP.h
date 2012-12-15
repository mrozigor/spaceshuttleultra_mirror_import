/****************************************************************************
  This file is part of Space Shuttle Ultra

  Space Shuttle Main Engine Subsystem Operating Program definition



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
#ifndef _dps_SSME_SOP_H_
#define _dps_SSME_SOP_H_


#include "SimpleGPCSoftware.h"


namespace dps
{
	// commands
	const unsigned short LOXD = 0xA400;
	const unsigned short XPOW = 0xAC00;
	const unsigned short STEN = 0xC000;
	const unsigned short IGNT = 0xC400;
	const unsigned short SDEN = 0xC800;
	const unsigned short STDN = 0xCC00;
	const unsigned short TMSQ = 0xD000;
	const unsigned short THRT = 0xD400;
	const unsigned short ENLS = 0xE000;
	const unsigned short INLS = 0xE400;

	class SSME_SOP:public SimpleGPCSoftware
	{
		private:
			unsigned short LastCommand[3];

			// command flags
			bool StartEnableCommand[3];
			bool EngineStartCommand[3];
			bool ShutdownEnableCommand[3];
			bool ShutdownCommand[3];
			bool Throttle[3];
			bool OxidizerDumpStart[3];
			bool DumpStop[3];
			bool MECOCommand;

			bool MECOConfirmed;

			bool ShutdownEnableCommandIssued[3];

			// phase/mode flags
			//bool EngineReadyMode[3]
			bool ShutdownPhase[3];
			bool PostShutdownPhase[3];

			double CommandedThrottle;

			// status flags
			bool DataPathFailure[3];
			//bool CommandPathFailure[3];

			double PercentChamberPress[3];


			unsigned short pridata[3][32];
			unsigned short secdata[3][6];

			bool PrimaryDataFail[3];
			bool SecondaryDataFail[3];

			double LOXDumptimeA;
			double LOXDumptimeB;
		public:
			SSME_SOP( SimpleGPCSystem* _gpc );
			~SSME_SOP( void );

			void OnPreStep( double SimT, double DeltaT, double MJD );// read data
			void OnPostStep( double SimT, double DeltaT, double MJD );// send commands

			bool OnParseLine( const char* keyword, const char* value );
			void OnSaveState( FILEHANDLE scn ) const;

			void Realize( void );

			bool OnMajorModeChange( unsigned int newMajorMode );

			void ProcessPriData( int eng );
			void ProcessSecData( int eng );

			void SetStartEnableCommandFlag( int eng );
			void SetEngineStartCommandFlag( int eng );
			void SetShutdownEnableCommandFlag( int eng );
			void SetShutdownCommandFlag( int eng );
			void SetThrottlePercent( double pct );
			void SetMECOCommandFlag( void );
			/*void SetOxidizerDumpStartFlag( int eng );
			void SetDumpStopFlag( int eng );*/

			bool GetMECOCommandFlag( void ) const;
			bool GetMECOConfirmedFlag( void ) const;
			bool GetShutdownEnableCommandIssuedFlag( int eng ) const;
			bool GetShutdownPhaseFlag( int eng ) const;
			bool GetPostShutdownPhaseFlag( int eng ) const;
			double GetPercentChamberPressVal( int eng ) const;
	};
}


#endif// _dps_SSME_SOP_H_