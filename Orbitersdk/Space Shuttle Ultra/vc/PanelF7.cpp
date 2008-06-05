#include "PanelF7.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "MDU.h"

namespace vc {

	PanelF7::PanelF7(Atlantis* _sts)
		: BasicPanel(_sts, "F7")
	{
		Add(pCRT1 = new MDU(_sts, "CRT1", MDUID_CRT1, true));
		Add(pCRT2 = new MDU(_sts, "CRT2", MDUID_CRT2, true));
		Add(pCRT3 = new MDU(_sts, "CRT3", MDUID_CRT3, true));
		Add(pMFD1 = new MDU(_sts, "MFD1", MDUID_MFD1, true));
		Add(pMFD2 = new MDU(_sts, "MFD2", MDUID_MFD2, true));
		
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
		VECTOR3 ofs = STS()->orbiter_ofs;
		oapiVCRegisterArea(AID_F7, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral (AID_F7, 
			_V(-0.389, 2.407, 14.804)+ofs, _V(0.385, 2.407, 14.805)+ofs,
			_V(-0.389, 1.962, 14.678)+ofs, _V(0.385, 1.962, 14.678) + ofs);
	}
		
};