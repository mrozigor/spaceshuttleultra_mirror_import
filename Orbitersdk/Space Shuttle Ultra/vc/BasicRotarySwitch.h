#ifndef __VC_BASICROTARYSWITCH_H
#define __VC_BASICROTARYSWITCH_H
#pragma once

#include "BasicVCComponent.h"
#include <vector>
#include <string>

namespace vc
{
	class BasicRotarySwitch: public BasicVCComponent
	{
	protected:
		unsigned short usNumPositions;
		vector<string> labels;
		unsigned short usCurrentPosition;

		float rotAngle;
		MGROUP_ROTATE* pSwitchRot;
		UINT anim_switch;
		UINT grpIndex;
	public:
		BasicRotarySwitch(Atlantis* _sts, unsigned short _usNumPositions, const std::string &_ident);
		virtual ~BasicRotarySwitch();

		const string& GetLabel(int iPosition) const;
		void SetLabel(int iPosition, const string& _label);
		void DefineSwitchGroup(UINT _grpIndex);
		void DefineRotationAngle(float _rotAngle); // angle measured in degrees
		virtual void SetInitialAnimState(double fState = 0.0);
		virtual void DefineVCAnimations(UINT vc_idx);
		virtual bool GetStateString(unsigned long ulBufferSize, char* pszBuffer);
		virtual bool OnParseLine(const char* line);
		virtual bool OnMouseEvent(int _event, float x, float y);

		virtual void OnRotateLeft();
		virtual void OnRotateRight();
		virtual void OnPositionChange(unsigned short usNewPosition);
	};
};

#endif //__VC_BASICROTARY_SWITCH_H