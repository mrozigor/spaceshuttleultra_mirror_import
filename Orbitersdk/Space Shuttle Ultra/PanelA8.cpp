#include "PanelA8.h"
#include "PlBayOp.h"
#include "meshres_vc_a8.h"
#include "meshres_vc_additions.h"
#include "RMSSystem.h"
#include <stdio.h>

extern GDIParams g_Param;

// ==============================================================

PanelA8::PanelA8(Atlantis *_sts): sts(_sts)
{
	int i;
	for(i=0;i<20;i++) {
		anim_VC_A8[i]=NULL;
		switch_state[i]=1;
		tkbk_state[0][i]=-1;
		tkbk_state[1][i]=-1;
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
	
	// PORT MPM RTL
	oapiVCRegisterArea(AID_A8_TKBK1, _R(895, 894, 922, 922), PANEL_REDRAW_ALWAYS, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
	oapiVCRegisterArea(AID_A8_TKBK3, _R(895, 769, 922, 797), PANEL_REDRAW_ALWAYS, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
	oapiVCRegisterArea(AID_A8_TKBK5, _R(895, 642, 922, 670), PANEL_REDRAW_ALWAYS, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
	// STBD MPM RTL
	oapiVCRegisterArea(AID_A8_TKBK2, _R(297, 896, 324, 924), PANEL_REDRAW_ALWAYS, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
	oapiVCRegisterArea(AID_A8_TKBK4, _R(297, 770, 324, 798), PANEL_REDRAW_ALWAYS, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
	oapiVCRegisterArea(AID_A8_TKBK8, _R(297, 643, 324, 671), PANEL_REDRAW_ALWAYS, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
	// PORT MPM LATCH
	oapiVCRegisterArea(AID_A8_TKBK6, _R(806, 650, 838, 668), PANEL_REDRAW_ALWAYS, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
	// PORT MPM DPY
	oapiVCRegisterArea(AID_A8_TKBK7, _R(717, 648, 749, 666), PANEL_REDRAW_ALWAYS, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
	// STBD MPM LATCH
	oapiVCRegisterArea(AID_A8_TKBK9, _R(210, 651, 242, 669), PANEL_REDRAW_ALWAYS, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
	// STBD MPM DPY
	oapiVCRegisterArea(AID_A8_TKBK10, _R(120, 648, 152, 666), PANEL_REDRAW_ALWAYS, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
	// EE CAPTURE
	oapiVCRegisterArea(AID_A8_TKBK11, _R(917, 326, 944, 354), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
	// EE EXTEND
	oapiVCRegisterArea(AID_A8_TKBK12, _R(917, 392, 944, 420), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
	// SHOULDER BRACE
	oapiVCRegisterArea(AID_A8_TKBK13, _R(901, 931, 933, 949), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
	// EE CLOSE
	oapiVCRegisterArea(AID_A8_TKBK16, _R(869, 327, 896, 355), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
	// EE OPEN
	oapiVCRegisterArea(AID_A8_TKBK17, _R(869, 391, 896, 419), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
	// EE RIGID
	oapiVCRegisterArea(AID_A8_TKBK18, _R(821, 327, 848, 355), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
	// EE DERIGID
	oapiVCRegisterArea(AID_A8_TKBK19, _R(821, 392, 848, 420), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
}

void PanelA8::Realize()
{
	DiscreteBundle* pBundle=sts->BundleManager()->CreateBundle("RMS", 16);
	PortMPMDeploy.Connect(pBundle, 0);
	PortMPMStow.Connect(pBundle, 1);
	PortMPMDeployed.Connect(pBundle, 2);
	PortMPMStowed.Connect(pBundle, 3);

	pBundle=sts->BundleManager()->CreateBundle("RMS_MRL", 16);
	PortMRLRelease.Connect(pBundle, 0);
	PortMRLLatch.Connect(pBundle, 1);
	for(int i=0;i<3;i++) PortMRL_RTL[i].Connect(pBundle, i+5);
	PortMRL_Released.Connect(pBundle, 11);
	PortMRL_Latched.Connect(pBundle, 12);

	pBundle=sts->BundleManager()->CreateBundle("STBD_MPM", 16);
	StbdMPMDeploy.Connect(pBundle, 0);
	StbdMPMStow.Connect(pBundle, 1);
	StbdMPMDeployed.Connect(pBundle, 2);
	StbdMPMStowed.Connect(pBundle, 3);

	pBundle=sts->BundleManager()->CreateBundle("STBD_MPM_MRL", 16);
	StbdMRLRelease.Connect(pBundle, 0);
	StbdMRLLatch.Connect(pBundle, 1);
	for(int i=0;i<3;i++) StbdMRL_RTL[i].Connect(pBundle, i+5);
	StbdMRL_Released.Connect(pBundle, 11);
	StbdMRL_Latched.Connect(pBundle, 12);
}

bool PanelA8::VCRedrawEvent (int id, int _event, SURFHANDLE surf)
{
	//bool bUpper, bLower;
	switch(id) {
		// PORT MPM FWD RTL
		case AID_A8_TKBK1:
			if(PortMRL_RTL[0]) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_GRAY);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_BARBERPOLE);
			break;
		// STBD MPM FWD RTL
		case AID_A8_TKBK2:
			if(StbdMRL_RTL[0]) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_GRAY);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_BARBERPOLE);
			break;
		// PORT MPM MID RTL
		case AID_A8_TKBK3:
			if(PortMRL_RTL[1]) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_GRAY);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_BARBERPOLE);
			break;
		// STBD MPM MID RTL
		case AID_A8_TKBK4:
			if(StbdMRL_RTL[1]) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_GRAY);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_BARBERPOLE);
			break;
		// PORT MPM AFT RTL
		case AID_A8_TKBK5:
			if(PortMRL_RTL[2]) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_GRAY);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_BARBERPOLE);
			break;
		case AID_A8_TKBK6:
			if(PortMRL_Released) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_REL);
			else if(PortMRL_Latched) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_LAT);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			break;
		case AID_A8_TKBK7:
			if(PortMPMStowed) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_STO);
			else if(PortMPMDeployed) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_DPY);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			return true;
			break;
		// STBD MPM AFT RTL
		case AID_A8_TKBK8:
			if(StbdMRL_RTL[2]) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_GRAY);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_BARBERPOLE);
			break;
		// STBD MPM LATCH
		case AID_A8_TKBK9:
			if(StbdMRL_Released) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_REL);
			else if(StbdMRL_Latched) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_LAT);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
		case AID_A8_TKBK10:
			if(StbdMPMStowed) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_STO);
			else if(StbdMPMDeployed) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_DPY);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, 0);
			break;
		//CAPTURE
		case AID_A8_TKBK11:
			if(sts->pRMS && sts->pRMS->Grappled()) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_GRAY);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_BARBERPOLE);
			break;
		//EXTEND
		case AID_A8_TKBK12:
			if(sts->Extend.Open()) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_GRAY);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_BARBERPOLE);
			break;
		//SHOULDER BRACE
		case AID_A8_TKBK13:
			if(sts->shoulder_brace==0.0 && switch_state[SWITCH11]==0)
				return VCDrawTalkback(surf, id-AID_A8_TKBK1, 8);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_BARBERPOLE);
			break;
		//CLOSE
		case AID_A8_TKBK16:
			if(sts->Grapple.Closed()) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_GRAY);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_BARBERPOLE);
			break;
		//OPEN
		case AID_A8_TKBK17:
			if(sts->Grapple.Open()) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_GRAY);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_BARBERPOLE);
			break;
		//RIGID
		case AID_A8_TKBK18:
			if(sts->Rigidize.Closed()) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_GRAY);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_BARBERPOLE);
		//DERIGID
		case AID_A8_TKBK19:
			if(sts->Rigidize.Open()) return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_GRAY);
			else return VCDrawTalkback(surf, id-AID_A8_TKBK1, vc::TB_GRAY, vc::TB_BARBERPOLE);
	}
	return false;
}

