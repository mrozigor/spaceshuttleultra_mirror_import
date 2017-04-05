#include "AftMDU.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "../Atlantis_defs.h"


namespace vc
{
	AftMDU::AftMDU(Atlantis* _sts)
		: AtlantisPanel(_sts, "AftMDU")
	{
		Add(pAFD = new MDU(_sts, "AFD", MDUID_AFD1, true));
	}

	AftMDU::~AftMDU()
	{
	}

	void AftMDU::DefineVC()
	{
		AddAIDToMouseEventList(AID_MDU_AFD1);

		pAFD->SetMouseRegion(0.0, 0.0, 1.0, 1.0);
		pAFD->DefineVCGroup( GRP_AFD1_VC );
	}

	void AftMDU::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		
		oapiVCRegisterArea(AID_MDU_AFD1, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		
		oapiVCSetAreaClickmode_Quadrilateral (AID_MDU_AFD1,
			_V(0.776, 3.314, 12.458)+ofs, _V(0.543, 3.297, 12.359)+ofs,
			_V(0.793, 3.089, 12.45)+ofs, _V(0.562, 3.072, 12.352)+ofs);
	}
};
