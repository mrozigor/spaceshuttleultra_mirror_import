#include "PanelA8.h"
#include "../meshres_vc_a8.h"
#include "../meshres_vc_additions.h"
#include "../Atlantis.h"

namespace vc
{
	PanelA8::PanelA8(Atlantis* _sts)
		: BasicPanel(_sts, "A8")
	{
		hPanelMesh=oapiLoadMeshGlobal(DEFAULT_MESHNAME_PANELA8);
		mesh_index=MESH_UNDEFINED;

		Add(pPortMPMCover = new StandardSwitchCover(_sts, "Port MPM Deploy Cover"));
		Add(pStbdMPMCover = new StandardSwitchCover(_sts, "Stbd MPM Deploy Cover"));

		Add(pPortMRL = new StdSwitch3(_sts, "Port RMS Latches"));
		Add(pStbdMRL = new StdSwitch3(_sts, "Stbd MPM Latches"));
		Add(pPortMPM = new StdSwitch3(_sts, "Port MPM Deploy"));
		Add(pStbdMPM = new StdSwitch3(_sts, "Stbd MPM Deploy"));
		Add(pEEMode = new StdSwitch3(_sts, "EE Mode"));
		Add(pEEManContr = new StdSwitch3(_sts, "EE Man Contr"));
		Add(pShoulderBrace = new StdSwitch3(_sts, "Shoulder Brace Release"));
		Add(pRMSSelect = new StdSwitch3(_sts, "RMS SELECT"));

		Add(pPortMRLTb = new StandardTalkback(_sts, "Port MRL Talkback", 2));
		Add(pStbdMRLTb = new StandardTalkback(_sts, "Stbd MRL Talkback", 2));
		Add(pPortMPMTb = new StandardTalkback(_sts, "Port MPM Talkback", 2));
		Add(pStbdMPMTb = new StandardTalkback(_sts, "Stbd MPM Talkback", 2));

		Add(pPortMRL_RTL[0] = new Std2SegTalkback(_sts, "Port MRL RTL FWD", 1));
		Add(pPortMRL_RTL[1] = new Std2SegTalkback(_sts, "Port MRL RTL MID", 1));
		Add(pPortMRL_RTL[2] = new Std2SegTalkback(_sts, "Port MRL RTL AFT", 1));
		Add(pStbdMRL_RTL[0] = new Std2SegTalkback(_sts, "Stbd MRL RTL FWD", 1));
		Add(pStbdMRL_RTL[1] = new Std2SegTalkback(_sts, "Stbd MRL RTL MID", 1));
		Add(pStbdMRL_RTL[2] = new Std2SegTalkback(_sts, "Stbd MRL RTL AFT", 1));
		Add(pEECapture = new Std2SegTalkback(_sts, "EE Capture", 1));
		Add(pEEExtend = new Std2SegTalkback(_sts, "EE Extend", 1));
		Add(pEEClose = new Std2SegTalkback(_sts, "EE Close", 1));
		Add(pEEOpen = new Std2SegTalkback(_sts, "EE Open", 1));
		Add(pEERigid = new Std2SegTalkback(_sts, "EE Rigid", 1));
		Add(pEEDerigid = new Std2SegTalkback(_sts, "EE Derigid", 1));
		Add(pShoulderBraceTb = new Std2SegTalkback(_sts, "Shoulder Brace", 1));

		pPortMRL->SetLabel(2, "REL");
		pPortMRL->SetLabel(1, "OFF");
		pPortMRL->SetLabel(0, "LAT");
		pStbdMRL->SetLabel(2, "REL");
		pStbdMRL->SetLabel(1, "OFF");
		pStbdMRL->SetLabel(0, "LAT");
		pPortMPM->SetLabel(2, "DEPLOY");
		pPortMPM->SetLabel(1, "OFF");
		pPortMPM->SetLabel(0, "STOW");
		pStbdMPM->SetLabel(2, "DEPLOY");
		pStbdMPM->SetLabel(1, "OFF");
		pStbdMPM->SetLabel(0, "STOW");
		pEEMode->SetLabel(2, "AUTO");
		pEEMode->SetLabel(1, "OFF");
		pEEMode->SetLabel(0, "MAN");
		pEEManContr->SetLabel(2, "RIGID");
		pEEManContr->SetLabel(1, "OFF");
		pEEManContr->SetLabel(0, "DERIGID");
		pShoulderBrace->SetLabel(2, "PORT");
		pShoulderBrace->SetLabel(1, "OFF");
		pShoulderBrace->SetLabel(0, "STBD");
		pRMSSelect->SetLabel(2, "PORT");
		pRMSSelect->SetLabel(1, "OFF");
		pRMSSelect->SetLabel(0, "STBD");

		EnableCoordinateDisplayMode();
	}

