#include "PanelC2.h"
#include "resource.h"
#include "meshres.h"
#include "meshres_vc.h"
#include "meshres_vc_additions.h"
#include "DlgCtrl.h"
#include <stdio.h>
#include "MasterTimingUnit.h"

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
	//sprintf(oapiDebugString(), "VCRedrawEvent %d %d", id, event);
	//oapiWriteLog("PanelC2::VCRedrawEvent\n");
	
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
	VECTOR3 ofs = sts->orbiter_ofs;
	//SURFHANDLE tkbk_tex = oapiGetTextureHandle (sts->hOrbiterVCMesh, 5);

	//oapiWriteLog("C2:RegisterVC...");
	SURFHANDLE digittex = oapiGetTextureHandle(sts->hOrbiterVCMesh, 8);
	oapiVCRegisterArea (AID_C2, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
	//oapiWriteLog("1...");
	oapiVCRegisterArea (AID_C2_WND0, _R(0,0,63,63), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex);
	oapiVCRegisterArea (AID_C2_WND1, _R(64,0,127,63), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex);
	oapiVCRegisterArea (AID_C2_WND2, _R(128,0,191,63), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex);
	oapiVCRegisterArea (AID_C2_WND3, _R(192,0,255,63), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digittex);
	
	oapiVCSetAreaClickmode_Quadrilateral (AID_C2, _V(-0.144, 1.888, 14.894)+ofs, _V(0.135, 1.888, 14.894)+ofs, 
		_V(-0.144, 1.688, 14.702)+ofs, _V(0.135, 1.688, 14.702)+ofs);
	//oapiVCSetAreaClickmode_Quadrilateral (AID_C3, _V(-0.0804, 1.667, 14.52)+ofs, _V(-0.111, 1.667, 14.52)+ofs, _V(-0.0804, 1.667, 14.52)+ofs, _V(0.276, 1.618, 14.066)+ofs);
	//sprintf(oapiDebugString(), "PanelC3 Registered");
	//TODO: Register the 4 event timer setting elements.
	//oapiWriteLog("end\n");
}

void PanelC2::CreateVisual(VISHANDLE hVisual)
{	
	
}

