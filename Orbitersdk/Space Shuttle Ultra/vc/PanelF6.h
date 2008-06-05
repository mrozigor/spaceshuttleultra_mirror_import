#pragma once

#include "BasicPanel.h"
#include "MDU.h"
#include "StandardSwitch.h"

namespace vc {

	class PanelF6: public BasicPanel
	{
		MDU* pCDR1;
		MDU* pCDR2;
		StdSwitch3* pHSISelectMode;
		StdSwitch3* pHSISelectSource;
		StdSwitch3* pHSISelectChannel;
		StdSwitch2* pRdrAltm;
		StdSwitch2* pFltCntlrPower;
		StdSwitch3* pADIAttitude;
		StdSwitch3* pADIError;
		StdSwitch3* pADIRate;
		StdSwitch3* pAirData;
	public:
		PanelF6(Atlantis* _sts);
		virtual ~PanelF6();
		virtual void DefineVC();
		virtual void RegisterVC();
		
	};
};