#include "PanelF7.h"
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

PanelF7::PanelF7(Atlantis *_sts): sts(_sts)
{
	sTimerMinutes = 0;
	sTimerSeconds = 0;
}

bool PanelF7::VCRedrawEvent(int id, int event, SURFHANDLE surf)
{
	const int NUMX[10] = {64, 0, 64, 128, 192, 256, 320, 384, 448, 0};
	const int NUMY[10] = {384, 448, 448, 448, 448, 448, 448, 448, 448, 384};
	int i;
	int digit[4];

	
	
	if(id == AID_F7_EVTTMR1)
	{
		sprintf(oapiDebugString(), "PanelF7::VCRedrawEvent");

		digit[0] = sTimerMinutes / 10;
		digit[1] = sTimerMinutes % 10;
		digit[2] = sTimerSeconds / 10;
		digit[3] = sTimerSeconds % 10;

		for(i = 0; i<4; i++)
		{
			oapiBlt(surf, g_Param.digits_7seg, i*64, 0, NUMX[digit[i]], NUMY[digit[i]], 63, 63);
		}
	}
	return true;
}

void PanelF7::RegisterVC()
{
	//VECTOR3 ofs = sts->orbiter_ofs;
	SURFHANDLE digit_tex = oapiGetTextureHandle (sts->hOrbiterVCMesh, TEX_DIGITS_VC);
	//oapiVCRegisterArea (AID_C3, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN);
	oapiVCRegisterArea (AID_F7_EVTTMR1, _R(0, 448, 255, 511), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digit_tex);
	//oapiVCSetAreaClickmode_Quadrilateral (AID_C3, _V(-0.281, 1.684, 14.702)+ofs, _V(0.276, 1.684, 14.702)+ofs, _V(-0.281, 1.619, 14.066)+ofs, _V(0.276, 1.619, 14.066)+ofs);
	//oapiVCSetAreaClickmode_Quadrilateral (AID_C3, _V(-0.0804, 1.667, 14.52)+ofs, _V(-0.111, 1.667, 14.52)+ofs, _V(-0.0804, 1.667, 14.52)+ofs, _V(0.276, 1.618, 14.066)+ofs);
	//sprintf(oapiDebugString(), "PanelC3 Registered");
}

bool PanelF7::VCMouseEvent(int id, int event, VECTOR3 &p)
{

	return false;
}

void PanelF7::UpdateVC()
{
	return;
}

void PanelF7::DefineVCAnimations (UINT vcidx)
{
	//static VECTOR3 switch_rot = {-1,0,0};
	return;
}

void PanelF7::Step(double t, double dt)
{
	//Check forward event timer for changes and update clock if needed

	if(sts->pMTU->GetEventTimerMin(TIMER_FORWARD) != sTimerMinutes || 
		sts->pMTU->GetEventTimerSec(TIMER_FORWARD) != sTimerSeconds) {
		sTimerMinutes = sts->pMTU->GetEventTimerMin(TIMER_FORWARD);
		sTimerSeconds = sts->pMTU->GetEventTimerSec(TIMER_FORWARD);
		oapiVCTriggerRedrawArea(-1, AID_F7_EVTTMR1);
	}
}


void PanelF7::SaveState(FILEHANDLE scn)
{
}

bool PanelF7::ParseScenarioLine (char *line)
{
//	int nNum;
	return false;
}
