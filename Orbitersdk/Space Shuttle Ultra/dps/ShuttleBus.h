// ShuttleBus.h: Schnittstelle für die Klasse ShuttleBus.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHUTTLEBUS_H__5E15D9D6_08AF_49BD_96FB_2835793F608E__INCLUDED_)
#define AFX_SHUTTLEBUS_H__5E15D9D6_08AF_49BD_96FB_2835793F608E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

namespace dps {

	using namespace std;

class ShuttleBus
{
public:
	ShuttleBus(const string& _ident);
	virtual ~ShuttleBus();

};

};

#endif // !defined(AFX_SHUTTLEBUS_H__5E15D9D6_08AF_49BD_96FB_2835793F608E__INCLUDED_)
