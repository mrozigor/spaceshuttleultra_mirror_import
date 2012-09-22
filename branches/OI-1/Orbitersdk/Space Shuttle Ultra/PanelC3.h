/****************************************************************************
  This file is part of Space Shuttle Ultra

  Old Panel C3 definition



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
#ifndef __PANELC3_H
#define __PANELC3_H

#include <windows.h>
#include "Atlantis.h"

// ==============================================================
// class Panel C3
// switches on Panel C3
// ==============================================================

//-0.1716415  1.728119  14.29085
const VECTOR3 REF_C3S1 = _V(-0.2114868, 1.728119, 14.29085);
const VECTOR3 REF_C3S2 = _V(-0.1716415, 1.728119, 14.29085);
//-0.1716415,  1.715764,  14.18536
const VECTOR3 REF_C3S19 = _V(-0.2114868,  1.715764,  14.18536);
const VECTOR3 REF_C3S20 = _V(-0.1716415,  1.715764,  14.18536);

const VECTOR3 REF_C3S8 = _V(-0.2114868, 1.680126, 13.8549);
const VECTOR3 REF_C3S9 = _V(-0.1716415, 1.680126, 13.8549);

class PanelC3 {
	friend class Atlantis;

public:
	PanelC3 (Atlantis *_sts);
	void DefineAnimations ();
	void DefineVCAnimations (UINT vcidx);
	void RegisterVC ();
	void UpdateVC ();
	void UpdateMesh();
	bool VCMouseEvent (int id, int nEvent, VECTOR3 &p);
	bool VCRedrawEvent (int id, int nEvent, SURFHANDLE surf);
	bool CheckProbesDeployed();
	bool CheckOMSArm(int nEng);
	void Step (double t, double dt);
	bool ParseScenarioLine (char *line);
	void SaveState (FILEHANDLE scn);

private:
	bool DrawPBILight(SURFHANDLE surf, int id, bool bOn);

	Atlantis *sts; // vessel instance pointer
	void EngControl(int eng);
	void AirDataProbeControl();
	// status of switches
	int Air_Data_Probe[2]; // 0=DEPLOY/HEAT, 1=DEPLOY, 2=STOW;
	int Air_Data_Stow[2]; // 0=ENABLE, 1=INHIBIT;
	int OMS_Eng[2]; // 0=ARM, 1=ARM/PRESS, 2=OFF;

	// VC switch animations
	UINT anim_VC_C3[37];
	//PBI lights
	bool PBI_Lights[24]; //true if light on

	AnimState AirDataProbe[2]; //0=Left, 1=Right
};

#endif  // !__PANELC3_H