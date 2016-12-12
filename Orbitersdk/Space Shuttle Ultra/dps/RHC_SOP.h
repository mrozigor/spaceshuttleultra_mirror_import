/****************************************************************************
  This file is part of Space Shuttle Ultra

  Rotational Hand Controller Subsystem Operating Program definition



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
#ifndef _dps_RHC_SOP_H_
#define _dps_RHC_SOP_H_


#include "SimpleGPCSoftware.h"
#include "discsignals.h"


namespace dps
{
	class RHC_RM;

	/**
	 * @brief	Implementation of the RHC SOP software that runs in the GPCs.
	 * 
	 * This class receives and processes RHC commands.
	 */
	class RHC_SOP:public SimpleGPCSoftware
	{
		private:
			RHC_RM* pRHC_RM;

			DiscInPort AftSense;

			bool FCS_MAN_TAKEOVER_PITCH;
			bool FCS_MAN_TAKEOVER_ROLL;
			bool FCS_MAN_TAKEOVER_YAW;

			bool DETENT_PITCH;
			bool DETENT_ROLL;
			bool DETENT_YAW;

			bool PAST_SOFTSTOP_PITCH;
			bool PAST_SOFTSTOP_ROLL;
			bool PAST_SOFTSTOP_YAW;

			double RHC_P;
			double RHC_R;
			double RHC_Y;

			void TwoAxis( void );
			void ThreeAxis( void );

		public:
			RHC_SOP( SimpleGPCSystem* _gpc );
			~RHC_SOP( void );

			void Realize( void );

			void OnPostStep( double SimT, double DeltaT, double MJD );

			bool OnParseLine( const char* keyword, const char* value );
			void OnSaveState( FILEHANDLE scn ) const;

			bool OnMajorModeChange( unsigned int newMajorMode );

			double GetPitchCommand( void ) const;
			double GetRollCommand( void ) const;
			double GetYawCommand( void ) const;

			bool GetPitchManTakeOver( void ) const;
			bool GetRollManTakeOver( void ) const;
			bool GetYawManTakeOver( void ) const;

			bool GetPitchDetent( void ) const;
			bool GetRollDetent( void ) const;
			bool GetYawDetent( void ) const;

			bool GetPitchPastSoftStop( void ) const;
			bool GetRollPastSoftStop( void ) const;
			bool GetYawPastSoftStop( void ) const;
	};
}


#endif// _dps_RHC_SOP_H_