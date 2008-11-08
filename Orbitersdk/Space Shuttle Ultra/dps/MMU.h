#pragma once

#include "../AtlantisSubsystem.h"

namespace dps {

	class MMU: public AtlantisSubsystem {
	public:
		MMU(SubsystemDirector* _direct, const string& _ident);
		virtual ~MMU();
		BIU channel;
		DiscInPort power_on;
	};
};