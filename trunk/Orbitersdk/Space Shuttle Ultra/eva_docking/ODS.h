#pragma once
#include "ExtAirlock.h"

const static char* ODS_MESHNAME = "ODS";

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
		double fVestPressure[2];
	public:
		ODS(SubsystemDirector* pdirect, const string& _ident);
		virtual ~ODS();
		virtual void AddMeshes(const VECTOR3& ofs);
		virtual double GetSubsystemEmptyMass() const;
	};

};

