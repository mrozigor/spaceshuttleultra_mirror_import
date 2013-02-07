#include "RMSSystem.h"
#include "ParameterValues.h"
#include "meshres_RMS.h"
#include <UltraMath.h>

RMSSystem::RMSSystem(AtlantisSubsystemDirector *_director)
	: MPMSystem(_director, "RMS", RMS_MESHNAME, RMS_MESH_OFFSET, "G"), RMSCameraMode(NONE), bFirstStep(true)
{
	joint_pos[SHOULDER_YAW] = 0.5;
	joint_pos[SHOULDER_PITCH] = 0.0136;
	joint_pos[ELBOW_PITCH] = 0.985312;
	joint_pos[WRIST_PITCH] = 0.5;
	joint_pos[WRIST_YAW] = 0.5;
	joint_pos[WRIST_ROLL] = 0.5;
	arm_tip[0] = RMS_EE_POS;
	arm_tip[1] = RMS_EE_POS+_V(0.0, 0.0, -1.0); // to calculate EE attachment direction (-Z coordinate of attachment point is negative, so subtract 1 here)
	//arm_tip[2] = RMS_EE_POS+_V(0.0, 1.0, 0.0);
	arm_tip[2] = RMS_EE_POS+RotateVectorZ(_V(0.0, 1.0, 0.0), RMS_ROLLOUT_ANGLE); // to calculate rot vector for attachment
	arm_tip[3] = RMS_EE_POS+RotateVectorZ(_V(0.0, -1.0, 0.0), RMS_ROLLOUT_ANGLE); // to calculate arm_ee_rot (rot vector in IK frame)
	arm_tip[4] = RMS_EE_CAM_POS; // to calculate EE camera position
	arm_ee_pos = _V(RMS_SP_JOINT.z - RMS_EE_POS.z, 0.0, 0.0);
	arm_ee_dir = _V(1.0, 0.0, 0.0);
	arm_ee_rot = _V(0.0, 0.0, 1.0);
	arm_ee_angles = _V(0.0, 0.0, 0.0);

	// default EE to grapple open and derigidized
	Grapple_State.Set(AnimState::OPEN, 1);
	Rigid_State.Set(AnimState::OPEN, 1);
	Extend_State.Set(AnimState::OPEN, 1);

	bAutoGrapple=false;
	bAutoRelease=false;

	shoulder_brace=0.0; // released

	//RMS elbow camera
	camRMSElbowLoc[0]=RMS_ELBOW_CAM_POS;
	camRMSElbowLoc[1]=camRMSElbowLoc[0]+_V(0, 0, -1);
	//camRMSElbow_rotation[0]=camRMSElbow_rotation[1]=0;
	camRMSElbow[PAN] = 0.0;
	camRMSElbow[TILT] = 0.0;
	camera_moved=false;

	bLastCamInternal = false;

	arm_moved=false;
	update_vectors=false;
	update_angles=false;
	for(int i=0;i<6;i++) joint_motion[i]=0;
	for(int i=0;i<3;i++) ee_translation[i]=0;

	display_angles=false;
}

RMSSystem::~RMSSystem()
{
}

void RMSSystem::Realize()
{
	MPMSystem::Realize();

	DiscreteBundle* pBundle=BundleManager()->CreateBundle("RMS_EE", 16);
	EEGrapple.Connect(pBundle, 0);
	EERelease.Connect(pBundle, 1);
	EEAuto.Connect(pBundle, 2);
	EEMan.Connect(pBundle, 3);
	EERigid.Connect(pBundle, 4);
	EEDerigid.Connect(pBundle, 5);
	EECapture.Connect(pBundle, 6);
	EEExtended.Connect(pBundle, 7);
	EEClosed.Connect(pBundle, 8);
	EEOpened.Connect(pBundle, 9);
	EERigidized.Connect(pBundle, 10);
	EEDerigidized.Connect(pBundle, 11);

	pBundle=BundleManager()->CreateBundle(GetIdentifier(), 16);
	ShoulderBrace.Connect(pBundle, 4);
	ShoulderBraceReleased.Connect(pBundle, 5);
	RMSSelect.Connect(pBundle, 6);

	pBundle=BundleManager()->CreateBundle(GetIdentifier()+"_DATA", 16);
	for(int i=0;i<6;i++) JointAngles[i].Connect(pBundle, i);
	for(int i=0;i<3;i++) {
		EEPosition[i].Connect(pBundle, i+6);
		EEAttitude[i].Connect(pBundle, i+9);
	}

	pBundle=BundleManager()->CreateBundle("RMS_HC_INPUT", 16);
	for(int i=0;i<3;i++) {
		RHCInput[i].Connect(pBundle, i);
		THCInput[i].Connect(pBundle, i+3);
	}

	pBundle=BundleManager()->CreateBundle("RMS_SINGLE_JOINT", 16);
	for(int i=0;i<6;i++) {
		JointSelect[i].Connect(pBundle, i);
	}
	// indexes 6 and 7 and used for temps
	DirectDrivePlus.Connect(pBundle, 8);
	DirectDriveMinus.Connect(pBundle, 9);

	pBundle=STS()->BundleManager()->CreateBundle("RMS_MODE", 16);
	for(int i=0;i<12;i++) RMSMode[i].Connect(pBundle, i);
	RMSSpeed.Connect(pBundle, 12);

	pBundle = STS()->BundleManager()->CreateBundle("RMS_ELBOW_CAM", 16);
	ElbowCamPanLeft.Connect(pBundle, 0);
	ElbowCamPanRight.Connect(pBundle, 1);
	ElbowCamTiltUp.Connect(pBundle, 2);
	ElbowCamTiltDown.Connect(pBundle, 3);
	CamLowSpeed.Connect(pBundle, 4);

	CreateArm();
	//MPM animation is only added in CreateArm function, so we have to set initial MPM position here
	STS()->SetAnimation(anim_mpm, MPMRollout.pos);

	// set lines
	if(Grappled()) EECapture.SetLine();
	if(Extend_State.Open()) EEExtended.SetLine();
	if(Grapple_State.Open()) EEOpened.SetLine();
	else if(Grapple_State.Closed()) EEClosed.SetLine();
	if(Rigid_State.Closed()) EERigidized.SetLine();
	else if(Rigid_State.Open()) EEDerigidized.SetLine();
	if(Eq(shoulder_brace, 0.0, 0.01)) ShoulderBraceReleased.SetLine();
	else ShoulderBraceReleased.ResetLine();
}

