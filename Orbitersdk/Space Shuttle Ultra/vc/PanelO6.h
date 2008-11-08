#pragma once

#include "BasicPanel.h"
#include "StandardSwitch.h"

namespace vc {

	class PanelO6 : public BasicPanel {
	private:
		StdSwitch3* pLGlareShieldFlood;
		StdSwitch3* pSTRKDoorControlSys1;
		StdSwitch3* pSTRKDoorControlSys2;
		StdSwitch2* pSTRKPowerNY;
		StdSwitch2* pSTRKPowerNZ;
		StdSwitch3* pLeftSeatCenterFlood;
		StdSwitch2* pUHFXmitFreq;
		StdSwitch2* pUHFSPLXPowerAmp;
		StdSwitch2* pUHFSquelch;
		StdSwitch3* pAnnunciatorLampTest;
		StdSwitch3* pBusSelectACA1;
		StdSwitch3* pBusSelectACA23;
		StdSwitch2* pAnnunciatorIntensitySel;
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
		StdSwitch2* pMDMFA3;
		StdSwitch2* pMDMFA4;
		StdSwitch2* pMDMFF1;
		StdSwitch2* pMDMFF2;
		StdSwitch2* pMDMFF3;
		StdSwitch2* pMDMFF4;
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
	public:
		PanelO6(Atlantis* psts);
		virtual ~PanelO6();
		virtual void DefineVC();
		virtual void RegisterVC();

	};
};