#ifndef __SSUCHUTE_H
#define __SSUCHUTE_H

#pragma once

#include "orbitersdk.h"

const double CHUTE_DEFLATE_TIME = 0.4;
// chute will deflate in 2.5 seconds

class SSUChute: public VESSEL2
{
public:
	SSUChute(OBJHANDLE hVessel, int flightmodel);
	~SSUChute();

	void DefineAnimations();

	void clbkPreStep(double simt, double simdt, double mjd);
	void clbkSaveState(FILEHANDLE scn);
	void clbkLoadStateEx(FILEHANDLE scn, void *status);
private:
	MESHHANDLE mesh;
	UINT mesh_idx;
	UINT anim_collapse;
	double anim_collapse_state;
};

#endif __SSUCHUTE_H
