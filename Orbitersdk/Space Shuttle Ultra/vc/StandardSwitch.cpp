// StandardSwitch.cpp: Implementierung der Klasse StandardSwitch.
//
//////////////////////////////////////////////////////////////////////

#include "StandardSwitch.h"
#include "../Atlantis.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace vc {
	LockableLever::LockableLever(Atlantis *psts, unsigned short usNumPositions, const std::string &_ident)
		: BasicSwitch(psts, usNumPositions, _ident), bIsPulled(false), bHasPullDir(false)
	{
		pswitchrot = NULL;
		pswitchpull = NULL;
		anim_switch = NULL;
	}

	LockableLever::~LockableLever()
	{
		if(pswitchrot) delete pswitchrot;
		if(pswitchpull) delete pswitchpull;
	}

	void LockableLever::DefineSwitchGroup(UINT _grpIndex)
	{
		grpIndex=_grpIndex;
	}

	/*void LockableLever::SetReference(const VECTOR3& _ref, const VECTOR3& _dir, const VECTOR3& _pullDir)
	{
		BasicSwitch::SetReference(_ref, _dir);
		SetPullDirection(_pullDir);
	}*/

	void LockableLever::SetPullDirection(VECTOR3 _pullDir)
	{
		pullDir=_pullDir;
		bHasPullDir = true;
	}

	void LockableLever::DefineVCAnimations(UINT vc_idx)
	{
		char pszBuffer[256];
		//oapiWriteLog("*");
		if(bHasReference && bHasPullDir && bHasDirection && !bHasAnimations)
		{
			sprintf_s(pszBuffer, 255, "Lockable Lever[%s]:\tDefine VC Animations()", 
				GetQualifiedIdentifier().c_str());
			oapiWriteLog(pszBuffer);
			//VECTOR3 ofs = STS()->GetOrbiterCoGOffset();
			
			pswitchrot = new MGROUP_ROTATE(LOCALVERTEXLIST, MAKEGROUPARRAY(&dummy_vec), 1, GetReference() /*+ ofs*/, GetDirection(), (float)(90 * RAD));
			anim_switch = STS()->CreateAnimation(InitialAnimState());
			ANIMATIONCOMPONENT_HANDLE parent = STS()->AddAnimationComponent(anim_switch, 0.0, 1.0, pswitchrot, NULL);

			pswitchpull = new MGROUP_TRANSLATE(vc_idx, &grpIndex, 1, pullDir*0.005);
			anim_pull = STS()->CreateAnimation(0.0);
			STS()->AddAnimationComponent(anim_pull, 0.0, 1.0, pswitchpull, parent);

			VerifyAnimations();
		}

		OnPositionChange(usCurrentPosition);
	}

	bool LockableLever::OnMouseEvent(int _event, float x, float y)
	{
		if((_event & PANEL_MOUSE_LBUP) && bIsPulled) { // switch released
			bIsPulled=false;
			OnRelease();

			return true;
		}
		else if((_event & PANEL_MOUSE_LBPRESSED) && bIsPulled) { // move switch position
			if(bOrientation) { // horizontal
				if((x-fInitialMousePos) > 0.2) {
					OnPositionDown();
					fInitialMousePos = x;
				}
				else if((x-fInitialMousePos) < -0.2) {
					OnPositionUp();
					fInitialMousePos = x;
				}
				if(x>0.99 || x<0.01) { // if mouse is about to go out-of-bounds, release switch
					bIsPulled=false;
					OnRelease();
				}
			}
			else { //vertical
				if((y-fInitialMousePos) > 0.2) {
					OnPositionDown();
					fInitialMousePos = y;
				}
				else if((y-fInitialMousePos) < -0.2) {
					OnPositionUp();
					fInitialMousePos = y;
				}
				if(y>0.99 || y<0.01) { // if mouse is about to go out-of-bounds, release switch
					bIsPulled=false;
					OnRelease();
				}
			}

			return true;
		}
		else if((_event & PANEL_MOUSE_LBDOWN) && !bIsPulled) { // switch pulled
			bIsPulled=true;
			if(bOrientation) { // horizontal
				fInitialMousePos=x;
			}
			else { // vertical
				fInitialMousePos=y;
			}
			OnPull();

			return true;
		}
		return false;
	}

	void LockableLever::OnPositionChange(unsigned short usNewPosition)
	{
		if(bHasAnimations) {
			SetAnimation(anim_switch, static_cast<double>(usNewPosition)/(usNumPositions - 1));
			//sprintf_s(oapiDebugString(), 255, "%s: Setting position %f", GetIdentifier().c_str(), static_cast<double>(usNewPosition)/(usNumPositions - 1));
		}
	}

	void LockableLever::OnPull()
	{
		if(bHasAnimations) SetAnimation(anim_pull, 1.0);
	}

	void LockableLever::OnRelease()
	{
		if(bHasAnimations) SetAnimation(anim_pull, 0.0);
	}

	void LockableLever::Realize()
	{
		// set animations and DiscPorts
		OnPositionChange(usCurrentPosition);
	}

	bool LockableLever::ConnectPort(unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine)
	{
		if(usPort==0) return input.Connect(pBundle, usLine);
		return false;
	}

	LockableLever2::LockableLever2(Atlantis* psts, const std::string& _ident)
		: LockableLever(psts, 2, _ident)
	{
	}

	LockableLever2::~LockableLever2()
	{
	}

	void LockableLever2::OnPositionChange(unsigned short usNewPosition)
	{
		LockableLever::OnPositionChange(usNewPosition);
		switch(usNewPosition) {
			case 0:
				output.ResetLine();
				break;
			case 1:
				output.SetLine();
				break;
		}
	}

	bool LockableLever2::ConnectPort(unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine)
	{
		if(usPort==0) return LockableLever::ConnectPort(0, pBundle, usLine);
		else if(usPort==1) return output.Connect(pBundle, usLine);
		else return false;
	}

	LockableLever3::LockableLever3(Atlantis* psts, const std::string& _ident)
		: LockableLever(psts, 3, _ident)
	{
		usCurrentPosition = 1; // true for most switch

		usA=0;
		usB=2;
	}

	LockableLever3::~LockableLever3()
	{
	}

	void LockableLever3::OnPositionChange(unsigned short usNewPosition)
	{
		LockableLever::OnPositionChange(usNewPosition);
		/*switch(usNewPosition) {
		case 0:
			outputA.SetLine();
			outputB.ResetLine();
			break;
		case 1:
			outputA.ResetLine();
			outputB.SetLine();
			break;
		default:
			outputA.ResetLine();
			outputB.ResetLine();
			break;
		}*/

		if(usNewPosition == usA) {
			outputA.SetLine();
			outputB.ResetLine();
		}
		else if(usNewPosition == usB) {
			outputA.ResetLine();
			outputB.SetLine();
		}
		else {
			outputA.ResetLine();
			outputB.ResetLine();
		}
	}

	bool LockableLever3::ConnectPort(unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine)
	{
		switch(usPort)
		{
		case 0:
			return LockableLever::ConnectPort(0, pBundle, usLine);
		case 1:
			return outputA.Connect(pBundle, usLine);
		case 2:
			return outputB.Connect(pBundle, usLine);
		default:
			return false;
		}
	}

	bool LockableLever3::ConnectSwitchPosition(unsigned short usPos, unsigned short usPort)
	{
		switch(usPort) {
			case 1:
				usA=usPos;
				return true;
			case 2:
				usB=usPos;
				return true;
			default:
				return false;
		}
	}

