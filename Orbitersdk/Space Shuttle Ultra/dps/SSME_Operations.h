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
	// HACK only know the first 2 for sure, the rest is guess
	const double NOM_LO2_LOW_LVL_TIME_DELAY_L = 0.358;// sec
	const double TWOENG_LO2_LOW_LVL_TIME_DELAY_L = 0;// sec
	const double ONEENG_LO2_LOW_LVL_TIME_DELAY_L = 0;// sec

	const double NOM_LH2_LOW_LVL_TIME_DELAY_L = 0;// sec
	const double TWOENG_LH2_LOW_LVL_TIME_DELAY_L = 0;// sec
	const double ONEENG_LH2_LOW_LVL_TIME_DELAY_L = 0;// sec

	const double TIME_DELAY_ADG = 4.5;// sec
	const double TIME_DELAY_CFI = 2;// sec
	const double TIME_DELAY_LH2_PVLV_CL = 4.9;// sec
	const double TIME_TO_ZERO_THRUST = 3.75;// sec, I-load


	class SSME_SOP;
	class IO_Control;

	/**
	 * @brief	Implementation of the SSME Operations software running in the GPCs.
	 * 
	 * Controls the sequence of operations related to the SSMEs, also containing low-level engine cutoff
	 * logic and also engine shutdown limits logic.
	 */
	class SSME_Operations:public SimpleGPCSoftware
	{
		private:
			SSME_SOP* pSSME_SOP;
			IO_Control* pIO_Control;

			DiscInPort dipLimitSwitchInhibit;
			DiscInPort dipLimitSwitchEnable;
			DiscInPort dipSSMESDPB[3];
			DiscInPort dipLO2LowLevelSensor[4];
			DiscInPort dipLO2UllagePressureSensor[4];
			DiscInPort dipLH2LowLevelSensor[4];
			DiscInPort dipLH2UllagePressureSensor[4];

			bool FailFlag[3];
			bool ManualShutdownFlag[3];
			bool ShutdownCommandIssued[3];
			bool ShutdownFlag_A[3];
			bool PVLV_CL_CMD[3];
			bool PVLV_CL_CMD_removed[3];

			bool LowLevelSensorArm;
			bool LO2LowLevelSensorDryFlag[4];
			bool LH2LowLevelSensorDryFlag[4];
			bool LO2LowLevelSensorDsblFlag[4];
			bool LH2LowLevelSensorDsblFlag[4];
			bool LowLevel1stRun;
			double LowLevelLO2timer;
			double LowLevelLH2timer;

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

			//bool OnParseLine( const char* keyword, const char* value );
			//void OnSaveState( FILEHANDLE scn ) const;

			bool OnMajorModeChange( unsigned int newMajorMode );

			/**
			 * Causes MECO to be issued.
			 */
			void SetMECOCommandFlag( void );

			/**
			 * Activates low-level sensor logic.
			 */
			void SetLowLevelSensorArmFlag( void );

			/**
			 * Sets the LO2 Low Level Sensor Disabled Flag for the specified sensor. For exclusive use by LPS.
			 * @param[in]	num	sensor number
			 */
			void SetLO2LowLevelSensorDsblFlag( int num );

			/**
			 * Sets the LH2 Low Level Sensor Disabled Flag for the specified sensor. For exclusive use by LPS.
			 * @param[in]	num	sensor number
			 */
			void SetLH2LowLevelSensorDsblFlag( int num );

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