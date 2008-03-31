// DiscretePort.cpp: Implementierung der Klasse DiscretePort.
//
//////////////////////////////////////////////////////////////////////

#include "DiscretePort.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace discsignals
{

DiscretePort::DiscretePort()
{

}

DiscretePort::~DiscretePort()
{

}

bool DiscretePort::Connect(DiscreteBundle *pBundle, int iLine)
{
	this->pBundle = pBundle;
	this->usLine = iLine;
	return true;
}

};