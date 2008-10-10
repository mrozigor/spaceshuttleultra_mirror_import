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

		pAFD->DefineVCGroup(MFDGROUPS[MDUID_AFD]);
	}

	void AftMDU::RegisterVC()
	{
		BasicPanel::RegisterVC();

		VECTOR3 ofs = STS()->orbiter_ofs;
		
		oapiVCRegisterArea(AID_MDU_AFD1, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		
		oapiVCSetAreaClickmode_Quadrilateral (AID_MDU_AFD1,
			_V(0.756, 3.120, 12.432)+ofs, _V(0.594, 3.108, 12.364)+ofs,
			_V(0.755, 3.111, 12.431)+ofs, _V(0.595, 3.099, 12.363) + ofs);
	}
};