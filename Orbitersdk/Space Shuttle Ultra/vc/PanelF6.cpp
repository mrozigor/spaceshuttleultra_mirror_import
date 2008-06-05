#include "PanelF6.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "MDU.h"

namespace vc {

	PanelF6::PanelF6(Atlantis* _sts)
		: BasicPanel(_sts, "F6")
	{
		Add(pCDR1 = new MDU(_sts, "CDR1", MDUID_CDR1, true));
		Add(pCDR2 = new MDU(_sts, "CDR2", MDUID_CDR2, true));
	}

	PanelF6::~PanelF6()
	{
	}

	void PanelF6::DefineVC()
	{
		AddAIDToMouseEventList(AID_F6);

		//pCDR1->SetMouseRegion(0.0f, 0.0f, 0.330f, 0.5136f);
		//pCDR2->SetMouseRegion(0.0f, 0.0f, 0.330f, 0.5136f);
		
		pCDR1->DefineVCGroup(MFDGROUPS[MDUID_CDR1]);
		pCDR2->DefineVCGroup(MFDGROUPS[MDUID_CDR2]);
	}

	void PanelF6::RegisterVC()
	{
		VECTOR3 ofs = STS()->orbiter_ofs;
		oapiVCRegisterArea(AID_F6, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral (AID_F6, 
			_V(-0.948, 2.349, 14.787)+ofs, _V(-0.421, 2.349, 14.787)+ofs,
			_V(-0.948, 1.951, 14.68)+ofs, _V(-0.412, 1.951, 14.68) + ofs);
	}
		
};