#include "PlBayOp.h"
#include "ParameterValues.h"
#include "resource.h"
#include "meshres_vc.h"
#include "vc\vc_defs.h"
#include "DlgCtrl.h"
#include <stdio.h>
#include "meshres_vc_additions.h"
#include "Atlantis_defs.h"

extern GDIParams g_Param;
extern HELPCONTEXT g_hc;
extern char *ActionString[5];

BOOL CALLBACK PlOp_DlgProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static Atlantis *sts_dlg;

// ==============================================================

PayloadBayOp::PayloadBayOp (Atlantis *_sts): sts(_sts)
{
	sts_dlg = sts;

	int i;

	// Cargo bay doors
	for (i = 0; i < 2; i++) BayDoor[i] = BD_DISABLE;
	BayDoorOp = BDO_STOP;
	BayDoorStatus.Set (AnimState::CLOSED, 0);
	for(i = 0; i < 4; i++) CLBayDoorLatch[i].Set(AnimState::CLOSED, 0);

	// Radiators
	for (i = 0; i < 2; i++) MechPwr[i] = MP_OFF;
	for (i = 0; i < 2; i++) RadiatorCtrl[i] = RC_OFF;
	for (i = 0; i < 2; i++) RadLatchCtrl[i] = LC_OFF;
	RadiatorStatus.Set (AnimState::CLOSED, 0);
	RadLatchStatus.Set (AnimState::CLOSED, 0);

	// Ku-band antenna
	KuCtrl = KU_GND;
	KuDirectCtrl = KU_DIRECT_OFF;
	KuAntennaStatus.Set (AnimState::CLOSED, 0);
}

// ==============================================================

void PayloadBayOp::Step (double t, double dt)
{
	// Operate cargo doors
	for(int i=0;i<4;i++) {
		//animate centerline door latches
		if(CLBayDoorLatch[i].Moving()) {
			double da=dt*DOORLATCH_OPERATING_SPEED;
			if(CLBayDoorLatch[i].Closing()) {
				if(CLBayDoorLatch[i].pos > 0.0)
					CLBayDoorLatch[i].pos=max(0.0, CLBayDoorLatch[i].pos-da);
				else {
					CLBayDoorLatch[i].action=AnimState::CLOSED;
					AutoDoorSequenceControl();
				}
			}
			else {
				if(CLBayDoorLatch[i].pos < 1.0)
					CLBayDoorLatch[i].pos=min(1.0, CLBayDoorLatch[i].pos+da);
				else {
					CLBayDoorLatch[i].action=AnimState::OPEN;
					AutoDoorSequenceControl();
				}
			}
			sts->SetBayDoorLatchPosition(i, CLBayDoorLatch[i].pos);
		}
	}
	if (BayDoorStatus.Moving()) {
		double da = dt * DOOR_OPERATING_SPEED;
		if (BayDoorStatus.Closing()) {
			if (BayDoorStatus.pos > 0.0)
				BayDoorStatus.pos = max (0.0, BayDoorStatus.pos-da);
			else {
				SetDoorAction (AnimState::CLOSED);
				AutoDoorSequenceControl();
			}
		} else { // door opening
			if (BayDoorStatus.pos < 1.0)
				BayDoorStatus.pos = min (1.0, BayDoorStatus.pos+da);
			else {
				SetDoorAction (AnimState::OPEN);
				AutoDoorSequenceControl();
			}
		}
		sts->SetBayDoorPosition (BayDoorStatus.pos);
	}

	// Operate radiators
	if (RadiatorStatus.Moving()) {
		double da;
		if(RadiatorCtrl[0]==RadiatorCtrl[1]) da = dt * RAD_OPERATING_SPEED;
		else da = dt * RAD_OPERATING_SPEED * 0.5;
		if (RadiatorStatus.Closing()) {
			if (RadiatorStatus.pos > 0.0)
				RadiatorStatus.pos = max (0.0, RadiatorStatus.pos-da);
			else
				SetRadiatorAction (AnimState::CLOSED);
		} else { // radiator deploying
			if (RadiatorStatus.pos < 1.0)
				RadiatorStatus.pos = min (1.0, RadiatorStatus.pos+da);
			else
				SetRadiatorAction (AnimState::OPEN);
		}
		sts->SetRadiatorPosition (RadiatorStatus.pos);
	}

	// Operate radiator latches
	if (RadLatchStatus.Moving()) {
		double da;
		if(RadLatchCtrl[0]==RadLatchCtrl[1]) da = dt * RADLATCH_OPERATING_SPEED;
		else da = dt * RADLATCH_OPERATING_SPEED * 0.5; //only one motor working
		if (RadLatchStatus.Closing()) {
			if (RadLatchStatus.pos > 0.0)
				RadLatchStatus.pos = max (0.0, RadLatchStatus.pos-da);
			else
				SetRadLatchAction (AnimState::CLOSED);
		} else { // radiator latches releasing
			if (RadLatchStatus.pos < 1.0)
				RadLatchStatus.pos = min (1.0, RadLatchStatus.pos+da);
			else
				SetRadLatchAction (AnimState::OPEN);
		}
		sts->SetRadLatchPosition (RadLatchStatus.pos);
	}

	// Operate Ku-band antenna
	if (KuAntennaStatus.Moving()) {
		double da = dt * KU_OPERATING_SPEED;
		if (KuAntennaStatus.Closing()) {
			if (KuAntennaStatus.pos > 0.0)
				KuAntennaStatus.pos = max (0.0, KuAntennaStatus.pos-da);
			else
				SetKuAntennaAction (AnimState::CLOSED);
		} else { // antenna deploying
			if (KuAntennaStatus.pos < 1.0)
				KuAntennaStatus.pos = min (1.0, KuAntennaStatus.pos+da);
			else
				SetKuAntennaAction (AnimState::OPEN);
		}
		sts->SetKuAntennaPosition (KuAntennaStatus.pos);
	}
}

