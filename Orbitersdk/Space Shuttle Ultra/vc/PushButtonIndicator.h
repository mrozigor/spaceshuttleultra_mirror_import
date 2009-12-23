/****************************************************************************
  This file is part of Space Shuttle Ultra

  Pushbutton/indicator VC component definition



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
#ifndef __PUSHBUTTONINDICATOR_H
#define __PUSHBUTTONINDICATOR_H
#pragma once

#include "../Atlantis.h"
#include "StandardLight.h"
#include "../discsignals/DiscInPort.h"
#include "../discsignals/DiscOutPort.h"

namespace vc
{
	using class discsignals::DiscOutPort;
	using class discsignals::DiscInPort;

	class PushButtonIndicator: public StandardLight
	{
		UINT anim_pb;
		UINT uiGroup;
		MGROUP_TRANSLATE* pPushDown;
		bool bAllowReset;
	public:
		PushButtonIndicator(Atlantis* _sts, const string& _ident);
		~PushButtonIndicator();

		//void DefineGroup(UINT _grpIndex);
		//virtual void DefineVCAnimations(UINT vc_idx);
		virtual bool OnMouseEvent(int _event, float x, float y);
		virtual void OnPress();
		virtual void OnRelease();

		void ConnectAll(DiscreteBundle* pBundle, int line);
		/**
		 * If true, pressing PBI will set output line to opposite of input line (if input line is set, output line will be reset)
		 * If false, output line will be set every time PBI is pushed
		 */
		void AllowReset(bool allow);

		DiscOutPort output; //generally same as input port
	};

	/**
	 * PBIs are normally in groups where only 1 DiscPort in the group is set
	 * Monitors group of DiscPorts
	 * If one DiscPort is set, resets all other ports in group
	 */
	class PBIDiscPortGroup
	{
		const static int MAX_SIZE = 16;
		DiscOutPort outPorts[MAX_SIZE];
		DiscInPort inPorts[MAX_SIZE];

		bool oldValues[MAX_SIZE];

		unsigned short usCount;
	public:
		PBIDiscPortGroup();
		~PBIDiscPortGroup();

		/**
		 * Add all ports from usStart to usEnd (inclusive) to group
		 */
		void AddPorts(DiscreteBundle* pBundle, unsigned short usStart, unsigned short usEnd);

		void OnPreStep();
	private:
		void SetLine(unsigned short usIndex);
	};
};

#endif //__PUSHBUTTONINDICATOR_H
