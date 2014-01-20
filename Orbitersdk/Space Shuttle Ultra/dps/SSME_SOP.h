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
	const unsigned short MPL = 67;
	const unsigned short FPL = 109;

	// commands
	const unsigned short SVRC = 0x2000;
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

	const unsigned short DATA_FAIL = 4;// I-LOAD


	class SSME_SOP:public SimpleGPCSoftware
	{
		private:
			double PercentChamberPress[3];
			unsigned short Phase[3];
			unsigned short Mode[3];
			unsigned short SelfTestStatus[3];
			unsigned short CommandStatus[3];

			// TODO KMIN, KMAX, KCMD
			// command flags
			bool StartEnableCommand[3];
			bool EngineStartCommand[3];
			bool ShutdownEnableCommand[3];
			bool ShutdownCommand[3];
			bool ThrottleCommand[3];
			bool OxidizerDumpStartCommand[3];
			bool DumpStopCommand[3];
			bool LimitInhibitCommand[3];
			bool LimitEnableCommand[3];
			bool DCUSwitchVDTCommand[3];

			unsigned short LastCommand[3];

			bool ShutdownEnableCommandIssued[3];// TODO have a StartEnableCommandIssued????

			double CommandedThrottle;

			// phase/mode flags
			bool ShutdownPhase[3];
			bool PostShutdownPhase[3];
			bool HydraulicLockupMode[3];
			bool ElectricalLockupMode[3];
			bool EngineReadyMode[3];

			// status flags
			bool PadDataPathFailure[3];
			bool FlightDataPathFailure[3];
			bool CommandPathFailure[3];
			bool MajorComponentFailure[3];
			bool LimitExceeded[3];

			unsigned short pridata[3][32];
			unsigned short secdata[3][6];

			bool PrimaryDataFail[3];
			bool SecondaryDataFail[3];
			bool DCUProcess[3];

			unsigned short PrimaryFailCounter[3];
			unsigned short SecondaryFailCounter[3];
			unsigned short DataFailCounter[3];

		public:
			SSME_SOP( SimpleGPCSystem* _gpc );
			~SSME_SOP( void );

			void OnPreStep( double SimT, double DeltaT, double MJD );// read data
			void OnPostStep( double SimT, double DeltaT, double MJD );// send commands

			bool OnMajorModeChange( unsigned int newMajorMode );

			void ProcessPriData( int eng );
			void ProcessSecData( int eng );

			/**
			 * Causes the Start Enable Command to be issued for a SSME.
			 * @param[in]	eng	SSME number
			 */
			void SetStartEnableCommandFlag( int eng );

			/**
			 * Causes the Engine Start Command to be issued for a SSME.
			 * @param[in]	eng	SSME number
			 */
			void SetEngineStartCommandFlag( int eng );

			/**
			 * Causes the Shutdown Enable Command to be issued for a SSME.
			 * @param[in]	eng	SSME number
			 */
			void SetShutdownEnableCommandFlag( int eng );

			/**
			 * Causes the Shutdown Command to be issued for a SSME.
			 * @param[in]	eng	SSME number
			 */
			void SetShutdownCommandFlag( int eng );

			/**
			 * Causes a Throttle Command to be issued to all SSMEs.
			 * @param[in]	pct	new throttle setting in percent of RPL
			 * @return		true = valid value
			 */
			bool SetThrottlePercent( double pct );

			/**
			 * Causes the Limit Inhibit Command to be issued for a SSME.
			 * @param[in]	eng	SSME number
			 */
			void SetLimitInhibitCommandFlag( int eng );

			/**
			 * Causes the Limit Enable Command to be issued for a SSME.
			 * @param[in]	eng	SSME number
			 */
			void SetLimitEnableCommandFlag( int eng );

			void SetOxidizerDumpStartCommandFlag( int eng );
			void SetDumpStopCommandFlag( int eng );

			/**
			 * Returns an indication of whether the Shutdown Enable Command has been issued for a SSME.
			 * @param[in]	eng	SSME number
			 * @return		true = command issued
			 */
			bool GetShutdownEnableCommandIssuedFlag( int eng ) const;

			/**
			 * Returns an indication of whether the phase in effect in a SSME is Shutdown.
			 * @param[in]	eng	SSME number
			 * @return		true = Shutdown Phase
			 */
			bool GetShutdownPhaseFlag( int eng ) const;

			/**
			 * Returns an indication of whether the phase in effect in a SSME is Post-Shutdown.
			 * @param[in]	eng	SSME number
			 * @return		true = Post-Shutdown Phase
			 */
			bool GetPostShutdownPhaseFlag( int eng ) const;

			/**
			 * Returns an indication of whether the operating mode in a SSME is Hydraulic Lockup.
			 * @param[in]	eng	SSME number
			 * @return		true = Hydraulic Lockup Mode
			 */
			bool GetHydraulicLockupModeFlag( int eng ) const;

			/**
			 * Returns an indication of whether the operating mode in a SSME is Electrical Lockup.
			 * @param[in]	eng	SSME number
			 * @return		true = Electrical Lockup Mode
			 */
			bool GetElectricalLockupModeFlag( int eng ) const;

			/**
			 * Returns an indication of whether the operating mode in a SSME is Engine Ready.
			 * @param[in]	eng	SSME number
			 * @return		true = Engine Ready Mode
			 */
			bool GetEngineReadyModeFlag( int eng ) const;

			/**
			 * Returns an indication of whether a Pad Data Path Failure has occurred for a SSME.
			 * @param[in]	eng	SSME number
			 * @return		true = Pad Data Path Failure
			 */
			bool GetPadDataPathFailureFlag( int eng ) const;

			/**
			 * Returns an indication of whether a Flight Data Path Failure has occurred for a SSME.
			 * @param[in]	eng	SSME number
			 * @return		true = Flight Data Path Failure
			 */
			bool GetFlightDataPathFailureFlag( int eng ) const;

			/**
			 * Returns an indication of whether a Command Path Failure has occurred for a SSME.
			 * @param[in]	eng	SSME number
			 * @return		true = Command Path Failure
			 */
			bool GetCommandPathFailureFlag( int eng ) const;

			/**
			 * Returns an indication of whether a MCF has been posted on a SSME.
			 * @param[in]	eng	SSME number
			 * @return		true = MCF
			 */
			bool GetMajorComponentFailureFlag( int eng ) const;

			/**
			 * Returns an indication of whether limits have been exceeded on a SSME.
			 * @param[in]	eng	SSME number
			 * @return		true = limits exceeded
			 */
			bool GetLimitExceededFlag( int eng ) const;

			/**
			 * Gets the chamber pressure in percent of RPL of a SSME.
			 * @param[in]	eng	SSME number
			 * @return		Chamber pressure in percent of RPL
			 */
			double GetPercentChamberPressVal( int eng ) const;
	};
}


#endif// _dps_SSME_SOP_H_