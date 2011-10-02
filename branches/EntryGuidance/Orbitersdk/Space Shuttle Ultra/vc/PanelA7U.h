/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel A7U definition



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
#ifndef _VC_PANELA7U_H
#define _VC_PANELA7U_H
#pragma once

#include "AtlantisPanel.h"
#include "StandardSwitch.h"
#include "PushButtonIndicator.h"

namespace vc
{
	class PanelA7U : public AtlantisPanel
	{
		StdSwitch3* pCameraPan;
		StdSwitch3* pCameraTilt;
		StdSwitch3* pPanTiltRate;

		PushButtonIndicator* pCamApbi;
		PushButtonIndicator* pCamBpbi;
		PushButtonIndicator* pCamCpbi;
		PushButtonIndicator* pCamDpbi;
		PushButtonIndicator* pCamRMSpbi;

		PBIDiscPortGroup VideoInputGroup;

		DiscOutPort CamTiltUp_Out[5], CamTiltDown_Out[5], CamPanLeft_Out[5], CamPanRight_Out[5], LowSpeed_Out[5];
		DiscInPort CamTiltUp_In, CamTiltDown_In, CamPanLeft_In, CamPanRight_In, LowSpeed_In;
		DiscInPort CamA, CamB, CamC, CamD, CamRMS;

	public:
		PanelA7U(Atlantis* _sts);
		virtual ~PanelA7U();

		virtual void DefineVC();
		virtual void RegisterVC();
		virtual void Realize();

		virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	};
};

#endif //_VC_PANELA7U_H