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

#include "AtlantisVCComponent.h"
#include <vector>
#include <string>

namespace vc
{
	class BasicRotarySwitch: public AtlantisVCComponent
	{
	protected:
		unsigned short usNumPositions;
		vector<string> labels;
		unsigned short usCurrentPosition;

		bool allowWraparound;

		float rotOffset; // angle in radians
		float rotAngle; // angle in radians
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
		/**
		 * Angles (in degreees) between 12-o'clock position and initial (usCurrentPosition=0) position
		 * Used for mouse click handling
		 */
		void SetOffset(float fOffset = 0.0);
		void SetWraparound(bool _allowWraparound);

		virtual void DefineVCAnimations(UINT vc_idx);
		virtual bool GetStateString(unsigned long ulBufferSize, char* pszBuffer);
		virtual bool OnParseLine(const char* line);
		virtual bool OnMouseEvent(int _event, float x, float y);
		virtual void Realize();

		virtual void OnRotateLeft();
		virtual void OnRotateRight();
		virtual void OnPositionChange(unsigned short usNewPosition);

		/**
		 * Sets initial position of switch
		 * Has to be called in DefineVC function
		 */
		void SetInitialPosition(unsigned short usInPos);
	};
};

#endif //__VC_BASICROTARY_SWITCH_H