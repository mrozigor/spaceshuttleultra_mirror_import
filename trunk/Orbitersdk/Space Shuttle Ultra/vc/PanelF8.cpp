#include "PanelF8.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "MDU.h"
#include "../Atlantis_defs.h"

namespace vc {

	PanelF8::PanelF8(Atlantis* _sts)
		: AtlantisPanel(_sts, "F8")
	{
		Add(pPLT1 = new MDU(_sts, "PLT1", MDUID_PLT1, true));
		Add(pPLT2 = new MDU(_sts, "PLT2", MDUID_PLT2, true));
		Add(pFltCntlrPower = new StdSwitch2(_sts, "Plt Flt Cntlr Pwr"));

		pFltCntlrPower->SetLabel(0, "OFF");
		pFltCntlrPower->SetLabel(1, "ON");
	}

	PanelF8::~PanelF8()
	{
	}

	void PanelF8::DefineVC()
	{
		VECTOR3 switch_rot = _V(1, 0, 0);

		AddAIDToMouseEventList(AID_F8);

		pPLT1->SetMouseRegion(0.0f, 0.0f, 0.491582f, 0.609860f);
		pPLT1->SetEdgekeyArea(0.210163f, 0.915644f, 0.783473f, 0.973985f);
		pPLT2->SetMouseRegion(0.511129f, 0.0f, 1.0f, 0.611175f);
		pPLT2->SetEdgekeyArea(0.210163f, 0.915644f, 0.783473f, 0.973985f);
		
		pPLT1->DefineVCGroup(MFDGROUPS[MDUID_PLT1]);
		pPLT2->DefineVCGroup(MFDGROUPS[MDUID_PLT2]);

		pFltCntlrPower->DefineSwitchGroup(GRP_F8SWITCH5_VC);
		pFltCntlrPower->SetInitialAnimState(0.5);
		pFltCntlrPower->SetReference(_V(-0.734, 1.99, 14.685), switch_rot);
		pFltCntlrPower->SetMouseRegion(0.372206f, 0.851179f, 0.428169f, 0.928952f);
	}

	void PanelF8::RegisterVC()
	{
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		oapiVCRegisterArea(AID_F8, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral (AID_F8, 
			_V(0.416, 2.312, 14.755)+ofs, _V(0.942, 2.312, 14.755)+ofs,
			_V(0.416, 1.949, 14.673)+ofs, _V(0.942, 1.949, 14.673) + ofs);
	}

	void PanelF8::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle("Controllers", 16);
		pFltCntlrPower->ConnectPort(1, pBundle, 2);
		
		AtlantisPanel::Realize();
	}
		
};