	PanelA8::~PanelA8()
	{
	}

	void PanelA8::AddMeshes(const VECTOR3 &ofs)
	{
		SetHasOwnVCMesh();

		mesh_index=STS()->AddMesh(hPanelMesh, &ofs);
		STS()->SetMeshVisibilityMode(mesh_index, MESHVIS_VC);
	}

	void PanelA8::SetMeshVisibility(bool visible)
	{
		if(visible) STS()->SetMeshVisibilityMode(mesh_index, MESHVIS_VC);
		else STS()->SetMeshVisibilityMode(mesh_index, MESHVIS_NEVER);
	}

	UINT PanelA8::GetVCMeshIndex() const
	{
		return mesh_index;
	}

	void PanelA8::RegisterVC()
	{
		oapiWriteLog("PanelA8::RegisterVC() called");
		BasicPanel::RegisterVC();
		oapiWriteLog("PanelA8::BasicPanel::RegisterVC() called");

		VECTOR3 ofs = STS()->orbiter_ofs;
		oapiVCRegisterArea(AID_A8, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP);
		oapiVCSetAreaClickmode_Quadrilateral (AID_A8, _V(-0.797, 2.892, 12.279)+ofs, _V(-0.266, 2.892, 12.279)+ofs, 
			_V(-0.797, 2.119, 12.522)+ofs, _V(-0.266, 2.119, 12.522)+ofs);

		SURFHANDLE panela8t_tex = oapiGetTextureHandle (hPanelMesh, TEX_A8TOP_VC);
		SURFHANDLE panela8b_tex = oapiGetTextureHandle (hPanelMesh, TEX_A8BOTTOM_VC);
		// PORT MPM RTL
		oapiVCRegisterArea(AID_A8_TKBK1, _R(895, 894, 922, 922), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
		oapiVCRegisterArea(AID_A8_TKBK3, _R(895, 769, 922, 797), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
		oapiVCRegisterArea(AID_A8_TKBK5, _R(895, 642, 922, 670), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
		// STBD MPM RTL
		oapiVCRegisterArea(AID_A8_TKBK2, _R(297, 896, 324, 924), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
		oapiVCRegisterArea(AID_A8_TKBK4, _R(297, 770, 324, 798), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
		oapiVCRegisterArea(AID_A8_TKBK8, _R(297, 643, 324, 671), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
		// PORT MPM LATCH
		oapiVCRegisterArea(AID_A8_TKBK6, _R(806, 650, 838, 668), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
		// PORT MPM DPY
		oapiVCRegisterArea(AID_A8_TKBK7, _R(717, 648, 749, 666), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
		// STBD MPM LATCH
		oapiVCRegisterArea(AID_A8_TKBK9, _R(210, 651, 242, 669), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
		// STBD MPM DPY
		oapiVCRegisterArea(AID_A8_TKBK10, _R(120, 648, 152, 666), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8b_tex);
		// EE CAPTURE
		oapiVCRegisterArea(AID_A8_TKBK11, _R(917, 326, 944, 354), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
		// EE EXTEND
		oapiVCRegisterArea(AID_A8_TKBK12, _R(917, 392, 944, 420), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
		// SHOULDER BRACE
		oapiVCRegisterArea(AID_A8_TKBK13, _R(902, 922, 929, 950), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
		// EE CLOSE
		oapiVCRegisterArea(AID_A8_TKBK16, _R(869, 327, 896, 355), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
		// EE OPEN
		oapiVCRegisterArea(AID_A8_TKBK17, _R(869, 391, 896, 419), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
		// EE RIGID
		oapiVCRegisterArea(AID_A8_TKBK18, _R(821, 327, 848, 355), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
		// EE DERIGID
		oapiVCRegisterArea(AID_A8_TKBK19, _R(821, 392, 848, 420), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panela8t_tex);
	}

	void PanelA8::DefineVC()
	{
		const VECTOR3 switch_rot_vert=_V(-1.0, 0.0, 0.0);
		const VECTOR3 switch_rot_horz=_V(0.0, -0.793, 0.249);

		AddAIDToMouseEventList(AID_A8);

		pPortMPMCover->SetMouseRegion(0, 0.252639f, 0.834143f, 0.305916f, 0.863428f);
		pPortMPMCover->SetMouseRegion(1, 0.235794f, 0.804317f, 0.298074f, 0.834143f);
		pPortMPMCover->SetReference(_V(-0.646, 2.257, 12.478), switch_rot_vert);
		pPortMPMCover->DefineCoverGroup(GRP_A8Scover5_VC);

		pStbdMPMCover->SetMouseRegion(0, 0.841143f, 0.836290f, 0.886384f, 0.867186f);
		pStbdMPMCover->SetMouseRegion(1, 0.841143f, 0.806764f, 0.886384f, 0.833786f);
		pStbdMPMCover->SetReference(_V(-0.339, 2.257, 12.479), switch_rot_vert);
		pStbdMPMCover->DefineCoverGroup(GRP_A8Scover1_VC);

		pPortMPM->SetMouseRegion(0.252639f, 0.834143f, 0.305916f, 0.863428f);
		pPortMPM->SetReference(_V(-0.647, 2.238, 12.486), switch_rot_vert);
		pPortMPM->DefineSwitchGroup(GRP_A8S17_VC);
		pPortMPM->SetInitialAnimState(0.5f);

		pStbdMPM->SetMouseRegion(0.841143f, 0.836290f, 0.886384f, 0.867186f);
		pStbdMPM->SetReference(_V(-0.693, 2.233, 12.486), switch_rot_vert);
		pStbdMPM->DefineSwitchGroup(GRP_A8S12_VC);
		pStbdMPM->SetInitialAnimState(0.5f);

		pPortMRL->SetMouseRegion(0.167373f, 0.841234f, 0.217567f, 0.867132f);
		pPortMRL->SetReference(_V(-0.693, 2.233, 12.486), switch_rot_vert);
		pPortMRL->DefineSwitchGroup(GRP_A8S18_VC);
		pPortMRL->SetInitialAnimState(0.5f);

		pStbdMRL->SetMouseRegion(0.755850f, 0.842015f, 0.801863f, 0.880786f);
		pStbdMRL->SetReference(_V(-0.384, 2.233, 12.487), switch_rot_vert);
		pStbdMRL->DefineSwitchGroup(GRP_A8S13_VC);
		pStbdMRL->SetInitialAnimState(0.5f);

		pEEMode->SetMouseRegion(0.298233f, 0.2227635f, 0.346051f, 0.250502f);
		pEEMode->SetReference(_V(-0.625, 2.709, 12.337), switch_rot_vert);
		pEEMode->DefineSwitchGroup(GRP_A8S4_VC);
		pEEMode->SetInitialAnimState(0.5f);

		pEEManContr->SetMouseRegion(0.243867f, 0.22259f, 0.288102f, 0.252219f);
		pEEManContr->SetReference(_V(-0.654, 2.709, 12.337), switch_rot_vert);
		pEEManContr->DefineSwitchGroup(GRP_A8S5_VC);
		pEEManContr->SetInitialAnimState(0.5f);
		pEEManContr->SetStringLoaded(true);

		pShoulderBrace->SetMouseRegion(0.170449f, 0.618925f, 0.226497f, 0.651137f);
		pShoulderBrace->SetReference(_V(-0.687, 2.402, 12.434), switch_rot_horz);
		pShoulderBrace->DefineSwitchGroup(GRP_A8S10_VC);
		pShoulderBrace->SetInitialAnimState(0.5f);
		pShoulderBrace->SetOrientation(true);
		pShoulderBrace->SetStringLoaded(true);

		pRMSSelect->SetMouseRegion(0.466754f, 0.757090f, 0.510409f, 0.788049f);
		pRMSSelect->SetReference(_V(-0.537, 2.294, 12.467), switch_rot_horz);
		pRMSSelect->DefineSwitchGroup(GRP_A8S11_VC);
		pRMSSelect->SetInitialAnimState(0.5f);
		pRMSSelect->SetOrientation(true);

		pPortMRLTb->AddAIDToRedrawEventList(AID_A8_TKBK6);
		pPortMRLTb->SetDimensions(32, 18);
		pPortMRLTb->SetTalkbackLocation(0, 0);

		pStbdMRLTb->AddAIDToRedrawEventList(AID_A8_TKBK9);
		pStbdMRLTb->SetDimensions(32, 18);
		pStbdMRLTb->SetTalkbackLocation(0, 0);

		pPortMPMTb->AddAIDToRedrawEventList(AID_A8_TKBK7);
		pPortMPMTb->SetDimensions(32, 18);
		pPortMPMTb->SetTalkbackLocation(0, 0);

		pStbdMPMTb->AddAIDToRedrawEventList(AID_A8_TKBK10);
		pStbdMPMTb->SetDimensions(32, 18);
		pStbdMPMTb->SetTalkbackLocation(0, 0);

		pStbdMRL_RTL[0]->AddAIDToRedrawEventList(AID_A8_TKBK2);
		pStbdMRL_RTL[1]->AddAIDToRedrawEventList(AID_A8_TKBK4);
		pStbdMRL_RTL[2]->AddAIDToRedrawEventList(AID_A8_TKBK8);
		for(int i=0;i<3;i++) {
			pPortMRL_RTL[i]->AddAIDToRedrawEventList(AID_A8_TKBK1+2*i);
			pPortMRL_RTL[i]->SetDimensions(27, 28);
			pPortMRL_RTL[i]->SetTalkbackLocation(0, 0);
			pPortMRL_RTL[i]->SetInactiveSegment(0, TB_GRAY);

			pStbdMRL_RTL[i]->SetDimensions(27, 28);
			pStbdMRL_RTL[i]->SetTalkbackLocation(0, 0);
			pStbdMRL_RTL[i]->SetInactiveSegment(0, TB_GRAY);
		}

		pShoulderBraceTb->AddAIDToRedrawEventList(AID_A8_TKBK13);
		pShoulderBraceTb->SetDimensions(27, 28);
		pShoulderBraceTb->SetTalkbackLocation(0, 0);
		pShoulderBraceTb->SetInactiveSegment(0, TB_GRAY);

		pEECapture->AddAIDToRedrawEventList(AID_A8_TKBK11);
		pEECapture->SetDimensions(27, 28);
		pEECapture->SetTalkbackLocation(0, 0);
		pEECapture->SetInactiveSegment(0, TB_GRAY);

		pEEExtend->AddAIDToRedrawEventList(AID_A8_TKBK12);
		pEEExtend->SetDimensions(27, 28);
		pEEExtend->SetTalkbackLocation(0, 0);
		pEEExtend->SetInactiveSegment(0, TB_GRAY);

		pEEClose->AddAIDToRedrawEventList(AID_A8_TKBK16);
		pEEClose->SetDimensions(27, 28);
		pEEClose->SetTalkbackLocation(0, 0);
		pEEClose->SetInactiveSegment(0, TB_GRAY);

		pEEOpen->AddAIDToRedrawEventList(AID_A8_TKBK17);
		pEEOpen->SetDimensions(27, 28);
		pEEOpen->SetTalkbackLocation(0, 0);
		pEEOpen->SetInactiveSegment(0, TB_GRAY);

		pEERigid->AddAIDToRedrawEventList(AID_A8_TKBK18);
		pEERigid->SetDimensions(27, 28);
		pEERigid->SetTalkbackLocation(0, 0);
		pEERigid->SetInactiveSegment(0, TB_GRAY);

		pEEDerigid->AddAIDToRedrawEventList(AID_A8_TKBK19);
		pEEDerigid->SetDimensions(27, 28);
		pEEDerigid->SetTalkbackLocation(0, 0);
		pEEDerigid->SetInactiveSegment(0, TB_GRAY);
	}

	void PanelA8::DefineVCAnimations(UINT vcidx)
	{
		// call DefineVCAnimations using correct index
		BasicPanel::DefineVCAnimations(mesh_index);
	}

	void PanelA8::Realize()
	{
		DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("RMS", 16);
		pPortMPM->outputB.Connect(pBundle, 0);
		pPortMPM->outputA.Connect(pBundle, 1);
		pPortMPMTb->SetInput(0, pBundle, 2, TB_DPY);
		pPortMPMTb->SetInput(1, pBundle, 3, TB_STO);
		// for the moment, ignore STBD connections for shoulder brace
		pShoulderBrace->outputB.Connect(pBundle, 4);
		pShoulderBraceTb->SetInput(0, pBundle, 5, TB_GRAY);
		pRMSSelect->outputB.Connect(pBundle, 6);

		pBundle=STS()->BundleManager()->CreateBundle("RMS_MRL", 16);
		pPortMRL->outputB.Connect(pBundle, 0);
		pPortMRL->outputA.Connect(pBundle, 1);
		for(int i=0;i<3;i++) pPortMRL_RTL[i]->SetInput(0, pBundle, i+5, TB_GRAY);
		pPortMRLTb->SetInput(0, pBundle, 11, TB_REL);
		pPortMRLTb->SetInput(1, pBundle, 12, TB_LAT);

		pBundle=STS()->BundleManager()->CreateBundle("RMS_EE", 16);
		pEEMode->outputB.Connect(pBundle, 2);
		pEEMode->outputA.Connect(pBundle, 3);
		pEEManContr->outputB.Connect(pBundle, 4);
		pEEManContr->outputA.Connect(pBundle, 5);
		pEECapture->SetInput(0, pBundle, 6, TB_GRAY);
		pEEExtend->SetInput(0, pBundle, 7, TB_GRAY);
		pEEClose->SetInput(0, pBundle, 8, TB_GRAY);
		pEEOpen->SetInput(0, pBundle, 9, TB_GRAY);
		pEERigid->SetInput(0, pBundle, 10, TB_GRAY);
		pEEDerigid->SetInput(0, pBundle, 11, TB_GRAY);

		pBundle=STS()->BundleManager()->CreateBundle("STBD_MPM", 16);
		pStbdMPM->outputB.Connect(pBundle, 0);
		pStbdMPM->outputA.Connect(pBundle, 1);
		pStbdMPMTb->SetInput(0, pBundle, 2, TB_DPY);
		pStbdMPMTb->SetInput(1, pBundle, 3, TB_STO);

		pBundle=STS()->BundleManager()->CreateBundle("STBD_MPM_MRL", 16);
		pStbdMRL->outputB.Connect(pBundle, 0);
		pStbdMRL->outputA.Connect(pBundle, 1);
		for(int i=0;i<3;i++) pStbdMRL_RTL[i]->SetInput(0, pBundle, i+5, TB_GRAY);
		pStbdMRLTb->SetInput(0, pBundle, 11, TB_REL);
		pStbdMRLTb->SetInput(1, pBundle, 12, TB_LAT);

		BasicPanel::Realize();
	}
};