#ifndef __TALKBACK_H
#define __TALKBACK_H
#pragma once

#include "BasicVCComponent.h"
#include "../discsignals/DiscInPort.h"

namespace vc
{
	using namespace discsignals;

	class BasicTalkback : public BasicVCComponent
	{
	protected:
		POINT talkbackLocation;
	public:
		BasicTalkback(Atlantis* _sts, const std::string& _ident);
		virtual ~BasicTalkback();

		void SetTalkbackLocation(unsigned short x, unsigned short y);
	};

	class StandardTalkback : public BasicTalkback
	{
	protected:
		unsigned short usHeight, usWidth;
		unsigned short numFlags;
		unsigned short tkbk_state;

		bool UpdateTalkback(SURFHANDLE surf, unsigned short label);
	public:
		// maximum number of different flags which can be displayed by a single talkback
		static const int MAX_FLAGS = 3;

		StandardTalkback(Atlantis* _sts, const std::string& _ident, unsigned short usFlags);
		virtual ~StandardTalkback();

		virtual void OnPreStep(double SimT, double DeltaT, double MJD);
		virtual bool OnVCRedrawEvent(int id, int _event, SURFHANDLE surf);

		void SetDimensions(unsigned short _usWidth, unsigned short _usHeight);

		unsigned short flags[MAX_FLAGS];
		DiscInPort input[MAX_FLAGS];
	};
};

#endif //__TALKBACK_H