void RMSSystem::CreateArm()
{
	//NOTE: all MGROUP_ROTATEs will be deleted by Atlantis class
	const VECTOR3 PitchAxis = RotateVectorZ(_V(1, 0, 0), RMS_ROLLOUT_ANGLE);
	const VECTOR3 YawAxis = RotateVectorZ(_V(0, 1, 0), RMS_ROLLOUT_ANGLE);

	//rollout animation
	static UINT RMSRolloutGrp[2] = {GRP_RMS_MPMs, GRP_base};
	MGROUP_ROTATE* pRMS_rollout_anim = new MGROUP_ROTATE(mesh_index, RMSRolloutGrp, 2,
		_V(-2.55784, 1.12081, 0.0), _V(0, 0, 1), (float)((RMS_ROLLOUT_ANGLE+RMS_STOWED_ANGLE)*RAD));
	anim_mpm = STS()->CreateAnimation(1.0);
	ANIMATIONCOMPONENT_HANDLE parent = STS()->AddManagedAnimationComponent(anim_mpm, 0, 1, pRMS_rollout_anim);

	//shoulder yaw
	static UINT RMSShoulderYawGrp[1] = {GRP_Shoulder_Yaw};
	MGROUP_ROTATE* pRMS_sy_anim = new MGROUP_ROTATE(mesh_index, RMSShoulderYawGrp, 1,
		RMS_SY_JOINT, YawAxis, (float)(-360*RAD)); // -180 .. +180
	anim_joint[SHOULDER_YAW] = STS()->CreateAnimation (0.5);
	parent = STS()->AddManagedAnimationComponent (anim_joint[SHOULDER_YAW], 0, 1, pRMS_sy_anim, parent);

	//shoulder pitch
	static UINT RMSShoulderPitchGrp[1] = {GRP_Humerus};
	MGROUP_ROTATE* pRMS_sp_anim = new MGROUP_ROTATE(mesh_index, RMSShoulderPitchGrp, 1,
		RMS_SP_JOINT, PitchAxis, (float)(147.0*RAD)); // -2 .. +145
	anim_joint[SHOULDER_PITCH] = STS()->CreateAnimation (0.0136);
	parent = STS()->AddManagedAnimationComponent (anim_joint[SHOULDER_PITCH], 0, 1, pRMS_sp_anim, parent);

	//elbow pitch
	static UINT RMSElbowPitchGrp[2] = {GRP_box, GRP_cambase};
	MGROUP_ROTATE* pRMS_ep_anim = new MGROUP_ROTATE(mesh_index, RMSElbowPitchGrp, 2,
		RMS_EP_JOINT, PitchAxis, (float)(163.4*RAD));
	anim_joint[ELBOW_PITCH] = STS()->CreateAnimation (0.985312);
	parent = STS()->AddManagedAnimationComponent (anim_joint[ELBOW_PITCH], 0, 1, pRMS_ep_anim, parent);

	//RMS elbow camera
	static UINT RMSElbowCamGrp[2] = {GRP_elbowcam, GRP_camswivel};
	MGROUP_ROTATE* pRMSElbowCamPan = new MGROUP_ROTATE(mesh_index, RMSElbowCamGrp+1, 1,
		_V(-2.76, 2.38, 1.93), _V(0.309015, 0.951057, 0), (float)(340*RAD));
	ANIMATIONCOMPONENT_HANDLE parent2;
	anim_camRMSElbow[PAN]=STS()->CreateAnimation(0.5);
	parent2 = STS()->AddManagedAnimationComponent (anim_camRMSElbow[PAN], 0, 1, pRMSElbowCamPan, parent);
	MGROUP_ROTATE* pRMSElbowCamTilt = new MGROUP_ROTATE(mesh_index, RMSElbowCamGrp, 1,
		_V(-2.69, 2.56, 1.93), _V(0.951057, -0.309014, 0), (float)(340*RAD));
	anim_camRMSElbow[TILT]=STS()->CreateAnimation(0.5);
	parent2 = STS()->AddManagedAnimationComponent(anim_camRMSElbow[TILT], 0, 1, pRMSElbowCamTilt, parent2);
	MGROUP_ROTATE* pRMSElbowCamLoc = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(camRMSElbowLoc), 2,
		_V(-2.765, 2.373, 2.073), _V(1, 0, 0), 0.0f);
	STS()->AddManagedAnimationComponent(anim_camRMSElbow[TILT], 0, 1, pRMSElbowCamLoc, parent2);

	//wrist pitch
	static UINT RMSWristPitchGrp[1] = {GRP_Wristpitch};
	MGROUP_ROTATE* pRMS_wp_anim = new MGROUP_ROTATE(mesh_index, RMSWristPitchGrp, 1,
		RMS_WP_JOINT, PitchAxis, (float)(242.8*RAD)); // -121.4 .. +121.4
	anim_joint[WRIST_PITCH] = STS()->CreateAnimation (0.5);
	parent = STS()->AddManagedAnimationComponent (anim_joint[WRIST_PITCH], 0, 1, pRMS_wp_anim, parent);

	//wrist yaw
	static UINT RMSWristYawGrp[1] = {GRP_Wrist_Yaw};
	MGROUP_ROTATE* pRMS_wy_anim = new MGROUP_ROTATE(mesh_index, RMSWristYawGrp, 1,
		RMS_WY_JOINT, YawAxis, (float)(242.6*RAD)); // -121.3 .. +121.3
	anim_joint[WRIST_YAW] = STS()->CreateAnimation (0.5);
	parent = STS()->AddManagedAnimationComponent (anim_joint[WRIST_YAW], 0, 1, pRMS_wy_anim, parent);

	//wrist roll
	static UINT RMSEndEffectorGrp[1] = {GRP_Endeffector};
	MGROUP_ROTATE* pRMS_wr_anim = new MGROUP_ROTATE(mesh_index, RMSEndEffectorGrp, 1,
		RMS_EE_POS, _V(0, 0, 1), (float)(894*RAD));  // -447 .. +447
	anim_joint[WRIST_ROLL] = STS()->CreateAnimation (0.5);
	parent = STS()->AddManagedAnimationComponent (anim_joint[WRIST_ROLL], 0, 1, pRMS_wr_anim, parent);

	MGROUP_ROTATE* pRMS_ee_anim = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(arm_tip), 5,
		RMS_EE_POS, _V(0,0,1), (float)(0.0));
	anim_rms_ee = STS()->CreateAnimation (0.0);
	STS()->AddManagedAnimationComponent (anim_rms_ee, 0, 1, pRMS_ee_anim, parent);

	//if joint angles were loaded from scenario file, 
	if(arm_moved) {
		SetJointPos(SHOULDER_YAW, joint_pos[SHOULDER_YAW]);
		SetJointPos(SHOULDER_PITCH, joint_pos[SHOULDER_PITCH]);
		SetJointPos(ELBOW_PITCH, joint_pos[ELBOW_PITCH]);
		SetJointPos(WRIST_PITCH, joint_pos[WRIST_PITCH]);
		SetJointPos(WRIST_YAW, joint_pos[WRIST_YAW]);
		SetJointPos(WRIST_ROLL, joint_pos[WRIST_ROLL]);
	}
	if(camera_moved) {
		for(int i=0;i<2;i++) {
			double anim=linterp(-170, 0, 170, 1, camRMSElbow[i]);
			STS()->SetAnimation(anim_camRMSElbow[i], anim);
		}
	}
}

