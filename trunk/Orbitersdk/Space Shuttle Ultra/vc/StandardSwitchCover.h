/****************************************************************************
  This file is part of Space Shuttle Ultra

  Standard Switch cover VC component



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
#ifndef __STANDARDSWITCHCOVER_H
#define __STANDARDSWITCHCOVER_H
#pragma once

#include "BasicSwitch.h"

namespace vc
{
	/**
	 * Class for switch cover.
	 * Cover component must be added to panel BEFORE switch it covers.
	 */
	class StandardSwitchCover : public BasicSwitch
	{
		UINT grpIndex;
		MGROUP_ROTATE* pcoverrot;
		UINT anim_cover;

		VCMOUSEEVENTPOINT mouse_a[2], mouse_d[2];
	public:
		StandardSwitchCover(Atlantis* _sts, const std::string& _ident);
		virtual ~StandardSwitchCover();

		virtual void DefineVCAnimations(UINT vc_idx);
		virtual bool OnMouseEvent(int _event, float x, float y);
		virtual void OnPositionChange(unsigned short usNewPosition);

		void DefineCoverGroup(UINT _grpIndex);
		void SetMouseRegion(unsigned short position, float xmin, float ymin, float xmax, float ymax);
	};
};

#endif // __STANDARDSWITCHCOVER_H