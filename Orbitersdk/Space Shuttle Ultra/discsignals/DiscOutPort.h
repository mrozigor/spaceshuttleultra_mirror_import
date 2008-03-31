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
	void SetLine();
	void ResetLine();
	DiscOutPort();
	virtual ~DiscOutPort();
};

};

#endif // !defined(AFX_DISCOUTPORT_H__B01F0E2B_BD2E_4A08_B236_7DCF6C1507CD__INCLUDED_)
