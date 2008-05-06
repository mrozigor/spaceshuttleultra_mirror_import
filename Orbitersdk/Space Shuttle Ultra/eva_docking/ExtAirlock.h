#pragma once
#include "BasicExtAirlock.h"

namespace eva_docking {
	
	class ExtAirlock: public BasicExternalAirlock {
	public:
		ExtAirlock(SubsystemDirector* pdirect, const string& _ident = "External Airlock");
		virtual ~ExtAirlock();
	};
};