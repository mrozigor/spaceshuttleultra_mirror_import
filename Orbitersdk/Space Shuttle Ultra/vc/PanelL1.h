/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel L1 definition



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
#ifndef __VC_PANELL1_H
#define __VC_PANELL1_H


#include "AtlantisPanel.h"
#include "StandardSwitchCover.h"
#include "PushButtonIndicator.h"
#include "StandardLight.h"


namespace vc
{
	class PanelL1 : public AtlantisPanel
	{
		private:
			StandardSwitchCover* pFireSuppressionAvBayDischAgentCover[3];
			PushButtonIndicatorSingleLight* pFireSuppressionAvBayDischAgent[3];

			StandardSingleLight* pSmokeDetectionA_Cabin;
			StandardSingleLight* pSmokeDetectionA_LFltDeck;
			StandardSingleLight* pSmokeDetectionA_AvBay1;
			StandardSingleLight* pSmokeDetectionA_AvBay2;
			StandardSingleLight* pSmokeDetectionA_AvBay3;
			StandardSingleLight* pSmokeDetectionB_Payload;
			StandardSingleLight* pSmokeDetectionB_RFltDeck;
			StandardSingleLight* pSmokeDetectionB_AvBay1;
			StandardSingleLight* pSmokeDetectionB_AvBay2;
			StandardSingleLight* pSmokeDetectionB_AvBay3;

		public:
			PanelL1( Atlantis* _sts );
			virtual ~PanelL1();

			virtual void DefineVC( void );
			virtual void RegisterVC( void );
			virtual void Realize( void );
	};
}

#endif// __VC_PANELL1_H