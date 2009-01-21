#ifndef __RMSSUBSYSTEM_H
#define __RMSSUBSYSTEM_H
#pragma once

#include "MPMSystems.h"
#include "Atlantis.h"

const static char* RMS_MESHNAME = "SSU/RMS";

// RMS joint positions
const VECTOR3 RMS_SY_JOINT = _V(-2.61,1.65,9.02);
const VECTOR3 RMS_SP_JOINT = _V(-2.844, 2.1315, 9.018);
const VECTOR3 RMS_EP_JOINT = _V(-2.7345, 1.97, 2.742);
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
const double RMS_EE_TRANSLATION_SPEED = 0.1;
// RMS IK translation speed (m/s)

class RMSSystem : public MPMSystem
{
public:
	typedef enum {SHOULDER_YAW=0, SHOULDER_PITCH=1, ELBOW_PITCH=2, WRIST_PITCH=3, WRIST_YAW=4, WRIST_ROLL=5} RMS_JOINT;

	RMSSystem(SubsystemDirector* _director);
	virtual ~RMSSystem();

	virtual void Realize();
	virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	virtual void OnPostStep(double SimT, double DeltaT, double MJD);
	virtual bool OnParseLine(const char* line);
	virtual void OnSaveState(FILEHANDLE scn) const;

	virtual double GetSubsystemEmptyMass() const {return 0.0;};

	/**
	 * Rotates individual joint.
	 * @param positive true if joint should rotate in positive direction
	 */
	void RotateJoint(RMS_JOINT joint, bool positive);
	void TranslateEE(const VECTOR3 &direction);

	//temporary function: used only until RMSSystem class is responsible for EE attachment creation
	void DefineEndEffector(ATTACHMENTHANDLE attachment) {end_effector=attachment;};

	void Grapple(VESSEL* vessel, ATTACHMENTHANDLE attachment);
	void Ungrapple();
	/**
	 * Destroys attachment to currently grappled vessel, but remains 'grappled'.
	 * @param target in not null, payload is detached only if target pointer matches grappled vessel.
	 */
	void Detach(VESSEL* target=NULL);

	bool Grappled() const {return (grapple!=NULL);};
	/**
	 * Returns true if arm is free to move.
	 * Returns false if arm is grappled to payload which is attached to something else.
	 */
	bool Movable() const {return (grapple==NULL || PayloadIsFree());};
private:
	void CreateArm();
	bool PayloadIsFree() const;

	void Translate(const VECTOR3 &dPos);
	void SetJointAngle(RMS_JOINT joint, double angle); //angle in degrees
	void SetJointPos(RMS_JOINT joint, double pos);

	VESSEL* payload;
	ATTACHMENTHANDLE grapple, end_effector;
	//true if RMS is still grappled, but Orbiter should not fromally attach the RMS to the payload.
	bool detached;

	UINT anim_camRMSElbowPan, anim_camRMSElbowTilt;
	UINT anim_joint[6], anim_rms_ee;

	//RMS Camera rot/direction
	double camRMSElbowPan, camRMSElbowTilt;
	VECTOR3 camRMSElbowLoc[2];

	//EE and IK parameters
	VECTOR3 arm_tip[3];
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

	bool arm_moved;
	bool update_data;

	int joint_motion[6];
	int ee_translation[3];
};

#endif //__RMSSUBSYSTEM_H