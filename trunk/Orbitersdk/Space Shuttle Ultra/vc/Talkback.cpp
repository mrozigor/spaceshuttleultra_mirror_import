#include "Talkback.h"
#include "../Atlantis.h"

extern GDIParams g_Param;

namespace vc
{
	BasicTalkback::BasicTalkback(Atlantis* _sts, const std::string& _ident)
		: BasicVCComponent(_sts, _ident)
	{
		talkbackLocation.x=0;
		talkbackLocation.y=0;
	}

	BasicTalkback::~BasicTalkback()
	{
	}

	void BasicTalkback::SetTalkbackLocation(unsigned short x, unsigned short y)
	{
		talkbackLocation.x=x;
		talkbackLocation.y=y;
	}


	StandardTalkback::StandardTalkback(Atlantis *_sts, const std::string &_ident, unsigned short usFlags)
		: BasicTalkback(_sts, _ident)
	{
		usHeight=0;
		usWidth=0;
		numFlags=usFlags;
		// true for most textures
		tkbk_state=TB_BARBERPOLE;
	}

	StandardTalkback::~StandardTalkback()
	{
	}

	void StandardTalkback::OnPreStep(double SimT, double DeltaT, double MJD)
	{
		// loop through all input signals; make sure talkback is set correctly
		for(int i=0;i<numFlags;i++) {
			if(input[i]) {
				if(flags[i]!=tkbk_state) TriggerRedraw();
				return;
			}
		}
		// if none of the input signals are set, TB should display barberpole
		if(tkbk_state!=TB_BARBERPOLE) TriggerRedraw();
	}

	bool StandardTalkback::OnVCRedrawEvent(int id, int _event, SURFHANDLE surf)
	{
		unsigned short label=TB_BARBERPOLE;
		for(int i=0;i<numFlags;i++) {
			if(input[i]) {
				label=flags[i];
				break;
			}
		}
		return UpdateTalkback(surf, label);
	}

	void StandardTalkback::SetDimensions(unsigned short _usWidth, unsigned short _usHeight)
	{
		usWidth=_usWidth;
		usHeight=_usHeight;
	}

	bool StandardTalkback::UpdateTalkback(SURFHANDLE surf, unsigned short label)
	{
		if(label==tkbk_state) return false;
		tkbk_state=label;

		if(label<8) oapiBlt(surf, g_Param.tkbk_label, 0, 0, label*32, 0, 32, 18);
		else oapiBlt(surf, g_Param.tkbk_label, 0, 0, (label-8)*32, 18, 32, 18);
		return true;
	}
};