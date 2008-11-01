#pragma once
#include "../AtlantisSubsystem.h"

namespace eva_docking {

	class BasicExternalAirlock: public AtlantisSubsystem {
	public:
		BasicExternalAirlock(SubsystemDirector* pdirect, const string& ident);
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
	};

	class NoExtAirlock: public BasicExternalAirlock {
	public:
		NoExtAirlock(SubsystemDirector* pdirect);
		virtual ~NoExtAirlock();
	};

};