void RMSSystem::CreateAttachment()
{
	if(!hAttach)
		hAttach=STS()->CreateAttachment(false, STS()->GetOrbiterCoGOffset()+arm_tip[0]+RMS_MESH_OFFSET, arm_tip[1]-arm_tip[0], arm_tip[2]-arm_tip[0], "G", true);
}

bool RMSSystem::Movable() const 
{ 
	return ( RMSSelect && ( (Eq(shoulder_brace, 0.0, 0.01) && MRLLatches.Open()) || !ArmStowed() ) 
		&& (hPayloadAttachment==NULL || (!doubleAttached && PayloadIsFree())) );
}

void RMSSystem::OnPreStep(double SimT, double DeltaT, double MJD)
{
	MPMSystem::OnPreStep(SimT, DeltaT, MJD);

	// make sure RMS is powered and can be operated
	if(!RMSSelect) return;

	//rotate joints
	if(Movable()) {
		// single joint rotation
		if(RMSMode[1] || RMSMode[2]) { // make sure RMS is in SINGLE or DIRECT mode
			if(DirectDrivePlus) {
				int joint=GetSelectedJoint();
				if(joint!=-1) {
					if(!RMSSpeed) SetJointAngle((RMS_JOINT)joint, joint_angle[joint]+RMS_JOINT_COARSE_ROTATION_SPEEDS[joint]*DeltaT);
					else SetJointAngle((RMS_JOINT)joint, joint_angle[joint]+RMS_JOINT_VERN_ROTATION_SPEEDS[joint]*DeltaT);
				}
				update_vectors=true;
			}
			else if(DirectDriveMinus) {
				int joint=GetSelectedJoint();
				if(joint!=-1) {
					if(!RMSSpeed) SetJointAngle((RMS_JOINT)joint, joint_angle[joint]-RMS_JOINT_COARSE_ROTATION_SPEEDS[joint]*DeltaT);
					else SetJointAngle((RMS_JOINT)joint, joint_angle[joint]-RMS_JOINT_VERN_ROTATION_SPEEDS[joint]*DeltaT);
				}
				update_vectors=true;
			}
		}
		/*for(int i=0;i<6;i++) {
			if(joint_motion[i]!=0) {
				//SetJointAngle((RMS_JOINT)i, joint_angle[i]+RMS_JOINT_ROTATION_SPEEDS[i]*DeltaT*joint_motion[i]);
				if(!RMSSpeed) SetJointAngle((RMS_JOINT)i, joint_angle[i]+RMS_JOINT_COARSE_ROTATION_SPEEDS[i]*DeltaT*joint_motion[i]);
				else SetJointAngle((RMS_JOINT)i, joint_angle[i]+RMS_JOINT_VERN_ROTATION_SPEEDS[i]*DeltaT*joint_motion[i]);
				update_vectors=true;
				joint_motion[i]=0;
			}
		}*/
		else { // not in single joint mode
			VECTOR3 newPos = arm_ee_pos;
			VECTOR3 newDir = arm_ee_dir;
			VECTOR3 newRot = arm_ee_rot;
			bool rotateEE=false, translateEE=false;

			// EE rotation
			VECTOR3 change=_V(0.0, 0.0, 0.0);
			//bool moveEE=false;
			for(int i=0;i<3;i++) {
				if(!Eq(RHCInput[i].GetVoltage(), 0.0, 0.05)) {
					//change.data[i]+=(RHCInput[i].GetVoltage()/5.0)*DeltaT*RMS_EE_ROTATION_SPEED;
					if(!RMSSpeed) change.data[i]+=(RHCInput[i].GetVoltage()/5.0)*DeltaT*RMS_EE_COARSE_ROTATION_SPEED;
					else change.data[i]+=(RHCInput[i].GetVoltage()/5.0)*DeltaT*RMS_EE_VERN_ROTATION_SPEED;
					rotateEE=true;
				}
			}
			if(rotateEE) Rotate(change, newDir, newRot);

			// EE translation
			change=_V(0.0, 0.0, 0.0);
			//moveEE=false;
			for(int i=0;i<3;i++) {
				if(!Eq(THCInput[i].GetVoltage(), 0.0, 0.05)) {
					//change.data[i]+=(THCInput[i].GetVoltage()/5.0)*DeltaT*RMS_EE_TRANSLATION_SPEED;
					if(!RMSSpeed) change.data[i]+=(THCInput[i].GetVoltage()/5.0)*DeltaT*RMS_EE_COARSE_TRANSLATION_SPEED;
					else change.data[i]+=(THCInput[i].GetVoltage()/5.0)*DeltaT*RMS_EE_VERN_TRANSLATION_SPEED;
					translateEE=true;
				}
				else if(ee_translation[i]!=0) {
					//change.data[i]+=ee_translation[i]*DeltaT*RMS_EE_TRANSLATION_SPEED;
					if(!RMSSpeed) change.data[i]+=ee_translation[i]*DeltaT*RMS_EE_COARSE_TRANSLATION_SPEED;
					else change.data[i]+=ee_translation[i]*DeltaT*RMS_EE_VERN_TRANSLATION_SPEED;
					ee_translation[i]=0;
					translateEE=true;
				}
			}
			if(translateEE) Translate(change, newPos);

			if(translateEE || rotateEE) MoveEE(newPos, newDir, newRot, DeltaT);
		}
	}

	if(EEAuto || EEMan) {
		if(Grapple_State.Moving()) {
			Grapple_State.Move(DeltaT*RMS_GRAPPLE_SPEED);

			if(Grapple_State.Closed()) {
				if(!STS()->GetAttachmentStatus(hAttach)) Grapple();
				EEClosed.SetLine();
				if(EEAuto) AutoGrappleSequence();
			}
			else if(Grapple_State.Open()) {
				EEOpened.SetLine();
				if(EEAuto) AutoReleaseSequence();
			}
			else {
				if(Grappled()) Ungrapple();
				EEClosed.ResetLine();
				EEOpened.ResetLine();
			}
		}
		if(Extend_State.Moving()) {
			Extend_State.Move(DeltaT*RMS_EXTEND_SPEED);
			if(Extend_State.Open()) {
				EEExtended.SetLine();
				if(EEAuto) AutoReleaseSequence();
			}
			else if(Extend_State.Closed() && EEAuto) AutoGrappleSequence();
			else EEExtended.ResetLine();
		}
		if(Rigid_State.Moving()) {
			Rigid_State.Move(DeltaT*RMS_RIGID_SPEED);
			if(Rigid_State.Open()) {
				EEDerigidized.SetLine();
				if(EEAuto) AutoReleaseSequence();
			}
			else if(Rigid_State.Closed()) {
				EERigidized.SetLine();
				if(EEAuto) AutoGrappleSequence();
			}
			else {
				EEDerigidized.ResetLine();
				EERigidized.ResetLine();
			}
		}

		if(EEAuto) {
			if(EEGrapple) {
				bAutoGrapple=true;
				bAutoRelease=false;
				AutoGrappleSequence();
			}
			else if(EERelease) {
				bAutoRelease=true;
				bAutoGrapple=false;
				AutoReleaseSequence();
			}
		}
		else { //EEMan must be set
			if(EEGrapple) Grapple_State.action=AnimState::CLOSING;
			else if(EERelease) Grapple_State.action=AnimState::OPENING;
			else if(Grapple_State.Moving()) Grapple_State.action=AnimState::STOPPED;

			if(EERigid) {
				if(!Extend_State.Closed()) Extend_State.action=AnimState::CLOSING;
				else if(!Rigid_State.Closed()) Rigid_State.action=AnimState::CLOSING;
			}
			else if(EEDerigid) {
				if(!Rigid_State.Open()) Rigid_State.action=AnimState::OPENING;
				else if(!Extend_State.Open()) Extend_State.action=AnimState::OPENING;
			}
			else {
				if(Extend_State.Moving()) Extend_State.action=AnimState::STOPPED;
				if(Rigid_State.Moving()) Rigid_State.action=AnimState::STOPPED;
			}
		}
	}
	else {
		if(Grapple_State.Moving()) Grapple_State.action=AnimState::STOPPED;
		if(Extend_State.Moving()) Extend_State.action=AnimState::STOPPED;
		if(Rigid_State.Moving()) Rigid_State.action=AnimState::STOPPED;
	}

	if(ShoulderBrace) {
		if(shoulder_brace>0.0) {
			shoulder_brace=max(shoulder_brace-DeltaT*SHOULDER_BRACE_SPEED, 0.0);
			if(Eq(shoulder_brace, 0.0, 0.01)) ShoulderBraceReleased.SetLine();
		}
	}

	/*for(int i=0;i<2;i++) {
		if(camRMSElbow_rotation[i]!=0) {
			if(camLowSpeed) camRMSElbow[i]+=camRMSElbow_rotation[i]*PTU_LOWRATE_SPEED*DeltaT;
			else camRMSElbow[i]+=camRMSElbow_rotation[i]*PTU_HIGHRATE_SPEED*DeltaT;

			double anim=linterp(-170, 0, 170, 1, camRMSElbow[i]);
			STS()->SetAnimation(anim_camRMSElbow[i], anim);

			camera_moved=true;
			camRMSElbow_rotation[i]=0;
		}
	}*/
	if(ElbowCamPanLeft) {
		if(CamLowSpeed) camRMSElbow[PAN] = max(camRMSElbow[PAN]-PTU_LOWRATE_SPEED*DeltaT, -MAX_PLBD_CAM_PAN);
		else camRMSElbow[PAN] = max(camRMSElbow[PAN]-PTU_HIGHRATE_SPEED*DeltaT, -MAX_PLBD_CAM_PAN);
		camera_moved=true;
	}
	else if(ElbowCamPanRight) {
		if(CamLowSpeed) camRMSElbow[PAN] = min(camRMSElbow[PAN]+PTU_LOWRATE_SPEED*DeltaT, MAX_PLBD_CAM_PAN);
		else camRMSElbow[PAN] = min(camRMSElbow[PAN]+PTU_HIGHRATE_SPEED*DeltaT, MAX_PLBD_CAM_PAN);
		camera_moved=true;
	}
	if(ElbowCamTiltDown) {
		if(CamLowSpeed) camRMSElbow[TILT] = max(camRMSElbow[TILT]-PTU_LOWRATE_SPEED*DeltaT, -MAX_PLBD_CAM_TILT);
		else camRMSElbow[TILT] = max(camRMSElbow[TILT]-PTU_HIGHRATE_SPEED*DeltaT, -MAX_PLBD_CAM_TILT);
		camera_moved=true;
	}
	else if(ElbowCamTiltUp) {
		if(CamLowSpeed) camRMSElbow[TILT] = min(camRMSElbow[TILT]+PTU_LOWRATE_SPEED*DeltaT, MAX_PLBD_CAM_TILT);
		else camRMSElbow[TILT] = min(camRMSElbow[TILT]+PTU_HIGHRATE_SPEED*DeltaT, MAX_PLBD_CAM_TILT);
		camera_moved=true;
	}
}

