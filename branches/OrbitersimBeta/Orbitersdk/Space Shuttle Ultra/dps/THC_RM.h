/****************************************************************************
  This file is part of Space Shuttle Ultra

  Translational Hand Controller Redundancy Management definition



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
#ifndef _dps_THC_RM_H_
#define _dps_THC_RM_H_


#include "SimpleGPCSoftware.h"
#include "discsignals.h"


namespace dps
{
	/**
	 * @brief	Implementation of the THC RM software that runs in the GPCs.
	 * 
	 * This class handles the RM of THC signals.
	 */
	class THC_RM:public SimpleGPCSoftware
	{
		private:
			DiscInPort LeftTHC[18];
			DiscInPort AftTHC[18];

			bool THC_L_pX;
			bool THC_L_mX;
			bool THC_L_pY;
			bool THC_L_mY;
			bool THC_L_pZ;
			bool THC_L_mZ;
			bool THC_A_pX;
			bool THC_A_mX;
			bool THC_A_pY;
			bool THC_A_mY;
			bool THC_A_pZ;
			bool THC_A_mZ;

		public:
			THC_RM( SimpleGPCSystem* _gpc );
			~THC_RM( void );

			void Realize( void );

			void OnPostStep( double SimT, double DeltaT, double MJD );

			bool OnParseLine( const char* keyword, const char* value );
			void OnSaveState( FILEHANDLE scn ) const;

			bool OnMajorModeChange( unsigned int newMajorMode );

			void GetTHCData_L( bool &px, bool &mx, bool &py, bool &my, bool &pz, bool &mz ) const;
			void GetTHCData_A( bool &px, bool &mx, bool &py, bool &my, bool &pz, bool &mz ) const;
	};
}


#endif// _dps_THC_RM_H_