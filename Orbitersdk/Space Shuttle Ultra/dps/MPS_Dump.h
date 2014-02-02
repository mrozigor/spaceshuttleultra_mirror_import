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
	const double LOX_DUMP_START_DELAY = 120;// from MECO
	const double LOX_DUMP_DURATION = 120;
	const double LOX_DUMP_PRESS_DURATION = 90;

	const double LH2_DUMP_BU_VLV_START_DELAY = 11.4;// from MECO
	const double LH2_DUMP_BU_VLV_DURATION = 228.6;
	const double LH2_DUMP_START_DELAY = 120;// from MECO
	const double LH2_DUMP_DURATION = 120;

	const double FIRST_AUTOMATED_VACUUM_INERT_START_DELAY = 1020;// from dump start
	const double FIRST_AUTOMATED_VACUUM_INERT_DURATION = 120;


	class SSME_SOP;
	class SSME_Operations;
	class IO_Control;


	class MPS_Dump:public SimpleGPCSoftware
	{
		private:
			SSME_SOP* pSSME_SOP;
			SSME_Operations* pSSME_Operations;
			IO_Control* pIO_Control;

			DiscOutPort dspLH2_OTBD_FD_VLV_OP;
			DiscOutPort dspLH2_OTBD_FD_VLV_CL;
			DiscOutPort dspLH2_INBD_FD_VLV_OP;
			DiscOutPort dspLH2_INBD_FD_VLV_CL;

			DiscOutPort dspLH2_TOPPING_VLV_OP;

			DiscOutPort dspLO2_Manf_Repress_1;
			DiscOutPort dspLO2_Manf_Repress_2;

			double t_MECO;
			double t_current;
			double t_last;

			bool active;

			bool LH2_OTBD_FD_VLV_OP_CMD;
			bool LH2_OTBD_FD_VLV_CL_CMD;
			bool LH2_INBD_FD_VLV_OP_CMD;
			bool LH2_INBD_FD_VLV_CL_CMD;

			bool LH2_TOPPING_VLV_OP_CMD;

		public:
			MPS_Dump( SimpleGPCSystem* _gpc );
			~MPS_Dump( void );

			void OnPreStep( double SimT, double DeltaT, double MJD );

			void Realize( void );

			bool OnMajorModeChange( unsigned int newMajorMode );

	};
}


#endif// _dps_MPS_Dump_H_