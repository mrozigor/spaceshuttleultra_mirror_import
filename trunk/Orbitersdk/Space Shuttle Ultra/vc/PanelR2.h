#ifndef __VC_PANEL_R2_H_12F72432_EB9D_4b53_BC9D_4BCD6DD61C17
#define __VC_PANEL_R2_H_12F72432_EB9D_4b53_BC9D_4BCD6DD61C17
#pragma once

#include "BasicPanel.h"
#include "StandardSwitch.h"
#include "Talkback.h"

namespace vc
{
	class PanelR2 : public BasicPanel
	{
		LockableLever3* pAPUControl[3];
		LockableLever2* pHydPumpPress[3];
		LockableLever2* pAPUCntlrPwr[3];
		LockableLever2* pAPUFuelTkVlv[3];
		StdSwitch2* pAPUSpeedSelect[3];

		StandardTalkback* pAPUReadyToStart[3];

		DiscInPort APU_HydraulicPress[3];
	public:
		PanelR2(Atlantis* psts);
		virtual ~PanelR2();

		virtual void RegisterVC();
		virtual void DefineVC();
		virtual void Realize();

		virtual bool HydraulicPressure() const;
		void CheckMPSArmed(int eng) const;
	};
};

#endif //__VC_PANEL_R2_H_12F72432_EB9D_4b53_BC9D_4BCD6DD61C17