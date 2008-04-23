// BasicSwitch.h: Schnittstelle für die Klasse BasicSwitch.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASICSWITCH_H__3790BCEE_A9B9_492A_A7F4_B0DA5E454AFB__INCLUDED_)
#define AFX_BASICSWITCH_H__3790BCEE_A9B9_492A_A7F4_B0DA5E454AFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasicVCComponent.h"
#include <vector>
#include <string>

namespace vc {

using namespace std;

class BasicSwitch : public BasicVCComponent  
{
protected:
	bool bOrientation;
	unsigned short usNumPositions;
	vector<string> labels;
public:
	BasicSwitch(Atlantis* _sts);
	virtual ~BasicSwitch();
	const string& GetLabel(int iPosition) const;

	virtual bool OnMouseEvent(int _event, float x, float y);
};

};

#endif // !defined(AFX_BASICSWITCH_H__3790BCEE_A9B9_492A_A7F4_B0DA5E454AFB__INCLUDED_)
