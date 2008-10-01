#pragma once

#include "BasicPanel.h"
#include "MDU.h"
#include "StandardSwitch.h"

namespace vc {

	class PanelF7: public BasicPanel
	{
	private:
		MDU* pCRT1;
		MDU* pCRT2;
		MDU* pCRT3;
		MDU* pMFD1;
		MDU* pMFD2;

		short sTimerSeconds;
		short sTimerMinutes;

		//Event timer class
		//C&W warning lights
	public:
		PanelF7(Atlantis* _sts);
		virtual ~PanelF7();
		virtual void DefineVC();
		virtual void RegisterVC();

		virtual void OnPostStep (double fSimT, double fDeltaT, double fMJD);
		virtual bool OnVCRedrawEvent (int id, int _event, SURFHANDLE surf);
	};
};
