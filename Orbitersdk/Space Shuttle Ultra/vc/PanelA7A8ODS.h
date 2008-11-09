#pragma once
#include "BasicPanel.h"
#include "StandardSwitch.h"
#include "PushButton.h"

namespace vc {
	/**
	 * Example implementation of a BasicPanel derived panel
	 */
	class PanelA7A8ODS: public BasicPanel
	{
		StdSwitch3* pSystemPowerMNA;
		StdSwitch3* pSystemPowerMNB;
		StdSwitch2* pPyroPowerMNA;
		StdSwitch2* pPyroPowerMNC;
		StdSwitch3* pSys1VentIsol;
		StdSwitch3* pSys1Vent;
		StdSwitch3* pSys2VentIsol;
		StdSwitch3* pSys2Vent;
		StdSwitch2* pPSUPowerMNA;
		StdSwitch2* pPSUPowerMNB;
		StdSwitch2* pLightsAirlock14;
		StdSwitch2* pLightsAirlock23;
		StdSwitch2* pLightsDockingFWD;
		StdSwitch2* pLightsDockingAFT;
		StdSwitch2* pARLKTNLFanA;
		StdSwitch2* pARLKTNLFanB;
		StdSwitch2* pLightsCLVestPORT;
		StdSwitch2* pLightsCLVestSTBD;
		StdSwitch2* pControlPanelPowerA;
		StdSwitch2* pControlPanelPowerB;
		StdSwitch2* pControlPanelPowerC;
		
		StdSwitch2* pHeatersDCUPowerH1;
		StdSwitch2* pHeatersDCUPowerH2;
		StdSwitch2* pHeatersDCUPowerH3;

		StdSwitch2* pAPDSPowerA;
		StdSwitch2* pAPDSPowerB;
		StdSwitch2* pAPDSPowerC;
		
		StdSwitch2* pPyrosAp;
		StdSwitch2* pPyrosBp;
		StdSwitch2* pPyrosCp;

		PushButton* pLampTest;
		PushButton* pPowerOn;
		PushButton* pPowerOff;
		PushButton* pRingOut;
		PushButton* pRingIn;
		PushButton* pAPDSCircProtOff;
		PushButton* pCloseHooks;
		PushButton* pCloseLatches;
		PushButton* pFixerOff;


		UINT midx_odspanel;
	public:
		PanelA7A8ODS(Atlantis* _sts);
		virtual ~PanelA7A8ODS();
		virtual void AddMeshes(const VECTOR3& ofs);
		virtual void DefineVCAnimations (UINT vcidx);
		virtual void DefineVC();
		virtual UINT GetVCMeshIndex() const;
		virtual void Realize();
		virtual void RegisterVC();
	};
};