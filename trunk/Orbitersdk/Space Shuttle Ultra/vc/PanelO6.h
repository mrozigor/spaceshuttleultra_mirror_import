/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel O6 definition



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

#include "AtlantisPanel.h"
#include "StandardSwitch.h"
#include "StandardSwitchCover.h"
#include "PushButton.h"
#include "Talkback.h"

namespace vc {

	class PanelO6 : public AtlantisPanel {
	private:
		StdSwitch3* pLGlareShieldFlood;
		StandardTalkback3* pSTYDoorPosition;
		StandardTalkback3* pSTZDoorPosition;
		StdSwitch3* pSTRKDoorControlSys1;
		StdSwitch3* pSTRKDoorControlSys2;
		StandardSwitchCover* pSTRKDoorControlSys1Cover;
		StandardSwitchCover* pSTRKDoorControlSys2Cover;
		StdSwitch2* pSTRKPowerNY;
		StdSwitch2* pSTRKPowerNZ;
		StdSwitch3* pLeftSeatCenterFlood;
		StdSwitch2* pUHFXmitFreq;
		StdSwitch2* pUHFSPLXPowerAmp;
		StdSwitch2* pUHFSquelch;

		StdSwitch3* pAnnunciatorLampTest;
		StdSwitch3* pAnnunciatorBusSelectACA1;
		StdSwitch3* pAnnunciatorBusSelectACA23;

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

		StandardSwitchCover* pGPCPowerCover[5];

		StdSwitch2* pGPC1Pwr;
		StdSwitch2* pGPC2Pwr;
		StdSwitch2* pGPC3Pwr;
		StdSwitch2* pGPC4Pwr;
		StdSwitch2* pGPC5Pwr;

		StandardTalkback2* pGPCOutput[5];

		StandardSwitchCover* pGPCOutputCover[5];

		StdSwitch3* pGPC1Output;
		StdSwitch3* pGPC2Output;
		StdSwitch3* pGPC3Output;
		StdSwitch3* pGPC4Output;
		StdSwitch3* pGPC5Output;

		StdSwitch3* pIPLSource;
		PushButton* pIPL1;
		PushButton* pIPL2;
		PushButton* pIPL3;
		PushButton* pIPL4;
		PushButton* pIPL5;

		StandardTalkback3* pGPCMode[5];

		LockableLever3* pGPC1Mode;
		LockableLever3* pGPC2Mode;
		LockableLever3* pGPC3Mode;
		LockableLever3* pGPC4Mode;
		LockableLever3* pGPC5Mode;
	public:
		PanelO6(Atlantis* psts);
		virtual ~PanelO6();
		virtual void DefineVC();
		virtual void RegisterVC();
		virtual void Realize();

	};
};