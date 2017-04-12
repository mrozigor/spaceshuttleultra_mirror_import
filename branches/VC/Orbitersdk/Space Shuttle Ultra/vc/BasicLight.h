/****************************************************************************
  This file is part of Space Shuttle Ultra

  Basic light VC component definition



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
#pragma once


#include "AtlantisVCComponent.h"
#include "DiscreteBundle.h"


namespace vc
{
	using namespace discsignals;

	class BasicLight:public AtlantisVCComponent
	{
		protected:
			int state;
			int next_state;
			int default_state;

		public:
			BasicLight( Atlantis* _sts, const string& _ident );
			virtual ~BasicLight();

			virtual void SetDefaultState( int _state );
			virtual void SetStateOffset( int _state, float _U, float _V );

			virtual bool ConnectLight( unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine );
	};
}