#include "../Atlantis.h"
#include "PanelO6.h"
#include "../meshres_vc.h"


namespace vc {

	PanelO6::PanelO6(Atlantis* _sts) 
		: BasicPanel(_sts, "O6") {

		const VECTOR3 SWITCH_VERTICAL = _V(1.0, 0.0, 0.0);

		oapiWriteLog("(PanelO6::PanelO6) Enter constructor.");

		Add(pLGlareShieldFlood	= new StdSwitch3(_sts, "L GLRSHLD FLOOD"));	
		pLGlareShieldFlood->SetLabel(0, "OFF");
		pLGlareShieldFlood->SetLabel(1, "VAR");
		pLGlareShieldFlood->SetLabel(2, "BRIGHT");


		Add(pSTRKDoorControlSys1	= new StdSwitch3(_sts, "S TRK DR CNTL SYS1"));		 
		pSTRKDoorControlSys1->SetLabel(0, "CLOSE");
		pSTRKDoorControlSys1->SetLabel(1, "OFF");
		pSTRKDoorControlSys1->SetLabel(2, "OPEN");
		pSTRKDoorControlSys1->DefineSwitchGroup(GRP_O6S2_VC);
		pSTRKDoorControlSys1->SetMouseRegion(0.64f, 0.1767f, 0.7078f, 0.2315f);
		//-0.382601  3.152154  13.74688
		pSTRKDoorControlSys1->SetReference(_V(-0.0, 3.152154, 13.74688), SWITCH_VERTICAL);
		pSTRKDoorControlSys1->SetInitialAnimState(0.0f);
		
		Add(pSTRKDoorControlSys2	= new StdSwitch3(_sts, "S TRK DR CNTL SYS2"));		 
		pSTRKDoorControlSys2->SetLabel(0, "CLOSE");
		pSTRKDoorControlSys2->SetLabel(1, "OFF");
		pSTRKDoorControlSys2->SetLabel(2, "OPEN");
		pSTRKDoorControlSys2->DefineSwitchGroup(GRP_O6S3_VC);
		pSTRKDoorControlSys2->SetMouseRegion(0.7267f, 0.1767f, 0.7706f, 0.2315f);
		pSTRKDoorControlSys2->SetReference(_V(-0.0, 3.152154, 13.74688), SWITCH_VERTICAL);
		pSTRKDoorControlSys2->SetInitialAnimState(0.0f);
		
		
		/*		
		StdSwitch2* pSTRKPowerNY;
		StdSwitch2* pSTRKPowerNZ;
		StdSwitch3* pLeftSeatCenterFlood;
		StdSwitch2* pUHFXmitFreq;
		StdSwitch2* pUHFSPLXPowerAmp;
		StdSwitch2* pUHFSquelch;
		StdSwitch3* pAnnunciatorLampTest;
		StdSwitch3* pBusSelectACA1;
		StdSwitch3* pBusSelectACA23;
		StdSwitch3* pBusSelectACA1;
		StdSwitch3* pMTU;
		StdSwitch2* pIDP1;
		StdSwitch2* pIDP2;
		StdSwitch2* pIDP3;
		StdSwitch2* pIDP4;
		StdSwitch2* pMDMPL1;
		StdSwitch2* pMDMPL2;
		StdSwitch2* pMDMPL3;
		StdSwitch2* pMDMFA1;
		StdSwitch2* pMDMFA2;
		StdSwitch2* pMDMFA2;
		StdSwitch2* pMDMFA4;
		StdSwitch2* pMDMFF2;
		StdSwitch2* pMDMFF2;
		StdSwitch2* pMDMFF2;
		StdSwitch2* pMDMFF2;
		StdSwitch2* pGPC1Pwr;
		StdSwitch2* pGPC2Pwr;
		StdSwitch2* pGPC3Pwr;
		StdSwitch2* pGPC4Pwr;
		StdSwitch2* pGPC5Pwr;
		*/

		//_V(-0.0, 3.075613, 14.06777)

		Add(pGPCOutputCover[0] = new StandardSwitchCover(_sts, "GPC_OUTPUT_1_COVER"));
		pGPCOutputCover[0]->SetMouseRegion(0, 0.4798f, 0.6438f, 0.5473f, 0.7007f);
		pGPCOutputCover[0]->SetMouseRegion(1, 0.4798f, 0.6238f, 0.5473f, 0.6438f);
		pGPCOutputCover[0]->DefineCoverGroup(GRP_O6Scover6_VC);
		pGPCOutputCover[0]->SetReference(_V(-0.0, 3.075613, 14.06777), SWITCH_VERTICAL);
		
		Add(pGPCOutputCover[1] = new StandardSwitchCover(_sts, "GPC_OUTPUT_2_COVER"));
		pGPCOutputCover[1]->SetMouseRegion(0, 0.5587f, 0.6438f, 0.6032f, 0.7007f);
		pGPCOutputCover[1]->SetMouseRegion(1, 0.5587f, 0.6238f, 0.6032f, 0.6438f);
		pGPCOutputCover[1]->DefineCoverGroup(GRP_O6Scover7_VC);
		pGPCOutputCover[1]->SetReference(_V(-0.0, 3.075613, 14.06777), SWITCH_VERTICAL);
		
		Add(pGPCOutputCover[2] = new StandardSwitchCover(_sts, "GPC_OUTPUT_3_COVER"));
		pGPCOutputCover[2]->SetMouseRegion(0, 0.6212f, 0.6438f, 0.6657f, 0.7007f);
		pGPCOutputCover[2]->SetMouseRegion(1, 0.6212f, 0.6238f, 0.6657f, 0.6438f);
		pGPCOutputCover[2]->DefineCoverGroup(GRP_O6Scover8_VC);
		pGPCOutputCover[2]->SetReference(_V(-0.0, 3.075613, 14.06777), SWITCH_VERTICAL);
		
		Add(pGPCOutputCover[3] = new StandardSwitchCover(_sts, "GPC_OUTPUT_4_COVER"));
		pGPCOutputCover[3]->SetMouseRegion(0, 0.6828f, 0.6438f, 0.7375f, 0.7007f);
		pGPCOutputCover[3]->SetMouseRegion(1, 0.6828f, 0.6238f, 0.7375f, 0.6438f);
		pGPCOutputCover[3]->DefineCoverGroup(GRP_O6Scover9_VC);
		pGPCOutputCover[3]->SetReference(_V(-0.0, 3.075613, 14.06777), SWITCH_VERTICAL);

		Add(pGPCOutputCover[4] = new StandardSwitchCover(_sts, "GPC_OUTPUT_5_COVER"));
		pGPCOutputCover[4]->SetMouseRegion(0, 0.7538f, 0.6438f, 0.8021f, 0.7007f);
		pGPCOutputCover[4]->SetMouseRegion(1, 0.7538f, 0.6238f, 0.8021f, 0.6438f);
		pGPCOutputCover[4]->DefineCoverGroup(GRP_O6Scover10_VC);
		pGPCOutputCover[4]->SetReference(_V(-0.0, 3.075613, 14.06777), SWITCH_VERTICAL);

		/*
		_V(0.0,  3.07197,  14.08016)
		StdSwitch3* pGPC1Output;
		StdSwitch3* pGPC2Output;
		StdSwitch3* pGPC3Output;
		StdSwitch3* pGPC4Output;
		StdSwitch3* pGPC5Output;
		*/

		Add(pGPC1Output = new StdSwitch3(_sts, "GPC OUTPUT 1"));
		pGPC1Output->SetLabel(0, "BACKUP");
		pGPC1Output->SetLabel(1, "NORMAL");
		pGPC1Output->SetLabel(2, "TERMINATE");
		pGPC1Output->SetMouseRegion(0.4798f, 0.6438f, 0.5473f, 0.7007f);
		pGPC1Output->DefineSwitchGroup(GRP_O6S35_VC);
		pGPC1Output->SetReference(_V(0.0,  3.07197,  14.08016), SWITCH_VERTICAL);
		pGPC1Output->SetInitialAnimState(0.5f);

		Add(pGPC2Output = new StdSwitch3(_sts, "GPC OUTPUT 2"));
		pGPC2Output->SetLabel(0, "BACKUP");
		pGPC2Output->SetLabel(1, "NORMAL");
		pGPC2Output->SetLabel(2, "TERMINATE");
		pGPC2Output->SetMouseRegion(0.5587f, 0.6438f, 0.6032f, 0.7007f);
		pGPC2Output->DefineSwitchGroup(GRP_O6S36_VC);
		pGPC2Output->SetReference(_V(0.0,  3.07197,  14.08016), SWITCH_VERTICAL);
		pGPC2Output->SetInitialAnimState(0.5f);

		Add(pGPC3Output = new StdSwitch3(_sts, "GPC OUTPUT 3"));
		pGPC3Output->SetLabel(0, "BACKUP");
		pGPC3Output->SetLabel(1, "NORMAL");
		pGPC3Output->SetLabel(2, "TERMINATE");
		pGPC3Output->SetMouseRegion(0.6212f, 0.6438f, 0.6657f, 0.7007f);
		pGPC3Output->DefineSwitchGroup(GRP_O6S37_VC);
		pGPC3Output->SetReference(_V(0.0,  3.07197,  14.08016), SWITCH_VERTICAL);
		pGPC3Output->SetInitialAnimState(0.5f);

		Add(pGPC4Output = new StdSwitch3(_sts, "GPC OUTPUT 4"));
		pGPC4Output->SetLabel(0, "BACKUP");
		pGPC4Output->SetLabel(1, "NORMAL");
		pGPC4Output->SetLabel(2, "TERMINATE");
		pGPC4Output->SetMouseRegion(0.6828f, 0.6438f, 0.7375f, 0.7007f);
		pGPC4Output->DefineSwitchGroup(GRP_O6S38_VC);
		pGPC4Output->SetReference(_V(0.0,  3.07197,  14.08016), SWITCH_VERTICAL);
		pGPC4Output->SetInitialAnimState(0.5f);

		Add(pGPC5Output = new StdSwitch3(_sts, "GPC OUTPUT 5"));
		pGPC5Output->SetLabel(0, "BACKUP");
		pGPC5Output->SetLabel(1, "NORMAL");
		pGPC5Output->SetLabel(2, "TERMINATE");
		pGPC5Output->SetMouseRegion(0.7538f, 0.6438f, 0.8021f, 0.7007f);
		pGPC5Output->DefineSwitchGroup(GRP_O6S39_VC);
		pGPC5Output->SetReference(_V(0.0,  3.07197,  14.08016), SWITCH_VERTICAL);
		pGPC5Output->SetInitialAnimState(0.5f);


		/*
		StdSwitch3* pIPLSource;
		*/

		Add(pIPL1 = new PushButton(_sts, "IPL1"));
		pIPL1->SetMouseRegion(0.4949f, 0.7365f, 0.5439f, 0.7741f);
		Add(pIPL2 = new PushButton(_sts, "IPL2"));
		pIPL2->SetMouseRegion(0.5599f, 0.7365f, 0.6079f, 0.7741f);
		Add(pIPL3 = new PushButton(_sts, "IPL3"));
		pIPL3->SetMouseRegion(0.6268f, 0.7365f, 0.6727f, 0.7741f);
		Add(pIPL4 = new PushButton(_sts, "IPL4"));
		pIPL4->SetMouseRegion(0.6905f, 0.7365f, 0.7396f, 0.7741f);
		Add(pIPL5 = new PushButton(_sts, "IPL5"));
		pIPL5->SetMouseRegion(0.7554f, 0.7365f, 0.8026f, 0.7741f);

		//0.0,  3.033531,  14.23673
		Add(pGPC1Mode = new StdSwitch3(_sts, "GPC MODE 1"));
		pGPC1Mode->SetLabel(0, "RUN");
		pGPC1Mode->SetLabel(1, "STBY");
		pGPC1Mode->SetLabel(2, "HALT");
		pGPC1Mode->SetMouseRegion(0.4798f, 0.8718f, 0.5473f, 0.9117f);
		pGPC1Mode->DefineSwitchGroup(GRP_O6S46_VC);
		pGPC1Mode->SetReference(_V(0.0,  3.033531,  14.23673), SWITCH_VERTICAL);
		pGPC1Mode->SetInitialAnimState(0.5f);

		Add(pGPC2Mode = new StdSwitch3(_sts, "GPC MODE 2"));
		pGPC2Mode->SetLabel(0, "RUN");
		pGPC2Mode->SetLabel(1, "STBY");
		pGPC2Mode->SetLabel(2, "HALT");
		pGPC2Mode->SetMouseRegion(0.5587f, 0.8718f, 0.6032f, 0.9117f);
		pGPC2Mode->DefineSwitchGroup(GRP_O6S47_VC);
		pGPC2Mode->SetReference(_V(0.0,  3.033531,  14.23673), SWITCH_VERTICAL);
		pGPC2Mode->SetInitialAnimState(0.5f);

		Add(pGPC3Mode = new StdSwitch3(_sts, "GPC MODE 3"));
		pGPC3Mode->SetLabel(0, "RUN");
		pGPC3Mode->SetLabel(1, "STBY");
		pGPC3Mode->SetLabel(2, "HALT");
		pGPC3Mode->SetMouseRegion(0.6212f, 0.8718f, 0.6657f, 0.9117f);
		pGPC3Mode->DefineSwitchGroup(GRP_O6S48_VC);
		pGPC3Mode->SetReference(_V(0.0,  3.033531,  14.23673), SWITCH_VERTICAL);
		pGPC3Mode->SetInitialAnimState(0.5f);

		Add(pGPC4Mode = new StdSwitch3(_sts, "GPC MODE 4"));
		pGPC4Mode->SetLabel(0, "RUN");
		pGPC4Mode->SetLabel(1, "STBY");
		pGPC4Mode->SetLabel(2, "HALT");
		pGPC4Mode->SetMouseRegion(0.6828f, 0.8718f, 0.7375f, 0.9117f);
		pGPC4Mode->DefineSwitchGroup(GRP_O6S49_VC);
		pGPC4Mode->SetReference(_V(0.0,  3.033531,  14.23673), SWITCH_VERTICAL);
		pGPC4Mode->SetInitialAnimState(0.5f);

		Add(pGPC5Mode = new StdSwitch3(_sts, "GPC MODE 5"));
		pGPC5Mode->SetLabel(0, "RUN");
		pGPC5Mode->SetLabel(1, "STBY");
		pGPC5Mode->SetLabel(2, "HALT");
		pGPC5Mode->SetMouseRegion(0.7538f, 0.8718f, 0.8021f, 0.9117f);
		pGPC5Mode->DefineSwitchGroup(GRP_O6S50_VC);
		pGPC5Mode->SetReference(_V(0.0,  3.033531,  14.23673), SWITCH_VERTICAL);
		pGPC5Mode->SetInitialAnimState(0.5f);
		oapiWriteLog("(PanelO6::PanelO6) Leave constructor.");
	}

