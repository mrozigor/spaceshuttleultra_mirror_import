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
