/****************************************************************************
  This file is part of Space Shuttle Ultra

  Standard Rotary Switch VC component



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
#ifndef _VC_STANDARDROTARYSWITCH_H
#define _VC_STANDARDROTARYSWITCH_H
#pragma once

#include "BasicRotarySwitch.h"
#include "discsignals.h"

namespace vc
{
	using ::discsignals::DiscreteBundle;
	using ::discsignals::DiscOutPort;
	using ::discsignals::DiscInPort;

	/**
	 * Rotary switch that sends single input signal to multiple possible output signals
	 */
	class RotaryDemuxSwitch: public BasicRotarySwitch
	{
	protected:
		vector<DiscOutPort> outputSignals;
		DiscInPort inputSignal;
		bool bInputConnected;
	public:
		RotaryDemuxSwitch(Atlantis* _sts, const std::string& _ident, unsigned short _usNumPositions);
		virtual ~RotaryDemuxSwitch();
		
		virtual void OnPositionChange(unsigned short usNewPosition);

		void ConnectInputSignal(DiscreteBundle* pBundle, unsigned short usLine);
		void ConnectOutputSignal(unsigned short usIndex, DiscreteBundle* pBundle, unsigned short usLine);
	};

	/**
	 * Rotary switch that sends single output signal from multiple possible input signals
	 */
	class StandardRotarySwitch:public BasicRotarySwitch
	{
		protected:
			vector<vector<DiscInPort>> inputSignals;
			vector<DiscOutPort> outputSignal;

		public:
			StandardRotarySwitch( Atlantis* _sts, const std::string& _ident, unsigned short _usNumPositionsunsigned, unsigned short _usNumMuxes );
			virtual ~StandardRotarySwitch();

			virtual void OnPreStep( double SimT, double DeltaT, double MJD );

			void ConnectInputSignal( unsigned short usMux, unsigned short usIndex, DiscreteBundle* pBundle, unsigned short usLine );
			void ConnectOutputSignal( unsigned short usMux, DiscreteBundle* pBundle, unsigned short usLine );
	};
};

#endif // _VC_STANDARDROTARYSWITCH_H