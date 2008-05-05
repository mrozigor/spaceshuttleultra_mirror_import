// DiscretePort.h: Schnittstelle für die Klasse DiscretePort.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISCRETEPORT_H__18D00C82_9652_493D_8A1D_D1CFF223A0B8__INCLUDED_)
#define AFX_DISCRETEPORT_H__18D00C82_9652_493D_8A1D_D1CFF223A0B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace discsignals
{

class DiscreteBundle;

class DiscretePort  
{
protected:
	DiscreteBundle* pBundle;
	unsigned short usLine;
public:
	bool Connect(DiscreteBundle* pBundle, int iLine);
	bool Disconnect();
	DiscretePort();
	DiscretePort(DiscreteBundle* pBundle, int iLine);
	virtual ~DiscretePort();
};

};

#endif // !defined(AFX_DISCRETEPORT_H__18D00C82_9652_493D_8A1D_D1CFF223A0B8__INCLUDED_)
