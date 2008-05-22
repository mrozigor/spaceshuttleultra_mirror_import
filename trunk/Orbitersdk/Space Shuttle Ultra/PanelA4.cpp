#include "PanelA4.h"
#include "resource.h"
#include "meshres.h"
#include "meshres_vc.h"
#include "meshres_vc_additions.h"
#include "DlgCtrl.h"
#include <stdio.h>
#include "dps/MasterTimingUnit.h"

extern GDIParams g_Param;
extern HELPCONTEXT g_hc;
extern char *ActionString[5];

// ==============================================================

PanelA4::PanelA4(Atlantis *_sts): sts(_sts)
{
	switch_state = 1;
	old_switch_state = 1;
    switch_timer = 0.0;
	sTimerMinutes = 0;
	sTimerSeconds = 0;
	sMETSeconds = 0;
	sMETMinutes = 0;
	sMETHours = 0;
	sMETDays = 0;
}

bool PanelA4::VCRedrawEvent(int id, int event, SURFHANDLE surf)
{
	const int NUMX[10] = {64, 0, 64, 128, 192, 256, 320, 384, 448, 0};
	const int NUMY[10] = {384, 448, 448, 448, 448, 448, 448, 448, 448, 384};
	int i;
	int digit[8];
	
	if(id ==AID_A4_EVTTMR)
	{
		//sprintf(oapiDebugString(), "PanelF7::VCRedrawEvent: A4 Event Timer");

		digit[0] = sTimerMinutes / 10;
		digit[1] = sTimerMinutes % 10;
		digit[2] = sTimerSeconds / 10;
		digit[3] = sTimerSeconds % 10;

		for(i = 0; i<4; i++)
		{
			oapiBlt(surf, g_Param.digits_7seg, i*64, 0, NUMX[digit[i]], NUMY[digit[i]], 64, 64);
		}
	}

	else if(id == AID_A4_METTMR1)
	{
		//sprintf(oapiDebugString(), "PanelF7::VCRedrawEvent: A4 MET display, section 1");

		digit[0] = sMETDays /100;
		digit[1] = (sMETDays / 10) % 10;
		digit[2] = sMETDays %10;
		digit[3] = sMETHours / 10;
		digit[4] = sMETHours % 10;
		digit[5] = sMETMinutes / 10;
		digit[6] = sMETMinutes % 10;
		digit[7] = sMETSeconds / 10;


		for(i = 0; i<8; i++)
		{
			oapiBlt(surf, g_Param.digits_7seg, i*64, 0, NUMX[digit[i]], NUMY[digit[i]], 64, 64);
		}
	}
	else if(id == AID_A4_METTMR2)
	{
		//sprintf(oapiDebugString(), "PanelF7::VCRedrawEvent: A4 MET display, section 2");

		digit[0] = sMETSeconds % 10;
		
		oapiBlt(surf, g_Param.digits_7seg, 0, 0, NUMX[digit[0]], NUMY[digit[0]], 64, 64);
		
	}
	return true;
}

void PanelA4::RegisterVC()
{
	VECTOR3 ofs = sts->orbiter_ofs;
	SURFHANDLE digit_tex = oapiGetTextureHandle (sts->hOrbiterVCMesh, TEX_DIGITS_VC);

	oapiVCRegisterArea (AID_A4, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
	//oapiVCSetAreaClickmode_Quadrilateral (AID_A4,  _V(2, 3.07024, 10)+ofs, _V(-2, 3.07024, 10)+ofs, 
	//	 _V(2, 0, 10)+ofs, _V(-2, 0, 10)+ofs );
	

	//Mesh data for this panel
	//-0.865687 2.87081 12.6344 0.258829 0 0.965923 0.99614 -0.0233488
	//-0.651648 2.87081 12.5771 0.258829 0 0.965923 0.00344518 -0.0233488
	//-0.651648 3.07024 12.5771 0.258829 0 0.965923 0.00344518 -0.974144
	//-0.865687 3.07024 12.6344 0.258829 0 0.965923 0.99614 -0.974144

	oapiVCSetAreaClickmode_Quadrilateral (AID_A4,  _V(-0.865687, 3.07024, 12.6344)+ofs, _V(-0.651648, 3.07024, 12.5771)+ofs, 
		 _V(-0.865687, 2.87081, 12.6344)+ofs, _V(-0.651648, 2.87081, 12.5771)+ofs );
	oapiVCRegisterArea (AID_A4_EVTTMR, _R(64, 384, 320, 448), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digit_tex);	
	oapiVCRegisterArea (AID_A4_METTMR1, _R(0, 448, 512, 512), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digit_tex);	
	oapiVCRegisterArea (AID_A4_METTMR2, _R(0, 384, 64, 448), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digit_tex);	
}

bool PanelA4::VCMouseEvent(int id, int event, VECTOR3 &p)
{
	if (id != AID_A4) return false;
	bool action = false;
	sprintf_s(oapiDebugString(), 255, "PANEL A4: id %d event %d p %f %f %f",id,event,p.x,p.y,p.z);

	if(p.x>0.067 && p.x < 0.177)
	{
		if(p.y > 0.171 && p.y < 0.223)
		{	
			if(switch_state > 0)
			{
				switch_state--;
				switch_timer = SWITCH_HOLD_TIME;
				action = true;
			}
		}
		else if(p.y > 0.223 && p.y < 0.273)
		{
			if(switch_state < 2)
			{
				switch_state++;
				switch_timer = SWITCH_HOLD_TIME;
				action = true;
			}
		}
	}

	if(action)
	{
		UpdateVC();
	}
	return false;
}

void PanelA4::UpdateVC()
{
	sts->SetAnimation(anim_VC_A4, switch_state * 0.5);
	return;
}

void PanelA4::DefineVCAnimations (UINT vcidx)
{
	
	//_V(-0.651648, 3.07024, 12.5771)+ofs, _V(-0.865687, 3.07024, 12.6344)+ofs
	//_V(-0.214039, 0, 0.0573);	//L=0.221576134
	static VECTOR3 switch_rot = {-0.96598,0,0.25860};

	static UINT VC_A4_METSWITCH_GRP = GRP_A4MissionSwitch_VC;

	static MGROUP_ROTATE VC_A4_METSWITCH (vcidx, &VC_A4_METSWITCH_GRP, 1,
		_V(-0.816909, 2.90472, 12.6216), switch_rot, (float)(90.0*RAD));
	anim_VC_A4=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_A4, 0, 1, &VC_A4_METSWITCH);
	return;
}

