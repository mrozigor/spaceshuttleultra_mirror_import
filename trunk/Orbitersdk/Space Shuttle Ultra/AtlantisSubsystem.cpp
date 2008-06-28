// AtlantisSubsystem.cpp: Implementierung der Klasse AtlantisSubsystem.
//
//////////////////////////////////////////////////////////////////////

#include "AtlantisSubsystem.h"
#include "SubsystemDirector.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

AtlantisSubsystem::AtlantisSubsystem(SubsystemDirector* _director, const string& _ident)
: director(_director), ident(_ident)
{

}

AtlantisSubsystem::~AtlantisSubsystem()
{

}

void AtlantisSubsystem::AddMeshes(const VECTOR3& ofs)
{
}

bool AtlantisSubsystem::RegisterComponent(SubsystemComponent* pComponent)
{
	return false;
}

bool AtlantisSubsystem::DelComponent(SubsystemComponent* pComponent)
{
	return false;
}

unsigned long AtlantisSubsystem::CountComponents() const
{
	return 0;
}

void AtlantisSubsystem::Realize()
{
}


bool AtlantisSubsystem::OnParseLine(const char* line)
{
	return false;
}

void AtlantisSubsystem::OnSaveState(FILEHANDLE scn) const
{

}

void AtlantisSubsystem::OnPostStep(double fSimT, double fDeltaT, double fMJD)
{

}

void AtlantisSubsystem::OnPreStep(double fSimT, double fDeltaT, double fMJD)
{

}

void AtlantisSubsystem::OnPropagate(double fSimT, double fDeltaT, double fMJD)
{
}

void AtlantisSubsystem::OnPlaybackEvent(double fSimT, double fEventT, const char* event_t, const char* event)
{
}

bool AtlantisSubsystem::OnFail(const string& strFailCode)
{
	return false;
}

void AtlantisSubsystem::OnDumpToLog() const
{
}

const string& AtlantisSubsystem::GetIdentifier() const
{
	return this->ident;
}


const string& AtlantisSubsystem::GetQualifiedIdentifier() const
{
	return GetIdentifier();
}

Atlantis* AtlantisSubsystem::STS()
{
	return director->STS();
}