void RMSSystem::OnPostStep(double SimT, double DeltaT, double MJD)
{
	MPMSystem::OnPostStep(SimT, DeltaT, MJD);

	if(display_angles) {
		sprintf_s(oapiDebugString(), 255, "SY:%f SP:%f EP:%f WP:%f WY:%f WR:%f", joint_angle[SHOULDER_YAW], joint_angle[SHOULDER_PITCH], joint_angle[ELBOW_PITCH], 
			joint_angle[WRIST_PITCH], joint_angle[WRIST_YAW], joint_angle[WRIST_ROLL]);
	}

	// if arm was moved, update attachment position and IK vectors/angles
	// due to bug in orbiter_ng/D3D9 client, this needs to be done on second timestep
	if(arm_moved) {
		if(hAttach) STS()->SetAttachmentParams(hAttach, STS()->GetOrbiterCoGOffset()+arm_tip[0]+RMS_MESH_OFFSET, arm_tip[1]-arm_tip[0], arm_tip[2]-arm_tip[0]);

		for(int i=0;i<3;i++) MRL_RTL_Microswitches[i].ResetLine();
		if(Eq(joint_angle[SHOULDER_YAW], 0.0, MRL_MAX_ANGLE_ERROR) && Eq(joint_angle[SHOULDER_PITCH], 0.0, MRL_MAX_ANGLE_ERROR)) {
			MRL_RTL_Microswitches[0].SetLine();

			if(Eq(joint_angle[ELBOW_PITCH], 0.0, MRL_MAX_ANGLE_ERROR)) {
				MRL_RTL_Microswitches[1].SetLine();

				if(Eq(joint_angle[WRIST_PITCH], 0.0, MRL_MAX_ANGLE_ERROR) && Eq(joint_angle[WRIST_YAW], 0.0, MRL_MAX_ANGLE_ERROR) && Eq(joint_angle[WRIST_ROLL], 0.0, MRL_MAX_ANGLE_ERROR)) {
					MRL_RTL_Microswitches[2].SetLine();
				}
			}
		}

		if(RMSCameraMode==EE) UpdateEECamView();
		else if(RMSCameraMode==ELBOW) UpdateElbowCamView();

		/*** Update output lines to LEDs ***/
		// calculate position
		VECTOR3 ee_pos_output=(arm_tip[0]-RMS_SP_JOINT)*12/fps_to_ms;
		ee_pos_output = _V(ee_pos_output.z, ee_pos_output.x, -ee_pos_output.y) + _V(-688.9, -108.0, -445.0);

		// calculate attitude
		VECTOR3 arm_ee_dir_orb[3]; // reference frame define by EE direction
		arm_ee_dir_orb[0]=arm_tip[0]-arm_tip[1];
		arm_ee_dir_orb[1]=-arm_tip[0]+arm_tip[2];
		arm_ee_dir_orb[2]=crossp(arm_ee_dir_orb[1], arm_ee_dir_orb[0]);
		MATRIX3 arm_ee_dir_mat = _M(arm_ee_dir_orb[2].x, arm_ee_dir_orb[2].y, arm_ee_dir_orb[2].z,
									arm_ee_dir_orb[1].x, arm_ee_dir_orb[1].y, arm_ee_dir_orb[1].z,
									arm_ee_dir_orb[0].x, arm_ee_dir_orb[0].y, arm_ee_dir_orb[0].z);
		VECTOR3 ee_att_output = GetZYX_RYPAnglesFromMatrix(arm_ee_dir_mat);
		// reference frame is a bit odd here, so we need this to get the math to work
		ee_att_output.data[PITCH]=-ee_att_output.data[PITCH];
		for(int i=0;i<3;i++) {
			if(ee_att_output.data[i]<0.0) ee_att_output.data[i]+=2*PI;

			EEPosition[i].SetLine((float)(ee_pos_output.data[i]/2000.0));
			EEAttitude[i].SetLine((float)(DEG*ee_att_output.data[i]/2000.0));
		}
		if(update_angles) {
			arm_ee_angles=ee_att_output;
			if(!bFirstStep) update_angles=false;
		}

		if(update_vectors) {
			arm_ee_dir=RotateVectorZ(arm_tip[1]-arm_tip[0], -RMS_ROLLOUT_ANGLE);
			arm_ee_dir=_V(-arm_ee_dir.z, -arm_ee_dir.x, -arm_ee_dir.y);
			//sprintf_s(oapiDebugString(), 255, "Calculated dir: %f %f %f", arm_ee_dir.x, arm_ee_dir.y, arm_ee_dir.z);

			arm_ee_rot=RotateVectorZ(arm_tip[3]-arm_tip[0], -RMS_ROLLOUT_ANGLE);
			arm_ee_rot=_V(-arm_ee_rot.z, -arm_ee_rot.x, -arm_ee_rot.y);
			//sprintf_s(oapiDebugString(), 255, "Calculated rot: %f %f %f", arm_ee_rot.x, arm_ee_rot.y, arm_ee_rot.z);

			//arm_ee_pos=RotateVectorZ(_V(-2.84, 2.13, 9.02)-arm_tip[0], -18.435);
			arm_ee_pos=RotateVectorZ(RMS_SP_JOINT-arm_tip[0], -RMS_ROLLOUT_ANGLE);
			arm_ee_pos=_V(arm_ee_pos.z, arm_ee_pos.x, arm_ee_pos.y);
			//sprintf_s(oapiDebugString(), 255, "Calculated EE pos: %f %f %f", arm_ee_pos.x, arm_ee_pos.y, arm_ee_pos.z);

			if(!bFirstStep) update_vectors=false;
		}

		if(!bFirstStep) arm_moved=false;
	}
	else if(camera_moved /*&& RMSCameraMode==ELBOW*/) {
		double anim=linterp(-MAX_PLBD_CAM_PAN, 0, MAX_PLBD_CAM_PAN, 1, camRMSElbow[PAN]);
		STS()->SetAnimation(anim_camRMSElbow[PAN], anim);
		anim=linterp(-MAX_PLBD_CAM_TILT, 0, MAX_PLBD_CAM_TILT, 1, camRMSElbow[TILT]);
		STS()->SetAnimation(anim_camRMSElbow[TILT], anim);

		if(RMSCameraMode==ELBOW) UpdateElbowCamView();
		camera_moved=false;
	}

	// if user enters cockpit view from external view, update camera direction
	// due to bug in Orbiter API, this cam position can't be updated when RMS is moved in external view
	if(!bLastCamInternal && oapiCameraInternal()) {
		if(RMSCameraMode==EE) UpdateEECamView();
		else if(RMSCameraMode==ELBOW) UpdateElbowCamView();
	}
	bLastCamInternal = oapiCameraInternal();

	if(bFirstStep) {
		//oapiWriteLog("RMSSystem: first step");
		bFirstStep = false;
	}
}

