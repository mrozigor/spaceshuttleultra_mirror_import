/****************************************************************************
  This file is part of Space Shuttle Ultra

  MPS defs definition



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
#ifndef _mps_defs_H_
#define _mps_defs_H_


#include "orbitersdk.h"


//#define _MPSDEBUG


namespace mps
{
	inline unsigned short GetMaskVal( unsigned short in, unsigned short mask )
	{
		return in & mask;
	}

	inline bool GetBoolFromInt( int var )
	{
		return var != 0;
	}
}


#endif// _mps_defs_H_
