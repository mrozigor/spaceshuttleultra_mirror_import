#pragma once

#include <vector>
#include "../vc/vc_defs.h"
#include "../Keyboard.h"
#include "../discsignals/DiscInPort.h"

namespace dps {
	class IDP;
};

namespace dps {	
	using class ::Keyboard;
	using class discsignals::DiscInPort;
	
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
		unsigned short usIDPID;
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
		unsigned short usGPCDriver;
		unsigned short usSelectedFC;
		DiscInPort KeybSelectA;
		DiscInPort KeybSelectB;
		DiscInPort MajorFuncGNC;
		DiscInPort MajorFuncPL;
	public:
		IDP(SubsystemDirector* pDirect, const string& _ident);
		virtual ~IDP();
		void ConnectToMDU(vc::PMDU pMDU, bool bPrimary = true);
		void ConnectToKeyboard(Keyboard* pKeyboardA, Keyboard* pKeyboardB);
	};
};
