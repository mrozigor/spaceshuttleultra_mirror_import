/****************************************************************************
  This file is part of Space Shuttle Ultra

  Block II SSME definition



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
#ifndef _mps_BLOCK_II_H_
#define _mps_BLOCK_II_H_


#include <orbitersdk.h>
#include "..\AtlantisSubsystem.h"
#include "SSME.h"


namespace mps
{
	//const double BLOCK_II_RPL_THRUST = 2090664.159; //100% thrust
	//const double BLOCK_II_NPL_THRUST = 2170732.15; //Nominal "104%" thrust
	const double BLOCK_II_FPL_THRUST = 2275094.273; //109% thrust

	const int BLOCK_II_FPL = 109;// % RPL
	const int BLOCK_II_MPL = 67;// % RPL

	const double BLOCK_II_RPL_PC_PRESS = 2746.789;// extrapolated from: 109% = 2994psia

	const double BLOCK_II_ISP0 = 453 * 9.80665;
	const double BLOCK_II_ISP1 = 363 * 9.80665;


	class BLOCK_II:public SSME
	{
	private:
		void ValveScheduleThrottle( double );

		void ValveShutdownTableUpdate( double dPC );

		void VDTUpdate( double );

		// data cookup fncts
		double dcPC_ESC( double );
		double dcPC_CO( double );
		double dcPC_MS( double );

	public:
		BLOCK_II( SubsystemDirector*, const string&, int );
		~BLOCK_II( void );

		// heart beat
		//void OnPostStep( double, double, double );
		//void OnPreStep( double, double, double );
		//void OnPropagate( double, double, double );

		void OnSaveState( FILEHANDLE ) const;
		virtual bool SingleParamParseLine() const {return true;};
		bool OnParseLine( const char* );
		};
}


#endif// _mps_BLOCK_II_H_
