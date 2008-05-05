// DiscretePort.cpp: Implementierung der Klasse DiscretePort.
//
//////////////////////////////////////////////////////////////////////

#include "DiscretePort.h"
#include <cstdio>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace discsignals
{

DiscretePort::DiscretePort()
{
	pBundle = NULL;
	usLine = 0;
}
DiscretePort::DiscretePort(DiscreteBundle* pBundle, int iLine)
{
	Connect(pBundle, iLine);
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

bool DiscretePort::Disconnect()
{
	pBundle = NULL;
	usLine = 0;
	return true;
}

};