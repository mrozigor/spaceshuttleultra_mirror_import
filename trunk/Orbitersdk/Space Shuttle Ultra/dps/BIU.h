/****************************************************************************
  This file is part of Space Shuttle Ultra

  Bus Interface Unit definition



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

#include "dps_defs.h"
#include <string>

class AtlantisSubsystem;

namespace dps {

	using namespace std;

	class FastQueue {
		unsigned short usBuffer[1024];
		unsigned short usTailPtr;
		unsigned short usHeadPtr;
	public:
		FastQueue() {
			usTailPtr = 0;
			usHeadPtr = 0;
		};


		inline unsigned short front() const {
			return usBuffer[usHeadPtr];
		};

		inline void Pop() {
			if(usHeadPtr != usTailPtr) {
				usHeadPtr = (usHeadPtr + 1) % 1024;
			}
		};

		inline void Push(unsigned short usVal) {
			if((usTailPtr + 1) % 1024 != usHeadPtr) {
				usBuffer[usTailPtr] = usVal;
				usTailPtr = (usTailPtr + 1) % 1024;
			}
		};

		inline void Purge() {
			usTailPtr = 0;
			usHeadPtr = 0;
		};

		inline bool IsEmpty() const {
			return usTailPtr == usHeadPtr;
		}
	};

	class BIU {
		AtlantisSubsystem* pParent;
		string label;
		unsigned short usReceiveBuffer[1024];
		unsigned short usTransmitBuffer[1024];
		ShuttleBus* pBus;
		bool bIsBusMaster;
		bool bDontListen;
		unsigned short usTerminalID;
		void notifyReceived(unsigned long ulChannel);
		void executeTransmitProgram(unsigned long ulChannel);
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