/****************************************************************************
  This file is part of Space Shuttle Ultra

  Space Shuttle Main Engine Block II definition



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
#ifndef _mps_SSME_BLOCK_II_H_
#define _mps_SSME_BLOCK_II_H_


#include <orbitersdk.h>
#include "..\AtlantisSubsystem.h"
#include "..\Atlantis.h"
#include "SSME.h"
#include "Sensor.h"


namespace mps
{
	// SSME Block II model values
	//const double SSME_BLOCK_II_RPL_THRUST = 2090664.159; //100% thrust
	//const double SSME_BLOCK_II_NPL_THRUST = 2170732.15; //Nominal "104%" thrust
	const double SSME_BLOCK_II_FPL_THRUST = 2278824.21641;//2275094.273; //109% thrust
	const int SSME_BLOCK_II_FPL = 109;// % RPL
	const int SSME_BLOCK_II_MPL = 67;// % RPL
	const double SSME_BLOCK_II_RPL_PC_PRESS = 2746.788990825688073394495412844;// extrapolated from: 109% = 2994psia
	const double SSME_BLOCK_II_ISP0 = 453 * 9.80665;
	const double SSME_BLOCK_II_ISP1 = 363 * 9.80665;

	// pressure
	const int SSME_BLOCK_II_FPB_PP = 0;
	const int SSME_BLOCK_II_OPB_PP = 1;
	const int SSME_BLOCK_II_LPOT_DP_A = 2;
	const int SSME_BLOCK_II_LPOT_DP_B = 3;
	const int SSME_BLOCK_II_LPFT_DP_A = 4;
	const int SSME_BLOCK_II_LPFT_DP_B = 5;
	const int SSME_BLOCK_II_HPOTP_DP_A = 6;
	const int SSME_BLOCK_II_HPOTP_DP_B = 7;
	const int SSME_BLOCK_II_HPFTP_DP_A = 8;
	const int SSME_BLOCK_II_HPFTP_DP_B = 9;
	const int SSME_BLOCK_II_HPOTPBP_DP_A = 10;
	const int SSME_BLOCK_II_HPOTPBP_DP_B = 11;
	const int SSME_BLOCK_II_FS_PP_A = 12;
	const int SSME_BLOCK_II_FS_PP_B = 13;
	const int SSME_BLOCK_II_MCC_P_A1 = 14;
	const int SSME_BLOCK_II_MCC_P_B1 = 15;
	const int SSME_BLOCK_II_MCC_P_A2 = 16;
	const int SSME_BLOCK_II_MCC_P_B2 = 17;
	const int SSME_BLOCK_II_HPOTP_IS_A = 18;
	const int SSME_BLOCK_II_HPOTP_IS_B = 19;
	const int SSME_BLOCK_II_PNEU_SD_P_A = 20;
	const int SSME_BLOCK_II_PNEU_SD_P_B = 21;

	// temp
	const int SSME_BLOCK_II_HPFT_DT_A2 = 0;
	const int SSME_BLOCK_II_HPFT_DT_B2 = 1;
	const int SSME_BLOCK_II_HPFT_DT_A3 = 2;
	const int SSME_BLOCK_II_HPFT_DT_B3 = 3;
	const int SSME_BLOCK_II_HPOT_DT_A2 = 4;
	const int SSME_BLOCK_II_HPOT_DT_B2 = 5;
	const int SSME_BLOCK_II_HPOT_DT_A3 = 6;
	const int SSME_BLOCK_II_HPOT_DT_B3 = 7;
	const int SSME_BLOCK_II_PBP_DT_A = 8;
	const int SSME_BLOCK_II_PBP_DT_B = 9;
	const int SSME_BLOCK_II_LPFP_DT_A = 10;
	const int SSME_BLOCK_II_LPFP_DT_B = 11;
	const int SSME_BLOCK_II_MOV_HT_A = 12;
	const int SSME_BLOCK_II_MOV_HT_B = 13;
	const int SSME_BLOCK_II_MFV_HT_A = 14;
	const int SSME_BLOCK_II_MFV_HT_B = 15;

	// flow
	const int SSME_BLOCK_II_FF_A1 = 0;
	const int SSME_BLOCK_II_FF_B1 = 1;
	const int SSME_BLOCK_II_FF_A2 = 2;
	const int SSME_BLOCK_II_FF_B2 = 3;

	// speed
	const int SSME_BLOCK_II_LPFTP_SS_A = 0;
	const int SSME_BLOCK_II_LPFTP_SS_B = 1;
	const int SSME_BLOCK_II_HPFTP_SS_A = 2;
	const int SSME_BLOCK_II_HPFTP_SS_B = 3;


	using class discsignals::DiscreteBundle;


	class SSME_BLOCK_II:public SSME
	{
		private:
			// vars for RUN2
			double posMOV;
			double posMFV;

			double IMSLHeUsage;

			Sensor Sensor_Press[22];
			Sensor Sensor_Temp[16];
			Sensor Sensor_Flow[4];
			Sensor Sensor_Speed[4];

		public:
			SSME_BLOCK_II( AtlantisSubsystemDirector* _director, const string& _ident, unsigned short ID, int controllertype, const string& sw, HeSysEng* HeSys );
			~SSME_BLOCK_II( void );
			
			void RUN1( double time, double dt );// pre-start
			void RUN2( double time, double dt );// ignition
			void RUN3( double time, double dt );// mainstage
			void RUN4( double time, double dt );// cutoff
			void RUN5( double time, double dt );// post-shutdown

			void ConnectSensors( DiscreteBundle* IEchA_Press, DiscreteBundle* IEchB_Press, DiscreteBundle* IEchA_Temp, DiscreteBundle* IEchB_Temp, DiscreteBundle* IEchA_Flow, DiscreteBundle* IEchB_Flow, DiscreteBundle* IEchA_Speed, DiscreteBundle* IEchB_Speed );

			void __OnSaveState( FILEHANDLE scn ) const;
			bool __OnParseLine( const char* line );
	};
}


#endif// _mps_SSME_BLOCK_II_H_
