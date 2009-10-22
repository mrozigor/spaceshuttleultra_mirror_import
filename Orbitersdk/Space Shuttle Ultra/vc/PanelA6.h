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

		//virtual bool OnVCMouseEvent (int id, int _event, VECTOR3 &p);
		//virtual bool OnVCRedrawEvent (int id, int _event, SURFHANDLE surf);
	private:
		//bool DrawPBILight(SURFHANDLE surf, int id, bool bOn);
		//void UpdatePBIs();

		LockableLever2* pFltCntlrPower;
		StdSwitch2* pSense;

		//DAP PBIs
		PushButtonIndicator* pPBIs[24];

		//PBI lights
		//bool PBI_Lights[24]; //true if light on
	};
};
