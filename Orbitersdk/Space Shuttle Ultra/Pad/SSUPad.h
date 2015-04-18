#ifndef __SSUPAD_H
#define __SSUPAD_H
#pragma once

#include "orbitersdk.h"
#include "../BaseSSUPad.h"
//#include "../Atlantis.h"

static const char* DEFAULT_MESHNAME_FSS="SSU/LC39A_FSS";
static const char* DEFAULT_MESHNAME_RSS="SSU/LC39A_RSS";
static const char* DEFAULT_MESHNAME_FSS_1985="SSU/LC39A_FSS_1985";
static const char* DEFAULT_MESHNAME_RSS_1985="SSU/LC39A_RSS_1985";
static const char* DEFAULT_MESHNAME_HARDSTAND="SSU/LC39A_hardstand";
static const char* DEFAULT_MESHNAME_WATERTOWER="SSU/LC-39_watertower";

const double TANK_MAX_PROPELLANT_MASS = 719115.0;

const double ORBITER_ACCESS_ARM_RATE_NORMAL = 0.007692;// 130sec
const double ORBITER_ACCESS_ARM_RATE_EMERGENCY = 0.066666;// 15sec
const double GVA_RATE = 0.03;
const double VENT_HOOD_RATE = 0.04166667;
const double RSS_OWP_RATE = 0.002381;
const double FSS_OWP_RATE = 0.005;
const double RSS_RATE = 0.00066666667;
const double FSS_GH2_ARM_RATE = 0.5;
const double FSS_IAA_RATE = 1.0/200.0;
const double FSS_RBUS_RATE = 0.35;
const double SRB_SFD_RATE = 0.00333333;// 300sec

//FSS OWP strut animation constants
const double FSS_OWP_BRACKET_LENGTH = 12.4;
const double FSS_OWP_STRUT_LENGTH = 18.02;
const double FSS_OWP_STRUT_OFFSET = 13.427;
const double FSS_OWP_STRUT_NULL_ANGLE = 88.5; //angle in degrees

const unsigned int FSS_NUM_LIGHTS = 1;
const unsigned int LC39_LIGHT_COUNT = 5;

//const VECTOR3 FSS_POS_GOXVENTL		= _V(-8.895552, 78.30047, 20.00000);
//const VECTOR3 FSS_POS_GOXVENTR		= _V(-8.895552, 78.30047, 22.25000);
const VECTOR3 FSS_POS_GOXVENTL		= _V(-8.5, 78.2, 19.7); //North duct
const VECTOR3 FSS_POS_GOXVENTR		= _V(-8.5, 78.2, 22.0); //South duct
const VECTOR3 FSS_POS_GOXVENTL_1985	= _V(-8.5, 78, 20.5); //North duct
const VECTOR3 FSS_POS_GOXVENTR_1985	= _V(-8.5, 78, 23.0); //South duct
//const VECTOR3 FSS_POS_GOXVENTL		= _V(-8.3, 78.30047, 24);
//const VECTOR3 FSS_POS_GOXVENTR		= _V(-7.9, 78.30047, 26.5);
const VECTOR3 FSS_POS_GOXVENTDIR	= _V(-9.469907,  80.14687, 20.18538);

const int RSS_ROTATE_SOUND = 1;
static const char* RSS_ROTATE_SOUND_FILE = "RSS_Rotation.wav";
const int CRYO_HISS = 2;
static const char* CRYO_HISS_SOUND_FILE = "Cryo_noise.wav";

class Atlantis;

class SSUPad: public BaseSSUPad
{
	friend BOOL CALLBACK SSUPad_DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
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

	void MoveRSS_OWP(AnimState::Action action);
	void MoveFSS_OWP(AnimState::Action action);
	void MoveRSS(AnimState::Action action);
	void MoveRBUS(AnimState::Action action);
	void MoveEastSRBSFD( AnimState::Action action );
	void MoveWestSRBSFD( AnimState::Action action );
	
private:
	void DefineAnimations();
	//void DefineAnimations();
	//void DefineAnimations1985();
	//void GOXArmSequence();
	void AnimateFSSOWPStrut();
	void CreateGOXVentThrusters();
	virtual void UpdateGOXVents();

	bool bPad1985;

	PROPELLANT_HANDLE phGOXVent;
	THRUSTER_HANDLE thGOXVent[2];

	bool fLightsOn;
	BEACONLIGHTSPEC lights[FSS_NUM_LIGHTS];

	MESHHANDLE FSSMesh, RSSMesh, HardStandMesh, WaterTowerMesh;
	UINT fss_mesh_idx, rss_mesh_idx, hs_mesh_idx, wt_mesh_idx;

	//animations; 0.0, CLOSED corresponds to state at t0
	UINT anim_rss_y_owp;
	UINT anim_fss_y_owp, anim_fss_y_owp_strut;
	UINT anim_rss; //NOTE: OPEN(1.0) corresponds to t0 state
	UINT anim_fss_rbus;
	UINT anim_East_SRB_SFD;
	UINT anim_West_SRB_SFD;

	//Vertex positions for the GN2/GOX vents and reference for direction
	VECTOR3 vtx_goxvent[3];

	double fNextLightUpdate;

	//AnimState AccessArmState, GVAState, VentHoodState;
	/**
	 * Intertank Access Arm animation state
	 * 0 = retracted
	 * 1 = deployed
	 */
	//AnimState IAA_State;
	
	AnimState RSS_OWP_State, FSS_OWP_State;
	AnimState RSS_State;
	//AnimState FSS_GH2_VentArmState;
	//AnimState::Action GOXArmAction;
	AnimState FSS_RBUS_UmbilicalState;
	AnimState East_SRB_SFD_State;
	AnimState West_SRB_SFD_State;

	/**
	 * Creates MGROUP_ROTATE struct, adds it to animation list, and returns pointer to struct
	 */
	/*MGROUP_ROTATE* DefineRotation(UINT mesh, UINT *grp, UINT ngrp, const VECTOR3 &ref, const VECTOR3 &axis, float angle)
	{
		MGROUP_ROTATE* mgrp = new MGROUP_ROTATE(mesh, grp, ngrp, ref, axis, angle);
		vpAnimations.push_back(mgrp);
		return mgrp;
	}
	inline bool Eq(const double d1, const double d2, double dDiff=0.00001)
	{
		if(fabs(d1-d2)>dDiff) return false;
		return true;
	}*/

	//void CreateLights();
	//void EnableLights();
	//void DisableLights();
	//bool IsNight() const;

	int SoundID;

	PROPELLANT_HANDLE phLights; // fake tank for lights
	THRUSTER_HANDLE thStadiumLights[LC39_LIGHT_COUNT];
	LightEmitter* pStadiumLights[LC39_LIGHT_COUNT];

	std::string ShuttleName;
	Atlantis* pSTS;
	
};

#endif //__SSUPAD_H

