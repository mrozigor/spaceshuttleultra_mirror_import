/****************************************************************************
  This file is part of Space Shuttle Ultra

  Basic subsystem definitions



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

#if !defined(AFX_ATLANTISSUBSYSTEM_H__6A9F0F48_D391_4E11_9536_F359182CA558__INCLUDED_)
#define AFX_ATLANTISSUBSYSTEM_H__6A9F0F48_D391_4E11_9536_F359182CA558__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "OrbiterAPI.h"
#include "Subsystem.h"
#include "dps/dps_defs.h"
//#include "discsignals\DiscreteBundleManager.h"

using namespace std;
using namespace discsignals;
using dps::ShuttleBusManager;

class Atlantis;
class AtlantisSubsystemDirector;
/**
 * base class for all subsystem simulations. 
 */
class AtlantisSubsystem : public Subsystem<Atlantis>
{
public:
	/**
	 * Create a new Atlantis subsystem object
	 */
	AtlantisSubsystem(AtlantisSubsystemDirector* _director, const string& _ident);
	/**
	 * Destructor
	 */
	virtual ~AtlantisSubsystem();
	
	/**
	 * Get a reference to the Space Shuttle external physical model.
	 * @return reference to the space shuttle object. If possible.
	 */
	Atlantis* STS() const;
	
	/** 
	 * Return the currently used discrete bundle managing object. 
	 * @return pointer to the bundle manager
	 */
	DiscreteBundleManager* BundleManager() const;
	/** 
	 * Return the currently used data bus managing object. 
	 * @return pointer to the bus manager
	 */
	dps::ShuttleBusManager* BusManager() const;
};

class AtlantisEmptySubsystem: AtlantisSubsystem {
public:
	AtlantisEmptySubsystem(AtlantisSubsystemDirector* _director);
	virtual ~AtlantisEmptySubsystem();
};

#endif // !defined(AFX_ATLANTISSUBSYSTEM_H__6A9F0F48_D391_4E11_9536_F359182CA558__INCLUDED_)
