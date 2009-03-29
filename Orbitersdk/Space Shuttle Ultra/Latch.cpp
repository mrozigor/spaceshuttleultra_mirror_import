#include "Latch.h"
#include "RMSSystem.h"
#include "StbdMPMSystem.h"

LatchSystem::LatchSystem(SubsystemDirector *_director, const std::string &_ident, const std::string &_attachID)
	: AtlantisSubsystem(_director, _ident)
{
	attachedPayload=NULL;
	hPayloadAttachment=NULL;
	hAttach=NULL;

	AttachID=_attachID;

	firstStep=true;

	payloadName="";
	attachmentIndex=-1;
}

LatchSystem::~LatchSystem()
{
}

void LatchSystem::OnPreStep(double SimT, double DeltaT, double MJD)
{
	if(firstStep) {
		CheckForAttachedObjects();
		firstStep=false;
	}

	if(attachedPayload!=NULL && !STS()->GetAttachmentStatus(hAttach)) {
		if(PayloadIsFree()) {
			STS()->AttachChild(attachedPayload->GetHandle(), hAttach, hPayloadAttachment);
			double mass=STS()->GetEmptyMass()+attachedPayload->GetMass();
			STS()->SetEmptyMass(mass);
		}
	}
}

bool LatchSystem::OnParseLine(const char* line)
{
	const std::string label = GetIdentifier()+"_ATTACHED_PAYLOAD ";
	if(!_strnicmp(line, label.c_str(), label.length())) {
		std::string temp=line+label.length();
		int index=temp.find(' ');
		payloadName=temp.substr(0, index);
		std::string num=temp.substr(index+1);
		attachmentIndex=atoi(num.c_str());
		
		char cbuf[255];
		sprintf_s(cbuf, 255, "%s payload: %s %d", GetIdentifier().c_str(), payloadName.c_str(), attachmentIndex);
		oapiWriteLog(cbuf);
		return true;
	}
	return false;
}

void LatchSystem::OnSaveState(FILEHANDLE scn) const
{
	if(hPayloadAttachment) {
		char pData[55], pIdent[55];
		sprintf_s(pData, 55, "%s %d", attachedPayload->GetName(), attachedPayload->GetAttachmentIndex(hPayloadAttachment));
		sprintf_s(pIdent, 55, "%s_ATTACHED_PAYLOAD", GetIdentifier().c_str());
		oapiWriteScenario_string(scn, pIdent, pData);
	}
}

void LatchSystem::AttachPayload(VESSEL* vessel, ATTACHMENTHANDLE attachment)
{
	//for the moment, assume attachment passed is completely valid
	hPayloadAttachment=attachment;
	attachedPayload=vessel;

	// needed to prevent RMS and MPMs from moving when payload they are attached to is latched to something else
	if(STS()->pRMS && STS()->pRMS!=this) STS()->pRMS->CheckDoubleAttach(vessel, true);
	if(STS()->pMPMs && STS()->pMPMs!=this) STS()->pMPMs->CheckDoubleAttach(vessel, true);
	//detached=false;

	OnAttach();
}

void LatchSystem::DetachPayload()
{
	if(attachedPayload) {
		// remove mass of released payload
		double mass=STS()->GetEmptyMass()-attachedPayload->GetMass();
		STS()->SetEmptyMass(mass);

		// signal to RMS and MPMs that payload they are attached to has been unlatched
		if(STS()->pRMS && STS()->pRMS!=this) STS()->pRMS->CheckDoubleAttach(attachedPayload, false);
		if(STS()->pMPMs && STS()->pMPMs!=this) STS()->pMPMs->CheckDoubleAttach(attachedPayload, false);
	}
	hPayloadAttachment=NULL;
	attachedPayload=NULL;
	STS()->DetachChild(hAttach);

	OnDetach();
}

/*void LatchSystem::Detach(VESSEL* vessel)
{
	if(vessel==NULL || vessel==attachedPayload) {
		STS()->DetachChild(hAttach);
		detached=true;
	}
}*/

