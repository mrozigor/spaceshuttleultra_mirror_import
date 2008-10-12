#include "PanelF8.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "MDU.h"

namespace vc {

	PanelF8::PanelF8(Atlantis* _sts)
		: BasicPanel(_sts, "F8")
	{
		Add(pPLT1 = new MDU(_sts, "PLT1", MDUID_PLT1, true));
		Add(pPLT2 = new MDU(_sts, "PLT2", MDUID_PLT2, true));
	}

	PanelF8::~PanelF8()
	{
	}

	void PanelF8::DefineVC()
	{
		AddAIDToMouseEventList(AID_F8);

		pPLT1->SetMouseRegion(0.0f, 0.0f, 0.491582f, 0.609860f);
		pPLT2->SetMouseRegion(0.511129f, 0.0f, 1.0f, 0.611175f);
		
		pPLT1->DefineVCGroup(MFDGROUPS[MDUID_PLT1]);
		pPLT2->DefineVCGroup(MFDGROUPS[MDUID_PLT2]);
	}

	void PanelF8::RegisterVC()
	{
		BasicPanel::RegisterVC();
		VECTOR3 ofs = STS()->orbiter_ofs;
		oapiVCRegisterArea(AID_F8, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral (AID_F8, 
			_V(0.416, 2.312, 14.755)+ofs, _V(0.942, 2.312, 14.755)+ofs,
			_V(0.416, 1.949, 14.673)+ofs, _V(0.942, 1.949, 14.673) + ofs);
	}
		
};
