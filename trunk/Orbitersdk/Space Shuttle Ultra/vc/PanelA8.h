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

#include "AtlantisPanel.h"
#include "StandardSwitch.h"
#include "StandardSwitchCover.h"
#include "StandardRotarySwitch.h"
#include "Talkback.h"
#include "StandardLight.h"
#include "7SegDisp_RMS.h"


namespace vc
{
	class PanelA8 : public AtlantisPanel
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

		StandardTalkback3* pPortMRLTb;
		StandardTalkback3* pStbdMRLTb;
		StandardTalkback3* pPortMPMTb;
		StandardTalkback3* pStbdMPMTb;

		StandardTalkback2* pPortMRL_RTL[3];
		StandardTalkback2* pStbdMRL_RTL[3];
		StandardTalkback2* pEECapture;
		StandardTalkback2* pEEExtend;
		StandardTalkback2* pEEClose;
		StandardTalkback2* pEEOpen;
		StandardTalkback2* pEERigid;
		StandardTalkback2* pEEDerigid;
		StandardTalkback2* pShoulderBraceTb;
		StandardTalkback2* pSoftStopTB;

		RotaryDemuxSwitch* pLEDParameter;
		RotaryDemuxSwitch* pLEDJoint;
		RotaryDemuxSwitch* pRMSMode;

		StandardLight* pModeLights[12];
		StandardLight* pCWLights[12];
		StandardLight* pSequenceLights[2];

		_7SegDisp_RMS* pRMS;

	public:
		PanelA8(Atlantis* _sts);
		virtual ~PanelA8();

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
	};
};

#endif // __VC_PANELA8_H