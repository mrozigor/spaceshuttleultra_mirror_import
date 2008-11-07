#pragma once

#define SSU_KEY_FAULTSUMM 0
#define SSU_KEY_SYSSUMM 1
#define SSU_KEY_MSGRESET 2
#define SSU_KEY_ACK 3
#define SSU_KEY_GPCIDP 4
#define SSU_KEY_A 5
#define SSU_KEY_B 6
#define SSU_KEY_C 7
#define SSU_KEY_IORESET 8
#define SSU_KEY_D 9
#define SSU_KEY_E 10
#define SSU_KEY_F 11
#define SSU_KEY_ITEM 12
#define SSU_KEY_1 13
#define SSU_KEY_2 14
#define SSU_KEY_3 15
#define SSU_KEY_EXEC 16
#define SSU_KEY_4 17
#define SSU_KEY_5 18
#define SSU_KEY_6 19
#define SSU_KEY_OPS 20
#define SSU_KEY_7 21
#define SSU_KEY_8 22
#define SSU_KEY_9 23
#define SSU_KEY_SPEC 24
#define SSU_KEY_MINUS 25
#define SSU_KEY_0 26
#define SSU_KEY_PLUS 27
#define SSU_KEY_RESUME 28
#define SSU_KEY_CLEAR 29
#define SSU_KEY_DOT 30
#define SSU_KEY_PRO 31

namespace dps {
	class IDP;
	class GPC;
	class GNCSoftware;
	class MMU;
	class MDM;
	class MDMIOModule;
	class MasterTimingUnit;
	class ShuttleBus;
	class ShuttleBusManager;
	class TimeBus;
	class AP101S;
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

	typedef struct {
		unsigned int uSeconds:7;
		unsigned int uMinutes:7;
		unsigned int uHours:6;
		unsigned int uDays:9;
	} GPC_TIME_RECORD;

	typedef struct {
		unsigned char ucGPCID;
		unsigned short ucMajorFunction;
		short sOPS;
		short sSPEC;
		short sDISP;
		GPC_TIME_RECORD gpc_time;
		GPC_TIME_RECORD crt_time;
		short sDataPage;
		long lData[16];
	} DEU_COMMAND;

	typedef struct {
		unsigned char ucMajorFunction;
		unsigned long ulKeyMap;
		unsigned short usDataSize;
		long lData[32];
	} DEU_DATA;

	

};
