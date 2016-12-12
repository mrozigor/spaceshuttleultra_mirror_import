/****************************************************************************
  This file is part of Space Shuttle Ultra

  Speedbrake/Thrust Controller Subsystem Operating Program definition



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
#ifndef _dps_SBTC_SOP_H_
#define _dps_SBTC_SOP_H_


#include "SimpleGPCSoftware.h"
#include "discsignals.h"


namespace dps
{
	class SBTC_RM;
	class SSME_SOP;
	class AscentDAP;
	class SSME_Operations;

	/**
	 * @brief	Implementation of the SBTC SOP software that runs in the GPCs.
	 * 
	 * This class receives and processes SBTC commands.
	 */
	class SBTC_SOP:public SimpleGPCSoftware
	{
		private:
			SBTC_RM* pSBTC_RM;
			SSME_SOP* pSSME_SOP;
			AscentDAP* pAscentDAP;
			SSME_Operations* pSSME_Operations;

			DiscInPort CDR_SPDBK_THROT;
			DiscInPort PLT_SPDBK_THROT;

			DiscOutPort CDR_SPDBK_THROT_AUTO_LT;
			DiscOutPort CDR_SPDBK_THROT_MAN_LT;
			DiscOutPort PLT_SPDBK_THROT_AUTO_LT;
			DiscOutPort PLT_SPDBK_THROT_MAN_LT;

			bool MAN_THROTTLE;
			bool MAN_THROTTLE_ACTIVE;
			double MAN_THROTTLE_COMMAND;

			bool CDR_TAKEOVER;
			bool PLT_TAKEOVER;
			double MAN_SPEEDBRAKE_COMMAND;
			bool CDR_DATA;

			void ManSpeedbrake( void );
			void ManThrottle( void );

		public:
			SBTC_SOP( SimpleGPCSystem* _gpc );
			~SBTC_SOP( void );

			void Realize( void );

			void OnPostStep( double SimT, double DeltaT, double MJD );

			bool OnParseLine( const char* keyword, const char* value );
			void OnSaveState( FILEHANDLE scn ) const;

			bool OnMajorModeChange( unsigned int newMajorMode );

			bool GetManThrottle( void ) const;
			double GetManThrottleCommand( void ) const;

			bool GetCDRTakeover( void ) const;
			bool GetPLTTakeover( void ) const;
			double GetManSpeedbrakeCommand( void ) const;
	};
}


#endif// _dps_SBTC_SOP_H_