// ==============================================================

void PayloadBayOp::SetDoorAction (AnimState::Action action)
{
	int i;

	//if (KuAntennaStatus.action != AnimState::CLOSED) return;
	// operate payload bay doors only if Ku-band antenna is stowed

	//if (RadiatorStatus.action != AnimState::CLOSED) return;
	// operate payload bay doors only if radiators are stowed

	//if(sts->RMSRollout.action != AnimState::CLOSED && sts->RMS) return;
	// RMS must be stowed

	for (i = 0; i < 2; i++)
		if ((action != AnimState::STOPPED) && (BayDoor[i] != BD_ENABLE)) return;
	// operate doors only if both systems are enabled

	if (action == AnimState::STOPPED && BayDoorStatus.Static()) return;
	// stopping doesn't make sense if the doors are already fully open or closed

	BayDoorStatus.action = action;
	sts->RecordEvent ("CARGODOOR", ActionString[action]);

	UpdateVC();
}

// ==============================================================

void PayloadBayOp::AutoDoorSequenceControl()
{
	if(BayDoorOp==BDO_OPEN) {
		if(!CLBayDoorLatch[1].Open() || !CLBayDoorLatch[2].Open()) { // open middle gangs first
			if(!CLBayDoorLatch[1].Open()) CLBayDoorLatch[1].action=AnimState::OPENING;
			if(!CLBayDoorLatch[2].Open()) CLBayDoorLatch[2].action=AnimState::OPENING;
		}
		else if(!CLBayDoorLatch[0].Open() || !CLBayDoorLatch[3].Open()) { // next open remaining gangs
			if(!CLBayDoorLatch[0].Open()) CLBayDoorLatch[0].action=AnimState::OPENING;
			if(!CLBayDoorLatch[3].Open()) CLBayDoorLatch[3].action=AnimState::OPENING;
		}
		else { // finally, open PLBD doors
			if(!BayDoorStatus.Open()) SetDoorAction(AnimState::OPENING);
		}
	}
	else if(BayDoorOp==BDO_CLOSE) {
		if(!BayDoorStatus.Closed()) { // close PLBD doors first
			SetDoorAction(AnimState::CLOSING);
		}
		else if(!CLBayDoorLatch[0].Closed() || !CLBayDoorLatch[3].Closed()) { // then close fwd and aft latch gangs
			if(!CLBayDoorLatch[0].Closed()) CLBayDoorLatch[0].action=AnimState::CLOSING;
			if(!CLBayDoorLatch[3].Closed()) CLBayDoorLatch[3].action=AnimState::CLOSING;
		}
		else { // finally, close middle latch gangs
			if(!CLBayDoorLatch[1].Closed()) CLBayDoorLatch[1].action=AnimState::CLOSING;
			if(!CLBayDoorLatch[2].Closed()) CLBayDoorLatch[2].action=AnimState::CLOSING;
		}
	}
	else { // BDO_STOP
		if(BayDoorStatus.Moving()) SetDoorAction(AnimState::STOPPED);
		else if(CLBayDoorLatch[0].Moving()) CLBayDoorLatch[0].action=AnimState::STOPPED;
		else if(CLBayDoorLatch[3].Moving()) CLBayDoorLatch[3].action=AnimState::STOPPED;
		else if(CLBayDoorLatch[1].Moving()) CLBayDoorLatch[1].action=AnimState::STOPPED;
		else if(CLBayDoorLatch[2].Moving()) CLBayDoorLatch[2].action=AnimState::STOPPED;
	}
}

