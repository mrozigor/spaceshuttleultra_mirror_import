#include "PanelC2.h"
#include "resource.h"
#include "meshres.h"
#include "meshres_vc.h"
#include "meshres_vc_additions.h"
#include "DlgCtrl.h"
#include <stdio.h>
#include "dps/MasterTimingUnit.h"
#include "Atlantis_defs.h"

extern GDIParams g_Param;
extern HELPCONTEXT g_hc;
extern char *ActionString[5];

const double EVTTMR_WHEELMAX[4] = {3.0, 5.0, 3.0, 5.0};
const UINT EVTTMR_WINDOW_GROUPS[4] = {211, 212, 213, 214};

// ==============================================================

PanelC2::PanelC2(Atlantis *_sts): sts(_sts) {
	int i;
	for(i = 0; i<4; i++)
	{
		tgtwheel_state[i] = 0;
		wheelState[i] = 0.0;
		wheelnumber[i] = 0;
		oldwheelnumber[i] = 9;
	}

	
	
	for(i = 0; i<12; i++)
	{
		anim_VC_C2[i] = NULL;
		switch_state[i] = 1;
		old_switch_state[i] = 1;
	}
	
}

PanelC2::~PanelC2()
{
}

bool PanelC2::VCRedrawEvent(int id, int event, SURFHANDLE surf)
{

	const int NUMX[10] = {0, 64, 128, 192, 0, 64, 128, 192, 0, 64};
	const int NUMY[10] = {0, 0, 0, 0, 64, 64, 64, 64, 128, 128};
	
	switch(id)
	{
	case AID_C2_WND0:
		oapiBlt(surf, g_Param.clock_digits, 0,0, NUMX[wheelnumber[0]], NUMY[wheelnumber[0]], 63, 63);
		return true;
	case AID_C2_WND1:
		oapiBlt(surf, g_Param.clock_digits, 0,0, NUMX[wheelnumber[1]], NUMY[wheelnumber[1]], 63, 63);
		return true;
	case AID_C2_WND2:
		oapiBlt(surf, g_Param.clock_digits, 0,0, NUMX[wheelnumber[2]], NUMY[wheelnumber[2]], 63, 63);
		return true;
	case AID_C2_WND3:
		oapiBlt(surf, g_Param.clock_digits, 0,0, NUMX[wheelnumber[3]], NUMY[wheelnumber[3]], 63, 63);
		return true;
	
	
	}

	return false;
}

void PanelC2::RegisterVC()
{
	VECTOR3 ofs = sts->GetOrbiterCoGOffset() + VC_OFFSET;
	//SURFHANDLE tkbk_tex = oapiGetTextureHandle (sts->hOrbiterVCMesh, 5);

	//oapiWriteLog("C2:RegisterVC...");
	SURFHANDLE digittex = oapiGetTextureHandle(sts->hOrbiterVCMesh, TEX_CLOCKNUMS_VC);
	oapiVCRegisterArea (AID_C2, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBPRESSED);
	//oapiWriteLog("1...");
	oapiVCRegisterArea (AID_C2_WND0, _R(0,0,63,63), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex);
	oapiVCRegisterArea (AID_C2_WND1, _R(64,0,127,63), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex);
	oapiVCRegisterArea (AID_C2_WND2, _R(128,0,191,63), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex);
	oapiVCRegisterArea (AID_C2_WND3, _R(192,0,255,63), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex);
	
	//-0.140169
	//-0.140169, 1.95676, 14.5416 
	//0.131577, 1.95676, 14.5416 
	//0.131577, 1.7619, 14.3552
	oapiVCSetAreaClickmode_Quadrilateral (AID_C2, _V(-0.140169, 1.95676, 14.5416)+ofs, _V(0.131577, 1.95676, 14.5416)+ofs, 
		_V(-0.140169, 1.7619, 14.3552)+ofs, _V(0.131577, 1.7619, 14.3552)+ofs);
	//oapiVCSetAreaClickmode_Quadrilateral (AID_C3, _V(-0.0804, 1.667, 14.52)+ofs, _V(-0.111, 1.667, 14.52)+ofs, _V(-0.0804, 1.667, 14.52)+ofs, _V(0.276, 1.618, 14.066)+ofs);
	//TODO: Register the 4 event timer setting elements.
}

void PanelC2::CreateVisual(VISHANDLE hVisual)
{	
	
}

