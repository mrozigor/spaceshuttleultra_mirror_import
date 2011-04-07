#ifndef __SSUPAD_H
#define __SSUPAD_H
#pragma once

#include "orbitersdk.h"
//#include "../Atlantis.h"

static const char* DEFAULT_MESHNAME_FSS="SSU/LC39A_FSS_1985";
static const char* DEFAULT_MESHNAME_RSS="SSU/LC39A_RSS";
static const char* DEFAULT_MESHNAME_HARDSTAND="SSU/LC39A_hardstand";
static const char* DEFAULT_MESHNAME_WATERTOWER="SSU/LC-39_watertower";

const double TANK_MAX_PROPELLANT_MASS = 719115.0;

const double ORBITER_ACCESS_ARM_RATE = 0.005263;
const double GVA_RATE = 0.023810;
const double VENT_HOOD_RATE = 0.04166667;
const double RSS_OWP_RATE = 0.002381;
const double RSS_RATE = 0.00066666667;
const double FSS_GH2_ARM_RATE = 2.0;
const double FSS_IAA_RATE = 1.0/200.0;
const double FSS_RBUS_RATE = 2.0;


const unsigned int FSS_NUM_LIGHTS = 1;
const unsigned int STADIUM_LIGHT_COUNT = 5;

const VECTOR3 FSS_POS_GOXVENTL		= _V(-8.895552, 78.30047, 20.00000);
const VECTOR3 FSS_POS_GOXVENTR		= _V(-8.895552, 78.30047, 22.25000);
const VECTOR3 FSS_POS_GOXVENTDIR	= _V(-9.469907,  80.14687, 20.18538);

const int RSS_ROTATE_SOUND = 1;
static const char* RSS_ROTATE_SOUND_FILE = "Sound\\ShuttleUltra\\RSS_Rotation.wav";

class Atlantis;

class SSUPad: public VESSEL3
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
	void MoveGH2Pipe(AnimState::Action action);
	void MoveVentHood(AnimState::Action action);
	void MoveLOXArm(AnimState::Action action);
	virtual AnimState::Action GetAccessArmState() const;
	virtual AnimState::Action GetGOXArmState() const;
	void MoveRSS_OWP(AnimState::Action action);
	void MoveRSS(AnimState::Action action);
	void DeployIAA();
	void HaltIAA();
	void RetractIAA();
	void MoveRBUS(AnimState::Action action);
	
private:
	void DefineAnimations();
	void GOXArmSequence();
	void CreateGOXVentThrusters();
	void UpdateGOXVentThrusters();

	PROPELLANT_HANDLE phGOXVent;
	THRUSTER_HANDLE thGOXVent[2];

	bool fLightsOn;
	BEACONLIGHTSPEC lights[FSS_NUM_LIGHTS];

	MESHHANDLE FSSMesh, RSSMesh, HardStandMesh, WaterTowerMesh;
	UINT fss_mesh_idx, rss_mesh_idx, hs_mesh_idx, wt_mesh_idx;

	//animations; 0.0, CLOSED corresponds to state at t0
	UINT anim_accessarm;
	UINT anim_gva, anim_venthood;
	UINT anim_rss_y_owp;
	UINT anim_iaa;
	UINT anim_rss; //NOTE: OPEN(1.0) corresponds to t0 state
	UINT anim_fss_gh2_ventarm; //NOTE: CLOSED (0.0) corresponds to arm attached to ET
	UINT anim_fss_rbus;


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
	
	AnimState RSS_OWP_State;
	AnimState RSS_State;
	AnimState FSS_GH2_VentArmState;
	AnimState::Action GOXArmAction;
	AnimState FSS_RBUS_UmbilicalState;

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

	PROPELLANT_HANDLE phLights; // fake tank for lights
	THRUSTER_HANDLE thStadiumLights[STADIUM_LIGHT_COUNT];
	LightEmitter* pStadiumLights[STADIUM_LIGHT_COUNT];

	std::string ShuttleName;
	Atlantis* pSTS;
	
};

#endif //__SSUPAD_H

