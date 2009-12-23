#include "PushButtonIndicator.h"

namespace vc
{
	PushButtonIndicator::PushButtonIndicator(Atlantis* _sts, const string& _ident)
		: StandardLight(_sts, _ident)
	{
		anim_pb = NULL;
		pPushDown = NULL;
		uiGroup = 0xFFFF;
		bAllowReset=false;
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

	void PushButtonIndicator::ConnectAll(DiscreteBundle *pBundle, int line)
	{
		input.Connect(pBundle, line);
		output.Connect(pBundle, line);
		test.Connect(pBundle, line);
	}

	void PushButtonIndicator::AllowReset(bool allow)
	{
		bAllowReset=allow;
	}

	bool PushButtonIndicator::OnMouseEvent(int _event, float x, float y) {
		/*switch(_event) {
			case PANEL_MOUSE_LBDOWN:
				OnPress();
				break;
			case PANEL_MOUSE_LBUP:
				OnRelease();
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
			OnRelease();
			return true;
		}
		return false;
	}

	void PushButtonIndicator::OnPress()
	{
		if(bHasAnimations)
		{
			SetAnimation(anim_pb, 1.0);
		}
		if(bAllowReset && input) output.ResetLine();
		else output.SetLine();
	}

	void PushButtonIndicator::OnRelease()
	{
		if(bHasAnimations)
		{
			SetAnimation(anim_pb, 0.0);
		}
	}

	PBIDiscPortGroup::PBIDiscPortGroup()
		: usCount(0)
	{
		for(int i=0;i<MAX_SIZE;i++) oldValues[i] = false;
	}

	PBIDiscPortGroup::~PBIDiscPortGroup()
	{
	}

	void PBIDiscPortGroup::AddPorts(DiscreteBundle* pBundle, unsigned short usStart, unsigned short usEnd)
	{
		if(usEnd < usStart) return;

		unsigned short index = usCount;
		for(unsigned short i=usStart;i<=usEnd, index<MAX_SIZE;i++, index++)
		{
			inPorts[index].Connect(pBundle, i);
			outPorts[index].Connect(pBundle, i);
		}
		usCount = (usEnd-usStart) + 1;
	}

	void PBIDiscPortGroup::OnPreStep()
	{
		for(unsigned short i=0;i<usCount;i++) {
			// loop through ports until we find one that has just been set
			if(!oldValues[i] && inPorts[i]) {
				// make sure that this line is the only line set
				SetLine(i);
				return;
			}
			//else oldValues[i] = inPorts[i];
		}
	}

	void PBIDiscPortGroup::SetLine(unsigned short usIndex)
	{
		if(usIndex>=usCount) return;

		for(unsigned short i=0;i<usCount;i++) {
			if(i!=usIndex) {
				outPorts[i].ResetLine();
				oldValues[i] = false;
			}
		}
		outPorts[usIndex].SetLine();
		oldValues[usIndex] = true;
	}
};
