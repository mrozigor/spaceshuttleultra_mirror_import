#pragma once

#include "dps_defs.h"

namespace dps {

	class BIU {
		unsigned short usReceiveBuffer[1024];
		unsigned short usTransmitBuffer[1024];
		ShuttleBus* pBus;
		bool bIsBusMaster;
		bool bDontListen;
		unsigned short usTerminalID;
	public:
		BIU();
		BIU(unsigned short usTerminalID, bool bIsMaster = false);
		virtual ~BIU();
		void Connect(ShuttleBus* pBus);
		void MakeBusMaster();
		void Listen(bool fEnable = true);
		virtual void OnNewMessage();
		virtual void SendFloat(unsigned short usTarget, float fValue);
		virtual void SendWord(unsigned short usTarget, short sValue);
		//virtual void Transmit();
	};
};