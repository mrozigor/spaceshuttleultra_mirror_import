#pragma once

namespace dps {
	class IDP;
	class GPC;
	class MMU;
	class MDM;
	class MDMIOModule;
	class MasterTimingUnit;
	class ShuttleBus;
	class TimeBus;
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

	/**
	 * @brief IRIG-B compatible time frame
	 * 
	 * Format used for transmitting GMT and MET over serial lines in
	 * the shuttle. One such frame requires 1 second to be transmitted,
	 * so millisecond timing comes from other time sources, such as the 
	 * pulses used for transmitting this frame.
	 * It takes half a second to send to actual time in 5 pulse coded bytes
	 * The other 5 bytes of the time are not used but get transmitted anyway.
	 */
	typedef struct {
		unsigned int uSeconds:7;
		unsigned int uMinutes:7;
		unsigned int uHours:6;
		unsigned int uDays:9;
		unsigned char uP5:8;
		unsigned char uP6:8;
		unsigned char uP7:8;
		unsigned char uP8:8;
		unsigned char uP9:8;
	} IRIGB_FRAME;

	

};