bool RMSSystem::OnParseLine(const char* line)
{
	if(!_strnicmp(line, "ARM_STATUS", 10)) {
		sscanf_s(line+10, "%lf%lf%lf%lf%lf%lf", &joint_pos[SHOULDER_YAW], &joint_pos[SHOULDER_PITCH], &joint_pos[ELBOW_PITCH], 
			&joint_pos[WRIST_PITCH], &joint_pos[WRIST_YAW], &joint_pos[WRIST_ROLL]);
		joint_pos[ELBOW_PITCH]=1.0-joint_pos[ELBOW_PITCH];
		arm_moved=true;
		update_vectors=true;
		update_angles=true;
		//oapiWriteLog("Read ARM_STATUS line");
		return true;
	}
	else if(!_strnicmp(line, "RMS_ELBOW_CAM", 13)) {
		sscanf_s(line+13, "%lf%lf", &camRMSElbow[PAN], &camRMSElbow[TILT]);
		camera_moved=true;
		return true;
	}
	else if(!_strnicmp(line, "GRAPPLE", 7)) {
		sscan_state((char*)line+7, Grapple_State);
		return true;
	}
	else if(!_strnicmp(line, "RIGIDIZE", 8)) {
		sscan_state((char*)line+8, Rigid_State);
		return true;
	}
	else if(!_strnicmp(line, "EXTEND", 6)) {
		sscan_state((char*)line+6, Extend_State);
		return true;
	}
	else if(!_strnicmp(line, "SHOULDER_BRACE", 14)) {
		sscanf_s(line+14, "%lf", &shoulder_brace);
		return true;
	}

	return MPMSystem::OnParseLine(line);
}

void RMSSystem::OnSaveState(FILEHANDLE scn) const
{
	char cbuf[255];

	//oapiWriteLine(scn, "  RMS");
	sprintf_s(cbuf, 255, "%0.6f %0.6f %0.6f %0.6f %0.6f %0.6f", joint_pos[SHOULDER_YAW], joint_pos[SHOULDER_PITCH], 1.0-joint_pos[ELBOW_PITCH], 
		joint_pos[WRIST_PITCH], joint_pos[WRIST_YAW], joint_pos[WRIST_ROLL]);
	oapiWriteScenario_string (scn, "ARM_STATUS", cbuf);
	oapiWriteScenario_float(scn, "SHOULDER_BRACE", shoulder_brace);
	WriteScenario_state(scn, "GRAPPLE", Grapple_State);
	WriteScenario_state(scn, "RIGIDIZE", Rigid_State);
	WriteScenario_state(scn, "EXTEND", Extend_State);
	sprintf_s(cbuf, 255, "%0.4f %0.4f", camRMSElbow[PAN], camRMSElbow[TILT]);
	oapiWriteScenario_string(scn, "RMS_ELBOW_CAM", cbuf);

	MPMSystem::OnSaveState(scn);
}

void RMSSystem::RotateJoint(RMS_JOINT joint, bool positive)
{
	if(positive) joint_motion[joint]=1;
	else joint_motion[joint]=-1;
}

/*void RMSSystem::TranslateEE(const VECTOR3 &direction)
{
	for (int i=0;i<3;i++) {
		if(direction.data[i]>0.25) ee_translation[i]=1;
		else if(direction.data[i]<-0.25) ee_translation[i]=-1;
		else ee_translation[i]=0;
	}
}*/

