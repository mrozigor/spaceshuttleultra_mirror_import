#ifndef __SSUPAD_H
#define __SSUPAD_H
#pragma once

#include "orbitersdk.h"

static const char* DEFAULT_MESHNAME_FSS="SSU/LC39A_FSS";
static const char* DEFAULT_MESHNAME_RSS="SSU/LC39A_RSS";

const double ORBITER_ACCESS_ARM_RATE = 0.005263;
const double GVA_RATE = 0.023810;
const double VENT_HOOD_RATE = 0.04166667;
const double RSS_Y_OWP_RATE = 0.01666666667;
const double FSS_Y_OWP_RATE = 0.01666666667;
const double RSS_RATE = 0.00066666667;

const double FSS_OWP_BRACKET_LENGTH = 12.987; //used for OWP strut animation
const double FSS_OWP_STRUT_LENGTH = 18.044;
const double FSS_OWP_STRUT_OFFSET = 13.465;

const unsigned int FSS_NUM_LIGHTS = 44;

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
	virtual AnimState::Action GetAccessArmState() const;
	virtual AnimState::Action GetGOXArmState() const;
private:
	void DefineAnimations();
	void GOXArmSequence();
	void CreateGOXVentThrusters();
	void UpdateGOXVentThrusters();

	PROPELLANT_HANDLE phGOXVent;
	THRUSTER_HANDLE thGOXVent[2];

	bool fLightsOn;
	BEACONLIGHTSPEC lights[FSS_NUM_LIGHTS];

	MESHHANDLE FSSMesh, RSSMesh;
	UINT fss_mesh_idx, rss_mesh_idx;

	//animations; 0.0, CLOSED corresponds to state at t0
	UINT anim_accessarm;
	UINT anim_gva, anim_venthood;
	UINT anim_rss_y_owp;
	UINT anim_fss_y_owp, anim_fss_y_owp_strut;
	UINT anim_rss; //NOTE: OPEN(1.0) corresponds to t0 state

	//Vertex positions for the GN2/GOX vents and reference for direction
	VECTOR3 vtx_goxvent[3];

	double fNextLightUpdate;

	AnimState AccessArmState, GVAState, VentHoodState;
	AnimState RSS_Y_OWP_State, FSS_Y_OWP_State;
	AnimState RSS_State;
	AnimState::Action GOXArmAction;

	inline bool Eq(const double d1, const double d2, double dDiff=0.00001)
	{
		if(fabs(d1-d2)>dDiff) return false;
		return true;
	}

	void CreateLights();
	void EnableLights();
	void DisableLights();
	bool IsDawn() const;
};

#endif //__SSUPAD_H

