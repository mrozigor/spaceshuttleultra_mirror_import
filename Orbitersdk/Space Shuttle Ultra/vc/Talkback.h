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

#include "BasicVCComponent.h"
#include "DiscInPort.h"

namespace vc
{
	using namespace discsignals;

	class BasicTalkback : public AtlantisVCComponent
	{
	protected:
		unsigned short usHeight, usWidth;
		POINT talkbackLocation;
	public:
		BasicTalkback(Atlantis* _sts, const std::string& _ident);
		virtual ~BasicTalkback();

		void SetTalkbackLocation(unsigned short x, unsigned short y);
		void SetDimensions(unsigned short _usWidth, unsigned short _usHeight);
	};

	class StandardTalkback : public BasicTalkback
	{
	protected:
		unsigned short usInputs;
		unsigned short tkbk_state;

		bool UpdateTalkback(SURFHANDLE surf, unsigned short label);
	public:
		// maximum number of different inputs to talkback
		static const int MAX_INPUTS = 2;

		StandardTalkback(Atlantis* _sts, const std::string& _ident, unsigned short _usInputs=MAX_INPUTS);
		virtual ~StandardTalkback();

		virtual void OnPreStep(double SimT, double DeltaT, double MJD);
		virtual bool OnVCRedrawEvent(int id, int _event, SURFHANDLE surf);

		void SetInput(unsigned short idx, DiscreteBundle* pBundle, unsigned short usLine, unsigned short usFlag);

		unsigned short flags[MAX_INPUTS];
		DiscInPort input[MAX_INPUTS];
	};

	/**
	 * Talkback split into two segments. One segment is inactive and will always display the same flag.
	 * In most cases, upper segment is always GRAY; lower segment is dependent on inputs.
	 */
	class Std2SegTalkback : public BasicTalkback
	{
	protected:
		unsigned short usInputs;
		unsigned short usInactive, usInactiveFlag;
		unsigned short upper_tkbk_state, lower_tkbk_state;

		bool UpdateTalkback(SURFHANDLE surf, unsigned short upper_label, unsigned short lower_label);
	public:
		// maximum number of different inputs to talkback
		static const int MAX_INPUTS = 3;

		Std2SegTalkback(Atlantis* _sts, const std::string& _ident, unsigned short _usInputs=MAX_INPUTS);
		virtual ~Std2SegTalkback();

		virtual void OnPreStep(double SimT, double DeltaT, double MJD);
		virtual bool OnVCRedrawEvent(int id, int _event, SURFHANDLE surf);

		void SetInput(unsigned short idx, DiscreteBundle* pBundle, unsigned short usLine, unsigned short usFlag);
		void SetInactiveSegment(unsigned short _usSegment, unsigned short _usFlag);

		unsigned short flags[MAX_INPUTS];
		DiscInPort input[MAX_INPUTS];
	};
};

#endif //__TALKBACK_H