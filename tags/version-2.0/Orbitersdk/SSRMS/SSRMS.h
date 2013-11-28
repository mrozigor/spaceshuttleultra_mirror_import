#ifndef __SSRMS_H
#define __SSRMS_H
#pragma once

#include "orbitersdk.h"
#include "Subsystem.h"
#include "SubsystemDirector.h"
#include "LEESystem.h"
#include <cmath>

static const char* ATTACH_ID = "GS";
//static const char* ATTACH_ID = "A";

const double JOINT_ROTATION_SPEED = 1.0; // rotation speed for single joint ops (deg/s)
const double JOINT_SPEED_LIMIT = 4.0; // maximum possible joint rotation speed (deg/s) - at the moment, this number is just a guess
const double EE_ROTATION_SPEED = RAD*1.0; // rad/s
const double EE_TRANSLATION_SPEED = 0.1048; // 1 fps

// joint positions
const VECTOR3 SR_JOINT = _V(0.70, 0.59, -8.44); // coincides with LEE position
const VECTOR3 SY_JOINT = _V(0.69, 0.59, -7.47);
const VECTOR3 SP_JOINT = _V(0.0, 0.00, -7.47);
const VECTOR3 EP_JOINT = _V(0.0, 0.0, 0.0);
const VECTOR3 WP_JOINT = _V(0.0, 0.0, 7.47);
const VECTOR3 WY_JOINT = _V(-0.69, 0.59, 7.47);
const VECTOR3 LEE_POS = _V(-0.70 ,0.59, 8.44); // coincides with WR joint position

/*const unsigned short SHOULDER_ROLL[] =	{6, 0};
const unsigned short SHOULDER_YAW[] =	{5, 1};
const unsigned short SHOULDER_PITCH[] =	{4, 2};
const unsigned short ELBOW_PITCH[] =	{3, 3};
const unsigned short WRIST_PITCH[] =	{2, 4};
const unsigned short WRIST_YAW[] =		{1, 5};
const unsigned short WRIST_ROLL[] =		{0, 6};*/

const double SR_SY_DIST = length(SY_JOINT-SR_JOINT);
// distance (metres) from SR joint to SY joint
const double SY_SP_VERT_DIST = SY_JOINT.y-SP_JOINT.y;
// distance (metres) from SY joint to SP joint in Orbiter Y axis
const double SP_EP_DIST = length(EP_JOINT-SP_JOINT);
// distance (metres) from SP joint to EP joint
const double EP_WP_DIST = length(WP_JOINT-EP_JOINT);
// distance (metres) from EP joint to WP joint
//const double WP_WY_DIST = length(WY_JOINT-WP_JOINT);
const double WP_WY_DIST = WY_JOINT.y-WP_JOINT.y;
// distance (metres) from WP joint to WY joint
const double WY_EE_DIST = length(WY_JOINT-LEE_POS);
// distance (metres) from WY joint to EE
const double LEE_OFFSET = SY_JOINT.x-LEE_POS.x;
// horizontal offset between the LEE and the reference point

const double JOINT_LIMITS[2] = {-280.0, +280.0};
const double JOINT_SOFTSTOPS[2] = {-270.0, +270.0}; // from ISS ROBO Console Handbook, 1.2-29 (p. 58)

enum FRAME{EE_FRAME, BASE_FRAME};

class SSRMS: public VESSEL2
{
public:
	typedef SubsystemDirector<SSRMS> SSRMSSubsystemDirector;
	typedef enum {SHOULDER_ROLL=0, SHOULDER_YAW, SHOULDER_PITCH, ELBOW_PITCH, WRIST_PITCH, WRIST_YAW, WRIST_ROLL} SSRMS_JOINT;

	SSRMS(OBJHANDLE hObj, int fmodel);
	~SSRMS();

	//callback functions
	void clbkLoadStateEx(FILEHANDLE scn, void *vs);
	void clbkSaveState(FILEHANDLE scn);
	void clbkPreStep(double simt, double simdt, double mjd);
	void clbkPostStep(double SimT, double SimDT, double MJD);
	void clbkDrawHUD (int mode, const HUDPAINTSPEC *hps, HDC hDC);
	void clbkPostCreation();
	int  clbkConsumeBufferedKey(DWORD key, bool down, char *kstate);
private:
	void DefineAnimations();
	void DefineThrusters();

	//SSRMS functions
	bool MoveEE(const VECTOR3 &newPos, const VECTOR3 &newDir, const VECTOR3 &newRot, double DeltaT);
	/**
	 * Sets joint angle
	 * @parameter angle - angle in degrees
	 * @returns - true if angle could be set, false otherwise
	 */
	bool SetJointAngle(SSRMS_JOINT joint, double angle);
	//bool SetJointAngle(unsigned short joint, double angle);
	/**
	 * Tries to change which LEE is used as base and which LEE is active
	 * @returns - true is active LEE was changed; false otherwise
	 */
	bool ChangeActiveLEE();
	void CalculateVectors();

private:
	SSRMSSubsystemDirector* pSubsystemDirector;
	LEESystem* pLEE[2];

	VECTOR3 arm_tip[3];
	VECTOR3 arm_ee_pos, arm_ee_dir, arm_ee_rot, arm_ee_angles;
	double joint_angle[7]; // angles in degrees
	bool arm_moved, update_angles, update_vectors;

	int activeLEE, passiveLEE; // either 0 or 1
	short joint_motion[7]; // 0=stationary, -1=negative, +1=positive
	unsigned short SpeedFactor;
	FRAME RefFrame;

	VECTOR3 old_arm_tip; // used to shift meshes so active LEE is at centre of external view

	int OrbiterSoundHandle;

	//mesh handles
	MESHHANDLE hSSRMSMesh;
	UINT mesh_ssrms;
	UINT anim_joint[2][7], anim_lee;
	//UINT anim_joint[7], anim_lee;

	bool ShowAttachmentPoints;

	THRUSTER_HANDLE rms_control[12];
	PROPELLANT_HANDLE ph_null;

	//ATTACHMENTHANDLE ahBase, ahGrapple;
	//LatchSystem* pLEE[2];
};

#endif // !__SSRMS_H
