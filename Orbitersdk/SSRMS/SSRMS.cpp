#define STRICT 1
#define ORBITER_MODULE
#include "SSRMS.h"
#include "OrbiterSoundSDK35.h"
#include "UltraMath.h"

//HINSTANCE g_hDLL;

SSRMS::SSRMS(OBJHANDLE hObj, int fmodel)
:VESSEL2(hObj, fmodel)
{
	SetEmptyMass(100);
	SetSize(10);
	SetPMI(_V(18.0, 18.0, 0.1));
	SetCrossSections(_V(4.7, 4.4, 0.4));

	pSubsystemDirector = new SSRMSSubsystemDirector(this);
	pSubsystemDirector->AddSubsystem(pLEE[0] = new LEESystem(pSubsystemDirector, "LEE1", ATTACH_ID));
	pSubsystemDirector->AddSubsystem(pLEE[1] = new LEESystem(pSubsystemDirector, "LEE2", ATTACH_ID));

	//pLEE[0] = new LatchSystem(this, "LEE1", ATTACH_ID);
	//pLEE[1] = new LatchSystem(this, "LEE2", ATTACH_ID);

	//load mesh
	hSSRMSMesh = oapiLoadMeshGlobal("SSRMSD");
	mesh_ssrms = AddMesh(hSSRMSMesh);
	SetMeshVisibilityMode(mesh_ssrms, MESHVIS_ALWAYS);

	//initialize RMS
	for(int i=0;i<7;i++) {
		joint_angle[i]=0.0;
		joint_motion[i]=0;
	}
	arm_ee_pos = _V(15.90, 0.7, 0.59);
	//arm_wrist_pos = WristPitchJoint-ShoulderPitchJoint;
	arm_ee_dir = _V(1, 0, 0);
	arm_ee_rot = _V(0, 1, 0);
	arm_tip[0] = LEE_POS;
	arm_tip[1] = _V(-0.70, 0.59, 9.44);
	arm_tip[2] = _V(-0.70, 1.59, 8.44);

	update_angles=false;
	update_vectors=true;
	arm_moved=false;

	SpeedFactor=1;
	RefFrame=EE_FRAME;
	activeLEE=1;
	passiveLEE=0;

	ShowAttachmentPoints=false;

	//SetCameraOffset(arm_tip[2]);
	SetCameraDefaultDirection(arm_tip[1]-arm_tip[0]);

	OrbiterSoundHandle = -1;

	DefineAnimations();
	DefineThrusters();

	//ahBase = CreateAttachment(true, SR_JOINT, _V(0, 0, -1), _V(0, 1, 0), ATTACH_ID);
	//ahGrapple = CreateAttachment(false, arm_tip[0], arm_tip[1]-arm_tip[0], arm_tip[2]-arm_tip[0], ATTACH_ID);
	//SetAttachmentParams(ahBase, arm_tip[0], arm_tip[1]-arm_tip[0], arm_tip[2]-arm_tip[0]);
	//pLEE[0]->SetAttachment(ahBase, false);
	//pLEE[1]->SetAttachment(ahGrapple, true);
}

SSRMS::~SSRMS()
{
	delete pSubsystemDirector;
	//for(int i=0;i<8;i++) delete rms_anim[i];
}

