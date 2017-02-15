/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel O2 definition



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
#ifndef __VC_PANELO2_H
#define __VC_PANELO2_H


#include "AtlantisPanel.h"
#include "StandardSwitch.h"
#include "StandardRotarySwitch.h"
#include "StandardCurvedMechMeter.h"


namespace vc
{
	class PanelO2 : public AtlantisPanel
	{
		private:
			StandardCurvedMechMeter* pCryoO2HtrAssyTempMeter;
			StandardCurvedMechMeter* pCryoO2PressMeter;
			StandardCurvedMechMeter* pCryoO2QtyMeter;
			StandardCurvedMechMeter* pCryoH2PressMeter;
			StandardCurvedMechMeter* pCryoH2QtyMeter;
			StandardCurvedMechMeter* pFuelCellStackTempMeter;

			StandardRotarySwitch* pCryoO2HtrAssyTemp;
			StandardRotarySwitch* pCryoPressQty;
			StdSwitch3* pFuelCellStackTemp;

			DiscInPort pFuelCellStackTemp1;
			DiscInPort pFuelCellStackTemp3;
			DiscOutPort pFuelCellStackTempMeterInput;

		public:
			PanelO2( Atlantis* _sts );
			virtual ~PanelO2();

			virtual void DefineVC( void );
			virtual void RegisterVC( void );
			virtual void Realize( void );
			virtual void OnPreStep( double SimT, double DeltaT, double MJD );
	};
}

#endif// __VC_PANELO2_H