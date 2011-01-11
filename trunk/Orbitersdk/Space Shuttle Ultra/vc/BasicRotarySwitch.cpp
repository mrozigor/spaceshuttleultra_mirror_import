#include "BasicRotarySwitch.h"
#include <UltraMath.h>
#include "../Atlantis.h"

namespace vc
{
	BasicRotarySwitch::BasicRotarySwitch(Atlantis *_sts, unsigned short _usNumPositions, const std::string &_ident)
		: BasicVCComponent(_sts, _ident), labels(_usNumPositions, ""), usNumPositions(_usNumPositions), usCurrentPosition(_usNumPositions), 
		rotAngle(0.0), rotOffset(0.0), allowWraparound(false)
	{
		pSwitchRot = NULL;
	}

	BasicRotarySwitch::~BasicRotarySwitch()
	{
		delete pSwitchRot;
	}

	void BasicRotarySwitch::Realize()
	{
		if(usCurrentPosition==usNumPositions) usCurrentPosition=0;
		OnPositionChange(usCurrentPosition);
	}

	const string& BasicRotarySwitch::GetLabel(int iPosition) const
	{
		return labels.at(iPosition);
	}

	void BasicRotarySwitch::SetLabel(int iPosition, const string& _label)
	{
		labels.at(iPosition)=_label;
	}

	void BasicRotarySwitch::DefineSwitchGroup(UINT _grpIndex)
	{
		grpIndex = _grpIndex;
	}

	void BasicRotarySwitch::DefineRotationAngle(float _rotAngle)
	{
		rotAngle = _rotAngle;
	}

	void BasicRotarySwitch::SetOffset(float fOffset)
	{
		rotOffset=fOffset; // get angle from horizontal
	}

	void BasicRotarySwitch::SetInitialAnimState(double fState)
	{
		BasicVCComponent::SetInitialAnimState(fState);
		if(usCurrentPosition==usNumPositions) 
			usCurrentPosition = static_cast<unsigned short>(fState*(usNumPositions-1));
	}

	void BasicRotarySwitch::SetWraparound(bool _allowWraparound)
	{
		allowWraparound=_allowWraparound;
	}

	void BasicRotarySwitch::DefineVCAnimations(UINT vc_idx)
	{
		char pszBuffer[256];
		if(bHasReference && bHasDirection && !bHasAnimations && !pSwitchRot)
		{
			sprintf_s(pszBuffer, 255, "BASIC ROTARY SWITCH[%s]:\tDefine VC Animations()", 
				GetQualifiedIdentifier().c_str());
			oapiWriteLog(pszBuffer);
			//VECTOR3 ofs = STS()->GetOrbiterCoGOffset();
			pSwitchRot = new MGROUP_ROTATE(vc_idx, &grpIndex, 1, GetReference() /*+ ofs*/, GetDirection(), (float)(rotAngle*RAD));
			anim_switch = STS()->CreateAnimation(InitialAnimState());
			STS()->AddAnimationComponent(anim_switch, 0.0, 1.0, pSwitchRot);
			VerifyAnimations();
		}

		//OnPositionChange(usCurrentPosition);
	}

	bool BasicRotarySwitch::GetStateString(unsigned long ulBufferSize, char* pszBuffer)
	{
		if(labels.at(usCurrentPosition).compare("")) {
			sprintf_s(pszBuffer, ulBufferSize, "%s", 
				labels.at(usCurrentPosition).c_str());
		} else {
			sprintf_s(pszBuffer, ulBufferSize, "[%d]", 
				usCurrentPosition);
		}
		return true;
	}

	bool BasicRotarySwitch::OnParseLine(const char *line)
	{
		if(line[0] == '[') {
			usCurrentPosition = atoi(line+1);
			//OnPositionChange(usCurrentPosition);
			return true;
		} else {
			for(unsigned short i = 0; i<usNumPositions; i++) {
				if(labels.at(i) == line) 
				{
					usCurrentPosition = i;
					//OnPositionChange(usCurrentPosition);
					return true;
				}
			}
		}
		return false;
	}

	bool BasicRotarySwitch::OnMouseEvent(int _event, float x, float y)
	{
		if(!(_event & PANEL_MOUSE_LBDOWN)) return false;

		float fPosition=static_cast<float>(usCurrentPosition);
		float fNumPositions=static_cast<float>(usNumPositions-1);
		float fAngle=rotOffset+fPosition*(rotAngle/fNumPositions); // angle between x-axis and switch label

		VECTOR3 curRot=RotateVectorZ(_V(0.0, 1.0, 0.0), fAngle);
		VECTOR3 cross_product=crossp(curRot, _V(x-0.5, y-0.5, 0.0));
		sprintf_s(oapiDebugString(), 255, "Angle: %f cp: %f", fAngle, cross_product.z);

		if(cross_product.z>0.05) {
			OnRotateLeft();
			return true;
		}
		else if(cross_product.z<-0.05) {
			OnRotateRight();
			return true;
		}

		return false;
	}

	void BasicRotarySwitch::OnRotateLeft()
	{
		if(usCurrentPosition>0) {
			usCurrentPosition--;
			OnPositionChange(usCurrentPosition);
		}
		else if(allowWraparound && usCurrentPosition==0) {
			usCurrentPosition=usNumPositions-1;
			OnPositionChange(usCurrentPosition);
		}
	}

	void BasicRotarySwitch::OnRotateRight()
	{
		if(usCurrentPosition < usNumPositions-1) {
			usCurrentPosition++;
			OnPositionChange(usCurrentPosition);
		}
		else if(allowWraparound && usCurrentPosition==usNumPositions-1) {
			usCurrentPosition=0;
			OnPositionChange(usCurrentPosition);
		}
	}

	void BasicRotarySwitch::OnPositionChange(unsigned short usNewPosition)
	{
		if(bHasAnimations)
		{
			if(usNewPosition>=0 && usNewPosition<usNumPositions) {
				SetAnimation(anim_switch, (double)(usNewPosition)/(usNumPositions - 1));
				//sprintf_s(oapiDebugString(), 255, "%s: animation %f %f", GetIdentifier().c_str(), rotAngle, (double)(usNewPosition)/(usNumPositions - 1));
			}
			//else sprintf_s(oapiDebugString(), 255, "%s: invalid switch position %d", GetIdentifier().c_str(), static_cast<int>(usNewPosition));
		}
	}
};
