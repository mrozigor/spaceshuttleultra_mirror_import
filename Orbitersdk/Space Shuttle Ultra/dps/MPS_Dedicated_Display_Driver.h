/****************************************************************************
  This file is part of Space Shuttle Ultra

  Main Propulsion System Dedicated Display Driver definition



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
#ifndef _dps_MPS_DDD_H_
#define _dps_MPS_DDD_H_


#include "SimpleGPCSoftware.h"
#include "discsignals.h"


using namespace discsignals;


namespace dps
{
	class SSME_SOP;

	class MPS_Dedicated_Display_Driver:public SimpleGPCSoftware
	{
		private:
			SSME_SOP* pSSME_SOP;

			DiscOutPort dspAmberStatusLight[3];
			DiscOutPort dpsRedStatusLight[3];
		public:
			MPS_Dedicated_Display_Driver( SimpleGPCSystem* _gpc );
			~MPS_Dedicated_Display_Driver( void );

			void OnPostStep( double SimT, double DeltaT, double MJD );

			void Realize( void );

			bool OnMajorModeChange( unsigned int newMajorMode );
	};
}


#endif// _dps_MPS_DDD_H_