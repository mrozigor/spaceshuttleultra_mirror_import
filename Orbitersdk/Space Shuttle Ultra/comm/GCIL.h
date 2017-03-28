/****************************************************************************
  This file is part of Space Shuttle Ultra

  Ground Command Interface Logic definition



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

#ifndef _comm_GCIL_H_
#define _comm_GCIL_H_


#include "..\AtlantisSubsystem.h"
#include <discsignals.h>


namespace comm
{
	class GCIL:public AtlantisSubsystem
	{
			DiscInPort pControl_Command;
			DiscInPort pSteeringMode_GPC;
			DiscInPort pSteeringMode_GPCDesig;
			DiscInPort pSteeringMode_AutoTrack;
			DiscInPort pSteeringMode_ManSlew;
			DiscInPort pSearch;
			DiscInPort pPower_Off;
			DiscInPort pPower_On;
			DiscInPort pMode_COMM;
			DiscInPort pMode_RDRCOOP;

			DiscOutPort COMM_ON;
			DiscOutPort RDR_STANDBY;
			DiscOutPort COMM_STANDBY;
			DiscOutPort RDR_ON;
			DiscOutPort ANT_STRG_GPC;
			DiscOutPort ANT_STRG_GPC_DESIG;
			DiscOutPort ANT_STRG_AUTO_TRK;
			DiscOutPort ANT_STRG_MAN_SLEW;

		public:
			GCIL( AtlantisSubsystemDirector* _direct );
			virtual ~GCIL();

			void Realize( void );
			void OnPreStep( double fSimT, double fDeltaT, double fMJD );
	};
};

#endif// _comm_GCIL_H_