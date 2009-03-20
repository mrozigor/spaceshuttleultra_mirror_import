#include "DiscreteBundleManager.h"
#include <OrbiterAPI.h>

namespace discsignals {

DiscreteBundleManager::DiscreteBundleManager(void)
{
}

DiscreteBundleManager::~DiscreteBundleManager(void)
{
	for(unsigned int i=0;i<bundles.size();i++) {
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
	if(pBundle)
		bundles.push_back(pBundle);
	return true;
}

DiscreteBundle* DiscreteBundleManager::CreateBundle(const std::string &_ident, unsigned short usLines)
{
	char pszBuffer[400];
	sprintf_s(pszBuffer, 400, "(DiscreteBundleManager::CreateBundle) Enter \"%s\"", _ident.c_str());
	oapiWriteLog(pszBuffer);
	if(ExistsBundle(_ident))
	{
		oapiWriteLog("(DiscreteBundleManager::CreateBundle) Found Bundle");
		return FindBundle(_ident);
	}
	DiscreteBundle* pR = new DiscreteBundle(_ident, usLines);
	if(AddBundle(pR))
	{
		oapiWriteLog("(DiscreteBundleManager::CreateBundle) Added new bundle");
		return pR;
	}
	else {
		oapiWriteLog("(DiscreteBundleManager::CreateBundle) Failed to create new bundle");
		if(pR)
			delete pR;
		return NULL;
	}
}

bool DiscreteBundleManager::ExistsBundle(const std::string &_ident) const {
	char pszBuffer[400];
	sprintf_s(pszBuffer, 400, "(DiscreteBundleManager::ExistsBundle) Enter \"%s\"", _ident.c_str());
	oapiWriteLog(pszBuffer);
	for(unsigned int i = 0; i<bundles.size(); i++)
	{
		if(bundles.at(i) && bundles.at(i)->GetIdentity() == _ident)
		{
			oapiWriteLog("(DiscreteBundleManager::ExistsBundle) Found.");
			return true;
		}
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