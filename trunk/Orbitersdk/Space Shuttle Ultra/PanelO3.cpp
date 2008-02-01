#include "PanelO3.h"
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

// ==============================================================

PanelO3::PanelO3(Atlantis *_sts): sts(_sts)
{
	int i;
	for(i = 0; i<NUM_O3_SWITCHES; i++)
	{
		switch_state[i] = 1;
		old_switch_state[i] = 1;
	    switch_timer[i] = 0.0;
	}
	sTimerMinutes = 0;
	sTimerSeconds = 0;
	sMETSeconds = 0;
	sMETMinutes = 0;
	sMETHours = 0;
	sMETDays = 0;

	for(i = 0; i<3; i++)
	{
		sOMSFuel[i] = 99;
	}
}

bool PanelO3::VCRedrawEvent(int id, int event, SURFHANDLE surf)
{
	const int NUMX[10] = {64, 0, 64, 128, 192, 256, 320, 384, 448, 0};
	const int NUMY[10] = {384, 448, 448, 448, 448, 448, 448, 448, 448, 384};
	int i;
	int digit[8];

	//METTMR1 = Days
	//METTMR2 = Rest
	
	if(id == AID_O3_METTMR1)
	{
		//sprintf(oapiDebugString(), "PanelF7::VCRedrawEvent: O3 MET display, section 1 (%f s)", oapiGetSimTime());

		digit[0] = sMETDays /100;
		digit[1] = (sMETDays / 10) % 10;
		digit[2] = sMETDays %10;
		

		for(i = 0; i<3; i++)
		{
			oapiBlt(surf, g_Param.digits_7seg, i*64, 0, NUMX[digit[i]], NUMY[digit[i]], 64, 64);
		}
	}
	else if(id == AID_O3_METTMR2)
	{
		//sprintf(oapiDebugString(), "PanelF7::VCRedrawEvent: O3 MET display, section 2 (%f s)",  oapiGetSimTime());

		digit[0] = sMETHours / 10;
		digit[1] = sMETHours % 10;
		digit[2] = sMETMinutes / 10;
		digit[3] = sMETMinutes % 10;
		digit[4] = sMETSeconds / 10;
		digit[5] = sMETSeconds % 10;

		for(i = 0; i<6; i++)
		{
			oapiBlt(surf, g_Param.digits_7seg, i*64, 0, NUMX[digit[i]], NUMY[digit[i]], 64, 64);
		}
		
	}
	else if(id == AID_O3_RCS)
	{
		digit[0] = sOMSFuel[0] / 10;
		digit[1] = sOMSFuel[0] % 10;
		digit[2] = sOMSFuel[1] / 10;
		digit[3] = sOMSFuel[1] % 10;
		digit[4] = sOMSFuel[2] / 10;
		digit[5] = sOMSFuel[2] % 10;

		//sprintf(oapiDebugString(), "PanelF7::VCRedrawEvent: A4 MET display, section 2");
		for(i = 0; i<6; i++)
		{
			oapiBlt(surf, g_Param.digits_7seg, i*64, 0, NUMX[digit[i]], NUMY[digit[i]], 64, 64);
		}

		//sprintf(oapiDebugString(), "PanelF7::VCRedrawEvent: O3 RCS display");
	}



	return true;
}

void PanelO3::RegisterVC()
{
	VECTOR3 ofs = sts->orbiter_ofs;
	SURFHANDLE digit_tex = oapiGetTextureHandle (sts->hOrbiterVCMesh, TEX_DIGITS_VC);

	oapiVCSetAreaClickmode_Quadrilateral (AID_O3,  _V(2, 3.07024, 10)+ofs, _V(-2, 3.07024, 10)+ofs, 
		 _V(2, 0, 10)+ofs, _V(-2, 0, 10)+ofs );
	

	//Mesh data for this panel
	//-0.865687 2.87081 12.6344 0.258829 0 0.965923 0.99614 -0.0233488
	//-0.651648 2.87081 12.5771 0.258829 0 0.965923 0.00344518 -0.0233488
	//-0.651648 3.07024 12.5771 0.258829 0 0.965923 0.00344518 -0.974144
	//-0.865687 3.07024 12.6344 0.258829 0 0.965923 0.99614 -0.974144

	//oapiVCSetAreaClickmode_Quadrilateral (AID_A4,  _V(-0.865687, 3.07024, 12.6344)+ofs, _V(-0.651648, 3.07024, 12.5771)+ofs, 
	//	 _V(-0.865687, 2.87081, 12.6344)+ofs, _V(-0.651648, 2.87081, 12.5771)+ofs );
	
	oapiVCRegisterArea (AID_O3_METTMR1, _R(320, 0, 512, 64), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digit_tex);	
	oapiVCRegisterArea (AID_O3_METTMR2, _R(0, 64, 384, 128), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digit_tex);	
	oapiVCRegisterArea (AID_O3_RCS, _R(0, 256, 384, 320), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digit_tex);	
}

