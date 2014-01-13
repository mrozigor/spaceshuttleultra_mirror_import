#include "PanelR11.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "../Atlantis_defs.h"

namespace vc
{
	PanelR11::PanelR11(Atlantis *_sts)
		: AtlantisPanel(_sts, "R11")
	{
		Add(pCRT4 = new MDU(_sts, "CRT4", MDUID_CRT4, true));
	}

	PanelR11::~PanelR11()
	{
	}

	void PanelR11::DefineVC()
	{
		AddAIDToMouseEventList(AID_R11);

		pCRT4->DefineVCGroup(MFDGROUPS[MDUID_CRT4]);
		pCRT4->SetMouseRegion(0.036582f, 0.020135f, 0.521159f, 0.402556f);
	}

	void PanelR11::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs=STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea(AID_R11, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral(AID_R11,
			_V(1.539, 2.431, 13.608)+ofs, _V(1.539, 2.433, 13.086)+ofs,
			_V(1.047, 2.109, 13.608)+ofs, _V(1.047, 2.110, 13.086)+ofs);
	}
};
