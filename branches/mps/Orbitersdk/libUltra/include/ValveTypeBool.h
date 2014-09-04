/****************************************************************************
  This file is part of Space Shuttle Ultra

  Boolean Valve class



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
#ifndef _g_ValveTypeBool_H_
#define _g_ValveTypeBool_H_


#include <BasicValve.h>


/**
 * @brief	Implementation of the ValveTypeBool class.
 * 
 * @deprecated	ValveTypeBool has been replaced by SolenoidValve, 
 * PressureActuatedValve and HydraulicActuatedValve.
 */
class ValveTypeBool
{
private:
	BasicValve* vlv;
	bool pos;
public:
	/**
	 * Opens valve
	 */
	bool Open( void );

	/**
	 * Closes valve
	 */
	bool Close( void );

	/**
	 * Returns valve position
	 * @return valve position (ON/OFF)
	 */
	bool GetPos( void ) const;

	/**
	 * Updates valve position (call from time step functions)
	 * @param dtme sim dt
	 */
	void tmestp( double dtme );

	/**
	 * Use from .scn loading function to set valve position
	 * @param ipos valve position
	 */
	void _backdoor( bool ipos );

	/**
	 * Create a new valve (only shows ON/OFF states)
	 * @param initpos initial valve position
	 * @param imaxrate maximum valve motion rate
	 */
	ValveTypeBool( bool initpos, double imaxrate );
	~ValveTypeBool( void );
};


#endif// _g_ValveTypeBool_H_
