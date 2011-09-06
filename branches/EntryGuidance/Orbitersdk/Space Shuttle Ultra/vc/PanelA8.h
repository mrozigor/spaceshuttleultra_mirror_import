/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel A8 definition



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
#ifndef __VC_PANELA8_H
#define __VC_PANELA8_H
#pragma once

#include "BasicPanel.h"
#include "StandardSwitch.h"
#include "StandardSwitchCover.h"
#include "StandardRotarySwitch.h"
#include "Talkback.h"

namespace vc
{
	class PanelA8 : public BasicPanel
	{
		StdSwitch3* pEEMode;
		StdSwitch3* pEEManContr;
		StdSwitch3* pPortMRL;
		StdSwitch3* pStbdMRL;
		StdSwitch3* pPortMPM;
		StdSwitch3* pStbdMPM;
		StdSwitch3* pRMSSelect;
		StdSwitch3* pShoulderBrace;
		StdSwitch3* pSingleDirectDrive;

		StandardSwitchCover* pPortMPMCover;
		StandardSwitchCover* pStbdMPMCover;

		StandardTalkback* pPortMRLTb;
		StandardTalkback* pStbdMRLTb;
		StandardTalkback* pPortMPMTb;
		StandardTalkback* pStbdMPMTb;

		Std2SegTalkback* pPortMRL_RTL[3];
		Std2SegTalkback* pStbdMRL_RTL[3];
		Std2SegTalkback* pEECapture;
		Std2SegTalkback* pEEExtend;
		Std2SegTalkback* pEEClose;
		Std2SegTalkback* pEEOpen;
		Std2SegTalkback* pEERigid;
		Std2SegTalkback* pEEDerigid;
		Std2SegTalkback* pShoulderBraceTb;

		RotaryDemuxSwitch* pLEDParameter;
		RotaryDemuxSwitch* pLEDJoint;
		RotaryDemuxSwitch* pRMSMode;
	public:
		PanelA8(Atlantis* _sts);
		virtual ~PanelA8();

		virtual void OnPreStep(double SimT, double DeltaT, double MJD);
		virtual bool OnVCRedrawEvent(int id, int _event, SURFHANDLE surf);

		virtual void RegisterVC();
		virtual void DefineVC();
		virtual void DefineVCAnimations(UINT vcidx);
		virtual void Realize();

		virtual void AddMeshes(const VECTOR3& ofs);
		virtual void SetMeshVisibility(bool visible);
		virtual UINT GetVCMeshIndex() const;
	private:
		MESHHANDLE hPanelMesh;
		UINT mesh_index;

		DiscInPort RMSJointAngles[6], EEPosition[3], EEAttitude[3];
		DiscInPort LED_JointSelect[8], LED_ParameterSelect[8];

		double LEDValues[3];
	};
};

#endif // __VC_PANELA8_H