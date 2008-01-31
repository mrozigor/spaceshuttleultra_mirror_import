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

	UINT anim_VC_O3[NUM_O3_ANIMATIONS];
};

#endif  // !__PANELC3_H