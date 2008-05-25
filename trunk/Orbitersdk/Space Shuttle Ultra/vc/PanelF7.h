#pragma once

#include "BasicPanel.h"
#include "MDU.h"
#include "StandardSwitch.h"

namespace vc {

	class PanelF7: public BasicPanel
	{
		MDU* CRT1;
		MDU* CRT2;
		MDU* CRT3;
		MDU* MFD1;
		MDU* MFD2;

		//Event timer class
		//C&W warning lights
	public:
		PanelF7(Atlantis* _sts);
		virtual ~PanelF7();
		virtual void DefineVC();
	};
};