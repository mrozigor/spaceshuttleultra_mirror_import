#pragma once

#include "dps_def.h"

namespace dps {

	class BIU {
		RAM16* pRAMBuffer;
		ShuttleBus* pBus;
		bool bIsBusMaster;
	public:
		BIU(RAM16* pRAMBuffer, bool bIsMaster = false);
		virtual ~BIU();
		void ConnectToBus(ShuttleBus* pBus);
		void MakeBusMaster();
		void Listen();
		virtual void OnNewMessage();
	};
};