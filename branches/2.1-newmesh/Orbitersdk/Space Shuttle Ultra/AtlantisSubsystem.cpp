// AtlantisSubsystem.cpp: Implementierung der Klasse AtlantisSubsystem.
//
//////////////////////////////////////////////////////////////////////

#include "AtlantisSubsystem.h"
#include "AtlantisSubsystemDirector.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

AtlantisSubsystem::AtlantisSubsystem(AtlantisSubsystemDirector* _director, const string& _ident)
: Subsystem(_director, _ident)
{
	char pszBuffer[300];
	oapiWriteLog("(AtlantisSubsystem::AtlantisSubsystem)");
	sprintf_s(pszBuffer, 300, "(AtlantisSubsystem) Create subsystem %s.", ident.c_str());
	oapiWriteLog(pszBuffer);
}

AtlantisSubsystem::~AtlantisSubsystem()
{

}

Atlantis* AtlantisSubsystem::STS() const
{
	return director->V();
}


DiscreteBundleManager* AtlantisSubsystem::BundleManager() const
{
	return static_cast<AtlantisSubsystemDirector*>(director)->BundleManager();
}

ShuttleBusManager* AtlantisSubsystem::BusManager() const {
	return static_cast<AtlantisSubsystemDirector*>(director)->BusManager();
}