void SSRMS::DefineAnimations()
{
	ANIMATIONCOMPONENT_HANDLE parent;

	static UINT ShoulderRollGrp[1] = {21};
	static MGROUP_ROTATE sr_anim (mesh_ssrms, ShoulderRollGrp, 1,
		SR_JOINT, _V(0, 0, -1), (float)(894.0*RAD));
	anim_joint[1][SHOULDER_ROLL] = CreateAnimation(0.5);
	//anim_joint[SHOULDER_ROLL[1]] = CreateAnimation(0.5);
	parent = AddAnimationComponent(anim_joint[1][SHOULDER_ROLL], 0, 1, &sr_anim);

	static UINT ShoulderYawGrp[1] = {22};
	static MGROUP_ROTATE sy_anim (mesh_ssrms, ShoulderYawGrp, 1,
		SY_JOINT, _V(0, 1, 0), (float)(894.0*RAD));
	anim_joint[1][SHOULDER_YAW] = CreateAnimation(0.5);
	//anim_joint[SHOULDER_YAW[1]] = CreateAnimation(0.5);
	parent = AddAnimationComponent(anim_joint[1][SHOULDER_YAW], 0, 1, &sy_anim, parent);

	static UINT ShoulderPitchGrp[10] = {23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
	static MGROUP_ROTATE sp_anim (mesh_ssrms, ShoulderPitchGrp, 10,
		SP_JOINT, _V(1, 0, 0), (float)(894.0*RAD));
	anim_joint[1][SHOULDER_PITCH] = CreateAnimation(0.5);
	//anim_joint[SHOULDER_PITCH[1]] = CreateAnimation(0.5);
	parent = AddAnimationComponent(anim_joint[1][SHOULDER_PITCH], 0, 1, &sp_anim, parent);

	static UINT ElbowPitchGrp[9] = {33, 34, 35, 36, 37, 38, 39, 40, 41};
	static MGROUP_ROTATE ep_anim(mesh_ssrms, ElbowPitchGrp, 9,
		EP_JOINT, _V(1, 0, 0), (float)(894.0*RAD));
	anim_joint[1][ELBOW_PITCH] = CreateAnimation(0.5);
	//anim_joint[ELBOW_PITCH[1]] = CreateAnimation(0.5);
	parent = AddAnimationComponent(anim_joint[1][ELBOW_PITCH], 0, 1, &ep_anim, parent);

	static UINT WristPitchGrp[1] = {42};
	static MGROUP_ROTATE wp_anim (mesh_ssrms, WristPitchGrp, 1,
		WP_JOINT, _V(1, 0, 0), (float)(894.0*RAD));
	anim_joint[1][WRIST_PITCH] = CreateAnimation(0.5);
	//anim_joint[WRIST_PITCH[1]] = CreateAnimation(0.5);
	parent = AddAnimationComponent(anim_joint[1][WRIST_PITCH], 0, 1, &wp_anim, parent);

	static UINT WristYawGrp[1] = {43};
	static MGROUP_ROTATE wy_anim(mesh_ssrms, WristYawGrp, 1,
		WY_JOINT, _V(0, 1, 0), (float)(894.0*RAD));
	anim_joint[1][WRIST_YAW] = CreateAnimation(0.5);
	//anim_joint[WRIST_YAW[1]] = CreateAnimation(0.5);
	parent = AddAnimationComponent(anim_joint[1][WRIST_YAW], 0, 1, &wy_anim, parent);

	static UINT WristRollGrp[21] = {44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64};
	static MGROUP_ROTATE wr_anim(mesh_ssrms, WristRollGrp, 21,
		LEE_POS, _V(0, 0, 1), (float)(894.0*RAD));
	anim_joint[1][WRIST_ROLL] = CreateAnimation(0.5);
	//anim_joint[WRIST_ROLL[1]] = CreateAnimation(0.5);
	parent = AddAnimationComponent(anim_joint[1][WRIST_ROLL], 0, 1, &wr_anim, parent);

	static MGROUP_ROTATE lee_anim(LOCALVERTEXLIST, MAKEGROUPARRAY(arm_tip), 3,
		LEE_POS, _V(0, 0, 1), 0.0);
	anim_lee = CreateAnimation(0.5);
	AddAnimationComponent(anim_lee, 0, 1, &lee_anim, parent);

	for(int i=0;i<7;i++) anim_joint[0][i]=anim_joint[1][6-i];
}

void SSRMS::DefineThrusters()
{
	//THRUSTER_HANDLE tmp;
	ph_null = CreatePropellantResource(1.0, 1.0);

	rms_control[0] = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0.0, ph_null, 1.0, 1.0);
	CreateThrusterGroup(rms_control, 1, THGROUP_ATT_UP);
	CreateThrusterGroup(rms_control, 1, THGROUP_ATT_PITCHUP);

	rms_control[1] = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0.0, ph_null, 1.0, 1.0);
	CreateThrusterGroup(rms_control+1, 1, THGROUP_ATT_DOWN);

	rms_control[2] = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0.0, ph_null, 1.0, 1.0);
	CreateThrusterGroup(rms_control+2, 1, THGROUP_ATT_LEFT);

	rms_control[3] = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0.0, ph_null, 1.0, 1.0);
	CreateThrusterGroup(rms_control+3, 1, THGROUP_ATT_RIGHT);

	rms_control[4] = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0.0, ph_null, 1.0, 1.0);
	CreateThrusterGroup(rms_control+4, 1, THGROUP_ATT_FORWARD);

	rms_control[5] = CreateThruster(_V(0, 0, 0), _V(1, 0, 0), 0.0, ph_null, 1.0, 1.0);
	CreateThrusterGroup(rms_control+5, 1, THGROUP_ATT_BACK);
}

