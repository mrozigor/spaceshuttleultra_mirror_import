#include "Latch.h"
#include "RMSSystem.h"
#include "StbdMPMSystem.h"
#include <UltraMath.h>
//#include "util/Stopwatch.h"

LatchSystem::LatchSystem(AtlantisSubsystemDirector *_director, const std::string &_ident, const std::string &_attachID)
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
	//oapiWriteLog((char*)(GetIdentifier().c_str()));
	if(firstStep) {
		//oapiWriteLog("First step");
		CheckForAttachedObjects();
		firstStep=false;
	}

	if(attachedPayload!=NULL && !STS()->GetAttachmentStatus(hAttach)) {
		if(PayloadIsFree()) {
			/*STS()->AttachChild(attachedPayload->GetHandle(), hAttach, hPayloadAttachment);
			double mass=STS()->GetEmptyMass()+attachedPayload->GetMass();
			STS()->SetEmptyMass(mass);*/
			STS()->AttachChildAndUpdateMass(attachedPayload->GetHandle(), hAttach, hPayloadAttachment);
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
		//double mass=STS()->GetEmptyMass()-attachedPayload->GetMass();
		//STS()->SetEmptyMass(mass);

		// signal to RMS and MPMs that payload they are attached to has been unlatched
		if(STS()->pRMS && STS()->pRMS!=this) STS()->pRMS->CheckDoubleAttach(attachedPayload, false);
		if(STS()->pMPMs && STS()->pMPMs!=this) STS()->pMPMs->CheckDoubleAttach(attachedPayload, false);
	}
	hPayloadAttachment=NULL;
	attachedPayload=NULL;
	//STS()->DetachChild(hAttach);
	STS()->DetachChildAndUpdateMass(hAttach);

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
	VECTOR3 grms, pos, dir, rot, grmsdir;
	ATTACHMENTHANDLE hAtt;
	STS()->GetAttachmentParams(hAttach, pos, dir, rot);
	STS()->Local2Global (STS()->GetOrbiterCoGOffset()+pos, grms);  // global position of RMS tip
	STS()->GlobalRot(dir, grmsdir);

	// Search the complete vessel list for a grappling candidate.
	// Not very scalable ...
	for (DWORD i = 0; i < oapiGetVesselCount(); i++) {
		OBJHANDLE hV = oapiGetVesselByIndex (i);
		/*if (hV == STS()->GetHandle()) continue; // we don't want to grapple ourselves ...
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
				sprintf_s(oapiDebugString(), 255, "%s %s Dist: %f", v->GetName(), id, dist(gpos, grms));
				//oapiWriteLog(oapiDebugString());
				if (dist (gpos, grms) < MAX_GRAPPLING_DIST) { 
					v->GlobalRot(dir, gdir);
					double dot_product = range(-1, dotp(gdir, grmsdir), 1);
					if(fabs(PI-acos(dot_product)) < MAX_GRAPPLING_ANGLE) {
						if(pVessel) *pVessel=v;
						return hAtt;
					}
				}
			}
		}*/
		hAtt = CanAttach(hV, grms, grmsdir);
		if(hAtt) {
			if(pVessel) *pVessel = oapiGetVesselInterface(hV);
			return hAtt;
		}
	}
	if(pVessel) *pVessel=NULL;
	return NULL;
}