/*void RMSSystem::RotateElbowCam(int pitch, int yaw)
{
	camRMSElbow_rotation[PAN]=yaw;
	camRMSElbow_rotation[TILT]=pitch;
}*/

/*void RMSSystem::SetElbowCamRotSpeed(bool low)
{
	camLowSpeed=low;
}*/

void RMSSystem::Translate(const VECTOR3 &dPos, VECTOR3& newPos)
{
	if(RMSMode[5].IsSet()) { // END EFF
		// Reference Frame:
		// X: in direction of EE (arm_ee_dir) Z: opposite to camera direction (-arm_ee_rot) Y: completes RH frame
		
		/*VECTOR3 change=RotateVectorX(arm_ee_dir, RMS_ROLLOUT_ANGLE)*dPos.x;
		change+=RotateVectorX(arm_ee_rot, RMS_ROLLOUT_ANGLE)*dPos.z;
		change+=RotateVectorX(crossp(arm_ee_rot, arm_ee_dir), RMS_ROLLOUT_ANGLE)*dPos.y;
		//RotateVectorX(change, -RMS_ROLLOUT_ANGLE);*/

		//VECTOR3 cdPos=RotateVectorX(dPos, -RMS_ROLLOUT_ANGLE);
		//VECTOR3 change=arm_ee_dir*cdPos.x+arm_ee_rot*cdPos.z+crossp(arm_ee_rot, arm_ee_dir)*cdPos.y;
		VECTOR3 change=arm_ee_dir*dPos.x+arm_ee_rot*dPos.z+crossp(arm_ee_rot, arm_ee_dir)*dPos.y;
		//MoveEE(arm_ee_pos+change, arm_ee_dir, arm_ee_rot);
		newPos = arm_ee_pos+change;
	}
	else if(RMSMode[6].IsSet()) { // ORB LD
		//MoveEE(arm_ee_pos+RotateVectorX(dPos, -RMS_ROLLOUT_ANGLE), arm_ee_dir, arm_ee_rot);
		newPos = arm_ee_pos+RotateVectorX(dPos, RMS_ROLLOUT_ANGLE);
	}
}

void RMSSystem::Rotate(const VECTOR3 &dAngles, VECTOR3& newDir, VECTOR3& newRot)
{
	//static const VECTOR3 inDir=_V(0.0, 0.0, -1.0);
	//static const VECTOR3 inRot=RotateVectorZ(_V(0.0, 1.0, 0.0), RMS_ROLLOUT_ANGLE);
	//VECTOR3 newDir, newRot;

	if(RMSMode[5].IsSet()) { // END EFF mode
		// NOTE: EE mode rotates relative to camera orientation
		// we do not need to compensate for angle with RMS and shuttle frames
		// in EE mode, Z-axis is in opposited direction to arm_ee_rot
		VECTOR3 y_axis = crossp(arm_ee_rot, arm_ee_dir);
		// create rotation matrix corresponding to current orientation
		MATRIX3 RotMatrix = _M(arm_ee_dir.x, y_axis.x, arm_ee_rot.x,
							   arm_ee_dir.y, y_axis.y, arm_ee_rot.y,
							   arm_ee_dir.z, y_axis.z, arm_ee_rot.z);
		//MATRIX3 RotMatrix = RotationMatrix(arm_ee_dir, , arm_ee_rot);
		MATRIX3 RotMatrixRoll, RotMatrixPitch, RotMatrixYaw;
		GetRotMatrixX(dAngles.data[ROLL], RotMatrixRoll);
		GetRotMatrixY(dAngles.data[PITCH], RotMatrixPitch);
		GetRotMatrixZ(dAngles.data[YAW], RotMatrixYaw);
		// update rotation matrix to adjust for new angles
		RotMatrix = mul(RotMatrix, RotMatrixPitch);
		RotMatrix = mul(RotMatrix, RotMatrixYaw);
		RotMatrix = mul(RotMatrix, RotMatrixRoll);

		newDir = _V(RotMatrix.m11, RotMatrix.m21, RotMatrix.m31);
		newRot = _V(RotMatrix.m13, RotMatrix.m23, RotMatrix.m33);

		/*VECTOR3 ee_dir=RotateVectorX(arm_ee_dir, RMS_ROLLOUT_ANGLE);
		// NOTE: for math to work, we need to swap yaw and pitch angles
		RotateVectorPYR(ee_dir, _V(dAngles.data[ROLL], dAngles.data[PITCH], dAngles.data[YAW]), newDir);
		newDir=RotateVectorX(newDir, -RMS_ROLLOUT_ANGLE);

		VECTOR3 ee_rot=RotateVectorX(arm_ee_rot, RMS_ROLLOUT_ANGLE);
		RotateVectorPYR(ee_rot, _V(dAngles.data[ROLL], dAngles.data[PITCH], dAngles.data[YAW]), newRot);
		newRot=RotateVectorX(newRot, -RMS_ROLLOUT_ANGLE);*/

		//MoveEE(arm_ee_pos, newDir, newRot);
		update_angles=true;
	}
	else if(RMSMode[6].IsSet()) { // ORB UNL
		/*VECTOR3 newAngles=arm_ee_angles+dAngles;
		RotateVectorPYR(inDir, _V(-newAngles.data[PITCH], newAngles.data[YAW], newAngles.data[ROLL]), newDir);
		RotateVectorPYR(inRot, _V(-newAngles.data[PITCH], newAngles.data[YAW], newAngles.data[ROLL]), newRot);
		newDir=_V(-newDir.z, -newDir.x, newDir.y);
		newRot=_V(-newRot.z, -newRot.x, newRot.y);
		newDir=RotateVectorX(newDir, -RMS_ROLLOUT_ANGLE);
		newRot=RotateVectorX(newRot, -RMS_ROLLOUT_ANGLE);*/

		//VECTOR3 newAngles=arm_ee_angles+dAngles;
		//RotateVectorPYR(inDir, _V(-newAngles.data[PITCH], newAngles.data[YAW], newAngles.data[ROLL]), newDir);
		//RotateVectorPYR(inRot, _V(-newAngles.data[PITCH], newAngles.data[YAW], newAngles.data[ROLL]), newRot);
		//newDir=_V(-newDir.z, -newDir.x, newDir.y);
		//newRot=_V(-newRot.z, -newRot.x, newRot.y);
		newDir=RotateVectorX(arm_ee_dir, -RMS_ROLLOUT_ANGLE);
		newRot=RotateVectorX(arm_ee_rot, -RMS_ROLLOUT_ANGLE);
		RotateVector(newDir, _V(dAngles.data[ROLL], dAngles.data[PITCH], dAngles.data[YAW]), newDir);
		RotateVector(newRot, _V(dAngles.data[ROLL], dAngles.data[PITCH], dAngles.data[YAW]), newRot);
		newDir=RotateVectorX(newDir, RMS_ROLLOUT_ANGLE);
		newRot=RotateVectorX(newRot, RMS_ROLLOUT_ANGLE);

		//if(MoveEE(arm_ee_pos, newDir, newRot)) arm_ee_angles=newAngles;
		update_angles=true;
		//else sprintf_s(oapiDebugString(), 255, "%s ERROR: MoveEE returned false", oapiDebugString());
	}
}

