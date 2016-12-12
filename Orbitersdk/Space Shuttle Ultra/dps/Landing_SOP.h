/****************************************************************************
  This file is part of Space Shuttle Ultra

  Landing Subsystem Operating Program definition



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
#ifndef _dps_LANDING_SOP_H_
#define _dps_LANDING_SOP_H_


#include "SimpleGPCSoftware.h"
#include "discsignals.h"


namespace dps
{
	class AerojetDAP;

	/**
	 * @brief	Implementation of the Landing SOP software that runs in the GPCs.
	 * 
	 * This class generates signals associated with the landing.
	 */
	class Landing_SOP:public SimpleGPCSoftware
	{
		private:
			AerojetDAP* pAerojetDAP;

			DiscInPort SRBSEPSW;
			DiscInPort SRBSEPPB;
			DiscInPort ETSEPSW;
			DiscInPort ETSEPPB;

			bool WOWLON;
			bool FLATTURN;
			bool WONG;
			bool GSENBL;

		public:
			Landing_SOP( SimpleGPCSystem* _gpc );
			~Landing_SOP( void );

			void Realize( void );

			void OnPostStep( double SimT, double DeltaT, double MJD );

			bool OnParseLine( const char* keyword, const char* value );
			void OnSaveState( FILEHANDLE scn ) const;

			bool OnMajorModeChange( unsigned int newMajorMode );

			bool GetWOWLON( void ) const;
			bool GetFLATTURN( void ) const;
			bool GetWONG( void ) const;
			bool GetGSENBL( void ) const;
	};
}


#endif// _dps_LANDING_SOP_H_