#include "ExtAirlock.h"

namespace eva_docking {

	ExtAirlock::ExtAirlock(SubsystemDirector *pdirect, const std::string &_ident)
		: BasicExternalAirlock(pdirect, _ident) 
	{
		fHatchState = 0.0;
	}

	ExtAirlock::~ExtAirlock() {
	}

	void ExtAirlock::AddMeshes(const VECTOR3 &ofs) {

	}

};