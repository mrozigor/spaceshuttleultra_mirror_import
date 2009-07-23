#ifndef _ETUMBDOORSYSTEM_2DC6F882_ECF3_4c68_B922_62BB3F035441
#define _ETUMBDOORSYSTEM_2DC6F882_ECF3_4c68_B922_62BB3F035441
#pragma once

#include "Atlantis.h"
#include "AtlantisSubsystem.h"
#include "discsignals/discsignals.h"

using namespace discsignals;

const double ET_UMB_DOOR_OPERATING_SPEED = 0.0416667;
const double LATCH_OPERATING_SPEED = 0.166666667;

class ETUmbDoorSystem : public AtlantisSubsystem
{
public:
	ETUmbDoorSystem(SubsystemDirector* _director);
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
	DiscOutPort CenterlineLatched, CenterlineReleased;

	// at some point replace these with MechActuators
	double LDoorPos[2], RDoorPos[2];
	double LLatchPos[2], RLatchPos[2], CLLatchPos[2];
};

#endif //_ETUMBDOORSYSTEM_2DC6F882_ECF3_4c68_B922_62BB3F035441