#include "BIU.h"

namespace dps {

	BIU::BIU() 
		: label(), pParent(NULL)
	{
		usTerminalID = 0xFFFF;
		bIsBusMaster = false;
		bDontListen = true;
		
	}

	BIU::BIU(unsigned short usTerminalID, bool bIsMaster) 
		: label(), pParent(NULL)
	{
		this->usTerminalID = usTerminalID;
		bIsBusMaster = bIsMaster;
		bDontListen = true;
		
	}

	BIU::~BIU() {
	}

	void BIU::Label(const string& _label) {
		label = _label;
	}

	const string& BIU::GetLabel() const {
		return label;
	}

	void BIU::SetParent(AtlantisSubsystem* pSubsys) {
		pParent = pSubsys;
	}

	AtlantisSubsystem* BIU::GetParent() const {
		return pParent;
	}
	
	void BIU::SetTerminalID(unsigned short TermID) {
		usTerminalID = TermID;
	}

	void BIU::Init(AtlantisSubsystem* pSubsys, const string& _label, 
		unsigned short usTermID, bool bIsMaster) {

			SetParent(pSubsys);
			Label(_label);
			SetTerminalID(usTermID);
			bIsBusMaster = bIsMaster;
	}

	void BIU::OnNewMessage() {
	}

	void BIU::SendFloat(unsigned short usTarget, float fValue) {
	}

	void BIU::SendWord(unsigned short usTarget, short sWord) {
	}

	void BIU::ConnectTo(ShuttleBus* _pBus) {
		//char pszBuffer[255];
		pBus = _pBus;
		
	}
};