bool SSRMS::MoveEE(const VECTOR3 &newPos, const VECTOR3 &newDir, const VECTOR3 &newRot)
{
	//sprintf_s(oapiDebugString(), 255, "Target pos: %f %f %f", newPos.x, newPos.y, newPos.z);

	SetJointAngle(SHOULDER_ROLL, 0.0);

	VECTOR3 arm_wy_pos = newPos-newDir*WY_EE_DIST;

	//sprintf_s(oapiDebugString(), 255, "arm_wy_pos: %f %f %f", arm_wy_pos.x, arm_wy_pos.y, arm_wy_pos.z);

	VECTOR3 corrected_wy_pos = _V(arm_wy_pos.x, arm_wy_pos.y, 0.0);
	double c_length = length(corrected_wy_pos);
	double offset_angle = asin(LEE_OFFSET/c_length);

	double sy_angle_r = -atan2(arm_wy_pos.y, arm_wy_pos.x)+offset_angle;
	VECTOR3 offset_vector = RotateVectorZ(_V(0, -1, 0), -sy_angle_r*DEG);
	//if(offset_vector.y>0.0) offset_vector=-offset_vector; // make sure vector always points in -Y direction; simplifies math
	//if(corrected_wy_pos.x<0.0) offset_vector=-offset_vector;
	VECTOR3 wp_dir = crossp(offset_vector, newDir);
	//VECTOR3 wp_dir;
	//if(offset_vector.y<=0.0) wp_dir = crossp(offset_vector, newDir);
	//else wp_dir = crossp(-offset_vector, newDir);
	wp_dir/=length(wp_dir); // normalize vector
	VECTOR3 offset_wp_pos = arm_wy_pos+wp_dir*WP_WY_DIST+offset_vector*LEE_OFFSET; // we can now treat SSRMS as a single boom with multiple joints (like SRMS)
	double r = length(offset_wp_pos);

	//sprintf_s(oapiDebugString(), 255, "%s offset_pos: %f %f %f", oapiDebugString(), offset_wp_pos.x, offset_wp_pos.y, offset_wp_pos.z);

	//sprintf_s(oapiDebugString(), 255, "offset_angle: %f pos_angle: %f", offset_angle*DEG, atan2(arm_wy_pos.y, arm_wy_pos.x)*DEG);

	//sprintf_s(oapiDebugString(), 255, "offset_vector: %f %f %f newDir: %f %f %f",
		//offset_vector.x, offset_vector.y, offset_vector.z, newDir.x, newDir.y, newDir.z);
	//sprintf_s(oapiDebugString(), 255, "%s offset_vector: %f %f %f wp_dir: %f %f %f", oapiDebugString(),
		//offset_vector.x, offset_vector.y, offset_vector.z, wp_dir.x, wp_dir.y, wp_dir.z);
	//sprintf_s(oapiDebugString(), 255, "wp_dir: %f %f %f newDir: %f %f %f",
		//wp_dir.x, wp_dir.y, wp_dir.z, newDir.x, newDir.y, newDir.z);

	double rho = sqrt(offset_wp_pos.x*offset_wp_pos.x+offset_wp_pos.y*offset_wp_pos.y);
	double cos_phibar_e=(r*r-SP_EP_DIST*SP_EP_DIST-EP_WP_DIST*EP_WP_DIST)/(-2*SP_EP_DIST*EP_WP_DIST);
	if (fabs(cos_phibar_e)>1) return false;//Can't reach new point with the elbow
	double ep_angle_d=DEG*acos(cos_phibar_e)-180.0;
	double cos_phi_s2=(EP_WP_DIST*EP_WP_DIST-SP_EP_DIST*SP_EP_DIST-r*r)/(-2*SP_EP_DIST*r);
	if(fabs(cos_phi_s2)>1) return false; //Can't reach with shoulder
	double sp_angle_d=DEG*(atan2(offset_wp_pos.z,rho)+acos(cos_phi_s2));

	double phi=-DEG*acos(wp_dir.z);
	//double phi = DEG*atan2(sqrt(wp_dir.x*wp_dir.x + wp_dir.y*wp_dir.y), wp_dir.z);
	//if((wp_dir.x>0.0 && corrected_wy_pos.x>=0.0) || (wp_dir.x<0.0 && corrected_wy_pos.x<0.0)) phi=-phi;
	if(wp_dir.x<0.0) phi=-phi;
	if(offset_vector.y>0.0) phi=-phi;
	double wp_angle_d=phi-sp_angle_d-ep_angle_d;

	double wy_angle_d;
	/*if((offset_vector.z>0.0 && newDir.z<0.0) || (offset_vector.z<0.0 && newDir.z>0.0)) 
		wy_angle_d=90.0+DEG*acos(dotp(offset_vector, newDir));
	else*/
	wy_angle_d=90.0-DEG*acos(dotp(offset_vector, newDir));
	//if(abs(sy_angle_r*DEG)<90.0) wy_angle_d=90.0-DEG*acos(dotp(offset_vector, newDir));
	//else wy_angle_d=180.0+DEG*acos(dotp(offset_vector, newDir));
	//if(corrected_wy_pos.x>=0.0) wy_angle_d=90.0-DEG*acos(dotp(offset_vector, newDir));
	//else wy_angle_d=-90.0-DEG*acos(dotp(offset_vector, newDir));
	//sprintf_s(oapiDebugString(), 255, "%s acos: %f", oapiDebugString(), DEG*acos(dotp(offset_vector, newDir)));

	//sprintf_s(oapiDebugString(), 255, "SY: %f SP: %f EP: %f WP: %f WY: %f", sy_angle_r*DEG, sp_angle_d, ep_angle_d, wp_angle_d, wy_angle_d);

	if(activeLEE==0) {
		SetJointAngle(WRIST_YAW, sy_angle_r*DEG);
		SetJointAngle(WRIST_PITCH, sp_angle_d);
		SetJointAngle(ELBOW_PITCH, ep_angle_d);
		SetJointAngle(SHOULDER_PITCH, wp_angle_d);
		SetJointAngle(SHOULDER_YAW, wy_angle_d);
		//SetJointAngle(SHOULDER_ROLL, wr_angle);
	}
	else {
		SetJointAngle(SHOULDER_YAW, sy_angle_r*DEG);
		SetJointAngle(SHOULDER_PITCH, sp_angle_d);
		SetJointAngle(ELBOW_PITCH, ep_angle_d);
		SetJointAngle(WRIST_PITCH, wp_angle_d);
		SetJointAngle(WRIST_YAW, wy_angle_d);
		//SetJointAngle(WRIST_ROLL, wr_angle);
	}

	arm_ee_pos = newPos;
	arm_ee_dir = newDir;
	arm_ee_rot = newRot;

	return true;
}

