#include "Talkback.h"
#include "../Atlantis.h"
#include <cassert>

extern GDIParams g_Param;

namespace vc
{
	BasicTalkback::BasicTalkback(Atlantis* _sts, const std::string& _ident)
		: BasicVCComponent(_sts, _ident)
	{
		talkbackLocation.x=0;
		talkbackLocation.y=0;
		usHeight=0;
		usWidth=0;
	}

	BasicTalkback::~BasicTalkback()
	{
	}

	void BasicTalkback::SetTalkbackLocation(unsigned short x, unsigned short y)
	{
		talkbackLocation.x=x;
		talkbackLocation.y=y;
	}

	void BasicTalkback::SetDimensions(unsigned short _usWidth, unsigned short _usHeight)
	{
		usWidth=_usWidth;
		usHeight=_usHeight;
	}

	StandardTalkback::StandardTalkback(Atlantis *_sts, const std::string &_ident, unsigned short _usInputs)
		: BasicTalkback(_sts, _ident)
	{
		assert(_usInputs <= MAX_INPUTS);
		usInputs=_usInputs;
		// force talkback to be redrawn on first time step
		tkbk_state=-1;
	}

	StandardTalkback::~StandardTalkback()
	{
	}

	void StandardTalkback::OnPreStep(double SimT, double DeltaT, double MJD)
	{
		// loop through all input signals; make sure talkback is set correctly
		for(int i=0;i<usInputs;i++) {
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
		for(int i=0;i<usInputs;i++) {
			if(input[i]) {
				label=flags[i];
				break;
			}
		}
		return UpdateTalkback(surf, label);
	}

	void StandardTalkback::SetInput(unsigned short idx, DiscreteBundle* pBundle, unsigned short usLine, unsigned short usFlag)
	{
		input[idx].Connect(pBundle, usLine);
		flags[idx]=usFlag;
	}

	bool StandardTalkback::UpdateTalkback(SURFHANDLE surf, unsigned short label)
	{
		if(label==tkbk_state) return false;
		tkbk_state=label;

		if(usWidth == 32)
		{
			if(label<8) 
				oapiBlt(surf, g_Param.tkbk_label, talkbackLocation.x, talkbackLocation.y, label*32, 0, usWidth, usHeight);
			else 
				oapiBlt(surf, g_Param.tkbk_label, talkbackLocation.x, talkbackLocation.y, (label-8)*32, 18, usWidth, usHeight);
		}
		else if(usWidth == 50)
		{
			//sprintf_s(oapiDebugString(), 255, "StandardTalkback::UpdateTalkback");
			if(label<5) 
				oapiBlt(surf, g_Param.tkbk_label, talkbackLocation.x, talkbackLocation.y, label*50, 35, usWidth, usHeight);
			else 
				oapiBlt(surf, g_Param.tkbk_label, talkbackLocation.x, talkbackLocation.y, (label-5)*50, 65, usWidth, usHeight);
		}
		else // use StretchBlt equivalent
		{
			RECT tgt = _R(talkbackLocation.x, talkbackLocation.y, talkbackLocation.x+usWidth, talkbackLocation.y+usHeight);
			RECT src;
			if(label<8) src = _R(label*32, 0, (label+1)*32, 18);
			else src = _R((label-8)*32, 18, (label-7)*32, 36);

			oapiBlt(surf, g_Param.tkbk_label, &tgt, &src);
		}
		return true;
	}

	Std2SegTalkback::Std2SegTalkback(Atlantis* _sts, const std::string& _ident, unsigned short _usInputs)
		: BasicTalkback(_sts, _ident)
	{
		assert(_usInputs <= MAX_INPUTS);
		usInputs=_usInputs;
		usInactive=0;
		usInactiveFlag=TB_GRAY;
		// force talkback to be redrawn on first time step
		upper_tkbk_state=-1;
		lower_tkbk_state=-1;
	}

	Std2SegTalkback::~Std2SegTalkback()
	{
	}

	void Std2SegTalkback::OnPreStep(double SimT, double DeltaT, double MJD)
	{
		unsigned short tkbk_state;
		if(usInactive==0) tkbk_state=lower_tkbk_state;
		else tkbk_state=upper_tkbk_state;

		// loop through all input signals; make sure talkback is set correctly
		for(int i=0;i<usInputs;i++) {
			if(input[i]) {
				if(flags[i]!=tkbk_state) TriggerRedraw();
				return;
			}
		}
		// if none of the input signals are set, TB should display barberpole
		if(tkbk_state!=TB_BARBERPOLE) TriggerRedraw();
	}

	bool Std2SegTalkback::OnVCRedrawEvent(int id, int _event, SURFHANDLE surf)
	{
		unsigned short label=TB_BARBERPOLE;
		for(int i=0;i<usInputs;i++) {
			if(input[i]) {
				label=flags[i];
				break;
			}
		}
		if(usInactive==0) return UpdateTalkback(surf, usInactiveFlag, label);
		else return UpdateTalkback(surf, label, usInactiveFlag);
	}

	void Std2SegTalkback::SetInput(unsigned short idx, DiscreteBundle* pBundle, unsigned short usLine, unsigned short usFlag)
	{
		input[idx].Connect(pBundle, usLine);
		flags[idx]=usFlag;
	}

	void Std2SegTalkback::SetInactiveSegment(unsigned short _usSegment, unsigned short _usFlag)
	{
		usInactive=_usSegment;
		usInactiveFlag=_usFlag;
	}

	bool Std2SegTalkback::UpdateTalkback(SURFHANDLE surf, unsigned short upper_label, unsigned short lower_label)
	{
		if(upper_label==upper_tkbk_state && lower_label==lower_tkbk_state) return false;
		upper_tkbk_state=upper_label;
		lower_tkbk_state=lower_label;

		RECT tgt, src;
		//draw upper segment
		tgt=_R(0, 0, usWidth, usHeight/2);
		
		if(usWidth == 50)
		{
			if(upper_label<5) 
				src=_R(upper_label*50, 35, (upper_label+1)*50, 66);
			else 
				src=_R((upper_label-5)*50, 66, (upper_label-4)*50, 97);
		} 
		else {
			if(upper_label<8) src=_R(upper_label*32, 0, (upper_label+1)*32, 18);
			else src=_R((upper_label-8)*32, 18, (upper_label-7)*32, 36);
		}


		oapiBlt(surf, g_Param.tkbk_label, &tgt, &src);

		//draw lower segment
		tgt=_R(0, usHeight/2, usWidth, usHeight);

		if(usWidth == 50)
		{
			if(lower_label<5) 
				src=_R(lower_label*50, 35, (lower_label+1)*50, 66);
			else 
				src=_R((lower_label-5)*50, 66, (lower_label-4)*50, 97);
		} 
		else {
			if(lower_label<8) src=_R(lower_label*32, 0, (lower_label+1)*32, 18);
			else src=_R((lower_label-8)*32, 18, (lower_label-7)*32, 36);
		}
		oapiBlt(surf, g_Param.tkbk_label, &tgt, &src);
		
		return true;
	}
};