// ShuttleBus.cpp: Implementierung der Klasse ShuttleBus.
//
//////////////////////////////////////////////////////////////////////

#include "ShuttleBus.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

namespace dps {

	ShuttleBus::ShuttleBus(const string& _ident)
	{
	
	}

	ShuttleBus::~ShuttleBus()
	{
	
	}

	void ShuttleBus::Connect(BIU* pTarget) {
		connections.insert(pTarget);
	}

	const string& ShuttleBus::GetIdent() const {
		return ident;
	}

	long ShuttleBus::Transmit(unsigned short usTarget, unsigned short usNumData, unsigned short usData[]) {
		return 0;
	}

};
