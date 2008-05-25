#pragma once

#include <vector>
#include "../vc/vc_defs.h"
#include "../Keyboard.h"

namespace dps {
	class IDP;
};

namespace dps {	
	using class ::Keyboard;
	
	class IDP;

	
	
	const char DEUATT_NORMAL = 0;
	const char DEUATT_OVERBRIGHT = 1;
	const char DEUATT_FLASHING = 2;
	const char DEUATT_UNDERLINED = 4;

	/**
	 * Implementation of the Integrated display processor. Each can deal with a infinite number of 
	 * MDUs. 
 	 */
	class IDP : public AtlantisSubsystem {
		vc::PMDU mdu_list[7];
		MAJORFUNCTION majfunc;
		unsigned short usOPS;
		unsigned short usSPEC;
		unsigned short usDISP;
		Keyboard* pKeyboardA;
		Keyboard* pKeyboardB;
		bool bUseKeyboardA;
		char cScratchPadLine[120];
		short sScratchPadLength;
	public:
		IDP(SubsystemDirector* pDirect, const string& _ident);
		virtual ~IDP();
		void ConnectToMDU(vc::PMDU pMDU, bool bPrimary = true);
		void ConnectToKeyboard(Keyboard* pKeyboardA, Keyboard* pKeyboardB);
	};
};