// ==============================================================

void PayloadBayOp::SetRadiatorAction (AnimState::Action action)
{
	int i;

	if (BayDoorStatus.action != AnimState::OPEN) return;
	// allow radiator operation only once the bay doors are fully open

	for (i = 0; i < 2; i++)
		if ((action != AnimState::STOPPED) && (MechPwr[i] != MP_ON)) return;
	// operate radiators only if power is online

	/*for (i = 0; i < 2; i++) { // check both systems are set correctly
		if (action == AnimState::OPENING && RadiatorCtrl[i] != RC_DEPLOY) return;
		if (action == AnimState::CLOSING && RadiatorCtrl[i] != RC_STOW) return;
	}*/
	if(action==AnimState::OPENING) {
		if(RadiatorCtrl[0]!=RC_DEPLOY && RadiatorCtrl[1]!=RC_DEPLOY) return;
		else if (RadiatorStatus.Closed() && !RadLatchStatus.Open()) return;
		// don't deploy radiators if the latches are not fully released
	}
	else if(action==AnimState::CLOSING) {
		if(RadiatorCtrl[0]!=RC_STOW && RadiatorCtrl[1]!=RC_STOW) return;
	}
	else if (action == AnimState::STOPPED && RadiatorStatus.Static()) return;
	// stopping doesn't make sense if the radiators are already fully deployed or stowed

	RadiatorStatus.action = action;
	sts->RecordEvent ("RADIATOR", ActionString[action]);

	UpdateVC();
}

// ==============================================================

/*void PayloadBayOp::RevertDoorAction ()
{
	int i;
	for (i = 0; i < 2; i++) BayDoor[i] = BD_ENABLE;
	// Make sure both systems are online

	SetDoorAction (BayDoorStatus.action == AnimState::CLOSED || BayDoorStatus.action == AnimState::CLOSING ?
		AnimState::OPENING : AnimState::CLOSING);
}*/

// ==============================================================

void PayloadBayOp::SetRadLatchAction (AnimState::Action action)
{
	int i;

	for (i = 0; i < 2; i++)
		if ((action != AnimState::STOPPED) && (MechPwr[i] != MP_ON)) return;
	// operate radiator latches only if power is online

	if(action==AnimState::OPENING) {
		if(RadLatchCtrl[0]!=LC_RELEASE && RadLatchCtrl[1]!=LC_RELEASE) return;
	}
	else if(action==AnimState::CLOSING) {
		if(RadLatchCtrl[0]!=LC_LATCH && RadLatchCtrl[1]!=LC_LATCH) return;
	}

	else if (action == AnimState::STOPPED && RadLatchStatus.Static()) return;
	// stopping doesn't make sense if the radiators are already fully deployed or stowed

	RadLatchStatus.action = action;
	sts->RecordEvent ("RADLATCH", ActionString[action]);

	UpdateVC();
}

// ==============================================================

void PayloadBayOp::SetKuAntennaAction (AnimState::Action action)
{
	if (BayDoorStatus.action != AnimState::OPEN) return;
	// allow radiator operation only once the bay doors are fully open

	if (action == AnimState::STOPPED && KuAntennaStatus.Static()) return;
	// stopping doesn't make sense if the doors are already fully open or closed

	for (int i = 0; i < 2; i++)
		if ((action != AnimState::STOPPED) && (MechPwr[i] != MP_ON)) return;
	// make sure power is online

	KuAntennaStatus.action = action;
	sts->RecordEvent ("KUBAND", ActionString[action]);

	UpdateVC();
}

// ==============================================================

void PayloadBayOp::RevertKuAntennaAction ()
{
	SetKuAntennaAction (KuAntennaStatus.action == AnimState::CLOSED || KuAntennaStatus.action == AnimState::CLOSING ?
		AnimState::OPENING : AnimState::CLOSING);
}

