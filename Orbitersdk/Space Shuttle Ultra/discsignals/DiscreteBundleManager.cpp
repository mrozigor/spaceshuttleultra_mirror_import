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
	if(ExistsBundle(_ident))
	{
		return FindBundle(_ident);
	}
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

bool DiscreteBundleManager::ExistsBundle(const std::string &_ident) const {
	for(unsigned int i = 0; i<bundles.size(); i++)
	{
		if(bundles.at(i)->GetIdentity() == _ident)
			return true;
	}
	return false;
};

DiscreteBundle* DiscreteBundleManager::FindBundle(const std::string &_ident) const {
	for(unsigned int i = 0; i<bundles.size(); i++)
	{
		if(bundles.at(i)->GetIdentity() == _ident)
			return bundles.at(i);
	}
	return NULL;
};



};