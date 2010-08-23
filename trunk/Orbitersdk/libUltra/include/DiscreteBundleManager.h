/****************************************************************************
  This file is part of Space Shuttle Ultra

  Discrete line bundle manager definition



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/
#ifndef __DISCRETEBUNDLEMANAGER_H_6C8AD5C7_ABE2_4842_AE3B_072F3951A4C9
#define __DISCRETEBUNDLEMANAGER_H_6C8AD5C7_ABE2_4842_AE3B_072F3951A4C9
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

#endif
