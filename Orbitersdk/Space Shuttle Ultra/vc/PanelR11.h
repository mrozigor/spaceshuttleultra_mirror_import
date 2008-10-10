#pragma once

#ifndef _PANELR11_H
#define _PANELR11_H

#include "BasicPanel.h"
#include "MDU.h"

namespace vc
{
	class PanelR11 : public BasicPanel
	{
	private:
		MDU* pCRT4;
	public:
		PanelR11(Atlantis* _sts);
		virtual ~PanelR11();
		virtual void DefineVC();
		virtual void RegisterVC();
	};
};

#endif //_PANELR11_H