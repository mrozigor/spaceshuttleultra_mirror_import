#include "PanelF4.h"
#include "../meshres_vc_additions.h"

extern GDIParams g_Param;

namespace vc
{
	PanelF4::PanelF4(Atlantis* _sts)
		: AtlantisPanel(_sts, "F4")
	{
		Add(pSbdbkThrot = new PushButtonIndicator(_sts, "F4_SBDTHROT_AUTO"));
		Add(pSbdbkThrotMan = new StandardLight(_sts, "F4_SBDKTHROT_MAN"));
		Add(pBodyFlap = new PushButtonIndicator(_sts, "F4_BODYFLAP_AUTO"));
		Add(pBodyFlapMan = new StandardLight(_sts, "F4_BODYFLAP_MAN"));

		Add(pPitchAuto = new PushButtonIndicator(_sts, "F4_PITCH_AUTO", true));
		Add(pPitchCSS = new PushButtonIndicator(_sts, "F4_PITCH_CSS", true));
		Add(pRollYawAuto = new PushButtonIndicator(_sts, "F4_RY_AUTO", true));
		Add(pRollYawCSS = new PushButtonIndicator(_sts, "F4_RY_CSS", true));
	}

	PanelF4::~PanelF4()
	{
	}

	void PanelF4::Realize()
	{
		DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("SBDBKTHROT_CONTROLS", 16);
		pSbdbkThrot->ConnectAll(pBundle, 0); //AUTO light; common to F2 and F4 PBIs
		pSbdbkThrotMan->input.Connect(pBundle, 2); //MAN light; F4 PBI only
		pSbdbkThrotMan->test.Connect(pBundle, 2); //MAN light; F4 PBI only

		pBundle=STS()->BundleManager()->CreateBundle("BODYFLAP_CONTROLS", 16);
		pBodyFlap->ConnectAll(pBundle, 0); //AUTO light; common to F2 and F4 PBIs
		pBodyFlapMan->input.Connect(pBundle, 1); //MAN light; common to F2 and F4 PBIs
		pBodyFlapMan->test.Connect(pBundle, 1); //MAN light; common to F2 and F4 PBIs

		pBundle=STS()->BundleManager()->CreateBundle("CSS_CONTROLS", 4);
		pPitchAuto->ConnectAll(pBundle, 0);
		pPitchCSS->ConnectAll(pBundle, 1);
		PitchPortGroup.AddPorts(pBundle, 0, 1);
		pRollYawAuto->ConnectAll(pBundle, 2);
		pRollYawCSS->ConnectAll(pBundle, 3);
		RollYawPortGroup.AddPorts(pBundle, 2, 3);
		
		AtlantisPanel::Realize();
	}

	void PanelF4::OnPreStep(double SimT, double DeltaT, double MJD)
	{
		PitchPortGroup.OnPreStep();
		RollYawPortGroup.OnPreStep();

		AtlantisPanel::OnPreStep(SimT, DeltaT, MJD);
	}

