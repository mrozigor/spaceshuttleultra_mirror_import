#pragma once

#include "BasicPanel.h"
#include "StandardSwitch.h"

namespace vc
{
	class PanelA6: public BasicPanel
	{
	public:
		PanelA6(Atlantis* _sts);
		virtual ~PanelA6();

		virtual void DefineVC();
		virtual void RegisterVC();

		virtual bool OnVCMouseEvent (int id, int _event, VECTOR3 &p);
		virtual bool OnVCRedrawEvent (int id, int _event, SURFHANDLE surf);
	private:
		bool DrawPBILight(SURFHANDLE surf, int id, bool bOn);
		void UpdatePBIs();

		LockableLever* pFltCntlrPower;
		StdSwitch2* pSense;

		//PBI lights
		bool PBI_Lights[24]; //true if light on
	};
};