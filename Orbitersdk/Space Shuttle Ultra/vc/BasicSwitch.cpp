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

bool BasicSwitch::OnMouseEvent(int _event, float x, float y)
{
	if(bOrientation) {
		if( x < 0.4) {
			OnPositionUp();
		}
		else if(x > 0.6) {
			OnPositionDown();
		}
	}
	else {
		if( y < 0.4) {
			OnPositionUp();
		}
		else if(y > 0.6) {
			OnPositionDown();
		}
	}
	return false;
}

void BasicSwitch::OnPositionUp()
{
}

void BasicSwitch::OnPositionDown()
{
}


};