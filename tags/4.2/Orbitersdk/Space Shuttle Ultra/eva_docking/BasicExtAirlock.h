/****************************************************************************
  This file is part of Space Shuttle Ultra

  Basic external airlock definition



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
#pragma once
#include "../AtlantisSubsystem.h"

namespace eva_docking {

	class BasicExternalAirlock: public AtlantisSubsystem {
	public:
		BasicExternalAirlock(AtlantisSubsystemDirector* pdirect, const string& ident);
		virtual ~BasicExternalAirlock();
		virtual void AddMeshes(const VECTOR3& ofs);
		virtual void Realize();
		virtual double GetSubsystemEmptyMass() const;
		virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPlaybackEvent(double fSimT, double fEventT, const char* event_t, const char* event);
		virtual bool OnParseLine(const char* line);
		virtual void OnSaveState(FILEHANDLE scn) const;
		virtual void OnDumpToLog() const;
		virtual void DefineAnimations(const VECTOR3& ofs) = 0;
	};

	class NoExtAirlock: public BasicExternalAirlock {
	public:
		NoExtAirlock(AtlantisSubsystemDirector* pdirect);
		virtual ~NoExtAirlock();
		virtual void DefineAnimations(const VECTOR3& ofs);
	};

};