#include "IOModule.h"

namespace dps {

	AODModule::AODModule(dps::MDM *parent)
		: IOModule(parent)
	{

	}

	AODModule::~AODModule()
	{
	}

	void AODModule::Reset()
	{
	}

	void AODModule::PowerOff()
	{
	}

	bool AODModule::WriteToChannel(unsigned int channel_no, dps::word16 data)
	{
		return false;
	}

	bool AODModule::ReadFromChannel(unsigned int channel_no, dps::word16 &data)
	{
		return false;
	}

	bool AODModule::BITE()
	{
		return false;
	}

	bool AODModule::ConnectDiscrete(unsigned int port, discsignals::DiscreteBundle *bundle, unsigned int line)
	{
		return false;
	}

	void AODModule::Propagate(double fSimT, double fDeltaT, double fMJD)
	{

	}
};
