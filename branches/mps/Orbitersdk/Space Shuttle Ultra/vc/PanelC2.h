/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel C2 definition



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
#ifndef VC_PANELC2
#define VC_PANELC2


#include "AtlantisPanel.h"
#include "StandardSwitch.h"
#include "Keyboard.h"


const double EVTTMR_WHEELMAX[4] = {3.0, 5.0, 3.0, 5.0};


namespace vc
{
	class PanelC2:public AtlantisPanel
	{
			StdSwitch2* pIDPCRTPower[3];
			StdSwitch3* pIDPCRTMajFunc[3];
			StdSwitch2* pCRTSEL[2];

			StdSwitch3* pEventTimerMode;
			StdSwitch3* pEventTimerControl;
			StdSwitch3* pTimer;

			Keyboard* pKeyboardCDR;
			Keyboard* pKeyboardPLT;

			DiscInPort pEventTimerMode_Up;
			DiscInPort pEventTimerMode_Test;

			DiscInPort pEventTimerControl_Start;
			DiscInPort pEventTimerControl_Stop;

			DiscInPort pTimer_Set;
			DiscInPort pTimer_Reset;

			UINT anim_VC_C2Wheel[4];

			double tgtwheel_state[4];
			short wheelnumber[4];
			short oldwheelnumber[4];

			double wheelState[4];

		public:
			PanelC2( Atlantis* psts );
			virtual ~PanelC2();

			virtual void OnPreStep( double SimT, double DeltaT, double MJD );

			virtual bool OnVCMouseEvent( int id, int _event, VECTOR3 &p );
			virtual bool OnVCRedrawEvent( int id, int _event, SURFHANDLE surf );

			virtual void DefineVC();
			virtual void RegisterVC();
			virtual void DefineVCAnimations( UINT vcidx );
			virtual void Realize();
	};
};

#endif //VC_PANELC2
