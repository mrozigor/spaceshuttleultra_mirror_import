#ifndef __SSULCC_H
#define __SSULCC_H
#pragma once

#include "orbitersdk.h"
#include "../Pad/SSUPad.h"

const double ACCESS_ARM_RETRACT_TIME = 450.0;
const double GOX_ARM_RETRACT_TIME = 150.0;

class SSULCC: public VESSEL2
{
public:
	SSULCC(OBJHANDLE hVessel, int flightmodel);
	~SSULCC();

	void clbkPostCreation();
	void clbkPreStep(double simt, double simdt, double mjd);
	void clbkSaveState(FILEHANDLE scn);
	void clbkLoadStateEx(FILEHANDLE scn, void *status);

	//communication with SSUPad class
private:
	double launch_mjd;
	double lastTTL;
	char PadName[256];

	SSUPad* pFSS;
};

#endif //__SSULCC_H