// ==============================================================

bool PayloadBayOp::ParseScenarioLine (char *line)
{
	if (!_strnicmp (line, "CARGODOOR", 9)) {
		sscan_state (line+9, BayDoorStatus);
		return true;
	} else if (!_strnicmp (line, "RADIATOR", 8)) {
		sscan_state (line+8, RadiatorStatus);
		return true;
	} else if (!_strnicmp (line, "RADLATCH", 8)) {
		sscan_state (line+8, RadLatchStatus);
		return true;
	} else if (!_strnicmp (line, "KUBAND", 6)) {
		sscan_state (line+6, KuAntennaStatus);
		return true;
	} else if (!_strnicmp (line, "BAYDOORLATCH", 12)) {
		int latch;
		sscanf_s (line+12, "%d", &latch);
		sscan_state (line+13, CLBayDoorLatch[latch]);
	}
	return false;
}

// ==============================================================

void PayloadBayOp::SaveState (FILEHANDLE scn)
{
	char cbuf[256];

	if (!BayDoorStatus.Closed())
		WriteScenario_state (scn, "CARGODOOR", BayDoorStatus);
	if (!RadiatorStatus.Closed())
		WriteScenario_state (scn, "RADIATOR", RadiatorStatus);
	if (!RadLatchStatus.Closed())
		WriteScenario_state (scn, "RADLATCH", RadLatchStatus);
	if (!KuAntennaStatus.Closed())
		WriteScenario_state (scn, "KUBAND", KuAntennaStatus);
	for(int i=0;i<4;i++) {
		sprintf_s(cbuf, 255, "BAYDOORLATCH%d", i);
		if(!CLBayDoorLatch[i].Closed()) WriteScenario_state (scn, cbuf, CLBayDoorLatch[i]);
	}
}

// ==============================================================

void PayloadBayOp::DefineAnimations (UINT vcidx)
{
	static VECTOR3 switch_rot = {0,0,1};
	static VECTOR3 switch_row1 = {1.271, 2.259, 12.501};
	static VECTOR3 switch_row2 = {1.18, 2.199, 12.504};
	static VECTOR3 switch_row3 = {1.093, 2.142, 12.503};

	// Animations for switches on panel R13L in the VC
	static UINT VC_R13L_S1_Grp = GRP_SWITCHR13L_1_VC;
	static MGROUP_ROTATE VC_R13L_S1 (vcidx, &VC_R13L_S1_Grp, 1,
		switch_row1, switch_rot, (float)(90.0*RAD));
	anim_VC_R13L[0] = sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R13L[0], 0, 1, &VC_R13L_S1);

	static UINT VC_R13L_S2_Grp = GRP_SWITCHR13L_2_VC;
	static MGROUP_ROTATE VC_R13L_S2 (vcidx, &VC_R13L_S2_Grp, 1,
		switch_row1, switch_rot, (float)(90.0*RAD));
	anim_VC_R13L[1] = sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R13L[1], 0, 1, &VC_R13L_S2);

	static UINT VC_R13L_S3_Grp = GRP_SWITCHR13L_3_VC;
	static MGROUP_ROTATE VC_R13L_S3 (vcidx, &VC_R13L_S3_Grp, 1,
		switch_row1, switch_rot, (float)(90.0*RAD));
	anim_VC_R13L[2] = sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R13L[2], 0, 1, &VC_R13L_S3);

	static UINT VC_R13L_S4_Grp = GRP_SWITCHR13L_4_VC;
	static MGROUP_ROTATE VC_R13L_S4 (vcidx, &VC_R13L_S4_Grp, 1,
		switch_row1, switch_rot, (float)(90.0*RAD));
	anim_VC_R13L[3] = sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R13L[3], 0, 1, &VC_R13L_S4);

	static UINT VC_R13L_S5_Grp = GRP_SWITCHR13L_5_VC;
	static MGROUP_ROTATE VC_R13L_S5 (vcidx, &VC_R13L_S5_Grp, 1,
		switch_row2, switch_rot, (float)(90.0*RAD));
	anim_VC_R13L[4] = sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R13L[4], 0, 1, &VC_R13L_S5);

	static UINT VC_R13L_S6_Grp = GRP_SWITCHR13L_6_VC;
	static MGROUP_ROTATE VC_R13L_S6 (vcidx, &VC_R13L_S6_Grp, 1,
		switch_row2, switch_rot, (float)(90.0*RAD));
	anim_VC_R13L[5] = sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R13L[5], 0, 1, &VC_R13L_S6);

	static UINT VC_R13L_S7_Grp = GRP_SWITCHR13L_7_VC;
	static MGROUP_ROTATE VC_R13L_S7 (vcidx, &VC_R13L_S7_Grp, 1,
		switch_row2, switch_rot, (float)(90.0*RAD));
	anim_VC_R13L[6] = sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R13L[6], 0, 1, &VC_R13L_S7);

	static UINT VC_R13L_S8_Grp = GRP_SWITCHR13L_8_VC;
	static MGROUP_ROTATE VC_R13L_S8 (vcidx, &VC_R13L_S8_Grp, 1,
		switch_row2, switch_rot, (float)(90.0*RAD));
	anim_VC_R13L[7] = sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R13L[7], 0, 1, &VC_R13L_S8);

	static UINT VC_R13L_S9_Grp = GRP_SWITCHR13L_9_VC;
	static MGROUP_ROTATE VC_R13L_S9 (vcidx, &VC_R13L_S9_Grp, 1,
		switch_row2, switch_rot, (float)(90.0*RAD));
	anim_VC_R13L[8] = sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R13L[8], 0, 1, &VC_R13L_S9);

	static UINT VC_R13L_S10_Grp = GRP_SWITCHR13L_10_VC;
	static MGROUP_ROTATE VC_R13L_S10 (vcidx, &VC_R13L_S10_Grp, 1,
		switch_row3, switch_rot, (float)(90.0*RAD));
	anim_VC_R13L[9] = sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R13L[9], 0, 1, &VC_R13L_S10);

	static UINT VC_R13L_S11_Grp = GRP_SWITCHR13L_11_VC;
	static MGROUP_ROTATE VC_R13L_S11 (vcidx, &VC_R13L_S11_Grp, 1,
		switch_row3, switch_rot, (float)(90.0*RAD));
	anim_VC_R13L[10] = sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_R13L[10], 0, 1, &VC_R13L_S11);
}

