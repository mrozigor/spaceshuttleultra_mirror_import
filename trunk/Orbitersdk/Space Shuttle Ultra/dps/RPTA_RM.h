/****************************************************************************
  This file is part of Space Shuttle Ultra

  Rudder Pedal Transducer Assembly Redundancy Management definition



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
#ifndef _dps_RPTA_RM_H_
#define _dps_RPTA_RM_H_


#include "SimpleGPCSoftware.h"
#include "discsignals.h"


namespace dps
{
	/**
	 * @brief	Implementation of the RPTA RM software that runs in the GPCs.
	 * 
	 * This class handles the RM of RPTA signals.
	 */
	class RPTA_RM:public SimpleGPCSoftware
	{
		private:
			DiscInPort LeftRPTA[3];
			DiscInPort RightRPTA[3];

			double RPTA_L;
			double RPTA_R;

			bool RPTA_L_DG;
			bool RPTA_R_DG;

		public:
			RPTA_RM( SimpleGPCSystem* _gpc );
			~RPTA_RM( void );

			void Realize( void );

			void OnPostStep( double SimT, double DeltaT, double MJD );

			bool OnParseLine( const char* keyword, const char* value );
			void OnSaveState( FILEHANDLE scn ) const;

			bool OnMajorModeChange( unsigned int newMajorMode );

			void GetRPTAData_L( double &rpta, bool &DG ) const;
			void GetRPTAData_R( double &rpta, bool &DG ) const;
	};
}


#endif// _dps_RPTA_RM_H_