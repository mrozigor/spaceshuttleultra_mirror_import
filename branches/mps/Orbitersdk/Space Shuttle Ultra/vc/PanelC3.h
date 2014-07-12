/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel C3 VC Component



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
#ifndef __VC_PANEL_C3_H_8B69A84E_82D7_49e2_BE5D_B389A8E2B914
#define __VC_PANEL_C3_H_8B69A84E_82D7_49e2_BE5D_B389A8E2B914
#pragma once

#include "AtlantisPanel.h"
#include "StandardSwitch.h"
#include "PushbuttonIndicator.h"
#include "PushButton.h"
#include "StandardSwitchCover.h"

namespace vc
{
	class PanelC3 : public AtlantisPanel
	{
	public:
		PanelC3(Atlantis* _sts);
		virtual ~PanelC3();

		virtual void DefineVC();
		virtual void RegisterVC();
		virtual void Realize();
	private:
		//DAP PBIs
		PushButtonIndicator* pPBIs[24];

		LockableLever3* pOMSArm[2];

		StdSwitch2* pAirDataProbeStowEnable[2];
		LockableLever3* pAirDataProbeDeploy[2];

		StdSwitch3* pSSMELimitShutDn;

		StandardSwitchCover* pSSMESDPBCover[3];
		PushButton* pSSMESDPB[3];

		StdSwitch2* pSRBSEPSW;
		StandardSwitchCover* pSRBSEPCover;
		PushButton* pSRBSEPPB;

		LockableLever2* pETSEPSW;
		StandardSwitchCover* pETSEPCover;
		PushButton* pETSEPPB;
	};
};

#endif //__VC_PANEL_C3_H_8B69A84E_82D7_49e2_BE5D_B389A8E2B914
