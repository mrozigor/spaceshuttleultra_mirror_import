// BasicSwitch.cpp: Implementierung der Klasse BasicSwitch.
//
//////////////////////////////////////////////////////////////////////

#include "BasicSwitch.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace vc {

BasicSwitch::BasicSwitch(Atlantis* _sts, unsigned short usNumPositions, const string& _ident)
: BasicVCComponent(_sts, _ident)
{
	this->usNumPositions = usNumPositions;
}

BasicSwitch::~BasicSwitch()
{

}

bool BasicSwitch::OnMouseEvent(int _event, float x, float y)
{
	return false;
}


};