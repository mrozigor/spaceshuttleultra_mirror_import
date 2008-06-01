#pragma once

namespace dps {
	class IDP;
	class GPC;
	class MMU;
	class MDM;
	class MDMIOModule;
	class MasterTimingUnit;
	class ShuttleBus;
	/**
	 * Connection between Shuttle Bus and subsystems.
	 */
	class BIU;
	/**
	 * Class representing a random access memory with 16 bit words. 
	 */
	class RAM16;

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