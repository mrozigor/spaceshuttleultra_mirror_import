#ifndef __SSUPAD_H
#define __SSUPAD_H
#pragma once

#include "orbitersdk.h"

static const char* DEFAULT_MESHNAME_FSS="SSU/LC39A_FSS";
static const char* DEFAULT_MESHNAME_RSS="SSU/LC39A_RSS";
static const char* DEFAULT_MESHNAME_HARDSTAND="SSU/LC39A_hardstand";

const double ORBITER_ACCESS_ARM_RATE = 0.005263;
const double GVA_RATE = 0.023810;
const double VENT_HOOD_RATE = 0.04166667;
const double RSS_OWP_RATE = 0.002381;
const double FSS_OWP_RATE = 0.00128205;
const double RSS_RATE = 0.00066666667;
const double FSS_GH2_ARM_RATE = 2.0;
const double FSS_IAA_RATE = 1.0/200.0;

//FSS OWP strut animation constants
const double FSS_OWP_BRACKET_LENGTH = 12.212;
const double FSS_OWP_STRUT_LENGTH = 18.3203;
const double FSS_OWP_STRUT_OFFSET = 13.427;
const double FSS_OWP_STRUT_NULL_ANGLE = 86.197; //angle in degrees

const unsigned int FSS_NUM_LIGHTS = 44;

const VECTOR3 FSS_POS_GOXVENTL		= _V(-8.895552, 78.85047, 20.18538);
const VECTOR3 FSS_POS_GOXVENTR		= _V(-8.895552, 78.85047, 22.48279);
const VECTOR3 FSS_POS_GOXVENTDIR	= _V(-9.469907,  80.14687, 20.18538);

const int RSS_ROTATE_SOUND = 1;
static const char* RSS_ROTATE_SOUND_FILE = "Sound\\ShuttleUltra\\RSS_Rotation.wav";

class SSUPad: public VESSEL2
{
public:
	SSUPad(OBJHANDLE hVessel, int flightmodel);
	~SSUPad();

	void clbkPostCreation();
	void clbkPreStep(double simt, double simdt, double mjd);
	void clbkSetClassCaps(FILEHANDLE cfg);
	void clbkSaveState(FILEHANDLE scn);
	void clbkLoadStateEx(FILEHANDLE scn, void *status);
	int clbkConsumeBufferedKey(DWORD key, bool down, char *keystate);

	//communication with LCC
	virtual void OnT0();
	virtual void MoveOrbiterAccessArm(AnimState::Action action);
	virtual void MoveGOXArm(AnimState::Action action);
	virtual AnimState::Action GetAccessArmState() const;
	virtual AnimState::Action GetGOXArmState() const;
	void MoveRSS_OWP(AnimState::Action action);
	void MoveFSS_OWP(AnimState::Action action);
	void MoveRSS(AnimState::Action action);
	void DeployIAA();
	void HaltIAA();
	void RetractIAA();
private:
	void DefineAnimations();
	void GOXArmSequence();
	void AnimateFSSOWPStrut();
	void CreateGOXVentThrusters();
	void UpdateGOXVentThrusters();

	PROPELLANT_HANDLE phGOXVent;
	THRUSTER_HANDLE thGOXVent[2];

	bool fLightsOn;
	BEACONLIGHTSPEC lights[FSS_NUM_LIGHTS];

	MESHHANDLE FSSMesh, RSSMesh, HardStandMesh;
	UINT fss_mesh_idx, rss_mesh_idx, hs_mesh_idx;

	//animations; 0.0, CLOSED corresponds to state at t0
	UINT anim_accessarm;
	UINT anim_gva, anim_venthood;
	UINT anim_rss_y_owp;
	UINT anim_fss_y_owp, anim_fss_y_owp_strut;
	UINT anim_iaa;
	UINT anim_rss; //NOTE: OPEN(1.0) corresponds to t0 state
	UINT anim_fss_gh2_ventarm; //NOTE: CLOSED (0.0) corresponds to arm attached to ET

	//Vertex positions for the GN2/GOX vents and reference for direction
	VECTOR3 vtx_goxvent[3];

	double fNextLightUpdate;

	AnimState AccessArmState, GVAState, VentHoodState;
	/**
	 * Intertank Access Arm animation state
	 * 0 = retracted
	 * 1 = deployed
	 */
	AnimState IAA_State;
	
	AnimState RSS_OWP_State, FSS_OWP_State;
	AnimState RSS_State;
	AnimState FSS_GH2_VentArmState;
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

	int SoundID;
};

#endif //__SSUPAD_H

