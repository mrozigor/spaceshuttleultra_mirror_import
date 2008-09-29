#include "PanelO3.h"
#include "resource.h"
#include "meshres.h"
#include "meshres_vc.h"
#include "meshres_vc_additions.h"
#include "DlgCtrl.h"
#include <stdio.h>
#include "dps/MasterTimingUnit.h"
//#include "Stopwatch.h"

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
		anim_VC_O3[i] = NULL;
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

	//Stopwatch st;

	//METTMR1 = Days
	//METTMR2 = Rest

	//st.Start();
	
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

//	double time_for_O3 = st.Stop();

//	if(time_for_O3 > 1000.0)
//	{
//		char buffer[100];
//		sprintf(buffer, "Panel O3 needed for than 1 ms for redraw (T = %f µs)\n", time_for_O3);
//		oapiWriteLog(buffer);
//	}

	return true;
}

void PanelO3::RegisterVC()
{
	VECTOR3 ofs = sts->orbiter_ofs;
	SURFHANDLE digit_tex = oapiGetTextureHandle (sts->hOrbiterVCMesh, TEX_DIGITS_VC);

	//Mesh data for this panel
	
	//0.110078, 3.0096, 14.3151
	//0.713058, 3.0096, 14.3151
	//0.110078, 2.72832, 14.3729
	//0.713058, 2.72832, 14.3729
	

	oapiVCRegisterArea (AID_O3, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
	oapiVCSetAreaClickmode_Quadrilateral (AID_O3,  _V(0.110078, 3.0096, 14.3151)+ofs, _V(0.713058, 3.0096, 14.3151)+ofs,
		  _V(0.110078, 2.72832, 14.3729)+ofs , _V(0.713058, 2.72832, 14.3729)+ofs);
	
	oapiVCRegisterArea (AID_O3_METTMR1, _R(320, 0, 512, 64), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digit_tex);	
	oapiVCRegisterArea (AID_O3_METTMR2, _R(0, 64, 384, 128), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digit_tex);	
	oapiVCRegisterArea (AID_O3_RCS, _R(0, 256, 384, 320), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digit_tex);	

	//sprintf(oapiDebugString(), "O3 Registered");
}

bool PanelO3::VCMouseEvent(int id, int event, VECTOR3 &p)
{
	if (id != AID_O3) return false;
	bool action = false;
	sprintf_s(oapiDebugString(),255, "PANEL O3: id %d event %d p %f %f %f",id,event,p.x,p.y,p.z);

	if(p.x>0.642 && p.x < 0.693)
	{
		if(p.y > 0.703 && p.y < 0.748)
		{	
			if(switch_state[SWITCH_O3S12] > 0)
			{
				switch_state[SWITCH_O3S12]--;
				switch_timer[SWITCH_O3S12] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
		else if(p.y > 0.748 && p.y < 0.792)
		{
			if(switch_state[SWITCH_O3S12] < 2)
			{
				switch_state[SWITCH_O3S12]++;
				switch_timer[SWITCH_O3S12] = SWITCH_HOLD_TIME;
				action = true;
			}
		}
	}
	if(p.x>=0.391509 && p.x<=0.455177 && p.y>=0.685045 && p.y<=0.868811) {
		UINT x=(UINT)(O3_NUM_POSITIONS[SWITCH_O3S11]*(p.x-0.391509)/(0.455177-0.391509));
		//switch_state[SWITCH_O3S11]=2;
		if(x<switch_state[SWITCH_O3S11] && switch_state[SWITCH_O3S11]>0)
			switch_state[SWITCH_O3S11]--;
		else if(x>switch_state[SWITCH_O3S11] && switch_state[SWITCH_O3S11]<4)
			switch_state[SWITCH_O3S11]++;
		action=true;
	}

	if(action)
	{
		UpdateVC();
	}
	return false;
}

void PanelO3::UpdateVC()
{
	int i;
	for(i = 0; i<NUM_O3_SWITCHES; i++)
	{
		if(anim_VC_O3[i])
		{
			//sts->SetAnimation(anim_VC_O3[i], switch_state[i] * 0.5);
			sts->SetAnimation(anim_VC_O3[i], switch_state[i]/((double)O3_NUM_POSITIONS[i]-1));
		}
	}
	return;
}

void PanelO3::DefineVCAnimations (UINT vcidx)
{
	
	//_V(-0.651648, 3.07024, 12.5771)+ofs, _V(-0.865687, 3.07024, 12.6344)+ofs
	//_V(-0.214039, 0, 0.0573);	//L=0.221576134
	static VECTOR3 switch_rot = {-1.0, 0.0, 0.0};

	static UINT VC_O3S12_GRP = GRP_O3S12_VC;

	static MGROUP_ROTATE VC_O3S12 (vcidx, &VC_O3S12_GRP, 1,
		REF_O3S12, switch_rot, (float)(90.0*RAD));
	anim_VC_O3[SWITCH_O3S12]=sts->CreateAnimation (0.5);
	sts->AddAnimationComponent (anim_VC_O3[SWITCH_O3S12], 0, 1, &VC_O3S12);

	static UINT VC_O3S11_GRP = GRP_O3S11_VC;
	static MGROUP_ROTATE VC_O3S11 (vcidx, &VC_O3S11_GRP, 1,
		REF_O3S11, _V(0, 0.1984, 0.9801), (float)(100.0*RAD));
	anim_VC_O3[SWITCH_O3S11]=sts->CreateAnimation(0.75);
	sts->AddAnimationComponent(anim_VC_O3[SWITCH_O3S11], 0, 1, &VC_O3S11);
	
	return;
}

void PanelO3::Step(double t, double dt)
{
	bool update = false;
	bool UpdateRCSgauge=false;
	short fuel;
	//Check forward event timer for changes and update clock if needed

	switch(switch_state[SWITCH_O3S12]) {
	case 0:
		if(sts->pMTU->GetGMTDay(0) != sMETDays || 
			sts->pMTU->GetGMTHour(0) != sMETHours ||
			sts->pMTU->GetGMTSec(0) != sMETSeconds ||
			sts->pMTU->GetGMTMin(0) != sMETMinutes) 
		{
			if(sts->pMTU->GetGMTDay(0) != sMETDays) 
			{
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
			sts->pMTU->GetMETMin(0) != sMETMinutes) 
		{
			if(sts->pMTU->GetMETDay(0) != sMETDays) 
			{
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
		if(sMETDays != 888)
		{
			sMETDays = 888;
			sMETHours = 88;
			sMETMinutes = 88;
			sMETSeconds = 88;
			oapiVCTriggerRedrawArea(-1, AID_O3_METTMR1);
			oapiVCTriggerRedrawArea(-1, AID_O3_METTMR2);
		}
		break;
	}

	switch(switch_state[SWITCH_O3S11]) {
	case 0: //RCS OXID
	case 1: //RCS FUEL
		fuel=min(99, sts->GetPropellantLevel(sts->ph_lrcs));
		if(sOMSFuel[0]!=fuel) {
			sOMSFuel[0]=fuel;
			UpdateRCSgauge=true;
		}
		fuel=min(99, sts->GetPropellantLevel(sts->ph_frcs));
		if(sOMSFuel[1]!=fuel) {
			sOMSFuel[1]=fuel;
			UpdateRCSgauge=true;
		}
		fuel=min(99, sts->GetPropellantLevel(sts->ph_rrcs));
		if(sOMSFuel[2]!=fuel) {
			sOMSFuel[2]=fuel;
			UpdateRCSgauge=true;
		}
		if(UpdateRCSgauge) oapiVCTriggerRedrawArea(-1, AID_O3_RCS);
		break;
	case 2: //LOWEST
		break;
	case 3: //OMS OXID
	case 4: //OMS FUEL
		fuel=min(99, sts->GetPropellantLevel(sts->ph_oms));
		if(sOMSFuel[0]!=fuel || sOMSFuel[1]!=0 || sOMSFuel[2]!=fuel) {
			sOMSFuel[0]=fuel;
			sOMSFuel[1]=0;
			sOMSFuel[2]=fuel;
			oapiVCTriggerRedrawArea(-1, AID_O3_RCS);
		}
		break;
	}

	if(switch_state[SWITCH_O3S12] == 2)
	{
		switch_timer[SWITCH_O3S12] -= dt;
		if(switch_timer[SWITCH_O3S12] <=0.0)
		{
			switch_state[SWITCH_O3S12] = 1;
			update = true;
		}
	}
	if(update)
	{
		UpdateVC();
	}
}


void PanelO3::SaveState(FILEHANDLE scn)
{

	char buffer[100];
	sprintf_s(buffer, 99, "PANELO3 %d %d %d %d %d %d %d", 
		switch_state[0], 
		switch_state[1], 
		switch_state[2],
		switch_state[3],
		switch_state[4],
		switch_state[5],
		switch_state[6]);
	oapiWriteLine(scn, buffer);
}

bool PanelO3::ParseScenarioLine (char *line)
{
//	int nNum;
	return false;
}
