/****************************************************************************
  This file is part of Space Shuttle Ultra

  Electronics Assembly 2 definition



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

#ifndef _comm_EA2_H_
#define _comm_EA2_H_


#include "..\Atlantis.h"
#include "..\AtlantisSubsystem.h"
#include <DiscInPort.h>


const VECTOR3 ANTENNA_OFFSET = _V( 3.5, 0.5, 10.5 );// m (approx)
const double RADAR_MAINLOBE_HALF_CONE = 5;// deg


namespace comm
{
	class DeployedAssembly;

	class ElectronicsAssembly2:public AtlantisSubsystem
	{
			DeployedAssembly* pDeployedAssembly;

			DiscInPort pPower_Off;
			DiscInPort pPower_On;
			DiscInPort KUaz;
			DiscInPort KUel;
			DiscInPort RADSTB;
			DiscInPort RADON;
			DiscInPort RADARPOWERLOW;
			DiscInPort RADARPOWERMEDIUM;

			DiscOutPort Range;
			DiscOutPort RangeRate;
			DiscOutPort Detect;
			DiscOutPort TGTaz;
			DiscOutPort TGTel;

			OBJHANDLE target;
			double rOLD;

			OBJHANDLE FindTarget( double &r, double &az, double &el );
			bool GetTargetInfo( OBJHANDLE hVessel, double &r, double &rr, double &az, double &el, double dt );

			bool IsDetected( OBJHANDLE hVessel, double &r, double &az, double &el );
			
			double GetPower( void );

		public:
			ElectronicsAssembly2( AtlantisSubsystemDirector* _director, DeployedAssembly* pDeployedAssembly );
			virtual ~ElectronicsAssembly2();

			void Realize( void );
			void OnPostStep( double fSimT, double fDeltaT, double fMJD );
	};

};

#endif// _comm_EA2_H_