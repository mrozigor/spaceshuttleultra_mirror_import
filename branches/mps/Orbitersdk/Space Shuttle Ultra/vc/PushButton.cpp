#include "../Atlantis.h"
#include "PushButton.h"

namespace vc {

	PushButton::PushButton(Atlantis* _sts, const string& _ident) 
		: AtlantisVCComponent(_sts, _ident), output()
	{
		anim_pb = NULL;
		pPushDown = NULL;
		uiGroup = 0xFFFF;
	}


	PushButton::~PushButton() {
	}

	void PushButton::DefineGroup(UINT _grpIndex) {
		uiGroup = _grpIndex;
	}

	void PushButton::DefineVCAnimations(UINT vc_idx) {
		char pszBuffer[256];
		if(bHasDirection && !bHasAnimations) {

			sprintf_s(pszBuffer, 255, "PUSH BUTTON[%s]:\tDefine VC Animations()", 
				GetQualifiedIdentifier().c_str());
			oapiWriteLog(pszBuffer);

			anim_pb = STS()->CreateAnimation(InitialAnimState());

			pPushDown = new MGROUP_TRANSLATE(vc_idx, &uiGroup, 1, 
				GetDirection());
			STS()->AddAnimationComponent(anim_pb, 0.0, 1.0, pPushDown);

			VerifyAnimations();
		}
	}

	bool PushButton::OnMouseEvent(int _event, float x, float y) {
		/*switch(_event) {
			case PANEL_MOUSE_LBDOWN:
			case PANEL_MOUSE_LBPRESSED:
				OnPress();
				break;
			case PANEL_MOUSE_LBUP:
				OnDepress();
				break;
			default:
				return false;
		}
		return true;*/
		if(_event & PANEL_MOUSE_LBDOWN) {
			OnPress();
			return true;
		}
		else if(_event & PANEL_MOUSE_LBUP) {
			OnDepress();
			return true;
		}
		return false;

	}

	void PushButton::Realize() {
	}

	void PushButton::OnPress() {
		if(bHasAnimations)
		{
			SetAnimation(anim_pb, 1.0);
		}
		output.SetLine();
	}

	void PushButton::OnDepress() {
		if(bHasAnimations)
		{
			SetAnimation(anim_pb, 0.0);
		}
		output.ResetLine();
	}

};