/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel R13L definition



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
#ifndef VC_PANELR13L
#define VC_PANELR13L


#include "AtlantisPanel.h"
#include "StandardSwitch.h"
#include "Talkback.h"


namespace vc
{
	class PanelR13L:public AtlantisPanel
	{
			StdSwitch2* pPLBayDoor[2];
			StdSwitch2* pPLBayMechPWR[2];

			StdSwitch3* pLatchControl[2];
			StdSwitch3* pRadiatorControl[2];
			StdSwitch3* pKUAntenna;
			StdSwitch3* pMMUGN2SPLYISOLVLV[2];

			LockableLever3* pPLBayDoorLL;
			LockableLever2* pKUAntennaDirectStow;

			StandardTalkback3* pPLBayDoorTB;
			StandardTalkback3* pLatch[2];
			StandardTalkback3* pRadiator[2];
			StandardTalkback3* pKUAntennaTB;
			StandardTalkback3* pMMUGN2SPLYISOLVLVTB[2];
		public:
			PanelR13L( Atlantis* psts );
			virtual ~PanelR13L();

			virtual void DefineVC();
			virtual void RegisterVC();
			virtual void Realize();
	};
};

#endif //VC_PANELR13L
