/****************************************************************************
  This file is part of Space Shuttle Ultra

  ET Umbilical Door Component



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
#ifndef _ETUMBDOORSYSTEM_2DC6F882_ECF3_4c68_B922_62BB3F035441
#define _ETUMBDOORSYSTEM_2DC6F882_ECF3_4c68_B922_62BB3F035441
#pragma once

#include "Atlantis.h"
#include "AtlantisSubsystem.h"
#include "discsignals.h"

using namespace discsignals;

const double ET_UMB_DOOR_OPERATING_SPEED = 0.0416667;
const double LATCH_OPERATING_SPEED = 0.166666667;

class ETUmbDoorSystem : public AtlantisSubsystem
{
public:
	ETUmbDoorSystem(AtlantisSubsystemDirector* _director);
	virtual ~ETUmbDoorSystem();

	virtual void Realize();
	virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	virtual void OnPostStep(double SimT, double DeltaT, double MJD);
	virtual void OnPropagate(double SimT, double DeltaT, double MJD);

	virtual void OnSaveState(FILEHANDLE scn) const;
	virtual bool OnParseLine(const char* keyword, const char* line);
private:
	DiscInPort LUmbDoorClose, LUmbDoorOpen, LUmbDoorLatch, LUmbDoorRelease;
	DiscInPort RUmbDoorClose, RUmbDoorOpen, RUmbDoorLatch, RUmbDoorRelease;
	DiscInPort CenterlineRelease;

	DiscOutPort LUmbDoorClosed, LUmbDoorOpened, LUmbDoorLatched, LUmbDoorReleased;
	DiscOutPort RUmbDoorClosed, RUmbDoorOpened, RUmbDoorLatched, RUmbDoorReleased;
	DiscOutPort CenterlineReleased;

	// at some point replace these with MechActuators
	double LDoorPos[2], RDoorPos[2];
	double LLatchPos[2], RLatchPos[2], CLLatchPos[2];
};

#endif //_ETUMBDOORSYSTEM_2DC6F882_ECF3_4c68_B922_62BB3F035441