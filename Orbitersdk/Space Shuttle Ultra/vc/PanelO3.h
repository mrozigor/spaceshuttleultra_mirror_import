/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel O3 definition



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
#ifndef __VC_PANELO3_H
#define __VC_PANELO3_H


#include "AtlantisPanel.h"
#include "StandardSwitch.h"
#include "StandardRotarySwitch.h"
#include "7SegDisp_RCSOMS_PRPLT_QTY.h"
#include "7SegDisp_MissionTime.h"


namespace vc
{
	class PanelO3 : public AtlantisPanel
	{
		private:
			RotaryDemuxSwitch* pRCSOMS_PRESS;
			RotaryDemuxSwitch* pRCSOMS_PRPLTQTY;

			StdSwitch3* pMissionTimer;
			
			DiscInPort pRCSOMS_PRESS_RCSHeX10;
			DiscInPort pRCSOMS_PRESS_RCSPRPLT;
			DiscInPort pRCSOMS_PRESS_OMSPRPLT;

			_7SegDisp_RCSOMS_PRPLT_QTY* pRCSOMS_PRPLT_QTY;
			_7SegDisp_MissionTime* pMissionTime;

		public:
			PanelO3( Atlantis* _sts );
			virtual ~PanelO3();

			virtual void DefineVC();
			virtual void RegisterVC();
			virtual void Realize();
	};
}

#endif// __VC_PANELO3_H