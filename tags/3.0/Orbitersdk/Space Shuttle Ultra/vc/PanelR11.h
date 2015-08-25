/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel R11 definition



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

#ifndef _PANELR11_H
#define _PANELR11_H

#include "AtlantisPanel.h"
#include "MDU.h"
#include "Keyboard.h"

namespace vc
{
	class PanelR11 : public AtlantisPanel
	{
	private:
		MDU* pCRT4;

		Keyboard* pKeyboardAft;

		StdSwitch2* pIDPCRTPower;
		StdSwitch3* pIDPCRTMajFunc;
	public:
		PanelR11(Atlantis* _sts);
		virtual ~PanelR11();
		virtual void DefineVC();
		virtual void RegisterVC();
		virtual void Realize();
	};
};

#endif //_PANELR11_H