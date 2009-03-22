#include "../Atlantis.h"
#include "PanelO6.h"
#include "../meshres_vc.h"


namespace vc {

	PanelO6::PanelO6(Atlantis* _sts) 
		: BasicPanel(_sts, "O6") {

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
		//-0.382601  3.152154  13.74688
		pSTRKDoorControlSys1->SetReference(_V(-0.0, 3.152154, 13.74688), _V(1.0, 0.0, 0.0));
		
		Add(pSTRKDoorControlSys2	= new StdSwitch3(_sts, "S TRK DR CNTL SYS2"));		 
		pSTRKDoorControlSys2->SetLabel(0, "CLOSE");
		pSTRKDoorControlSys2->SetLabel(1, "OFF");
		pSTRKDoorControlSys2->SetLabel(2, "OPEN");
		pSTRKDoorControlSys2->DefineSwitchGroup(GRP_O6S3_VC);
		pSTRKDoorControlSys2->SetReference(_V(-0.0, 3.152154, 13.74688), _V(1.0, 0.0, 0.0));
		
		
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
		StdSwitch3* pGPC1Output;
		StdSwitch3* pGPC2Output;
		StdSwitch3* pGPC3Output;
		StdSwitch3* pGPC4Output;
		StdSwitch3* pGPC5Output;
		StdSwitch3* pIPLSource;
		StdSwitch3* pGPC1Mode;
		StdSwitch3* pGPC2Mode;
		StdSwitch3* pGPC3Mode;
		StdSwitch3* pGPC4Mode;
		StdSwitch3* pGPC5Mode;
		*/
		oapiWriteLog("(PanelO6::PanelO6) Leave constructor.");
	}

	PanelO6::~PanelO6() {
	}

	void PanelO6::DefineVC()
	{
		oapiWriteLog("Panel O6:\tDefine VC");
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