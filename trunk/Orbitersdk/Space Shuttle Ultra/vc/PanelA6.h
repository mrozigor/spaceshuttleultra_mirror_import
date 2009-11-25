/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel A6 definition



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
#include "StandardRotarySwitch.h"
#include "Talkback.h"
#include "PushButtonIndicator.h"

namespace vc
{
	class PanelA6: public BasicPanel
	{
	public:
		PanelA6(Atlantis* _sts);
		virtual ~PanelA6();

		virtual void DefineVC();
		virtual void RegisterVC();
		virtual void Realize();

		virtual void OnPreStep(double SimT, double DeltaT, double MJD);

		//virtual bool OnVCMouseEvent (int id, int _event, VECTOR3 &p);
		//virtual bool OnVCRedrawEvent (int id, int _event, SURFHANDLE surf);
	private:
		//bool DrawPBILight(SURFHANDLE surf, int id, bool bOn);
		//void UpdatePBIs();
		typedef enum { PL1=0, PL2=1, PL3=2, MON=3 } PAYLOAD; // both monitor positions are the same, so we only need 1 MON enum

		PanelA6::PAYLOAD GetSelectedPayload() const;

		LockableLever2* pFltCntlrPower;
		StdSwitch2* pSense;

		StdSwitch3* pPayloadRetentionLatches[5];

		RotaryDemuxSwitch* pPayloadSelect;

		//DAP PBIs
		PushButtonIndicator* pPBIs[24];

		DiscInPort LatchSwitch_Latch[5], LatchSwitch_Release[5];
		DiscOutPort Latch_Latch[3][5], Latch_Release[3][5];
		DiscInPort Latch_Latched[3][5], Latch_Released[3][5], Latch_RTL[3][5];
		DiscOutPort Latch_LatchedTkbk[5], Latch_ReleasedTkbk[5], Latch_RTLTkbk[5];
		DiscInPort PayloadSelect[5];

		StandardTalkback* pLatchState[5];
		StandardTalkback* pLatchRTL[5];

		//PBI lights
		//bool PBI_Lights[24]; //true if light on
	};
};
