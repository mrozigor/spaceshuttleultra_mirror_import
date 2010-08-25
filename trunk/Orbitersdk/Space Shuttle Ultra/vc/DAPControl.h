/****************************************************************************
  This file is part of Space Shuttle Ultra

  Orbital DAP control definition



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
#ifndef __DAP_H
#define __DAP_H
#pragma once

#include "../Atlantis.h"
#include "DiscOutPort.h"
#include "DiscInPort.h"

// ==============================================================
// DAPControl class
// Controls DAP PBIs
// ==============================================================

namespace vc {
	using class discsignals::DiscInPort;
	using class discsignals::DiscOutPort;

	class DAPControl {
	public:
		DAPControl(Atlantis *_sts);
		/**
		 * Handles mouse events from either set of PBIs.
		 * block==1 represents top row of PBIs; block==2 represents all other PBIs.
		 */
		bool VCMouseEvent(int block, int nEvent, const VECTOR3 &p);
		/**
		 * Returns true if PBI on
		 */
		bool GetPBIState(int id);
		/*
		 * Called after control mode is changed
		 * Adds extra initialization
		 */
		void InitializeControlMode();
		void OnPostStep(double SimT, double DeltaT, double MJD);

		void Realize();

	private:
		void ButtonPress(int id);
		//bool DrawPBILight(SURFHANDLE surf, bool &bState, bool bOn);

		Atlantis* sts;

		DiscInPort input[24];
		DiscOutPort output[24];
		bool oldValues[24];
	};
};

#endif //__DAP_H
