/****************************************************************************
  This file is part of Space Shuttle Ultra

  (old) Panel C2 definition



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
#ifndef __PANELC2_H
#define __PANELC2_H

#include "Atlantis.h"

// ==============================================================
// class Panel C2
// switches on Panel C2
// ==============================================================

const VECTOR3 REF_C2S1 = _V(-0.10, 1.911417, 14.50198);
const VECTOR3 REF_C2S2 = _V(-0.07, 1.911417, 14.50198);
const VECTOR3 REF_C2S3 = _V(-0.02, 1.911417, 14.50198);
const VECTOR3 REF_C2S4 = _V( 0.01, 1.911417, 14.50198);
const VECTOR3 REF_C2S5 = _V( 0.07, 1.911417, 14.50198);
const VECTOR3 REF_C2S6 = _V(-0.10, 1.911417, 14.50198);
//0.131577,  1.859752,  14.45152
const VECTOR3 REF_C2S7 = _V( -0.100152, 1.859752, 14.45152);
const VECTOR3 REF_C2S8 = _V( 0.100152, 1.859752, 14.45152);

const VECTOR3 REF_C2S9 = _V(0.0, 1.801141, 14.3961);
const VECTOR3 REF_C2S10 = _V(-0.07, 1.801141, 14.3961);
const VECTOR3 REF_C2S11 = _V(0.131577, 1.80661, 14.42035);
const VECTOR3 REF_C2S12 = _V(0.0994235, 1.801141, 14.3961);
 

class PanelC2 {
	friend class Atlantis;
	friend class CRT;

public:
	PanelC2 (Atlantis *_sts);
	~PanelC2();
	void DefineAnimations ();
	void DefineVCAnimations (UINT vcidx);
	void RegisterVC ();
	void UpdateVC ();
	void CreateVisual(VISHANDLE hVisual);
	void UpdateMesh();
	bool VCMouseEvent (int id, int event, VECTOR3 &p);
	bool VCRedrawEvent (int id, int event, SURFHANDLE surf);
	void Step (double t, double dt);
	bool ParseScenarioLine (char *line);
	void SaveState (FILEHANDLE scn);

private:
	Atlantis *sts; // vessel instance pointer

	// VC switch animations
	UINT anim_VC_C2[12];
	UINT anim_VC_C2Wheel[4];
	SURFHANDLE wheel_tex[4];
	SURFHANDLE digit_texture;

	double tgtwheel_state[4];
	short wheelnumber[4];
	short oldwheelnumber[4];

	UINT switch_state[12];
	UINT old_switch_state[12];
	double switch_timer[12];
	

	double wheelState[4];
	UINT EventTimerWheelRevs[4];
	UINT EventTimerMode;    //Only switch position, not real mode
	UINT EventTimerControl; //Only switch position, not real mode

};

#endif  // !__PANELC2_H
