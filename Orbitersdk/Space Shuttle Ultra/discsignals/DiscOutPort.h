// DiscOutPort.h: Schnittstelle für die Klasse DiscOutPort.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISCOUTPORT_H__B01F0E2B_BD2E_4A08_B236_7DCF6C1507CD__INCLUDED_)
#define AFX_DISCOUTPORT_H__B01F0E2B_BD2E_4A08_B236_7DCF6C1507CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DiscretePort.h"

namespace discsignals
{

class DiscOutPort : public DiscretePort  
{
public:
	DiscOutPort();
	virtual ~DiscOutPort();
	void Propagate(double fSimT, double fDeltaT, double fMJD);
	virtual void ResetLine();
	void SetLine();
	void SetLine(float fVoltage);
	
};

class DiscMultiplex {
public:
	DiscMultiplex();
	virtual ~DiscMultiplex();
	virtual void Propagate(double fSimT, double fDeltaT, double fMJD);
};

class DiscMultiplex16: public DiscMultiplex {
protected:
	unsigned short usLatch;
public:
	DiscOutPort r[16];
public:
	DiscMultiplex16();
	virtual ~DiscMultiplex16();
	void Set(unsigned short usNewLatch);
	virtual void Propagate(double fSimT, double fDeltaT, double fMJD);
};

};

#endif // !defined(AFX_DISCOUTPORT_H__B01F0E2B_BD2E_4A08_B236_7DCF6C1507CD__INCLUDED_)
