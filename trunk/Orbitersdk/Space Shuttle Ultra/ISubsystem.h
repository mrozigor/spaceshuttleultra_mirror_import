// ISubsystem.h: Schnittstelle für die Klasse ISubsystem.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISUBSYSTEM_H__A4896D4F_46E0_4D68_AEA4_F6B9CF259711__INCLUDED_)
#define AFX_ISUBSYSTEM_H__A4896D4F_46E0_4D68_AEA4_F6B9CF259711__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

using namespace std;

class SubsystemComponent;

class ISubsystem  
{
public:
	/**
	 * Override this function to create all Orbiter objects required: Thrusters, animations, meshes. 
	 */
	virtual void Realize() = 0;
	virtual bool RegisterComponent(SubsystemComponent* pComponent) = 0;
	virtual bool DelComponent(SubsystemComponent* pComponent) = 0;
	virtual unsigned long CountComponents() const = 0;
	virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD) = 0;
	virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD) = 0;
	virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD) = 0;
	virtual void OnPlaybackEvent(double fSimT, double fEventT, const char* event_t, const char* event) = 0;
	virtual bool OnParseLine(const char* line) = 0;
	virtual void OnSaveState(FILEHANDLE scn) const = 0;
	virtual void OnDumpToLog() const = 0;
	virtual const string& GetQualifiedIdentifier() const = 0;
	virtual const string& GetIdentifier() const = 0;
	virtual bool OnFail(const string& strFailureCode) = 0;
	

};

#endif // !defined(AFX_ISUBSYSTEM_H__A4896D4F_46E0_4D68_AEA4_F6B9CF259711__INCLUDED_)