StandardSwitch::StandardSwitch(Atlantis* _sts, unsigned short usNumPositions, const string& _ident)
: BasicSwitch(_sts, usNumPositions, _ident)
{
	pswitchrot = NULL;
	anim_switch = NULL;
}

StandardSwitch::~StandardSwitch()
{
	if(pswitchrot)
		delete pswitchrot;
}


void StandardSwitch::Realize()
{
	//Don't add anything for Orbiter, use existing meshes

	// set animations and DiscPorts
	OnPositionChange(usCurrentPosition);
}

void StandardSwitch::DefineVCAnimations(UINT vc_idx)
{
	char pszBuffer[256];
	//oapiWriteLog("*");
	if(bHasReference && bHasDirection && !bHasAnimations)
	{
		sprintf_s(pszBuffer, 255, "STANDARD SWITCH[%s]:\tDefine VC Animations()", 
			GetQualifiedIdentifier().c_str());
		oapiWriteLog(pszBuffer);
		//VECTOR3 ofs = STS()->GetOrbiterCoGOffset();
		pswitchrot = new MGROUP_ROTATE(vc_idx, &grpIndex, 1, GetReference() /*+ ofs*/, GetDirection(), (float)(90 * RAD));
		anim_switch = STS()->CreateAnimation(InitialAnimState());
		STS()->AddAnimationComponent(anim_switch, 0.0, 1.0, pswitchrot, NULL);
		VerifyAnimations();
	}

	OnPositionChange(usCurrentPosition);
}

