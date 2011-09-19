/****************************************************************************
  This file is part of Space Shuttle Ultra

  Basic VC component definitions



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
#ifndef ATLANTISVCCOMPONENT_H_33CAE525_BC21_457E_A51D_83C566AFFE0A
#define ATLANTISVCCOMPONENT_H_33CAE525_BC21_457E_A51D_83C566AFFE0A

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vc_defs.h"
#include "BasicPanel.h"
//#include "../ISubsystem.h"
#include <BasicVCComponent.h>
#include <string>

class Atlantis;

namespace vc {

	using namespace std;
	using class ::Atlantis;

class AtlantisVCComponent : public BasicVCComponent<Atlantis>
{
public:
	AtlantisVCComponent(Atlantis* _sts) : BasicVCComponent(_sts) {};
	AtlantisVCComponent(Atlantis* _sts, const std::string& _name) : BasicVCComponent(_sts, _name) {};
	virtual ~AtlantisVCComponent() {};
	Atlantis* STS() const { return V(); };
};


};

#endif // #define ATLANTISVCCOMPONENT_H_33CAE525_BC21_457E_A51D_83C566AFFE0A
