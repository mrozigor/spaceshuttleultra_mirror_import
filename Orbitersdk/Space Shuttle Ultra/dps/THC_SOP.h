/****************************************************************************
  This file is part of Space Shuttle Ultra

  Translational Hand Controller Subsystem Operating Program definition



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
#ifndef _dps_THC_SOP_H_
#define _dps_THC_SOP_H_


#include "SimpleGPCSoftware.h"
#include "discsignals.h"


namespace dps
{
	class THC_RM;

	/**
	 * @brief	Implementation of the THC SOP software that runs in the GPCs.
	 * 
	 * This class receives and processes THC commands.
	 */
	class THC_SOP:public SimpleGPCSoftware
	{
		private:
			THC_RM* pTHC_RM;

			DiscInPort AftSense;

			int THC_X;
			int THC_Y;
			int THC_Z;

		public:
			THC_SOP( SimpleGPCSystem* _gpc );
			~THC_SOP( void );

			void Realize( void );

			void OnPostStep( double SimT, double DeltaT, double MJD );

			bool OnParseLine( const char* keyword, const char* value );
			void OnSaveState( FILEHANDLE scn ) const;

			bool OnMajorModeChange( unsigned int newMajorMode );

			int GetXCommand( void ) const;
			int GetYCommand( void ) const;
			int GetZCommand( void ) const;
	};
}


#endif// _dps_THC_SOP_H_