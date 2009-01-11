#include "RMSSystem.h"

RMSSystem::RMSSystem(SubsystemDirector *_director)
	: AtlantisSubsystem(_director, "RMS")
{
	payload=NULL;
	grapple=NULL;
	end_effector=NULL;
	detached=false;
}

RMSSystem::~RMSSystem()
{
}

void RMSSystem::Realize()
{
}

void RMSSystem::OnPostStep(double fSimT, double fDeltaT, double fMJD)
{
	if(!detached && payload!=NULL && !STS()->GetAttachmentStatus(end_effector)) {
		if(PayloadIsFree()) STS()->AttachChild(payload->GetHandle(), end_effector, grapple);
	}
}

bool RMSSystem::OnParseLine(const char* line)
{
	return false;
}

void RMSSystem::OnSaveState(FILEHANDLE scn) const
{
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
		DWORD count=payload->AttachmentCount(true);
		for(DWORD i=0;i<count;i++) {
			ATTACHMENTHANDLE att=payload->GetAttachmentHandle(true, i);
			if(payload->GetAttachmentStatus(att)) return false;
		}
	}
	return true;
}