	void PanelF4::DefineVC()
	{
		AddAIDToMouseEventList(AID_F4);

		pSbdbkThrot->AddAIDToRedrawEventList(AID_F4_ST_AUTO);
		pSbdbkThrot->SetSourceImage(g_Param.pbi_lights);
		pSbdbkThrot->SetBase(0, 0);
		pSbdbkThrot->SetSourceCoords(true, 0, 0);
		pSbdbkThrot->SetSourceCoords(false, 0, 14);
		pSbdbkThrot->SetDimensions(42, 14);
		pSbdbkThrot->SetMouseRegion(0.823518f, 0.583791f, 0.867013f, 0.697191f);
		pSbdbkThrot->AllowReset(true);

		pSbdbkThrotMan->AddAIDToRedrawEventList(AID_F4_ST_MAN);
		pSbdbkThrotMan->SetSourceImage(g_Param.pbi_lights);
		pSbdbkThrotMan->SetBase(0, 0);
		pSbdbkThrotMan->SetSourceCoords(true, 0, 0);
		pSbdbkThrotMan->SetSourceCoords(false, 0, 14);
		pSbdbkThrotMan->SetDimensions(42, 14);

		pBodyFlap->AddAIDToRedrawEventList(AID_F4_BF_AUTO);
		pBodyFlap->SetSourceImage(g_Param.pbi_lights);
		pBodyFlap->SetBase(0, 0);
		pBodyFlap->SetSourceCoords(true, 0, 0);
		pBodyFlap->SetSourceCoords(false, 0, 14);
		pBodyFlap->SetDimensions(42, 14);
		pBodyFlap->SetMouseRegion(0.872915f, 0.660717f, 0.916913f, 0.770096f);
		pBodyFlap->AllowReset(true);

		pBodyFlapMan->AddAIDToRedrawEventList(AID_F4_BF_MAN);
		pBodyFlapMan->SetSourceImage(g_Param.pbi_lights);
		pBodyFlapMan->SetBase(0, 0);
		pBodyFlapMan->SetSourceCoords(true, 0, 0);
		pBodyFlapMan->SetSourceCoords(false, 0, 14);
		pBodyFlapMan->SetDimensions(42, 14);

		pPitchAuto->AddAIDToRedrawEventList(AID_F4_P_AUTO);
		pPitchAuto->SetSourceImage(g_Param.pbi_lights);
		pPitchAuto->SetBase(0, 0);
		pPitchAuto->SetSourceCoords(true, 0, 0);
		pPitchAuto->SetSourceCoords(false, 0, 14);
		pPitchAuto->SetDimensions(42, 14);
		pPitchAuto->SetMouseRegion(0.635229f, 0.286244f, 0.677304f, 0.395611f);

		pPitchCSS->AddAIDToRedrawEventList(AID_F4_P_CSS);
		pPitchCSS->SetSourceImage(g_Param.pbi_lights);
		pPitchCSS->SetBase(0, 0);
		pPitchCSS->SetSourceCoords(true, 0, 0);
		pPitchCSS->SetSourceCoords(false, 0, 14);
		pPitchCSS->SetDimensions(42, 14);
		pPitchCSS->SetMouseRegion(0.681582f, 0.358571f, 0.724378f, 0.468767f);

		pRollYawAuto->AddAIDToRedrawEventList(AID_F4_RY_AUTO);
		SetCommonPBIParameters(pRollYawAuto);
		pRollYawAuto->SetMouseRegion(0.613049f, 0.389149f, 0.656107f, 0.494769f);

		pRollYawCSS->AddAIDToRedrawEventList(AID_F4_RY_CSS);
		SetCommonPBIParameters(pRollYawCSS);
		pRollYawCSS->SetMouseRegion(0.658947f, 0.459885f, 0.702498f, 0.573177f);
	}

	void PanelF4::SetCommonPBIParameters(PushButtonIndicator* pPBI)
	{
		pPBI->SetSourceImage(g_Param.pbi_lights);
		pPBI->SetBase(0, 0);
		pPBI->SetSourceCoords(true, 0, 0);
		pPBI->SetSourceCoords(false, 0, 14);
		pPBI->SetDimensions(42, 14);
	}

	void PanelF4::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		SURFHANDLE tex=oapiGetTextureHandle(STS()->hOrbiterVCMesh, TEX_FPANELS_VC);

		VECTOR3 ofs=STS()->orbiter_ofs;

		oapiVCRegisterArea(AID_F4, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP);
		oapiVCSetAreaClickmode_Quadrilateral(AID_F4,
			_V(0.38, 2.535, 14.612)+ofs, _V(1.147, 2.535, 14.612)+ofs,
			_V(0.38, 2.25, 14.533)+ofs, _V(1.147, 2.25, 14.533)+ofs);

		oapiVCRegisterArea(AID_F4_ST_AUTO, _R(1296, 548, 1338, 562), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F4_ST_MAN, _R(1296, 585, 1338, 599), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F4_BF_AUTO, _R(1436, 542, 1478, 556), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F4_BF_MAN, _R(1437, 583, 1479, 597), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F4_P_AUTO, _R(773, 562, 815, 576), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F4_P_CSS, _R(899, 565, 941, 579), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F4_RY_AUTO, _R(773, 678, 815, 692), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
		oapiVCRegisterArea(AID_F4_RY_CSS, _R(901, 680, 943, 694), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, tex);
	}
};