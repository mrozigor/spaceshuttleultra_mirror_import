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