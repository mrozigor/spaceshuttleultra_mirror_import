/****************************************************************************
  This file is part of Space Shuttle Ultra

  Remote Manipulator System definition



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/
#ifndef __RMSSUBSYSTEM_H
#define __RMSSUBSYSTEM_H
#pragma once

#include "MPMSystems.h"
#include "Atlantis.h"

const static char* RMS_MESHNAME = "SSU/RMS";

// RMS joint positions
//const VECTOR3 RMS_SY_JOINT = _V(-2.61,1.65,9.02);
const VECTOR3 RMS_SY_JOINT = _V(-2.6855, 1.6515, 9.017);
const VECTOR3 RMS_SP_JOINT = _V(-2.844, 2.1315, 9.018);
const VECTOR3 RMS_EP_JOINT = _V(-2.787, 1.9705, 2.742);
const VECTOR3 RMS_WP_JOINT = _V(-2.89, 2.133, -4.191);
const VECTOR3 RMS_WY_JOINT = _V(-2.845, 2.1325, -4.689);
const VECTOR3 RMS_EE_POS = _V(-2.84382, 2.13334, -6.049);

const double RMS_SP_EP_DIST = length(RMS_EP_JOINT-RMS_SP_JOINT);
// distance (metres) from SP joint to EP joint
const double RMS_EP_WP_DIST = length(RMS_WP_JOINT-RMS_EP_JOINT);
// distance (metres) from EP joint to WP joint
const double RMS_WP_WY_DIST = length(RMS_WY_JOINT-RMS_WP_JOINT);
// distance (metres) from WP joint to WY joint
const double RMS_WY_EE_DIST = length(RMS_WY_JOINT-RMS_EE_POS);
// distance (metres) from WY joint to EE
const double RMS_SP_NULL_ANGLE = DEG * acos((RMS_SP_JOINT.z-RMS_EP_JOINT.z)/RMS_SP_EP_DIST);
// effective angle (degrees) of shoulder boom in null position
const double RMS_EP_NULL_ANGLE = DEG * acos((RMS_EP_JOINT.z-RMS_WP_JOINT.z)/RMS_EP_WP_DIST);
// effective angle (degrees) of elbow boom in null position

const double RMS_JOINT_LIMITS[2][6] = {{-180.0, -2.0, -161.0, -121.4, -121.3, -447.0},
									{+180.0, +145.0, +2.4, +121.4, +121.3, +447.0}};
const double RMS_JOINT_SOFTSTOPS[2][6] = {{-177.4, +0.6, -157.6, -116.4, -116.6, -442.0},
										  {+177.4, +142.4, -0.4, +116.4, +116.6, +442.0}};

const double RMS_JOINT_ROTATION_SPEED = 1.5;
// RMS arm joint rotation speed (deg/sec)
const double RMS_EE_ROTATION_SPEED = 1.0*RAD;
// RMS IK rotation speed (rad/sec)
const double RMS_EE_TRANSLATION_SPEED = 0.1;
// RMS IK translation speed (m/s)

const double SHOULDER_BRACE_SPEED = 0.11765;
// shoulder brace speed (8.5 seconds)

//const VECTOR3 RMS_EE_CAM_OFFSET = {-0.091886, 0.276656, 0.666001};
const VECTOR3 RMS_EE_CAM_OFFSET = {0.0, 0.276656, 0.666001};
// Wrist camera offset from grapple point (assuming wrist roll angle of 0.0)
const VECTOR3 RMS_ELBOW_CAM_POS = _V(-2.681, 2.641, 1.806);

const double PTU_HIGHRATE_SPEED = 12.0;
const double PTU_LOWRATE_SPEED = 1.2;
// Payload camera speed rates (deg/sec)

class RMSSystem : public MPMSystem
{
public:
	typedef enum {SHOULDER_YAW=0, SHOULDER_PITCH=1, ELBOW_PITCH=2, WRIST_PITCH=3, WRIST_YAW=4, WRIST_ROLL=5} RMS_JOINT;
	typedef enum {PAN=0, TILT=1} CAMERA_MOTION;

	RMSSystem(SubsystemDirector* _director);
	virtual ~RMSSystem();

	virtual void Realize();
	virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	virtual void OnPostStep(double SimT, double DeltaT, double MJD);
	virtual bool OnParseLine(const char* line);
	virtual void OnSaveState(FILEHANDLE scn) const;

	virtual double GetSubsystemEmptyMass() const {return 0.0;};

	virtual void CreateAttachment();

	/**
	 * Rotates individual joint.
	 * @param positive true if joint should rotate in positive direction
	 */
	void RotateJoint(RMS_JOINT joint, bool positive);
	void TranslateEE(const VECTOR3 &direction);

	void RotateElbowCam(int pitch, int yaw);
	void SetElbowCamRotSpeed(bool low);

	void SetEECameraView(bool Active);
	void SetElbowCamView(bool Active);

	void ToggleJointAngleDisplay();

	OBJHANDLE Grapple();
	//void Grapple(VESSEL* vessel, ATTACHMENTHANDLE attachment);
	void Ungrapple() {DetachPayload();};

	bool Grappled() const {return (hPayloadAttachment!=NULL);};
	/**
	 * Returns true if arm is free to move.
	 * Returns false if arm is grappled to payload which is attached to something else.
	 */
	bool Movable() const;
