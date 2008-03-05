#include "PanelA8.h"
#include "resource.h"
//#include "meshres.h"
#include "meshres_vc.h"
#include "meshres_vc_additions.h"
#include "DlgCtrl.h"
#include <stdio.h>
//#include "MasterTimingUnit.h"

extern GDIParams g_Param;
extern HELPCONTEXT g_hc;
extern char *ActionString[5];

// ==============================================================

PanelA8::PanelA8(Atlantis *_sts): sts(_sts)
{
	int i;
	for(i=0;i<20;i++) {
		anim_VC_A8[i]=NULL;
		switch_state[i]=1;
		tkbk_state[i]=0;
	}
}

void PanelA8::RegisterVC()
{
	sprintf(oapiDebugString(), "Registering panelA8");
	VECTOR3 ofs=sts->orbiter_ofs;
	SURFHANDLE panela8t_tex = oapiGetTextureHandle (sts->hOrbiterVCMesh, TEX_A8TOP_VC);
	SURFHANDLE panela8b_tex = oapiGetTextureHandle (sts->hOrbiterVCMesh, TEX_A8BOTTOM_VC);

	oapiVCRegisterArea(AID_A8, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP);
	//oapiVCSetAreaClickmode_Quadrilateral (AID_A8, _V(-0.274, 2.848, 12.567)+ofs, _V(-0.813, 2.848, 12.567)+ofs, _V(-0.277, 2.055, 12.816)+ofs, _V(-0.819, 2.055, 12.816)+ofs);
	//oapiVCSetAreaClickmode_Quadrilateral (AID_A8, _V(-0.813, 2.848, 12.567)+ofs, _V(-0.274, 2.848, 12.567)+ofs, _V(-0.813, 2.3, 12.74)+ofs, _V(-0.274, 2.3, 12.74)+ofs);
	oapiVCSetAreaClickmode_Quadrilateral (AID_A8, _V(-0.813, 2.848, 12.567)+ofs, _V(-0.274, 2.848, 12.567)+ofs, _V(-0.813, 2.055, 12.816)+ofs, _V(-0.274, 2.055, 12.816)+ofs);
	
	oapiVCRegisterArea(AID_A8_TKBK7, _R(717, 648, 749, 666), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
	oapiVCRegisterArea(AID_A8_TKBK11, _R(901, 931, 933, 949), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
	//oapiVCRegisterArea(AID_A8_TKBK7, _R(614, 617, 646, 635), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
}

bool PanelA8::VCRedrawEvent (int id, int event, SURFHANDLE surf)
{
	switch(id) {
		case AID_A8_TKBK7:
			if(sts->RMSRollout.action==AnimState::OPEN)
				return VCDrawTalkback(surf, id-AID_A8_TKBK1, 9);
			else if(sts->RMSRollout.Moving()) return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			else if(sts->RMSRollout.action==AnimState::CLOSED)
				return VCDrawTalkback(surf, id-AID_A8_TKBK1, 1);
			break;
		case AID_A8_TKBK11:
			if(sts->shoulder_brace==0.0) return VCDrawTalkback(surf, id-AID_A8_TKBK11, 8);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK11, 0);
			break;
	}
	return false;
}

bool PanelA8::VCDrawTalkback(SURFHANDLE surf, int idx, int label)
{
	if (tkbk_state[idx] == label) return false; // nothing to do
	sprintf(oapiDebugString(), "Panel A8 Talkback: %d %d %d", idx, label, tkbk_state[idx]);
	tkbk_state[idx] = label;
	//if(label<=1) oapiBlt (surf, g_Param.tkbk_label, 0, 0, 0, label*18, 32, 18);
	//else oapiBlt (surf, g_Param.tkbk_label, 0, 0, (label-1)*32, 0, 32, 18);
	//oapiBlt(surf, g_Param.tkbk_label, 0, 0, 0, 0, 32, 18);
	if(label<8) oapiBlt (surf, g_Param.tkbk_label, 0, 0, label*32, 0, 32, 18);
	else oapiBlt (surf, g_Param.tkbk_label, 0, 0, (label-8)*32, 18, 32, 18);
	return true;
}

void PanelA8::DefineVCAnimations(UINT vcidx)
{
	static VECTOR3 switch_rot_vert=_V(1.0, 0.0, 0.0);
	static VECTOR3 switch_rot_horz=_V(0.0, 0.793, -0.249);

	static UINT VC_A8b6_GRP = GRP_A8b6_VC;
	static MGROUP_ROTATE VC_A8b6 (vcidx, &VC_A8b6_GRP, 1,
		_V(-0.665, 2.177, 12.780), switch_rot_vert, (float)(90.0*RAD));
	anim_VC_A8[SWITCH6]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent(anim_VC_A8[SWITCH6], 0, 1, &VC_A8b6);

	static UINT VC_A8b9_GRP = GRP_A8b9_VC;
	static MGROUP_ROTATE VC_A8b9 (vcidx, &VC_A8b9_GRP, 1,
		_V(-0.665, 2.177, 12.779), switch_rot_vert, (float)(90.0*RAD));
	anim_VC_A8[SWITCH9]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent(anim_VC_A8[SWITCH9], 0, 1, &VC_A8b9);

	static UINT VC_A8b10_GRP = GRP_A8b10_VC;
	static MGROUP_ROTATE VC_A8b10 (vcidx, &VC_A8b10_GRP, 1,
		_V(-0.707, 2.347, 12.727), switch_rot_horz, (float)(90.0*RAD));
	anim_VC_A8[SWITCH10]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent(anim_VC_A8[SWITCH10], 0, 1, &VC_A8b10);
}

void PanelA8::UpdateVC()
{
	sts->SetAnimation(anim_VC_A8[SWITCH6], switch_state[SWITCH6]/2.0);
	sts->SetAnimation(anim_VC_A8[SWITCH10], switch_state[SWITCH10]/2.0);
	oapiVCTriggerRedrawArea(-1, AID_A8_TKBK7);
	oapiVCTriggerRedrawArea(-1, AID_A8_TKBK11);
}

bool PanelA8::VCMouseEvent(int id, int event, VECTOR3 &p)
{
	bool action = false;
	//sprintf(oapiDebugString(), "Panel A8 event: %f %f %f %f", p.x, p.y, p.z);
	if(event & PANEL_MOUSE_LBPRESSED) sprintf(oapiDebugString(), "LBPressed");
	else if(event == PANEL_MOUSE_LBUP) sprintf(oapiDebugString(), "LBUp");
	else if(event & PANEL_MOUSE_LBDOWN) sprintf(oapiDebugString(), "LBDown");

	if(event & PANEL_MOUSE_LBDOWN) {
		if(p.x>=0.224348 && p.x<=0.298777) {
			if(p.y>=0.828706 && p.y<=0.877639) {
				if(p.y<0.853703) {
					//sprintf(oapiDebugString(), "Deploying PORT MPMs");
					if(switch_state[SWITCH6]>0) switch_state[SWITCH6]--;
				}
				else {
					//sprintf(oapiDebugString(), "Stowing PORT MPMs");
					if(switch_state[SWITCH6]<2) switch_state[SWITCH6]++;
				}
				if(switch_state[SWITCH6]==0) {
					if(sts->RMSRollout.action!=AnimState::OPEN) {
						sts->RMSRollout.action=AnimState::OPENING;
					}
				}
				else if(switch_state[SWITCH6]==1) {
					if(sts->RMSRollout.Moving()) sts->RMSRollout.action=AnimState::STOPPED;
				}
				else {
					if(sts->RMSRollout.action!=AnimState::CLOSED) {
						sts->RMSRollout.action=AnimState::CLOSING;
					}
				}
				/*if(RMSRollout.action==AnimState::CLOSED) {
					RMSRollout.action=AnimState::OPENING;
				}
				else {
					RMSRollout.action=AnimState::CLOSING;
				}*/
				action=true;
			}
		}

		else if(p.x>=0.840619 && p.x<=0.894526) {
			if(p.y>=0.832063 && p.y<=0.885879) {
				if(p.y<0.857561) {
					sprintf(oapiDebugString(), "Deploying STBD MPMs");
				}
				else {
					sprintf(oapiDebugString(), "Stowing STBD MPMs");
				}
				action=true;
			}
		}

		else if( p.x>=0.138298 && p.x<=0.211856) {
			if(p.y>=0.619803 && p.y<=0.657381) {
				if(p.x<0.176970) {
					if(switch_state[SWITCH10]>0) switch_state[SWITCH10]--;
				}
				else {
					if(switch_state[SWITCH10]<2) switch_state[SWITCH10]++;
				}
				//sprintf(oapiDebugString(), "%d", switch_state[SWITCH10]);
				action=true;
			}
		}
	}

	else {
		if( p.x>=0.138298 && p.x<=0.211856) {
			if(p.y>=0.619803 && p.y<=0.657381) {
				switch_state[SWITCH10]=1;
				action=true;
			}
		}
	}

	if(action) {
		UpdateVC();
		return true;
	}
	return false;
}

void PanelA8::Step(double t, double dt)
{
	if(switch_state[SWITCH10]==0) {
		double da = dt*SHOULDER_BRACE_SPEED;
		sts->shoulder_brace=max(0.0, sts->shoulder_brace-da);
		UpdateVC();
	}
	return;
}

bool PanelA8::ParseScenarioLine(char *line)
{
	if(!strnicmp(line, "STBD_RMS", 8)) {
		sscanf(line+8, "%d", &switch_state[SWITCH6]);
		/*if(switch_state[SWITCH6]==0 && sts->RMSRollout.action!=AnimState::OPEN) {
			sts->RMSRollout.action=AnimState::OPENING;
		}
		else if(switch_state[SWITCH6]==1 && sts->RMSRollout.Moving()) {
			sts->RMSRollout.action=AnimState::STOPPED;
		}
		else if(sts->RMSRollout.action!=AnimState::CLOSED) {
			sts->RMSRollout.action=AnimState::CLOSING;
		}*/
	}
	/*else if(!strnicmp(line, "SHOULDER_BRACE_RELEASE", 22)) {
		sscanf(line+22, "%d", &switch_state[SWITCH10]);
	}*/
	return false;
}

void PanelA8::SaveState(FILEHANDLE scn)
{
	oapiWriteScenario_int(scn, "STBD_RMS", switch_state[SWITCH6]);
	//oapiWriteScenario_int(scn, "SHOULDER_BRACE_RELEASE", switch_state[SWITCH10]);
	return;
}