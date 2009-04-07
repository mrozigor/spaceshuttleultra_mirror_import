#ifndef _VC_STANDARDROTARYSWITCH_H
#define _VC_STANDARDROTARYSWITCH_H
#pragma once

#include "BasicRotarySwitch.h"
#include "../discsignals/discsignals.h"

namespace vc
{
	using class ::discsignals::DiscreteBundle;
	using class ::discsignals::DiscOutPort;
	using class ::discsignals::DiscInPort;

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
};

#endif // _VC_STANDARDROTARYSWITCH_H