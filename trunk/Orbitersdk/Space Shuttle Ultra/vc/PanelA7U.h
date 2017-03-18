/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel A7U definition



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
#ifndef _VC_PANELA7U_H
#define _VC_PANELA7U_H
#pragma once

#include "AtlantisPanel.h"
#include "StandardSwitch.h"
#include "PushButtonIndicator.h"
#include "Talkback.h"


namespace vc
{
	class PanelA7U : public AtlantisPanel
	{
		StandardTalkback2* pCameraPowerA_TB;
		StandardTalkback2* pCameraPowerB_TB;
		StandardTalkback2* pCameraPowerC_TB;
		StandardTalkback2* pCameraPowerD_TB;
		StandardTalkback2* pCameraPowerRMS_TB;

		PushButtonIndicatorSingleLight* pMasterAlarm;

		PushButtonIndicatorSingleLight* pVideoInputA;
		PushButtonIndicatorSingleLight* pVideoInputB;
		PushButtonIndicatorSingleLight* pVideoInputC;
		PushButtonIndicatorSingleLight* pVideoInputD;
		PushButtonIndicatorSingleLight* pVideoInputRMS;
		PushButtonIndicatorSingleLight* pVideoInputFltDeck;
		PushButtonIndicatorSingleLight* pVideoInputMidDeck;
		PushButtonIndicatorSingleLight* pVideoInputPL1;
		PushButtonIndicatorSingleLight* pVideoInputPL2;
		PushButtonIndicatorSingleLight* pVideoInputPL3;
		PushButtonIndicatorSingleLight* pVideoInputMUX1;
		PushButtonIndicatorSingleLight* pVideoInputMUX2;
		PushButtonIndicatorSingleLight* pVideoInputTest;

		PushButtonIndicatorSingleLight* pVideoOutputMon1;
		PushButtonIndicatorSingleLight* pVideoOutputMon2;
		PushButtonIndicatorSingleLight* pVideoOutputDownLink;
		PushButtonIndicatorSingleLight* pVideoOutputDTV;
		PushButtonIndicatorSingleLight* pVideoOutputMUX1L;
		PushButtonIndicatorSingleLight* pVideoOutputMUX1R;
		PushButtonIndicatorSingleLight* pVideoOutputMUX2L;
		PushButtonIndicatorSingleLight* pVideoOutputMUX2R;
		
		PushButtonIndicatorSingleLight* pMenuALC;
		PushButtonIndicatorSingleLight* pMenuManGain;
		PushButtonIndicatorSingleLight* pMenuColorBalLTLevel;
		PushButtonIndicatorSingleLight* pMode1;
		PushButtonIndicatorSingleLight* pMode2;
		PushButtonIndicatorSingleLight* pMode3;

		StdSwitch3* pCameraPowerA;
		StdSwitch3* pCameraPowerB;
		StdSwitch3* pCameraPowerC;
		StdSwitch3* pCameraPowerD;
		StdSwitch3* pCameraPowerRMS;
		
		StdSwitch3* pCameraPan;
		StdSwitch3* pCameraTilt;
		StdSwitch3* pPanTiltRate;

		StdSwitch2* pPLBDLights[7];
		StdSwitch3* pDockingLight;
		StdSwitch2* pRMSLight;

	public:
		PanelA7U(Atlantis* _sts);
		virtual ~PanelA7U();

		virtual void DefineVC();
		virtual void RegisterVC();
		virtual void Realize();
	};
};

#endif //_VC_PANELA7U_H