bool SSRMS::SetJointAngle(SSRMS::SSRMS_JOINT joint, double angle)
{
	if(angle>=JOINT_LIMITS[0] && angle<=JOINT_LIMITS[1]) {
		double pos=linterp(JOINT_LIMITS[0], 0.0, JOINT_LIMITS[1], 1.0, angle);
		SetAnimation(anim_joint[activeLEE][joint], pos);
		joint_angle[joint]=angle;
		arm_moved=true;
		return true;
	}
	else return false;
}

bool SSRMS::ChangeActiveLEE()
{
	if(!pLEE[0]->GrappledToBase() || !pLEE[1]->GrappledToBase()) return false;

	/*pLEE[activeLEE]->Ungrapple();
	pLEE[passiveLEE]->Ungrapple();

	activeLEE=passiveLEE;
	passiveLEE=1-activeLEE;
	
	// set attachment points
	pLEE[activeLEE]->SetAttachment(ahGrapple, true);
	pLEE[passiveLEE]->SetAttachment(ahBase, false);
	pLEE[0]->SetAttachmentParams(SR_JOINT, _V(0, 0, -1), _V(0, 1, 0));
	pLEE[1]->SetAttachmentParams(arm_tip[0], arm_tip[1]-arm_tip[0], arm_tip[2]-arm_tip[0]);

	pLEE[activeLEE]->Grapple();
	pLEE[passiveLEE]->Grapple();*/
	pLEE[passiveLEE]->DetachFromBase();
	pLEE[activeLEE]->AttachToBase();

	activeLEE=passiveLEE;
	passiveLEE=1-activeLEE;

	// swap joint angles
	for(unsigned short i=0;i<ELBOW_PITCH;i++) {
		double temp=joint_angle[i];
		joint_angle[i]=joint_angle[6-i];
		joint_angle[6-i]=temp;
	}
	CalculateVectors();

	return true;
}

