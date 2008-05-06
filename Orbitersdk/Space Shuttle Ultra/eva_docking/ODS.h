#pragma once
#include "ExtAirlock.h"

/**
 * EVA and Docking systems
 */
namespace eva_docking {

	class ODS: public ExtAirlock {
	protected:
		UINT midxODS;
		double fRingState;
		double fHookState;
		double fLatchState;
	public:
		ODS(SubsystemDirector* pdirect, const string& _ident);
		virtual ~ODS();
	};

};

