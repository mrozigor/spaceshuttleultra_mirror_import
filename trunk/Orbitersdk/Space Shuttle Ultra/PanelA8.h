#ifndef __PANELA8_H
#define __PANELA8_H

#include <windows.h>
#include "Atlantis.h"

// ==============================================================
// class Panel A8
// switches on Panel A8
// ==============================================================


class PanelA8
{
	friend class Atlantis;
	friend class CRT;

public:
	PanelA8 (Atlantis *_sts);
	//~PanelA8 ();
	//void DefineAnimations ();
	void DefineVCAnimations (UINT vcidx);
	void RegisterVC ();
	void UpdateVC ();
	//void CreateVisual(VISHANDLE hVisual);
	void UpdateMesh();
	bool VCMouseEvent (int id, int nEvent, VECTOR3 &p);
	bool VCRedrawEvent (int id, int event, SURFHANDLE surf);
	void Step (double t, double dt);
	bool ParseScenarioLine (char *line);
	void SaveState (FILEHANDLE scn);

private:
	Atlantis *sts; // vessel instance pointer

	//draws entire talkback with single bitmap
	bool VCDrawTalkback (SURFHANDLE surf, int idx, int label);
	//draws upper portion of talkback
	bool VCDrawUpperTalkback (SURFHANDLE surf, int idx, int label);
	//draws lower portion of talkback
	bool VCDrawLowerTalkback (SURFHANDLE surf, int idx, int label);

	// VC switch animations
	UINT anim_VC_A8[20];
	UINT anim_VC_A8_cover[7]; //switch cover animations
	int switch_state[20];
	int cover_state[7];
	/**
	 * current bitmap displayed by talkback
	 * [0][idx] used for upper half, or whole talkback where applicable
	 * [1][idx] used for lower half
	 */
	int tkbk_state[2][20];
};

#endif  // !__PANELA8_H
