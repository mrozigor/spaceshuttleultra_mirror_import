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


#include "..\Atlantis.h"
#include "..\AtlantisSubsystem.h"
#include <DiscInPort.h>


namespace comm
{
	class GCIL:public AtlantisSubsystem
	{
			discsignals::DiscInPort pControl_Command;
			discsignals::DiscInPort pSteeringMode_GPC;
			discsignals::DiscInPort pSteeringMode_GPCDesig;
			discsignals::DiscInPort pSteeringMode_AutoTrack;
			discsignals::DiscInPort pSteeringMode_ManSlew;
			discsignals::DiscInPort pSearch;
			discsignals::DiscInPort pPower_Off;
			discsignals::DiscInPort pPower_On;
			discsignals::DiscInPort pMode_COMM;
			discsignals::DiscInPort pMode_RDRCOOP;

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