bool PanelC2::VCMouseEvent(int id, int event, VECTOR3 &p)
{
	if (id != AID_C2) return false;
	bool action = false;
	//sprintf(oapiDebugString(), "PANEL C2: id %d event %d p %f %f %f",id,event,p.x,p.y,p.z);

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
				sts->pMTU->SetEventTimerMode(TIMER_FORWARD, COUNT_UP);
				break;
			case 1:
				sts->pMTU->SetEventTimerMode(TIMER_FORWARD, COUNT_DOWN);
				break;
			case 2:
				sts->pMTU->SetEventTimerMode(TIMER_FORWARD, COUNT_TEST);
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
				sts->pMTU->StartEventTimer(TIMER_FORWARD);
				break;
			case 2:
				sts->pMTU->StopEventTimer(TIMER_FORWARD);
				break;
			}
		}
	}
	else if(p.x>0.831 && p.x < 0.949)
	{
		if(p.y > 0.736 && p.y < 0.789)
		{
			if(switch_state[SWITCH12] > 0)
			{
				switch_state[SWITCH12]--;
				switch_timer[SWITCH12] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
		else if(p.y > 0.789 && p.y < 0.841)
		{
			if(switch_state[SWITCH12] < 2)
			{
				switch_state[SWITCH12]++;
				switch_timer[SWITCH12] = SWITCH_HOLD_TIME;
				action = true;
			}

		}
		if(action)
		{
			switch(switch_state[SWITCH12]) {
			case 0:
				sts->pMTU->SetEventTimer(TIMER_FORWARD, wheelnumber[0]*10+wheelnumber[1], wheelnumber[2]*10 + wheelnumber[3]);
				break;
			case 2:
				sts->pMTU->ResetEventTimer(TIMER_FORWARD);
				break;
			}
		}
	}
	else if(p.x> 0.45 && p.x < 0.49)
	{
		if(p.y > 0.62 && p.y < 0.68)
		{
			tgtwheel_state[0] = tgtwheel_state[0] + 0.25;
			sprintf(oapiDebugString(), "PANEL C2: counting 10 minutes up.%4.2f %4.2f %4.2f %4.2f", tgtwheel_state[0], 
				tgtwheel_state[1],tgtwheel_state[2],tgtwheel_state[3]);
			
			action = true;
		}
		else if(p.y >0.72 && p.y < 0.79)
		{
			tgtwheel_state[0] -= 0.25;
			if(tgtwheel_state[0] < 0)
				tgtwheel_state[0] +=EVTTMR_WHEELMAX[0];
			sprintf(oapiDebugString(), "PANEL C2: counting 10 minutes down. %4.2f %4.2f %4.2f %4.2f", tgtwheel_state[0], 
				tgtwheel_state[1],tgtwheel_state[2],tgtwheel_state[3]);

			action = true;
		}
	}
	else if(p.x> 0.52 && p.x < 0.56)
	{
		if(p.y > 0.62 && p.y < 0.68)
		{
			tgtwheel_state[1] = tgtwheel_state[1] + 0.25;
			sprintf(oapiDebugString(), "PANEL C2: counting 1 minute up. %4.2f %4.2f %4.2f %4.2f", tgtwheel_state[0], 
				tgtwheel_state[1],tgtwheel_state[2],tgtwheel_state[3]);
			
			action = true;
		}
		else if(p.y >0.72 && p.y < 0.79)
		{
			tgtwheel_state[1] -= 0.25;
			if(tgtwheel_state[1] < 0)
				tgtwheel_state[1] +=EVTTMR_WHEELMAX[1];
			sprintf(oapiDebugString(), "PANEL C2: counting 1 minute down.%4.2f %4.2f %4.2f %4.2f", tgtwheel_state[0], 
				tgtwheel_state[1],tgtwheel_state[2],tgtwheel_state[3]);

			action = true;
		}
	}
	else if(p.x> 0.61 && p.x < 0.65)
	{
		if(p.y > 0.62 && p.y < 0.68)
		{
			tgtwheel_state[2] = tgtwheel_state[2] + 0.25;
			sprintf(oapiDebugString(), "PANEL C2: counting 10 seconds up. %4.2f %4.2f %4.2f %4.2f", tgtwheel_state[0], 
				tgtwheel_state[1],tgtwheel_state[2],tgtwheel_state[3]);			
			action = true;
		}
		else if(p.y >0.72 && p.y < 0.79)
		{
			tgtwheel_state[2] -= 0.25;
			if(tgtwheel_state[2] < 0)
				tgtwheel_state[2] +=EVTTMR_WHEELMAX[2];
			sprintf(oapiDebugString(), "PANEL C2: counting 10 seconds down.%4.2f %4.2f %4.2f %4.2f", tgtwheel_state[0], 
				tgtwheel_state[1],tgtwheel_state[2],tgtwheel_state[3]);

			action = true;
		}
	}
	else if(p.x> 0.68 && p.x < 0.72)
	{
		if(p.y > 0.62 && p.y < 0.68)
		{
			tgtwheel_state[3] = tgtwheel_state[3] + 0.25;
			sprintf(oapiDebugString(), "PANEL C2: counting 1 second up. %4.2f %4.2f %4.2f %4.2f", tgtwheel_state[0], 
				tgtwheel_state[1],tgtwheel_state[2],tgtwheel_state[3]);
			
			action = true;
		}
		else if(p.y >0.72 && p.y < 0.79)
		{
			tgtwheel_state[3] -= 0.25;
			if(tgtwheel_state[3] < 0)
				tgtwheel_state[3] +=EVTTMR_WHEELMAX[3];
			sprintf(oapiDebugString(), "PANEL C2: counting 1 second down. %4.2f %4.2f %4.2f %4.2f", tgtwheel_state[0], 
				tgtwheel_state[1],tgtwheel_state[2],tgtwheel_state[3]);

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
			sts->SetAnimation(anim_VC_C2[i], switch_state[i] * 0.5);
		}
	}
	return;
}

void PanelC2::DefineVCAnimations (UINT vcidx)
{
	
	static VECTOR3 switch_rot = {-1,0,0};
	static VECTOR3 wheel_rot = {1.0, 0, 0};

	static VECTOR3 wheel_ctr = _V(-4.448496E-03, 1.733672, 14.77007);

	static UINT VC_C2Evt10MW_Grp = GRP_C2Evt10MW_VC;

	static MGROUP_ROTATE VC_C2Evt10MW (vcidx, &VC_C2Evt10MW_Grp, 1,
		wheel_ctr, wheel_rot, (float)(360.0*RAD));
	anim_VC_C2Wheel[0]=sts->CreateAnimation (0.0);
	sts->AddAnimationComponent (anim_VC_C2Wheel[0], 0, 1, &VC_C2Evt10MW);

	static UINT VC_C2Evt1MW_Grp = GRP_C2Evt1MW_VC;

	static MGROUP_ROTATE VC_C2Evt1MW (vcidx, &VC_C2Evt1MW_Grp, 1,
		wheel_ctr, wheel_rot, (float)(360.0*RAD));
	anim_VC_C2Wheel[1]=sts->CreateAnimation (0.0);
	sts->AddAnimationComponent (anim_VC_C2Wheel[1], 0, 1, &VC_C2Evt1MW);

	static UINT VC_C2Evt10SW_Grp = GRP_C2Evt10SW_VC;

	static MGROUP_ROTATE VC_C2Evt10SW (vcidx, &VC_C2Evt10SW_Grp, 1,
		wheel_ctr, wheel_rot, (float)(360.0*RAD));
	anim_VC_C2Wheel[2]=sts->CreateAnimation (0.0);
	sts->AddAnimationComponent (anim_VC_C2Wheel[2], 0, 1, &VC_C2Evt10SW);

	static UINT VC_C2Evt1SW_Grp = GRP_C2Evt1SW_VC;

	static MGROUP_ROTATE VC_C2Evt1SW (vcidx, &VC_C2Evt1SW_Grp, 1,
		wheel_ctr, wheel_rot, (float)(360.0*RAD));
	anim_VC_C2Wheel[3]=sts->CreateAnimation (0.0);
	sts->AddAnimationComponent (anim_VC_C2Wheel[3], 0, 1, &VC_C2Evt1SW);

	static UINT VC_C2b1_Grp = GRP_C2b1_VC;

	static MGROUP_ROTATE VC_C2b1 (vcidx, &VC_C2b1_Grp, 1,
		_V(-0.1105, 1.8436,  14.8518), switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH1]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH1], 0, 1, &VC_C2b1);

	static UINT VC_C2b2_Grp = GRP_C2b2_VC;

	static MGROUP_ROTATE VC_C2b2 (vcidx, &VC_C2b2_Grp, 1,
		_V(-0.0781, 1.8436,  14.8518), switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH2]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH2], 0, 1, &VC_C2b2);

	static UINT VC_C2b3_Grp = GRP_C2b3_VC;

	static MGROUP_ROTATE VC_C2b3 (vcidx, &VC_C2b3_Grp, 1,
		_V(-0.0203, 1.8436,  14.8518), switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH3]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH3], 0, 1, &VC_C2b3);

	static UINT VC_C2b4_Grp = GRP_C2b4_VC;

	static MGROUP_ROTATE VC_C2b4 (vcidx, &VC_C2b4_Grp, 1,
		_V(0.0124, 1.8436,  14.8518), switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH4]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH4], 0, 1, &VC_C2b4);

	static UINT VC_C2b5_Grp = GRP_C2b5_VC;

	static MGROUP_ROTATE VC_C2b5 (vcidx, &VC_C2b5_Grp, 1,
		_V(0.0701, 1.8436,  14.8518), switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH5]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH5], 0, 1, &VC_C2b5);

	static UINT VC_C2b6_Grp = GRP_C2b6_VC;

	static MGROUP_ROTATE VC_C2b6 (vcidx, &VC_C2b6_Grp, 1,
		_V(0.1022, 1.8436,  14.8518), switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH6]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH6], 0, 1, &VC_C2b6);




	static UINT VC_C2b9_Grp = GRP_C2b9_VC;

	static MGROUP_ROTATE VC_C2b9 (vcidx, &VC_C2b9_Grp, 1,
		_V(-0.1046921, 1.731414,  14.74131), switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH9]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH9], 0, 1, &VC_C2b9);


	static UINT VC_C2b10_Grp = GRP_C2b10_VC;

	//-0.0736091  
	static MGROUP_ROTATE VC_C2b10 (vcidx, &VC_C2b10_Grp, 1,
		_V(-0.0736091,  1.731414,  14.74131), switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH10]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH10], 0, 1, &VC_C2b10);

	static UINT VC_C2b12_Grp = GRP_C2b12_VC;

	//-0.0736091  
	static MGROUP_ROTATE VC_C2b12 (vcidx, &VC_C2b12_Grp, 1,
		_V( 0.1022747,  1.731414,  14.74131), switch_rot, (float)(90.0*RAD));
	anim_VC_C2[SWITCH12]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_C2[SWITCH12], 0, 1, &VC_C2b12);
	
	

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
	//sprintf(oapiDebugString(), "Panel C2 installed");
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

	/*
	sprintf(oapiDebugString(), "PANEL C2: States %4.2f/%4.2f %4.2f/%4.2f %4.2f/%4.2f %4.2f/%4.2f",
		wheelState[0], tgtwheel_state[0], wheelState[1], tgtwheel_state[1], wheelState[2], tgtwheel_state[2], 
		wheelState[3], tgtwheel_state[3]);

	*/

	
	//sprintf(oapiDebugString(), "PANEL C2: EVT TIMER: %1d%1d:%1d%1d", 
	//	wheelnumber[0], wheelnumber[1], wheelnumber[2], wheelnumber[3]);

	
	
	//Switch 9: EVENT TIMER MODE
	if(switch_state[SWITCH9] == 2)
	{
		switch_timer[SWITCH9] -= dt;
		if(switch_timer[SWITCH9] <=0.0)
		{
			switch_state[SWITCH9] = 1;
			sts->pMTU->SetEventTimerMode(TIMER_FORWARD, COUNT_DOWN);
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

	if(switch_state[SWITCH12] != 1)
	{
		switch_timer[SWITCH12] -= dt;
		if(switch_timer[SWITCH12] <=0.0)
		{
			switch_state[SWITCH12] = 1;
			update = true;
		}
		
	}

	if(update)
		UpdateVC();
}


void PanelC2::SaveState(FILEHANDLE scn)
{
	//char cbuf[256];
}

bool PanelC2::ParseScenarioLine (char *line)
{
	return false;
}