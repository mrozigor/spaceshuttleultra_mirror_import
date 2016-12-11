/****************************************************************************
  This file is part of Space Shuttle Ultra

  Keyboard VC Component



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
#ifndef __KEYBOARD_H
#define __KEYBOARD_H


#include "AtlantisVCComponent.h"
#include "../dps/IDP.h"


namespace vc
{
	class Keyboard:public AtlantisVCComponent
	{
			int ID;

			dps::IDP* pIDP[2];
		public:
			Keyboard( Atlantis* _sts, const std::string& _ident, int ID );
			virtual ~Keyboard();

			void ConnectIDP( int num, dps::IDP* p_idp );

			virtual bool OnMouseEvent( int _event, float x, float y );
	};
};

#endif //__TALKBACK_H