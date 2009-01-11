#include "DiscreteBundleManager.h"
#include <OrbiterAPI.h>

namespace discsignals {

DiscreteBundleManager::DiscreteBundleManager(void)
{
}

DiscreteBundleManager::~DiscreteBundleManager(void)
{
	for(int i=0;i<bundles.size();i++) {
		DiscreteBundle* pBundle=bundles[i];
		delete pBundle;
	}
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
	//oapiWriteLog("(DiscreteBundleManager::CreateBundle) Enter");
	if(ExistsBundle(_ident))
	{
		//oapiWriteLog("(DiscreteBundleManager::CreateBundle) Found Bundle");
		return FindBundle(_ident);
	}
	DiscreteBundle* pR = new DiscreteBundle(_ident, usLines);
	if(AddBundle(pR))
	{
		//oapiWriteLog("(DiscreteBundleManager::CreateBundle) Added new bundle");
		return pR;
	}
	else {
		//oapiWriteLog("(DiscreteBundleManager::CreateBundle) Failed to create new bundle");
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