void SSRMS::CalculateVectors()
{
	arm_ee_dir=arm_tip[1]-arm_tip[0];
	//if(activeLEE==0) arm_ee_dir=_V(arm_ee_dir.z, arm_ee_dir.x, -arm_ee_dir.y);
	//else arm_ee_dir=_V(arm_ee_dir.z, -arm_ee_dir.x, -arm_ee_dir.y);
	arm_ee_dir=_V(arm_ee_dir.z, -arm_ee_dir.x, -arm_ee_dir.y);
	//sprintf_s(oapiDebugString(), 255, "Calculated dir: %f %f %f", arm_ee_dir.x, arm_ee_dir.y, arm_ee_dir.z);
	//oapiWriteLog(oapiDebugString());

	arm_ee_rot=arm_tip[2]-arm_tip[0];
	arm_ee_rot=_V(-arm_ee_rot.z, -arm_ee_rot.x, -arm_ee_rot.y);
	//sprintf_s(oapiDebugString(), 255, "Calculated rot: %f %f %f", arm_ee_rot.x, arm_ee_rot.y, arm_ee_rot.z);
	//oapiWriteLog(oapiDebugString());

	/*if(activeLEE==0) {
		VECTOR3 ee_dir=arm_tip[1]-arm_tip[0];
		VECTOR3 ee_rot=arm_tip[2]-arm_tip[0];
		VECTOR3 arm_sy_pos = arm_tip[0]-ee_dir*WY_EE_DIST-ee_rot*WP_WY_DIST;
		arm_ee_pos=SR_JOINT-arm_sy_pos;
		arm_ee_pos=_V(arm_ee_pos.z, -arm_ee_pos.x, -arm_ee_pos.y);

		VECTOR3 arm_ee_cross = crossp(arm_ee_dir, arm_ee_rot);
		MATRIX3 RotMatrix = _M(arm_ee_dir.x, arm_ee_dir.y, arm_ee_dir.z,
							arm_ee_cross.x, arm_ee_cross.y, arm_ee_cross.z,
							arm_ee_rot.x, arm_ee_rot.y, arm_ee_rot.z);
		arm_ee_pos=mul(RotMatrix, arm_ee_pos);

		sprintf_s(oapiDebugString(), 255, "Calculated EE pos: %f %f %f", arm_ee_pos.x, arm_ee_pos.y, arm_ee_pos.z);
		//sprintf_s(oapiDebugString(), 255, "Calculated SY pos: %f %f %f", arm_sy_pos.x, arm_sy_pos.y, arm_sy_pos.z);
		//arm_ee_pos=_V(arm_ee_pos.x, -arm_ee_pos.y, -arm_ee_pos.z);
	}
	else {*/
		//arm_ee_pos=arm_tip[0]-SY_JOINT;
		arm_ee_pos=arm_tip[0]-_V(SY_JOINT.x, SP_JOINT.y, SY_JOINT.z);
		arm_ee_pos=_V(arm_ee_pos.z, -arm_ee_pos.x, -arm_ee_pos.y);
		sprintf_s(oapiDebugString(), 255, "Calculated EE pos: %f %f %f", arm_ee_pos.x, arm_ee_pos.y, arm_ee_pos.z);
	//}
	//oapiWriteLog(oapiDebugString());
}

void SSRMS::clbkLoadStateEx(FILEHANDLE scn, void *vs)
{
	char *line;

	while(oapiReadScenario_nextline(scn, line)) {
		if(!_strnicmp(line, "ARM_STATUS", 10)) {
			sscanf(line+10, "%lf%lf%lf%lf%lf%lf%lf", &joint_angle[SHOULDER_ROLL], &joint_angle[SHOULDER_YAW], &joint_angle[SHOULDER_PITCH],
				&joint_angle[ELBOW_PITCH], &joint_angle[WRIST_PITCH], &joint_angle[WRIST_YAW], &joint_angle[WRIST_ROLL]);
			arm_moved=true;
			update_vectors=true;
			update_angles=true;
		}
		else if(!_strnicmp(line, "ACTIVE_LEE", 10)) {
			sscanf(line+10, "%d", &activeLEE);
			passiveLEE=1-activeLEE;
		}
		else if(!pSubsystemDirector->ParseScenarioLine(scn, line)) {
			ParseScenarioLineEx(line, vs);
		}
	}
}

void SSRMS::clbkSaveState(FILEHANDLE scn)
{
	VESSEL2::clbkSaveState(scn);

	char cbuf[255];
	sprintf(cbuf, "%f %f %f %f %f %f %f", joint_angle[SHOULDER_ROLL], joint_angle[SHOULDER_YAW], joint_angle[SHOULDER_PITCH],
				joint_angle[ELBOW_PITCH], joint_angle[WRIST_PITCH], joint_angle[WRIST_YAW], joint_angle[WRIST_ROLL]);
	oapiWriteScenario_string(scn, "ARM_STATUS", cbuf);
	oapiWriteScenario_int(scn, "ACTIVE_LEE", activeLEE);
	pSubsystemDirector->SaveState(scn);
}

void SSRMS::clbkPreStep(double SimT, double SimDT, double mjd)
{
	pSubsystemDirector->PreStep(SimT, SimDT, mjd);

	for(unsigned short i=0;i<7;i++) {
		if(joint_motion[i]!=0) {
			SetJointAngle((SSRMS_JOINT)i, joint_angle[i]+SimDT*JOINT_ROTATION_SPEED*SpeedFactor*joint_motion[i]);
			update_vectors=true;
			update_angles=true;
		}
	}

	VECTOR3 RHCInput=_V(0.0, 0.0, 0.0);
	VECTOR3 THCInput=_V(0.0, 0.0, 0.0);
	VECTOR3 EETrans;
	
	THCInput.x=GetThrusterGroupLevel(THGROUP_ATT_FORWARD)-GetThrusterGroupLevel(THGROUP_ATT_BACK);
	THCInput.y=GetThrusterGroupLevel(THGROUP_ATT_LEFT)-GetThrusterGroupLevel(THGROUP_ATT_RIGHT);
	THCInput.z=GetThrusterGroupLevel(THGROUP_ATT_UP)-GetThrusterGroupLevel(THGROUP_ATT_DOWN);

	if(activeLEE==0) {
		VECTOR3 arm_ee_cross = crossp(arm_ee_dir, arm_ee_rot);
		MATRIX3 RotMatrix = _M(arm_ee_dir.x, arm_ee_dir.y, arm_ee_dir.z,
							arm_ee_cross.x, arm_ee_cross.y, arm_ee_cross.z,
							arm_ee_rot.x, arm_ee_rot.y, arm_ee_rot.z);
		EETrans=tmul(RotMatrix, THCInput)*EE_TRANSLATION_SPEED*SimDT*SpeedFactor;
		//EETrans=mul(RotMatrix, THCInput)*EE_TRANSLATION_SPEED*SimDT*SpeedFactor;
		sprintf_s(oapiDebugString(), 255, "arm_ee_cross: %f %f %f", arm_ee_cross.x, arm_ee_cross.y, arm_ee_cross.z);
	}
	else EETrans=THCInput*EE_TRANSLATION_SPEED*SimDT*SpeedFactor;
	
	sprintf_s(oapiDebugString(), 255, "EETrans: %f %f %f", EETrans.x/length(EETrans), EETrans.y/length(EETrans), EETrans.z/length(EETrans));
	if(!Eq(THCInput, _V(0.0, 0.0, 0.0), 0.01)) MoveEE(arm_ee_pos+EETrans, arm_ee_dir, arm_ee_rot);
}

