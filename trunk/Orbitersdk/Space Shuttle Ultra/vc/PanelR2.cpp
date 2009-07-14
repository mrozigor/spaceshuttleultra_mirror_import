#include "PanelR2.h"
#include "../Atlantis.h"
#include "../meshres_vc_additions.h"
#include "../SSUMath.h"

namespace vc
{
	PanelR2::PanelR2(Atlantis* psts)
		: BasicPanel(psts, "R2")
	{
		Add(pAPUControl[0] = new LockableLever3(psts, "APU1 Run"));
		Add(pAPUControl[1] = new LockableLever3(psts, "APU2 Run"));
		Add(pAPUControl[2] = new LockableLever3(psts, "APU3 Run"));

		Add(pHydPumpPress[0] = new LockableLever2(psts, "Hyd Main Pump Press 1"));
		Add(pHydPumpPress[1] = new LockableLever2(psts, "Hyd Main Pump Press 2"));
		Add(pHydPumpPress[2] = new LockableLever2(psts, "Hyd Main Pump Press 3"));

		Add(pAPUCntlrPwr[0] = new LockableLever2(psts, "APU1 Cntlr Pwr "));
		Add(pAPUCntlrPwr[1] = new LockableLever2(psts, "APU2 Cntlr Pwr "));
		Add(pAPUCntlrPwr[2] = new LockableLever2(psts, "APU3 Cntlr Pwr "));

		Add(pAPUFuelTkVlv[0] = new LockableLever2(psts, "APU1 Fuel Tank Valve"));
		Add(pAPUFuelTkVlv[1] = new LockableLever2(psts, "APU2 Fuel Tank Valve"));
		Add(pAPUFuelTkVlv[2] = new LockableLever2(psts, "APU3 Fuel Tank Valve"));

		Add(pAPUReadyToStart[0] = new StandardTalkback(psts, "APU1 Ready to Start", 1));
		Add(pAPUReadyToStart[1] = new StandardTalkback(psts, "APU2 Ready to Start", 1));
		Add(pAPUReadyToStart[2] = new StandardTalkback(psts, "APU3 Ready to Start", 1));
	}

	PanelR2::~PanelR2()
	{
	}

	void PanelR2::RegisterVC()
	{
		oapiWriteLog("Registering PanelR2");
		BasicPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset();
		SURFHANDLE panelr2_tex = oapiGetTextureHandle (STS()->hOrbiterVCMesh, TEX_PANELR2_VC);

		oapiVCRegisterArea (AID_R2, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBPRESSED | PANEL_MOUSE_LBUP);
		oapiVCSetAreaClickmode_Quadrilateral (AID_R2, _V(0.9524, 1.8417, 14.6602)+ofs, _V(1.4477, 1.9868, 14.6602)+ofs,
			_V(.9524, 1.7286, 13.8048)+ofs, _V(1.4477, 1.8755, 13.8048)+ofs);

		// register the talkbacks
		/**** APU Ready to Start ****/
		oapiVCRegisterArea (AID_R2_TKBK1, _R( 451, 706, 483, 724), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelr2_tex);
		oapiVCRegisterArea (AID_R2_TKBK2, _R( 525, 706, 557, 724), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelr2_tex);
		oapiVCRegisterArea (AID_R2_TKBK3, _R( 598, 706, 630, 724), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelr2_tex);
		//oapiVCRegisterArea (AID_R2_TKBK4, _R( 253, 1723, 285, 1741), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelr2_tex);
		//oapiVCRegisterArea (AID_R2_TKBK5, _R( 379, 1723, 411, 1741), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelr2_tex);
		//oapiVCRegisterArea (AID_R2_TKBK6, _R( 480, 1723, 512, 1741), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelr2_tex);
		//oapiVCRegisterArea (AID_R2_TKBK7, _R( 574, 1723, 606, 1741), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelr2_tex);
		//oapiVCRegisterArea (AID_R2_TKBK8, _R( 672, 1726, 704, 1744), PANEL_REDRAW_USER, PANEL_MOUSE_IGNORE, PANEL_MAP_NONE, panelr2_tex);
	}

