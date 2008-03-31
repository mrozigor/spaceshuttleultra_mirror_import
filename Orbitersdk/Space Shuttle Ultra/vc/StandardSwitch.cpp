// StandardSwitch.cpp: Implementierung der Klasse StandardSwitch.
//
//////////////////////////////////////////////////////////////////////

#include "StandardSwitch.h"
#include "../Atlantis.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace vc {

StandardSwitch::StandardSwitch(Atlantis* _sts)
: BasicSwitch(_sts)
{
	pswitchrot = NULL;
}

StandardSwitch::~StandardSwitch()
{
	if(pswitchrot)
		delete pswitchrot;
}


void StandardSwitch::Realize()
{

}

void StandardSwitch::DefineVCAnimations(UINT vc_idx)
{
	pswitchrot = new MGROUP_ROTATE(vc_idx, &grpIndex, 1, GetReference(), GetDirection(), (float)(45 * RAD));
	anim_switch = STS()->CreateAnimation(0.5);
	STS()->AddAnimationComponent(anim_switch, 0.0, 1.0, pswitchrot, NULL);
}

};