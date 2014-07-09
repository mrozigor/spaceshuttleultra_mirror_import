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
//const double JOINT_SPEED_LIMIT = 4.0; // maximum possible joint rotation speed (deg/s) - at the moment, this number is just a guess
const double EE_ROTATION_SPEED = RAD*1.0; // rad/s
const double EE_TRANSLATION_SPEED = 0.1048; // 1 fps

const double SSRMS_UNFOLD_SPEED = 1/10.0; // time for SSRMS unfolding animation

// joint positions
const VECTOR3 SR_JOINT = _V(0.826, 0.703, -8.605); // coincides with LEE position
const VECTOR3 SY_JOINT = _V(0.826, 0.703, -7.298);
const VECTOR3 SP_JOINT = _V(0.0,0.00,-7.298);
const VECTOR3 EP_JOINT = _V(0.0,0.00,0.0);
const VECTOR3 WP_JOINT = _V(0.0,0.00,7.299);
const VECTOR3 WY_JOINT = _V(-0.829,0.703,7.299);
const VECTOR3 LEE_POS = _V(-0.829,0.703,8.605); // coincides with WR joint position

const VECTOR3 LEE1_CAM_POS = _V(0.826, 0.985, -8.217);
const VECTOR3 LEE2_CAM_POS = _V(-0.826, 0.985, 8.217);

const VECTOR3 LEE1_LIGHT_POS = _V(0.828, 1.033, -8.217);
const VECTOR3 LEE2_LIGHT_POS = _V(-0.832, 1.033, 8.217);

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

const double CAM_PAN_TILT_RATE = 6.0; //rate in deg/s
const double CAM_PAN_MAX = 175.0; // deg
const double CAM_PAN_MIN = -175.0; // deg
const double CAM_TILT_MAX = 90.0; // deg
const double CAM_TILT_MIN = -92.0; // deg

class SSRMS: public VESSEL3
{
public:
	typedef SubsystemDirector<SSRMS> SSRMSSubsystemDirector;
	typedef enum {SHOULDER_ROLL=0, SHOULDER_YAW, SHOULDER_PITCH, ELBOW_PITCH, WRIST_PITCH, WRIST_YAW, WRIST_ROLL} SSRMS_JOINT;
	typedef enum FRAME{EE_FRAME, BASE_FRAME};
	typedef enum CAMERA_VIEW{ACTIVE_LEE, BOOM_A, BOOM_B};

	SSRMS(OBJHANDLE hObj, int fmodel);
	~SSRMS();

	//callback functions
	void clbkLoadStateEx(FILEHANDLE scn, void *vs);
	void clbkSaveState(FILEHANDLE scn);
	void clbkPreStep(double simt, double simdt, double mjd);
	void clbkPostStep(double SimT, double SimDT, double MJD);
	bool clbkDrawHUD (int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad* skp);
	void clbkRenderHUD(int mode, const HUDPAINTSPEC* hps, SURFHANDLE hDefaultTex);
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
	/**
	 * Shifts SSRMS mesh and attachment points so active LEE is at centre of external view
	 */
	void UpdateMeshPosition();
	/**
	 * Updates cockpit view to match LEE camera position
	 */
	void UpdateCameraView();
	void UpdateCameraAnimations();
	void CalculateVectors();
	
	/**
	 * Displays label indicating current camera view
	 */
	void ShowCameraViewLabel();

	/**
	 * Converts vector from Orbitersim frame to SSRMS IK frame
	 */
	VECTOR3 ConvertVectorToSSRMSFrame(const VECTOR3& v) const
	{
		return _V(v.z, v.x, -v.y);
	}

private:
	SSRMSSubsystemDirector* pSubsystemDirector;
	LEESystem* pLEE[2];

	VECTOR3 arm_tip[5];
	VECTOR3 arm_ee_pos, arm_ee_dir, arm_ee_rot;
	VECTOR3 ee_pos_output, ee_angles_output;
	double joint_angle[7]; // angles in degrees
	bool arm_moved, update_vectors, update_camera;

	int activeLEE, passiveLEE; // either 0 or 1
	short joint_motion[7]; // 0=stationary, -1=negative, +1=positive
	unsigned short SpeedFactor;
	FRAME RefFrame;

	LightEmitter* LEELight[2];

	CAMERA_VIEW cameraView;
	double camAPan, camATilt, camBPan, camBTilt;
	bool bTiltUp, bTiltDown, bPanLeft, bPanRight;
	VECTOR3 cameraA[3], cameraB[3]; // pos/dir/rot vectors for boom cameras
	UINT anim_CamATilt, anim_CamAPan, anim_CamBTilt, anim_CamBPan;
	
	NOTEHANDLE nhCameraLabel; // annotation to display current camera view
	double annotationDisplayTime; // counter used to show/hide camera label
	
	VECTOR3 AltKybdInput;

	VECTOR3 mesh_center; // used to shift meshes so active LEE is at centre of external view

	int OrbiterSoundHandle;

	//mesh handles
	MESHHANDLE hSSRMSMesh;
	UINT mesh_ssrms;
	UINT anim_joint[2][7], anim_lee;
	UINT anim_fold;
	//UINT anim_joint[7], anim_lee;
	
	AnimState foldState;

	bool ShowAttachmentPoints;

	THRUSTER_HANDLE rms_control[12];
	PROPELLANT_HANDLE ph_null;

	//ATTACHMENTHANDLE ahBase, ahGrapple;
	//LatchSystem* pLEE[2];
};

#endif // !__SSRMS_H
