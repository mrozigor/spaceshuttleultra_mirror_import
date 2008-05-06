// BasicVCComponent.cpp: Implementierung der Klasse BasicVCComponent.
//
//////////////////////////////////////////////////////////////////////

#include "BasicVCComponent.h"

namespace vc {

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

BasicVCComponent::BasicVCComponent(Atlantis* _sts)
: sts(_sts)
{

}

BasicVCComponent::BasicVCComponent(Atlantis* _sts, const string& _ident)
: sts(_sts), ident(_ident)
{

}

BasicVCComponent::~BasicVCComponent()
{

}

const string& BasicVCComponent::Ident() const
{
	return ident;
}

void BasicVCComponent::Realize()
{
}

Atlantis* BasicVCComponent::STS() const
{
	return sts;
}

bool BasicVCComponent::SetMouseRegion(float xmin, float ymin, float xmax, float ymax)
{
	p_a.x = xmin;
	p_a.y = ymin;
	p_d.x = xmax;
	p_d.y = ymax;
	return true;
}

bool BasicVCComponent::IsPointOver(float x, float y) const
{
	if(p_a.x <= x && p_d.x >= x && p_a.y <= y && p_d.y >= y) {
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

void BasicVCComponent::DefineAnimations(UINT vc_idx)
{

}

const string& BasicVCComponent::GetIdentifier() const
{
	return "";
}

const string& BasicVCComponent::GetQualifiedIdentifier() const
{
	return "";
}

bool BasicVCComponent::OnFail(const string& strFailureCode)
{
	return false;
}

void BasicVCComponent::OnDumpToLog() const
{
}

void BasicVCComponent::OnSaveState(FILEHANDLE scn) const
{
	
}

bool BasicVCComponent::OnParseLine(const char* line)
{
	return true;
}

void BasicVCComponent::OnPlaybackEvent(double fSimT, double fEventT, const char* event_t, const char* event)
{
}

unsigned long BasicVCComponent::CountComponents() const
{
	return 0;
}

};