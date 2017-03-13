/****************************************************************************
  This file is part of Space Shuttle Ultra

  MPS Dump definition



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
#ifndef _dps_MPS_Dump_H_
#define _dps_MPS_Dump_H_


#include "SimpleGPCSoftware.h"
#include "discsignals.h"


using namespace discsignals;


namespace dps
{
	const double DUMP_START_DELAY = 120;// from MECO confirmed

	const double HE_IC_OP_DELAY = 20;// from MECO confirmed
	const double HE_IC_CL_DELAY = 120;// from dump start

	const double LOX_DUMP_START_DELAY = 0;// from dump start
	const double LOX_DUMP_DURATION = 120;
	const double LOX_DUMP_PRESS_DURATION = 90;

	const double LH2_DUMP_START_DELAY = 0;// from MECO
	const double LH2_DUMP_DURATION = 120;

	const double LH2_DUMP_BU_VLV_START_DELAY = 11.4;// from MECO confirmed
	const double LH2_DUMP_BU_VLV_DURATION = 228.6;

	const double FIRST_AUTOMATED_VACUUM_INERT_START_DELAY = 1020;// from dump start
	const double FIRST_AUTOMATED_VACUUM_INERT_DURATION = 120;

	const double SECOND_AUTOMATED_VACUUM_INERT_START_DELAY = 0;// from MM106
	const double SECOND_AUTOMATED_VACUUM_INERT_DURATION = 180;// approx


	class SSME_SOP;
	class SSME_Operations;
	class IO_Control;
	class ATVC_SOP;

	/**
	 * @brief	Implementation of the MPS Dump software running in the GPCs.
	 * 
	 * Controls the post-MECO MPS Dump sequence by issuing open and close commands to appropriate valves at specified times.
	 */
	class MPS_Dump:public SimpleGPCSoftware
	{
		private:
			SSME_SOP* pSSME_SOP;
			SSME_Operations* pSSME_Operations;
			IO_Control* pIO_Control;
			ATVC_SOP* pATVC_SOP;

			DiscOutPort CDR_BodyFlapManLight;
			DiscOutPort PLT_BodyFlapManLight;

			double t_MECO;
			double t_dump_start;
			double t_last;
			double t_MM106_trans;

			bool active;
			bool dump_started;
			bool MM106_trans;

		public:
			MPS_Dump( SimpleGPCSystem* _gpc );
			~MPS_Dump( void );

			void OnPreStep( double SimT, double DeltaT, double MJD );

			void Realize( void );

			bool OnMajorModeChange( unsigned int newMajorMode );
	};
}


#endif// _dps_MPS_Dump_H_