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

		pCDR1->SetMouseRegion(0.0f, 0.0f, 0.487004f, 0.615168f);
		pCDR2->SetMouseRegion(0.508892f, 0.0f, 0.993170f, 0.613257f);
		
		pCDR1->DefineVCGroup(MFDGROUPS[MDUID_CDR1]);
		pCDR2->DefineVCGroup(MFDGROUPS[MDUID_CDR2]);
	}

	void PanelF6::RegisterVC()
	{
		BasicPanel::RegisterVC();
		VECTOR3 ofs = STS()->orbiter_ofs;
		
		oapiVCRegisterArea(AID_F6, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral (AID_F6, 
			_V(-0.940, 2.312, 14.756)+ofs, _V(-0.413, 2.312, 14.755)+ofs,
			_V(-0.947, 1.951, 14.674)+ofs, _V(-0.413, 1.949, 14.673)+ofs);
	}
		
};
