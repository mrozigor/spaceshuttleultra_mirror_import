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


namespace comm
{
	class DeployedAssembly;

	class ElectronicsAssembly2:public AtlantisSubsystem
	{
			DeployedAssembly* pDeployedAssembly;

			discsignals::DiscInPort pPower_Off;
			discsignals::DiscInPort pPower_On;

			bool FindTarget( double &rng, double &az, double &el );

		public:
			ElectronicsAssembly2( AtlantisSubsystemDirector* _director, DeployedAssembly* pDeployedAssembly );
			virtual ~ElectronicsAssembly2();

			void Realize( void );
			void OnPostStep( double fSimT, double fDeltaT, double fMJD );
	};

};

#endif// _comm_EA2_H_