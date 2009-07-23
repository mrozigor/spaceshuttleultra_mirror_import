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

		Add(pCenterlineLatch = new StdSwitch2(psts, "ET Umb Centerline Latch"));
		Add(pLeftUmbDoor = new StdSwitch3(psts, "ET Umb Left Door"));
		Add(pLeftUmbDoorLatch = new StdSwitch3(psts, "ET Umb Left Door Latch"));
		Add(pRightUmbDoor = new StdSwitch3(psts, "ET Umb Right Door"));
		Add(pRightUmbDoorLatch = new StdSwitch3(psts, "ET Umb Right Door Latch"));

		Add(pMPSPower[0][0] = new StdSwitch2(psts, "MPS Pwr Left AC2"));
		Add(pMPSPower[0][1] = new StdSwitch2(psts, "MPS Pwr Ctr AC1"));
		Add(pMPSPower[0][2] = new StdSwitch2(psts, "MPS Pwr Right AC3"));
		Add(pMPSPower[1][0] = new StdSwitch2(psts, "MPS Pwr Left AC3"));
		Add(pMPSPower[1][1] = new StdSwitch2(psts, "MPS Pwr Ctr AC2"));
		Add(pMPSPower[1][2] = new StdSwitch2(psts, "MPS Pwr Right AC1"));

		Add(pMPSHeIsolA[0] = new StdSwitch3(psts, "MPS He Isol A Left"));
		Add(pMPSHeIsolA[1] = new StdSwitch3(psts, "MPS He Isol A Ctr"));
		Add(pMPSHeIsolA[2] = new StdSwitch3(psts, "MPS He Isol A Right"));
		Add(pMPSHeIsolB[0] = new StdSwitch3(psts, "MPS He Isol B Left"));
		Add(pMPSHeIsolB[1] = new StdSwitch3(psts, "MPS He Isol B Ctr"));
		Add(pMPSHeIsolB[2] = new StdSwitch3(psts, "MPS He Isol B Right"));

		for(int i=0;i<3;i++) {
			pAPUControl[i]->SetLabel(0, "START ORIDE/RUN");
			pAPUControl[i]->SetLabel(1, "OFF");
			pAPUControl[i]->SetLabel(2, "START/RUN");
			pHydPumpPress[i]->SetLabel(0, "LOW");
			pHydPumpPress[i]->SetLabel(1, "NORM");
			pAPUCntlrPwr[i]->SetLabel(0, "OFF");
			pAPUCntlrPwr[i]->SetLabel(1, "ON");
			pAPUFuelTkVlv[i]->SetLabel(0, "CLOSE");
			pAPUFuelTkVlv[i]->SetLabel(1, "OPEN");
		}
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

		/**** ET Umbilical Doors ****/
		pCenterlineLatch->SetMouseRegion(0.193928f, 0.884566f, 0.253453f, 0.916514f);
		pCenterlineLatch->SetReference(_V(1.062, 1.773, 13.888), switch_rot);
		pCenterlineLatch->DefineSwitchGroup(GRP_R2b49_VC);
		pCenterlineLatch->SetInitialAnimState(0.5f);

		pLeftUmbDoor->SetMouseRegion(0.292763f, 0.885751f, 0.351700f, 0.920420f);
		pLeftUmbDoor->SetReference(_V(1.11, 1.787, 13.885), switch_rot);
		pLeftUmbDoor->DefineSwitchGroup(GRP_R2b50_VC);
		pLeftUmbDoor->SetInitialAnimState(0.5f);

		pLeftUmbDoorLatch->SetMouseRegion(0.375219f, 0.887672f, 0.433632f, 0.922404f);
		pLeftUmbDoorLatch->SetReference(_V(1.151, 1.799, 13.884), switch_rot);
		pLeftUmbDoorLatch->DefineSwitchGroup(GRP_R2b51_VC);
		pLeftUmbDoorLatch->SetInitialAnimState(0.5f);

		pRightUmbDoor->SetMouseRegion(0.455335f, 0.889641f, 0.513106f, 0.923580f);
		pRightUmbDoor->SetReference(_V(1.19, 1.811, 13.883), switch_rot);
		pRightUmbDoor->DefineSwitchGroup(GRP_R2b52_VC);
		pRightUmbDoor->SetInitialAnimState(0.5f);

		pRightUmbDoorLatch->SetMouseRegion(0.53287f, 0.890511f, 0.590665f, 0.925481f);
		pRightUmbDoorLatch->SetReference(_V(1.228, 1.822, 13.881), switch_rot);
		pRightUmbDoorLatch->DefineSwitchGroup(GRP_R2b53_VC);
		pRightUmbDoorLatch->SetInitialAnimState(0.5f);

		/**** MPS He Isol A ****/
		pMPSHeIsolA[0]->SetMouseRegion(0.081538f, 0.322382f, 0.157321f, 0.352672f);
		pMPSHeIsolA[0]->SetReference(_V(1.008, 1.822, 14.372), switch_rot);
		pMPSHeIsolA[0]->DefineSwitchGroup(GRP_R2b9_VC);
		pMPSHeIsolA[0]->SetInitialAnimState(0.5f);

		pMPSHeIsolA[1]->SetMouseRegion(0.157321f, 0.323196f, 0.239897f, 0.354704f);
		pMPSHeIsolA[1]->SetReference(_V(1.049, 1.834, 14.371), switch_rot);
		pMPSHeIsolA[1]->DefineSwitchGroup(GRP_R2b10_VC);
		pMPSHeIsolA[1]->SetInitialAnimState(0.5f);

		pMPSHeIsolA[2]->SetMouseRegion(0.239897f, 0.323704f, 0.312041f, 0.354271f);
		pMPSHeIsolA[2]->SetReference(_V(1.089, 1.845, 14.37), switch_rot);
		pMPSHeIsolA[2]->DefineSwitchGroup(GRP_R2b11_VC);
		pMPSHeIsolA[2]->SetInitialAnimState(0.5f);

		/**** MPS He Isol B ****/
		pMPSHeIsolB[0]->SetMouseRegion(0.085931f, 0.391285f, 0.159351f, 0.422357f);
		pMPSHeIsolB[0]->SetReference(_V(1.008, 1.814, 14.313), switch_rot);
		pMPSHeIsolB[0]->DefineSwitchGroup(GRP_R2b12_VC);
		pMPSHeIsolB[0]->SetInitialAnimState(0.5f);

		pMPSHeIsolB[1]->SetMouseRegion(0.159351f, 0.389469f, 0.238773f, 0.424157f);
		pMPSHeIsolB[1]->SetReference(_V(1.048, 1.825, 14.312), switch_rot);
		pMPSHeIsolB[1]->DefineSwitchGroup(GRP_R2b13_VC);
		pMPSHeIsolB[1]->SetInitialAnimState(0.5f);

		pMPSHeIsolB[2]->SetMouseRegion(0.238773f, 0.390728f, 0.310743f, 0.425303f);
		pMPSHeIsolB[2]->SetReference(_V(1.088, 1.837, 14.31), switch_rot);
		pMPSHeIsolB[2]->DefineSwitchGroup(GRP_R2b14_VC);
		pMPSHeIsolB[2]->SetInitialAnimState(0.5f);

		/**** MPS Engine Power ****/
		pMPSPower[0][0]->SetMouseRegion(0.091011f, 0.177120f, 0.159388f, 0.207124f);
		pMPSPower[0][0]->SetReference(_V(1.009, 1.838, 14.495), switch_rot);
		pMPSPower[0][0]->DefineSwitchGroup(GRP_R2b3_VC);
		pMPSPower[0][0]->SetInitialAnimState(0.5f);

		pMPSPower[0][1]->SetMouseRegion(0.159388f, 0.178765f, 0.239655f, 0.208946f);
		pMPSPower[0][1]->SetReference(_V(1.05, 1.85, 14.494), switch_rot);
		pMPSPower[0][1]->DefineSwitchGroup(GRP_R2b4_VC);
		pMPSPower[0][1]->SetInitialAnimState(0.5f);

		pMPSPower[0][2]->SetMouseRegion(0.239655f, 0.179134f, 0.313464f, 0.208574f);
		pMPSPower[0][2]->SetReference(_V(1.088, 1.861, 14.493), switch_rot);
		pMPSPower[0][2]->DefineSwitchGroup(GRP_R2b5_VC);
		pMPSPower[0][2]->SetInitialAnimState(0.5f);

		pMPSPower[1][0]->SetMouseRegion(0.089520f, 0.244445f, 0.159198f, 0.272745f);
		pMPSPower[1][0]->SetReference(_V(1.008, 1.83, 14.439), switch_rot);
		pMPSPower[1][0]->DefineSwitchGroup(GRP_R2b6_VC);
		pMPSPower[1][0]->SetInitialAnimState(0.5f);

		pMPSPower[1][1]->SetMouseRegion(0.159198f, 0.246158f, 0.240562f, 0.276445f);
		pMPSPower[1][1]->SetReference(_V(1.05, 1.843, 14.437), switch_rot);
		pMPSPower[1][1]->DefineSwitchGroup(GRP_R2b7_VC);
		pMPSPower[1][1]->SetInitialAnimState(0.5f);

		pMPSPower[1][2]->SetMouseRegion(0.240562f, 0.247841f, 0.316475f, 0.276847f);
		pMPSPower[1][2]->SetReference(_V(1.091, 1.855, 14.435), switch_rot);
		pMPSPower[1][2]->DefineSwitchGroup(GRP_R2b8_VC);
		pMPSPower[1][2]->SetInitialAnimState(0.5f);
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

		DiscreteBundle* pBundle=STS()->BundleManager()->CreateBundle("ETUmbDoors", 16);
		pLeftUmbDoor->outputA.Connect(pBundle, 0); // close door
		pLeftUmbDoor->outputB.Connect(pBundle, 1); // open door
		pLeftUmbDoorLatch->outputA.Connect(pBundle, 2); // latch
		pLeftUmbDoorLatch->outputB.Connect(pBundle, 3); // release
		pRightUmbDoor->outputA.Connect(pBundle, 4); // close door
		pRightUmbDoor->outputB.Connect(pBundle, 5); // open door
		pRightUmbDoorLatch->outputA.Connect(pBundle, 6); // latch
		pRightUmbDoorLatch->outputB.Connect(pBundle, 7); // release
		pCenterlineLatch->output.Connect(pBundle, 8);

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