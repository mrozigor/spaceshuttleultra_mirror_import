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