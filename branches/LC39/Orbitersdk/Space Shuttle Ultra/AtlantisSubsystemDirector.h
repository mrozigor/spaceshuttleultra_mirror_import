/****************************************************************************
  This file is part of Space Shuttle Ultra

  Subsystem Director



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
#ifndef ___ATLANTIS_ULTRA_SUBSYSTEM_DIRECTOR_H___
#define ___ATLANTIS_ULTRA_SUBSYSTEM_DIRECTOR_H___

#include "SubsystemDirector.h"
#include "dps/dps_defs.h"

class Atlantis;
class AtlantisSubsystem;

//using class discsignals::DiscreteBundleManager;
//using class dps::ShuttleBusManager;

class AtlantisSubsystemDirector : public SubsystemDirector<Atlantis>
{
public:
	AtlantisSubsystemDirector(Atlantis* _sts);
	~AtlantisSubsystemDirector();

	virtual DiscreteBundleManager* BundleManager() const;
	virtual dps::ShuttleBusManager* BusManager() const;
};

#endif