bool PanelC2::VCMouseEvent(int id, int _event, VECTOR3 &p)
{
	if (id != AID_C2) return false;
	bool action = false;
	

	if((_event & PANEL_MOUSE_DOWN) == 0)
	{
		return false;
	}

	if(p.x>0.067 && p.x < 0.177)
	{
		if(p.y > 0.171 && p.y < 0.223)
		{	
			if(switch_state[SWITCH1] > 0)
			{
				switch_state[SWITCH1]--;
				switch_timer[SWITCH1] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
		else if(p.y > 0.223 && p.y < 0.273)
		{
			if(switch_state[SWITCH1] < 2)
			{
				switch_state[SWITCH1]++;
				switch_timer[SWITCH1] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
	}
	else if(p.x>0.177 && p.x < 0.292)
	{
		if(p.y > 0.171 && p.y < 0.223)
		{	
			if(switch_state[SWITCH2] > 0)
			{
				switch_state[SWITCH2]--;
				switch_timer[SWITCH2] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
		else if(p.y > 0.223 && p.y < 0.273)
		{
			if(switch_state[SWITCH2] < 2)
			{
				switch_state[SWITCH2]++;
				switch_timer[SWITCH2] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
	}
	else if(p.x>0.392 && p.x < 0.504)
	{
		if(p.y > 0.171 && p.y < 0.223)
		{	
			if(switch_state[SWITCH3] > 0)
			{
				switch_state[SWITCH3]--;
				switch_timer[SWITCH3] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
		else if(p.y > 0.223 && p.y < 0.273)
		{
			if(switch_state[SWITCH3] < 2)
			{
				switch_state[SWITCH3]++;
				switch_timer[SWITCH3] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
	}
	else if(p.x>0.504 && p.x < 0.615)
	{
		if(p.y > 0.171 && p.y < 0.223)
		{	
			if(switch_state[SWITCH4] > 0)
			{
				switch_state[SWITCH4]--;
				switch_timer[SWITCH4] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
		else if(p.y > 0.223 && p.y < 0.273)
		{
			if(switch_state[SWITCH4] < 2)
			{
				switch_state[SWITCH4]++;
				switch_timer[SWITCH4] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
	}
	else if(p.x>0.711 && p.x < 0.830)
	{
		if(p.y > 0.171 && p.y < 0.223)
		{	
			if(switch_state[SWITCH5] > 0)
			{
				switch_state[SWITCH5]--;
				switch_timer[SWITCH5] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
		else if(p.y > 0.223 && p.y < 0.273)
		{
			if(switch_state[SWITCH5] < 2)
			{
				switch_state[SWITCH5]++;
				switch_timer[SWITCH5] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
	}
	else if(p.x>0.830 && p.x < 0.944)
	{
		if(p.y > 0.171 && p.y < 0.223)
		{	
			if(switch_state[SWITCH6] > 0)
			{
				switch_state[SWITCH6]--;
				switch_timer[SWITCH6] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
		else if(p.y > 0.223 && p.y < 0.273)
		{
			if(switch_state[SWITCH6] < 2)
			{
				switch_state[SWITCH6]++;
				switch_timer[SWITCH6] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
	}

	//Second row of switches
	if(p.x>=0.113359 && p.x<=0.194960) {
		if(p.y>=0.445173 && p.y<=0.531996) {
			if(p.x<0.155331) {
				switch_state[SWITCH7]=0;
				sts->CRT_SEL[0]=0;
			}
			else {
				switch_state[SWITCH7]=1;
				sts->CRT_SEL[0]=2;
			}
			action=true;
		}
	}
	else if(p.x>=0.839770 && p.x<=0.926774) {
		if(p.y>=0.430792 && p.y<=0.544745) {
			if(p.x<0.883762) {
				switch_state[SWITCH8]=0;
				sts->CRT_SEL[1]=2;
			}
			else {
				switch_state[SWITCH8]=1;
				sts->CRT_SEL[1]=1;
			}
			action=true;
		}
	}
	
	//Third row of switches
	//Switch 9: EVENT TIMER MODE
	if(p.x>0.098 && p.x < 0.195)
	{
		if(p.y > 0.736 && p.y < 0.789)
		{	
			if(switch_state[SWITCH9] > 0)
			{
				switch_state[SWITCH9]--;
				switch_timer[SWITCH9] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
		else if(p.y > 0.789 && p.y < 0.841)
		{
			if(switch_state[SWITCH9] < 2)
			{
				switch_state[SWITCH9]++;
				switch_timer[SWITCH9] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
		if(action)
		{
			switch(switch_state[SWITCH9]) {
			case 0:
				sts->pMTU->SetEventTimerMode(dps::TIMER_FORWARD, dps::COUNT_UP);
				break;
			case 1:
				sts->pMTU->SetEventTimerMode(dps::TIMER_FORWARD, dps::COUNT_DOWN);
				break;
			case 2:
				sts->pMTU->SetEventTimerMode(dps::TIMER_FORWARD, dps::COUNT_TEST);
				break;
			}
		}

	} 
	else if(p.x>0.195 && p.x < 0.294)
	{
		if(p.y > 0.736 && p.y < 0.789)
		{
			if(switch_state[SWITCH10] > 0)
			{
				switch_state[SWITCH10]--;
				switch_timer[SWITCH10] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
		else if(p.y > 0.789 && p.y < 0.841)
		{
			if(switch_state[SWITCH10] < 2)
			{
				switch_state[SWITCH10]++;
				switch_timer[SWITCH10] = SWITCH_HOLD_TIME;
				action = true;
			}

		}
		if(action)
		{
			switch(switch_state[SWITCH10]) {
			case 0:
				sts->pMTU->StartEventTimer(dps::TIMER_FORWARD);
				break;
			case 2:
				sts->pMTU->StopEventTimer(dps::TIMER_FORWARD);
				break;
			}
		}
	}
	else if(p.x>0.831 && p.x < 0.949)
	{
		if(p.y > 0.736 && p.y < 0.789)
		{
			if(switch_state[SWITCH11] > 0)
			{
				switch_state[SWITCH11]--;
				switch_timer[SWITCH11] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
		else if(p.y > 0.789 && p.y < 0.841)
		{
			if(switch_state[SWITCH11] < 2)
			{
				switch_state[SWITCH11]++;
				switch_timer[SWITCH11] = SWITCH_HOLD_TIME;
				action = true;
			}

		}
		if(action)
		{
			switch(switch_state[SWITCH11]) {
			case 0:
				sts->pMTU->SetEventTimer(dps::TIMER_FORWARD, wheelnumber[0]*10+wheelnumber[1], wheelnumber[2]*10 + wheelnumber[3]);
				break;
			case 2:
				sts->pMTU->ResetEventTimer(dps::TIMER_FORWARD);
				break;
			}
		}
	}
	else if(p.x> 0.45 && p.x < 0.49)
	{
		if(p.y > 0.62 && p.y < 0.68)
		{
			tgtwheel_state[0] = tgtwheel_state[0] + 0.25;
			action = true;
		}
		else if(p.y >0.72 && p.y < 0.79)
		{
			tgtwheel_state[0] -= 0.25;
			if(tgtwheel_state[0] < 0)
				tgtwheel_state[0] +=EVTTMR_WHEELMAX[0];
			action = true;
		}
	}
	else if(p.x> 0.52 && p.x < 0.56)
	{
		if(p.y > 0.62 && p.y < 0.68)
		{
			tgtwheel_state[1] = tgtwheel_state[1] + 0.25;
			action = true;
		}
		else if(p.y >0.72 && p.y < 0.79)
		{
			tgtwheel_state[1] -= 0.25;
			if(tgtwheel_state[1] < 0)
				tgtwheel_state[1] +=EVTTMR_WHEELMAX[1];
			action = true;
		}
	}
	else if(p.x> 0.61 && p.x < 0.65)
	{
		if(p.y > 0.62 && p.y < 0.68)
		{
			tgtwheel_state[2] = tgtwheel_state[2] + 0.25;
			action = true;
		}
		else if(p.y >0.72 && p.y < 0.79)
		{
			tgtwheel_state[2] -= 0.25;
			if(tgtwheel_state[2] < 0)
				tgtwheel_state[2] +=EVTTMR_WHEELMAX[2];
			action = true;
		}
	}
	else if(p.x> 0.68 && p.x < 0.72)
	{
		if(p.y > 0.62 && p.y < 0.68)
		{
			tgtwheel_state[3] = tgtwheel_state[3] + 0.25;
			action = true;
		}
		else if(p.y >0.72 && p.y < 0.79)
		{
			tgtwheel_state[3] -= 0.25;
			if(tgtwheel_state[3] < 0)
				tgtwheel_state[3] +=EVTTMR_WHEELMAX[3];
			action = true;
		}
	}

	if(action)
	{
		UpdateVC();
	}
	return false;
}

void PanelC2::UpdateVC() {
    int i;
	for(i = 0; i<4; i++)
	{
		sts->SetAnimation(anim_VC_C2Wheel[i], fmod(wheelState[i], 1.0));
	}
	for(i = 0; i<12; i++)
	{
		if(anim_VC_C2[i])
		{
			if(i!=SWITCH7 && i!=SWITCH8) sts->SetAnimation(anim_VC_C2[i], switch_state[i] * 0.5);
			else sts->SetAnimation(anim_VC_C2[i], switch_state[i]);
		}
	}
	return;
}

void PanelC2::DefineVCAnimations (UINT vcidx)
{
	
	static VECTOR3 switch_rot = {-1,0,0};
	static VECTOR3 wheel_rot = {1.0, 0, 0};

	static UINT VC_C2Evt10MW_Grp = GRP_C2TOG1_VC;

	static MGROUP_ROTATE VC_C2Evt10MW (vcidx, &VC_C2Evt10MW_Grp, 1,
		REF_C2S11, wheel_rot, (float)(360.0*RAD));
	anim_VC_C2Wheel[0]=sts->CreateAnimation (0.0);
	sts->AddAnimationComponent (anim_VC_C2Wheel[0], 0, 1, &VC_C2Evt10MW);

	static UINT VC_C2Evt1MW_Grp = GRP_C2TOG2_VC;

	static MGROUP_ROTATE VC_C2Evt1MW (vcidx, &VC_C2Evt1MW_Grp, 1,
		REF_C2S11, wheel_rot, (float)(360.0*RAD));
	anim_VC_C2Wheel[1]=sts->CreateAnimation (0.0);
	sts->AddAnimationComponent (anim_VC_C2Wheel[1], 0, 1, &VC_C2Evt1MW);

	static UINT VC_C2Evt10SW_Grp = GRP_C2TOG3_VC;

	static MGROUP_ROTATE VC_C2Evt10SW (vcidx, &VC_C2Evt10SW_Grp, 1,
		REF_C2S11, wheel_rot, (float)(360.0*RAD));
	anim_VC_C2Wheel[2]=sts->CreateAnimation (0.0);
	sts->AddAnimationComponent (anim_VC_C2Wheel[2], 0, 1, &VC_C2Evt10SW);

	static UINT VC_C2Evt1SW_Grp = GRP_C2TOG4_VC;

	static MGROUP_ROTATE VC_C2Evt1SW (vcidx, &VC_C2Evt1SW_Grp, 1,
		REF_C2S11, wheel_rot, (float)(360.0*RAD));
	anim_VC_C2Wheel[3]=sts->CreateAnimation (0.0);
	sts->AddAnimationComponent (anim_VC_C2Wheel[3], 0, 1, &VC_C2Evt1SW);

	static UINT VC_C2b1_Grp = GRP_C2B1_VC;

	static MGROUP_ROTATE VC_C2b1 (vcidx, &VC_C2b1_Grp, 1,
		REF_C2S1, switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH1]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH1], 0, 1, &VC_C2b1);

	static UINT VC_C2b2_Grp = GRP_C2B2_VC;

	static MGROUP_ROTATE VC_C2b2 (vcidx, &VC_C2b2_Grp, 1,
		REF_C2S2, switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH2]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH2], 0, 1, &VC_C2b2);

	static UINT VC_C2b3_Grp = GRP_C2B3_VC;

	static MGROUP_ROTATE VC_C2b3 (vcidx, &VC_C2b3_Grp, 1,
		REF_C2S3, switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH3]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH3], 0, 1, &VC_C2b3);

	static UINT VC_C2b4_Grp = GRP_C2B4_VC;

	static MGROUP_ROTATE VC_C2b4 (vcidx, &VC_C2b4_Grp, 1,
		REF_C2S4, switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH4]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH4], 0, 1, &VC_C2b4);

	static UINT VC_C2b5_Grp = GRP_C2B5_VC;

	static MGROUP_ROTATE VC_C2b5 (vcidx, &VC_C2b5_Grp, 1,
		REF_C2S5, switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH5]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH5], 0, 1, &VC_C2b5);

	static UINT VC_C2b6_Grp = GRP_C2B6_VC;

	static MGROUP_ROTATE VC_C2b6 (vcidx, &VC_C2b6_Grp, 1,
		REF_C2S6, switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH6]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH6], 0, 1, &VC_C2b6);

	static UINT VC_C2b7_Grp = GRP_C2B7V_VC;
	static MGROUP_ROTATE VC_C2b7 (vcidx, &VC_C2b7_Grp, 1,
		REF_C2S7, _V(0, -sin(45.0*RAD), -cos(45.0*RAD)), (float)(90.0*RAD));
	anim_VC_C2[SWITCH7]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent(anim_VC_C2[SWITCH7], 0, 1, &VC_C2b7);

	static UINT VC_C2b8_Grp = GRP_C2B8V_VC;
	static MGROUP_ROTATE VC_C2b8 (vcidx, &VC_C2b8_Grp, 1,
		REF_C2S8, _V(0, -sin(45.0*RAD), -cos(45.0*RAD)), (float)(90.0*RAD));
	anim_VC_C2[SWITCH8]=sts->CreateAnimation(0.5);
	sts->AddAnimationComponent(anim_VC_C2[SWITCH8], 0, 1, &VC_C2b8);
	
	static UINT VC_C2b9_Grp = GRP_C2B9_VC;

	static MGROUP_ROTATE VC_C2b9 (vcidx, &VC_C2b9_Grp, 1,
		REF_C2S9, switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH9]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH9], 0, 1, &VC_C2b9);


	static UINT VC_C2b10_Grp = GRP_C2B10_VC;

	//-0.0736091  
	static MGROUP_ROTATE VC_C2b10 (vcidx, &VC_C2b10_Grp, 1,
		REF_C2S10, switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH10]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH10], 0, 1, &VC_C2b10);

	static UINT VC_C2b11_Grp = GRP_C2B11_VC;

	//-0.0736091  
	static MGROUP_ROTATE VC_C2b11 (vcidx, &VC_C2b11_Grp, 1,
		REF_C2S12, switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH11]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH11], 0, 1, &VC_C2b11);
	
	

	/*
	static UINT VC_C3b1_Grp = GRP_C3b1_VC;
	static MGROUP_ROTATE VC_C3b1 (vcidx, &VC_C3b1_Grp, 1,
		_V(-.225,1.653,14.627), switch_rot, (float)(90.0*RAD));
	anim_VC_C3[0]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C3[0], 0, 1, &VC_C3b1);

	

	static UINT VC_C3b10_Grp = GRP_C3b10_VC;
	static MGROUP_ROTATE VC_C3b10 (vcidx, &VC_C3b10_Grp, 1,
		_V(-.223,1.642,14.521), switch_rot, (float)(90.0*RAD));
	anim_VC_C3[11]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C3[11], 0, 1, &VC_C3b10);

	static UINT VC_C3b11_Grp = GRP_C3b11_VC;
	static MGROUP_ROTATE VC_C3b11 (vcidx, &VC_C3b11_Grp, 1,
		_V(-.188,1.642,14.521), switch_rot, (float)(90.0*RAD));
	anim_VC_C3[12]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C3[12], 0, 1, &VC_C3b11);

	static UINT VC_C3b23_Grp = GRP_C3b23_VC;
	static MGROUP_ROTATE VC_C3b23 (vcidx, &VC_C3b23_Grp, 1,
		_V(-.217,1.608,14.186), switch_rot, (float)(90.0*RAD));
	anim_VC_C3[22]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C3[22], 0, 1, &VC_C3b23);

	static UINT VC_C3b24_Grp = GRP_C3b24_VC;
	static MGROUP_ROTATE VC_C3b24 (vcidx, &VC_C3b24_Grp, 1,
		_V(-.176,1.608,14.186), switch_rot, (float)(90.0*RAD));
	anim_VC_C3[23]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C3[23], 0, 1, &VC_C3b24);*/
	return;
}

void PanelC2::Step(double t, double dt)
{
	bool update = false;
	double fCurrState = 0.0;
	double fTgtState = 0.0;

	double fDeltaWheel = 0.5 * dt;

	for(int i = 0; i<4; i++)
	{
		fCurrState = wheelState[i];
		fTgtState = tgtwheel_state[i];

	
		if(0.0 == fCurrState && fTgtState >= EVTTMR_WHEELMAX[i] - 0.5)
		{
			fCurrState = EVTTMR_WHEELMAX[i];
		}

		if(fCurrState <= 0.5 && fTgtState == EVTTMR_WHEELMAX[i])
		{
			fTgtState = 0.0;
		}

		if(fCurrState == fTgtState)
		{
			if(tgtwheel_state[i] >= EVTTMR_WHEELMAX[i])
			{
				tgtwheel_state[i] -= EVTTMR_WHEELMAX[i];
			}
			continue;
		}
		else if(fCurrState > fTgtState)
		{
			if(fCurrState - fDeltaWheel < fTgtState)
				wheelState[i] = fTgtState;
			else
				wheelState[i] = fCurrState - fDeltaWheel;

			update = true;
		}
		else
		{
			if(fCurrState + fDeltaWheel > fTgtState)
				wheelState[i] = fTgtState;
			else
				wheelState[i] = fCurrState + fDeltaWheel;
			update = true;
		}

		

		if(wheelState[i] >= EVTTMR_WHEELMAX[i])
		{
			wheelState[i] -= EVTTMR_WHEELMAX[i];
			if(tgtwheel_state[i] > EVTTMR_WHEELMAX[i])
				tgtwheel_state[i] -= EVTTMR_WHEELMAX[i];
		}

		wheelnumber[i] = (int)(fmod(wheelState[i]/ 0.25, EVTTMR_WHEELMAX[i] * 2.0) );
	}


	if(wheelnumber[0] != oldwheelnumber[0])
	{
		oapiVCTriggerRedrawArea(-1, AID_C2_WND0);
		oldwheelnumber[0] = wheelnumber[0];
	}

	if(wheelnumber[1] != oldwheelnumber[1])
	{
		oapiVCTriggerRedrawArea(-1, AID_C2_WND1);
		oldwheelnumber[1] = wheelnumber[1];
	}

	if(wheelnumber[2] != oldwheelnumber[2])
	{
		oapiVCTriggerRedrawArea(-1, AID_C2_WND2);
		oldwheelnumber[2] = wheelnumber[2];
	}

	if(wheelnumber[3] != oldwheelnumber[3])
	{
		oapiVCTriggerRedrawArea(-1, AID_C2_WND3);
		oldwheelnumber[3] = wheelnumber[3];
	}
	
	//Switch 9: EVENT TIMER MODE
	if(switch_state[SWITCH9] == 2)
	{
		switch_timer[SWITCH9] -= dt;
		if(switch_timer[SWITCH9] <=0.0)
		{
			switch_state[SWITCH9] = 1;
			sts->pMTU->SetEventTimerMode(dps::TIMER_FORWARD, dps::COUNT_DOWN);
			update = true;
		}
	}

	if(switch_state[SWITCH10] != 1)
	{
		switch_timer[SWITCH10] -= dt;
		if(switch_timer[SWITCH10] <=0.0)
		{
			switch_state[SWITCH10] = 1;
			update = true;
		}
		
	}

	if(switch_state[SWITCH11] != 1)
	{
		switch_timer[SWITCH11] -= dt;
		if(switch_timer[SWITCH11] <=0.0)
		{
			switch_state[SWITCH11] = 1;
			update = true;
		}
		
	}

	if(update)
		UpdateVC();
}


void PanelC2::SaveState(FILEHANDLE scn)
{
	char cbuf[256];
	sprintf_s(cbuf, 255, "%d %d", switch_state[SWITCH7], switch_state[SWITCH8]);
	oapiWriteScenario_string(scn, "CRT_SEL", cbuf);
}

bool PanelC2::ParseScenarioLine (char *line)
{
	if(!_strnicmp(line, "CRT_SEL", 7)) {
		sscanf_s(line+7, "%d %d", &switch_state[SWITCH7], &switch_state[SWITCH8]);
		if(switch_state[SWITCH7]==0) sts->CRT_SEL[0]=0;
		else sts->CRT_SEL[0]=2;
		if(switch_state[SWITCH8]==0) sts->CRT_SEL[1]=2;
		else sts->CRT_SEL[1]=1;
		return true;
	}
	return false;
}
