#ifndef __SSUPAD_H
#define __SSUPAD_H
#pragma once

#include "orbitersdk.h"

static const char* DEFAULT_MESHNAME_FSS="SSU/LC39A_FSS";
static const char* DEFAULT_MESHNAME_RSS="SSU/LC39A_RSS";

const double ORBITER_ACCESS_ARM_RATE = 0.005263;
const double GVA_RATE = 0.023810;
const double VENT_HOOD_RATE = 0.04166667;

const VECTOR3 FSS_POS_GOXVENTL		= _V(-8.895552, 78.85047, 20.18538);
const VECTOR3 FSS_POS_GOXVENTR		= _V(-8.895552, 78.85047, 22.48279);
const VECTOR3 FSS_POS_GOXVENTDIR	= _V(-9.469907,  80.14687, 20.18538);

class SSUPad: public VESSEL2
{
public:
	SSUPad(OBJHANDLE hVessel, int flightmodel);
	~SSUPad();

	void clbkPreStep(double simt, double simdt, double mjd);
	void clbkSetClassCaps(FILEHANDLE cfg);
	void clbkSaveState(FILEHANDLE scn);
	void clbkLoadStateEx(FILEHANDLE scn, void *status);
	int clbkConsumeBufferedKey(DWORD key, bool down, char *keystate);

	//communication with LCC
	virtual void MoveOrbiterAccessArm(AnimState::Action action);
	virtual void MoveGOXArm(AnimState::Action action);
	virtual AnimState::Action GetAccessArmState();
	virtual AnimState::Action GetGOXArmState();
private:
	void DefineAnimations();
	void GOXArmSequence();
	void CreateGOXVentThrusters();
	void UpdateGOXVentThrusters();

	PROPELLANT_HANDLE phGOXVent;
	THRUSTER_HANDLE thGOXVent[2];

	MESHHANDLE mesh;
	UINT mesh_idx;

	//animations; 0.0, CLOSED corresponds to state at t0
	UINT anim_accessarm;
	UINT anim_gva, anim_venthood;

	//Vertex positions for the GN2/GOX vents and reference for direction
	VECTOR3 vtx_goxvent[3];

	AnimState AccessArmState, GVAState, VentHoodState;
	AnimState::Action GOXArmAction;

	inline bool Eq(const double d1, const double d2, double dDiff=0.00001)
	{
		if(fabs(d1-d2)>dDiff) return false;
		return true;
	}
};

#endif //__SSUPAD_H
