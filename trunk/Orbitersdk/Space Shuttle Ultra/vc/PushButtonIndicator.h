/****************************************************************************
  This file is part of Space Shuttle Ultra

  Pushbutton/indicator VC component definition



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
#ifndef __PUSHBUTTONINDICATOR_H
#define __PUSHBUTTONINDICATOR_H
#pragma once

#include "../Atlantis.h"
#include "PushButton.h"
#include "StandardLight.h"
#include "DiscInPort.h"
#include "DiscOutPort.h"


namespace vc
{
	using discsignals::DiscOutPort;
	using discsignals::DiscInPort;

	class PushButtonIndicatorSingleLight : public StandardSingleLight
	{
			UINT anim_pb;
			double motionlength;
			MGROUP_TRANSLATE* pPushDown;
		public:
			PushButtonIndicatorSingleLight( Atlantis* _sts, const string& _ident );
			~PushButtonIndicatorSingleLight();

			virtual void DefineVCAnimations( UINT vc_idx );
			virtual bool OnMouseEvent( int _event, float x, float y );
			virtual void OnPress();
			virtual void OnRelease();
			void ConnectPushButton( DiscreteBundle* pBundle, unsigned short usLine );

			void SetMotionLength( double _motionlength );

			DiscOutPort output;
	};



	class PushButtonIndicatorDoubleLight : public StandardDoubleLight
	{
			UINT anim_pb;
			double motionlength;
			MGROUP_TRANSLATE* pPushDown;
		public:
			PushButtonIndicatorDoubleLight( Atlantis* _sts, const string& _ident );
			~PushButtonIndicatorDoubleLight();

			virtual void DefineVCAnimations( UINT vc_idx );
			virtual bool OnMouseEvent( int _event, float x, float y );
			virtual void OnPress();
			virtual void OnRelease();
			void ConnectPushButton( DiscreteBundle* pBundle, unsigned short usLine );

			void SetMotionLength( double _motionlength );

			DiscOutPort output;
	};
};

#endif //__PUSHBUTTONINDICATOR_H
