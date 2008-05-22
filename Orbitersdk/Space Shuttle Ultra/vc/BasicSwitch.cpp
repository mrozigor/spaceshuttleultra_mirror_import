// BasicSwitch.cpp: Implementierung der Klasse BasicSwitch.
//
//////////////////////////////////////////////////////////////////////

#include "BasicSwitch.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace vc {

BasicSwitch::BasicSwitch(Atlantis* _sts, unsigned short usNumPositions, const string& _ident)
: BasicVCComponent(_sts, _ident), bOrientation(false)
{
	this->usNumPositions = usNumPositions;
}

BasicSwitch::~BasicSwitch()
{

}
void BasicSwitch::DefineVCAnimations(UINT vc_idx)
{
	
}

bool BasicSwitch::OnMouseEvent(int _event, float x, float y)
{
	/*
	sprintf_s(oapiDebugString(), 255, "BASICSWITCH %s [%d/%d] (%d, %f, %f)",
		GetQualifiedIdentifier().c_str(), 
		usCurrentPosition,
		usNumPositions,
		_event, x, y);

	*/

	if(_event != PANEL_MOUSE_LBDOWN)
		return false;

	if(bOrientation) {
		if(x > 0.6) {
			OnPositionDown();
			return true;
		}
		else if(x < 0.4) {
			OnPositionUp();
			return true;
		}
	}
	else {
		if(y > 0.6) {
			OnPositionDown();
			return true;
		}
		else if( y < 0.4) {
			OnPositionUp();
			return true;
		}
	}
	return false;
}

void BasicSwitch::OnPositionChange(unsigned short usNewPosition)
{
}

void BasicSwitch::OnPositionUp()
{
	if(usCurrentPosition < usNumPositions - 1)
	{
		usCurrentPosition++;
		OnPositionChange(usCurrentPosition);
	}
}

void BasicSwitch::OnPositionDown()
{
	if(usCurrentPosition > 0)
	{
		usCurrentPosition--;
		OnPositionChange(usCurrentPosition);
	}
}



void BasicSwitch::OnSaveState(FILEHANDLE scn) const
{
	
}

};