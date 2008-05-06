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
#include "ISubsystem.h"

using namespace std;


class SubsystemDirector;
/**
 * base class for all subsystem simulations. 
 */
class AtlantisSubsystem: public ISubsystem 
{
protected:
	SubsystemDirector* director;
	string ident;
public:
	AtlantisSubsystem(SubsystemDirector* _director, const string& _ident);
	virtual ~AtlantisSubsystem();
	virtual void AddMeshes(const VECTOR3& ofs);
	virtual void Realize();
	virtual bool RegisterComponent(SubsystemComponent* pComponent);
	virtual bool DelComponent(SubsystemComponent* pComponent);
	virtual unsigned long CountComponents() const;
	virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
	virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
	virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
	virtual void OnPlaybackEvent(double fSimT, double fEventT, const char* event_t, const char* event);
	virtual bool OnParseLine(const char* line);
	virtual void OnSaveState(FILEHANDLE scn) const;
	virtual void OnDumpToLog() const;
	virtual const string& GetQualifiedIdentifier() const;
	virtual const string& GetIdentifier() const;
	virtual bool OnFail(const string& strFailureCode);
	virtual double GetSubsystemEmptyMass() const {return 0.0;};
};

#endif // !defined(AFX_ATLANTISSUBSYSTEM_H__6A9F0F48_D391_4E11_9536_F359182CA558__INCLUDED_)
