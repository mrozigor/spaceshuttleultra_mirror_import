#pragma once

#include <vector>
#include <string>
#include "DiscreteBundle.h"

namespace discsignals {

	using namespace std;

class DiscreteBundleManager
{
	vector <DiscreteBundle*> bundles;
public:
	DiscreteBundleManager(void);
	virtual ~DiscreteBundleManager(void);

	unsigned long CountBundles() const;
	bool AddBundle(DiscreteBundle* bBundle);
	bool RemoveBundle(DiscreteBundle* pBundle);
	bool ExistsBundle(const string& _ident) const;
	DiscreteBundle* FindBundle(const string& _ident) const;

	DiscreteBundle* GetBundleByIndex(unsigned long ulIndex) const;
	DiscreteBundle* CreateBundle(const string& _ident, unsigned short usLines);
	void DumpToLog() const;
};

};
