/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel A7A3 and Panel A8A3 definition



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/
#pragma once
#include "BasicPanel.h"
#include "StandardSwitch.h"
#include "PushButton.h"
#include "StandardLight.h"

namespace vc {
	/**
	 * Example implementation of a BasicPanel derived panel
	 */
	class PanelA7A8ODS: public BasicPanel
	{

		StandardLight* plADS;
		StandardLight* plBDS;
		StandardLight* plCDS;
		StandardLight* plPowerOn;
		StandardLight* plAPDSProtectCircOff;
		StandardLight* plRingAligned;
		StandardLight* plRingInitialPos;
		StandardLight* plFixersOff;
		StandardLight* plHooks1Open;
		StandardLight* plHooks2Open;
		StandardLight* plLatchesClosed;
		StandardLight* plUndockComplete;
		StandardLight* plInitialContact;
		StandardLight* plCapture;
		StandardLight* plRingForward;
		StandardLight* plReadyToHook;
		StandardLight* plInterpSealed;
		StandardLight* plHooks1Closed;
		StandardLight* plHooks2Closed;
		StandardLight* plLatchesOpen;
		StandardLight* plRingFinal;
		StandardLight* plAp;
		StandardLight* plBp;
		StandardLight* plCp;

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
		MESHHANDLE mesh_odspanel;
	public:
		PanelA7A8ODS(Atlantis* _sts);
		virtual ~PanelA7A8ODS();
		virtual void AddMeshes(const VECTOR3& ofs);
		virtual void SetMeshVisibility(bool visible);
		virtual void DefineVCAnimations (UINT vcidx);
		virtual void DefineVC();
		virtual UINT GetVCMeshIndex() const;
		virtual void Realize();
		virtual void RegisterVC();
	};
};