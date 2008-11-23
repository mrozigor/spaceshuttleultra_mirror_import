#include "PushButtonIndicator.h"

namespace vc
{
	PushButtonIndicator::PushButtonIndicator(Atlantis* _sts, const string& _ident)
		: StandardLight(_sts, _ident)
	{
		anim_pb = NULL;
		pPushDown = NULL;
		uiGroup = 0xFFFF;
	}

	PushButtonIndicator::~PushButtonIndicator()
	{
	}

	/*void PushButtonIndicator::DefineGroup(UINT _grpIndex) {
		uiGroup = _grpIndex;
	}

	void PushButtonIndicator::DefineVCAnimations(UINT vc_idx) {
		char pszBuffer[256];
		if(bHasDirection && !bHasAnimations) {

			/*sprintf_s(pszBuffer, 255, "PUSH BUTTON[%s]:\tDefine VC Animations()", 
				GetQualifiedIdentifier().c_str());
			oapiWriteLog(pszBuffer);

			anim_pb = STS()->CreateAnimation(InitialAnimState());

			pPushDown = new MGROUP_TRANSLATE(vc_idx, &uiGroup, 1, 
				GetDirection());
			STS()->AddAnimationComponent(anim_pb, 0.0, 1.0, pPushDown);

			VerifyAnimations();
		}
	}*/

	bool PushButtonIndicator::OnMouseEvent(int _event, float x, float y) {
		switch(_event) {
			case PANEL_MOUSE_LBDOWN:
				OnPress();
				break;
			case PANEL_MOUSE_LBUP:
				OnRelease();
				break;
			default:
				return false;
		}
		return true;
	}

	void PushButtonIndicator::OnPress()
	{
		if(bHasAnimations)
		{
			SetAnimation(anim_pb, 1.0);
		}
		output.SetLine();
	}

	void PushButtonIndicator::OnRelease()
	{
		if(bHasAnimations)
		{
			SetAnimation(anim_pb, 0.0);
		}
	}
};