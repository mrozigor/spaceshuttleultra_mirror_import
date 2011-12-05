#ifndef ___ATLANTIS_MLP_H___
#define ___ATLANTIS_MLP_H___

#include "OrbiterAPI.h"
#include "ISSUMLP.h"

const VECTOR3 POS_MPS_SMOKE = _V(0.0, -15.0, -20.0);
const VECTOR3 POS_SRB_SMOKE = _V(0.0, -15.0, +20.0);

const VECTOR3 DIR_MPS_SMOKE = _V(0.0, 0.0, -1.0);
const VECTOR3 DIR_SRB_SMOKE = _V(0.0, 0.0, 1.0);

const VECTOR3 FWD_LEFT_ROFI_POS = _V(-4.868061, 2.137968, -14.134);
const VECTOR3 FWD_RIGHT_ROFI_POS = _V(5.022561, 2.137968, -14.134);
const VECTOR3 AFT_LEFT_ROFI_POS = _V(-4.519, 2.137968, -11.186);
const VECTOR3 AFT_RIGHT_ROFI_POS = _V(4.518, 2.137968, -11.186);

const VECTOR3 HDP_POS = _V(0, -0.56, -5.75);

const double TSM_UMBILICAL_RETRACT_SPEED = 1.5;

class Atlantis;

class MLP: public VESSEL2, public ISSUMLP
{
	
public:
	virtual void SignalGSEStart();
	virtual void OnT0();
	virtual void TriggerHDP();
	virtual void TriggerROFIs();
	virtual void ActivateSSS();

	MLP(OBJHANDLE hVessel, int iFlightModel);
	virtual ~MLP();

	virtual void clbkSetClassCaps(FILEHANDLE cfg);
	virtual void clbkLoadStateEx(FILEHANDLE scn, void* vs);
	virtual bool clbkLoadVC(int id);
	virtual void clbkSaveState(FILEHANDLE scn);
	virtual void clbkPreStep(double fSimT, double fDeltaT, double mjd);
	virtual void clbkVisualCreated(VISHANDLE _vis, int refcount);
	virtual void clbkVisualDestroyed(VISHANDLE _vis, int refcount);
	virtual int clbkConsumeBufferedKey(DWORD key, bool down, char* keystate);
private:
	MESHHANDLE mshMLP;
	double fCountdown;
	bool bStartSequence;
	void CalculateSteamProduction(double fSimT, double fDeltaT);
	void Twang(double TMinus);
	bool bSSS_Active;
	double fT_SSSActive;
	ATTACHMENTHANDLE ahHDP;
	double fSRBSteam;
	double fSSMESteam;
	double ROFILevel;
	double ROFIStartTime;

	double dTimer;
	bool bPadLightsOn;
	UINT msh_idx;
	VISHANDLE vis;

	UINT anim_t0umb;			//T0 umbilical animation
	AnimState T0UmbilicalState;

	ATTACHMENTHANDLE ahBase; // used to attach to crawler/LC39/VAB

	Atlantis* GetShuttleOnPad();

	double CalculateThrustPower(
		const VECTOR3& vtx_lt,
		const VECTOR3& vtx_rt,
		const VECTOR3& vtx_lb,
		const VECTOR3& vtx_rb);

	void DefineAnimations();
};

#endif
