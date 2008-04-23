// BasicSwitch.cpp: Implementierung der Klasse BasicSwitch.
//
//////////////////////////////////////////////////////////////////////

#include "BasicSwitch.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace vc {

BasicSwitch::BasicSwitch(Atlantis* _sts)
: BasicVCComponent(_sts)
{

}

BasicSwitch::~BasicSwitch()
{

}

bool BasicSwitch::OnMouseEvent(int _event, float x, float y)
{
	return false;
}


};