/****************************************************************************
  This file is part of Space Shuttle Ultra

  Centaur G/G-Prime MFD definition



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
#ifndef __SSUSCMFD_H
#define __SSUSCMFD_H


#include "MFDAPI.h"
#include "SSU_Centaur.h"


class SSU_Centaur_MFD:public MFD2
{
	private:
		SSU_Centaur* centaurvessel;

	public:
		SSU_Centaur_MFD( DWORD w, DWORD h, VESSEL *v );
		~SSU_Centaur_MFD( void );

		bool Update( oapi::Sketchpad *skp );
		static int MsgProc( UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam );
};

#endif// __SSUSCMFD_H