void SSRMS::clbkPostStep(double SimT, double SimDT, double MJD)
{
	pSubsystemDirector->PostStep(SimT, SimDT, MJD);

	if(arm_moved) {
		/*if(hAttach) STS()->SetAttachmentParams(hAttach, STS()->GetOrbiterCoGOffset()+arm_tip[0], arm_tip[1]-arm_tip[0], arm_tip[2]-arm_tip[0]);

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
		else if(RMSCameraMode==ELBOW) UpdateElbowCamView();*/

		/*** Update output lines to LEDs ***/
		// calculate position
		VECTOR3 ee_pos_output=(arm_tip[0]-SP_JOINT)*12/fps_to_ms;
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
		if(update_angles) {
			arm_ee_angles=ee_att_output;
			update_angles=false;
		}		

		pLEE[1]->SetAttachmentParams(arm_tip[0], -arm_tip[1]+arm_tip[0], arm_tip[2]-arm_tip[0]);

		arm_moved=false;
	}
	if(update_vectors) {
			/*arm_ee_dir=arm_tip[1]-arm_tip[0];
			arm_ee_dir=_V(arm_ee_dir.z, -arm_ee_dir.x, -arm_ee_dir.y);
			sprintf_s(oapiDebugString(), 255, "Calculated dir: %f %f %f", arm_ee_dir.x, arm_ee_dir.y, arm_ee_dir.z);
			//oapiWriteLog(oapiDebugString());

			arm_ee_rot=arm_tip[2]-arm_tip[0];
			arm_ee_rot=_V(-arm_ee_rot.z, -arm_ee_rot.x, -arm_ee_rot.y);
			//sprintf_s(oapiDebugString(), 255, "Calculated rot: %f %f %f", arm_ee_rot.x, arm_ee_rot.y, arm_ee_rot.z);
			//oapiWriteLog(oapiDebugString());

			//arm_ee_pos=arm_tip[0]-SY_JOINT;
			arm_ee_pos=arm_tip[0]-_V(SY_JOINT.x, SP_JOINT.y, SY_JOINT.z);
			arm_ee_pos=_V(arm_ee_pos.z, -arm_ee_pos.x, -arm_ee_pos.y);
			sprintf_s(oapiDebugString(), 255, "Calculated EE pos: %f %f %f", arm_ee_pos.x, arm_ee_pos.y, arm_ee_pos.z);
			//oapiWriteLog(oapiDebugString());

			if(activeLEE==0) {
				arm_ee_pos=_V(arm_ee_pos.x, -arm_ee_pos.y, -arm_ee_pos.z);
			}*/
		CalculateVectors();

		update_vectors=false;
	}

	//VECTOR3 arm_ee_pos2=arm_tip[0]-SY_JOINT;
	//VECTOR3 arm_ee_pos2=arm_tip[0]-_V(SY_JOINT.x, SP_JOINT.y, SY_JOINT.z);
	//arm_ee_pos2=_V(arm_ee_pos2.z, -arm_ee_pos2.x, -arm_ee_pos2.y);
	//VECTOR3 dPos=arm_ee_pos2-arm_ee_pos;
	//sprintf_s(oapiDebugString(), 255, "Pos Error: %f %f %f Act: %f %f %f Calc: %f %f %f", dPos.x, dPos.y, dPos.z,
		//arm_ee_pos2.x, arm_ee_pos2.y, arm_ee_pos2.z, arm_ee_pos.x, arm_ee_pos.y, arm_ee_pos.z);

	/*VECTOR3 arm_ee_dir2=arm_tip[1]-arm_tip[0];
	arm_ee_dir2=_V(arm_ee_dir2.z, -arm_ee_dir2.x, -arm_ee_dir2.y);
	VECTOR3 dDir=arm_ee_dir2-arm_ee_dir;
	sprintf_s(oapiDebugString(), 255, "Dir Error: %f %f %f Act: %f %f %f Calc: %f %f %f", dDir.x, dDir.y, dDir.z,
		arm_ee_dir2.x, arm_ee_dir2.y, arm_ee_dir2.z, arm_ee_dir.x, arm_ee_dir.y, arm_ee_dir.z);*/

	/*VECTOR3 old_arm_ee_pos = arm_ee_pos;
	VECTOR3 old_arm_ee_dir = arm_ee_dir;
	VECTOR3 old_arm_ee_rot = arm_ee_rot;
	CalculateVectors();
	//sprintf_s(oapiDebugString(), 255, "Pos: %f %f %f Dir: %f %f %f", arm_ee_pos.x, arm_ee_pos.y, arm_ee_pos.z, arm_ee_dir.x, arm_ee_dir.y, arm_ee_dir.z);
	//sprintf_s(oapiDebugString(), 255, "Act Pos: %f %f %f Calc Pos: %f %f %f", arm_ee_pos.x, arm_ee_pos.y, arm_ee_pos.z, old_arm_ee_pos.x, old_arm_ee_pos.y, old_arm_ee_pos.z);
	//sprintf_s(oapiDebugString(), 255, "Act Dir: %f %f %f Calc Dir: %f %f %f", arm_ee_dir.x, arm_ee_dir.y, arm_ee_dir.z, old_arm_ee_dir.x, old_arm_ee_dir.y, old_arm_ee_dir.z);
	arm_ee_pos = old_arm_ee_pos;
	arm_ee_dir = old_arm_ee_dir;
	arm_ee_rot = old_arm_ee_rot;*/
}

