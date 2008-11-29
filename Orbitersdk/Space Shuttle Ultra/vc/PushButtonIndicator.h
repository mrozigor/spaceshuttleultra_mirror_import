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
		 * True if pressing PBI when output line is set will reset line
		 */
		void AllowReset(bool allow);

		DiscOutPort output; //generally same as input port
	};
};

#endif //__PUSHBUTTONINDICATOR_H
