#ifndef __PANELA8_H
#define __PANELA8_H

#include "Atlantis.h"
#include "discsignals/discsignals.h"

using namespace discsignals;

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
	bool VCRedrawEvent (int id, int _event, SURFHANDLE surf);
	void Step (double t, double dt);
	bool ParseScenarioLine (char *line);
	void SaveState (FILEHANDLE scn);

	void Realize();
private:
	Atlantis *sts; // vessel instance pointer

	//draws entire talkback with single bitmap
	bool VCDrawTalkback (SURFHANDLE surf, int idx, int label);
	//draws 2-segment talkback
	bool VCDrawTalkback (SURFHANDLE surf, int idx, int upper_label, int lower_label);

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

	DiscOutPort PortMPMDeploy, PortMPMStow, PortMRLRelease, PortMRLLatch;
	DiscInPort PortMPMDeployed, PortMPMStowed, PortMRL_Released, PortMRL_Latched, PortMRL_RTL[3];
	DiscOutPort StbdMPMDeploy, StbdMPMStow, StbdMRLRelease, StbdMRLLatch;
	DiscInPort StbdMPMDeployed, StbdMPMStowed, StbdMRL_Released, StbdMRL_Latched, StbdMRL_RTL[3];
};

#endif  // !__PANELA8_H