protected:
	virtual void OnMRLLatched();

	virtual void OnAttach();
	virtual void OnDetach();
private:
	void CreateArm();
	bool ArmStowed() const;

	void Translate(const VECTOR3 &dPos);
	void Rotate(const VECTOR3 &dAngles); // angles in radians
	bool MoveEE(const VECTOR3 &newPos, const VECTOR3 &newAtt);
	void SetJointAngle(RMS_JOINT joint, double angle); //angle in degrees
	void SetJointPos(RMS_JOINT joint, double pos);

	int GetSelectedJoint() const;

	void UpdateEECamView() const;
	void UpdateElbowCamView() const;

	void AutoGrappleSequence();
	void AutoReleaseSequence();

	DiscInPort RMSSelect;

	UINT anim_camRMSElbow[2];
	UINT anim_joint[6], anim_rms_ee;

	//RMS Camera rot/direction
	double camRMSElbow[2];
	bool camLowSpeed;
	int camRMSElbow_rotation[2];
	VECTOR3 camRMSElbowLoc[2];
	bool camera_moved;

	//EE and IK parameters
	VECTOR3 arm_tip[4];
	VECTOR3 arm_wrist_pos;
	/** Refence frame for internal calculations:
	 * +X: Towards tail
	 * +Y: towards port side (right from aft windows)
	 * +Z: Up
	 * Frame corresponds to negative version of frame used by real shuttle
	 */
	VECTOR3 arm_ee_pos, arm_ee_dir, arm_ee_rot;
	VECTOR3 arm_tgt_pos, arm_tgt_dir;
	double joint_pos[6], joint_angle[6];
	double sp_null, ep_null; //required to compensate for elbow joint being 'below' booms
	int joint_motion[6];
	int ee_translation[3];
	DiscInPort JointSelect[6], DirectDrivePlus, DirectDriveMinus;
	DiscInPort RHCInput[3], THCInput[3];

	double shoulder_brace;
	DiscInPort ShoulderBrace;
	DiscOutPort ShoulderBraceReleased;

	AnimState Grapple_State, Rigid_State, Extend_State;
	DiscInPort EEAuto, EEMan, EERigid, EEDerigid, EEGrapple, EERelease;
	DiscOutPort EECapture, EEExtended, EEClosed, EEOpened, EERigidized, EEDerigidized;
	bool bAutoGrapple, bAutoRelease;

	bool arm_moved;
	bool update_data;

	bool display_angles;

	enum {NONE, EE, ELBOW} RMSCameraMode;

	// for LED displays on panel A8
	DiscOutPort JointAngles[6], EEPosition[3], EEAttitude[3];
};

#endif //__RMSSUBSYSTEM_H
