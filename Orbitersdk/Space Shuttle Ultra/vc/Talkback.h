/****************************************************************************
  This file is part of Space Shuttle Ultra

  Talkback VC Component



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
#ifndef __TALKBACK_H
#define __TALKBACK_H
#pragma once

#include "AtlantisVCComponent.h"
#include "DiscInPort.h"

namespace vc
{
	using namespace discsignals;

	const float TALKBACK_U_OFFSET[19] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.250980f, 0.250980f, 0.0f, 0.250980f, 0.250980f, 0.250980f, 0.250980f, 0.250980f, 0.501961f, 0.501961f, 0.501961f, 0.501961f, 0.501961f};
	const float TALKBACK_V_OFFSET[19] = {0.0f, 0.564706f, 0.282353f, 0.705882f, 0.847059f, 0.423529f, 0.0f, 0.141176f, 0.141176f, 0.282353f, 0.423529f, 0.564706f, 0.705882f, 0.847059f, 0.0f, 0.141176f, 0.282353f, 0.423529f, 0.564706f};

	class BasicTalkback : public AtlantisVCComponent
	{
		protected:
			UINT panelmesh;
			unsigned short usInactiveFlag;
			unsigned short tkbk_default_state;

			void SoundOff( void ) const;
			void SoundOn( void ) const;

		public:
			BasicTalkback(Atlantis* _sts, const std::string& _ident);
			virtual ~BasicTalkback();

			void SetInactiveSegment( unsigned short _usFlag );
	};

	class StandardTalkback2 : public BasicTalkback
	{
		protected:
			unsigned short tkbk_state_U;
			unsigned short tkbk_state_L;
			unsigned short tkbk_next_state_U;
			unsigned short tkbk_next_state_L;
			unsigned short usCover;
			UINT grpIndex_U;
			UINT grpIndex_L;

			void UpdateUV( void );

		public:
			StandardTalkback2( Atlantis* _sts, const std::string& _ident );
			virtual ~StandardTalkback2();

			virtual void OnPostStep(double SimT, double DeltaT, double MJD);

			void DefineMeshGroups( UINT _panelmesh, UINT _grpIndex_U, UINT _grpIndex_L );
			void SetInitialState( unsigned short _usFlag );
			void SetInput( DiscreteBundle* pBundle, unsigned short usLine, unsigned short usFlag );
			void SetCoverSegment( unsigned short _usFlag );
			virtual void VisualCreated( void );

			unsigned short flags;
			DiscInPort input;
	};

	class StandardTalkback3 : public BasicTalkback
	{
		protected:
			unsigned short tkbk_state;
			unsigned short tkbk_next_state;
			UINT grpIndex;

			void UpdateUV( void );

		public:
			StandardTalkback3( Atlantis* _sts, const std::string& _ident );
			virtual ~StandardTalkback3();

			virtual void OnPostStep(double SimT, double DeltaT, double MJD);

			void DefineMeshGroup( UINT _panelmesh, UINT _grpIndex );
			void SetInitialState( unsigned short _usFlag );
			void SetInput( unsigned short idx, DiscreteBundle* pBundle, unsigned short usLine, unsigned short usFlag );
			virtual void VisualCreated( void );

			unsigned short flags[2];
			DiscInPort input[2];
	};
};

#endif //__TALKBACK_H