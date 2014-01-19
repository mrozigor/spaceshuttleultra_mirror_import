/****************************************************************************
  This file is part of Space Shuttle Ultra

  Sensor Definition



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
#ifndef _g_Sensor_H_
#define _g_Sensor_H_


#include "DiscOutPort.h"


using class discsignals::DiscOutPort;
using class discsignals::DiscreteBundle;


class Sensor
{
	private:
		DiscOutPort dipOutput;

		double conversor;

		double MaxValue;
		double MinValue;
	public:
		Sensor( double MaxValue = 5000, double MinValue = 0 );
		~Sensor( void );

		void Connect( DiscreteBundle* pBundle, int iLine );

		void SetValue( double value );
};


#endif// _g_Sensor_H_
