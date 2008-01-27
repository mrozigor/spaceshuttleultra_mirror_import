#ifndef __PANELF7_H
#define __PANELF7_H

#include <windows.h>
#include "Atlantis.h"

// ==============================================================
// class Panel F7
// switches and indicators on Panel F7
// ==============================================================

class PanelF7 {
	friend class Atlantis;

public:
	PanelF7 (Atlantis *_sts);
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

	short sTimerSeconds;
	short sTimerMinutes;
};

#endif  // !__PANELC3_H