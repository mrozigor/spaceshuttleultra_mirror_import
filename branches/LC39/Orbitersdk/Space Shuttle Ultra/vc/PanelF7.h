/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel F7 definition



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
#include "StandardLight.h"

namespace vc {

	class PanelF7: public AtlantisPanel
	{
	private:
		MDU* pCRT1;
		MDU* pCRT2;
		MDU* pCRT3;
		MDU* pMFD1;
		MDU* pMFD2;

		short sTimerSeconds;
		short sTimerMinutes;

		StandardLight* pMainEngStatusR[3];
		StandardLight* pMainEngStatusY[3];

		//Event timer class
		//C&W warning lights
	public:
		PanelF7(Atlantis* _sts);
		virtual ~PanelF7();
		virtual void DefineVC();
		virtual void RegisterVC();
		virtual void Realize();

		virtual void OnPostStep (double fSimT, double fDeltaT, double fMJD);
		virtual bool OnVCRedrawEvent (int id, int _event, SURFHANDLE surf);
	};
};
