#pragma once
#include "BasicExtAirlock.h"

namespace eva_docking {
	
	class ExtAirlock: public BasicExternalAirlock {
	protected:
		double fHatchState;
		double fExtALPress[2];
	public:
		ExtAirlock(SubsystemDirector* pdirect, const string& _ident = "External Airlock");
		virtual ~ExtAirlock();
		virtual void AddMeshes(const VECTOR3& ofs);
	};
};