#include "SSRMS_Latch.h"
#include "SSRMS.h"

/*LatchSystem::LatchSystem(SubsystemDirector *_director, const std::string &_ident, const std::string &_attachID)
{
	attachedPayload=NULL;
	hPayloadAttachment=NULL;
	hAttach=NULL;

	AttachID=_attachID;

	firstStep=true;

	payloadName="";
	attachmentIndex=-1;
}*/

LatchSystem::LatchSystem(SSRMS* _ssrms, const std::string &_ident, const std::string &_attachID)
: isActive(true), isGrappledToBase(false), firstStep(true)
{
	ssrms=_ssrms;
	identifier=_ident;

	attachedPayload=NULL;
	hPayloadAttachment=NULL;
	hAttach=NULL;

	AttachID=_attachID;

	payloadName="";
	attachmentIndex=-1;
}

LatchSystem::~LatchSystem()
{
}

void LatchSystem::SetAttachment(ATTACHMENTHANDLE ah, bool active)
{
	if(ah) {
		hAttach = ah;
		isActive = active;
	}
}

void LatchSystem::SetAttachmentParams(const VECTOR3& pos, const VECTOR3& dir, const VECTOR3& rot) const
{
	GetSSRMS()->SetAttachmentParams(hAttach, pos, dir, rot);
}

OBJHANDLE LatchSystem::Grapple()
{
	VESSEL* pVessel=NULL;

	// check child attach points first
	ATTACHMENTHANDLE hAtt=FindPayload(false, &pVessel);
	if(hAtt) {
		AttachPayload(pVessel, hAtt);
		isGrappledToBase = false;
	}
	else { // check for base PDGF
		hAtt=FindPayload(true, &pVessel);		
		if(hAtt) {
			AttachPayload(pVessel, hAtt);
			isGrappledToBase = true;
			oapiWriteLog("Grappled base PDGF");
		}
	}

	if(pVessel) return pVessel->GetHandle();
	return NULL;
}

void LatchSystem::Ungrapple()
{
	DetachPayload();
}

void LatchSystem::OnPreStep(double SimT, double DeltaT, double MJD)
{
	if(firstStep) {
		CheckForAttachedObjects();
		firstStep=false;
	}

	if(attachedPayload!=NULL && !GetSSRMS()->GetAttachmentStatus(hAttach)) {
		if(isActive && !isGrappledToBase) {
			if(PayloadIsFree()) {
				GetSSRMS()->AttachChild(attachedPayload->GetHandle(), hAttach, hPayloadAttachment);
				//double mass=GetSSRMS()->GetEmptyMass()+attachedPayload->GetMass();
				//GetSSRMS()->SetEmptyMass(mass);
			}
		}
		else if(!isActive && isGrappledToBase) { // force parent vessel to attach
			attachedPayload->AttachChild(GetSSRMS()->GetHandle(), hPayloadAttachment, hAttach);
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

	//detached=false;

	//OnAttach();
}

void LatchSystem::DetachPayload()
{
	/*if(attachedPayload) {
		// remove mass of released payload
		double mass=GetSSRMS()->GetEmptyMass()-attachedPayload->GetMass();
		GetSSRMS()->SetEmptyMass(mass);
	}*/
	if(isActive) {
		GetSSRMS()->DetachChild(hAttach);
	}
	else {
		if(attachedPayload) {
			attachedPayload->DetachChild(hPayloadAttachment);
		}
	}
	hPayloadAttachment=NULL;
	attachedPayload=NULL;

	//OnDetach();
}

ATTACHMENTHANDLE LatchSystem::FindPayload(bool checkBaseAttachment, VESSEL** pVessel) const
{
	oapiWriteLog("Looking for payload");
	VECTOR3 gpos, gdir, grms, pos, dir, rot, grmsdir;
	GetSSRMS()->GetAttachmentParams(hAttach, pos, dir, rot);
	GetSSRMS()->Local2Global (pos, grms);  // global position of RMS tip
	GetSSRMS()->GlobalRot(dir, grmsdir);

	// Search the complete vessel list for a grappling candidate.
	// Not very scalable ...
	for (DWORD i = 0; i < oapiGetVesselCount(); i++) {
		OBJHANDLE hV = oapiGetVesselByIndex (i);
		if (hV == GetSSRMS()->GetHandle()) continue; // we don't want to grapple ourselves ...
		oapiGetGlobalPos (hV, &gpos);
		if (dist (gpos, grms) < 10*oapiGetSize (hV)) { // in range
			VESSEL *v = oapiGetVesselInterface (hV);

			DWORD nAttach = v->AttachmentCount (!checkBaseAttachment);
			for (DWORD j = 0; j < nAttach; j++) { // now scan all attachment points of the candidate
				ATTACHMENTHANDLE hAtt = v->GetAttachmentHandle (!checkBaseAttachment, j);
				if(CanAttach(v, hAtt, grms, grmsdir)) {
					if(pVessel) *pVessel=v;
					return hAtt;
				}
			}
		}
	}
	if(pVessel) *pVessel=NULL;
	return NULL;
}

bool LatchSystem::CanAttach(VESSEL* v, ATTACHMENTHANDLE ah, const VECTOR3& glatchpos, const VECTOR3& glatchdir) const
{
	const char *id = v->GetAttachmentId (ah);
	if (strncmp (id, AttachID.c_str(), AttachID.length())) 
		return false; // attachment point not compatible

	VECTOR3 pos, dir, rot;
	VECTOR3 gpos, gdir;
	v->GetAttachmentParams (ah, pos, dir, rot);
	v->Local2Global (pos, gpos);
	if (dist (gpos, glatchpos) < MAX_GRAPPLING_DIST) { 
		v->GlobalRot(dir, gdir);
		if(fabs(PI-acos(dotp(gdir, glatchdir))) < MAX_GRAPPLING_ANGLE)
			return true;
	}
	return false;
}

bool LatchSystem::PayloadIsFree() const
{
	if(attachedPayload) {
		//if we are attached to payload, it must be 'free'
		if(GetSSRMS()->GetAttachmentStatus(hAttach)) return true;
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
		OBJHANDLE hV=GetSSRMS()->GetAttachmentStatus(hAttach);
		if(hV) {
			attachedPayload=oapiGetVesselInterface(hV);
			//double mass=GetSSRMS()->GetEmptyMass()+attachedPayload->GetMass();
			//GetSSRMS()->SetEmptyMass(mass);
			// find handle of attachment point on payload
			for(DWORD i=0;i<attachedPayload->AttachmentCount(isActive);i++) {
				ATTACHMENTHANDLE hAtt=attachedPayload->GetAttachmentHandle(isActive, i);
				if(attachedPayload->GetAttachmentStatus(hAtt)==GetSSRMS()->GetHandle()) {
					hPayloadAttachment=hAtt;
					//OnAttach();
					isGrappledToBase = !isActive;
					oapiWriteLog("Found grappled object");
					oapiWriteLog((char*)GetIdentifier().c_str());
					return;
				}
			}
		}
		else { // check data loaded from scenario
			if(attachmentIndex!=-1 && !payloadName.empty()) {
				hV=oapiGetVesselByName((char*)payloadName.c_str());
				if(hV) {
					VESSEL* v=oapiGetVesselInterface(hV);
					ATTACHMENTHANDLE attach=v->GetAttachmentHandle(isActive, attachmentIndex);
					AttachPayload(v, attach);
				}
			}
		}
	}
}