bool PanelO3::VCMouseEvent(int id, int event, VECTOR3 &p)
{
	if (id != AID_O3) return false;
	bool action = false;
	sprintf(oapiDebugString(), "PANEL O3: id %d event %d p %f %f %f",id,event,p.x,p.y,p.z);
/*
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
*/
	if(action)
	{
		UpdateVC();
	}
	return false;
}

void PanelO3::UpdateVC()
{
	//sts->SetAnimation(anim_VC_A4, switch_state * 0.5);
	return;
}

void PanelO3::DefineVCAnimations (UINT vcidx)
{
	
	//_V(-0.651648, 3.07024, 12.5771)+ofs, _V(-0.865687, 3.07024, 12.6344)+ofs
	//_V(-0.214039, 0, 0.0573);	//L=0.221576134
	static VECTOR3 switch_rot = {-0.96598,0,0.25860};
/*
	static UINT VC_A4_METSWITCH_GRP = GRP_A4MissionSwitch_VC;

	static MGROUP_ROTATE VC_A4_METSWITCH (vcidx, &VC_A4_METSWITCH_GRP, 1,
		_V(-0.816909, 2.90472, 12.6216), switch_rot, (float)(90.0*RAD));
	anim_VC_A4=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_A4, 0, 1, &VC_A4_METSWITCH);
	*/
	return;
}

void PanelO3::Step(double t, double dt)
{
	//Check forward event timer for changes and update clock if needed

	switch(switch_state[0]) {
	case 0:
		if(sts->pMTU->GetGMTDay(0) != sMETDays || 
			sts->pMTU->GetGMTHour(0) != sMETHours ||
			sts->pMTU->GetGMTSec(0) != sMETSeconds ||
			sts->pMTU->GetGMTMin(0) != sMETMinutes) {
			if(sts->pMTU->GetGMTDay(0) != sMETDays) {
				sMETDays = sts->pMTU->GetGMTDay(0);
				oapiVCTriggerRedrawArea(-1, AID_O3_METTMR1);
			}
			//sMETDays = sts->pMTU->GetGMTDay(0);
			sMETHours = sts->pMTU->GetGMTHour(0);
			sMETMinutes = sts->pMTU->GetGMTMin(0);
			sMETSeconds = sts->pMTU->GetGMTSec(0);
			oapiVCTriggerRedrawArea(-1, AID_O3_METTMR2);
		}
		break;
	case 1:
		if(sts->pMTU->GetMETDay(0) != sMETDays || 
			sts->pMTU->GetMETHour(0) != sMETHours ||
			sts->pMTU->GetMETSec(0) != sMETSeconds ||
			sts->pMTU->GetMETMin(0) != sMETMinutes) {
			if(sts->pMTU->GetMETDay(0) != sMETDays) {
				sMETDays = sts->pMTU->GetMETDay(0);
				oapiVCTriggerRedrawArea(-1, AID_O3_METTMR1);
			}
			//sMETDays = sts->pMTU->GetMETDay(0);
			sMETHours = sts->pMTU->GetMETHour(0);
			sMETMinutes = sts->pMTU->GetMETMin(0);
			sMETSeconds = sts->pMTU->GetMETSec(0);
			
			//sprintf(oapiDebugString(), "Panel A4::%d %d %d %d", sMETDays, sMETHours, sMETMinutes, sMETSeconds);
			oapiVCTriggerRedrawArea(-1, AID_O3_METTMR2);
		}
		break;
	case 2:
		sMETDays = 888;
		sMETHours = 88;
		sMETMinutes = 88;
		sMETSeconds = 88;
		oapiVCTriggerRedrawArea(-1, AID_O3_METTMR1);
		oapiVCTriggerRedrawArea(-1, AID_O3_METTMR2);
		break;
	}
}


void PanelO3::SaveState(FILEHANDLE scn)
{
}

bool PanelO3::ParseScenarioLine (char *line)
{
//	int nNum;
	return false;
}
