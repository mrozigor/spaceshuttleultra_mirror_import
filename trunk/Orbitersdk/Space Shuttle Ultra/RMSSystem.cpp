#include "RMSSystem.h"
#include "ParameterValues.h"
#include "meshres_RMS.h"
#include <UltraMath.h>

RMSSystem::RMSSystem(AtlantisSubsystemDirector *_director)
	: MPMSystem(_director, "RMS", RMS_MESHNAME, _V(0.0, 0.0, 0.0), "G"), RMSCameraMode(NONE)
{
	joint_pos[SHOULDER_YAW] = 0.5;
	joint_pos[SHOULDER_PITCH] = 0.0136;
	joint_pos[ELBOW_PITCH] = 0.985312;
	joint_pos[WRIST_PITCH] = 0.5;
	joint_pos[WRIST_YAW] = 0.5;
	joint_pos[WRIST_ROLL] = 0.5;
	arm_tip[0] = RMS_EE_POS;
	arm_tip[1] = RMS_EE_POS+_V(0.0, 0.0, -1.0); // -Z coordinate of attachment point is negative, so subtract 1 here
	arm_tip[2] = RMS_EE_POS+_V(0.0, 1.0, 0.0);
	arm_tip[3] = RMS_EE_POS+RotateVectorZ(_V(0.0, 1.0, 0.0), RMS_ROLLOUT_ANGLE);
	arm_tip[4] = RMS_EE_POS+RMS_EE_CAM_OFFSET;
	arm_ee_dir = _V(1.0, 0.0, 0.0);
	arm_ee_pos = _V(15.069, 0.0, 0.0);
	arm_ee_rot = _V(0.0, 1.0, 0.0);
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

	//rollout animation
	static UINT RMSRolloutGrp[2] = {GRP_RMS_MPMs, GRP_base};
	MGROUP_ROTATE* pRMS_rollout_anim = new MGROUP_ROTATE(mesh_index, RMSRolloutGrp, 2,
		_V(-2.643, 1.282, 0.0), _V(0, 0, 1), (float)(31.36*RAD));
	anim_mpm = STS()->CreateAnimation(1.0);
	ANIMATIONCOMPONENT_HANDLE parent = STS()->AddManagedAnimationComponent(anim_mpm, 0, 1, pRMS_rollout_anim);

	//shoulder yaw
	static UINT RMSShoulderYawGrp[1] = {GRP_Shoulder_Yaw};
	MGROUP_ROTATE* pRMS_sy_anim = new MGROUP_ROTATE(mesh_index, RMSShoulderYawGrp, 1,
		RMS_SY_JOINT, _V(-0.321040041302228, 0.947065621739415, 0), (float)(-360*RAD)); // -180 .. +180
	anim_joint[SHOULDER_YAW] = STS()->CreateAnimation (0.5);
	parent = STS()->AddManagedAnimationComponent (anim_joint[SHOULDER_YAW], 0, 1, pRMS_sy_anim, parent);

	//shoulder pitch
	static UINT RMSShoulderPitchGrp[1] = {GRP_Humerus};
	MGROUP_ROTATE* pRMS_sp_anim = new MGROUP_ROTATE(mesh_index, RMSShoulderPitchGrp, 1,
		RMS_SP_JOINT, _V(0.948683298050514, 0.316227766016838, 0), (float)(147.0*RAD)); // -2 .. +145
	anim_joint[SHOULDER_PITCH] = STS()->CreateAnimation (0.0136);
	parent = STS()->AddManagedAnimationComponent (anim_joint[SHOULDER_PITCH], 0, 1, pRMS_sp_anim, parent);

	//elbow pitch
	static UINT RMSElbowPitchGrp[2] = {GRP_Radii, GRP_cambase};
	MGROUP_ROTATE* pRMS_ep_anim = new MGROUP_ROTATE(mesh_index, RMSElbowPitchGrp, 2,
		RMS_EP_JOINT, _V(0.948683598, 0.316226863954669, 0), (float)(163.4*RAD));
	anim_joint[ELBOW_PITCH] = STS()->CreateAnimation (0.985312);
	parent = STS()->AddManagedAnimationComponent (anim_joint[ELBOW_PITCH], 0, 1, pRMS_ep_anim, parent);

	//RMS elbow camera
	static UINT RMSElbowCamGrp[2] = {GRP_elbowcam, GRP_camswivel};
	MGROUP_ROTATE* pRMSElbowCamPan = new MGROUP_ROTATE(mesh_index, RMSElbowCamGrp+1, 1,
		_V(-2.765, 2.373, 2.073), _V(0.2974, 0.95475, 0), (float)(340*RAD));
	ANIMATIONCOMPONENT_HANDLE parent2;
	anim_camRMSElbow[PAN]=STS()->CreateAnimation(0.5);
	parent2 = STS()->AddManagedAnimationComponent (anim_camRMSElbow[PAN], 0, 1, pRMSElbowCamPan, parent);
	MGROUP_ROTATE* pRMSElbowCamTilt = new MGROUP_ROTATE(mesh_index, RMSElbowCamGrp, 1,
		_V(-2.68, 2.64, 2.073), _V(0.9513, -0.3082, 0), (float)(340*RAD));
	anim_camRMSElbow[TILT]=STS()->CreateAnimation(0.5);
	parent2 = STS()->AddManagedAnimationComponent(anim_camRMSElbow[TILT], 0, 1, pRMSElbowCamTilt, parent2);
	MGROUP_ROTATE* pRMSElbowCamLoc = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(camRMSElbowLoc), 2,
		_V(-2.765, 2.373, 2.073), _V(1, 0, 0), 0.0f);
	STS()->AddManagedAnimationComponent(anim_camRMSElbow[TILT], 0, 1, pRMSElbowCamLoc, parent2);

	//wrist pitch
	static UINT RMSWristPitchGrp[1] = {GRP_Wristpitch};
	MGROUP_ROTATE* pRMS_wp_anim = new MGROUP_ROTATE(mesh_index, RMSWristPitchGrp, 1,
		RMS_WP_JOINT, _V(0.949637404032871, 0.313350922867173, 0), (float)(242.8*RAD)); // -121.4 .. +121.4
	anim_joint[WRIST_PITCH] = STS()->CreateAnimation (0.5);
	parent = STS()->AddManagedAnimationComponent (anim_joint[WRIST_PITCH], 0, 1, pRMS_wp_anim, parent);

	//wrist yaw
	static UINT RMSWristYawGrp[1] = {GRP_Wrist_Yaw};
	MGROUP_ROTATE* pRMS_wy_anim = new MGROUP_ROTATE(mesh_index, RMSWristYawGrp, 1,
		RMS_WY_JOINT, _V(0.314338082679218, -0.949311102735849, 0), (float)(-242.6*RAD)); // -121.3 .. +121.3
	anim_joint[WRIST_YAW] = STS()->CreateAnimation (0.5);
	parent = STS()->AddManagedAnimationComponent (anim_joint[WRIST_YAW], 0, 1, pRMS_wy_anim, parent);

	//wrist roll
	static UINT RMSEndEffectorGrp[1] = {GRP_Endeffector};
	MGROUP_ROTATE* pRMS_wr_anim = new MGROUP_ROTATE(mesh_index, RMSEndEffectorGrp, 1,
		_V(-2.844, 2.133, -5.52), _V(0, 0, 1), (float)(894*RAD));  // -447 .. +447
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
		hAttach=STS()->CreateAttachment(false, STS()->GetOrbiterCoGOffset()+arm_tip[0], arm_tip[1]-arm_tip[0], arm_tip[2]-arm_tip[0], "G", true);
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
			// EE rotation
			VECTOR3 change=_V(0.0, 0.0, 0.0);
			bool moveEE=false;
			for(int i=0;i<3;i++) {
				if(!Eq(RHCInput[i].GetVoltage(), 0.0, 0.05)) {
					//change.data[i]+=(RHCInput[i].GetVoltage()/5.0)*DeltaT*RMS_EE_ROTATION_SPEED;
					if(!RMSSpeed) change.data[i]+=(RHCInput[i].GetVoltage()/5.0)*DeltaT*RMS_EE_COARSE_ROTATION_SPEED;
					else change.data[i]+=(RHCInput[i].GetVoltage()/5.0)*DeltaT*RMS_EE_VERN_ROTATION_SPEED;
					moveEE=true;
				}
			}
			if(moveEE) Rotate(change);

			// EE translation
			change=_V(0.0, 0.0, 0.0);
			moveEE=false;
			for(int i=0;i<3;i++) {
				if(!Eq(THCInput[i].GetVoltage(), 0.0, 0.05)) {
					//change.data[i]+=(THCInput[i].GetVoltage()/5.0)*DeltaT*RMS_EE_TRANSLATION_SPEED;
					if(!RMSSpeed) change.data[i]+=(THCInput[i].GetVoltage()/5.0)*DeltaT*RMS_EE_COARSE_TRANSLATION_SPEED;
					else change.data[i]+=(THCInput[i].GetVoltage()/5.0)*DeltaT*RMS_EE_VERN_TRANSLATION_SPEED;
					moveEE=true;
				}
				else if(ee_translation[i]!=0) {
					//change.data[i]+=ee_translation[i]*DeltaT*RMS_EE_TRANSLATION_SPEED;
					if(!RMSSpeed) change.data[i]+=ee_translation[i]*DeltaT*RMS_EE_COARSE_TRANSLATION_SPEED;
					else change.data[i]+=ee_translation[i]*DeltaT*RMS_EE_VERN_TRANSLATION_SPEED;
					ee_translation[i]=0;
					moveEE=true;
				}
			}
			if(moveEE) Translate(change);
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

	if(arm_moved) {
		if(hAttach) STS()->SetAttachmentParams(hAttach, STS()->GetOrbiterCoGOffset()+arm_tip[0], arm_tip[1]-arm_tip[0], arm_tip[2]-arm_tip[0]);

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
		VECTOR3 ee_att_output = GetPYRAnglesFromMatrix(arm_ee_dir_mat);
		// reference frame is a bit odd here, so we need this to get the math to work
		ee_att_output.data[PITCH]=-ee_att_output.data[PITCH];
		for(int i=0;i<3;i++) {
			if(ee_att_output.data[i]<0.0) ee_att_output.data[i]+=2*PI;

			EEPosition[i].SetLine((float)(ee_pos_output.data[i]/2000.0));
			EEAttitude[i].SetLine((float)(DEG*ee_att_output.data[i]/2000.0));
		}
		if(update_angles) {
			arm_ee_angles=ee_att_output;
			update_angles=false;
		}

		if(update_vectors) {
			arm_ee_dir=RotateVectorZ(arm_tip[1]-arm_tip[0], -RMS_ROLLOUT_ANGLE);
			arm_ee_dir=_V(-arm_ee_dir.z, -arm_ee_dir.x, arm_ee_dir.y);
			//sprintf_s(oapiDebugString(), 255, "Calculated dir: %f %f %f", arm_ee_dir.x, arm_ee_dir.y, arm_ee_dir.z);

			arm_ee_rot=RotateVectorZ(arm_tip[3]-arm_tip[0], -RMS_ROLLOUT_ANGLE);
			arm_ee_rot=_V(-arm_ee_rot.z, -arm_ee_rot.x, arm_ee_rot.y);
			//sprintf_s(oapiDebugString(), 255, "Calculated rot: %f %f %f", arm_ee_rot.x, arm_ee_rot.y, arm_ee_rot.z);

			//arm_ee_pos=RotateVectorZ(_V(-2.84, 2.13, 9.02)-arm_tip[0], -18.435);
			arm_ee_pos=RotateVectorZ(RMS_SP_JOINT-arm_tip[0], -RMS_ROLLOUT_ANGLE);
			arm_ee_pos=_V(arm_ee_pos.z, arm_ee_pos.x, -arm_ee_pos.y);
			//sprintf_s(oapiDebugString(), 255, "Calculated EE pos: %f %f %f", arm_ee_pos.x, arm_ee_pos.y, arm_ee_pos.z);

			update_vectors=false;
		}

		arm_moved=false;
	}
	else if(camera_moved /*&& RMSCameraMode==ELBOW*/) {
		double anim=linterp(-MAX_PLBD_CAM_PAN, 0, MAX_PLBD_CAM_PAN, 1, camRMSElbow[PAN]);
		STS()->SetAnimation(anim_camRMSElbow[PAN], anim);
		anim=linterp(-MAX_PLBD_CAM_TILT, 0, MAX_PLBD_CAM_TILT, 1, camRMSElbow[TILT]);
		STS()->SetAnimation(anim_camRMSElbow[TILT], anim);

		if(RMSCameraMode==ELBOW) UpdateElbowCamView();
		camera_moved=false;
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

void RMSSystem::TranslateEE(const VECTOR3 &direction)
{
	for (int i=0;i<3;i++) {
		if(direction.data[i]>0.25) ee_translation[i]=1;
		else if(direction.data[i]<-0.25) ee_translation[i]=-1;
		else ee_translation[i]=0;
	}
}

/*void RMSSystem::RotateElbowCam(int pitch, int yaw)
{
	camRMSElbow_rotation[PAN]=yaw;
	camRMSElbow_rotation[TILT]=pitch;
}*/

/*void RMSSystem::SetElbowCamRotSpeed(bool low)
{
	camLowSpeed=low;
}*/

void RMSSystem::Translate(const VECTOR3 &dPos)
{
	if(RMSMode[5].IsSet()) { // END EFF
		/*VECTOR3 change=RotateVectorX(arm_ee_dir, RMS_ROLLOUT_ANGLE)*dPos.x;
		change+=RotateVectorX(arm_ee_rot, RMS_ROLLOUT_ANGLE)*dPos.z;
		change+=RotateVectorX(crossp(arm_ee_rot, arm_ee_dir), RMS_ROLLOUT_ANGLE)*dPos.y;
		//RotateVectorX(change, -RMS_ROLLOUT_ANGLE);*/

		VECTOR3 cdPos=RotateVectorX(dPos, -RMS_ROLLOUT_ANGLE);
		VECTOR3 change=arm_ee_dir*cdPos.x+arm_ee_rot*cdPos.z+crossp(arm_ee_rot, arm_ee_dir)*cdPos.y;
		MoveEE(arm_ee_pos+change, arm_ee_dir, arm_ee_rot);
	}
	else if(RMSMode[6].IsSet()) { // ORB LD
		MoveEE(arm_ee_pos+RotateVectorX(dPos, -RMS_ROLLOUT_ANGLE), arm_ee_dir, arm_ee_rot);
	}
}

void RMSSystem::Rotate(const VECTOR3 &dAngles)
{
	static const VECTOR3 inDir=_V(0.0, 0.0, -1.0);
	static const VECTOR3 inRot=RotateVectorZ(_V(0.0, 1.0, 0.0), RMS_ROLLOUT_ANGLE);
	VECTOR3 newDir, newRot;

	if(RMSMode[5].IsSet()) { // END EFF mode
		VECTOR3 ee_dir=RotateVectorX(arm_ee_dir, RMS_ROLLOUT_ANGLE);
		// NOTE: for math to work, we need to swap yaw and pitch angles
		RotateVectorPYR(ee_dir, _V(dAngles.data[ROLL], dAngles.data[PITCH], dAngles.data[YAW]), newDir);
		newDir=RotateVectorX(newDir, -RMS_ROLLOUT_ANGLE);

		VECTOR3 ee_rot=RotateVectorX(arm_ee_rot, RMS_ROLLOUT_ANGLE);
		RotateVectorPYR(ee_rot, _V(dAngles.data[ROLL], dAngles.data[PITCH], dAngles.data[YAW]), newRot);
		newRot=RotateVectorX(newRot, -RMS_ROLLOUT_ANGLE);

		MoveEE(arm_ee_pos, newDir, newRot);
		update_angles=true;
	}

	else if(RMSMode[6].IsSet()) { // ORB LD
		VECTOR3 newAngles=arm_ee_angles+dAngles;
		RotateVectorPYR(inDir, _V(-newAngles.data[PITCH], newAngles.data[YAW], newAngles.data[ROLL]), newDir);
		RotateVectorPYR(inRot, _V(-newAngles.data[PITCH], newAngles.data[YAW], newAngles.data[ROLL]), newRot);
		newDir=_V(-newDir.z, -newDir.x, newDir.y);
		newRot=_V(-newRot.z, -newRot.x, newRot.y);
		newDir=RotateVectorX(newDir, -RMS_ROLLOUT_ANGLE);
		newRot=RotateVectorX(newRot, -RMS_ROLLOUT_ANGLE);

		if(MoveEE(arm_ee_pos, newDir, newRot)) arm_ee_angles=newAngles;
		//else sprintf_s(oapiDebugString(), 255, "%s ERROR: MoveEE returned false", oapiDebugString());
	}
}

bool RMSSystem::MoveEE(const VECTOR3 &newPos, const VECTOR3 &newDir, const VECTOR3 &newRot)
{
	// NOTE: for cross products, |a x b|=|a||b|sin(theta)
	// all vectors obtained as cross products should be normalized

	VECTOR3 arm_wy_cpos=newPos-newDir*RMS_WY_EE_DIST;
	double yaw_angle=atan2(arm_wy_cpos.y, arm_wy_cpos.x);

	VECTOR3 boom_plane=_V(cos(yaw_angle), sin(yaw_angle), 0.0);
	//find vector in XY plane and perpendicular to arm booms
	VECTOR3 normal=crossp(boom_plane, _V(0.0, 0.0, 1.0));
	normal/=length(normal);

	double beta_w;
	if((newDir.z>normal.z && newDir.z<boom_plane.z) || (newDir.z<normal.z && newDir.z>boom_plane.z))
		beta_w=-DEG*acos(dotp(normal, newDir))+90.0;
	else beta_w=-90.0+DEG*acos(dotp(normal, newDir));

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
		if(newDir.z<0.0) phi=-phi;
	}
	/*sprintf_s(oapiDebugString(), 255, "normal: %f %f %f wp_normal: %f %f %f phi: %f, beta_w: %f", normal.x, normal.y, normal.z, wp_normal.x, wp_normal.y, wp_normal.z,
		phi, beta_w);*/

	double theta_w; // wrist roll
	theta_w=acos(dotp(wp_normal, newRot))*DEG;
	//if((newRot.x>wp_normal.x && newRot.y<wp_normal.y) || (newRot.x<wp_normal.x && newRot.y>wp_normal.y))
	VECTOR3 cross_product=crossp(wp_normal, newRot);
	if(Eq(1.0, dotp(cross_product/length(cross_product), newDir), 0.05))
		theta_w=-theta_w;

	VECTOR3 arm_wp_dir=crossp(wp_normal, normal); // wp_normal and normal vectors are perpendicular
	VECTOR3 arm_wp_cpos=arm_wy_cpos-arm_wp_dir*RMS_WP_WY_DIST;
	double r=length(arm_wp_cpos);

	double beta_s=-DEG*yaw_angle;
	double rho=sqrt(arm_wp_cpos.x*arm_wp_cpos.x+arm_wp_cpos.y*arm_wp_cpos.y);
	double cos_phibar_e=(r*r-RMS_SP_EP_DIST*RMS_SP_EP_DIST-RMS_EP_WP_DIST*RMS_EP_WP_DIST)/(-2*RMS_SP_EP_DIST*RMS_EP_WP_DIST);
	if (fabs(cos_phibar_e)>1) return false;//Can't reach new point with the elbow
	double phi_e=DEG*acos(cos_phibar_e)-180.0-RMS_EP_NULL_ANGLE-RMS_SP_NULL_ANGLE;
	double cos_phi_s2=(RMS_EP_WP_DIST*RMS_EP_WP_DIST-RMS_SP_EP_DIST*RMS_SP_EP_DIST-r*r)/(-2*RMS_SP_EP_DIST*r);
	if(fabs(cos_phi_s2)>1) return false; //Can't reach with shoulder
	double phi_s=DEG*(atan2(arm_wp_cpos.z,rho)+acos(cos_phi_s2))+RMS_SP_NULL_ANGLE;

	//make sure values calculated are within bounds
	if(beta_s<RMS_JOINT_SOFTSTOPS[0][SHOULDER_YAW] || beta_s>RMS_JOINT_SOFTSTOPS[1][SHOULDER_YAW]) return false;
	if(phi_s<RMS_JOINT_SOFTSTOPS[0][SHOULDER_PITCH] || phi_s>RMS_JOINT_SOFTSTOPS[1][SHOULDER_PITCH]) return false;
	if(phi_e<RMS_JOINT_SOFTSTOPS[0][ELBOW_PITCH] || phi_e>RMS_JOINT_SOFTSTOPS[1][ELBOW_PITCH]) return false;

	//wrist pitch
	double phi_w=phi-phi_s-phi_e;

	//check values are within bounds
	if(phi_w<RMS_JOINT_SOFTSTOPS[0][WRIST_PITCH] || phi_w>RMS_JOINT_SOFTSTOPS[1][WRIST_PITCH]) return false;
	if(beta_w<RMS_JOINT_SOFTSTOPS[0][WRIST_YAW] || beta_w>RMS_JOINT_SOFTSTOPS[1][WRIST_YAW]) return false;

	SetJointAngle(SHOULDER_YAW, beta_s);
	SetJointAngle(SHOULDER_PITCH, phi_s);
	SetJointAngle(ELBOW_PITCH, phi_e);
	SetJointAngle(WRIST_PITCH, phi_w);
	SetJointAngle(WRIST_YAW, beta_w);
	SetJointAngle(WRIST_ROLL, theta_w);

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
	double tilt = joint_angle[WRIST_ROLL];
	if(tilt<-180.0) tilt+=360.0;
	else if(tilt>180.0) tilt-=360.0;
	STS()->SetCameraOffset(STS()->GetOrbiterCoGOffset()+arm_tip[4]);
	STS()->SetCameraDefaultDirection (arm_tip[1]-arm_tip[0], 0.0);
}

void RMSSystem::UpdateElbowCamView() const
{
	STS()->SetCameraDefaultDirection(camRMSElbowLoc[1]-camRMSElbowLoc[0]);
	STS()->SetCameraOffset(camRMSElbowLoc[0]);
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
