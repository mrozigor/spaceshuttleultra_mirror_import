#include "PanelF7.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "../meshres_vc_additions.h"
#include "MDU.h"

extern GDIParams g_Param;

namespace vc {

	PanelF7::PanelF7(Atlantis* _sts)
		: AtlantisPanel(_sts, "F7")
	{
		Add(pCRT1 = new MDU(_sts, "CRT1", MDUID_CRT1, true));
		Add(pCRT2 = new MDU(_sts, "CRT2", MDUID_CRT2, true));
		Add(pCRT3 = new MDU(_sts, "CRT3", MDUID_CRT3, true));
		Add(pMFD1 = new MDU(_sts, "MFD1", MDUID_MFD1, true));
		Add(pMFD2 = new MDU(_sts, "MFD2", MDUID_MFD2, true));
		
		sTimerMinutes=0;
		sTimerSeconds=0;
	}

	PanelF7::~PanelF7()
	{
	}

	void PanelF7::DefineVC()
	{
		AddAIDToMouseEventList(AID_F7);

		pCRT1->SetMouseRegion(0.0f, 0.0f, 0.330f, 0.5136f);
		pCRT2->SetMouseRegion(0.6715f, 0.0f, 1.0f, 0.5136f);
		pCRT3->SetMouseRegion(0.340f, 0.26f, 0.6715f, 0.75f);
		pMFD1->SetMouseRegion(0.0f, 0.5284f, 0.330f, 1.0f);
		pMFD2->SetMouseRegion(0.6715f, 0.5284f, 1.0f, 1.0f);

		pCRT1->DefineVCGroup(MFDGROUPS[MDUID_CRT1]);
		pCRT2->DefineVCGroup(MFDGROUPS[MDUID_CRT2]);
		pCRT3->DefineVCGroup(MFDGROUPS[MDUID_CRT3]);
		pMFD1->DefineVCGroup(MFDGROUPS[MDUID_MFD1]);
		pMFD2->DefineVCGroup(MFDGROUPS[MDUID_MFD2]);
	}

	void PanelF7::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->orbiter_ofs;
		oapiVCRegisterArea(AID_F7, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		//timer
		SURFHANDLE digit_tex = oapiGetTextureHandle(STS()->hOrbiterVCMesh, TEX_DIGITS_VC);
		oapiVCRegisterArea(AID_F7_EVTTMR1, _R(0, 320, 256, 384), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_CURRENT, digit_tex);	
		
		oapiVCSetAreaClickmode_Quadrilateral (AID_F7, 
			 
			_V(- 0.389, 2.4112365, 14.790174)+ ofs, 
			_V(0.385, 2.4112365, 14.791174) + ofs,
			_V(- 0.389, 1.9662365, 14.664174)+ ofs, 
			_V(0.385, 1.9662365, 14.664174) + ofs);
	}
	
	void PanelF7::OnPostStep(double fSimT, double fDeltaT, double fMJD)
	{		
		//Check forward event timer for changes and update clock if needed
		if(STS()->pMTU->GetEventTimerMin(dps::TIMER_FORWARD) != sTimerMinutes || 
			STS()->pMTU->GetEventTimerSec(dps::TIMER_FORWARD) != sTimerSeconds)
		{
			oapiWriteLog("PanelF7::OnPostStep called");

			sTimerMinutes = STS()->pMTU->GetEventTimerMin(dps::TIMER_FORWARD);
			sTimerSeconds = STS()->pMTU->GetEventTimerSec(dps::TIMER_FORWARD);
			oapiVCTriggerRedrawArea(-1, AID_F7_EVTTMR1);
		}
	}

	bool PanelF7::OnVCRedrawEvent(int id, int _event, SURFHANDLE surf)
	{
		const int NUMX[10] = {64, 0, 64, 128, 192, 256, 320, 384, 448, 0};
		const int NUMY[10] = {384, 448, 448, 448, 448, 448, 448, 448, 448, 384};
		int i;
		int digit[4];
		
		if(id == AID_F7_EVTTMR1)
		{
			oapiWriteLog("PanelF7::OnVCRedrawEvent called");

			digit[0] = sTimerMinutes / 10;
			digit[1] = sTimerMinutes % 10;
			digit[2] = sTimerSeconds / 10;
			digit[3] = sTimerSeconds % 10;

			for(i = 0; i<4; i++)
			{
				oapiBlt(surf, g_Param.digits_7seg, i*64, 0, NUMX[digit[i]], NUMY[digit[i]], 64, 64);
			}
			return true;
		}

		return AtlantisPanel::OnVCRedrawEvent(id, _event, surf);
	}
};
