#if !defined (___SPACE_SHUTTLE_ULTRA_MDU_H_INCLUDED___)
#define ___SPACE_SHUTTLE_ULTRA_MDU_H_INCLUDED___

#include "../Atlantis.h"
#include "../AtlantisSubsystem.h"

namespace dps {

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

	class MDU: public AtlantisSubsystem
	{	
	public:
		typedef enum {
			MDU_ADI_AVVI,
			MDU_HSI_AMI,
			MDU_COMP_ADI_HSI,
			MDU_ADI,
			MDU_OMS_MPS,
			MDU_HYD_APU,
			MDU_SPI,
			MDU_PORT_SELECT,
			MDU_DPS,
			MDU_FAULTSUMM,
			MDU_CONFIGSTATUS,
			MDU_CST,
			MDU_MEMORYMGMT
		} MDU_MODE;
	protected:
		SURFHANDLE shBackGround;
		DEUCHAR textBuffer[51][26];
	public:
		MDU(SubsystemDirector* pdirect, const string& _ident);
		virtual ~MDU();
		bool PrintChar(unsigned short x, unsigned short y, DEUCHAR c);
		bool PrintString(unsigned short x, unsigned short y, char* pText, short sLength, char cAttr = DEUATT_NORMAL);
		DEUCHAR GetTextBuffer(unsigned short x, unsigned short y) const;
		char* GetEdgeKeyMenu(int iButton);
	};

};

#endif