#include "ExtAirlock.h"

namespace eva_docking {

	ExtAirlock::ExtAirlock(AtlantisSubsystemDirector *pdirect, const std::string &_ident)
		: BasicExternalAirlock(pdirect, _ident) 
	{
		fHatchState = 0.0;
	}

	ExtAirlock::~ExtAirlock() {
	}

	void ExtAirlock::AddMeshes(const VECTOR3 &ofs) {

	}

	void ExtAirlock::DefineAirlockAnimations(UINT midx_extal, 
		UINT midx_ods, const VECTOR3& ofs) {
			//No animations
	}

};