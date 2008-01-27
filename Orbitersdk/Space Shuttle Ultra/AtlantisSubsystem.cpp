// AtlantisSubsystem.cpp: Implementierung der Klasse AtlantisSubsystem.
//
//////////////////////////////////////////////////////////////////////

#include "AtlantisSubsystem.h"

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

bool AtlantisSubsystem::ParseLine(const char* line)
{
	return false;
}

void AtlantisSubsystem::SaveState(FILEHANDLE scn)
{
}

void AtlantisSubsystem::PostStep(double fSimT, double fDeltaT)
{

}

void AtlantisSubsystem::PreStep(double fSimT, double fDeltaT)
{

}
