/****************************************************************************
  This file is part of Space Shuttle Ultra

  Standard Linear Mechanical Meter VC Component definition



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
#ifndef __VC_STDLINEARMECHMETER_H
#define __VC_STDLINEARMECHMETER_H


#include "AtlantisVCComponent.h"
#include "BasicMechMeter.h"


namespace vc
{
	class StandardLinearMechMeter:public BasicMechMeter  
	{
		protected:
			MGROUP_TRANSLATE* needle_trans;
		public:
			StandardLinearMechMeter( Atlantis* _sts, const string& _ident );
			virtual ~StandardLinearMechMeter();

			virtual void DefineVCAnimations( UINT vc_idx );

			virtual void OnPostStep( double SimT, double DeltaT, double MJD );
	};
}

#endif// __VC_STDLINEARMECHMETER_H
