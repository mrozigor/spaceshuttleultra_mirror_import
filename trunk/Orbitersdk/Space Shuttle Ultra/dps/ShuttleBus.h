/****************************************************************************
  This file is part of Space Shuttle Ultra

  Shuttle Bus class definition



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/
#if !defined(AFX_SHUTTLEBUS_H__5E15D9D6_08AF_49BD_96FB_2835793F608E__INCLUDED_)
#define AFX_SHUTTLEBUS_H__5E15D9D6_08AF_49BD_96FB_2835793F608E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>
#include <set>
#include "dps_defs.h"

namespace dps {

	using namespace std;

	class ShuttleBus
	{
		string ident;
		set<BIU*> connections;
	public:
		ShuttleBus(const string& _ident);
		virtual ~ShuttleBus();
		virtual const string& GetIdent() const;
		virtual void ConnectTo(BIU* pTarget);
		virtual long Transmit(unsigned short usTarget, unsigned short usNumData, unsigned short usData[]);
	};

	class ShuttleBusManager {
		vector<ShuttleBus*> busses;
	public:
		ShuttleBusManager();
		virtual ~ShuttleBusManager();
		ShuttleBus* CreateBus(const string& _ident);
		ShuttleBus* FindBus(const string& _ident) const;
		ShuttleBus* GetBus(const string& _ident);
	};

};

#endif // !defined(AFX_SHUTTLEBUS_H__5E15D9D6_08AF_49BD_96FB_2835793F608E__INCLUDED_)
