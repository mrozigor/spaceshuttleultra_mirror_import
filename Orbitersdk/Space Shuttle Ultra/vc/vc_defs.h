#if !defined(___SPACE_SHUTTLE_ULTRA_VC_DEFINITIONS_H_INCLUDED___)
#define ___SPACE_SHUTTLE_ULTRA_VC_DEFINITIONS_H_INCLUDED___
/**
 * Namespace for all VC related classes, representing visual objects 
 * and switches.
 * Define the public contents of the VC namespace.
 */
namespace vc {
	const int TB_BARBERPOLE = 0;
	const int TB_STO = 1;
	const int TB_LAT = 2;
	const int TB_CLO = 3;
	const int TB_OP = 4;
	const int TB_ON = 5;
	const int TB_OFF = 6;
	const int TB_X10 = 7;
	const int TB_GRAY = 8;
	const int TB_DPY = 9;
	const int TB_REL = 10;

	const int MDUID_CDR1 = 0;
	const int MDUID_CDR2 = 1;
	const int MDUID_PLT1 = 2;
	const int MDUID_PLT2 = 3;
	const int MDUID_CRT1 = 4;
	const int MDUID_CRT2 = 5;
	const int MDUID_CRT3 = 6;
	const int MDUID_CRT4 = 7;
	const int MDUID_MFD1 = 8;
	const int MDUID_MFD2 = 9;
	const int MDUID_AFD  = 10;


	class MDU;
	class BasicVCComponent;
	class BasicSwitch;
	class BasicPanel;
	class PanelGroup;
	class PanelF7;
	class DAPControl;

	typedef struct {
		float x, y;
	} VCMOUSEEVENTPOINT;

	typedef MDU* PMDU;
};

#endif