void SSRMS::clbkDrawHUD(int mode, const HUDPAINTSPEC *hps, HDC hDC)
{
	char cbuf[255];

	//VESSEL2::clbkDrawHUD(mode, hps, hDC);

	//display joint angles
	sprintf(cbuf, "   SR      SY      SP      EP      WP      WY      WR");
	TextOut(hDC, hps->W/3, hps->H/10, cbuf, strlen(cbuf));
	sprintf(cbuf, "%7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f", joint_angle[SHOULDER_ROLL], joint_angle[SHOULDER_YAW], joint_angle[SHOULDER_PITCH],
		joint_angle[ELBOW_PITCH], joint_angle[WRIST_PITCH], joint_angle[WRIST_YAW], joint_angle[WRIST_ROLL]);
	TextOut(hDC, hps->W/3, (hps->H/10)+(hps->Markersize/2), cbuf, strlen(cbuf));

	//rotation/translation speed
	sprintf(cbuf, "Rotation speed: %d deg/sec", SpeedFactor);
	TextOut(hDC, hps->W/3, (hps->H/10)+(1.5*hps->Markersize), cbuf, strlen(cbuf));
	sprintf(cbuf, "Translation speed: %d ft/sec", SpeedFactor);
	TextOut(hDC, hps->W/3, (hps->H/10)+(2.5*hps->Markersize), cbuf, strlen(cbuf));

	if(RefFrame==EE_FRAME)
		TextOut(hDC, hps->W/3, (hps->H/10)+(3.5*hps->Markersize), "Using EE frame", 14);
	else TextOut(hDC, hps->W/3, (hps->H/10)+(3.5*hps->Markersize), "Using shoulder frame", 20);

	sprintf(cbuf, "LEE%d active", activeLEE+1);
	TextOut(hDC, hps->W/3, (hps->H/10)+(4.5*hps->Markersize), cbuf, strlen(cbuf));
	
	/*sprintf(cbuf, "SR: %.2f", sr_angle);
	TextOut(hDC, hps->W/3, hps->H/10, cbuf, strlen(cbuf));
	sprintf(cbuf, "SY: %.2f", sy_angle);
	TextOut(hDC, hps->W/3, (hps->H/10)+hps->Markersize, cbuf, strlen(cbuf));*/
}

void SSRMS::clbkPostCreation()
{
	//turn off attitude control sound
	OrbiterSoundHandle = ConnectToOrbiterSoundDLL3(GetHandle());
	if(OrbiterSoundHandle!=-1)SoundOptionOnOff3(OrbiterSoundHandle, PLAYATTITUDETHRUST, FALSE);

	pSubsystemDirector->RealizeAll();

	SetPropellantMass(ph_null, 1.0);

	if(arm_moved) {
		for(int i=0;i<7;i++) SetJointAngle((SSRMS_JOINT)i, joint_angle[i]);
	}

	VESSEL2::clbkPostCreation();
}