// ==============================================================

void PayloadBayOp::RegisterVC ()
{
	VECTOR3 ofs = sts->GetOrbiterCoGOffset() + VC_OFFSET;
	SURFHANDLE tkbk_tex = oapiGetTextureHandle (sts->hOrbiterVCMesh, TEX_TALKBACK_VC);

	// register the complete panel for mouse events
	oapiVCRegisterArea (AID_R13L, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);

	//The top left is X=1.32, Y=2.29, Z=12.56
	//bottom right is X=1.06, Y=2.12, Z=12.26

	oapiVCSetAreaClickmode_Quadrilateral (AID_R13L, _V(1.32,2.29,12.56)+ofs, _V(1.32,2.29,12.26)+ofs,
		_V(1.06,2.12,12.56)+ofs, _V(1.06,2.12,12.26)+ofs);

	// register the talkbacks
	oapiVCRegisterArea (AID_R13L_TKBK1, _R(  0,0, 32,18), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tkbk_tex);
	oapiVCRegisterArea (AID_R13L_TKBK2, _R( 32,0, 64,18), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tkbk_tex);
	oapiVCRegisterArea (AID_R13L_TKBK3, _R( 64,0, 96,18), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tkbk_tex);
	oapiVCRegisterArea (AID_R13L_TKBK4, _R( 96,0,128,18), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tkbk_tex);
	oapiVCRegisterArea (AID_R13L_TKBK5, _R(128,0,160,18), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tkbk_tex);
	oapiVCRegisterArea (AID_R13L_TKBK6, _R(160,0,192,18), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tkbk_tex);

	for (int i = 0; i < 6; i++)
		tkbk_state[i] = -1;
}

// ==============================================================

