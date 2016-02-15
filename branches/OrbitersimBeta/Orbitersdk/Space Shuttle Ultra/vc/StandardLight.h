/****************************************************************************
  This file is part of Space Shuttle Ultra

  Standard light VC component



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

#include "BasicLight.h"
#include "DiscInPort.h"



namespace vc {

	using discsignals::DiscInPort;

	class StandardLight: public BasicLight {
		SURFHANDLE shSource;
		POINT sourceOn;
		POINT sourceOff;
		unsigned short usWidth;
		unsigned short usHeight;

		bool bOldState;
		bool bCurrentState;
		bool bHasSourceImage;
	public:
		StandardLight(Atlantis* _sts, const string& _ident);
		~StandardLight();
		void SetSourceImage(SURFHANDLE _shSource);
		void SetDimensions(unsigned short usWidth, unsigned short usHeight);
		void SetSourceCoords(bool fState, unsigned short usX, unsigned short usY);
//		virtual bool Connect(unsigned short usPort, ::discsignals::DiscreteBundle* pBundle, unsigned short usLine);
		virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
		virtual bool OnVCRedrawEvent(int id, int _event, SURFHANDLE surf);
		/**
		 * Connect light to specifed port.
		 * \param usPort 0 to connect input port, 1 to connect test port
		 */
		virtual bool Connect(unsigned short usPort, discsignals::DiscreteBundle* pBundle, unsigned short usLine);

		DiscInPort input;
		DiscInPort test;
	};
}
