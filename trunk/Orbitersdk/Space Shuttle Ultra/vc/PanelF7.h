#pragma once

#include "BasicPanel.h"
#include "MDU.h"
#include "StandardSwitch.h"

namespace vc {

	class PanelF7: public BasicPanel
	{
		MDU* pCRT1;
		MDU* pCRT2;
		MDU* pCRT3;
		MDU* pMFD1;
		MDU* pMFD2;

		//Event timer class
		//C&W warning lights
	public:
		PanelF7(Atlantis* _sts);
		virtual ~PanelF7();
		virtual void DefineVC();
		virtual void RegisterVC();
		
	};
};