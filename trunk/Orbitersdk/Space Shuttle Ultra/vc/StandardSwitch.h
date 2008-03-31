// StandardSwitch.h: Schnittstelle für die Klasse StandardSwitch.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STANDARDSWITCH_H__A87D233D_E5CF_4343_90D8_BF65F16C4DB4__INCLUDED_)
#define AFX_STANDARDSWITCH_H__A87D233D_E5CF_4343_90D8_BF65F16C4DB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasicSwitch.h"

namespace vc {

class StandardSwitch : public BasicSwitch  
{
	UINT grpIndex;
	MGROUP_ROTATE* pswitchrot;

	UINT anim_switch;
	
public:
	StandardSwitch(Atlantis* _sts);
	virtual ~StandardSwitch();

	void DefineSwitchGroup(UINT _grpIndex);

	virtual void DefineVCAnimations(UINT vc_idx);

	virtual void Realize();

};

};

#endif // !defined(AFX_STANDARDSWITCH_H__A87D233D_E5CF_4343_90D8_BF65F16C4DB4__INCLUDED_)