bool StandardSwitch::ConnectPort(unsigned short usPort, discsignals::DiscreteBundle *pBundle, unsigned short usLine)
{
	if(usPort == 0)
	{
		return input.Connect(pBundle, usLine);
	}
	return false;
}

void StandardSwitch::DefineSwitchGroup(UINT _grpIndex)
{
	grpIndex = _grpIndex;
}


void StandardSwitch::OnPositionChange(unsigned short usNewPosition)
{
	if(bHasAnimations)
	{
		SetAnimation(anim_switch, (double)(usNewPosition)/(usNumPositions - 1));
	}
}


StdSwitch2::StdSwitch2(Atlantis *_sts, const std::string &_ident)
: StandardSwitch(_sts, 2, _ident), usOnPosition(1)
{
}

StdSwitch2::~StdSwitch2()
{
}

bool StdSwitch2::ConnectPort(unsigned short usPort, discsignals::DiscreteBundle *pBundle, unsigned short usLine)
{
	if(usPort == 1)
	{
		return output.Connect(pBundle, usLine);	
	} else {
		return StandardSwitch::ConnectPort(usPort, pBundle, usLine);
	}
}

void StdSwitch2::OnPositionChange(unsigned short usNewPosition)
{
	StandardSwitch::OnPositionChange(usNewPosition);
	/*switch(usNewPosition) {
	case 0:
		output.ResetLine();
		break;
	case 1:
		output.SetLine();
		break;
	}*/
	if(usNewPosition == usOnPosition) output.SetLine();
	else output.ResetLine();
}

void StdSwitch2::SetOnPosition(unsigned short _usPos)
{
	usOnPosition=_usPos;
}

StdSwitch3::StdSwitch3(Atlantis *_sts, const std::string &_ident)
: StandardSwitch(_sts, 3, _ident)
{
	usCurrentPosition = 1; // true for most meshes

	usA=0;
	usB=2;
}

StdSwitch3::~StdSwitch3()
{
}

bool StdSwitch3::ConnectPort(unsigned short usPort, discsignals::DiscreteBundle *pBundle, unsigned short usLine)
{
	if(usPort == 1)
	{
		return outputA.Connect(pBundle, usLine);	
	} else if (usPort == 2) {
		return outputB.Connect(pBundle, usLine);
	} else {
		return StandardSwitch::ConnectPort(usPort, pBundle, usLine);
	}
}

bool StdSwitch3::ConnectSwitchPosition(unsigned short usPos, unsigned short usPort)
{
	switch(usPort) {
	case 1:
		usA=usPos;
		return true;
	case 2:
		usB=usPos;
		return true;
	default:
		return false;
	}
}

void StdSwitch3::OnPositionChange(unsigned short usNewPosition)
{
	StandardSwitch::OnPositionChange(usNewPosition);

	if(usNewPosition == usA) {
		outputA.SetLine();
		outputB.ResetLine();
	}
	else if(usNewPosition == usB) {
		outputA.ResetLine();
		outputB.SetLine();
	}
	else {
		outputA.ResetLine();
		outputB.ResetLine();
	}
}

};
