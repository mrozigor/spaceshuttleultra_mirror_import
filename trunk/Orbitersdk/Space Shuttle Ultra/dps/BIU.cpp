#include "BIU.h"

namespace dps {

	BIU::BIU() {
		usTerminalID = 0xFFFF;
		bIsBusMaster = false;
		bDontListen = true;
	}

	BIU::BIU(unsigned short usTerminalID, bool bIsMaster) {
		this->usTerminalID = usTerminalID;
		bIsBusMaster = bIsMaster;
		bDontListen = true;
	}

	BIU::~BIU() {
	}

	void BIU::OnNewMessage() {
	}

	void BIU::SendFloat(unsigned short usTarget, float fValue) {
	}

	void BIU::SendWord(unsigned short usTarget, short sWord) {
	}
};