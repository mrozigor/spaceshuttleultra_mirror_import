/****************************************************************************
  This file is part of Space Shuttle Ultra

  External Tank Separation Sequence definition



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
#ifndef _dps_ETSEP_H_
#define _dps_ETSEP_H_


#include "SimpleGPCSoftware.h"
#include "discsignals.h"


// HACK little info on the time sequence
const double ET_SEP_DELAY_MECO = 18;// sec
const double ET_SEP_CLOSE_PD = 8;// sec
const double ET_SEP_OPEN_SOV = 8;// sec
const double ET_SEP_SSME_STOW = 8;// sec
const double ET_SEP_ARM_SEP_PIC = 9.5;// sec
const double ET_SEP_DELAY_MINUSZ = 0.16;// sec
const double ET_SEP_ROLL_RATE_LIMIT = 0.7;// deg/sec (I-load)
const double ET_SEP_PITCH_RATE_LIMIT = 0.7;// deg/sec (I-load)
const double ET_SEP_YAW_RATE_LIMIT = 0.7;// deg/sec (I-load)


using namespace discsignals;


namespace dps
{
	class SSME_Operations;
	class TransitionDAP;
	class IO_Control;
	class ATVC_SOP;

	/**
	 * @brief	Implementation of the ET Separation Sequencer running in the GPCs.
	 * 
	 * Controls and executes the ET Separation, closing the appropriate valves and checking that separation conditions are met.
	 */
	class ETSepSequence:public SimpleGPCSoftware
	{
		private:
			SSME_Operations* pSSME_Operations;
			TransitionDAP* pTransitionDAP;
			IO_Control* pIO_Control;
			ATVC_SOP* pATVC_SOP;

			DiscInPort ETSEPSW;
			DiscInPort ETSEPPB;

			DiscOutPort PD1_OP;
			DiscOutPort PD1_CL;
			DiscOutPort PD2_OP;
			DiscOutPort PD2_CL;
			DiscOutPort PD3_OP;
			DiscOutPort PD3_CL;

			DiscInPort PD1_CL_Ind_A;
			DiscInPort PD1_CL_Ind_B;
			DiscInPort PD2_CL_Ind_A;
			DiscInPort PD2_CL_Ind_B;
			DiscInPort PD3_CL_Ind;

			bool active;
			bool done;
			bool autoETSEP;
			bool ETSEPCommand;
			bool ETSEPINH;

			double t_MECO;
			double t_last;
			double timerSEP;
		public:
			ETSepSequence( SimpleGPCSystem* _gpc );
			~ETSepSequence( void );

			void OnPostStep( double SimT, double DeltaT, double MJD );

			void Realize( void );

			bool OnMajorModeChange( unsigned int newMajorMode );

			/**
			 * Returns an indication of whether the ET Separation inhibit flag has been set.
			 * @return		true = ET Separation inhibited
			 */
			bool GetETSEPINHFlag( void ) const;
	};
}


#endif// _dps_ETSEP_H_