	PanelO6::~PanelO6() {
	}

	void PanelO6::DefineVC()
	{
		oapiWriteLog("(PanelO6::Define VC)");
		//Define VC regions
		//AddAIDToMouseEventList(AID_A7A3);
		AddAIDToMouseEventList(AID_O6);
	}

	void PanelO6::Realize()
	{
		//Warning: This is not correct, panel should talk to FMCA
		discsignals::DiscreteBundle* pBundle = 
			STS()->BundleManager()->CreateBundle("FMCA_STARTRACKER", 16);

		pSTRKDoorControlSys1->outputA.Connect(pBundle, 0);
		pSTRKDoorControlSys1->outputB.Connect(pBundle, 1);

		pSTRKDoorControlSys2->outputA.Connect(pBundle, 4);
		pSTRKDoorControlSys2->outputB.Connect(pBundle, 5);
		
	}

	void PanelO6::RegisterVC()
	{
		oapiWriteLog("(PanelO6::RegisterVC) Begin registration.");
		BasicPanel::RegisterVC();
		VECTOR3 ofs = STS()->orbiter_ofs;

		oapiVCRegisterArea(AID_O6, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP);
		oapiVCSetAreaClickmode_Quadrilateral (AID_O6, 
			_V(-0.726224, 3.18582, 13.6082)+ofs, _V(-0.217782, 3.18582, 13.6082)+ofs, 
			_V(-0.726224, 3.01496, 14.3133) + ofs, _V(-0.217782, 3.01496, 14.3133)+ofs);
	}

};