ATTACHMENTHANDLE LatchSystem::FindPayload(VESSEL** pVessel) const
{
	VECTOR3 gpos, gdir, grms, pos, dir, rot, grmsdir;
	STS()->GetAttachmentParams(hAttach, pos, dir, rot);
	STS()->Local2Global (STS()->GetOrbiterCoGOffset()+pos, grms);  // global position of RMS tip
	STS()->GlobalRot(dir, grmsdir);

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
				if (strncmp (id, AttachID.c_str(), AttachID.length())) 
					continue; // attachment point not compatible

				v->GetAttachmentParams (hAtt, pos, dir, rot);
				v->Local2Global (pos, gpos);
				//sprintf_s(oapiDebugString(), 255, "%s %s Dist: %f", v->GetName(), id, dist(gpos, grms));
				//oapiWriteLog(oapiDebugString());
				if (dist (gpos, grms) < MAX_GRAPPLING_DIST) { 
					v->GlobalRot(dir, gdir);
					if(fabs(PI-acos(dotp(gdir, grmsdir))) < MAX_GRAPPLING_ANGLE) {
						if(pVessel) *pVessel=v;
						return hAtt;
					}
				}
			}
		}
	}
	if(pVessel) *pVessel=NULL;
	return NULL;
}

bool LatchSystem::PayloadIsFree() const
{
	if(attachedPayload) {
		//if we are attached to payload, it must be 'free'
		if(STS()->GetAttachmentStatus(hAttach)) return true;
		//otherwise, loop through all attachment points on payload and check if any of them are in use
		DWORD count=attachedPayload->AttachmentCount(true);
		for(DWORD i=0;i<count;i++) {
			ATTACHMENTHANDLE att=attachedPayload->GetAttachmentHandle(true, i);
			if(attachedPayload->GetAttachmentStatus(att)) return false;
		}
	}
	return true;
}

void LatchSystem::CheckForAttachedObjects()
{
	if(hAttach) {
		OBJHANDLE hV=STS()->GetAttachmentStatus(hAttach);
		if(hV) {
			attachedPayload=oapiGetVesselInterface(hV);
			double mass=STS()->GetEmptyMass()+attachedPayload->GetMass();
			STS()->SetEmptyMass(mass);
			// find handle of attachment point on payload
			for(DWORD i=0;i<attachedPayload->AttachmentCount(true);i++) {
				ATTACHMENTHANDLE hAtt=attachedPayload->GetAttachmentHandle(true, i);
				if(attachedPayload->GetAttachmentStatus(hAtt)==STS()->GetHandle()) {
					hPayloadAttachment=hAtt;
					OnAttach();
					return;
				}
			}
		}
		else { // check data loaded from scenario
			if(attachmentIndex!=-1 && !payloadName.empty()) {
				OBJHANDLE hV=oapiGetVesselByName((char*)payloadName.c_str());
				if(hV) {
					VESSEL* v=oapiGetVesselInterface(hV);
					ATTACHMENTHANDLE attach=v->GetAttachmentHandle(true, attachmentIndex);
					AttachPayload(v, attach);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////
// ActiveLatch class
//////////////////////////////////////////////////////////////

ActiveLatch::ActiveLatch(SubsystemDirector *_director, const std::string &_ident, const VECTOR3 &_pos, const VECTOR3 &_dir, const VECTOR3 &_rot)
	: LatchSystem(_director, _ident, "XS")
{
	pos=_pos;
	dir=_dir;
	rot=_rot;
}

ActiveLatch::~ActiveLatch()
{
}

void ActiveLatch::CreateAttachment()
{
	if(!hAttach)
		hAttach=STS()->CreateAttachment(false, STS()->GetOrbiterCoGOffset()+pos, dir, rot, AttachID.c_str());
	else STS()->SetAttachmentParams(hAttach, STS()->GetOrbiterCoGOffset()+pos, dir, rot);
}

void ActiveLatch::Latch()
{
	ATTACHMENTHANDLE hTarget=NULL;
	VESSEL* pTarget=NULL;
	hTarget=FindPayload(&pTarget);
	if(hTarget && pTarget) AttachPayload(pTarget, hTarget);
	sprintf_s(oapiDebugString(), 55, "%s", AttachID.c_str());
}

void ActiveLatch::Release()
{
	DetachPayload();
}

void ActiveLatch::OnAttach()
{
	char cbuf[255];
	sprintf_s(cbuf, 255, "%s latched", GetIdentifier().c_str());
	oapiWriteLog(cbuf);
}

void ActiveLatch::OnDetach()
{
	char cbuf[255];
	sprintf_s(cbuf, 255, "%s released", GetIdentifier().c_str());
	oapiWriteLog(cbuf);
}