/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel F6 definition



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

#include "AtlantisPanel.h"
#include "MDU.h"
#include "StandardSwitch.h"

namespace vc {

	class PanelF6: public AtlantisPanel
	{
		MDU* pCDR1;
		MDU* pCDR2;
		StdSwitch3* pHSISelectMode;
		StdSwitch3* pHSISelectSource;
		StdSwitch3* pHSISelectChannel;
		StdSwitch2* pRdrAltm;
		StdSwitch2* pFltCntlrPower;
		StdSwitch3* pADIAttitude;
		StdSwitch3* pADIError;
		StdSwitch3* pADIRate;
		StdSwitch3* pAirData;
	public:
		PanelF6(Atlantis* _sts);
		virtual ~PanelF6();
		virtual void DefineVC();
		virtual void RegisterVC();

		virtual void Realize();
		
	};
};