/****************************************************************************
  This file is part of Space Shuttle Ultra

  IO Control definition



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
#ifndef _dps_IO_Control_H_
#define _dps_IO_Control_H_


#include "SimpleGPCSoftware.h"
#include "discsignals.h"


using namespace discsignals;


namespace dps
{
	const int SW_DUMPSEQUENCE = 0;
	const int SW_BACKUPLH2VLV = 1;
	const int SW_PNEU_L_HE_XOVR = 2;
	const int SW_PNEU_HE_ISOL = 3;
	const int SW_HE_ISOL_A_CTR = 4;
	const int SW_HE_ISOL_A_LEFT = 5;
	const int SW_HE_ISOL_A_RIGHT = 6;
	const int SW_HE_ISOL_B_CTR = 7;
	const int SW_HE_ISOL_B_LEFT = 8;
	const int SW_HE_ISOL_B_RIGHT = 9;
	const int SW_HE_IC_CTR = 10;
	const int SW_HE_IC_LEFT = 11;
	const int SW_HE_IC_RIGHT = 12;
	const int SW_LH2ULLAGEPRESS = 13;
	const int COUNT_INPUT = 14;

	const int LH2_INBD_BU_DV_OP = 0;
	const int LH2_OTBD_BU_DV_OP = 1;
	const int PNEU_L_HE_XOVR_OP = 2;
	const int PNEU_HE_ISOL_1_OP = 3;
	const int PNEU_HE_ISOL_2_OP = 4;
	const int HE_ISOL_A_CTR_OP = 5;
	const int HE_ISOL_A_LEFT_OP = 6;
	const int HE_ISOL_A_RIGHT_OP = 7;
	const int HE_ISOL_B_CTR_OP = 8;
	const int HE_ISOL_B_LEFT_OP = 9;
	const int HE_ISOL_B_RIGHT_OP = 10;
	const int HE_IC_CTR_IN_OP = 11;
	const int HE_IC_LEFT_IN_OP = 12;
	const int HE_IC_RIGHT_IN_OP = 13;
	const int HE_IC_CTR_OUT_OP = 14;
	const int HE_IC_LEFT_OUT_OP = 15;
	const int HE_IC_RIGHT_OUT_OP = 16;
	const int LOX_OVBD_BV = 17;
	const int ME1_LOX_PVLV_CL_A = 18;
	const int ME2_LOX_PVLV_CL_A = 19;
	const int ME3_LOX_PVLV_CL_A = 20;
	const int ME1_LOX_PVLV_CL_B = 21;
	const int ME2_LOX_PVLV_CL_B = 22;
	const int ME3_LOX_PVLV_CL_B = 23;
	const int ME1_LOX_PVLV_OP_A = 24;
	const int ME2_LOX_PVLV_OP_A = 25;
	const int ME3_LOX_PVLV_OP_A = 26;
	const int ME1_LOX_PVLV_OP_B = 27;
	const int ME2_LOX_PVLV_OP_B = 28;
	const int ME3_LOX_PVLV_OP_B = 29;
	const int ME1_LH2_PVLV_CL = 30;
	const int ME2_LH2_PVLV_CL = 31;
	const int ME3_LH2_PVLV_CL = 32;
	const int ME1_LH2_PVLV_OP = 33;
	const int ME2_LH2_PVLV_OP = 34;
	const int ME3_LH2_PVLV_OP = 35;
	const int LOX_POGO_RECIRC_1 = 36;
	const int LOX_POGO_RECIRC_2 = 37;
	const int LOX_OTBD_FD_VLV_CL = 38;
	const int LOX_OTBD_FD_VLV_OP = 39;
	const int LOX_INBD_FD_VLV_CL = 40;
	const int LOX_INBD_FD_VLV_OP = 41;
	const int LH2_OTBD_FD_VLV_CL = 42;
	const int LH2_OTBD_FD_VLV_OP = 43;
	const int LH2_INBD_FD_VLV_CL = 44;
	const int LH2_INBD_FD_VLV_OP = 45;
	const int LH2_TOPPING_VLV_OP = 46;
	const int LOX_REPRESS_1_OP = 47;
	const int LOX_REPRESS_2_OP = 48;
	const int LH2_REPRESS_1_OP = 49;
	const int LH2_REPRESS_2_OP = 50;
	const int LO2_FEEDLINE_RLF_ISOL_CL = 51;
	const int LH2_FEEDLINE_RLF_ISOL_CL = 52;
	const int GOX_FCV_1 = 53;
	const int GOX_FCV_2 = 54;
	const int GOX_FCV_3 = 55;
	const int GH2_FCV_1 = 56;
	const int GH2_FCV_2 = 57;
	const int GH2_FCV_3 = 58;
	const int COUNT_OUTPUT = 59;

	class IO_Control:public SimpleGPCSoftware
	{
		private:
			DiscInPort dspINPUT_1[COUNT_INPUT];
			DiscInPort dspINPUT_2[COUNT_INPUT];

			DiscOutPort dspOUTPUT[COUNT_OUTPUT];

			bool CMD[COUNT_OUTPUT];

		public:
			IO_Control( SimpleGPCSystem* _gpc );
			~IO_Control( void );

			void OnPreStep( double SimT, double DeltaT, double MJD );
			void OnPostStep( double SimT, double DeltaT, double MJD );

			void Realize( void );

			bool OnMajorModeChange( unsigned int newMajorMode );

			int GetSWPos( int sw );
			void SetCommand( int output, bool cmd );
	};
}


#endif// _dps_IO_Control_H_