bool RMSSystem::MoveEE(const VECTOR3 &newPos, const VECTOR3 &newDir, const VECTOR3 &newRot, double DeltaT)
{
	// NOTE: for cross products, |a x b|=|a||b|sin(theta)
	// all vectors obtained as cross products should be normalized

	double new_joint_angles[6]; // angles in degrees

	VECTOR3 arm_wy_cpos=newPos-newDir*RMS_WY_EE_DIST;
	double yaw_angle=atan2(arm_wy_cpos.y, arm_wy_cpos.x);

	VECTOR3 boom_plane=_V(cos(yaw_angle), sin(yaw_angle), 0.0);
	//find vector in XY plane and perpendicular to arm booms
	VECTOR3 normal=crossp(boom_plane, _V(0.0, 0.0, 1.0));
	normal/=length(normal);

	//double beta_w;
	if((newDir.z>normal.z && newDir.z<boom_plane.z) || (newDir.z<normal.z && newDir.z>boom_plane.z))
		new_joint_angles[WRIST_YAW] = -DEG*acos(dotp(normal, newDir))+90.0;
	else new_joint_angles[WRIST_YAW] = -90.0+DEG*acos(dotp(normal, newDir));

	double phi;
	//find vector in same plane as arm booms and perpendicular to EE direction
	VECTOR3 wp_normal=crossp(normal, newDir);
	if(Eq(length(wp_normal), 0.0, 0.001)) //check if newAtt is perpendicular to arm booms
	{
		// use same phi angle as for previous position
		phi=joint_angle[SHOULDER_PITCH]+joint_angle[ELBOW_PITCH]+joint_angle[WRIST_PITCH];
		wp_normal=RotateVectorZ(boom_plane, phi+90.0);
	}
	else {
		wp_normal/=length(wp_normal);
		phi=DEG*acos(wp_normal.z);
		if(newDir.z>0.0) phi=-phi;
	}
	/*sprintf_s(oapiDebugString(), 255, "normal: %f %f %f wp_normal: %f %f %f phi: %f, beta_w: %f", normal.x, normal.y, normal.z, wp_normal.x, wp_normal.y, wp_normal.z,
		phi, beta_w);*/

	new_joint_angles[WRIST_ROLL]=-acos(dotp(wp_normal, newRot))*DEG;
	//if((newRot.x>wp_normal.x && newRot.y<wp_normal.y) || (newRot.x<wp_normal.x && newRot.y>wp_normal.y))
	VECTOR3 cross_product=crossp(wp_normal, newRot);
	if(Eq(1.0, dotp(cross_product/length(cross_product), newDir), 0.05))
		new_joint_angles[WRIST_ROLL]=-new_joint_angles[WRIST_ROLL];

	VECTOR3 arm_wp_dir=crossp(wp_normal, normal); // wp_normal and normal vectors are perpendicular
	VECTOR3 arm_wp_cpos=arm_wy_cpos-arm_wp_dir*RMS_WP_WY_DIST;
	double r=length(arm_wp_cpos);

	new_joint_angles[SHOULDER_YAW] = -DEG*yaw_angle;
	double rho=sqrt(arm_wp_cpos.x*arm_wp_cpos.x+arm_wp_cpos.y*arm_wp_cpos.y);
	double cos_phibar_e=(r*r-RMS_SP_EP_DIST*RMS_SP_EP_DIST-RMS_EP_WP_DIST*RMS_EP_WP_DIST)/(-2*RMS_SP_EP_DIST*RMS_EP_WP_DIST);
	if (fabs(cos_phibar_e)>1) return false;//Can't reach new point with the elbow
	new_joint_angles[ELBOW_PITCH]=DEG*acos(cos_phibar_e)-180.0-RMS_EP_NULL_ANGLE-RMS_SP_NULL_ANGLE;
	double cos_phi_s2=(RMS_EP_WP_DIST*RMS_EP_WP_DIST-RMS_SP_EP_DIST*RMS_SP_EP_DIST-r*r)/(-2*RMS_SP_EP_DIST*r);
	if(fabs(cos_phi_s2)>1) return false; //Can't reach with shoulder
	new_joint_angles[SHOULDER_PITCH]=DEG*(-atan2(arm_wp_cpos.z,rho)+acos(cos_phi_s2))+RMS_SP_NULL_ANGLE;

	new_joint_angles[WRIST_PITCH]=phi-new_joint_angles[SHOULDER_PITCH]-new_joint_angles[ELBOW_PITCH];

	// check values are within bounds
	// make sure speed of each joint is within limits
	for(int i=SHOULDER_YAW;i<=WRIST_ROLL;i++)
	{
		//if(new_joint_angles[i]<RMS_JOINT_SOFTSTOPS[0][i] || new_joint_angles[i]>RMS_JOINT_SOFTSTOPS[1][i]) return false;
		if(new_joint_angles[i]<RMS_JOINT_SOFTSTOPS[0][i] || new_joint_angles[i]>RMS_JOINT_SOFTSTOPS[1][i]) {
			sprintf_s(oapiDebugString(), 255, "Error: joint %d reached angle limit %f", i, new_joint_angles[i]);
			return false;
		}
		double speed = abs(new_joint_angles[i]-joint_angle[i])/DeltaT;
		//if(speed > RMS_JOINT_MAX_ROTATION_SPEED[i]) return false;
		if(speed > RMS_JOINT_MAX_ROTATION_SPEED[i]) {
			sprintf_s(oapiDebugString(), 255, "Error: joint %d reached speed limit %f", i, speed);
			return false;
		}
	}

	for(int i=SHOULDER_YAW;i<=WRIST_ROLL;i++) {
		SetJointAngle(static_cast<RMS_JOINT>(i), new_joint_angles[i]);
	}

	arm_ee_pos=newPos;
	arm_ee_dir=newDir;
	arm_ee_rot=newRot;

	return true;
}

void RMSSystem::SetJointAngle(RMS_JOINT joint, double angle)
{
	double pos=linterp(RMS_JOINT_LIMITS[0][joint], 0.0, RMS_JOINT_LIMITS[1][joint], 1.0, angle);
	//sprintf_s(oapiDebugString(), 255, "Joint Angle: %f %f", angle, pos);
	if(pos>=0.0 && pos<=1.0) {
		STS()->SetAnimation(anim_joint[joint], pos);
		joint_pos[joint]=pos;
		joint_angle[joint]=angle;
		JointAngles[joint].SetLine(static_cast<float>(5.0*joint_angle[joint]/9999.0));
		arm_moved=true;
	}
}

