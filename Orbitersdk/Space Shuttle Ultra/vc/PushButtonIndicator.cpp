#include "PushButtonIndicator.h"
#include <OrbiterSoundSDK40.h>

namespace vc
{
	PushButtonIndicator::PushButtonIndicator(Atlantis* _sts, const string& _ident, bool _saveState)
		: StandardLight(_sts, _ident), bSaveState(_saveState), bInitialState(false)
	{
		anim_pb = NULL;
		pPushDown = NULL;
		uiGroup = 0xFFFF;
		bAllowReset=false;
		bMomentary = false;
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

	void PushButtonIndicator::SetMomentary( bool momentary )
	{
		bMomentary = momentary;
	}

	bool PushButtonIndicator::OnMouseEvent(int _event, float x, float y) {
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
		if(bAllowReset && input && !bMomentary) output.ResetLine();
		else output.SetLine();
		PlayVesselWave( STS()->GetSoundID(), KEY_PRESS_SOUND );
	}

	void PushButtonIndicator::OnRelease()
	{
		if(bHasAnimations)
		{
			SetAnimation(anim_pb, 0.0);
		}
		if (bMomentary) output.ResetLine();
	}

	void PushButtonIndicator::Realize()
	{
		if(bSaveState) {
			if(bInitialState) output.SetLine();
			else output.ResetLine();
		}
	}

	bool PushButtonIndicator::OnParseLine(const char* line)
	{
		if(bSaveState) {
			int result = atoi(line);
			if(result == 0) bInitialState = false;
			else bInitialState = true;
			return true;
		}
		return false;
	}

	bool PushButtonIndicator::GetStateString(unsigned long ulBufferSize, char* pszBuffer)
	{
		if(!bSaveState) return false; // no need to save state
		
		if(input) strcpy_s(pszBuffer, ulBufferSize, "1");
		else strcpy_s(pszBuffer, ulBufferSize, "0");
		return true;
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
		for(unsigned short i=usStart;(i<=usEnd && index<MAX_SIZE);i++, index++)
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
