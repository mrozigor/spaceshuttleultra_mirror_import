/****************************************************************************
  This file is part of Space Shuttle Ultra

  Pushbutton VC Component definition



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

#include "BasicVCComponent.h"
#include "../discsignals/DiscOutPort.h"

namespace vc {

	using class discsignals::DiscOutPort;

	class PushButton: public BasicVCComponent {
		UINT anim_pb;
		UINT uiGroup;
		MGROUP_TRANSLATE* pPushDown;
	public:
		PushButton(Atlantis* p, const string& name);
		virtual ~PushButton();
		void DefineGroup(UINT _grpIndex);
		virtual void DefineVCAnimations(UINT vc_idx);
		virtual bool OnMouseEvent(int _event, float x, float y);
		virtual void Realize();
		virtual void OnPress();
		virtual void OnDepress();

		DiscOutPort output;
	};
};