void RMSSystem::SetJointPos(RMS_JOINT joint, double pos)
{
	if(pos>=0.0 && pos<=1.0) {
		STS()->SetAnimation(anim_joint[joint], pos);
		joint_pos[joint]=pos;
		joint_angle[joint]=linterp(0.0, RMS_JOINT_LIMITS[0][joint], 1.0, RMS_JOINT_LIMITS[1][joint], pos);
		JointAngles[joint].SetLine(static_cast<float>(5.0*joint_angle[joint]/9999.0));
		arm_moved=true;
	}
}

int RMSSystem::GetSelectedJoint() const
{
	for(int i=0;i<6;i++) {
		if(JointSelect[i]) return i;
	}
	return -1;
}

OBJHANDLE RMSSystem::Grapple()
{
	VESSEL* pVessel=NULL;
	ATTACHMENTHANDLE hAtt=FindPayload(&pVessel);
	if(hAtt) AttachPayload(pVessel, hAtt);

	if(pVessel) return pVessel->GetHandle();
	return NULL;
}

void RMSSystem::OnMRLLatched()
{
	if(ArmStowed()) {
		for(int i=0;i<6;i++) SetJointAngle((RMS_JOINT)i, 0.0);
	}
}

void RMSSystem::OnAttach()
{
	EECapture.SetLine();
}

void RMSSystem::OnDetach()
{
	EECapture.ResetLine();
}

bool RMSSystem::ArmStowed() const
{
	if(!Eq(joint_angle[SHOULDER_YAW], 0.0, MRL_MAX_ANGLE_ERROR)) return false;
	if(!Eq(joint_angle[SHOULDER_PITCH], 0.0, MRL_MAX_ANGLE_ERROR)) return false;
	if(!Eq(joint_angle[ELBOW_PITCH], 0.0, MRL_MAX_ANGLE_ERROR)) return false;
	if(!Eq(joint_angle[WRIST_PITCH], 0.0, MRL_MAX_ANGLE_ERROR)) return false;
	if(!Eq(joint_angle[WRIST_YAW], 0.0, MRL_MAX_ANGLE_ERROR)) return false;
	if(!Eq(joint_angle[WRIST_ROLL], 0.0, MRL_MAX_ANGLE_ERROR)) return false;
	return true;
}

void RMSSystem::SetEECameraView(bool Active)
{
	if(Active) {
		RMSCameraMode=EE;
		UpdateEECamView();
	}
	else if(RMSCameraMode==EE) RMSCameraMode=NONE;
}

void RMSSystem::SetElbowCamView(bool Active)
{
	if(Active) {
		RMSCameraMode=ELBOW;
		UpdateElbowCamView();
	}
	else if(RMSCameraMode==ELBOW) RMSCameraMode=NONE;
}

void RMSSystem::UpdateEECamView() const
{
	if(oapiCameraInternal()) {
		// calculate rotation angle for EE cam
		VECTOR3 dir = arm_tip[1]-arm_tip[0];
		// if camera is pointing straight up or down, make it slightly offset from (0,1,0) vector
		if(Eq(dotp(dir, _V(0, -1, 0)), 1.0, 1e-4)) dir = _V(1.74532924314e-4, -0.999999984769, 0.0);
		else if(Eq(dotp(dir, _V(0, 1, 0)), 1.0, 1e-4)) dir = _V(1.74532924314e-4, 0.999999984769, 0.0);
		VECTOR3 orbiter_cam_rot = crossp(crossp(dir, _V(0, 1, 0)), dir);
		orbiter_cam_rot /= length(orbiter_cam_rot);
		if(orbiter_cam_rot.y < 0) orbiter_cam_rot = -orbiter_cam_rot;
		double angle = SignedAngle(orbiter_cam_rot, arm_tip[2]-arm_tip[0], dir);

		//sprintf_s(oapiDebugString(), 255, "Rot Vec: %f %f %f dir: %f %f %f dot_prod: %f Angle: %f %f", orbiter_cam_rot.x, orbiter_cam_rot.y, orbiter_cam_rot.z, dir.x, dir.y, dir.z, dot_prod, angle, angle*DEG);
		sprintf_s(oapiDebugString(), 255, "Rot Vec: %f %f %f cam dir: %f %f %f dir: %f %f %f Angle: %f %f length: %f", orbiter_cam_rot.x, orbiter_cam_rot.y, orbiter_cam_rot.z, arm_tip[2].x-arm_tip[0].x, arm_tip[2].y-arm_tip[0].y, arm_tip[2].z-arm_tip[0].z,  dir.x, dir.y, dir.z, angle, angle*DEG, length(dir));
		//sprintf_s(oapiDebugString(), 255, "dot_prod: %f Angle: %f %f", dot_prod, angle, angle*DEG);

		STS()->SetCameraOffset(STS()->GetOrbiterCoGOffset()+arm_tip[4]+RMS_MESH_OFFSET);
		//STS()->SetCameraDefaultDirection (arm_tip[1]-arm_tip[0], 0.0);
		STS()->SetCameraDefaultDirection (dir, angle);
		oapiCameraSetCockpitDir(0.0, 0.0);
	}
}

void RMSSystem::UpdateElbowCamView() const
{
	if(oapiCameraInternal()) {
		STS()->SetCameraDefaultDirection(camRMSElbowLoc[1]-camRMSElbowLoc[0]);
		STS()->SetCameraOffset(camRMSElbowLoc[0]+RMS_MESH_OFFSET);
		oapiCameraSetCockpitDir(0.0, 0.0);
	}
}

void RMSSystem::ToggleJointAngleDisplay()
{
	display_angles=!display_angles;
	if(!display_angles) sprintf_s(oapiDebugString(), 255, "");
}

void RMSSystem::AutoGrappleSequence()
{
	if(bAutoGrapple) {
		if(!Grapple_State.Closed()) {
			Grapple_State.action=AnimState::CLOSING;
			if(Extend_State.Moving()) Extend_State.action=AnimState::STOPPED;
			if(Rigid_State.Moving()) Rigid_State.action=AnimState::STOPPED;
			sprintf_s(oapiDebugString(), 255, "Grappling");
		}
		else if(!Extend_State.Closed()) {
			Extend_State.action=AnimState::CLOSING;
			if(Rigid_State.Moving()) Rigid_State.action=AnimState::STOPPED;
		}
		else if(!Rigid_State.Closed()) Rigid_State.action=AnimState::CLOSING;
		else bAutoGrapple=false;
	}
}

void RMSSystem::AutoReleaseSequence()
{
	if(bAutoRelease) {
		if(!Rigid_State.Open()) {
			Rigid_State.action=AnimState::OPENING;
			if(Grapple_State.Moving()) Grapple_State.action=AnimState::STOPPED;
			if(Extend_State.Moving()) Extend_State.action=AnimState::STOPPED;
		}
		else if(!Grapple_State.Open()) {
			Grapple_State.action=AnimState::OPENING;
			if(Extend_State.Moving()) Extend_State.action=AnimState::STOPPED;
		}
		else if(!Extend_State.Open()) Extend_State.action=AnimState::OPENING;
		else bAutoRelease=false;
	}
}