	void PanelR2::DefineVC()
	{
		const static VECTOR3 switch_rot = _V(0.95, 0.31225, 0.0);
		const static VECTOR3 switch_pull = Normalize(crossp(_V(0, 0, 1), switch_rot));

		AddAIDToMouseEventList(AID_R2);

		/**** APU 1 ****/
		pAPUControl[0]->SetMouseRegion(0.347421f, 0.382021f, 0.412748f, 0.424506f);
		pAPUControl[0]->SetReference(_V(1.1416, 1.8473, 14.307), switch_rot, switch_pull);
		pAPUControl[0]->DefineSwitchGroup(GRP_R2b15_VC);
		pAPUControl[0]->SetInitialAnimState(0.5f);

		pHydPumpPress[0]->SetMouseRegion(0.364955f, 0.534468f, 0.454298f, 0.568684f);
		pHydPumpPress[0]->SetReference(_V(1.1496, 1.8333, 14.184), switch_rot, switch_pull);
		pHydPumpPress[0]->DefineSwitchGroup(GRP_R2b27_VC);
		pHydPumpPress[0]->SetInitialAnimState(0.5f);

		pAPUCntlrPwr[0]->SetMouseRegion(0.495306f, 0.606395f, 0.553809f, 0.649438f);
		pAPUCntlrPwr[0]->SetReference(_V(1.2106, 1.8433, 14.122), switch_rot, switch_pull);
		pAPUCntlrPwr[0]->DefineSwitchGroup(GRP_R2b34_VC);
		pAPUCntlrPwr[0]->SetInitialAnimState(0.5f);

		pAPUFuelTkVlv[0]->SetMouseRegion(0.496828f, 0.675809f, 0.575494f, 0.714835f);
		pAPUFuelTkVlv[0]->SetReference(_V(1.2136, 1.8363, 14.064), switch_rot, switch_pull);
		pAPUFuelTkVlv[0]->DefineSwitchGroup(GRP_R2b37_VC);
		pAPUFuelTkVlv[0]->SetInitialAnimState(0.5f);

		/**** APU 2 ****/
		pAPUControl[1]->SetMouseRegion(0.412748f, 0.382021f, 0.472092f, 0.424506f);
		pAPUControl[1]->SetReference(_V(1.1706, 1.8563, 14.306), switch_rot, switch_pull);
		pAPUControl[1]->DefineSwitchGroup(GRP_R2b16_VC);
		pAPUControl[1]->SetInitialAnimState(0.5f);

		pHydPumpPress[1]->SetMouseRegion(0.454298f, 0.534468f, 0.549506f, 0.568684f);
		pHydPumpPress[1]->SetReference(_V(1.1986, 1.8473, 14.182), switch_rot, switch_pull);
		pHydPumpPress[1]->DefineSwitchGroup(GRP_R2b28_VC);
		pHydPumpPress[1]->SetInitialAnimState(0.5f);

		pAPUCntlrPwr[1]->SetMouseRegion(0.553809f, 0.606395f, 0.617025f, 0.649438f);
		pAPUCntlrPwr[1]->SetReference(_V(1.2416, 1.8523, 14.121), switch_rot, switch_pull);
		pAPUCntlrPwr[1]->DefineSwitchGroup(GRP_R2b35_VC);
		pAPUCntlrPwr[1]->SetInitialAnimState(0.5f);

		pAPUFuelTkVlv[1]->SetMouseRegion(0.575494f, 0.675809f, 0.649903f, 0.714835f);
		pAPUFuelTkVlv[1]->SetReference(_V(1.2536, 1.8483, 14.062), switch_rot, switch_pull);
		pAPUFuelTkVlv[1]->DefineSwitchGroup(GRP_R2b38_VC);
		pAPUFuelTkVlv[1]->SetInitialAnimState(0.5f);

		/**** APU 3 ****/
		pAPUControl[2]->SetMouseRegion(0.472092f, 0.382021f, 0.535220f, 0.424506f);
		pAPUControl[2]->SetReference(_V(1.2006, 1.8643, 14.305), switch_rot, switch_pull);
		pAPUControl[2]->DefineSwitchGroup(GRP_R2b17_VC);
		pAPUControl[2]->SetInitialAnimState(0.5f);

		pHydPumpPress[2]->SetMouseRegion(0.549506f, 0.534468f, 0.625087f, 0.568684f);
		pHydPumpPress[2]->SetReference(_V(1.2446, 1.8613, 14.181), switch_rot, switch_pull);
		pHydPumpPress[2]->DefineSwitchGroup(GRP_R2b29_VC);
		pHydPumpPress[2]->SetInitialAnimState(0.5f);

		pAPUCntlrPwr[2]->SetMouseRegion(0.617025f, 0.606395f, 0.682484f, 0.649438f);
		pAPUCntlrPwr[2]->SetReference(_V(1.2726, 1.8613, 14.12), switch_rot, switch_pull);
		pAPUCntlrPwr[2]->DefineSwitchGroup(GRP_R2b36_VC);
		pAPUCntlrPwr[2]->SetInitialAnimState(0.5f);

		pAPUFuelTkVlv[2]->SetMouseRegion(0.649903f, 0.675809f, 0.726327f, 0.714835f);
		pAPUFuelTkVlv[2]->SetReference(_V(1.2946, 1.8603, 14.061), switch_rot, switch_pull);
		pAPUFuelTkVlv[2]->DefineSwitchGroup(GRP_R2b39_VC);
		pAPUFuelTkVlv[2]->SetInitialAnimState(0.5f);

		for(int i=0;i<3;i++) {
			pAPUReadyToStart[i]->AddAIDToRedrawEventList(AID_R2_TKBK1+i);
			pAPUReadyToStart[i]->SetDimensions(32, 18);
			pAPUReadyToStart[i]->SetTalkbackLocation(0, 0);
		}
	}

