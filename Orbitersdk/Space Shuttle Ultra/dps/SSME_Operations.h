/****************************************************************************
  This file is part of Space Shuttle Ultra

  Space Shuttle Main Engine Operations definition



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
#ifndef _dps_SSME_OPS_H_
#define _dps_SSME_OPS_H_


#include "SimpleGPCSoftware.h"
#include "discsignals.h"


using namespace discsignals;


namespace dps
{
	const double TIME_DELAY_ADG = 4.5;// sec
	const double TIME_DELAY_CFI = 2;// sec
	const double TIME_DELAY_LH2_PVLV_CL = 4.9;// sec
	const double TIME_TO_ZERO_THRUST = 3.75;// sec, I-load


	class SSME_SOP;
	class IO_Control;


	class SSME_Operations:public SimpleGPCSoftware
	{
		private:
			SSME_SOP* pSSME_SOP;
			IO_Control* pIO_Control;

			DiscInPort dspLimitSwitchInhibit;
			DiscInPort dspLimitSwitchEnable;
			DiscInPort dspSSMESDPB[3];

			bool FailFlag[3];
			bool ManualShutdownFlag[3];
			bool ShutdownCommandIssued[3];
			bool ShutdownFlag_A[3];
			bool PVLV_CL_CMD[3];
			bool PVLV_CL_CMD_removed[3];

			bool MECOCommand;
			bool MECOConfirmed;
			bool ZeroThrust;

			unsigned short counter_A;
			unsigned short counter_B;
			unsigned short counter_CEG[3];
			unsigned short counter_DFH[3];

			double timerADG[3];
			double timerCFI[3];
			double timerLH2PVLV[3];
			double timerMECOConfirmed;

			int LimitSwitch;// 0-INH, 1-AUTO, 2-ENA

			int otherSSMEs[3][2];// engine index

		public:
			SSME_Operations( SimpleGPCSystem* _gpc );
			~SSME_Operations( void );

			void OnPreStep( double SimT, double DeltaT, double MJD );

			void Realize( void );

			bool OnMajorModeChange( unsigned int newMajorMode );

			/**
			 * Causes MECO to be issued.
			 */
			void SetMECOCommandFlag( void );

			/**
			 * Returns an indication of whether the MECO Command has been issued.
			 * @return		true = command issued
			 */
			bool GetMECOCommandFlag( void ) const;

			/**
			 * Returns an indication of whether the MECO Confirmed Flag has been set.
			 * @return		true = flag set
			 */
			bool GetMECOConfirmedFlag( void ) const;

			/**
			 * Returns an indication of whether the Zero Thrust Flag has been set.
			 * @return		true = flag set
			 */
			bool GetZeroThrustFlag( void ) const;

			/**
			 * Returns an indication of whether the Fail Flag has been set for a SSME.
			 * @param[in]	eng	SSME number
			 * @return		true = flag set
			 */
			bool GetFailFlag( int eng ) const;
	};
}


#endif// _dps_SSME_OPS_H_