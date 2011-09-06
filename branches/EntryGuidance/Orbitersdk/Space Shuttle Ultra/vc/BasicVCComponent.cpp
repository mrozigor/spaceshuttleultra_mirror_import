// BasicVCComponent.cpp: Implementierung der Klasse BasicVCComponent.
//
//////////////////////////////////////////////////////////////////////

#include "BasicVCComponent.h"
#include "../Atlantis.h"

namespace vc {

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

BasicVCComponent::BasicVCComponent(Atlantis* _sts)
: sts(_sts), pParent(NULL), bHasReference(false), bHasDirection(false),
	bHasAnimations(false), fInitialAnimState(0.0)
{
}

BasicVCComponent::BasicVCComponent(Atlantis* _sts, const string& _ident)
: sts(_sts), ident(_ident), pParent(NULL), bHasReference(false), 
	bHasDirection(false), bHasAnimations(false), fInitialAnimState(0.0)
{

}

BasicVCComponent::~BasicVCComponent()
{

}

void BasicVCComponent::AddAIDToRedrawEventList(UINT aid)
{
	availableForRendering.insert(aid);
}

bool BasicVCComponent::GetStateString(unsigned long ulBufferSize, char* pszBuffer) {
	return false;
}

void BasicVCComponent::Realize()
{
}

Atlantis* BasicVCComponent::STS() const
{
	return sts;
}

void BasicVCComponent::SetParentPanel(BasicPanel* pPanel) {
	pParent = pPanel;
}

bool BasicVCComponent::SetMouseRegion(float xmin, float ymin, float xmax, float ymax)
{
	p_a.x = xmin;
	p_a.y = ymin;
	p_d.x = xmax;
	p_d.y = ymax;
	return true;
}

bool BasicVCComponent::IsMultiLineSaveState() const {
	return false;
};

bool BasicVCComponent::IsOwnRegion(UINT aid) const
{
	if(availableForRendering.find(aid) != availableForRendering.end())
		return true;
	else
		return false;
}

bool BasicVCComponent::IsPointOver(float x, float y) const
{
	if(p_a.x <= x && p_d.x >= x && 
		p_a.y <= y && p_d.y >= y) {
		return true;
	}
	else 
		return false;
}

bool BasicVCComponent::ProjectMouse(float x, float y, float &mx, float &my) const
{
	if(IsPointOver(x, y))
	{
		mx = (x - p_a.x) /(p_d.x - p_a.x);
		my = (y - p_a.y) /(p_d.y - p_a.y);
		return true;
	}
	return false;
}

const VECTOR3& BasicVCComponent::GetReference() const
{
	return reference;
}

const VECTOR3& BasicVCComponent::GetDirection() const
{
	return dir;
}

bool BasicVCComponent::RegisterComponent(SubsystemComponent* pComponent)
{
	return false;
}

bool BasicVCComponent::DelComponent(SubsystemComponent* pComponent)
{
	return false;
}



double BasicVCComponent::GetState() const
{
	return 0.0;
}

bool BasicVCComponent::OnMouseEvent(int event, float x, float y)
{
	return false;
}

void BasicVCComponent::OnPreStep(double fSimT, double fDeltaT, double fMJD)
{

}

void BasicVCComponent::OnPostStep(double fSimT, double fDeltaT, double fMJD)
{

}

void BasicVCComponent::OnPropagate(double fSimT, double fDeltaT, double fMJD)
{
	//Do nothing - switches propagate by user events.
}

void BasicVCComponent::DefineVCAnimations(UINT vc_idx)
{
	oapiWriteLog("BasicVCComponent:\tDefine VC Animations");
}

const string& BasicVCComponent::GetIdentifier() const
{
	return ident;
}

const string& BasicVCComponent::GetQualifiedIdentifier() const
{
	static string result;
	if(pParent != NULL)
	{
		result = pParent->GetQualifiedIdentifier() + "::" + ident;
		return result;
	} 
	else 
	{
		return ident;
	}
}

bool BasicVCComponent::OnFail(const string& strFailureCode)
{
	return false;
}

void BasicVCComponent::OnDumpToLog() const
{
}

bool BasicVCComponent::OnParseLine(const char* line)
{
	return true;
}

bool BasicVCComponent::OnParseLine(const char* keyword, const char* line) {
	return true;
}

void BasicVCComponent::OnPlaybackEvent(double fSimT, double fEventT, const char* event_t, const char* event)
{
}

bool BasicVCComponent::OnReadState(FILEHANDLE scn) {
	char* line;
	char pszKey[256];
	while(oapiReadScenario_nextline(scn, line)) {
		if(!_strnicmp(line, "@ENDOBJECT", 10)) {
			return true;
		} else if (!_strnicmp(line, "@ENDPANEL", 9)) {
			return false;
		} else {
			unsigned long i = 0;
			while(*line != ' ' && *line != '\0') {
				pszKey[i++] = *line;
				line++;
			}
			pszKey[++i] = '\0';
			if(*line != '\0') {
				OnParseLine(pszKey, line);
			} else {
				OnParseLine(pszKey, NULL);
			}
		}
	}
	return false;
}

void BasicVCComponent::OnSaveState(FILEHANDLE scn) const
{
	
}


bool BasicVCComponent::OnVCRedrawEvent(int id, int _event, SURFHANDLE surf)
{
	return false;
}

unsigned long BasicVCComponent::CountComponents() const
{
	return 0;
}

void BasicVCComponent::RegisterVC()
{
}

bool BasicVCComponent::SetAnimation(UINT anim, double fState)
{
	if(bHasAnimations)
	{
		sprintf_s(oapiDebugString(), 255, "SET ANIMATION");
		STS()->SetAnimation(anim, fState);
		return true;
	}
	return false;
}

void BasicVCComponent::SetInitialAnimState(double fState)
{
	fInitialAnimState = fState;
}

void BasicVCComponent::SetReference(const VECTOR3& ref) 
{
	bHasReference = true;
	reference = ref;
}

void BasicVCComponent::SetDirection(const VECTOR3& _dir) 
{
	bHasDirection = true;
	dir = _dir;
}

void BasicVCComponent::SetReference(const VECTOR3& ref, const VECTOR3& dir) 
{
	bHasReference = true;
	bHasDirection = true;
	reference = ref;
	this->dir = dir;
}

bool BasicVCComponent::TriggerRedraw() {

	set<UINT>::iterator iter = availableForRendering.begin();
	while(iter != availableForRendering.end()) {
		oapiVCTriggerRedrawArea( -1, *iter);
		iter++;
	}
	return true;
}

bool BasicVCComponent::VerifyAnimations()
{
	bHasAnimations = true;
	return true;
}

bool BasicVCComponent::WriteScenarioInt(FILEHANDLE scn, const std::string &key, const int iValue) const
{
	
	return false;
}

bool BasicVCComponent::WriteScenarioString(FILEHANDLE scn, const std::string &key, const std::string &value) const
{
	return false;
}

bool BasicVCComponent::WriteScenarioVector(FILEHANDLE scn, const std::string &key, const VECTOR3 &vector) const
{
	return false;
}

};
