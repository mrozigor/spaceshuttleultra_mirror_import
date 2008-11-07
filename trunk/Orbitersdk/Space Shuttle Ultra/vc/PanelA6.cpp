#include "PanelA6.h"
#include "../Atlantis.h"
#include "../meshres_vc.h"
#include "../meshres_vc_additions.h"

extern GDIParams g_Param;

//bounding box for DAP Block1
const VECTOR3 DAP1ULCorner=_V(0.715293, 0.10022, 0); // Upper Left
const VECTOR3 DAP1LRCorner=_V(0.370117, 0.190631, 0); // Lower Right
//bounding box for DAP Block2
const VECTOR3 DAP2ULCorner=_V(0.708954, 0.263163, 0); // Upper Left
const VECTOR3 DAP2LRCorner=_V(0.364316, 0.494076, 0); // Lower Right

namespace vc
{
	PanelA6::PanelA6(Atlantis* _sts)
		:BasicPanel(_sts, "A6")
	{
		Add(pSense=new StdSwitch2(_sts, "SENSE"));

		for(int i=0;i<24;i++) PBI_Lights[i]=false;
	}

	PanelA6::~PanelA6()
	{
	}

	void PanelA6::DefineVC()
	{
		oapiWriteLog("PanelA6: DefineVC called");
	}

