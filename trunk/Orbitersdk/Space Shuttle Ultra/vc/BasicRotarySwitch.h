/****************************************************************************
  This file is part of Space Shuttle Ultra

  Basic rotary switch vc component definition



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
		virtual void Realize();

		virtual void OnRotateLeft();
		virtual void OnRotateRight();
		virtual void OnPositionChange(unsigned short usNewPosition);
	};
};

#endif //__VC_BASICROTARY_SWITCH_H