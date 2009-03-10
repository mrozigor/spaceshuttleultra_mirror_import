#include "RMSSystem.h"
#include "meshres_RMS.h"

extern double linterp(double x0, double y0, double x1, double y1, double x);

RMSSystem::RMSSystem(SubsystemDirector *_director)
	: MPMSystem(_director, "RMS", RMS_MESHNAME, _V(0.0, 0.0, 0.0), "G"), RMSCameraMode(NONE), camLowSpeed(false)
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
	arm_tip[3] = RMS_EE_POS+RMS_EE_CAM_OFFSET;
	arm_ee_dir = _V(1.0, 0.0, 0.0);
	arm_ee_pos = _V(15.069, 0.0, 0.0);

	Grapple_State.Set(AnimState::OPEN, 1);
	Rigid_State.Set(AnimState::OPEN, 1);
	Extend_State.Set(AnimState::OPEN, 1);

	//RMS elbow camera
	camRMSElbowLoc[0]=RMS_ELBOW_CAM_POS;
	camRMSElbowLoc[1]=camRMSElbowLoc[0]+_V(0, 0, -1);
	camRMSElbow_rotation[0]=camRMSElbow_rotation[1]=0;
	camera_moved=false;

	arm_moved=false;
	update_data=false;
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

	CreateArm();
	//MPM animation is only added in CreateArm function, so we have to set initial MPM position here
	STS()->SetAnimation(anim_mpm, MPMRollout.pos);

	DiscreteBundle* pBundle=BundleManager()->CreateBundle("RMS_EE", 16);
	EEGrapple.Connect(pBundle, 0);
	EERelease.Connect(pBundle, 1);
	EEAuto.Connect(pBundle, 2);
	EEMan.Connect(pBundle, 3);
	EERigid.Connect(pBundle, 4);
	EEDerigid.Connect(pBundle, 5);
}

