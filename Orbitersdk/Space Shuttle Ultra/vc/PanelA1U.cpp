#include "PanelA1U.h"
#include "../meshres_vc_additions.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"

extern GDIParams g_Param;

const VECTOR3 S1_REF = _V(0.0, 3.048204, 12.22);
const VECTOR3 S3_REF = _V(0.0, 2.97289, 12.22);
const VECTOR3 S5_REF = _V(0.0, 2.91259, 12.22);

namespace vc
{
	PanelA1U::PanelA1U(Atlantis* _sts)
		: AtlantisPanel(_sts, "A1U")
	{
		Add(pSignalStrengthSelect = new StdSwitch3(_sts, "SIG STR"));
		pSignalStrengthSelect->SetLabel(0, "SBAND FM");
		pSignalStrengthSelect->SetLabel(1, "KU");
		pSignalStrengthSelect->SetLabel(2, "SBAND PM");
	}

	PanelA1U::~PanelA1U()
	{
	}

	void PanelA1U::OnPreStep(double SimT, double DeltaT, double MJD)
	{
		return;
	}

	void PanelA1U::DefineVC()
	{
		//const VECTOR3 switch_rot_vert =  _V(-1, 0, 0);
		//const VECTOR3 switch_rot_horz = _V(0.0, -0.9499, 0.3126);
		//VECTOR3 switch_pull = _V(0, 0.3126, 0.9499);

		AddAIDToMouseEventList(AID_A1U1);
		AddAIDToMouseEventList(AID_A1U2);
	}

	void PanelA1U::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea(AID_A1U1, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral(AID_A1U1,
			_V(-0.264, 2.893, 12.277)+ofs, _V(0.255, 2.893, 12.277)+ofs,
			_V(-0.264, 2.507, 12.397)+ofs, _V(0.255, 2.507, 12.397)+ofs);

		oapiVCRegisterArea(AID_A1U2, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);
		oapiVCSetAreaClickmode_Quadrilateral(AID_A1U2,
			_V(-0.264, 2.893, 12.277)+ofs, _V(0.255, 2.893, 12.277)+ofs,
			_V(-0.264, 2.507, 12.397)+ofs, _V(0.255, 2.507, 12.397)+ofs);
	}

	void PanelA1U::Realize()
	{
		AtlantisPanel::Realize();
	}
};