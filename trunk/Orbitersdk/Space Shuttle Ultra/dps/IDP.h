#if !defined(___SPACE_SHUTTLE_ULTRA_DPS_IDP_H_INCLUDED___)
#define ___SPACE_SHUTTLE_ULTRA_DPS_IDP_H_INCLUDED___

#include <vector>
#include "../Keyboard.h"
#include "../vc/vc_defs.h"

namespace dps {	
	using class ::Keyboard;

	typedef enum {
		GNC = 0,
		SM = 1,
		PL = 2
	} MAJORFUNCTION;

	typedef struct {
		char cSymbol;
		char cAttr;
	} DEUCHAR;

	
	const char DEUATT_NORMAL = 0;
	const char DEUATT_OVERBRIGHT = 1;
	const char DEUATT_FLASHING = 2;
	const char DEUATT_UNDERLINED = 4;

	/**
	 * Implementation of the Integrated display processor. Each can deal with a infinite number of 
	 * MDUs. 
 	 */
	class IDP : public AtlantisSubsystem {
		vc::MDU* mdu_list[7];
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
		void ConnectToMDU(vc::MDU* pMDU, bool bPrimary = true);
		void ConnectToKeyboard(Keyboard* pKeyboardA, Keyboard* pKeyboardB);
	};
};

#endif