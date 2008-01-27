#ifndef __PANELC3_H
#define __PANELC3_H

#include <windows.h>
#include "Atlantis.h"

// ==============================================================
// class Panel C3
// switches on Panel C3
// ==============================================================

class PanelC3 {
	friend class Atlantis;

public:
	PanelC3 (Atlantis *_sts);
	void DefineAnimations ();
	void DefineVCAnimations (UINT vcidx);
	void RegisterVC ();
	void UpdateVC ();
	void UpdateMesh();
	bool VCMouseEvent (int id, int event, VECTOR3 &p);
	bool VCRedrawEvent (int id, int event, SURFHANDLE surf);
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

	AnimState AirDataProbe[2]; //0=Left, 1=Right
};

#endif  // !__PANELC3_H