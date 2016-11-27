/****************************************************************************
  This file is part of Space Shuttle Ultra

  Rudder Pedal Transducer Assembly Subsystem Operating Program definition



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
#ifndef _dps_RPTA_SOP_H_
#define _dps_RPTA_SOP_H_


#include "SimpleGPCSoftware.h"


namespace dps
{
	class RPTA_RM;

	/**
	 * @brief	Implementation of the RPTA SOP software that runs in the GPCs.
	 * 
	 * This class receives and processes RPTA commands.
	 */
	class RPTA_SOP:public SimpleGPCSoftware
	{
		private:
			RPTA_RM* pRPTA_RM;

			double RPTA;

		public:
			RPTA_SOP( SimpleGPCSystem* _gpc );
			~RPTA_SOP( void );

			void Realize( void );

			void OnPostStep( double SimT, double DeltaT, double MJD );

			bool OnParseLine( const char* keyword, const char* value );
			void OnSaveState( FILEHANDLE scn ) const;

			bool OnMajorModeChange( unsigned int newMajorMode );

			double GetYawCommand( void ) const;
	};
}


#endif// _dps_RPTA_SOP_H_