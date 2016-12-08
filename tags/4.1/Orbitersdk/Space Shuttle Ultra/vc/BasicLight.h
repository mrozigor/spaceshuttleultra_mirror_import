/****************************************************************************
  This file is part of Space Shuttle Ultra

  Basic light VC component definition



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

#pragma once
#include "AtlantisVCComponent.h"
#include "DiscreteBundle.h"

namespace vc {

	using discsignals::DiscreteBundle;


	class BasicLight:public AtlantisVCComponent {
	protected:

		POINT redrawBase;
	public:
		BasicLight(Atlantis* _sts, const string& _ident);
		virtual ~BasicLight();
		void SetBase(unsigned short usX, unsigned short usY);
		virtual bool Connect(unsigned short usPort, discsignals::DiscreteBundle* pBundle, unsigned short usLine);
		virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
		virtual bool OnVCRedrawEvent(int id, int _event, SURFHANDLE surf);
	};


};