#ifndef __PANELO3_H
#define __PANELO3_H

#include <windows.h>
#include "Atlantis.h"

// ==============================================================
// class Panel O3
// switches and indicators on Panel O3
// ==============================================================

#define NUM_O3_SWITCHES		20
#define NUM_O3_ANIMATIONS	10

const VECTOR3 REF_O3S12 = _V(0.5121638,  2.799416,  14.35993);

// .3651996 2.796465  14.35028

const int SWITCH_O3S12 = 0;

class PanelO3 {
	friend class Atlantis;

public:
	PanelO3 (Atlantis *_sts);
	void DefineAnimations ();
	void DefineVCAnimations (UINT vcidx);
	void RegisterVC ();
	void UpdateVC ();
	void UpdateMesh();
	bool VCMouseEvent (int id, int event, VECTOR3 &p);
	bool VCRedrawEvent (int id, int event, SURFHANDLE surf);
	void Step (double t, double dt);
	bool ParseScenarioLine (char *line);
	void SaveState (FILEHANDLE scn);

private:
	Atlantis *sts; // vessel instance pointer

	UINT anim_VC_O3[NUM_O3_SWITCHES];
	UINT switch_state[NUM_O3_SWITCHES];
	UINT old_switch_state[NUM_O3_SWITCHES];
	double switch_timer[NUM_O3_SWITCHES];

	short sTimerSeconds;
	short sTimerMinutes;

	short sMETSeconds;
	short sMETMinutes;
	short sMETHours;
	short sMETDays;

	short sOMSFuel[3];
};

#endif  // !__PANELC3_H