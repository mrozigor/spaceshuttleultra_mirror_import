#include "PanelF6.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "MDU.h"

namespace vc {

	PanelF6::PanelF6(Atlantis* _sts)
		: AtlantisPanel(_sts, "F6")
	{
		Add(pCDR1 = new MDU(_sts, "CDR1", MDUID_CDR1, true));
		Add(pCDR2 = new MDU(_sts, "CDR2", MDUID_CDR2, true));
		Add(pFltCntlrPower = new StdSwitch2(_sts, "Cdr Flt Cntlr Pwr"));

		pFltCntlrPower->SetLabel(0, "OFF");
		pFltCntlrPower->SetLabel(1, "ON");
	}

	PanelF6::~PanelF6()
	{
	}

	void PanelF6::DefineVC()
	{
		VECTOR3 switch_rot = _V(1, 0, 0);

		AddAIDToMouseEventList(AID_F6);

		pCDR1->SetMouseRegion(0.0f, 0.0f, 0.487004f, 0.615168f);
		pCDR2->SetMouseRegion(0.508892f, 0.0f, 0.993170f, 0.613257f);
		
		pCDR1->DefineVCGroup(MFDGROUPS[MDUID_CDR1]);
		pCDR2->DefineVCGroup(MFDGROUPS[MDUID_CDR2]);

		pFltCntlrPower->DefineSwitchGroup(GRP_F6SWITCH_5_VC);
		pFltCntlrPower->SetInitialAnimState(0.5);
		pFltCntlrPower->SetReference(_V(-0.734, 1.99, 14.685), switch_rot);
		pFltCntlrPower->SetMouseRegion(0.372206f, 0.851179f, 0.428169f, 0.928952f);
	}

	void PanelF6::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->orbiter_ofs;
		
		oapiVCRegisterArea(AID_F6, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral (AID_F6, 
			_V(-0.940, 2.312, 14.756)+ofs, _V(-0.413, 2.312, 14.755)+ofs,
			_V(-0.947, 1.951, 14.674)+ofs, _V(-0.413, 1.949, 14.673)+ofs);
	}

	void PanelF6::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle("Controllers", 16);
		pFltCntlrPower->ConnectPort(1, pBundle, 1);

		AtlantisPanel::Realize();
	}
		
};
