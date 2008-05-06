#pragma once
#include "BasicPanel.h"
#include "StandardSwitch.h"

namespace vc {
	/**
	 * Example implementation of a BasicPanel derived panel
	 */
	class PanelA7A8ODS: public BasicPanel
	{
		StdSwitch3 SystemPowerMNA;
		StdSwitch3 SystemPowerMNB;
		StdSwitch2 PyroPowerMNA;
		StdSwitch2 PyroPowerMNC;
		StdSwitch3 Sys1VentIsol;
		StdSwitch3 Sys1Vent;
		StdSwitch3 Sys2VentIsol;
		StdSwitch3 Sys2Vent;
		StdSwitch2 PSUPowerMNA;
		StdSwitch2 PSUPowerMNB;
		StdSwitch2 LightsAirlock14;
		StdSwitch2 LightsAirlock23;
		StdSwitch2 LightsDockingFWD;
		StdSwitch2 LightsDockingAFT;
		StdSwitch2 ARLKTNLFanA;
		StdSwitch2 ARLKTNLFanB;
		StdSwitch2 LightsCLVestPORT;
		StdSwitch2 LightsCLVestSTBD;

		UINT midx_odspanel;
	public:
		PanelA7A8ODS(Atlantis* _sts);
		virtual ~PanelA7A8ODS();
		virtual void AddMeshes(const VECTOR3& ofs);
		virtual void Realize();
		virtual void DefineVCAnimations (UINT vcidx);
		virtual void DefineVC();
	};
};