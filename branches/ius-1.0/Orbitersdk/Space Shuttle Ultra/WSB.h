/****************************************************************************
  This file is part of Space Shuttle Ultra

  Water Spray Boiler simulation definition



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
#ifndef __WSB_H_BDF34C5B_092D_4dfd_B7B8_184C46C919E9
#define __WSB_H_BDF34C5B_092D_4dfd_B7B8_184C46C919E9
#pragma once

#include "AtlantisSubsystem.h"
#include "DiscInPort.h"
#include "DiscOutPort.h"

/**
 * Simulates Water Spray Boiler used to cool APUs and hyd fluid
 * At the moment, all it does is check WSB ready for APU talkbacks
 * TODO: accurately calculate boiloff rate
 * TODO: propulsive force when venting steam
 */
class WSB : public AtlantisSubsystem
{
	unsigned short usID; // from 1 to 3

	double waterMass[2];

	// indicates controller selected and if controller is powered
	DiscInPort ControllerPwr, ControllerA, ControllerB;
	DiscInPort BoilerN2Supply;
	DiscOutPort Ready;
public:
	WSB(AtlantisSubsystemDirector* _director, const string& _ident, unsigned short _ID);
	virtual ~WSB();

	virtual void Realize();

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	virtual void OnPostStep(double SimT, double DeltaT, double MJD);
	virtual void OnPropagate(double SimT, double DeltaT, double MJD);

	virtual void OnSaveState(FILEHANDLE scn) const;
	virtual bool OnParseLine(const char* keyword, const char* line);
};

#endif //__WSB_H_BDF34C5B_092D_4dfd_B7B8_184C46C919E9