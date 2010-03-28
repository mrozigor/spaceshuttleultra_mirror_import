// DiscreteBundle.cpp: Implementierung der Klasse DiscreteBundle.
//
//////////////////////////////////////////////////////////////////////

#include "DiscreteBundle.h"
#include <cassert>
#include <OrbiterAPI.h>

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace discsignals
{

DiscreteBundle::DiscreteBundle(const string& _ident, int nLines)
{
	if(nLines>16) {
		oapiWriteLog("ERROR: too many lines in bundle");
	}
	assert(nLines <= 16);
	snLines = nLines;
	ident = _ident;
}

DiscreteBundle::~DiscreteBundle()
{
	char pszBuffer[400];
	sprintf_s(pszBuffer, 400, "(DiscreteBundle::~DiscreteBundle) destructor called for %s:%d",
		ident.c_str(),
		snLines);
	oapiWriteLog(pszBuffer);
}

const string& DiscreteBundle::GetIdentity() const
{
	return ident;
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

unsigned int DiscreteBundle::CountLines() const throw() 
{
	return snLines;
}

};