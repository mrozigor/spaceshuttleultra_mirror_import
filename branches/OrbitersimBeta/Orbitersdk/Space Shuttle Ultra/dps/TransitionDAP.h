/****************************************************************************
  This file is part of Space Shuttle Ultra

  Transition Digital Auto Pilot definition



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
#ifndef _dps_TRANS_DAP_H_
#define _dps_TRANS_DAP_H_


#include "SimpleGPCSoftware.h"
#include "discsignals.h"


const double MINUSZ_TARGET_DV_NOM = 1.2192;// m/s (4 fps)
const double MINUSZ_TARGET_DV_TAL = 3.3528;// m/s (11 fps)
const double MINUSZ_TARGET_DT_RTLS = 10;// sec


using namespace discsignals;


namespace dps
{
	class SSME_Operations;

	class TransitionDAP:public SimpleGPCSoftware
	{
		private:
			SSME_Operations* pSSME_Operations;

			DiscOutPort ZTransCommand;
			DiscOutPort RotThrusterCommands[3];
			DiscOutPort PBI[4];

			bool MinusZTranslation;

			double ETSepMinusZDV;
			double ETSepMinusZDT;
		public:
			TransitionDAP( SimpleGPCSystem* _gpc );
			~TransitionDAP( void );

			void OnPostStep( double SimT, double DeltaT, double MJD );

			void Realize( void );

			bool OnMajorModeChange( unsigned int newMajorMode );

			void MinusZTranslationCommand();

			void NullRates( double DeltaT );
	};
}


#endif// _dps_TRANS_DAP_H_