ATTACHMENTHANDLE LatchSystem::CanAttach(OBJHANDLE hV, const VECTOR3& glatchpos, const VECTOR3& glatchdir) const
{
	if (hV == STS()->GetHandle()) return NULL; // we don't want to grapple ourselves ...

	VECTOR3 gpos, gdir, pos, dir, rot;
	oapiGetGlobalPos (hV, &gpos);
	if (dist (gpos, glatchpos) < oapiGetSize (hV)) { // in range
		VESSEL *v = oapiGetVesselInterface (hV);
		DWORD nAttach = v->AttachmentCount (true);
		for (DWORD j = 0; j < nAttach; j++) { // now scan all attachment points of the candidate
			ATTACHMENTHANDLE hAtt = v->GetAttachmentHandle (true, j);
			const char *id = v->GetAttachmentId (hAtt);
			if (strncmp (id, AttachID.c_str(), AttachID.length())) 
				continue; // attachment point not compatible

			v->GetAttachmentParams (hAtt, pos, dir, rot);
			v->Local2Global (pos, gpos);
			//sprintf_s(oapiDebugString(), 255, "%s %s Dist: %f", v->GetName(), id, dist(gpos, glatchpos));
			//oapiWriteLog(oapiDebugString());
			if (dist (gpos, glatchpos) < MAX_GRAPPLING_DIST) { 
				v->GlobalRot(dir, gdir);
				double dot_product = range(-1, dotp(gdir, glatchdir), 1);
				if(fabs(PI-acos(dot_product)) < MAX_GRAPPLING_ANGLE) {
					return hAtt;
				}
			}
		}
	}

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
			//double mass=STS()->GetEmptyMass()+attachedPayload->GetMass();
			//STS()->SetEmptyMass(mass);
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
				hV=oapiGetVesselByName((char*)payloadName.c_str());
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
// ActiveLatchGroup class
//////////////////////////////////////////////////////////////

ActiveLatchGroup::ActiveLatchGroup(AtlantisSubsystemDirector *_director, const std::string &_ident, const VECTOR3 &_pos, const VECTOR3 &_dir, const VECTOR3 &_rot)
	: LatchSystem(_director, _ident, "XS"), usLatchNum(5), lastUpdateTime(-100.0)
{
	SetAttachmentParams(_pos, _dir, _rot);

	for(unsigned short i=0;i<5;i++) LatchState[i].Set(AnimState::OPEN, 1.0);
}

ActiveLatchGroup::~ActiveLatchGroup()
{
}

void ActiveLatchGroup::Realize()
{
	LatchSystem::Realize();

	DiscreteBundle* pBundle = BundleManager()->CreateBundle(GetIdentifier(), 10);
	for(unsigned short i=0;i<5;i++) {
		LatchSignal[i].Connect(pBundle, 2*i);
		ReleaseSignal[i].Connect(pBundle, 2*i+1);
	}

	pBundle = BundleManager()->CreateBundle(GetIdentifier()+"_STATE", 15);
	for(unsigned short i=0;i<5;i++) {
		Latched[i].Connect(pBundle, 3*i);
		Released[i].Connect(pBundle, 3*i+1);
		ReadyToLatch[i].Connect(pBundle, 3*i+2);
	}
}

void ActiveLatchGroup::OnPreStep(double SimT, double DeltaT, double MJD)
{
	LatchSystem::OnPreStep(SimT, DeltaT, MJD);

	for(unsigned short i=0;i<usLatchNum;i++) {
		if(LatchSignal[i] && !LatchState[i].Closed()) {
			LatchState[i].action=AnimState::CLOSING;
			LatchState[i].Move(DeltaT/LATCH_CLOSE_TIME);
			// if even one latch is closed, we can latch payload
			if(LatchState[i].Closed()) {
				Latch();
				//sprintf_s(oapiDebugString(), 255, "%s: latched latch %d", GetIdentifier().c_str(), i);
			}
		}
		else if(ReleaseSignal[i] && !LatchState[i].Open()) {
			LatchState[i].action=AnimState::OPENING;
			LatchState[i].Move(DeltaT/LATCH_CLOSE_TIME);
			// if all latches are open, release payload
			if(LatchState[i].Open()) {
				sprintf_s(oapiDebugString(), 255, "%s: released latch %d", GetIdentifier().c_str(), i);
				/*bool rel=true;
				for(unsigned short j=0;j<usLatchNum;j++) {
					if(!LatchState[j].Open()) rel=false;
				}
				if(rel) Release();*/
				if(AllLatchesOpen()) Release();
			}
		}

		if(LatchState[i].Open()) {
			Latched[i].ResetLine();
			Released[i].SetLine();
		}
		else if(LatchState[i].Closed()) {
			Latched[i].SetLine();
			Released[i].ResetLine();
		}
		else {
			Latched[i].ResetLine();
			Released[i].ResetLine();
		}
	}

	// every 10 seconds, update list of nearby payloads
	double Update = SimT-lastUpdateTime;
	if(Update>10.0 || Update<0.0) {
		PopulatePayloadList();
		lastUpdateTime=SimT;
	}

	//Stopwatch st;
	//st.Start();
	if(CheckRTL()) {
		sprintf_s(oapiDebugString(), 255, "%s: Ready to Latch %f", GetIdentifier().c_str(), oapiRand());
		for(int i=0;i<5;i++) ReadyToLatch[i].SetLine();
	}
	else {
		for(int i=0;i<5;i++) ReadyToLatch[i].ResetLine();
	}
	//double time = st.Stop();
	//sprintf_s(oapiDebugString(), 255, "Time: %f", time);

	//sprintf_s(oapiDebugString(), 255, "Latch state: %f %f %f %f %f", LatchState[0].pos, LatchState[1].pos, LatchState[2].pos,
		//LatchState[3].pos, LatchState[4].pos);
}

bool ActiveLatchGroup::OnParseLine(const char *line)
{
	if(LatchSystem::OnParseLine(line)) return true;
	else {
		if(!_strnicmp(line, "LATCH_STATE", 11)) {
			char num[2];
			num[0]=line[11];
			num[1]='\0';
			int index = atoi(num)-1;

			if(index>=0 && index<5) {
				sscan_state((char*)(line+12), LatchState[index]);
				return true;
			}
		}
	}

	return false;
}

void ActiveLatchGroup::OnSaveState(FILEHANDLE scn) const
{
	LatchSystem::OnSaveState(scn);
	WriteScenario_state(scn, "LATCH_STATE1", LatchState[0]);
	WriteScenario_state(scn, "LATCH_STATE2", LatchState[1]);
	WriteScenario_state(scn, "LATCH_STATE3", LatchState[2]);
	WriteScenario_state(scn, "LATCH_STATE4", LatchState[3]);
	WriteScenario_state(scn, "LATCH_STATE5", LatchState[4]);
}

void ActiveLatchGroup::CreateAttachment()
{
	if(!hAttach)
		hAttach=STS()->CreateAttachment(false, STS()->GetOrbiterCoGOffset()+pos, dir, rot, AttachID.c_str());
	else STS()->SetAttachmentParams(hAttach, STS()->GetOrbiterCoGOffset()+pos, dir, rot);
}

void ActiveLatchGroup::SetAttachmentParams(const VECTOR3 &_pos, const VECTOR3 &_dir, const VECTOR3 &_rot)
{
	pos=_pos;
	dir=_dir;
	rot=_rot;
	if(hAttach) CreateAttachment(); // update attachment
}

void ActiveLatchGroup::Latch()
{
	if(!IsLatched()) {
		ATTACHMENTHANDLE hTarget=NULL;
		VESSEL* pTarget=NULL;
		hTarget=FindPayload(&pTarget);
		//hTarget=LatchSystem::FindPayload(&pTarget);
		if(hTarget && pTarget) AttachPayload(pTarget, hTarget);
		//sprintf_s(oapiDebugString(), 255, "%s", AttachID.c_str());
		//sprintf_s(oapiDebugString(), 355, "%s: Latch called %f", GetIdentifier().c_str(), oapiRand());
	}
}

void ActiveLatchGroup::Release()
{
	if(IsLatched()) DetachPayload();
	//sprintf_s(oapiDebugString(), 355, "%s: Release called %f", GetIdentifier().c_str(), oapiRand());
}

void ActiveLatchGroup::OnAttach()
{
	if(IsFirstStep() && AllLatchesOpen()) { 
		// if scn file says we are attached and latches are open, set all latch states to LATCHED
		for(unsigned short i=0;i<5;i++) LatchState[i].Set(AnimState::CLOSED, 0.0);
	}

	char cbuf[255];
	sprintf_s(cbuf, 255, "%s latched", GetIdentifier().c_str());
	oapiWriteLog(cbuf);
}

void ActiveLatchGroup::OnDetach()
{
	char cbuf[255];
	sprintf_s(cbuf, 255, "%s released", GetIdentifier().c_str());
	oapiWriteLog(cbuf);
}

ATTACHMENTHANDLE ActiveLatchGroup::FindPayload(VESSEL** pVessel) const
{
	ATTACHMENTHANDLE hAtt = NULL;
	VECTOR3 glatchpos, glatchdir, pos, dir, rot;

	STS()->GetAttachmentParams(hAttach, pos, dir, rot);
	STS()->Local2Global (STS()->GetOrbiterCoGOffset()+pos, glatchpos);
	STS()->GlobalRot(dir, glatchdir);

	for(unsigned int i=0;i<vhPayloads.size();i++) {
		// if handle has become invalid, skip vessel
		// payload list is updated every 10 seconds and should delete invalid element
		if(!oapiIsVessel(vhPayloads.at(i))) continue;

		hAtt = CanAttach(vhPayloads.at(i), glatchpos, glatchdir);
		if(hAtt) {
			//sprintf_s(oapiDebugString(), 255, "%s: found payload", GetIdentifier().c_str());
			if(pVessel) *pVessel=oapiGetVesselInterface(vhPayloads[i]);
			return hAtt;
		}
	}

	if(pVessel) *pVessel=NULL;
	return NULL;
}


bool ActiveLatchGroup::CheckRTL() const
{
	return (FindPayload()!=NULL);
}

bool ActiveLatchGroup::AllLatchesOpen() const
{
	for(unsigned short j=0;j<usLatchNum;j++) {
		if(!LatchState[j].Open()) return false;
	}
	return true;
}

void ActiveLatchGroup::PopulatePayloadList()
{
	VECTOR3 gpos, grms, pos, dir, rot;
	STS()->GetAttachmentParams(hAttach, pos, dir, rot);
	STS()->Local2Global (STS()->GetOrbiterCoGOffset()+pos, grms);  // global position of RMS tip

	// clear list
	vhPayloads.clear();

	// Search the complete vessel list for a grappling candidate.
	// Not very scalable ...
	for (DWORD i = 0; i < oapiGetVesselCount(); i++) {
		OBJHANDLE hV = oapiGetVesselByIndex (i);
		if(hV == STS()->GetHandle()) continue;
		oapiGetGlobalPos (hV, &gpos);
		if(dist(grms, gpos) < 2*oapiGetSize(hV)) vhPayloads.push_back(hV);
		//sprintf_s(oapiDebugString(), 255, "dist: %f", dist(grms, gpos));
	}
}