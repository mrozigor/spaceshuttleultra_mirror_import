/****************************************************************************
  This file is part of Space Shuttle Ultra

  Subsystem interface



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/
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
