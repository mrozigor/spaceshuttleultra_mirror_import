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
#include "..\Atlantis.h"


namespace eva_docking
{
	const static char* DEFAULT_MESHNAME_EXTAL = "SSU\\ExtAL";

	const VECTOR3 EXTERNAL_AIRLOCK_POS = _V( 0.0, -1.03, 0 );// Only X and Y axes used. Z pos is set in Mission.cpp (GetExternalAirlockZPos)
	//const VECTOR3 EXTERNAL_AIRLOCK_POS = _V( 0.0, -1.1, 7 );// Only X and Y axes used. Z pos is set in Mission.cpp (GetExternalAirlockZPos)
	
	class ExtAirlock: public BasicExternalAirlock {
	protected:
		double fHatchState;
		double fExtALPress[2];

		bool HideTopCover;

		UINT mesh_extal;
		MESHHANDLE hExtALMesh;
	public:
		ExtAirlock(AtlantisSubsystemDirector* pdirect, const string& _ident, bool HideTopCover = false);
		virtual ~ExtAirlock();
		virtual void AddMeshes(const VECTOR3& ofs);
		virtual void DefineAnimations(const VECTOR3& ofs);
		void VisualCreated( VISHANDLE vis );
	};
};