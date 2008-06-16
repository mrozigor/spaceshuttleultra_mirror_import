#include "IDP.h"

namespace dps {

	IDP::IDP(SubsystemDirector* _direct, const std::string& _ident, 
		unsigned short _usIDPID)
		: AtlantisSubsystem(_direct, _ident), usIDPID(_usIDPID)
	{
	}

	IDP::~IDP()
	{
	}

	unsigned short IDP::GetIDPID() const {
		return usIDPID;
	}
};