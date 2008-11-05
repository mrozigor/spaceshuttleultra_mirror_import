// ShuttleBus.cpp: Implementierung der Klasse ShuttleBus.
//
//////////////////////////////////////////////////////////////////////

#include <OrbiterAPI.h>
#include "ShuttleBus.h"
#include "BIU.h"
#include "../AtlantisSubsystem.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace dps {

	ShuttleBus::ShuttleBus(const string& _ident)
		:ident( _ident)
	{
		
	}

	ShuttleBus::~ShuttleBus()
	{
	
	}

	void ShuttleBus::ConnectTo(BIU* pTarget) {
		char pszBuffer[255];
		char pszBIABuffer[255];
		pTarget->ConnectTo(this);
		connections.insert(pTarget);
		if(pTarget->GetParent() != NULL) {
			sprintf_s(pszBIABuffer, 255, "%s-%s", 
				pTarget->GetParent()->GetQualifiedIdentifier().c_str(),
				pTarget->GetLabel().c_str());
		} else {
			sprintf_s(pszBIABuffer, 255, "%s", 
				pTarget->GetLabel().c_str());
		}
		sprintf_s(pszBuffer, 255, "ShuttleBus::ConnectTo(): Connect bus \"%s\" to \"%s\"",
			GetIdent().c_str(),
			pszBIABuffer);
		oapiWriteLog(pszBuffer);		
	}

	const string& ShuttleBus::GetIdent() const {
		return ident;
	}

	long ShuttleBus::Transmit(unsigned short usTarget, unsigned short usNumData, unsigned short usData[]) {
		return 0;
	}


	ShuttleBusManager::ShuttleBusManager() {
	}

	ShuttleBusManager::~ShuttleBusManager() {
		vector<ShuttleBus*>::iterator iter = busses.begin();
		while(iter != busses.end()) {
			delete (*iter);
			iter++;
		}
	}

	ShuttleBus* ShuttleBusManager::CreateBus(const string& _ident) {
		char pszBuffer[255];
		ShuttleBus* m_r = new ShuttleBus(_ident);
		busses.push_back(m_r);
		sprintf_s(pszBuffer, 255, "ShuttleBusManager::Create bus \"%s\"",
			_ident.c_str());
		oapiWriteLog(pszBuffer);
		return m_r;
	}

	ShuttleBus* ShuttleBusManager::FindBus(const string& _ident) const {
		vector<ShuttleBus*>::const_iterator iter = busses.begin();
		while(iter != busses.end()) {
			if((*iter)->GetIdent() == _ident) {
				return ( *iter );
			}
			iter++;
		}
		return NULL;
	}

	ShuttleBus* ShuttleBusManager::GetBus(const std::string &_ident) {
		ShuttleBus* m_r = FindBus(_ident);
		if(m_r != NULL) {
			return m_r;
		} else {
			return CreateBus(_ident);
		}
	}

};
