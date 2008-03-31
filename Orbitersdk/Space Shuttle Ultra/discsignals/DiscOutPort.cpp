// DiscOutPort.cpp: Implementierung der Klasse DiscOutPort.
//
//////////////////////////////////////////////////////////////////////

#include "DiscOutPort.h"
#include "DiscreteBundle.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace discsignals {

DiscOutPort::DiscOutPort()
{

}

DiscOutPort::~DiscOutPort()
{

}

void DiscOutPort::SetLine()
{
	pBundle->SetDiscrete(usLine, 5.0f);
}

void DiscOutPort::ResetLine()
{
	pBundle->SetDiscrete(usLine, 0.0f);
}


};

