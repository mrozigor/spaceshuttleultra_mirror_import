/****************************************************************************
  This file is part of Space Shuttle Ultra

  Ascent Thrust Vector Control subsystem definition



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

#ifndef _gnc_ATVC_H_
#define _gnc_ATVC_H_


#include "..\AtlantisSubsystem.h"
#include "discsignals.h"


using namespace discsignals;


namespace gnc
{
	class ATVC:public AtlantisSubsystem
	{
		private:
			short ID;
		public:
			DiscInPort dipPpos[3];
			DiscInPort dipYpos[3];
			DiscInPort dipRpos[2];
			DiscInPort dipTpos[2];

			double Ppos[3];
			double Ypos[3];
			double Rpos[2];
			double Tpos[2];

			ATVC( AtlantisSubsystemDirector* _director, const string& _ident, short nID );
			~ATVC( void );

			void Realize( void );

			void OnPostStep( double fSimT, double fDeltaT, double fMJD );
	};
}


#endif// _gnc_ATVC_H_
