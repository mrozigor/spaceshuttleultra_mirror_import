#include "StbdMPMSystem.h"
#include "RMSSystem.h"

StbdMPMSystem::StbdMPMSystem(SubsystemDirector *_director)
	: MPMSystem(_director, "STBD_MPM", MPM_MESHNAME, MPM_MESH_OFFSET, "OBSS")
{
	obss_attach_point[0]=OBSS_ATTACHMENT_POINT;
	obss_attach_point[1]=OBSS_ATTACHMENT_POINT+_V(0.00, 1.00, 0.00);
}

StbdMPMSystem::~StbdMPMSystem()
{
}

void StbdMPMSystem::Realize()
{
	MPMSystem::Realize();

	// MPM animation
	static UINT STBDMPMGrp[1] = {0}; //only group in mesh
	static MGROUP_ROTATE MPMAnim (mesh_index, STBDMPMGrp, 1,
		_V(-0.164, -0.356, 0), _V(0, 0, 1), (float)(-31.36*RAD));
	anim_mpm = STS()->CreateAnimation(1.0);
	ANIMATIONCOMPONENT_HANDLE parent = STS()->AddAnimationComponent(anim_mpm, 0, 1, &MPMAnim);

	static MGROUP_ROTATE MPMAttachment (LOCALVERTEXLIST, MAKEGROUPARRAY(obss_attach_point), 2,
		_V(2.87, 1.90, 3.15), _V(0,0,1), (float)(0.0));
	parent = STS()->AddAnimationComponent(anim_mpm, 0, 1, &MPMAttachment, parent);

	if(hAttach && STS()->GetAttachmentStatus(hAttach)) MRLLatches.Set(AnimState::CLOSED, 0.0);
	STS()->SetAnimation(anim_mpm, MPMRollout.pos);
}

void StbdMPMSystem::CreateAttachment()
{
	hAttach=STS()->CreateAttachment(false, STS()->GetOrbiterCoGOffset()+OBSS_ATTACHMENT_POINT, _V(0,1,0), _V(0,0,1), "OBSS");
}

void StbdMPMSystem::OnPreStep(double SimT, double DeltaT, double MJD)
{
	// if we haven't found the OBSS yet, check for any new vessels added
	if(vhOBSSAttach.empty()) FindOBSSAttachments();

	if(CheckRTL()) {
		for(int i=0;i<3;i++) MRL_RTL_Microswitches[i].SetLine();
	}
	else {
		for(int i=0;i<3;i++) MRL_RTL_Microswitches[i].ResetLine();
	}

	MPMSystem::OnPreStep(SimT, DeltaT, MJD);

	// check for SimT is needed to ensure OBSS attachment point is positioned correctly
	if(mpm_moved || SimT<0.25) {
		STS()->SetAttachmentParams(hAttach, STS()->GetOrbiterCoGOffset()+obss_attach_point[0]+MPM_MESH_OFFSET, 
			obss_attach_point[1]-obss_attach_point[0], _V(0, 0, 1));
	}
}

void StbdMPMSystem::OnSaveState(FILEHANDLE scn) const
{
	oapiWriteLine(scn, "  STBD_MPM");
	MPMSystem::OnSaveState(scn);
}

void StbdMPMSystem::OnMRLLatched()
{
	if(!hPayloadAttachment) AttachOBSS();
}

void StbdMPMSystem::OnMRLReleased()
{
	if(hPayloadAttachment) DetachPayload();
}

void StbdMPMSystem::AttachOBSS()
{
	if(!hPayloadAttachment) {
		int index=FindOBSS();
		if(index!=-1) {
			// if OBSS is attached to RMS, detach it so it can be attached to MPM
			//STS()->pRMS->Detach(vpOBSS[index]);
			AttachPayload(vpOBSS[index], vhOBSSAttach[index]);
		}
	}
	//if(index!=-1) STS()->AttachChild(vpOBSS[index]->GetHandle(), hAttach, vhOBSSAttach[index]);
}

void StbdMPMSystem::FindOBSSAttachments()
{
	//iterate through all vessels and search for attachments with 'OS' string
	DWORD vesselCount=oapiGetVesselCount();
	for (DWORD i = 0; i < vesselCount; i++) {
		OBJHANDLE hV = oapiGetVesselByIndex (i);
		if (hV != STS()->GetHandle()) // we don't want to grapple ourselves ...
		{
			VESSEL* v=oapiGetVesselInterface(hV);
			DWORD attachCount = v->AttachmentCount (true);
			for (DWORD j = 0; j < attachCount; j++) { // now scan all attachment points of the candidate
				ATTACHMENTHANDLE hAtt = v->GetAttachmentHandle (true, j);
				const char *id = v->GetAttachmentId (hAtt);
				if(!_strnicmp(id, "OS", 2)) {
					vpOBSS.push_back(v);
					vhOBSSAttach.push_back(hAtt);
				}
			}
		}
	}
}

int StbdMPMSystem::FindOBSS() const
{
	VECTOR3 gpos, gdir, gattach, pos, dir, rot, gattachdir;
	STS()->Local2Global (STS()->GetOrbiterCoGOffset()+obss_attach_point[0]+MPM_MESH_OFFSET, gattach);  // global position of attachment point
	STS()->GlobalRot(obss_attach_point[1]-obss_attach_point[0], gattachdir);
	//loop through OBSS attachments and check each one
	for(int i=0;i<vhOBSSAttach.size();i++) {
		VESSEL* v=vpOBSS[i];
		v->GetAttachmentParams (vhOBSSAttach[i], pos, dir, rot);
		v->Local2Global (pos, gpos);
		sprintf_s(oapiDebugString(), 255, "%s Dist: %f", v->GetName(), dist(gpos, gattach));
		if (dist (gpos, gattach) < MAX_ATTACHMENT_DIST) { 
			v->GlobalRot(dir, gdir);
			sprintf_s(oapiDebugString(), 255, "Attitude difference: %f", fabs(180-DEG*acos(dotp(gdir, gattachdir))));
			if(fabs(PI-acos(range(-1.0, dotp(gdir, gattachdir), 1.0))) < MAX_ATTACHMENT_ANGLE) {  // found one!
				return i;
			}
		}
	}

	return -1;
}

bool StbdMPMSystem::CheckRTL() const
{
	//if OBSS is latched to MPMs, RTL switches should be set
	if(STS()->GetAttachmentStatus(hAttach)) return true;

	return (FindOBSS()!=-1);
}