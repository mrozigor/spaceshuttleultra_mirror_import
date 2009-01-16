#include "RMSSystem.h"
#include "meshres_RMS.h"

extern double linterp(double x0, double y0, double x1, double y1, double x);

RMSSystem::RMSSystem(SubsystemDirector *_director)
	: AtlantisSubsystem(_director, "RMS")
{
	hRMSMesh=oapiLoadMeshGlobal(RMS_MESHNAME);
	mesh_index=MESH_UNDEFINED;

	payload=NULL;
	grapple=NULL;
	end_effector=NULL;
	detached=false;

	joint_pos[SHOULDER_YAW] = 0.5;
	joint_pos[SHOULDER_PITCH] = 0.0136;
	joint_pos[ELBOW_PITCH] = 0.985312;
	joint_pos[WRIST_PITCH] = 0.5;
	joint_pos[WRIST_YAW] = 0.5;
	joint_pos[WRIST_ROLL] = 0.5;
	arm_tip[0] = RMS_EE_POS;
	arm_tip[1] = RMS_EE_POS+_V(0.0, 0.0, 1.0);
	arm_tip[2] = RMS_EE_POS+_V(0.0, 1.0, 0.0);
	arm_ee_dir = _V(1.0, 0.0, 0.0);
	arm_ee_pos = _V(15.069, 0.0, 0.0);
	//RMS elbow camera
	camRMSElbowLoc[0]=_V(-2.681, 2.641, 1.806);
	camRMSElbowLoc[1]=camRMSElbowLoc[0]+_V(0, 0, -1);

	for(int i=0;i<6;i++) joint_motion[i]=0;
	for(int i=0;i<3;i++) ee_translation[i]=0;
}

RMSSystem::~RMSSystem()
{
}

void RMSSystem::Realize()
{
	CreateArm();
}

void RMSSystem::CreateArm()
{
	//add RMS mesh
	VECTOR3 ofs=STS()->GetOrbiterCoGOffset();
	mesh_index=STS()->AddMesh(hRMSMesh, &ofs);
	STS()->SetMeshVisibilityMode(mesh_index, MESHVIS_EXTERNAL|MESHVIS_VC|MESHVIS_EXTPASS);

	//rollout animation
	static UINT RMSRolloutGrp[11] = {GRP_RMS_MPMs, GRP_base, GRP_Shoulder_Yaw, GRP_Humerus, GRP_Radii, GRP_elbowcam, GRP_camswivel, GRP_cambase, GRP_Wristpitch, GRP_Wrist_Yaw, GRP_Endeffector};
	static MGROUP_ROTATE rms_rollout_anim(mesh_index, RMSRolloutGrp, 11,
		_V(-2.643, 1.282, 0.0), _V(0, 0, 1), (float)(31.36*RAD)); //1.05 or 1.10
	anim_rollout = STS()->CreateAnimation(1.0);
	STS()->AddAnimationComponent(anim_rollout, 0, 1, &rms_rollout_anim);

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
	anim_camRMSElbowPan=STS()->CreateAnimation(0.5);
	parent2 = STS()->AddAnimationComponent (anim_camRMSElbowPan, 0, 1, &RMSElbowCamPan, parent);
	static MGROUP_ROTATE RMSElbowCamTilt (mesh_index, RMSElbowCamGrp, 1,
		_V(-2.68, 2.64, 2.073), _V(0.9513, -0.3082, 0), (float)(340*RAD));
	anim_camRMSElbowTilt=STS()->CreateAnimation(0.5);
	parent2 = STS()->AddAnimationComponent(anim_camRMSElbowTilt, 0, 1, &RMSElbowCamTilt, parent2);
	static MGROUP_ROTATE RMSElbowCamLoc (LOCALVERTEXLIST, MAKEGROUPARRAY(camRMSElbowLoc), 2,
		_V(-2.765, 2.373, 2.073), _V(1, 0, 0), 0.0f);
	STS()->AddAnimationComponent(anim_camRMSElbowTilt, 0, 1, &RMSElbowCamLoc, parent2);

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

	static MGROUP_ROTATE rms_ee_anim(LOCALVERTEXLIST, MAKEGROUPARRAY(arm_tip), 3,
		_V(-2.844, 2.133, -6.049), _V(0,0,1), (float)(0.0));
	anim_rms_ee = STS()->CreateAnimation (0.0);
	STS()->AddAnimationComponent (anim_rms_ee, 0, 1, &rms_ee_anim, parent);
}

