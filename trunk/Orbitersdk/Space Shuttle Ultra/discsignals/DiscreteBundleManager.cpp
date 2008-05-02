#include "DiscreteBundleManager.h"

namespace discsignals {

DiscreteBundleManager::DiscreteBundleManager(void)
{
}

DiscreteBundleManager::~DiscreteBundleManager(void)
{
}

void DiscreteBundleManager::DumpToLog() const
{
	//Dump all registered Bundles to the orbiter log
}

bool DiscreteBundleManager::AddBundle(discsignals::DiscreteBundle *pBundle)
{
	bundles.push_back(pBundle);
	return true;
}

DiscreteBundle* DiscreteBundleManager::CreateBundle(const std::string &_ident, unsigned short usLines)
{
	DiscreteBundle* pR = new DiscreteBundle(_ident, usLines);
	if(AddBundle(pR))
	{
		return pR;
	}
	else {
		delete pR;
		return NULL;
	}
}

};