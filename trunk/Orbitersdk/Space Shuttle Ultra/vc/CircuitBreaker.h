/****************************************************************************
  This file is part of Space Shuttle Ultra

  Circuit Breaker VC Component



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
#ifndef __CIRCUITBREAKER_H
#define __CIRCUITBREAKER_H


#include "AtlantisVCComponent.h"
#include "DiscOutPort.h"


const double CB_MOVEMENT_RANGE = 0.015;// meters
const double CB_MOVEMENT_TIME = 0.3;// seconds


namespace vc
{
	using namespace discsignals;

	class CircuitBreaker:public AtlantisVCComponent
	{
		private:
			bool CBin;

			DiscOutPort output;

			UINT grpIndex;
			VECTOR3 dir;
			MGROUP_TRANSLATE* move;
			UINT anim_move;

			double t0;
			bool counting;

			void OnChange( bool _CBin );

		public:
			CircuitBreaker( Atlantis* _sts, const std::string& _ident );
			virtual ~CircuitBreaker();

			virtual bool OnParseLine( const char* line );
			virtual bool GetStateString( unsigned long ulBufferSize, char* pszBuffer );
			void DefineGroup( UINT _grpIndex );
			void DefineDirection( VECTOR3 _dir );
			void Connect( DiscreteBundle* pBundle, unsigned short usLine );
			virtual void DefineVCAnimations( UINT vc_idx );
			virtual bool OnMouseEvent( int _event, float x, float y );
	};
};

#endif// __CIRCUITBREAKER_H