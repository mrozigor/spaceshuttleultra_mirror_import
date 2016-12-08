#include "AtlantisSubsystemDirector.h"
#include "Atlantis.h"

AtlantisSubsystemDirector::AtlantisSubsystemDirector(Atlantis *_sts)
: SubsystemDirector(_sts)
{
}

AtlantisSubsystemDirector::~AtlantisSubsystemDirector()
{
}

DiscreteBundleManager* AtlantisSubsystemDirector::BundleManager() const
{
	return V()->BundleManager();
}
ShuttleBusManager* AtlantisSubsystemDirector::BusManager() const {
	return V()->BusManager();
}
