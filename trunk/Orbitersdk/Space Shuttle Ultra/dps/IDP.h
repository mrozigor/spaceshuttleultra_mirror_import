#if !defined(___SPACE_SHUTTLE_ULTRA_DPS_IDP_H_INCLUDED___)
#define ___SPACE_SHUTTLE_ULTRA_DPS_IDP_H_INCLUDED___

#include <vector>
#include "../Keyboard.h"
#include "MDU.h"

namespace dps {	
	using class ::Keyboard;
	class MDU;

	/**
	 * Implementation of the Integrated display processor. Each can deal with a infinite number of 
	 * MDUs. 
 	 */
	class IDP : public AtlantisSubsystem {
		MDU* mdu_list[7];
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
		void ConnectToMDU(MDU* pMDU, bool bPrimary = true);
		void ConnectToKeyboard(Keyboard* pKeyboardA, Keyboard* pKeyboardB);
	};
};

#endif