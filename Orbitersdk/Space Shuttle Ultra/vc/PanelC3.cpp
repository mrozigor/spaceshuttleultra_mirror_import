#include "PanelC3.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "../meshres_vc_additions.h"

extern GDIParams g_Param;

namespace vc
{
	PanelC3::PanelC3(Atlantis *_sts)
		: BasicPanel(_sts, "C3")
	{
		char cbuf[5];

		for(int i=0;i<24;i++) {
			sprintf_s(cbuf, 5, "%d", i+1);
			std::string name="A6_PBI";
			name+=cbuf;
			Add(pPBIs[i]=new PushButtonIndicator(_sts, name));
		}

		Add(pOMSArm[0] = new LockableLever3(_sts, "LOMS Arm"));
		Add(pOMSArm[1] = new LockableLever3(_sts, "ROMS Arm"));
	}

	PanelC3::~PanelC3()
	{
	}

	void PanelC3::RegisterVC()
	{
		oapiWriteLog("Registering Panel C3");
		BasicPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset();
		SURFHANDLE panelc3_tex = oapiGetTextureHandle (STS()->hOrbiterVCMesh, TEX_PANELC3_VC);
		oapiVCRegisterArea (AID_C3, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED);

		oapiVCSetAreaClickmode_Quadrilateral (AID_C3, _V(-0.2732771, 1.76045, 14.35651)+ofs, _V(0.2732771, 1.76045, 14.35651)+ofs, 
			_V(-0.2732771, 1.69551,  13.73551)+ofs, _V(0.2732771, 1.69551,  13.73551)+ofs);

		//register DAP PBIs
		//A
		oapiVCRegisterArea(AID_C3_PBI1, _R(155, 888, 197, 902), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//B
		oapiVCRegisterArea(AID_C3_PBI2, _R(250, 887, 292, 901), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//AUTO
		oapiVCRegisterArea(AID_C3_PBI3, _R(344, 888, 386, 902), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//INRTL
		oapiVCRegisterArea(AID_C3_PBI4, _R(434, 889, 476, 903), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//LVLH
		oapiVCRegisterArea(AID_C3_PBI5, _R(527, 889, 569, 903), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//FREE
		oapiVCRegisterArea(AID_C3_PBI6, _R(622, 889, 664, 903), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//PCT
		oapiVCRegisterArea(AID_C3_PBI7, _R(157, 1089, 199, 1103), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//LOW Z
		oapiVCRegisterArea(AID_C3_PBI8, _R(247, 1089, 289, 1103), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//HIGH Z
		oapiVCRegisterArea(AID_C3_PBI9, _R(339, 1090, 381, 1104), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//PRI
		oapiVCRegisterArea(AID_C3_PBI10, _R(440, 1089, 482, 1103), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//ALT
		oapiVCRegisterArea(AID_C3_PBI11, _R(531, 1090, 573, 1104), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//VERN
		oapiVCRegisterArea(AID_C3_PBI12, _R(623, 1090, 665, 1104), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//TRANS X NORM
		oapiVCRegisterArea(AID_C3_PBI13, _R(156, 1186, 198, 1200), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//TRANS Y NORM
		oapiVCRegisterArea(AID_C3_PBI14, _R(249, 1186, 291, 1200), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//TRANS Z NORM
		oapiVCRegisterArea(AID_C3_PBI15, _R(338, 1187, 380, 1201), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//ROT ROLL DISC RATE
		oapiVCRegisterArea(AID_C3_PBI16, _R(439, 1186, 481, 1200), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//ROT PITCH DISC RATE
		oapiVCRegisterArea(AID_C3_PBI17, _R(530, 1185, 572, 1199), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//ROT YAW DISC RATE
		oapiVCRegisterArea(AID_C3_PBI18, _R(624, 1185, 666, 1199), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//TRANS X PULSE
		oapiVCRegisterArea(AID_C3_PBI19, _R(158, 1284, 200, 1298), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//TRANS Y PULSE
		oapiVCRegisterArea(AID_C3_PBI20, _R(249, 1284, 291, 1298), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//TRANS Z PULSE
		oapiVCRegisterArea(AID_C3_PBI21, _R(339, 1285, 381, 1299), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//ROT ROLL PULSE
		oapiVCRegisterArea(AID_C3_PBI22, _R(441, 1284, 483, 1298), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//ROT PITCH PULSE
		oapiVCRegisterArea(AID_C3_PBI23, _R(531, 1285, 573, 1299), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
		//ROT YAW PULSE
		oapiVCRegisterArea(AID_C3_PBI24, _R(624, 1285, 666, 1299), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelc3_tex);
	}

	void PanelC3::DefineVC()
	{
		const VECTOR3 switch_rot = _V(1, 0, 0);
		const VECTOR3 pull_dir = _V(0, 1, 0);

		oapiWriteLog("PanelC3: DefineVC called");

		AddAIDToMouseEventList(AID_C3);

		for(int i=0;i<24;i++) {
			pPBIs[i]->AddAIDToRedrawEventList(AID_C3_PBI1+i);
			pPBIs[i]->SetSourceImage(g_Param.pbi_lights);
			pPBIs[i]->SetBase(0, 0);
			pPBIs[i]->SetSourceCoords(true, 0, 0);
			pPBIs[i]->SetSourceCoords(false, 0, 14);
			pPBIs[i]->SetDimensions(37, 11);
		}
		// PBI mouse regions
		pPBIs[0]->SetMouseRegion(0.075540f, 0.421567f, 0.107636f, 0.455563f); //A
		pPBIs[1]->SetMouseRegion(0.129797f, 0.423483f, 0.160799f, 0.456580f); //B
		pPBIs[2]->SetMouseRegion(0.181586f, 0.424022f, 0.213814f, 0.455769f); //AUTO
		pPBIs[3]->SetMouseRegion(0.231808f, 0.423959f, 0.263971f, 0.456584f); //INRTL
		pPBIs[4]->SetMouseRegion(0.285346f, 0.423488f, 0.314472f, 0.455417f); //LVLH
		pPBIs[5]->SetMouseRegion(0.336988f, 0.423070f, 0.370935f, 0.457239f); //FREE
		// NOTE: PCT pbi does not work on PanelC3 (SiameseCat)
		//pPBIs[6]->SetMouseRegion(0.653568f, 0.271157f, 0.699381f, 0.331649f); //PCT
		pPBIs[9]->SetMouseRegion(0.235090f, 0.526461f, 0.268066f, 0.560655f); //PRI
		pPBIs[10]->SetMouseRegion(0.285877f, 0.527114f, 0.319531f, 0.560210f); //ALT
		pPBIs[11]->SetMouseRegion(0.337574f, 0.527336f, 0.372188f, 0.560061f); //VERN
		pPBIs[12]->SetMouseRegion(0.075623f, 0.576277f, 0.110736f, 0.611008f); //X NORM
		pPBIs[13]->SetMouseRegion(0.128061f, 0.576498f, 0.160462f, 0.609389f); //Y NORM
		pPBIs[14]->SetMouseRegion(0.179329f, 0.576932f, 0.211752f, 0.609589f); //Z NORM
		pPBIs[15]->SetMouseRegion(0.235932f, 0.575860f, 0.267935f, 0.609234f); //ROLL DISC RATE
		pPBIs[16]->SetMouseRegion(0.288161f, 0.576933f, 0.318657f, 0.608937f); //PITCH DISC RATE
		pPBIs[17]->SetMouseRegion(0.341027f, 0.575933f, 0.372842f, 0.608609f); //YAW DISC RATE
		pPBIs[18]->SetMouseRegion(0.076191f, 0.627008f, 0.109321f, 0.661322f); //X PULSE
		pPBIs[19]->SetMouseRegion(0.128366f, 0.626492f, 0.159067f, 0.660411f); //Y PULSE
		pPBIs[20]->SetMouseRegion(0.177013f, 0.626544f, 0.213859f, 0.661142f); //Z PULSE
		pPBIs[21]->SetMouseRegion(0.233781f, 0.625428f, 0.269470f, 0.661436f); //ROLL PULSE
		pPBIs[22]->SetMouseRegion(0.284747f, 0.627472f, 0.318325f, 0.661340f); //PITCH PULSE
		pPBIs[23]->SetMouseRegion(0.336868f, 0.626517f, 0.373105f, 0.660984f); //YAW PULSE

		pOMSArm[0]->SetMouseRegion(0.063487f, 0.070910f, 0.117992f, 0.173581f);
		pOMSArm[0]->SetReference(_V(-0.2114868, 1.728119, 14.29085), switch_rot, pull_dir);
		pOMSArm[0]->DefineSwitchGroup(GRP_C3b1_VC);
		pOMSArm[0]->SetInitialAnimState(0.5f);

		pOMSArm[1]->SetMouseRegion(0.117992f, 0.070910f, 0.179360f, 0.173581f);
		pOMSArm[1]->SetReference(_V(-0.1716415, 1.728119, 14.29085), switch_rot, pull_dir);
		pOMSArm[1]->DefineSwitchGroup(GRP_C3b2_VC);
		pOMSArm[1]->SetInitialAnimState(0.5f);
	}

	void PanelC3::Realize()
	{
		BasicPanel::Realize();

		DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("DAP_PBIS1", 16);
		for(int i=0;i<16;i++) {
			pPBIs[i]->input.Connect(pBundle, i);
			pPBIs[i]->output.Connect(pBundle, i);
			pPBIs[i]->test.Connect(pBundle, i);
		}
		pBundle=STS()->BundleManager()->CreateBundle("DAP_PBIS2", 16);
		for(int i=16;i<24;i++) {
			pPBIs[i]->input.Connect(pBundle, i-16);
			pPBIs[i]->output.Connect(pBundle, i-16);
			pPBIs[i]->test.Connect(pBundle, i-16);
		}
	}
};