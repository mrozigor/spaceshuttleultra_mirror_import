#include "AftMDU.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
//#include "../meshres_vc_additions.h"

namespace vc
{
	AftMDU::AftMDU(Atlantis* _sts)
		: BasicPanel(_sts, "AftMDU")
	{
		Add(pAFD = new MDU(_sts, "AFD", MDUID_AFD, true));
	}

	AftMDU::~AftMDU()
	{
	}

	void AftMDU::DefineVC()
	{
		AddAIDToMouseEventList(AID_MDU_AFD1);

		pAFD->SetMouseRegion(0.0, 0.0, 1.0, 1.0);
		pAFD->DefineVCGroup(MFDGROUPS[MDUID_AFD]);
	}

	void AftMDU::RegisterVC()
	{
		BasicPanel::RegisterVC();

		VECTOR3 ofs = STS()->orbiter_ofs;
		
		oapiVCRegisterArea(AID_MDU_AFD1, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		
		oapiVCSetAreaClickmode_Quadrilateral (AID_MDU_AFD1,
			_V(0.764, 3.328, 12.450)+ofs, _V(0.555, 3.313, 12.361)+ofs,
			_V(0.784, 3.090, 12.439)+ofs, _V(0.575, 3.074, 12.353)+ofs);
	}
};