void RMSSystem::CreateArm()
{
	//rollout animation
	static UINT RMSRolloutGrp[11] = {GRP_RMS_MPMs, GRP_base, GRP_Shoulder_Yaw, GRP_Humerus, GRP_Radii, GRP_elbowcam, GRP_camswivel, GRP_cambase, GRP_Wristpitch, GRP_Wrist_Yaw, GRP_Endeffector};
	static MGROUP_ROTATE rms_rollout_anim(mesh_index, RMSRolloutGrp, 11,
		_V(-2.643, 1.282, 0.0), _V(0, 0, 1), (float)(31.36*RAD)); //1.05 or 1.10
	anim_mpm = STS()->CreateAnimation(1.0);
	STS()->AddAnimationComponent(anim_mpm, 0, 1, &rms_rollout_anim);

	//shoulder yaw
	static UINT RMSShoulderYawGrp[1] = {GRP_Shoulder_Yaw};
	static MGROUP_ROTATE rms_sy_anim(mesh_index, RMSShoulderYawGrp, 1,
		RMS_SY_JOINT, _V(-0.321040041302228, 0.947065621739415, 0), (float)(-360*RAD)); // -180 .. +180
	anim_joint[SHOULDER_YAW] = STS()->CreateAnimation (0.5);
	ANIMATIONCOMPONENT_HANDLE parent = STS()->AddAnimationComponent (anim_joint[SHOULDER_YAW], 0, 1, &rms_sy_anim);

	//shoulder pitch
	static UINT RMSShoulderPitchGrp[1] = {GRP_Humerus};
	static MGROUP_ROTATE rms_sp_anim(mesh_index, RMSShoulderPitchGrp, 1,
		RMS_SP_JOINT, _V(0.948683298050514, 0.316227766016838, 0), (float)(147.0*RAD)); // -2 .. +145
	anim_joint[SHOULDER_PITCH] = STS()->CreateAnimation (0.0136);
	parent = STS()->AddAnimationComponent (anim_joint[SHOULDER_PITCH], 0, 1, &rms_sp_anim, parent);

	//elbow pitch
	static UINT RMSElbowPitchGrp[2] = {GRP_Radii, GRP_cambase};
	static MGROUP_ROTATE rms_ep_anim(mesh_index, RMSElbowPitchGrp, 2,
		RMS_EP_JOINT, _V(0.948683598, 0.316226863954669, 0), (float)(163.4*RAD));
	anim_joint[ELBOW_PITCH] = STS()->CreateAnimation (0.985312);
	parent = STS()->AddAnimationComponent (anim_joint[ELBOW_PITCH], 0, 1, &rms_ep_anim, parent);

	//RMS elbow camera
	static UINT RMSElbowCamGrp[2] = {GRP_elbowcam, GRP_camswivel};
	static MGROUP_ROTATE RMSElbowCamPan (mesh_index, RMSElbowCamGrp+1, 1,
		_V(-2.765, 2.373, 2.073), _V(0.2974, 0.95475, 0), (float)(340*RAD));
	ANIMATIONCOMPONENT_HANDLE parent2;
	anim_camRMSElbow[PAN]=STS()->CreateAnimation(0.5);
	parent2 = STS()->AddAnimationComponent (anim_camRMSElbow[PAN], 0, 1, &RMSElbowCamPan, parent);
	static MGROUP_ROTATE RMSElbowCamTilt (mesh_index, RMSElbowCamGrp, 1,
		_V(-2.68, 2.64, 2.073), _V(0.9513, -0.3082, 0), (float)(340*RAD));
	anim_camRMSElbow[TILT]=STS()->CreateAnimation(0.5);
	parent2 = STS()->AddAnimationComponent(anim_camRMSElbow[TILT], 0, 1, &RMSElbowCamTilt, parent2);
	static MGROUP_ROTATE RMSElbowCamLoc (LOCALVERTEXLIST, MAKEGROUPARRAY(camRMSElbowLoc), 2,
		_V(-2.765, 2.373, 2.073), _V(1, 0, 0), 0.0f);
	STS()->AddAnimationComponent(anim_camRMSElbow[TILT], 0, 1, &RMSElbowCamLoc, parent2);

	//wrist pitch
	static UINT RMSWristPitchGrp[1] = {GRP_Wristpitch};
	static MGROUP_ROTATE rms_wp_anim(mesh_index, RMSWristPitchGrp, 1,
		RMS_WP_JOINT, _V(0.949637404032871, 0.313350922867173, 0), (float)(242.8*RAD)); // -121.4 .. +121.4
	anim_joint[WRIST_PITCH] = STS()->CreateAnimation (0.5);
	parent = STS()->AddAnimationComponent (anim_joint[WRIST_PITCH], 0, 1, &rms_wp_anim, parent);

	//wrist yaw
	static UINT RMSWristYawGrp[1] = {GRP_Wrist_Yaw};
	static MGROUP_ROTATE rms_wy_anim(mesh_index, RMSWristYawGrp, 1,
		RMS_WY_JOINT, _V(0.314338082679218, -0.949311102735849, 0), (float)(-242.6*RAD)); // -121.3 .. +121.3
	anim_joint[WRIST_YAW] = STS()->CreateAnimation (0.5);
	parent = STS()->AddAnimationComponent (anim_joint[WRIST_YAW], 0, 1, &rms_wy_anim, parent);

	//wrist roll
	static UINT RMSEndEffectorGrp[1] = {GRP_Endeffector};
	static MGROUP_ROTATE rms_wr_anim(mesh_index, RMSEndEffectorGrp, 1,
		_V(-2.844, 2.133, -5.52), _V(0, 0, 1), (float)(894*RAD));  // -447 .. +447
	anim_joint[WRIST_ROLL] = STS()->CreateAnimation (0.5);
	parent = STS()->AddAnimationComponent (anim_joint[WRIST_ROLL], 0, 1, &rms_wr_anim, parent);

	static MGROUP_ROTATE rms_ee_anim(LOCALVERTEXLIST, MAKEGROUPARRAY(arm_tip), 4,
		RMS_EE_POS, _V(0,0,1), (float)(0.0));
	anim_rms_ee = STS()->CreateAnimation (0.0);
	STS()->AddAnimationComponent (anim_rms_ee, 0, 1, &rms_ee_anim, parent);

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

void RMSSystem::OnPreStep(double SimT, double DeltaT, double MJD)
{
	MPMSystem::OnPreStep(SimT, DeltaT, MJD);

	//rotate joints
	if(Movable()) {
		//rotation
		for(int i=0;i<6;i++) {
			if(joint_motion[i]!=0) {
				SetJointAngle((RMS_JOINT)i, joint_angle[i]+RMS_JOINT_ROTATION_SPEED*DeltaT*joint_motion[i]);
				update_data=true;
				joint_motion[i]=0;
			}
		}
		//translation
		VECTOR3 translation=_V(0.0, 0.0, 0.0);
		bool translate=false;
		for(int i=0;i<3;i++) {
			if(ee_translation[i]!=0) {
				translation.data[i]+=ee_translation[i]*DeltaT*RMS_EE_TRANSLATION_SPEED;
				ee_translation[i]=0;
				translate=true;
			}
		}
		if(translate) Translate(translation);
	}

	if(EEAuto || EEMan) {
		//Grapple sequence
		if(EEGrapple) {
			// if grapple is set, always try to close grapple
			if(!Grapple_State.Closed()) {
				Grapple_State.Move(DeltaT*ARM_GRAPPLE_SPEED);
				if(Grapple_State.Closed()) {
					if(!STS()->GetAttachmentStatus(hAttach)) Grapple();
					if(EEAuto) Extend_State.action=AnimState::CLOSING;
				}
			}
		}
		else if(EERigid || (EEGrapple && EEAuto)) {
			if(!Extend_State.Closed()) {
				Extend_State.Move(DeltaT*ARM_EXTEND_SPEED);
				if(Extend_State.Closed() && EEAuto) {
					Rigid_State.action=AnimState::CLOSING;
				}
			}
			else if(!Rigid_State.Closed()) {
				Rigid_State.Move(DeltaT*ARM_RIGID_SPEED);
			}
		}
		else if(EERelease) {
			if(!Grapple_State.Open()) {
				Grapple_State.Move(DeltaT*ARM_GRAPPLE_SPEED);
				if(Grapple_State.Open()) {
					if(STS()->GetAttachmentStatus(hAttach)) Ungrapple();
					if(EEAuto) Extend_State.action=AnimState::OPENING;
				}
			}
		}
		else if(EEDerigid || (EERelease && EEAuto)) {
			if(!Rigid_State.Open()) {
				Rigid_State.Move(DeltaT*ARM_RIGID_SPEED);
				if(Rigid_State.Open() && EEAuto) {
					Grapple_State.action=AnimState::OPENING;
				}
			}
			else if(!Extend_State.Open()) {
				Extend_State.Move(DeltaT*ARM_EXTEND_SPEED);;
			}
		}
	}

	for(int i=0;i<2;i++) {
		if(camRMSElbow_rotation[i]!=0) {
			if(camLowSpeed) camRMSElbow[i]+=camRMSElbow_rotation[i]*PTU_LOWRATE_SPEED*DeltaT;
			else camRMSElbow[i]+=camRMSElbow_rotation[i]*PTU_HIGHRATE_SPEED*DeltaT;

			double anim=linterp(-170, 0, 170, 1, camRMSElbow[i]);
			STS()->SetAnimation(anim_camRMSElbow[i], anim);

			camera_moved=true;
			camRMSElbow_rotation[i]=0;
		}
	}

	if(display_angles) {
		sprintf_s(oapiDebugString(), 255, "SY:%f SP:%f EP:%f WP:%f WY:%f WR:%f", joint_angle[SHOULDER_YAW], joint_angle[SHOULDER_PITCH], joint_angle[ELBOW_PITCH], 
			joint_angle[WRIST_PITCH], joint_angle[WRIST_YAW], joint_angle[WRIST_ROLL]);
	}
}

void RMSSystem::OnPostStep(double SimT, double DeltaT, double MJD)
{
	MPMSystem::OnPostStep(SimT, DeltaT, MJD);

	if(arm_moved) {
		if(hAttach) STS()->SetAttachmentParams(hAttach, STS()->GetOrbiterCoGOffset()+arm_tip[0], arm_tip[1]-arm_tip[0], arm_tip[2]-arm_tip[0]);
		if(update_data) {
			arm_ee_dir=RotateVectorZ(arm_tip[1]-arm_tip[0], -18.435);
			arm_ee_dir=_V(-arm_ee_dir.z, -arm_ee_dir.x, arm_ee_dir.y);
			//sprintf_s(oapiDebugString(), 255, "Calculated dir: %f %f %f", arm_ee_dir.x, arm_ee_dir.y, arm_ee_dir.z);

			arm_ee_rot=RotateVectorZ(arm_tip[2]-arm_tip[0], -18.435);
			arm_ee_rot=_V(-arm_ee_rot.z, -arm_ee_rot.x, arm_ee_rot.y);
			//sprintf_s(oapiDebugString(), 255, "Calculated rot: %f %f %f", arm_ee_rot.x, arm_ee_rot.y, arm_ee_rot.z);

			//arm_ee_pos=RotateVectorZ(_V(-2.84, 2.13, 9.02)-arm_tip[0], -18.435);
			arm_ee_pos=RotateVectorZ(RMS_SP_JOINT-arm_tip[0], -18.435);
			arm_ee_pos=_V(arm_ee_pos.z, arm_ee_pos.x, -arm_ee_pos.y);
			//sprintf_s(oapiDebugString(), 255, "Calculated EE pos: %f %f %f", arm_ee_pos.x, arm_ee_pos.y, arm_ee_pos.z);

			update_data=false;
		}

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

		arm_moved=false;
	}
	else if(camera_moved && RMSCameraMode==ELBOW) {
		UpdateElbowCamView();
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
		update_data=true;
		//oapiWriteLog("Read ARM_STATUS line");
		return true;
	}
	else if(!_strnicmp(line, "RMS_ELBOW_CAM", 13)) {
		sscanf_s(line+13, "%lf%lf", &camRMSElbow[PAN], &camRMSElbow[TILT]);
		camera_moved=true;
		return true;
	}

	return MPMSystem::OnParseLine(line);
}

void RMSSystem::OnSaveState(FILEHANDLE scn) const
{
	char cbuf[255];

	oapiWriteLine(scn, "  RMS");
	sprintf_s(cbuf, 255, "%0.6f %0.6f %0.6f %0.6f %0.6f %0.6f", joint_pos[SHOULDER_YAW], joint_pos[SHOULDER_PITCH], 1.0-joint_pos[ELBOW_PITCH], 
		joint_pos[WRIST_PITCH], joint_pos[WRIST_YAW], joint_pos[WRIST_ROLL]);
	oapiWriteScenario_string (scn, "ARM_STATUS", cbuf);
	/*oapiWriteScenario_float(scn, "SHOULDER_BRACE", shoulder_brace);
	WriteScenario_state(scn, "GRAPPLE", Grapple);
	WriteScenario_state(scn, "RIGIDIZE", Rigidize);
	WriteScenario_state(scn, "EXTEND", Extend);*/
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

void RMSSystem::RotateElbowCam(int pitch, int yaw)
{
	camRMSElbow_rotation[PAN]=yaw;
	camRMSElbow_rotation[TILT]=pitch;
}

void RMSSystem::SetElbowCamRotSpeed(bool low)
{
	camLowSpeed=low;
}

void RMSSystem::Translate(const VECTOR3 &dPos)
{
	VECTOR3 arm_cpos=arm_ee_pos+RotateVectorX(dPos, -18.435);
	VECTOR3 arm_wy_cpos=arm_cpos-arm_ee_dir*RMS_WY_EE_DIST;
	double yaw_angle=atan2(arm_wy_cpos.y, arm_wy_cpos.x);

	VECTOR3 boom_plane=_V(cos(yaw_angle), sin(yaw_angle), 0.0);
	//find vector in XY plane and perpendicular to arm booms
	VECTOR3 normal=crossp(boom_plane, _V(0.0, 0.0, 1.0));
	normal/=length(normal);
	//sprintf_s(oapiDebugString(), 255, "Normal: %f %f %f Boom Plane: %f %f %f", normal.x, normal.y, normal.z, boom_plane.x, boom_plane.y, boom_plane.z);

	double beta_w;
	if((arm_ee_dir.z>normal.z && arm_ee_dir.z<boom_plane.z) || (arm_ee_dir.z<normal.z && arm_ee_dir.z>boom_plane.z))
		beta_w=-DEG*acos(dotp(normal, arm_ee_dir))+90.0;
	else beta_w=-90.0+DEG*acos(dotp(normal, arm_ee_dir));

	double phi;
	//find vector in same plane as arm booms and perpendicular to EE direction
	VECTOR3 wp_normal=crossp(normal, arm_ee_dir);
	if(Eq(length(wp_normal), 0.0, 0.001)) //check if arm_ee_dir is perpendicular to arm booms
	{
		// use same phi angle as for previous position
		phi=joint_angle[SHOULDER_PITCH]+joint_angle[ELBOW_PITCH]+joint_angle[WRIST_PITCH];
		wp_normal=RotateVectorZ(boom_plane, phi+90.0);
	}
	else {
		wp_normal/=length(wp_normal);
		phi=DEG*acos(wp_normal.z);
		if(arm_ee_dir.z<0.0) phi=-phi;
	}
	sprintf_s(oapiDebugString(), 255, "normal: %f %f %f wp_normal: %f %f %f phi: %f, beta_w: %f", normal.x, normal.y, normal.z, wp_normal.x, wp_normal.y, wp_normal.z,
		phi, beta_w);

	//VECTOR3 arm_wp_dir=RotateVectorY(boom_plane, phi);
	VECTOR3 arm_wp_dir=crossp(wp_normal, normal); // wp_normal and normal vectors are perpendicular
	VECTOR3 arm_wp_cpos=arm_wy_cpos-arm_wp_dir*RMS_WP_WY_DIST;
	double r=length(arm_wp_cpos);

	double beta_s=-DEG*yaw_angle;
	double rho=sqrt(arm_wp_cpos.x*arm_wp_cpos.x+arm_wp_cpos.y*arm_wp_cpos.y);
	double cos_phibar_e=(r*r-RMS_SP_EP_DIST*RMS_SP_EP_DIST-RMS_EP_WP_DIST*RMS_EP_WP_DIST)/(-2*RMS_SP_EP_DIST*RMS_EP_WP_DIST);
	if (fabs(cos_phibar_e)>1) return;//Can't reach new point with the elbow
	double phi_e=DEG*acos(cos_phibar_e)-180.0-RMS_EP_NULL_ANGLE-RMS_SP_NULL_ANGLE;
	double cos_phi_s2=(RMS_EP_WP_DIST*RMS_EP_WP_DIST-RMS_SP_EP_DIST*RMS_SP_EP_DIST-r*r)/(-2*RMS_SP_EP_DIST*r);
	if(fabs(cos_phi_s2)>1) return; //Can't reach with shoulder
	double phi_s=DEG*(atan2(arm_wp_cpos.z,rho)+acos(cos_phi_s2))+RMS_SP_NULL_ANGLE;

	//sprintf_s(oapiDebugString(), 255, "Angles: %f %f %f %f %f", beta_s, phi_s, phi_e, phi-phi_s-phi_e, beta_w);

	//make sure values calculated are within bounds
	if(beta_s<RMS_JOINT_SOFTSTOPS[0][SHOULDER_YAW] || beta_s>RMS_JOINT_SOFTSTOPS[1][SHOULDER_YAW]) return;
	if(phi_s<RMS_JOINT_SOFTSTOPS[0][SHOULDER_PITCH] || phi_s>RMS_JOINT_SOFTSTOPS[1][SHOULDER_PITCH]) return;
	if(phi_e<RMS_JOINT_SOFTSTOPS[0][ELBOW_PITCH] || phi_e>RMS_JOINT_SOFTSTOPS[1][ELBOW_PITCH]) return;

	//wrist pitch
	double phi_w=phi-phi_s-phi_e;

	//check values are within bounds
	if(phi_w<RMS_JOINT_SOFTSTOPS[0][WRIST_PITCH] || phi_w>RMS_JOINT_SOFTSTOPS[1][WRIST_PITCH]) return;
	if(beta_w<RMS_JOINT_SOFTSTOPS[0][WRIST_YAW] || beta_w>RMS_JOINT_SOFTSTOPS[1][WRIST_YAW]) return;

	SetJointAngle(SHOULDER_YAW, beta_s);
	SetJointAngle(SHOULDER_PITCH, phi_s);
	SetJointAngle(ELBOW_PITCH, phi_e);
	SetJointAngle(WRIST_PITCH, phi_w);
	SetJointAngle(WRIST_YAW, beta_w);
	/*joint_angle[SHOULDER_YAW]=0.0;
	joint_angle[SHOULDER_PITCH]=phi_s;
	joint_angle[ELBOW_PITCH]=phi_e;
	joint_angle[WRIST_PITCH]=phi_w;
	joint_angle[WRIST_YAW]=0.0;*/

	arm_ee_pos=arm_cpos;

	VECTOR3 temp=RotateVectorZ(_V(-2.84, 2.13, 9.02)-arm_tip[0], -18.435);
	temp=_V(temp.z, temp.x, -temp.y);
	sprintf_s(oapiDebugString(), 255, "Pos: %f %f %f Calc: %f %f %f Error: %f", temp.x, temp.y, temp.z, arm_ee_pos.x, arm_ee_pos.y, arm_ee_pos.z, length(arm_ee_pos-temp));
}

void RMSSystem::SetJointAngle(RMS_JOINT joint, double angle)
{
	double pos=linterp(RMS_JOINT_LIMITS[0][joint], 0.0, RMS_JOINT_LIMITS[1][joint], 1.0, angle);
	//sprintf_s(oapiDebugString(), 255, "Joint Angle: %f %f", angle, pos);
	if(pos>=0.0 && pos<=1.0) {
		STS()->SetAnimation(anim_joint[joint], pos);
		joint_pos[joint]=pos;
		joint_angle[joint]=angle;
		arm_moved=true;
	}
}

void RMSSystem::SetJointPos(RMS_JOINT joint, double pos)
{
	if(pos>=0.0 && pos<=1.0) {
		STS()->SetAnimation(anim_joint[joint], pos);
		joint_pos[joint]=pos;
		joint_angle[joint]=linterp(0.0, RMS_JOINT_LIMITS[0][joint], 1.0, RMS_JOINT_LIMITS[1][joint], pos);
		arm_moved=true;
	}
}

OBJHANDLE RMSSystem::Grapple()
{
	/*oapiWriteLog("Grappling satellite");

	VECTOR3 gpos, gdir, grms, pos, dir, rot, grmsdir;
	STS()->Local2Global (STS()->GetOrbiterCoGOffset()+arm_tip[0], grms);  // global position of RMS tip
	STS()->GlobalRot(arm_tip[1]-arm_tip[0], grmsdir);

	// Search the complete vessel list for a grappling candidate.
	// Not very scalable ...
	for (DWORD i = 0; i < oapiGetVesselCount(); i++) {
		OBJHANDLE hV = oapiGetVesselByIndex (i);
		if (hV == STS()->GetHandle()) continue; // we don't want to grapple ourselves ...
		oapiGetGlobalPos (hV, &gpos);
		if (dist (gpos, grms) < oapiGetSize (hV)) { // in range
			VESSEL *v = oapiGetVesselInterface (hV);
			DWORD nAttach = v->AttachmentCount (true);
			for (DWORD j = 0; j < nAttach; j++) { // now scan all attachment points of the candidate
				ATTACHMENTHANDLE hAtt = v->GetAttachmentHandle (true, j);
				const char *id = v->GetAttachmentId (hAtt);
				if (strncmp (id, "GS", 2)) 
					continue; // attachment point not compatible
				v->GetAttachmentParams (hAtt, pos, dir, rot);
				v->Local2Global (pos, gpos);
				sprintf_s(oapiDebugString(), 255, "%s %s Dist: %f", v->GetName(), id, dist(gpos, grms));
				oapiWriteLog(oapiDebugString());
				if (dist (gpos, grms) < MAX_GRAPPLING_DIST) { 
					v->GlobalRot(dir, gdir);
					//sprintf_s(oapiDebugString(), 255, "Attitude difference: %f", fabs(180-DEG*acos(dotp(gdir, grmsdir))));
					if(fabs(PI-acos(dotp(gdir, grmsdir))) < MAX_GRAPPLING_ANGLE) {
						AttachPayload(v, hAtt);
						return hV;
					}
				}
			}
		}
	}
	return NULL;*/

	VESSEL* pVessel=NULL;
	ATTACHMENTHANDLE hAtt=FindPayload(&pVessel);
	if(hAtt) AttachPayload(pVessel, hAtt);

	if(pVessel) return pVessel->GetHandle();
	return NULL;
}

/*void RMSSystem::Detach(VESSEL* target)
{
	if(!target || target==payload) {
		STS()->DetachChild(hAttach);
		detached=true;
	}
}*/

void RMSSystem::OnMRLLatched()
{
	if(ArmStowed()) {
		for(int i=0;i<6;i++) SetJointAngle((RMS_JOINT)i, 0.0);
	}
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
	STS()->SetCameraOffset(STS()->GetOrbiterCoGOffset()+arm_tip[3]);
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
