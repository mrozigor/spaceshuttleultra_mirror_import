#include "StandardRotarySwitch.h"

namespace vc
{
	RotaryDemuxSwitch::RotaryDemuxSwitch(Atlantis *_sts, const std::string &_ident, unsigned short _usNumPositions)
		: BasicRotarySwitch(_sts, _usNumPositions, _ident), outputSignals(_usNumPositions),
		bInputConnected(false)
	{
	}

	RotaryDemuxSwitch::~RotaryDemuxSwitch()
	{
	}

	void RotaryDemuxSwitch::ConnectInputSignal(discsignals::DiscreteBundle *pBundle, unsigned short usLine)
	{
		inputSignal.Connect(pBundle, usLine);
		bInputConnected=true;
	}

	void RotaryDemuxSwitch::ConnectOutputSignal(unsigned short usIndex, discsignals::DiscreteBundle *pBundle, unsigned short usLine)
	{
		outputSignals.at(usIndex).Connect(pBundle, usLine);
	}

	void RotaryDemuxSwitch::OnPositionChange(unsigned short usNewPosition)
	{
		BasicRotarySwitch::OnPositionChange(usNewPosition);

		bool bOutput=true;
		if(bInputConnected) bOutput=inputSignal.IsSet();

		for(unsigned short i=0;i<usNumPositions;i++) {
			if(i==usNewPosition && bOutput) outputSignals.at(i).SetLine();
			else outputSignals.at(i).ResetLine();
		}
	}
};