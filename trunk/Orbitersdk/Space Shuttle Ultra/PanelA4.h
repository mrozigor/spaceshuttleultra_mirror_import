#ifndef __PANELA4_H
#define __PANELA4_H

#include <windows.h>
#include "Atlantis.h"

// ==============================================================
// class Panel A4
// switches and indicators on Panel A4
// ==============================================================

class PanelA4 {
	friend class Atlantis;

public:
	PanelA4 (Atlantis *_sts);
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

	UINT switch_state;
	UINT old_switch_state;
	double switch_timer;

	short sTimerSeconds;
	short sTimerMinutes;

	short sMETSeconds;
	short sMETMinutes;
	short sMETHours;
	short sMETDays;

	UINT anim_VC_A4;
};

#endif  // !__PANELC3_H