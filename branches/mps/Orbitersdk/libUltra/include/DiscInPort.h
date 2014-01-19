/****************************************************************************
  This file is part of Space Shuttle Ultra

  Discrete input port definition



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

#include "DiscretePort.h"

namespace discsignals {

	class DiscInPort: public DiscretePort {
	public:
		DiscInPort();
		virtual ~DiscInPort();

		bool IsSet() const;
		bool IsSet(float fThreshold) const;

		float GetVoltage() const;

		inline operator bool() const {
			return IsSet();
		};
	};

	class DiscDemultiplex16: public DiscMultiplex {
	protected:
		unsigned short usLatch;
	public:
		DiscInPort r[16];
	public:
		DiscDemultiplex16();
		virtual ~DiscDemultiplex16();
		unsigned short GetValue() const;
		unsigned short GetValueBCD() const;
		virtual void Propagate(double fSimT, double fDeltaT, double fMJD);
	};
};