bool PanelA8::VCDrawTalkback(SURFHANDLE surf, int idx, int label)
{
	if (tkbk_state[0][idx] == label) return false; // nothing to do
	//sprintf(oapiDebugString(), "Panel A8 Talkback: %d %d %d", idx, label, tkbk_state[idx]);
	tkbk_state[0][idx] = label;
	//if(label<=1) oapiBlt (surf, g_Param.tkbk_label, 0, 0, 0, label*18, 32, 18);
	//else oapiBlt (surf, g_Param.tkbk_label, 0, 0, (label-1)*32, 0, 32, 18);
	//oapiBlt(surf, g_Param.tkbk_label, 0, 0, 0, 0, 32, 18);
	if(label<8) oapiBlt (surf, g_Param.tkbk_label, 0, 0, label*32, 0, 32, 18);
	else oapiBlt (surf, g_Param.tkbk_label, 0, 0, (label-8)*32, 18, 32, 18);
	return true;
}

bool PanelA8::VCDrawTalkback(SURFHANDLE surf, int idx, int upper_label, int lower_label)
{
	RECT tgt, src;

	if (tkbk_state[0][idx] == upper_label && tkbk_state[1][idx] == lower_label) return false; // nothing to do
	tkbk_state[0][idx] = upper_label;
	tkbk_state[1][idx] = lower_label;

	//draw upper segment
	tgt=_R(0, 0, 27, 14);
	if(upper_label<8) src=_R(upper_label*32, 0, (upper_label+1)*32, 18);
	else src=_R((upper_label-8)*32, 18, (upper_label-7)*32, 36);
	oapiBlt(surf, g_Param.tkbk_label, &tgt, &src);

	//draw lower segment
	tgt=_R(0, 14, 27, 28);
	if(lower_label<8) src=_R(lower_label*32, 0, (lower_label+1)*32, 18);
	else src=_R((lower_label-8)*32, 18, (lower_label-7)*32, 36);
	oapiBlt(surf, g_Param.tkbk_label, &tgt, &src);
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
				if(switch_state[SWITCH18]==0) {
					PortMRLRelease.SetLine();
					PortMRLLatch.ResetLine();
				}
				else if(switch_state[SWITCH18]==1) {
					PortMRLRelease.ResetLine();
					PortMRLLatch.ResetLine();
				}
				else {
					PortMRLRelease.ResetLine();
					PortMRLLatch.SetLine();
				}
				action=true;
			}
		}

		if(p.x>=0.252639 && p.x<=0.305916) {
			if(cover_state[SWITCH5]==0) {
				if(p.y>=0.834143 && p.y<=0.863428) {
					if(p.y<0.848199) {
						if(switch_state[SWITCH17]>0) switch_state[SWITCH17]--;
					}
					else {
						if(switch_state[SWITCH17]<2) switch_state[SWITCH17]++;
					}
					if(switch_state[SWITCH17]==0) {
						/*if(sts->RMSRollout.action!=AnimState::OPEN) {
							sts->RMSRollout.action=AnimState::OPENING;
						}*/
						PortMPMDeploy.SetLine();
						PortMPMStow.ResetLine();
					}
					else if(switch_state[SWITCH17]==1) {
						//if(sts->RMSRollout.Moving()) sts->RMSRollout.action=AnimState::STOPPED;
						PortMPMDeploy.ResetLine();
						PortMPMStow.ResetLine();
					}
					else {
						/*if(sts->RMSRollout.action!=AnimState::CLOSED) {
							sts->RMSRollout.action=AnimState::CLOSING;
						}*/
						PortMPMDeploy.ResetLine();
						PortMPMStow.SetLine();
						//oapiWriteLog("Stowing port MPMs");
					}
					action=true;
				}
			}
			else if(p.y>=0.828852 && p.y<=0.863428) {
				cover_state[SWITCH5]=0;
				action=true;
			}
		}
		if(p.x>=0.235794 && p.x<=0.298074) {
			if(p.y>=0.804317 && p.y<=0.834143 && cover_state[SWITCH5]==0) {
				cover_state[SWITCH5]=1;
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
				if(switch_state[SWITCH13]==0) {
					oapiWriteLog("Stbd MRL Release set.");
					StbdMRLRelease.SetLine();
					StbdMRLLatch.ResetLine();
				}
				else if(switch_state[SWITCH13]==1) {
					StbdMRLRelease.ResetLine();
					StbdMRLLatch.ResetLine();
				}
				else {
					StbdMRLRelease.ResetLine();
					StbdMRLLatch.SetLine();
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
						StbdMPMDeploy.SetLine();
						StbdMPMStow.ResetLine();
					}
					else if(switch_state[SWITCH12]==1) {
						StbdMPMDeploy.ResetLine();
						StbdMPMStow.ResetLine();
					}
					else {
						StbdMPMDeploy.ResetLine();
						StbdMPMStow.SetLine();
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
		UpdateVC();
		return true;
	}
	return false;
}

void PanelA8::Step(double t, double dt)
{
	//if(!sts->RMS) return;
	bool bUpdate=false;
	/*if(switch_state[SWITCH18]==0 && sts->ArmCradled() && switch_state[SWITCH16]!=1) {
		double da = dt*MPM_MRL_SPEED;
		sts->MRL[0]=max(0.0, sts->MRL[0]-da);
		sts->UpdateMRLMicroswitches();
		bUpdate=true;
	}
	else if(switch_state[SWITCH18]==2 && sts->ArmCradled() && switch_state[SWITCH16]!=1) {
		double da = dt*MPM_MRL_SPEED;
		sts->MRL[0]=min(1.0, sts->MRL[0]+da);
		sts->UpdateMRLMicroswitches();
		bUpdate=true;
	}*/

	/*if(switch_state[SWITCH13]==0 && switch_state[SWITCH16]!=1) {
		if(!Eq(sts->MRL[1], 0.00000)) {
			double da = dt*MPM_MRL_SPEED;
			sts->MRL[1]=max(0.0, sts->MRL[1]-da);
			sts->UpdateMRLMicroswitches();
			if(Eq(sts->MRL[1], 0.0)) sts->DetachOBSS();
			bUpdate=true;
		}
	}
	else if(switch_state[SWITCH13]==2 && switch_state[SWITCH16]!=1) {
		if(!Eq(sts->MRL[1], 1.0000)) {
			double da = dt*MPM_MRL_SPEED;
			sts->MRL[1]=min(1.0, sts->MRL[1]+da);
			sts->UpdateMRLMicroswitches();
			if(Eq(sts->MRL[1], 1.00)) sts->AttachOBSS();
			bUpdate=true;
		}
	}*/

	if(switch_state[SWITCH10]==0) {
		double da = dt*SHOULDER_BRACE_SPEED;
		sts->shoulder_brace=max(0.0, sts->shoulder_brace-da);
		bUpdate=true;
	}
	
	if(switch_state[SWITCH4]==2 && switch_state[SWITCH5]==0) {
		//rigidize
		if(!sts->Extend.Closed()) {
			sts->Extend.action=AnimState::CLOSING;
			sts->Extend.Move(dt*ARM_EXTEND_SPEED);
			if(sts->Extend.Closed()) {
				sts->Rigidize.action=AnimState::CLOSING;
			}
			bUpdate=true;
		}
		else if(!sts->Rigidize.Closed()) {
			sts->Rigidize.action=AnimState::CLOSING;
			sts->Rigidize.Move(dt*ARM_RIGID_SPEED);
			if(sts->Rigidize.Closed()) {
				sts->bGrappleInProgress=false;
				sprintf_s(oapiDebugString(), 255, "Rigidize sequence completed");
				bUpdate=true;
			}
		}
	}
	else if(switch_state[SWITCH4]==2 && switch_state[SWITCH5]==2) {
		//derigidize
		if(!sts->Rigidize.Open()) {
			if(sts->Rigidize.Closed()) bUpdate=true;
			sts->Rigidize.action=AnimState::OPENING;
			sts->Rigidize.Move(dt*ARM_RIGID_SPEED);
			if(sts->Rigidize.Open()) {
				sts->Extend.action=AnimState::OPENING;
				bUpdate=true;
			}
		}
		else if(!sts->Extend.Open()) {
			sts->Extend.action=AnimState::OPENING;
			sts->Extend.Move(dt*ARM_EXTEND_SPEED);
			if(sts->Extend.Open()) {
				sts->bReleaseInProgress=false;
				sprintf_s(oapiDebugString(), 255, "Derigidize sequence completed");
			}
			bUpdate=true;
		}
	}

	if(bUpdate) UpdateVC();
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
		return true;
	}
	if(!_strnicmp(line, "SWITCH_MPM_DPLY", 15)) {
		sscanf_s(line+15, "%d %d %d %d", &switch_state[SWITCH17], &cover_state[SWITCH5], &switch_state[SWITCH12], &cover_state[SWITCH1]);
	}
	if(!_strnicmp(line, "SWITCH_RMS_RET_LATCH", 20)) {
		sscanf_s(line+20, "%d %d", &switch_state[SWITCH13], &switch_state[SWITCH18]);
	}
	return false;
}

void PanelA8::SaveState(FILEHANDLE scn)
{
	char cbuf[255];
	
	sprintf_s(cbuf, 255, "%d %d %d %d", switch_state[SWITCH17], cover_state[SWITCH5], switch_state[SWITCH12], cover_state[SWITCH1]);
	oapiWriteScenario_string(scn, "SWITCH_MPM_DPLY", cbuf);
	
	sprintf_s(cbuf, 255, "%d %d", switch_state[SWITCH16], cover_state[SWITCH4]);
	oapiWriteScenario_string(scn, "SWITCH_RMS_POWER", cbuf);
	
	sprintf_s(cbuf, 255, "%d %d", switch_state[SWITCH13], switch_state[SWITCH18]);
	oapiWriteScenario_string(scn, "SWITCH_RMS_RET_LATCH", cbuf);
	
	oapiWriteScenario_int(scn, "EE MODE", switch_state[SWITCH4]);
	
	return;
}