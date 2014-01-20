#ifndef __SSULCC_H
#define __SSULCC_H
#pragma once

#include "orbitersdk.h"
#include "../Atlantis.h"
#include "ISSULaunchTower.h"

const double MPS_HE_SUPPLY_START_TIME = 21600;// HACK T-6h, not sure
const double ACCESS_ARM_RETRACT_TIME = 450.0;
const double GOX_ARM_RETRACT_TIME = 150.0;
const double PSN4_TIME = 239;
const double RSLS_SEQUENCE_START_TIME = 31.0;
const double APU_CHECK_TIME = 245.0;
const double MPS_HE_SUPPLY_END_TIME = 12.5;

class SSULCC: public VESSEL2
{
public:
	SSULCC(OBJHANDLE hVessel, int flightmodel);
	~SSULCC();

	void clbkPostCreation();
	void clbkPreStep(double simt, double simdt, double mjd);
	void clbkSaveState(FILEHANDLE scn);
	void clbkLoadStateEx(FILEHANDLE scn, void *status);
private:
	double launch_mjd;
	double lastTTL;
	double MPSHeSupplyPressure1;
	double MPSHeSupplyPressure2;
	bool _firstrun;
	bool MPSHeSupply;
	char PadName[256], ShuttleName[256];

	ISSULaunchTower* pFSS;
	Atlantis* pSSU;
};

#endif //__SSULCC_H
