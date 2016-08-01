/****************************************************************************
  This file is part of Space Shuttle Ultra

  Radar Altimeter subsystem definition



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

#ifndef _gnc_RA_H_
#define _gnc_RA_H_


#include "..\AtlantisSubsystem.h"
#include "discsignals.h"


// radar position (assumes both radars are in the same place, centered in the Y axis)
const double RADAR_POS_X = 11;
const double RADAR_POS_Z = -3.5;


using namespace discsignals;


namespace gnc
{
	class RadarAltimeter:public AtlantisSubsystem
	{
		private:
			int ID;
			DiscInPort power;

		public:
			RadarAltimeter( AtlantisSubsystemDirector* _director, const string& _ident, int ID );
			~RadarAltimeter( void );

			void Realize( void );

			void OnPostStep( double fSimT, double fDeltaT, double fMJD );
	};
}


#endif// _gnc_RA_H_
