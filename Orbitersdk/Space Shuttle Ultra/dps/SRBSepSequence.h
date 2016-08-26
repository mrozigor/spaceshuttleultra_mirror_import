/****************************************************************************
  This file is part of Space Shuttle Ultra

  Solid Rocket Booster Separation Sequence definition



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
#ifndef _dps_SRBSEP_H_
#define _dps_SRBSEP_H_


#include "SimpleGPCSoftware.h"
#include "discsignals.h"


const double SRB_SEP_SEQUENCE_START_TIME = 100;// sec
const double SRB_SEP_PC50_DT = 5.5;// sec
const double SRB_SEP_BACKUP_CUE_T = 131.28;// sec (I-load)
const double SRB_SEP_MOD_DELAY = 3.2;// sec
const double SRB_SEP_CMD_DELAY = 4.9;// sec

// STS 1
//const double SRB_SEP_ROLL_RATE_LIMIT = 5.0;// deg/sec
//const double SRB_SEP_PITCH_RATE_LIMIT = 2.0;// deg/sec
//const double SRB_SEP_YAW_RATE_LIMIT = 2.0;// deg/sec
//const double SRB_SEP_QBAR_LIMIT = 55;// lb/ft2

// pre STS 73
//const double SRB_SEP_ROLL_RATE_LIMIT = 5.0;// deg/sec
//const double SRB_SEP_PITCH_RATE_LIMIT = 2.0;// deg/sec
//const double SRB_SEP_YAW_RATE_LIMIT = 2.0;// deg/sec
//const double SRB_SEP_QBAR_LIMIT = 63;// lb/ft2

// STS 73 and subs
const double SRB_SEP_ROLL_RATE_LIMIT = 99999;// deg/sec
const double SRB_SEP_PITCH_RATE_LIMIT = 99999;// deg/sec
const double SRB_SEP_YAW_RATE_LIMIT = 99999;// deg/sec
const double SRB_SEP_QBAR_LIMIT = 99999;// lb/ft2


using namespace discsignals;


namespace dps
{
	class SSME_Operations;
	class AscentGuidance;
	class MEC_SOP;

	/**
	 * @brief	Implementation of the SRB Separation Sequencer running in the GPCs.
	 * 
	 * Controls and executes the SRB Separation, checking that separation conditions are met.
	 */
	class SRBSepSequence:public SimpleGPCSoftware
	{
		private:
			SSME_Operations* pSSME_Operations;
			AscentGuidance* pAscentGuidance;
			MEC_SOP* pMEC_SOP;

			DiscInPort SRBSEPSW;
			DiscInPort SRBSEPPB;

			bool PC50;
			bool SRBSEPinit;
			bool SRBSEPCommand;
			bool SRBSEPINH;

			double timePC50L;
			double timePC50R;
			double timeSRBSEPinit;
		public:
			SRBSepSequence( SimpleGPCSystem* _gpc );
			~SRBSepSequence( void );

			void OnPostStep( double SimT, double DeltaT, double MJD );

			void Realize( void );

			bool OnMajorModeChange( unsigned int newMajorMode );

			/**
			 * Returns an indication of whether the SRB PC < 50 flag has been set.
			 * @return		true = PC < 50
			 */
			bool GetPC50Flag( void ) const;

			/**
			 * Returns an indication of whether the SRB Separation inhibit flag has been set.
			 * @return		true = SRB Separation inhibited
			 */
			bool GetSRBSEPINHFlag( void ) const;
	};
}


#endif// _dps_SRBSEP_H_