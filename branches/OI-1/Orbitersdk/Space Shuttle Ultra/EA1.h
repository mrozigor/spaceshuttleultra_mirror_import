#pragma once
#include "AtlantisSubsystem.h"

namespace comm 
{
	class EA1: public AtlantisSubsystem
	{

	public:
		EA1(AtlantisSubsystemDirector* _director);
		virtual ~EA1();
	};
	
};