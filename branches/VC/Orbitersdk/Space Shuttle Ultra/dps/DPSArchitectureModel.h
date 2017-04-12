#pragma once

#include "../AtlantisSubsystem.h"
#include "dps_defs.h"

namespace dps
{

	/**
	 * Interface for defining the way how SSU implements the DPS.
	 * Right now, there are two models on the roadmap:
	 * - legacy		The simple single GPC model initially implemented
	 * - simple		An improved partition-based model that supports DPS overlay operations
	 *
	 * Responsibilities of this class:
	 * - Create assisting classes
	 * - Load and save DPS state, especially software
	 * - Connect to VC ressources and other subsystems
	 * 
	 */
	class DPSArchitectureModel : AtlantisSubsystem {
	public:
		DPSArchitectureModel(Atlantis *sts);
		virtual ~DPSArchitectureModel();
		/** 
		 * Create additional subsystems necessary for the DPS, if not available in any implementation
		 */
		virtual void Implement() = 0;
		virtual void Realize() = 0;
	};

};