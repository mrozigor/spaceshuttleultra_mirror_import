#include "PanelA8.h"
#include "PlBayOp.h"
#include "resource.h"
//#include "meshres.h"
//#include "meshres_vc.h"
#include "meshres_vc_a8.h"
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
	for(i=0;i<7;i++) {
		anim_VC_A8_cover[i]=NULL;
		cover_state[i]=1;
	}
}

void PanelA8::RegisterVC()
{
	if(!sts->RMS) return;
	sprintf_s(oapiDebugString(), 255, "Registering panelA8");
	VECTOR3 ofs=sts->orbiter_ofs;
	SURFHANDLE panela8t_tex = oapiGetTextureHandle (sts->hPanelA8Mesh, TEX_A8TOP_VC);
	//! TODO: Fix RMS panel animations, should be optional by now.
	SURFHANDLE panela8b_tex = oapiGetTextureHandle (sts->hPanelA8Mesh, TEX_A8BOTTOM_VC);

	oapiVCRegisterArea(AID_A8, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP);
	//oapiVCSetAreaClickmode_Quadrilateral (AID_A8, _V(-0.274, 2.848, 12.567)+ofs, _V(-0.813, 2.848, 12.567)+ofs, _V(-0.277, 2.055, 12.816)+ofs, _V(-0.819, 2.055, 12.816)+ofs);
	//oapiVCSetAreaClickmode_Quadrilateral (AID_A8, _V(-0.813, 2.848, 12.567)+ofs, _V(-0.274, 2.848, 12.567)+ofs, _V(-0.813, 2.3, 12.74)+ofs, _V(-0.274, 2.3, 12.74)+ofs);
	oapiVCSetAreaClickmode_Quadrilateral (AID_A8, _V(-0.797, 2.892, 12.279)+ofs, _V(-0.266, 2.892, 12.279)+ofs, _V(-0.797, 2.119, 12.522)+ofs, _V(-0.266, 2.119, 12.522)+ofs);
	
	oapiVCRegisterArea(AID_A8_TKBK1, _R(895, 900, 927, 918), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
	oapiVCRegisterArea(AID_A8_TKBK3, _R(895, 774, 927, 792), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
	oapiVCRegisterArea(AID_A8_TKBK5, _R(895, 647, 927, 665), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
	oapiVCRegisterArea(AID_A8_TKBK6, _R(806, 650, 838, 668), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
	oapiVCRegisterArea(AID_A8_TKBK7, _R(717, 648, 749, 666), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
	oapiVCRegisterArea(AID_A8_TKBK10, _R(120, 648, 152, 666), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
	oapiVCRegisterArea(AID_A8_TKBK11, _R(914, 334, 946, 352), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
	oapiVCRegisterArea(AID_A8_TKBK12, _R(914, 400, 946, 418), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
	oapiVCRegisterArea(AID_A8_TKBK13, _R(901, 931, 933, 949), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
	oapiVCRegisterArea(AID_A8_TKBK16, _R(868, 334, 900, 352), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
	oapiVCRegisterArea(AID_A8_TKBK17, _R(868, 400, 900, 418), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
	oapiVCRegisterArea(AID_A8_TKBK18, _R(819, 334, 851, 352), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
	oapiVCRegisterArea(AID_A8_TKBK19, _R(819, 400, 851, 418), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
}

bool PanelA8::VCRedrawEvent (int id, int event, SURFHANDLE surf)
{
	switch(id) {
		case AID_A8_TKBK1:
			/*if(sts->Eq(sts->arm_sy, 0.5) && sts->Eq(sts->arm_sp, sts->shoulder_neutral))
				return VCDrawTalkback(surf, id-AID_A8_TKBK1, 8);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);*/
			if(sts->MRL_FwdMicroswitches[0][2]==1)
				return VCDrawTalkback(surf, id-AID_A8_TKBK1, 8);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			//sprintf(oapiDebugString(), "AID_A8_TKBK1: %d", tkbk_state[id-AID_A8_TKBK1]);
			break;
		case AID_A8_TKBK3:
			/*if(sts->Eq(sts->arm_sy, 0.5) && sts->Eq(sts->arm_sp, sts->shoulder_neutral) && sts->Eq(sts->arm_ep, sts->elbow_neutral))
				return VCDrawTalkback(surf, id-AID_A8_TKBK1, 8);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);*/
			if(sts->MRL_MidMicroswitches[0][2]==1)
				return VCDrawTalkback(surf, id-AID_A8_TKBK1, 8);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			break;
		case AID_A8_TKBK5:
			/*if(sts->ArmCradled()) return VCDrawTalkback(surf, id-AID_A8_TKBK1, 8);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);*/
			if(sts->MRL_AftMicroswitches[0][2]==1)
				return VCDrawTalkback(surf, id-AID_A8_TKBK1, 8);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			break;
		case AID_A8_TKBK6:
			if(sts->MRL[0]==0.0) return VCDrawTalkback(surf, id-AID_A8_TKBK1, 10);
			else if(sts->MRL[0]==1.0) return VCDrawTalkback(surf, id-AID_A8_TKBK1, 2);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			break;
		case AID_A8_TKBK7:
			/*if(sts->RMSRollout.action==AnimState::OPEN)
				return VCDrawTalkback(surf, id-AID_A8_TKBK1, 9);
			else if(sts->RMSRollout.Moving()) return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			else if(sts->RMSRollout.action==AnimState::CLOSED)
				return VCDrawTalkback(surf, id-AID_A8_TKBK1, 1);*/
			if(sts->MPM_Microswitches[0][0]==1) return VCDrawTalkback(surf, id-AID_A8_TKBK1, 1);
			else if(sts->MPM_Microswitches[0][1]==1) return VCDrawTalkback(surf, id-AID_A8_TKBK1, 9);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			break;
		case AID_A8_TKBK10:
			if(sts->MPM_Microswitches[1][0]==1) return VCDrawTalkback(surf, id-AID_A8_TKBK1, 1);
			else if(sts->MPM_Microswitches[1][1]==1) return VCDrawTalkback(surf, id-AID_A8_TKBK1, 9);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			break;
		case AID_A8_TKBK11:
			//if(sts->Extend.Closed()) return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			//else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 9);
			if(sts->GetAttachmentStatus(sts->ahRMS)) 
				return VCDrawTalkback(surf, id-AID_A8_TKBK1, 8);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			break;
		case AID_A8_TKBK12:
			if(sts->Extend.Open()) return VCDrawTalkback(surf, id-AID_A8_TKBK1, 8);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			break;
		case AID_A8_TKBK13:
			if(sts->shoulder_brace==0.0 && switch_state[SWITCH11]==0)
				return VCDrawTalkback(surf, id-AID_A8_TKBK1, 8);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			break;
		case AID_A8_TKBK16:
			if(sts->Grapple.Closed()) return VCDrawTalkback(surf, id-AID_A8_TKBK1, 8);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			break;
		case AID_A8_TKBK17:
			if(sts->Grapple.Open()) return VCDrawTalkback(surf, id-AID_A8_TKBK1, 8);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			break;
		case AID_A8_TKBK18:
			if(sts->Rigidize.Closed()) return VCDrawTalkback(surf, id-AID_A8_TKBK1, 8);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			break;
		case AID_A8_TKBK19:
			if(sts->Rigidize.Open()) return VCDrawTalkback(surf, id-AID_A8_TKBK1, 8);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			break;
	}
	return false;
}

bool PanelA8::VCDrawTalkback(SURFHANDLE surf, int idx, int label)
{
	if (tkbk_state[idx] == label) return false; // nothing to do
	//sprintf(oapiDebugString(), "Panel A8 Talkback: %d %d %d", idx, label, tkbk_state[idx]);
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

	//PORT RMS RETENTION LATCHES
	static UINT VC_A8S18_GRP = GRP_A8S18_VC;
	static MGROUP_ROTATE VC_A8S18 (vcidx, &VC_A8S18_GRP, 1,
		_V(-0.693, 2.233, 12.486), switch_rot_vert, (float)(90.0*RAD));
	anim_VC_A8[SWITCH18]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent(anim_VC_A8[SWITCH18], 0, 1, &VC_A8S18);
	
	//PORT MPM DEPLOY
	static UINT VC_A8S17_GRP = GRP_A8S17_VC;
	static MGROUP_ROTATE VC_A8S17 (vcidx, &VC_A8S17_GRP, 1,
		_V(-0.647, 2.238, 12.486), switch_rot_vert, (float)(90.0*RAD));
	anim_VC_A8[SWITCH17]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent(anim_VC_A8[SWITCH17], 0, 1, &VC_A8S17);
	//PORT MPM DEPLOY switch cover
	static UINT VC_A8Scover5_GRP = GRP_A8Scover5_VC;
	static MGROUP_ROTATE VC_A8Scover5 (vcidx, &VC_A8Scover5_GRP, 1,
		_V(-0.646, 2.257, 12.478), switch_rot_vert, (float)(90.0*RAD));
	anim_VC_A8_cover[SWITCH5]=sts->CreateAnimation(1);
	sts->AddAnimationComponent(anim_VC_A8_cover[SWITCH5], 0, 1, &VC_A8Scover5);

	//RMS POWER
	static UINT VC_A8S16_GRP = GRP_A8S16_VC;
	static MGROUP_ROTATE VC_A8S16 (vcidx, &VC_A8S16_GRP, 1,
		_V(-0.647, 2.238, 12.486), switch_rot_vert, (float)(90.0*RAD));
	anim_VC_A8[SWITCH16]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent(anim_VC_A8[SWITCH16], 0, 1, &VC_A8S16);
	//RMS POWER switch cover
	static UINT VC_A8Scover4_GRP=GRP_A8Scover4_VC;
	static MGROUP_ROTATE VC_A8Scover4 (vcidx, &VC_A8Scover4_GRP, 1,
		_V(-0.537, 2.256, 12.478), switch_rot_vert, (float)(90.0*RAD));
	anim_VC_A8_cover[SWITCH4]=sts->CreateAnimation(1);
	sts->AddAnimationComponent(anim_VC_A8_cover[SWITCH4], 0, 1, &VC_A8Scover4);

	//STBD RMS RETENTION LATCHES
	static UINT VC_A8S13_GRP = GRP_A8S13_VC;
	static MGROUP_ROTATE VC_A8S13 (vcidx, &VC_A8S13_GRP, 1,
		_V(-0.384, 2.233, 12.487), switch_rot_vert, (float)(90.0*RAD));
	anim_VC_A8[SWITCH13]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent(anim_VC_A8[SWITCH13], 0, 1, &VC_A8S13);

	//STBD MPM DEPLOY
	static UINT VC_A8S12_GRP = GRP_A8S12_VC;
	static MGROUP_ROTATE VC_A8S12 (vcidx, &VC_A8S12_GRP, 1,
		_V(-0.693, 2.233, 12.486), switch_rot_vert, (float)(90.0*RAD));
	anim_VC_A8[SWITCH12]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent(anim_VC_A8[SWITCH12], 0, 1, &VC_A8S12);
	//STBD MPM DEPLOY switch cover
	static UINT VC_A8Scover1_GRP=GRP_A8Scover1_VC;
	static MGROUP_ROTATE VC_A8Scover1 (vcidx, &VC_A8Scover1_GRP, 1,
		_V(-0.339, 2.257, 12.479), switch_rot_vert, (float)(90.0*RAD));
	anim_VC_A8_cover[SWITCH1]=sts->CreateAnimation(1);
	sts->AddAnimationComponent(anim_VC_A8_cover[SWITCH1], 0, 1, &VC_A8Scover1);

	//SHOULDER BRACE
	static UINT VC_A8S10_GRP = GRP_A8S10_VC;
	static MGROUP_ROTATE VC_A8S10 (vcidx, &VC_A8S10_GRP, 1,
		_V(-0.687, 2.402, 12.434), switch_rot_horz, (float)(90.0*RAD));
	anim_VC_A8[SWITCH10]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent(anim_VC_A8[SWITCH10], 0, 1, &VC_A8S10);

	//RMS SELECT
	static UINT VC_A8S11_GRP = GRP_A8S11_VC;
	static MGROUP_ROTATE VC_A8S11 (vcidx, &VC_A8S11_GRP, 1,
		_V(-0.537, 2.294, 12.467), switch_rot_horz, (float)(90.0*RAD));
	anim_VC_A8[SWITCH11]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent(anim_VC_A8[SWITCH11], 0, 1, &VC_A8S11);

	//MAN CONTR
	static UINT VC_A8S5_GRP = GRP_A8S5_VC;
	static MGROUP_ROTATE VC_A8S5 (vcidx, &VC_A8S5_GRP, 1,
		_V(-0.654, 2.709, 12.337), switch_rot_vert, (float)(90.0*RAD));
	anim_VC_A8[SWITCH5]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent(anim_VC_A8[SWITCH5], 0, 1, &VC_A8S5);

	//MODE
	static UINT VC_A8S4_GRP = GRP_A8S4_VC;
	static MGROUP_ROTATE VC_A8S4 (vcidx, &VC_A8S4_GRP, 1,
		_V(-0.625, 2.709, 12.337), switch_rot_vert, (float)(90.0*RAD));
	anim_VC_A8[SWITCH4]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent(anim_VC_A8[SWITCH4], 0, 1, &VC_A8S4);
}

void PanelA8::UpdateVC()
{
	if(!(sts->RMS)) return;
	sts->SetAnimation(anim_VC_A8[SWITCH18], switch_state[SWITCH18]/2.0);
	sts->SetAnimation(anim_VC_A8[SWITCH17], switch_state[SWITCH17]/2.0);
	sts->SetAnimation(anim_VC_A8[SWITCH16], switch_state[SWITCH16]/2.0);
	sts->SetAnimation(anim_VC_A8[SWITCH13], switch_state[SWITCH13]/2.0);
	sts->SetAnimation(anim_VC_A8[SWITCH12], switch_state[SWITCH12]/2.0);
	sts->SetAnimation(anim_VC_A8[SWITCH11], switch_state[SWITCH11]/2.0);
	sts->SetAnimation(anim_VC_A8[SWITCH10], switch_state[SWITCH10]/2.0);
	sts->SetAnimation(anim_VC_A8[SWITCH5], switch_state[SWITCH5]/2.0);
	sts->SetAnimation(anim_VC_A8[SWITCH4], switch_state[SWITCH4]/2.0);

	sts->SetAnimation(anim_VC_A8_cover[SWITCH1], cover_state[SWITCH1]);
	sts->SetAnimation(anim_VC_A8_cover[SWITCH4], cover_state[SWITCH4]);
	sts->SetAnimation(anim_VC_A8_cover[SWITCH5], cover_state[SWITCH5]);
	if(anim_VC_A8_cover[SWITCH4]) {
		char cbuf[255];
		sprintf_s(cbuf, 255, "Cover positions updated %f", cover_state[SWITCH4]);
		oapiWriteLog(cbuf);
	}
	
	oapiVCTriggerRedrawArea(-1, AID_A8_TKBK1);
	oapiVCTriggerRedrawArea(-1, AID_A8_TKBK3);
	oapiVCTriggerRedrawArea(-1, AID_A8_TKBK5);
	oapiVCTriggerRedrawArea(-1, AID_A8_TKBK6);
	oapiVCTriggerRedrawArea(-1, AID_A8_TKBK7);
	oapiVCTriggerRedrawArea(-1, AID_A8_TKBK10);
	oapiVCTriggerRedrawArea(-1, AID_A8_TKBK11);
	oapiVCTriggerRedrawArea(-1, AID_A8_TKBK12);
	oapiVCTriggerRedrawArea(-1, AID_A8_TKBK13);
	oapiVCTriggerRedrawArea(-1, AID_A8_TKBK16);
	oapiVCTriggerRedrawArea(-1, AID_A8_TKBK17);
	oapiVCTriggerRedrawArea(-1, AID_A8_TKBK18);
	oapiVCTriggerRedrawArea(-1, AID_A8_TKBK19);
}

bool PanelA8::VCMouseEvent(int id, int nEvent, VECTOR3 &p)
{
	bool action = false;
	sprintf_s(oapiDebugString(), 255, "Panel A8 event: X: %f Y: %f Z: %f", p.x, p.y, p.z);
	/*if(event & PANEL_MOUSE_LBPRESSED) sprintf(oapiDebugString(), "LBPressed");
	else if(event == PANEL_MOUSE_LBUP) sprintf(oapiDebugString(), "LBUp");
	else if(event & PANEL_MOUSE_LBDOWN) sprintf(oapiDebugString(), "LBDown");*/

	if(nEvent == PANEL_MOUSE_LBDOWN) {
		if(p.x>=0.167373 && p.x<=0.217567) {
			if(p.y>=0.841234 && p.y<=0.867132) {
				if(p.y<0.854145) {
					if(switch_state[SWITCH18]>0) switch_state[SWITCH18]--;
				}
				else {
					if(switch_state[SWITCH18]<2) switch_state[SWITCH18]++;
				}
				action=true;
			}
		}

		if(p.x>=0.252639 && p.x<=0.305916) {
			if(p.y>=0.828852 && p.y<=0.863428) {
				if(p.y<0.848199) {
					sprintf(oapiDebugString(), "Deploying STBD MPMs");
					if(switch_state[SWITCH17]>0) switch_state[SWITCH17]--;
				}
				else {
					sprintf(oapiDebugString(), "Stowing STBD MPMs");
					if(switch_state[SWITCH17]<2) switch_state[SWITCH17]++;
				}
				if(switch_state[SWITCH17]==0) {
					if(sts->RMSRollout.action!=AnimState::OPEN) {
						sts->RMSRollout.action=AnimState::OPENING;
					}
				}
				else if(switch_state[SWITCH17]==1) {
					if(sts->RMSRollout.Moving()) sts->RMSRollout.action=AnimState::STOPPED;
				}
				else {
					if(sts->RMSRollout.action!=AnimState::CLOSED) {
						sts->RMSRollout.action=AnimState::CLOSING;
					}
				}
				action=true;
			}
		}

		if(p.x>=0.4649455 && p.x<=0.508725) {
			//if(p.y>=0.8319185 && p.y<=0.877751) {
			if(p.y>=0.8375 && p.y<=0.877751) {
				if(cover_state[SWITCH4]==0) { //if cover is open, change switch position
					if(p.y<0.854939) {
						if(switch_state[SWITCH16]>0) {
							switch_state[SWITCH16]--;
							action=true;
						}
					}
					else {
						if(switch_state[SWITCH16]<2) {
							switch_state[SWITCH16]++;
							action=true;
						}
					}
				}
				else { //lift switch cover
					sprintf_s(oapiDebugString(), 255, "RMS POWER switch cover raised");
					cover_state[SWITCH4]=0;
					action=true;
				}
			}
		}
		if(cover_state[SWITCH4]==0 && p.x>=0.454365 && p.x<=0.504558) {
			if(p.y>=0.808621 && p.y<=0.834) {
				cover_state[SWITCH4]=1;
				action=true;
			}
		}

		if(p.x>=0.755850 && p.x<=0.801863) {
			if(p.y>=0.842015 && p.y<=0.880786) {
				if(p.y<0.861401) {
					if(switch_state[SWITCH13]>0) {
						switch_state[SWITCH13]--;
						action=true;
					}
				}
				else {
					if (switch_state[SWITCH13]<2) {
						switch_state[SWITCH13]++;
						action=true;
					}
				}
			}
		}

		if(p.x>=0.841143 && p.x<=0.886384) {
			if(cover_state[SWITCH1]==0) {
				if(p.y>=0.836290 && p.y<=0.867186) { //switch clicked on
					if(p.y<0.848995) {
						if(switch_state[SWITCH12]>0) {
							switch_state[SWITCH12]--;
							action=true;
						}
					}
					else {
						if(switch_state[SWITCH12]<2) {
							switch_state[SWITCH12]++;
							action=true;
						}
					}

					if(switch_state[SWITCH12]==0) {
						if(sts->StbdMPMRollout.action!=AnimState::OPEN) {
							sts->StbdMPMRollout.action=AnimState::OPENING;
						}
					}
					else if(switch_state[SWITCH12]==1) {
						if(sts->StbdMPMRollout.Moving()) sts->StbdMPMRollout.action=AnimState::STOPPED;
					}
					else {
						if(sts->StbdMPMRollout.action!=AnimState::CLOSED) {
							sts->StbdMPMRollout.action=AnimState::CLOSING;
						}
					}
				}
				else if(p.y>=0.808047 && p.y<=0.836290) { //cover clicked on
					cover_state[SWITCH1]=1;
					action=true;
				}
			}
			else if(p.y>=0.829965 && p.y<=0.867186) {
				cover_state[SWITCH1]=0;
				action=true;
			}
		}

		if(p.x>=0.170449 && p.x<=0.226497) {
			if(p.y>=0.618925 && p.y<=0.651137) {
				if(p.x<0.200508) {
					if(switch_state[SWITCH10]>0) switch_state[SWITCH10]--;
				}
				else {
					if(switch_state[SWITCH10]<2) switch_state[SWITCH10]++;
				}
				sprintf_s(oapiDebugString(), 255, "%d", switch_state[SWITCH10]);
				action=true;
			}
		}

		if(p.x>=0.466754 && p.x<=0.510409) {
			if(p.y>=0.757090 && p.y<=0.788049) {
				if(p.x<0.486494) {
					if(switch_state[SWITCH11]>0) switch_state[SWITCH11]--;
				}
				else {
					if(switch_state[SWITCH11]<2) switch_state[SWITCH11]++;
				}
				action=true;
			}
		}

		if(p.x>=0.243867 && p.x<=0.288102) {
			if(p.y>=0.22259 && p.y<=0.252219) {
				if(p.y<0.2374045) {
					if(switch_state[SWITCH5]>0) switch_state[SWITCH5]--;
					sprintf_s(oapiDebugString(), 255, "SWITCH5");
				}
				else {
					if(switch_state[SWITCH5]<2) switch_state[SWITCH5]++;
				}
				action=true;
			}
		}

		if(p.x>=0.298233 && p.x<=0.346051) {
			if(p.y>=0.2227635 && p.y<=0.250502) {
				if(p.y<0.23663275) {
					if(switch_state[SWITCH4]>0) switch_state[SWITCH4]--;
					//sprintf_s(oapiDebugString(), 255, "SWITCH16");
				}
				else {
					if(switch_state[SWITCH4]<2) switch_state[SWITCH4]++;
				}
				if(switch_state[SWITCH4]==1) {
					sts->EEGrappleMode=0;
					//stop grapple/release sequences
					if(sts->bGrappleInProgress) sts->AutoGrappleSequence();
					else if(sts->bReleaseInProgress) sts->AutoReleaseSequence();
				}
				else if(switch_state[SWITCH4]==2) sts->EEGrappleMode=1;
				else sts->EEGrappleMode=2;
				action=true;
			}
		}
	}

	else { //reset spring-loaded switches
		if(p.x>=0.170449 && p.x<=0.226497) {
			if(p.y>=0.618925 && p.y<=0.651137) {
				switch_state[SWITCH10]=1;
				action=true;
			}
		}

		if(p.x>=0.243867 && p.x<=0.288102) {
			if(p.y>=0.22259 && p.y<=0.252219) {
				switch_state[SWITCH5]=1;
				action=true;
			}
		}
	}

	if(action) {
		//sprintf_s(oapiDebugString(), 255, "Updating PanelA8");
		//oapiWriteLog(oapiDebugString());
		sprintf_s(oapiDebugString(), 255, "SWITCH16: %d %d", switch_state[SWITCH16], cover_state[SWITCH4]);
		oapiWriteLog(oapiDebugString());
		UpdateVC();
		return true;
	}
	return false;
}

void PanelA8::Step(double t, double dt)
{
	//if(!sts->RMS) return;
	if(switch_state[SWITCH18]==0 && sts->ArmCradled() && switch_state[SWITCH16]!=1) {
		double da = dt*SHOULDER_BRACE_SPEED;
		sts->MRL[0]=max(0.0, sts->MRL[0]-da);
		sts->UpdateMRLMicroswitches();
		UpdateVC();
	}
	else if(switch_state[SWITCH18]==2 && sts->ArmCradled() && switch_state[SWITCH16]!=1) {
		double da = dt*SHOULDER_BRACE_SPEED;
		sts->MRL[0]=min(1.0, sts->MRL[0]+da);
		sts->UpdateMRLMicroswitches();
		UpdateVC();
	}

	if(switch_state[SWITCH10]==0) {
		double da = dt*SHOULDER_BRACE_SPEED;
		sts->shoulder_brace=max(0.0, sts->shoulder_brace-da);
		UpdateVC();
	}
	
	if(switch_state[SWITCH4]==2 && switch_state[SWITCH5]==0) {
		//rigidize
		if(!sts->Extend.Closed()) {
			sts->Extend.action=AnimState::CLOSING;
			sts->Extend.Move(dt*ARM_EXTEND_SPEED);
			if(sts->Extend.Closed()) {
				sts->Rigidize.action=AnimState::CLOSING;
			}
			UpdateVC();
		}
		else if(!sts->Rigidize.Closed()) {
			sts->Rigidize.action=AnimState::CLOSING;
			sts->Rigidize.Move(dt*ARM_RIGID_SPEED);
			if(sts->Rigidize.Closed()) {
				sts->bGrappleInProgress=false;
				sprintf_s(oapiDebugString(), 255, "Rigidize sequence completed");
				UpdateVC();
			}
		}
	}
	else if(switch_state[SWITCH4]==2 && switch_state[SWITCH5]==2) {
		//derigidize
		if(!sts->Rigidize.Open()) {
			sts->Rigidize.action=AnimState::OPENING;
			sts->Rigidize.Move(dt*ARM_RIGID_SPEED);
			if(sts->Rigidize.Open()) {
				sts->Extend.action=AnimState::OPENING;
				UpdateVC();
			}
		}
		else if(!sts->Extend.Open()) {
			sts->Extend.action=AnimState::OPENING;
			sts->Extend.Move(dt*ARM_EXTEND_SPEED);
			if(sts->Extend.Open()) {
				sts->bReleaseInProgress=false;
				sprintf_s(oapiDebugString(), 255, "Derigidize sequence completed");
			}
			UpdateVC();
		}
	}
}

bool PanelA8::ParseScenarioLine(char *line)
{
	if(!_strnicmp(line, "EE MODE", 7)) {
		sscanf_s(line+7, "%d", &switch_state[SWITCH4]);
		if(switch_state[SWITCH4]==1) sts->EEGrappleMode=0;
		else if(switch_state[SWITCH4]==2) sts->EEGrappleMode=1;
		else sts->EEGrappleMode=2;
		return true;
	}
	if(!_strnicmp(line, "SWITCH_RMS_POWER", 16)) {
		sscanf_s(line+16, "%d %d", &switch_state[SWITCH16], &cover_state[SWITCH4]);
		//sprintf_s(oapiDebugString(), 255, "RMS POWER switch updated %d", switch_state[SWITCH16]);
		//oapiWriteLog(oapiDebugString());
		return true;
	}
	/*else if(!strnicmp(line, "SHOULDER_BRACE_RELEASE", 22)) {
		sscanf(line+22, "%d", &switch_state[SWITCH10]);
	}*/
	return false;
}

void PanelA8::SaveState(FILEHANDLE scn)
{
	char cbuf[255];
	oapiWriteScenario_int(scn, "EE MODE", switch_state[SWITCH4]);
	sprintf_s(cbuf, 255, "%d %d", switch_state[SWITCH16], cover_state[SWITCH4]);
	oapiWriteScenario_string(scn, "SWITCH_RMS_POWER", cbuf);
	//oapiWriteScenario_int(scn, "SHOULDER_BRACE_RELEASE", switch_state[SWITCH10]);
	return;
}
