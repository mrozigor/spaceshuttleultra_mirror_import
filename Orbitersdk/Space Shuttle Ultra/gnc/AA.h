#pragma once
#include "../AtlantisSubsystem.h"
#include "../discsignals/DiscOutPort.h"

namespace gnc {

	using class discsignals::DiscOutPort;

	class AccAssembly: AtlantisSubsystem
	{
		VECTOR3 ref;
		VECTOR3 vY; //lateral acceleration
		VECTOR3 vZ; //normal acceleration
		DiscOutPort outY;
		DiscOutPort outZ;
	public:
		/**
		 * @param direct Reference to the responsible SubsystemDirector
		 * @param ident Identification of the AA
		 * @param ref Reference position of the assembly in local 
		 * vessel coordinates
		 */
		AccAssembly(SubsystemDirector* direct, const string& ident, 
			const VECTOR3& ref);
		virtual ~AccAssembly();

	};

};