void PayloadBayOp::UpdateVC ()
{
	int i;
	for (i = 0; i < 2; i++) {
		bool enable = (BayDoor[i] == BD_ENABLE);
		sts->SetAnimation (anim_VC_R13L[i], enable ? 0:1);
	}
	for (i = 0; i < 2; i++) {
		bool enable = (MechPwr[i] == MP_ON);
		sts->SetAnimation (anim_VC_R13L[i+2], enable ? 0:1);
	}
	sts->SetAnimation(anim_VC_R13L[4], BayDoorOp == BDO_OPEN ? 0 : BayDoorOp == BDO_CLOSE ? 1 : 0.5);
	for (i = 0; i < 2; i++) {
		sts->SetAnimation (anim_VC_R13L[5+i], RadLatchCtrl[i] == LC_RELEASE ? 0 : RadLatchCtrl[i] == LC_LATCH ? 1 : 0.5);
	}
	for (i = 0; i < 2; i++) {
		sts->SetAnimation (anim_VC_R13L[7+i], RadiatorCtrl[i] == RC_DEPLOY ? 0 : RadiatorCtrl[i] == RC_STOW ? 1 : 0.5);
	}
	sts->SetAnimation (anim_VC_R13L[9], KuDirectCtrl == KU_DIRECT_ON ? 0 : 1);
	sts->SetAnimation (anim_VC_R13L[10], KuCtrl == KU_DEPLOY ? 0 : KuCtrl == KU_STOW ? 1 : 0.5);

	for (i = 0; i < 6; i++)
		oapiVCTriggerRedrawArea (-1, AID_R13L_TKBK1+i);
}

// ==============================================================

bool PayloadBayOp::VCDrawTalkback (SURFHANDLE surf, int idx, int label)
{
	if (tkbk_state[idx] == label) return false; // nothing to do
	tkbk_state[idx] = label;
	if(label<8) oapiBlt (surf, g_Param.tkbk_label, 0, 0, label*32, 0, 32, 18);
	else oapiBlt (surf, g_Param.tkbk_label, 0, 0, (label-8)*32, 18, 32, 18);
	return true;
}

// ==============================================================