void RMSSystem::OnPostStep(double SimT, double DeltaT, double MJD)
{
	if(!detached && payload!=NULL && !STS()->GetAttachmentStatus(end_effector)) {
		if(PayloadIsFree()) STS()->AttachChild(payload->GetHandle(), end_effector, grapple);
	}

	//rotate joints
	if(Movable()) {
		//rotation
		for(int i=0;i<6;i++) {
			if(joint_motion[i]!=0) {
				SetJointAngle((RMS_JOINT)i, joint_angle[i]+RMS_JOINT_ROTATION_SPEED*DeltaT*joint_motion[i]);
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
}

bool RMSSystem::OnParseLine(const char* line)
{
	return false;
}

void RMSSystem::OnSaveState(FILEHANDLE scn) const
{
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

void RMSSystem::Translate(const VECTOR3 &dPos)
{
	double beta_s=joint_angle[SHOULDER_YAW], beta_w=joint_angle[WRIST_YAW];
	double phi_s=joint_angle[SHOULDER_PITCH], phi_e=joint_angle[ELBOW_PITCH], phi_w=joint_angle[WRIST_PITCH];

	VECTOR3 arm_cpos=arm_ee_pos+RotateVectorX(dPos, 18.435);
	//arm_cpos=_V(15.069, 0.0, 0.0);
	int iterations=0;
	do
	{
		/*double current_phi_s=linterp(0,shoulder_min,1,shoulder_max,arm_sp);
		double current_phi_e=linterp(0,elbow_min,1,elbow_max,arm_ep);
		double current_phi_w=linterp(0, wrist_min, 1, wrist_max, arm_wp);
		double current_beta_s=linterp(0,-180,1,180,arm_sy);
		double current_beta_w=linterp(0, wrist_yaw_min, 1, wrist_yaw_max, arm_wy);*/

		VECTOR3 temp=RotateVectorZ(_V(1.0, 0.0, 0.0), beta_s);
		VECTOR3 arm_wp_dir=RotateVectorY(temp, phi_w+phi_e+phi_s);

		VECTOR3 arm_wrist_cpos=arm_cpos-arm_ee_dir*RMS_WY_EE_DIST-arm_wp_dir*RMS_WP_WY_DIST;
		double r=length(arm_wrist_cpos);
		beta_s=DEG*atan2(arm_wrist_cpos.y,arm_wrist_cpos.x);
		double rho=sqrt(arm_wrist_cpos.x*arm_wrist_cpos.x+arm_wrist_cpos.y*arm_wrist_cpos.y);
		double cos_phibar_e=(r*r-RMS_SP_EP_DIST*RMS_SP_EP_DIST-RMS_EP_WP_DIST*RMS_EP_WP_DIST)/(-2*RMS_SP_EP_DIST*RMS_EP_WP_DIST);
		if (fabs(cos_phibar_e)>1) return;//Can't reach new point with the elbow
		phi_e=DEG*acos(cos_phibar_e)-180.0-RMS_EP_NULL_ANGLE-RMS_SP_NULL_ANGLE;
		double cos_phi_s2=(RMS_EP_WP_DIST*RMS_EP_WP_DIST-RMS_SP_EP_DIST*RMS_SP_EP_DIST-r*r)/(-2*RMS_SP_EP_DIST*r);
		if(fabs(cos_phi_s2)>1) return; //Can't reach with shoulder
		phi_s=DEG*(atan2(arm_wrist_cpos.z,rho)+acos(cos_phi_s2))+RMS_SP_NULL_ANGLE;
		/*double anim_phi_s=linterp(shoulder_min,0,shoulder_max,1,phi_s);
		double anim_phi_e=linterp(elbow_min,0,elbow_max,1,phi_e);
		double anim_beta_s=linterp(-180,0,180,1,beta_s);
		if(anim_beta_s<0 || anim_beta_s>1) return;
		if(anim_phi_s<0 || anim_phi_s>1) return;
		if(anim_phi_e<0 || anim_phi_e>1) return;*/

		//make sure values calculated are within bounds
		//if(beta_s<RMS_JOINT_SOFTSTOPS[0][SHOULDER_YAW] || beta_s>RMS_JOINT_SOFTSTOPS[1][SHOULDER_YAW]) return;
		//if(phi_s<RMS_JOINT_SOFTSTOPS[0][SHOULDER_PITCH] || phi_s>RMS_JOINT_SOFTSTOPS[1][SHOULDER_PITCH]) return;
		//if(phi_e<RMS_JOINT_SOFTSTOPS[0][ELBOW_PITCH] || phi_e>RMS_JOINT_SOFTSTOPS[1][ELBOW_PITCH]) return;

		//Limited IK on the wrist
		double new_phi_l=phi_s+phi_e;
		double current_phi_l=joint_angle[SHOULDER_PITCH]+joint_angle[ELBOW_PITCH];
		phi_w=joint_angle[WRIST_PITCH]-new_phi_l+current_phi_l;
		//double anim_phi_w=linterp(wrist_min,0,wrist_max,1,new_phi_w);

		//wrist yaw
		beta_w=joint_angle[WRIST_YAW]+beta_s-joint_angle[SHOULDER_YAW];
		//double anim_beta_w=linterp(wrist_yaw_min, 0, wrist_yaw_max, 1, beta_w);

		/*arm_sy=anim_beta_s;
		arm_sp=anim_phi_s;
		arm_ep=anim_phi_e;
		arm_wp=anim_phi_w;
		arm_wy=anim_beta_w;*/

		iterations++;
	}while(iterations<1);

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
	temp=_V(temp.z, -temp.x, -temp.y);
	sprintf_s(oapiDebugString(), 255, "Pos: %f %f %f Calc: %f %f %f", temp.x, temp.y, temp.z, arm_ee_pos.x, arm_ee_pos.y, arm_ee_pos.z);
}

void RMSSystem::SetJointAngle(RMS_JOINT joint, double angle)
{
	double pos=linterp(RMS_JOINT_LIMTS[0][joint], 0.0, RMS_JOINT_LIMTS[1][joint], 1.0, angle);
	sprintf_s(oapiDebugString(), 255, "Joint Angle: %f %f", angle, pos);
	if(pos>=0.0 && pos<=1.0) {
		STS()->SetAnimation(anim_joint[joint], pos);
		joint_pos[joint]=pos;
		joint_angle[joint]=angle;
	}
}

void RMSSystem::Grapple(VESSEL* vessel, ATTACHMENTHANDLE attachment)
{
	//for the moment, assume attachment passed is completely valid
	grapple=attachment;
	payload=vessel;
	detached=false;
}

void RMSSystem::Ungrapple()
{
	grapple=NULL;
	payload=NULL;
	STS()->DetachChild(end_effector);
}

void RMSSystem::Detach(VESSEL* target)
{
	if(!target || target==payload) {
		STS()->DetachChild(end_effector);
		detached=true;
	}
}

bool RMSSystem::PayloadIsFree() const
{
	if(payload) {
		//if we are attached to payload, it must be 'free'
		if(STS()->GetAttachmentStatus(end_effector)) return true;
		//otherwise, loop through all attachment points on payload and check if any of them are in use
		DWORD count=payload->AttachmentCount(true);
		for(DWORD i=0;i<count;i++) {
			ATTACHMENTHANDLE att=payload->GetAttachmentHandle(true, i);
			if(payload->GetAttachmentStatus(att)) return false;
		}
	}
	return true;
}