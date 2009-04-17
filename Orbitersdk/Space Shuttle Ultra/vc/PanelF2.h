/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel F2 definition



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
#ifndef __PANELF2_H
#define __PANELF2_H
#pragma once

#include "BasicPanel.h"
#include "PushButtonIndicator.h"

namespace vc
{
	class PanelF2: public BasicPanel
	{
		/*PushButtonIndicator *pBFAuto, *pBFMan;
		PushButtonIndicator *pSbdThrotAuto, *pSbdThrotMan;*/
		PushButtonIndicator *pBodyFlap, *pSbdbkThrot; //handle AUTO light
		StandardLight *pBodyFlapMan, *pSbdbkThrotMan;

	public:
		PanelF2(Atlantis* _sts);
		~PanelF2();

		virtual void Realize();

		virtual void DefineVC();
		virtual void RegisterVC();
	};
};

#endif //__PANELF2_H