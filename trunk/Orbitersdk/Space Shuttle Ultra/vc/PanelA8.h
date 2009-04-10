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

		DiscInPort RMSJointAngles[6];
		DiscInPort LED_JointSelect[8], LED_ParameterSelect[8];

		double LEDValues[3];
	};
};

#endif // __VC_PANELA8_H