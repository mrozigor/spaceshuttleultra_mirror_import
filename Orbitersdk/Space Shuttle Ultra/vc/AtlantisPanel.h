/****************************************************************************
  This file is part of Space Shuttle Ultra

  Basic panel definitions



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
#ifndef ___BASIC_PANEL_H_INCLUDED___
#define ___BASIC_PANEL_H_INCLUDED___

#include "OrbiterAPI.h"
#include <set>
#include <string>
#include <vector>
#include <BasicPanel.h>
#include "vc_defs.h"
//#include "../ISubsystem.h"

class Atlantis;

namespace vc 
{

using namespace std;

//class BasicVCComponent;
class BasicSwitch;

class AtlantisPanel : public BasicPanel<Atlantis>
{
public:
	AtlantisPanel(Atlantis* _sts, const std::string& _name) : BasicPanel(_sts, _name) {};
	virtual ~AtlantisPanel() {};
	Atlantis* STS() const { return V(); };
};

};

#endif