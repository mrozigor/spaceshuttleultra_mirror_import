#pragma once

#include "dps_defs.h"
#include <string>

class AtlantisSubsystem;

namespace dps {

	using namespace std;

	class BIU {
		AtlantisSubsystem* pParent;
		string label;
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
		void Label(const string& _label);
		const string& GetLabel() const;
		void SetParent(AtlantisSubsystem* pSubsys);
		AtlantisSubsystem* GetParent() const;
		void SetTerminalID(unsigned short TermID);
		void Init(AtlantisSubsystem* pSubsys, const string& _label, 
			unsigned short usTermID, bool bIsMaster);
		void ConnectTo(ShuttleBus* pBus);
		void MakeBusMaster();
		void Listen(bool fEnable = true);
		virtual void OnNewMessage();
		virtual void SendFloat(unsigned short usTarget, float fValue);
		virtual void SendWord(unsigned short usTarget, short sValue);
		//virtual void Transmit();
	};
};