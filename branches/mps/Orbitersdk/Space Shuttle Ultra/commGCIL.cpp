#include "commGCIL.h"

namespace comm {
	
	GCIL::GCIL(AtlantisSubsystemDirector *_direct)
		: AtlantisSubsystem(_direct, "GCIL")
	{
	}

	GCIL::~GCIL()
	{
	}

	void GCIL::OnPropagate(double fSimT, double fDeltaT, double fMJD)
	{
	}
	
};