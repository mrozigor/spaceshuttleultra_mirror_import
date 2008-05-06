// StandardSwitch.cpp: Implementierung der Klasse StandardSwitch.
//
//////////////////////////////////////////////////////////////////////

#include "StandardSwitch.h"
#include "../Atlantis.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace vc {

StandardSwitch::StandardSwitch(Atlantis* _sts, unsigned short usNumPositions, const string& _ident)
: BasicSwitch(_sts, usNumPositions, _ident)
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
	//Don't add anything for Orbiter, use existing meshes
}

void StandardSwitch::DefineVCAnimations(UINT vc_idx)
{
	pswitchrot = new MGROUP_ROTATE(vc_idx, &grpIndex, 1, GetReference(), GetDirection(), (float)(45 * RAD));
	anim_switch = STS()->CreateAnimation(0.5);
	STS()->AddAnimationComponent(anim_switch, 0.0, 1.0, pswitchrot, NULL);
}

bool StandardSwitch::ConnectPort(unsigned short usPort, discsignals::DiscreteBundle *pBundle, unsigned short usLine)
{
	if(usPort == 0)
	{
		return input.Connect(pBundle, usLine);
	}
	return false;
}

StdSwitch2::StdSwitch2(Atlantis *_sts, const std::string &_ident)
: StandardSwitch(_sts, 2, _ident)
{
}

StdSwitch2::~StdSwitch2()
{
}

bool StdSwitch2::ConnectPort(unsigned short usPort, discsignals::DiscreteBundle *pBundle, unsigned short usLine)
{
	if(usPort == 1)
	{
		return outPort.Connect(pBundle, usLine);	
	} else {
		return StandardSwitch::ConnectPort(usPort, pBundle, usLine);
	}
}


StdSwitch3::StdSwitch3(Atlantis *_sts, const std::string &_ident)
: StandardSwitch(_sts, 3, _ident)
{
}

StdSwitch3::~StdSwitch3()
{
}

bool StdSwitch3::ConnectPort(unsigned short usPort, discsignals::DiscreteBundle *pBundle, unsigned short usLine)
{
	if(usPort == 1)
	{
		return outPortA.Connect(pBundle, usLine);	
	} else if (usPort == 2) {
		return outPortB.Connect(pBundle, usLine);
	} else {
		return StandardSwitch::ConnectPort(usPort, pBundle, usLine);
	}
}

};