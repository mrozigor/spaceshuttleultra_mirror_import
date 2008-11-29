#include "PanelF2.h"
#include "../meshres_vc_additions.h"

extern GDIParams g_Param;

namespace vc
{
	PanelF2::PanelF2(Atlantis* _sts)
		: BasicPanel(_sts, "F2")
	{
		Add(pSbdbkThrot = new PushButtonIndicator(_sts, "F2_SBDTHROT_AUTO"));
		Add(pSbdbkThrotMan = new StandardLight(_sts, "F2_SBDKTHROT_MAN"));
		Add(pBodyFlap = new PushButtonIndicator(_sts, "F2_BODYFLAP_AUTO"));
		Add(pBodyFlapMan = new StandardLight(_sts, "F2_BODYFLAP_MAN"));
	}

	PanelF2::~PanelF2()
	{
	}

	void PanelF2::Realize()
	{
		BasicPanel::Realize();

		DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("SBDBKTHROT_CONTROLS", 16);
		pSbdbkThrot->ConnectAll(pBundle, 0); //AUTO light; common to F2 and F4 PBIs
		pSbdbkThrotMan->input.Connect(pBundle, 1); //MAN light; F2 PBI only
		pSbdbkThrotMan->test.Connect(pBundle, 1); //MAN light; F2 PBI only

		pBundle=STS()->BundleManager()->CreateBundle("BODYFLAP_CONTROLS", 16);
		pBodyFlap->ConnectAll(pBundle, 0); //AUTO light; common to F2 and F4 PBIs
		pBodyFlapMan->input.Connect(pBundle, 1); //MAN light; common to F2 and F4 PBIs
		pBodyFlapMan->test.Connect(pBundle, 1); //MAN light; common to F2 and F4 PBIs
	}

	void PanelF2::DefineVC()
	{
		AddAIDToMouseEventList(AID_F2);

		pSbdbkThrot->AddAIDToRedrawEventList(AID_F2_ST_AUTO);
		pSbdbkThrot->SetSourceImage(g_Param.pbi_lights);
		pSbdbkThrot->SetBase(0, 0);
		pSbdbkThrot->SetSourceCoords(true, 0, 0);
		pSbdbkThrot->SetSourceCoords(false, 0, 14);
		pSbdbkThrot->SetDimensions(42, 14);
		pSbdbkThrot->SetMouseRegion(0.133675f, 0.579283f, 0.176833f, 0.687911f);
		pSbdbkThrot->AllowReset(true);

		pSbdbkThrotMan->AddAIDToRedrawEventList(AID_F2_ST_MAN);
		pSbdbkThrotMan->SetSourceImage(g_Param.pbi_lights);
		pSbdbkThrotMan->SetBase(0, 0);
		pSbdbkThrotMan->SetSourceCoords(true, 0, 0);
		pSbdbkThrotMan->SetSourceCoords(false, 0, 14);
		pSbdbkThrotMan->SetDimensions(42, 14);

		pBodyFlap->AddAIDToRedrawEventList(AID_F2_BF_AUTO);
		pBodyFlap->SetSourceImage(g_Param.pbi_lights);
		pBodyFlap->SetBase(0, 0);
		pBodyFlap->SetSourceCoords(true, 0, 0);
		pBodyFlap->SetSourceCoords(false, 0, 14);
		pBodyFlap->SetDimensions(42, 14);
		pBodyFlap->SetMouseRegion(0.089873f, 0.6449937f, 0.131254f, 0.757229f);
		pBodyFlap->AllowReset(true);

		pBodyFlapMan->AddAIDToRedrawEventList(AID_F2_BF_MAN);
		pBodyFlapMan->SetSourceImage(g_Param.pbi_lights);
		pBodyFlapMan->SetBase(0, 0);
		pBodyFlapMan->SetSourceCoords(true, 0, 0);
		pBodyFlapMan->SetSourceCoords(false, 0, 14);
		pBodyFlapMan->SetDimensions(42, 14);
	}

	void PanelF2::RegisterVC()
	{
		BasicPanel::RegisterVC();

		SURFHANDLE tex=oapiGetTextureHandle(STS()->hOrbiterVCMesh, TEX_FPANELS_VC);

		VECTOR3 ofs=STS()->orbiter_ofs;

		oapiVCRegisterArea(AID_F2, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP);
		oapiVCSetAreaClickmode_Quadrilateral(AID_F2,
			_V(-1.154, 2.535, 14.612)+ofs, _V(-0.385, 2.535, 14.612)+ofs,
			_V(-1.154, 2.25, 14.533)+ofs, _V(-0.385, 2.25, 14.533)+ofs);

		oapiVCRegisterArea(AID_F2_ST_AUTO, _R(867, 255, 909, 269), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F2_ST_MAN, _R(867, 293, 909, 307), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F2_BF_AUTO, _R(737, 252, 779, 266), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F2_BF_MAN, _R(738, 294, 780, 308), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
	}
};