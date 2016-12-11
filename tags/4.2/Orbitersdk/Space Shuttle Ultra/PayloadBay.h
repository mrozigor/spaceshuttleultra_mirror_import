/****************************************************************************
  This file is part of Space Shuttle Ultra

  PayloadBay System definition



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
#ifndef __PLB_H_
#define __PLB_H_


#include <orbitersdk.h>
#include "Atlantis.h"


using namespace discsignals;


class PayloadBay:public AtlantisSubsystem
{
	friend class Atlantis;

	private:
		DiscInPort PLBayDoorSYS_ENABLE[2];
		DiscInPort PLBayDoor_CLOSE;
		DiscInPort PLBayDoor_OPEN;
		DiscOutPort PLBayDoorTB_OP;
		DiscOutPort PLBayDoorTB_CL;

		DiscInPort PLBayMECHPWRSYS_ON[2];
		DiscInPort LatchControlSYS_LATCH[2];
		DiscInPort LatchControlSYS_RELEASE[2];
		DiscInPort RadiatorControlSYS_STOW[2];
		DiscInPort RadiatorControlSYS_DEPLOY[2];

		DiscOutPort LatchSTBDTB_LAT;
		DiscOutPort LatchSTBDTB_REL;
		DiscOutPort LatchPORTTB_LAT;
		DiscOutPort LatchPORTTB_REL;

		DiscOutPort RadiatorSTBDTB_STO;
		DiscOutPort RadiatorSTBDTB_DPY;
		DiscOutPort RadiatorPORTTB_STO;
		DiscOutPort RadiatorPORTTB_DPY;

		DiscInPort KUAntennaDirectStow_ON;
		DiscInPort KUAntenna_STO;
		DiscInPort KUAntenna_DPY;
		DiscOutPort KUAntennaTB_STO;
		DiscOutPort KUAntennaTB_DPY;

		DiscInPort BoomStowEnableI;
		DiscInPort BoomStowEnableII;

		// physical status
		AnimState BayDoorStatus;
		AnimState CLBayDoorLatch[4]; //1_4, 5_8, 9_12, 13_16
		AnimState RadiatorStatus[2];// left, right
		AnimState RadLatchStatus[2];// left, right
		AnimState KuAntennaStatus;

		bool KuRndz_Radar_Stow_Ind[2];
		bool KuRndz_Radar_Dpy_Ind[2];

		bool hasAntenna;

		void SetTalkbacks( void );
	public:
		PayloadBay( AtlantisSubsystemDirector* _director );
		~PayloadBay( void );

		virtual bool SingleParamParseLine() const {return true;};
		bool OnParseLine( const char* line );
		void OnSaveState( FILEHANDLE scn ) const;
		void Realize( void );
		virtual void OnPostStep( double fSimT, double fDeltaT, double fMJD );
};


#endif// __PLB_H_