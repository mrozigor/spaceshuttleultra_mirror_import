// AtlantisSubsystem.h: Schnittstelle für die Klasse AtlantisSubsystem.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATLANTISSUBSYSTEM_H__6A9F0F48_D391_4E11_9536_F359182CA558__INCLUDED_)
#define AFX_ATLANTISSUBSYSTEM_H__6A9F0F48_D391_4E11_9536_F359182CA558__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "OrbiterAPI.h"

using namespace std;


class SubsystemDirector;
/**
 * base class for all subsystem simulations. 
 */
class AtlantisSubsystem  
{
	SubsystemDirector* director;
	string ident;
public:
	AtlantisSubsystem(SubsystemDirector* _director, const string& _ident);
	virtual ~AtlantisSubsystem();
	virtual bool ParseLine(const char* line);
	virtual void SaveState(FILEHANDLE scn);
	virtual void PreStep(double fSimT, double fDeltaT);
	virtual void PostStep(double fSimT, double fDeltaT);
	
};

#endif // !defined(AFX_ATLANTISSUBSYSTEM_H__6A9F0F48_D391_4E11_9536_F359182CA558__INCLUDED_)
