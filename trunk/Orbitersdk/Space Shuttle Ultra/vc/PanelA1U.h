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
#include "StandardRotarySwitch.h"
#include "Talkback.h"
#include "PushButtonIndicator.h"

namespace vc
{
	class PanelA1U: public BasicPanel
	{
	public:
		PanelA1U(Atlantis* _sts);
		virtual ~PanelA1U();

		virtual void DefineVC();
		virtual void RegisterVC();
		virtual void Realize();

		virtual void OnPreStep(double SimT, double DeltaT, double MJD);

		//virtual bool OnVCMouseEvent (int id, int _event, VECTOR3 &p);
		//virtual bool OnVCRedrawEvent (int id, int _event, SURFHANDLE surf);
	private:
		//bool DrawPBILight(SURFHANDLE surf, int id, bool bOn);
		//void UpdatePBIs();

		StdSwitch3* SignalStrengthSelect;
		StdSwitch3* SlewAzimuth;
		StdSwitch3* SlewElevation;
		StdSwitch2* SlewRate;
		StdSwitch2* Control;
		RotaryDemuxSwitch* SteeringMode;
		StdSwitch2* Search;
		StdSwitch3* Power;
		StdSwitch3* Mode;
		StdSwitch3* RadarOutput;
		RotaryDemuxSwitch* HighDataRateSP;
		RotaryDemuxSwitch* LowDataRateSP;
		LockableLever2* TAGSEncrypt;

	};
};
