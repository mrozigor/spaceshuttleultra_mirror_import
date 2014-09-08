/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel O17 definition



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
#ifndef VC_PANELO17_FBF32366_0422_44ca_9FBA_FE983D60F2D9
#define VC_PANELO17_FBF32366_0422_44ca_9FBA_FE983D60F2D9
#pragma once

#include "AtlantisPanel.h"
#include "StandardSwitch.h"

namespace vc {

class PanelO17 : public AtlantisPanel {
	StdSwitch2* pEIUPowerLC;
	StdSwitch2* pEIUPowerCR;
	StdSwitch2* pEIUPowerRL;
public:
	PanelO17(Atlantis* psts);
	virtual ~PanelO17();

	virtual void DefineVC();
	virtual void RegisterVC();
	virtual void Realize();
};

};

#endif //VC_PANELO17_FBF32366_0422_44ca_9FBA_FE983D60F2D9