bool PayloadBayOp::VCMouseEvent (int id, int event, VECTOR3 &p)
{
	if (id != AID_R13L) return false;
	bool action = false;

	if (p.y >= 0.1113 && p.y <= 0.2461) {
		if (p.x >= 0.1387 && p.x <= 0.2617) {
			BayDoor[0] = (p.y < 0.1787 ? BD_ENABLE:BD_DISABLE);
			if (BayDoor[0] == BD_DISABLE) SetDoorAction (AnimState::STOPPED);
			action = true;
		} else if (p.x >= 0.2910 && p.x <= 0.4180) {
			BayDoor[1] = (p.y < 0.1787 ? BD_ENABLE:BD_DISABLE);
			if (BayDoor[1] == BD_DISABLE) SetDoorAction (AnimState::STOPPED);
			action = true;
		} else if (p.x >= 0.4395 && p.x <= 0.5625) {
			MechPwr[0] = (p.y < 0.1787 ? MP_ON:MP_OFF);
			if (MechPwr[0] == MP_OFF) {
				SetRadiatorAction (AnimState::STOPPED);
				SetRadLatchAction (AnimState::STOPPED);
			}
			else if(MechPwr[1] == MP_ON) {
				if(KuCtrl==KU_DEPLOY) SetKuAntennaAction(AnimState::OPENING);
				else if(KuCtrl==KU_STOW) SetKuAntennaAction(AnimState::CLOSING);
			}
			action = true;
		} else if (p.x >= 0.5996 && p.x < 0.7188) {
			MechPwr[1] = (p.y < 0.1787 ? MP_ON:MP_OFF);
			if (MechPwr[1] == MP_OFF) {
				SetRadiatorAction (AnimState::STOPPED);
				SetRadLatchAction (AnimState::STOPPED);
			}
			else if(MechPwr[0] == MP_ON) {
				if(KuCtrl==KU_DEPLOY) SetKuAntennaAction(AnimState::OPENING);
				else if(KuCtrl==KU_STOW) SetKuAntennaAction(AnimState::CLOSING);
			}
			action = true;
		}
	} else if (p.y >= 0.4590 && p.y <= 0.6016) {
		bool up = (p.y < 0.5303);
		if (p.x >= 0.125 && p.x <= 0.2539) {
			BayDoorOp = (up ? (BayDoorOp == BDO_STOP ? BDO_OPEN : BDO_STOP) : BayDoorOp == BDO_STOP ? BDO_CLOSE : BDO_STOP);
			//SetDoorAction (BayDoorOp == BDO_OPEN ? AnimState::OPENING : BayDoorOp == BDO_CLOSE ? AnimState::CLOSING : AnimState::STOPPED);
			AutoDoorSequenceControl();
			action = true;
		} else if (p.x >= 0.2832 && p.x <= 0.4082) {
			RadLatchCtrl[0] = (up ? (RadLatchCtrl[0] == LC_OFF ? LC_RELEASE : LC_OFF) : RadLatchCtrl[0] == LC_OFF ? LC_LATCH : LC_OFF);
			SetRadLatchAction (RadLatchCtrl[0] == LC_RELEASE ? AnimState::OPENING : RadLatchCtrl[0] == LC_LATCH ? AnimState::CLOSING : AnimState::STOPPED);
			action = true;
		} else if (p.x >= 0.4414 && p.x <= 0.5645) {
			RadLatchCtrl[1] = (up ? (RadLatchCtrl[1] == LC_OFF ? LC_RELEASE : LC_OFF) : RadLatchCtrl[1] == LC_OFF ? LC_LATCH : LC_OFF);
			SetRadLatchAction (RadLatchCtrl[1] == LC_RELEASE ? AnimState::OPENING : RadLatchCtrl[1] == LC_LATCH ? AnimState::CLOSING : AnimState::STOPPED);
			action = true;
		} else if (p.x >= 0.5996 && p.x <= 0.7227) {
			RadiatorCtrl[0] = (up ? (RadiatorCtrl[0] == RC_OFF ? RC_DEPLOY : RC_OFF) : RadiatorCtrl[0] == RC_OFF ? RC_STOW : RC_OFF);
			SetRadiatorAction (RadiatorCtrl[0] == RC_DEPLOY ? AnimState::OPENING : RadiatorCtrl[0] == RC_STOW ? AnimState::CLOSING : AnimState::STOPPED);
			action = true;
		} else if (p.x >= 0.7559 && p.x <= 0.8789) {
			RadiatorCtrl[1] = (up ? (RadiatorCtrl[1] == RC_OFF ? RC_DEPLOY : RC_OFF) : RadiatorCtrl[1] == RC_OFF ? RC_STOW : RC_OFF);
			SetRadiatorAction (RadiatorCtrl[1] == RC_DEPLOY ? AnimState::OPENING : RadiatorCtrl[1] == RC_STOW ? AnimState::CLOSING : AnimState::STOPPED);
			action = true;
		}
	} else if (p.y >= 0.7891 && p.y <= 0.9219) {
		bool up = (p.y < 0.8555);
		if (p.x >= 0.1328 && p.x <= 0.2559) {
			KuDirectCtrl = (up ? KU_DIRECT_ON : KU_DIRECT_OFF);
			if (KuDirectCtrl == KU_DIRECT_ON) SetKuAntennaAction (AnimState::CLOSING);
			action = true;
		} else if (p.x >= 0.2871 && p.x <= 0.4082) {
			KuCtrl = (up ? (KuCtrl == KU_GND ? KU_DEPLOY : KU_GND) : KuCtrl == KU_GND ? KU_STOW : KU_GND);
			SetKuAntennaAction (KuCtrl == KU_DEPLOY ? AnimState::OPENING : KuCtrl == KU_STOW ? AnimState::CLOSING : AnimState::STOPPED);
			action = true;
		}
	}

	if (action) {
		UpdateVC ();
	}
	return false;
}

// ==============================================================

bool PayloadBayOp::VCRedrawEvent (int id, int event, SURFHANDLE surf)
{
	switch (id) {
	case AID_R13L_TKBK1: {
		static int label[5] = {0,3,4,0,0};
		return VCDrawTalkback (surf, 0, label[BayDoorStatus.action]);
		}
	case AID_R13L_TKBK2:
	case AID_R13L_TKBK3: {
		static int label[5] = {0,vc::TB_LAT,vc::TB_REL,0,0};
		return VCDrawTalkback (surf, id-AID_R13L_TKBK1, label[RadLatchStatus.action]);
		}
	case AID_R13L_TKBK4:
	case AID_R13L_TKBK5: {
		static int label[5] = {0,vc::TB_STO,vc::TB_DPY,0,0};
		return VCDrawTalkback (surf, id-AID_R13L_TKBK1, label[RadiatorStatus.action]);
		}
	case AID_R13L_TKBK6: {
		static int label[5] = {0,vc::TB_STO,vc::TB_DPY,0,0};
		return VCDrawTalkback (surf, id-AID_R13L_TKBK1, label[KuAntennaStatus.action]);
		}
	}
	return false;
}
