/****************************************************************************
  This file is part of Space Shuttle Ultra

  Inertial Upper Stage MFD definition



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
#ifndef __SSUIUSMFD_H
#define __SSUIUSMFD_H


#include "MFDAPI.h"
#include "SSU_IUS.h"


class SSU_IUS_MFD:public MFD2
{
	private:
		SSU_IUS* iusvessel;

	public:
		SSU_IUS_MFD( DWORD w, DWORD h, VESSEL *v );
		~SSU_IUS_MFD( void );

		bool Update( oapi::Sketchpad *skp );
		static int MsgProc( UINT msg, UINT mfd, WPARAM wparam, LPARAM lparam );
};

#endif// __SSUIUSMFD_H
