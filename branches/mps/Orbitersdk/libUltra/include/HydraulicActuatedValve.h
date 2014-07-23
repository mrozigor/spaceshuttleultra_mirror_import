/****************************************************************************
  This file is part of Space Shuttle Ultra

  Hydraulic Actuated Valve Definition



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
#ifndef _g_HydraulicActuatedValve_H_
#define _g_HydraulicActuatedValve_H_


#include "PressureSource.h"
#include "DiscInPort.h"


using class discsignals::DiscInPort;
using class discsignals::DiscreteBundle;


// HACK no clue about right amounts
const double MIN_PRESS_CLOSE_HAV = 150;
const double HE_USE_CLOSE_HAV = 20;


class HydraulicActuatedValve:public PressureSource
{
	private:
		DiscInPort dipServovalve[2];

		DiscInPort dipFailOperationalServoswitch[2];
		DiscInPort dipFailSafeServoswitch[2];

		DiscInPort dipHYD;

		PressureSource* PneumaticClose;

		double pos;
		double rate_hyd;
		double rate_pneu;

		double SequenceValveOpenPos;
	public:
		/**
		 * Returns valve position
		 * @return valve position (range: 0 - closed, 1 - open)
		 */
		double GetPos( void ) const;

		/**
		 * Updates valve position (call from time step functions)
		 * @param dt sim dt
		 */
		void tmestp( double dt );

		/**
		 * Use from .scn loading function to set valve position
		 * @param ipos valve position
		 */
		void _backdoor( double ipos );

		/**
		 * Create a new valve
		 * @param initpos initial valve position
		 * @param rate_hyd valve motion rate under hydraulic pressure
		 */
		HydraulicActuatedValve( double initpos, double rate_hyd );
		/**
		 * Create a new valve
		 * @param initpos initial valve position
		 * @param rate_hyd valve motion rate under hydraulic pressure
		 * @param rate_pneu valve motion rate under pneumatic pressure
		 * @param PneumaticClose handle to PressureSource for pneumatic actuator close
		 * @param SequenceValveOpenPos valve position at which the sequence valve opens
		 */
		HydraulicActuatedValve( double initpos, double rate_hyd, double rate_pneu, PressureSource* PneumaticClose, double SequenceValveOpenPos = 0 );
		~HydraulicActuatedValve( void );

		void Connect( DiscreteBundle* pBundle, DiscreteBundle* pBundleHYD );

		double Use( double flow );// HACK just for sequence valve
};


#endif// _g_HydraulicActuatedValve_H_
