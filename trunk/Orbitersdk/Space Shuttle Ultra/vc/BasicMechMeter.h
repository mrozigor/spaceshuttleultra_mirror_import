/****************************************************************************
  This file is part of Space Shuttle Ultra

  Basic Mechanical Meter VC Component definition



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
#ifndef __VC_BASICMECHMETER_H
#define __VC_BASICMECHMETER_H


#include "AtlantisVCComponent.h"
#include "DiscInPort.h"


namespace vc
{
	using ::discsignals::DiscreteBundle;
	using ::discsignals::DiscInPort;

	class BasicMechMeter:public AtlantisVCComponent  
	{
		protected:
			UINT anim;

			UINT grpIndex;

			DiscInPort input;

			double motionrange;// degrees or meters

		public:
			BasicMechMeter( Atlantis* _sts, const string& _ident );
			virtual ~BasicMechMeter();

			void DefineNeedleGroup( UINT _grpIndex );
			void SetMotionRange( double range );

			void SetInput( DiscreteBundle* pBundle, unsigned short usLine );
	};
}

#endif// __VC_BASICMECHMETER_H
