/****************************************************************************
  This file is part of Space Shuttle Ultra

  External Airlock subsystem definition



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
#pragma once
#include "BasicExtAirlock.h"

namespace eva_docking {
	
	class ExtAirlock: public BasicExternalAirlock {
	protected:
		double fHatchState;
		double fExtALPress[2];
	public:
		ExtAirlock(AtlantisSubsystemDirector* pdirect, const string& _ident = "External Airlock");
		virtual ~ExtAirlock();
		virtual void AddMeshes(const VECTOR3& ofs);
		virtual void DefineAirlockAnimations(UINT midx_extal, UINT midx_ods, const VECTOR3& ofs);
	};
};