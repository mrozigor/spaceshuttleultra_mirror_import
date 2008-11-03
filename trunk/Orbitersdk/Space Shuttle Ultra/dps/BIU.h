#pragma once

#include "dps_def.h"

namespace dps {

	class BIU {
		unsigned short usReceiveBuffer[1024];
		unsigned short usTransmitBuffer[1024];
		ShuttleBus* pBus;
		bool bIsBusMaster;
	public:
		BIU(bool bIsMaster = false);
		virtual ~BIU();
		void Connect(ShuttleBus* pBus);
		void MakeBusMaster();
		void Listen();
		virtual void OnNewMessage();
		virtual void SendFloat(float fValue);
		virtual void SendWord(short sValue);
		//virtual void Transmit();
	};
};