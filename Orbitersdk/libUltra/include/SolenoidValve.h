/****************************************************************************
  This file is part of Space Shuttle Ultra

  Solenoid Valve Definition



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
#ifndef _g_SolenoidValve_H_
#define _g_SolenoidValve_H_


#include "PressureSource.h"
#include "DiscInPort.h"


using class discsignals::DiscInPort;
using class discsignals::DiscreteBundle;


class SolenoidValve:public PressureSource
{
	private:
		DiscInPort dipInput[2];
		PressureSource* psource;
		PressureSource* psourceinvent;

		bool NormallyClosed;

		double pos;
		double rate;
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
		 * @param imaxrate maximum valve motion rate
		 */
		SolenoidValve( double initpos, double rate, bool NormallyClosed, PressureSource* psource, PressureSource* psourceinvent );
		~SolenoidValve( void );

		void Connect( int input, DiscreteBundle* pBundle, int iLine );

		double Use( double flow );
};


#endif// _g_SolenoidValve_H_