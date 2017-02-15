/****************************************************************************
  This file is part of Space Shuttle Ultra

  Basic Valve Definition



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
#ifndef _g_BasicValve_H_
#define _g_BasicValve_H_


#include "orbitersdk.h"
#include <string>


using namespace std;


/**
 * @brief	Implementation of the BasicValve class.
 */
class BasicValve
{
	protected:
		string name;

		double pos;
		double rate;

	public:
		/**
		 * Create a new valve
		 * @param name		valve name
		 * @param initpos	initial valve position
		 * @param rate		valve motion rate
		 */
		BasicValve( const string& name, double initpos, double rate );
		virtual ~BasicValve( void );

		/**
		 * Returns valve position
		 * @return valve position (range: 0 - closed, 1 - open)
		 */
		double GetPos( void ) const;

		virtual bool OnParseLine( const char* line );

		virtual void OnSaveState( FILEHANDLE scn ) const;

		virtual void OnPreStep( double fSimT, double fDeltaT, double fMJD );

		virtual void OnPostStep( double fSimT, double fDeltaT, double fMJD );
};


#endif// _g_BasicValve_H_


/*
// 1->open, 0->closed
// rate -> %/s
*/