void PanelA4::Step(double t, double dt)
{
	//Check forward event timer for changes and update clock if needed

	if(sts->pMTU->GetEventTimerMin(dps::TIMER_AFT) != sTimerMinutes || 
		sts->pMTU->GetEventTimerSec(dps::TIMER_AFT) != sTimerSeconds) {
		sTimerMinutes = sts->pMTU->GetEventTimerMin(dps::TIMER_AFT);
		sTimerSeconds = sts->pMTU->GetEventTimerSec(dps::TIMER_AFT);
		oapiVCTriggerRedrawArea(-1, AID_A4_EVTTMR);
	}

	switch(switch_state) {
	case 0:
		if(sts->pMTU->GetGMTDay(0) != sMETDays || 
			sts->pMTU->GetGMTHour(0) != sMETHours ||
			sts->pMTU->GetGMTSec(0) != sMETSeconds ||
			sts->pMTU->GetGMTMin(0) != sMETMinutes) {
			if(sts->pMTU->GetGMTSec(0) != sMETSeconds) {
				oapiVCTriggerRedrawArea(-1, AID_A4_METTMR2);
			}
			sMETDays = sts->pMTU->GetGMTDay(0);
			sMETHours = sts->pMTU->GetGMTHour(0);
			sMETMinutes = sts->pMTU->GetGMTMin(0);
			sMETSeconds = sts->pMTU->GetGMTSec(0);
			oapiVCTriggerRedrawArea(-1, AID_A4_METTMR1);
		}
		break;
	case 1:
		if(sts->pMTU->GetMETDay(0) != sMETDays || 
			sts->pMTU->GetMETHour(0) != sMETHours ||
			sts->pMTU->GetMETSec(0) != sMETSeconds ||
			sts->pMTU->GetMETMin(0) != sMETMinutes) {
			if(sts->pMTU->GetMETSec(0) != sMETSeconds) {
				sMETSeconds = sts->pMTU->GetMETSec(0);
				oapiVCTriggerRedrawArea(-1, AID_A4_METTMR2);
			}
			sMETDays = sts->pMTU->GetMETDay(0);
			sMETHours = sts->pMTU->GetMETHour(0);
			sMETMinutes = sts->pMTU->GetMETMin(0);
			
			//sprintf(oapiDebugString(), "Panel A4::%d %d %d %d", sMETDays, sMETHours, sMETMinutes, sMETSeconds);
			oapiVCTriggerRedrawArea(-1, AID_A4_METTMR1);
		}
		break;
	case 2:
		if(sMETDays != 888)
		{
			sMETDays = 888;
			sMETHours = 88;
			sMETMinutes = 88;
			sMETSeconds = 88;
			oapiVCTriggerRedrawArea(-1, AID_A4_METTMR1);
			oapiVCTriggerRedrawArea(-1, AID_A4_METTMR2);
		}
		break;
	}
}


void PanelA4::SaveState(FILEHANDLE scn)
{
}

bool PanelA4::ParseScenarioLine (char *line)
{
//	int nNum;
	return false;
}
