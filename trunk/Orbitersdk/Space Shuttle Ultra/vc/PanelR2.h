/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel R2 VC Component



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
#ifndef __VC_PANEL_R2_H_12F72432_EB9D_4b53_BC9D_4BCD6DD61C17
#define __VC_PANEL_R2_H_12F72432_EB9D_4b53_BC9D_4BCD6DD61C17
#pragma once

#include "AtlantisPanel.h"
#include "StandardSwitch.h"
#include "Talkback.h"

namespace vc
{
	class PanelR2 : public AtlantisPanel
	{
		StdSwitch3* pBlrCntlrPwrHtr[3];
		StdSwitch2* pBlrCntlr[3];
		StdSwitch2* pBlrN2Supply[3];

		LockableLever3* pAPUControl[3];
		LockableLever2* pHydPumpPress[3];
		LockableLever2* pAPUCntlrPwr[3];
		LockableLever2* pAPUFuelTkVlv[3];
		StdSwitch2* pAPUSpeedSelect[3];

		StdSwitch2* pCenterlineLatch;
		StdSwitch3* pLeftUmbDoor;
		StdSwitch3* pLeftUmbDoorLatch;
		StdSwitch3* pRightUmbDoor;
		StdSwitch3* pRightUmbDoorLatch;

		StdSwitch2* pMPSPower[2][3];
		StdSwitch3* pMPSHeIsolA[3];
		StdSwitch3* pMPSHeIsolB[3];

		StdSwitch3* pMPSPRPLTDumpSequence;
		StdSwitch3* pMPSPRPLTBackupLH2vlv;

		StdSwitch3* pPneuLEngHeXOVR;
		StdSwitch3* pPneuHeIsol;

		StdSwitch3* pHeInterconnect[3];

		LockableLever2* pLH2UllagePress;

		StandardTalkback* pAPUReadyToStart[3];

		StandardTalkback* pETUmbDoorCLLatch;
		StandardTalkback* pETUmbLDoor;
		StandardTalkback* pETUmbLLatch;
		StandardTalkback* pETUmbRDoor;
		StandardTalkback* pETUmbRLatch;

		DiscInPort APU_HydraulicPress[3];
	public:
		PanelR2(Atlantis* psts);
		virtual ~PanelR2();

		virtual void RegisterVC();
		virtual void DefineVC();
		virtual void Realize();
	};
};

#endif //__VC_PANEL_R2_H_12F72432_EB9D_4b53_BC9D_4BCD6DD61C17