/****************************************************************************
  This file is part of Space Shuttle Ultra

  Engine Interface Unit definition



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
#ifndef _mps_EIU_H_
#define _mps_EIU_H_


#include "SSME.h"
#include <orbitersdk.h>
#include "..\AtlantisSubsystem.h"
#include "..\dps\dps_defs.h"
#include "..\dps\BIU.h"


namespace mps
{
	class EIU:public AtlantisSubsystem, public dps::IConnectedToBus
	{
	private:
		int ID;
		SSME* engine;
		VDT_32* PrimaryData;
		VDT_6* SecondaryData;
	public:
		EIU( SubsystemDirector*, const string&, int, SSME* );
		~EIU( void );

		virtual void busCommandPhase(dps::BusController* biu);
		virtual void busReadPhase(dps::BusController* biu);
		virtual dps::BUS_COMMAND_WORD busCommand(dps::BusTerminal* biu, dps::BUS_COMMAND_WORD cw, 
			unsigned long num_data, dps::word16 *cdw);

		// GPCs only

		/**
		 * Return primary data
		 * @param ptrPrimaryData pointer to primary data structure VDT_32
		 */
		int RequestPrimaryData( VDT_32* ptrPrimaryData );

		/**
		 * Return secondary data
		 * @param ptrSecondaryData pointer to secondary data structure VDT_6
		 */
		int RequestSecondaryData( VDT_6* ptrSecondaryData );
		int RequestCMD( int, double );

		// heart beat
		void OnPostStep( double, double, double );
		//void OnPreStep( double, double, double );
		//void OnPropagate( double, double, double );

		// TODO power swicth -> O17

		dps::BusTerminal mia[4];
	};
}


#endif// _mps_EIU_H_
