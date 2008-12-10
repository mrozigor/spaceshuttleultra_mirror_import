#ifndef __PANELF4_H
#define __PANELF4_H
#pragma once

#include "BasicPanel.h"
#include "PushButtonIndicator.h"

namespace vc
{
	class PanelF4: public BasicPanel
	{
		PushButtonIndicator *pBodyFlap, *pSbdbkThrot; //handles AUTO light
		StandardLight *pBodyFlapMan, *pSbdbkThrotMan;

	public:
		PanelF4(Atlantis* _sts);
		~PanelF4();

		virtual void Realize();

		virtual void DefineVC();
		virtual void RegisterVC();
	};
};

#endif //__PANELF4_H