/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel A2 definition



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
#ifndef __VC_PANELA2_H
#define __VC_PANELA2_H


#include "AtlantisPanel.h"
#include "StandardSwitch.h"
#include "7SegDisp_RE_RRA.h"


namespace vc
{
	class PanelA2:public AtlantisPanel
	{
			UINT anim_M1_AZ;
			UINT anim_M1_EL;

			bool power;

			StdSwitch3* pDIGI_DIS_SELECT;
			StdSwitch2* pXPNTR_SCALE;

			_7SegDisp_RE_RRA* pRE_RRA;

			DiscInPort pXPNTR_SCALE_X1;

			DiscInPort pELrate0_20;
			DiscInPort pELrate0_2;
			DiscInPort pAZrate0_20;
			DiscInPort pAZrate0_2;

			DiscInPort pPower_On;

		public:
			PanelA2( Atlantis* _sts );
			virtual ~PanelA2();

			virtual void DefineVC();
			virtual void RegisterVC();
			void DefineVCAnimations( UINT vcidx );
			void OnPreStep( double SimT, double DeltaT, double MJD );
			virtual void Realize();
	};
}

#endif// __VC_PANELA2_H