	void PanelA6::RegisterVC()
	{
		BasicPanel::RegisterVC();
		oapiWriteLog("PanelA6: RegisterVC called");

		VECTOR3 ofs=STS()->orbiter_ofs;

		oapiVCRegisterArea(AID_A6, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP);
		oapiVCSetAreaClickmode_Quadrilateral(AID_A6,
			_V(0.262, 2.892, 12.277)+ofs, _V(0.789, 2.892, 12.277)+ofs,
			_V(0.262, 2.511, 12.397)+ofs, _V(0.789, 2.511, 12.397)+ofs);

		//register DAP PBIs
		SURFHANDLE panela6_tex = oapiGetTextureHandle (STS()->hOrbiterVCMesh, TEX_PANELA6A7_VC);
		//A
		oapiVCRegisterArea(AID_A6_PBI1, _R(703, 1178, 740, 1189), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//B
		oapiVCRegisterArea(AID_A6_PBI2, _R(779, 1178, 816, 1189), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//AUTO
		oapiVCRegisterArea(AID_A6_PBI3, _R(855, 1178, 892, 1189), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//INRTL
		oapiVCRegisterArea(AID_A6_PBI4, _R(934, 1180, 971, 1191), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//LVLH
		oapiVCRegisterArea(AID_A6_PBI5, _R(1012, 1180, 1049, 1191), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//FREE
		oapiVCRegisterArea(AID_A6_PBI6, _R(1092, 1181, 1129, 1192), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//TRANS X
		//oapiVCRegisterArea(AID_A6_PBI7, _R(157, 1089, 199, 1103), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//LOW Z
		//oapiVCRegisterArea(AID_A6_PBI8, _R(247, 1089, 289, 1103), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//HIGH Z
		//oapiVCRegisterArea(AID_A6_PBI9, _R(339, 1090, 381, 1104), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//PRI
		oapiVCRegisterArea(AID_A6_PBI10, _R(943, 1340, 980, 1351), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//ALT
		oapiVCRegisterArea(AID_A6_PBI11, _R(1023, 1340, 1060, 1351), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//VERN
		oapiVCRegisterArea(AID_A6_PBI12, _R(1100, 1340, 1137, 1351), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//TRANS X NORM
		oapiVCRegisterArea(AID_A6_PBI13, _R(712, 1416, 749, 1427), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//TRANS Y NORM
		oapiVCRegisterArea(AID_A6_PBI14, _R(790, 1416, 827, 1427), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//TRANS Z NORM
		oapiVCRegisterArea(AID_A6_PBI15, _R(866, 1418, 903, 1429), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//ROT ROLL DISC RATE
		oapiVCRegisterArea(AID_A6_PBI16, _R(944, 1416, 981, 1427), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//ROT PITCH DISC RATE
		oapiVCRegisterArea(AID_A6_PBI17, _R(1023, 1416, 1060, 1427), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//ROT YAW DISC RATE
		oapiVCRegisterArea(AID_A6_PBI18, _R(1100, 1415, 1137, 1426), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//TRANS X PULSE
		oapiVCRegisterArea(AID_A6_PBI19, _R(711, 1496, 748, 1507), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//TRANS Y PULSE
		oapiVCRegisterArea(AID_A6_PBI20, _R(790, 1496, 827, 1507), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//TRANS Z PULSE
		oapiVCRegisterArea(AID_A6_PBI21, _R(867, 1496, 904, 1507), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//ROT ROLL PULSE
		oapiVCRegisterArea(AID_A6_PBI22, _R(945, 1496, 982, 1507), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//ROT PITCH PULSE
		oapiVCRegisterArea(AID_A6_PBI23, _R(1023, 1496, 1060, 1507), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
		//ROT YAW PULSE
		oapiVCRegisterArea(AID_A6_PBI24, _R(1102, 1496, 1139, 1507), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela6_tex);
	}

	bool PanelA6::OnVCMouseEvent(int id, int _event, VECTOR3 &p)
	{
		bool bRet=BasicPanel::OnVCMouseEvent(id, _event, p);

		if(!bRet) {
			if(_event==PANEL_MOUSE_LBDOWN) {
				if(p.x>=DAP1LRCorner.x && p.x<=DAP1ULCorner.x && p.y>=DAP1ULCorner.y && p.y<=DAP1LRCorner.y) {
					//top row of DAP PBIs clicked
					VECTOR3 pos;
					pos.x=abs((p.x-DAP1ULCorner.x)/(DAP1LRCorner.x-DAP1ULCorner.x));
					pos.y=(p.y-DAP1ULCorner.y)/(DAP1LRCorner.y-DAP1ULCorner.y);
					pos.z=0.0;
					if(STS()->dapcontrol->VCMouseEvent(1, _event, pos)) {
						bRet=true;
						UpdatePBIs();
					}
				}
				else if(p.x>=DAP2LRCorner.x && p.x<=DAP2ULCorner.x && p.y>=DAP2ULCorner.y && p.y<=DAP2LRCorner.y) {
					//second block of DAP PBIs clicked
					VECTOR3 pos;
					pos.x=fabs((p.x-DAP2ULCorner.x)/(DAP2LRCorner.x-DAP2ULCorner.x));
					pos.y=(p.y-DAP2ULCorner.y)/(DAP2LRCorner.y-DAP2ULCorner.y);
					pos.z=0.0;
					if(STS()->dapcontrol->VCMouseEvent(2, _event, pos)) {
						bRet=true;
						UpdatePBIs();
					}
				}
			}
		}

		return bRet;
	}

	bool PanelA6::OnVCRedrawEvent(int id, int _event, SURFHANDLE surf)
	{
		bool bRet=BasicPanel::OnVCRedrawEvent(id, _event, surf);

		if(!bRet) {
			if(id>=AID_A6_PBI1 && id<=AID_A6_PBI24) {
				bRet=DrawPBILight(surf, id-AID_A6_PBI1, STS()->dapcontrol->GetPBIState(id-AID_A6_PBI1));
			}
		}

		return bRet;
	}

	bool PanelA6::DrawPBILight(SURFHANDLE surf, int id, bool bOn)
	{
		if(PBI_Lights[id]==bOn) return false; //nothing to do
		else {
			RECT src, tgt=_R(0, 0, 37, 11);
			//if(bOn) oapiBlt(surf, g_Param.pbi_lights, 0, 0, 0, 0, 42, 14);
			if(bOn) src=_R(0, 0, 42, 14);
			//else oapiBlt(surf, g_Param.pbi_lights, 0, 0, 0, 14, 42, 14);
			else src=_R(0, 14, 42, 28);
			oapiBlt(surf, g_Param.pbi_lights, &tgt, &src);
			PBI_Lights[id]=bOn;
		}
		return true;
	}

	void PanelA6::UpdatePBIs()
	{
		for(int i=0;i<24;i++) oapiVCTriggerRedrawArea(-1, AID_A6_PBI1+i);
	}
};
