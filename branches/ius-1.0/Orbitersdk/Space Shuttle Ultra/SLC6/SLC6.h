#ifndef SLC6_H_640EA460_C8C8_46B1_8604_B46C72BD8984
#define SLC6_H_640EA460_C8C8_46B1_8604_B46C72BD8984
#pragma once
#include <vector>
#include "orbitersdk.h"
#include "ISSUMLP.h"
#include "ISSULaunchTower.h"

static const char* DEFAULT_MESHNAME_PAD = "VandenbergAFB/SLC-6_STS_LaunchMount";
static const char* DEFAULT_MESHNAME_TOWER = "VandenbergAFB/SLC-6_STS_AT";
static const char* DEFAULT_MESHNAME_PCR = "VandenbergAFB/SLC-6_STS_PCR";
static const char* DEFAULT_MESHNAME_SAB = "VandenbergAFB/SLC-6_STS_MAS";
static const char* DEFAULT_MESHNAME_MST = "VandenbergAFB/SLC-6_STS_MST";

const double VERT_MESH_OFFSET = -2.0;
const VECTOR3 PAD_MESH_OFFSET = _V(0, VERT_MESH_OFFSET, 0);
const VECTOR3 TOWER_MESH_OFFSET = _V(22.54, VERT_MESH_OFFSET, 15.84);
const VECTOR3 PCR_MESH_OFFSET = _V(0, -1.8, 241.7);
const VECTOR3 SAB_MESH_OFFSET = _V(0, VERT_MESH_OFFSET, 138.25);
const VECTOR3 MST_MESH_OFFSET = _V(0, VERT_MESH_OFFSET, -113.5);

const double SLC6_ORBITER_ACCESS_ARM_RATE = 0.02222;
const double SLC6_VENT_ARM_RATE = 0.023810;
const double SLC6_VENT_HOOD_RATE = 0.04166667;
const double SLC6_GH2_ARM_RATE = 2.0;
const double SLC6_IAA_RATE = 0.004762;
const double SLC6_PCR_TRANSLATE_RATE = 1/2400.0; // 40 minutes to move
const double SLC6_SAB_TRANSLATE_RATE = 1/2400.0; // 40 minutes to move
const double SLC6_MST_TRANSLATE_RATE = 1/2400.0; // 40 minutes to move
const double SLC6_SAB_DOOR_RATE = 1/600.0;

class SLC6 : public VESSEL3, public ISSULaunchTower, public ISSUMLP
{
	friend BOOL CALLBACK SLC6_DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	MESHHANDLE hPadMesh, hTowerMesh;
	MESHHANDLE hPCRMesh, hMSTMesh, hSABMesh;
	UINT pad_mesh_idx, tower_mesh_idx;
	UINT pcr_mesh_idx, mst_mesh_idx, sab_mesh_idx;

	ATTACHMENTHANDLE ahHDP;

	AnimState AccessArmState, VentHoodState, VentArmState;
	AnimState GH2VentlineState;
	AnimState IAAState;
	AnimState PCRState, SABState, MSTState;
	AnimState SABDoorState;

	UINT anim_AccessArm;
	UINT anim_VentHood, anim_VentArm;
	UINT anim_GH2Ventline;
	UINT anim_IAA;
	UINT anim_PCR, anim_SAB, anim_MST;
	UINT anim_SABDoor;

	std::vector<MGROUP_TRANSFORM*> vpAnimations;

	double ROFILevel, ROFIStartTime;
	//bool bSSSActive;
	double SSSLevel, SSS_SSMESteam, SSS_SRBSteam;
	//double SSS_SSMELevel, SSS_SRBLevel;

	bool bGLSAutoSeq;
	double timeToLaunch;
public:
	SLC6(OBJHANDLE hVessel, int flightmodel);
	~SLC6();

	void clbkSetClassCaps(FILEHANDLE cfg);
	void clbkPostCreation();
	void clbkPreStep(double simt, double simdt, double mjd);
	void clbkSaveState(FILEHANDLE scn);
	void clbkLoadStateEx(FILEHANDLE scn, void *status);
	int clbkConsumeBufferedKey(DWORD key, bool down, char *keystate);

	// ISSUMLP
	virtual void GLSAutoSeqStart();
	virtual void OnT0();
	virtual void TriggerROFIs();
	virtual void ActivateSSS();

	// ISSULaunchTower
	// NOTE: OnT0() is required by both interfaces
	virtual void ExtendOrbiterAccessArm();
	virtual void RetractOrbiterAccessArm();
	virtual void HaltOrbiterAccessArm();

	virtual void ExtendGOXArm();
	virtual void RetractGOXArm();
	virtual void HaltGOXArm();

	virtual void AttachGH2Ventline();
	virtual void DetachGH2Ventline();

	virtual void RaiseVentHood();
	virtual void LowerVentHood();
	virtual void HaltVentHood();

	virtual void ExtendGOXArmAndHood();
	virtual void RetractGOXArmAndHood();
	virtual void HaltGOXArmAndHood();

	virtual void DeployIAA();
	virtual void HaltIAA();
	virtual void RetractIAA();

	virtual void ExtendPCR();
	virtual void HaltPCR();
	virtual void RetractPCR();

	virtual void ExtendSAB();
	virtual void HaltSAB();
	virtual void RetractSAB();
	virtual void OpenSABDoor();
	virtual void HaltSABDoor();
	virtual void CloseSABDoor();

	virtual void ExtendMST();
	virtual void HaltMST();
	virtual void RetractMST();
private:
	void DetachShuttle() const;

	void DefineAnimations();
	void DefineROFIs();
	void DefineSSS();

	/**
	 * Creates MGROUP_ROTATE struct, adds it to animation list, and returns pointer to struct
	 */
	MGROUP_ROTATE* DefineRotation(UINT mesh, UINT *grp, UINT ngrp, const VECTOR3 &ref, const VECTOR3 &axis, float angle);
	MGROUP_TRANSLATE* DefineTranslation(UINT mesh, UINT* grp, UINT ngrp, const VECTOR3& shift);
};

#endif //SLC6_H_640EA460_C8C8_46B1_8604_B46C72BD8984