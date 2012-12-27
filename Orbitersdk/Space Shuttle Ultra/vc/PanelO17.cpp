#include "PanelO17.h"
#include "../Atlantis.h"

namespace vc
{

PanelO17::PanelO17(Atlantis* psts)
: AtlantisPanel(psts, "O17")
{
	Add(pEIUPowerLC = new StdSwitch2(psts, "EIU L-C"));
	Add(pEIUPowerCR = new StdSwitch2(psts, "EIU C-R"));
	Add(pEIUPowerRL = new StdSwitch2(psts, "EIU R-L"));

	pEIUPowerLC->SetLabel(1, "OFF");
	pEIUPowerLC->SetLabel(0, "ON");
	pEIUPowerLC->SetOnPosition(0);
	pEIUPowerCR->SetLabel(1, "OFF");
	pEIUPowerCR->SetLabel(0, "ON");
	pEIUPowerCR->SetOnPosition(0);
	pEIUPowerRL->SetLabel(1, "OFF");
	pEIUPowerRL->SetLabel(0, "ON");
	pEIUPowerRL->SetOnPosition(0);
}

PanelO17::~PanelO17()
{
}

void PanelO17::DefineVC()
{
	const VECTOR3 SWITCH_ROT = _V(-0.788502306017, 0.615031798693, 0.0);
	AddAIDToMouseEventList(AID_O17);

	pEIUPowerLC->SetInitialAnimState(0.5f);
	pEIUPowerLC->DefineSwitchGroup(GRP_O17S8_VC);
	pEIUPowerLC->SetReference(_V(0.7605, 3.1545, 13.336), SWITCH_ROT);
	pEIUPowerLC->SetMouseRegion(0.073f, 0.625f, 0.159f, 0.7f);

	pEIUPowerCR->SetInitialAnimState(0.5f);
	pEIUPowerCR->DefineSwitchGroup(GRP_O17S7_VC);
	pEIUPowerCR->SetReference(_V(0.78925, 3.1325, 13.336), SWITCH_ROT);
	pEIUPowerCR->SetMouseRegion(0.171f, 0.625f, 0.265f, 0.7f);

	pEIUPowerRL->SetInitialAnimState(0.5f);
	pEIUPowerRL->DefineSwitchGroup(GRP_O17S9_VC);
	pEIUPowerRL->SetReference(_V(0.81725, 3.1105, 13.3365), SWITCH_ROT);
	pEIUPowerRL->SetMouseRegion(0.270f, 0.625f, 0.362f, 0.7f);
}

void PanelO17::RegisterVC()
{
	AtlantisPanel::RegisterVC();
	VECTOR3 ofs = STS()->orbiter_ofs;

	oapiVCRegisterArea(AID_O17, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP);
	oapiVCSetAreaClickmode_Quadrilateral(AID_O17, 
										 _V(0.721, 3.186, 13.609)+ofs, _V(1.021, 2.949, 13.609)+ofs, 
										 _V(0.721, 3.186, 13.205) + ofs, _V(1.021, 2.952, 13.242)+ofs);
}

void PanelO17::Realize()
{
	DiscreteBundle* O17_to_EIU_AC = STS()->BundleManager()->CreateBundle( "O17_to_EIU_AC", 3 );

	pEIUPowerLC->output.Connect( O17_to_EIU_AC, 0 );// AC2
	pEIUPowerCR->output.Connect( O17_to_EIU_AC, 1 );// AC1
	pEIUPowerRL->output.Connect( O17_to_EIU_AC, 2 );// AC3

	AtlantisPanel::Realize();
}

};
