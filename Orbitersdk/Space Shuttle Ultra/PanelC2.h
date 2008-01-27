#ifndef __PANELC2_H
#define __PANELC2_H

#include <windows.h>
#include "Atlantis.h"

// ==============================================================
// class Panel C2
// switches on Panel C2
// ==============================================================


class PanelC2 {
	friend class Atlantis;

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

#endif  // !__PANELC3_H