	void PanelR2::Realize()
	{
		char cbuf[255];

		for(int i=0;i<3;i++) {
			sprintf_s(cbuf, 255, "R2_To_APU%d", i+1);
			DiscreteBundle *pBundle=STS()->BundleManager()->CreateBundle(cbuf, 16);

			/*APU_Run[i].Connect(pBundle, 0);
			APU_CntlrPwr[i].Connect(pBundle, 1);
			APU_FuelTankValves[i].Connect(pBundle, 2);
			APU_HydPumpPress[i].Connect(pBundle, 3);
			APU_HydraulicPressure[i].Connect(pBundle, 4);
			APU_ReadyToStart[i].Connect(pBundle, 5);*/
			pAPUControl[i]->ConnectPort(1, pBundle, 0);
			pAPUControl[i]->ConnectPort(2, pBundle, 0); // temporary; THIS IS NOT CORRECT
			pAPUCntlrPwr[i]->ConnectPort(1, pBundle, 1);
			pAPUFuelTkVlv[i]->ConnectPort(1, pBundle, 2);
			pHydPumpPress[i]->ConnectPort(1, pBundle, 3);
			APU_HydraulicPress[i].Connect(pBundle, 4);
			pAPUReadyToStart[i]->SetInput(0, pBundle, 5, TB_GRAY);
		}

		BasicPanel::Realize();
	}

	bool PanelR2::HydraulicPressure() const
	{
		return (APU_HydraulicPress[0].IsSet(2.8f) || APU_HydraulicPress[1].IsSet(2.8f) || APU_HydraulicPress[2].IsSet(2.8f));
	}

	void PanelR2::CheckMPSArmed(int eng) const
	{
	}
};