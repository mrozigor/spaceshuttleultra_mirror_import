/****************************************************************************
  This file is part of Space Shuttle Ultra

  Gear and payload bay operations definition



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
#ifndef __GEAROP_H
#define __GEAROP_H

#include <windows.h>
#include "Atlantis.h"

// ==============================================================
// class GearOp
// Manages payload bay operations (bay doors and radiators),
// both the physical state of the systems, and the state of
// the user interface (panel switches, etc.)
// ==============================================================

class GearOp {
	friend class Atlantis;

public:
	GearOp (Atlantis *_sts);
	void OpenDialog ();
	void DefineAnimations ();
	void DefineVCAnimations (UINT vcidx);
	void RegisterVC ();
	void UpdateVC ();
	void UpdateMesh();
	bool VCMouseEvent (int id, int event, VECTOR3 &p);
	bool VCRedrawEvent (int id, int event, SURFHANDLE surf);


	void Step (double t, double dt);
	inline AnimState::Action GetGearAction () const { return gear_status; }
	void OperateLandingGear (AnimState::Action action);
	void RevertLandingGear ();
	void ArmGear();
	void DamageGear();
	void SetGearParameters (double state);
	bool GearArmed();

	bool ParseScenarioLine (char *line);
	void SaveState (FILEHANDLE scn);

private:
	Atlantis *sts; // vessel instance pointer
	bool VCDrawTalkback (SURFHANDLE surf, int idx, int label);

	// status of control switches
	enum {SWC_CLOSE,SWC_OPEN} SwitchCover[2];
	enum {SW_NOT_PUSH,SW_PUSH} SwitchPush[2];

	// VC switch animations
	UINT anim_VC_F6[2];
	int tkbk_state[3];

	//Physical position of landing gear
    AnimState::Action gear_status;
	double gear_proc; // landing gear deployment state (0=retracted, 1=deployed)

	//Physical gear animations
	UINT anim_gear;                            // handle for landing gear animation

	bool GearDamaged;

};

#endif // !__GEAROP_H