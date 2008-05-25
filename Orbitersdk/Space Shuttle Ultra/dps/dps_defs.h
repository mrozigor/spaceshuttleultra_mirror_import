#pragma once

namespace dps {
	class IDP;
	class GPC;
	class MMU;
	class MDM;
	class MDMIOModule;
	class MasterTimingUnit;
	class ShuttleBus;

	typedef enum {
		GNC = 0,
		SM = 1,
		PL = 2
	} MAJORFUNCTION;

	typedef struct {
		char cSymbol;
		char cAttr;
	} DEUCHAR;

};