int SSRMS::clbkConsumeBufferedKey(DWORD key, bool down, char *kstate)
{
	if(KEYMOD_CONTROL(kstate)) {
		switch(key) {
			case OAPI_KEY_1:
				if(down) joint_motion[SHOULDER_ROLL]=-1;
				else joint_motion[SHOULDER_ROLL]=0;
				return 1;
			case OAPI_KEY_2:
				if(down) joint_motion[SHOULDER_YAW]=-1;
				else joint_motion[SHOULDER_YAW]=0;
				return 1;
			case OAPI_KEY_3:
				if(down) joint_motion[SHOULDER_PITCH]=-1;
				else joint_motion[SHOULDER_PITCH]=0;
				return 1;
			case OAPI_KEY_4:
				if(down) joint_motion[ELBOW_PITCH]=-1;
				else joint_motion[ELBOW_PITCH]=0;
				return 1;
			case OAPI_KEY_5:
				if(down) joint_motion[WRIST_PITCH]=-1;
				else joint_motion[WRIST_PITCH]=0;
				return 1;
			case OAPI_KEY_6:
				if(down) joint_motion[WRIST_YAW]=-1;
				else joint_motion[WRIST_YAW]=0;
				return 1;
			case OAPI_KEY_7:
				if(down) joint_motion[WRIST_ROLL]=-1;
				else joint_motion[WRIST_ROLL]=0;
				return 1;
			case OAPI_KEY_G:
				//if(!down) ToggleGrapple();
				if(!down) {
					oapiWriteLog("Toggling Grapple");
					if(pLEE[activeLEE]->Grappled()) pLEE[activeLEE]->Ungrapple();
					else pLEE[activeLEE]->Grapple();
				}
				return 1;
			case OAPI_KEY_F:
				if(!down) {
					if(RefFrame==EE_FRAME) RefFrame=(Frame)(RefFrame+1);
					else RefFrame=EE_FRAME;
				}
				return 1;
			case OAPI_KEY_A: // switch active LEE
				if(!down) {
					ChangeActiveLEE();
				}
				return 1;
		}
	}

	else if(!KEYMOD_SHIFT(kstate) && !KEYMOD_ALT(kstate)) {
		switch(key) {
			case OAPI_KEY_1:
				if(down) joint_motion[SHOULDER_ROLL]=1;
				else joint_motion[SHOULDER_ROLL]=0;
				return 1;
			case OAPI_KEY_2:
				if(down) joint_motion[SHOULDER_YAW]=1;
				else joint_motion[SHOULDER_YAW]=0;
				return 1;
			case OAPI_KEY_3:
				if(down) joint_motion[SHOULDER_PITCH]=1;
				else joint_motion[SHOULDER_PITCH]=0;
				return 1;
			case OAPI_KEY_4:
				if(down) joint_motion[ELBOW_PITCH]=1;
				else joint_motion[ELBOW_PITCH]=0;
				return 1;
			case OAPI_KEY_5:
				if(down) joint_motion[WRIST_PITCH]=1;
				else joint_motion[WRIST_PITCH]=0;
				return 1;
			case OAPI_KEY_6:
				if(down) joint_motion[WRIST_YAW]=1;
				else joint_motion[WRIST_YAW]=0;
				return 1;
			case OAPI_KEY_7:
				if(down) joint_motion[WRIST_ROLL]=1;
				else joint_motion[WRIST_ROLL]=0;
				return 1;
			case OAPI_KEY_G:
				if(!down) {
					if(!ShowAttachmentPoints) ShowAttachmentPoints=true;
					else ShowAttachmentPoints=false;
					oapiSetShowGrapplePoints(ShowAttachmentPoints);
				}
				return 1;
			case OAPI_KEY_S:
				if(!down) {
					if(SpeedFactor==1) SpeedFactor=2;
					else if(SpeedFactor==2) SpeedFactor=5;
					else if(SpeedFactor==5) SpeedFactor=10;
					else SpeedFactor=1;
				}
				return 1;
		}
	}

	return 0;
}

// --------------------------------------------------------------
// Module initialisation
// --------------------------------------------------------------
DLLCLBK void InitModule (HINSTANCE hModule)
{
	//g_hDLL = hModule;
}

DLLCLBK void ExitModule (HINSTANCE hModule)
{
}

// --------------------------------------------------------------
// Vessel initialisation
// --------------------------------------------------------------
DLLCLBK VESSEL *ovcInit (OBJHANDLE hvessel, int flightmodel)
{
	return new SSRMS(hvessel, flightmodel);
}

// --------------------------------------------------------------
// Vessel cleanup
// --------------------------------------------------------------
DLLCLBK void ovcExit (VESSEL *vessel)
{
	if (vessel) delete static_cast<SSRMS*>(vessel);
}