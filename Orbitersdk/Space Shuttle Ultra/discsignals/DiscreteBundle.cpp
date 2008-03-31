// DiscreteBundle.cpp: Implementierung der Klasse DiscreteBundle.
//
//////////////////////////////////////////////////////////////////////

#include "DiscreteBundle.h"
#include <cassert>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace discsignals
{

DiscreteBundle::DiscreteBundle(int nLines)
{
	assert(nLines <= 16);
	snLines = nLines;
}

DiscreteBundle::~DiscreteBundle()
{

}

void DiscreteBundle::SetDiscrete(int iIndex, float fVoltage)
{
	if(iIndex < snLines)
	{
		fLines[iIndex] = fVoltage;
	}
}

float DiscreteBundle::GetVoltage(int iIndex)
{
	if(iIndex < snLines)
	{
		return fLines[iIndex];
	}
	else
	{
		return 0.0;
	}
}

void DiscreteBundle::SetLogMode(int iMode)
{

}

};