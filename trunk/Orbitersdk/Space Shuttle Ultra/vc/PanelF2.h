#ifndef __PANELF2_H
#define __PANELF2_H
#pragma once

#include "BasicPanel.h"
#include "PushButtonIndicator.h"

namespace vc
{
	class PanelF2: public BasicPanel
	{
		/*PushButtonIndicator *pBFAuto, *pBFMan;
		PushButtonIndicator *pSbdThrotAuto, *pSbdThrotMan;*/
		PushButtonIndicator *pBodyFlap, *pSbdbkThrot; //handle AUTO light
		StandardLight *pBodyFlapMan, *pSbdbkThrotMan;

	public:
		PanelF2(Atlantis* _sts);
		~PanelF2();

		virtual void Realize();

		virtual void DefineVC();
		virtual void RegisterVC();
	};
};

#endif //__PANELF2_H