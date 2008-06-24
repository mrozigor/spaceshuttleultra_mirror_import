#ifndef _mps_SSME_H_
#define _mps_SSME_H_


#include <orbitersdk.h>
#include "..\AtlantisSubsystem.h"


namespace mps
{
	typedef enum _ENGINE_STATUS
	{
		// CHECKOUT
		CHECKOUT_STANDBY,
		CHECKOUT_COMPONENTCHECKOUT,
		// START PREPARATION
		STARTPREPARATION_PURGESEQUENCE1,
		STARTPREPARATION_PURGESEQUENCE2,
		STARTPREPARATION_PURGESEQUENCE3,
		STARTPREPARATION_PURGESEQUENCE4,
		STARTPREPARATION_ENGINEREADY,
		// START/MAINSTAGE
		STARTMAINSTAGE_STARTPHASESTARTINITIATION,
		STARTMAINSTAGE_STARTPHASETHRUSTBUILDUP,
		STARTMAINSTAGE_MAINSTAGEPHASENORMALCONTROL,
		STARTMAINSTAGE_FIXEDDENSITY,
		STARTMAINSTAGE_THRUSTLIMITING,
		STARTMAINSTAGE_ELECTRICALLOCKUP,
		STARTMAINSTAGE_HYDRAULICLOCKUP,
		// SHUTDOWN
		SHUTDOWN_THROTTLINGTOZEROTHRUST,
		SHUTDOWN_PROPELLANTVALVESCLOSED,
		SHUTDOWN_FAILSAFEPNEUMATIC,
		// POST SHUTDOWN
		POSTSHUTDOWN_STANDBY,
		POSTSHUTDOWN_OXIDIZERDUMP,
		POSTSHUTDOWN_TERMINATESEQUENCE,
		// PROM
		PROM_STANDBY
	} ENGINE_STATUS;


	typedef enum _DCU
	{
		DCU_A,
		DCU_B
	} DCU;

	typedef struct _DATA_TABLE
	{
		double timeTAG;// simtime
		ENGINE_STATUS STATUS;

		double cmdPC;// inside table??????
		bool StartEnable;// inside table??????
		bool ShutdownEnable;// inside table??????

		double powerlevel;// % RPL

		double posOPOV;// 1 open
		double posFPOV;// 1 open
		double posMFV;// 1 open
		double posMOV;// 1 open
		double posCCV;// 1 open

		// TODO PC % and psia
		/*double pressMCCbri_1;
		double pressMCCbri_2;

		double MR;// O/F

		double pressMCClincav;// R

		double FFM;
		double rpmLPFTP;// rpm
		double rpmHPFTP;// rpm
		double tempHPFTdis_chA;// R
		double tempHPFTdis_chB;// R
		double pressHPFTPdis;// ? psia
		double pressHPFTPcoolin;// ? psia
		double accHPFTPrad;
		double pressHPFTPbalcav;// ? psia

		double tempHEXbypmix;// R
		double dpressHEXven;// ? psid
		double rpmLPOTP;// rpm
		double rpmHPOTP;// rpm
		double tempHPOTdis_chA;// R
		double tempHPOTdis_chB;// R
		double pressHPOTPintseal;// psia
		double pressHPOTPsecseal;// psia
		double pressHPOTPdis;// ? psia
		double pressPBPdis;// ? psia
		double accPBPrad;
		double tempPBPbeacoo;// R
		double pressLPOTPdis;// ? psia*/

		double timeESC;// simtime
		double timeCO;// simtime
	} DATA_TABLE;

	class SSME:public AtlantisSubsystem
	{
	protected:
		VESSEL *OV;
		int ID;

		// internal data
		THRUSTER_HANDLE thSSME;

		double COtimecoef;
		double OPOV_pos_CO;
		double FPOV_pos_CO;
		double PSN4time;// temp

		int MPL;// %
		int FPL;// %

		ENGINE_STATUS cmdSTATUS;
		DATA_TABLE* SSME_DATA_TABLE;
		
		DCU activeDCU;// inside table??????

		void Ignition( double );
		void Shutdown( double );
		void Throttling( double );
		void SetCOTime( void );


		double PCfromOSFStoSTS( double );
		double PCfromSTStoOSFS( double );
		double PCfromPCTtoPSI( double );

		// data cookup fncts
		virtual double dcPC_ESC( double ) = 0;
		virtual double dcMOV_ESC( double ) = 0;
		virtual double dcMFV_ESC( double ) = 0;
		virtual double dcCCV_ESC( double ) = 0;
		virtual double dcFPOV_ESC( double ) = 0;
		virtual double dcOPOV_ESC( double ) = 0;
		
		virtual double dcPC_CO( double ) = 0;
		virtual double dcMOV_CO( double ) = 0;
		virtual double dcMFV_CO( double ) = 0;
		virtual double dcCCV_CO( double ) = 0;
		virtual double dcFPOV_CO( double, double ) = 0;
		virtual double dcOPOV_CO( double, double ) = 0;

		virtual double dcPC_MS( double ) = 0;
		virtual double dcCCV_MS( double ) = 0;
		virtual double dcFPOV_MS( double ) = 0;
		virtual double dcOPOV_MS( double ) = 0;	

	public:
		// EIU only
		// command
		bool cmdPurgeSequence1( void );
		bool cmdPurgeSequence2( void );
		bool cmdPurgeSequence3( void );
		bool cmdPurgeSequence4( void );
		bool cmdStart( void );
		virtual bool cmdThrottle( double ) = 0;
		bool cmdShutdown( void );
		bool cmdStartEnable( void );
		bool cmdShutdownEnable( void );
		bool cmdOxidizerDump( void );
		bool cmdTerminateSequence( void );
		
		// data
		int dataDataTable( DATA_TABLE* );

		SSME( SubsystemDirector*, const string&, int, VESSEL*, PROPELLANT_HANDLE, VECTOR3, VECTOR3, double, double, double );
		~SSME( void );

		// heart beat
		void OnPostStep( double, double, double );
		//void OnPreStep( double, double, double );
		//void OnPropagate( double, double, double );

		virtual void OnSaveState( FILEHANDLE ) const = 0;
		virtual bool OnParseLine( const char* ) = 0;
		};
}


#endif// _mps_SSME_H_

/*
***SSME
0.200 sec delay cmd/PC
generate data table
send data table to EIU
don't do anything on your own
respond to MPS ENGINE POWER -> OFF (R2)
*/

/*
EIU:: converts ME-1 to 0, ME-2 to 1, ME-3 to 2 for use in SSME::
*/

/*
redlines
>>>CO only if MAIN ENGINE LIMIT SHUT DN -> ENABLE (AUTO) (C3)
HPFTP dis temp > 1960R
HPOTP dis temp > 1760R
HPOTP int seal purge press < 170psia (phase ii), 159psia (block i/ii)
HPOTP sec seal purge press > 100psia (phase ii only)
HPFTP cool liner press > DCU calc (~3675psig @ 104)
current PC 400psig below ref PC
*/


/*
HPFTP shaft speed
HPFT discharge temperature channel A
HPFT discharge temperature channel B
HPFTP coolant liner pressure
LPFTP shaft speed
HPFTP balance cavity pressure

HPOTP discharge pressure
HPOTP intermediate seal purge pressure
HPOTP secondary seal drain pressure
PBP discharge pressure
PBP radial accelerometer
PBP bearing coolant temperature
HPOT discharge temperature channel A
HPOT discharge temperature channel B
LPOTP discharge pressure

MCC pressure bridge 1,2
MCC liner cavity pressure
HEX bypass mix temperature
HEX venturi delta pressure
*/

/*
HEX discharge pressure
HEX interface temperature

LPFT discharge pressure
LPFT discharge temperature
FPB chamber pressure

HPFT discharge pressure
HPOT discharge pressure

MCC coolant discharge temperature
MCC coolant discharge pressure
MCC pressure

HPFTP discharge pressure
HPFTP radial accelerometer
HPFTP axial accelerometer

HPOP speed
HPFP speed
*/

///////////////////
// engine data
///////////////////

/*
B IIA
PC 109% 3008psia
PC ???% 2871psia
PC 104.5% 2865psia

104.5%
LPOTP 5050rpm
LPFTP 15400rpm
HPOTP  22220rpm
HPFTP  34360rpm
MR 6.032/1
*/

/*
B II
67-109
PC 109% 2994psia
*/

/*
Phase II

*model in data
power level 1.03999 0.0
mr 6.011 0.0
LPFP inlet pr 30.0 0.0
LPOP inlet pr 100.0 0.0
LPFP inlet tmp 37.0 0.0
LPOP inlet tmp 164.0 0.0
fuel repress flow 0.728 0.0
ox repress flow 1.612 0.0
chamber pressure command 3126.2 0.0

*eng perf
vacuum thrust 492035.0 983.06
engine flow 1089.51 3.11868
engine ox flow 934.115 2.67395
engine fuel flow 155.399 0.44484
vehicle isp 452.581 0.64378
engine MR-w/pres flow 6.01109 0.0
MCC c-star mult 1.00056 0.00221

*key op param
HPFT T/D tmp average 1641.05 46.6774
HPOT T/D tmp average 1354.61 36.3918
FPB chamber pr 5234.98 42.7615
OPB chamber pr 5299.15 37.3274
HPFT speed 35231.2 204.051
HPOT speed 28165.0 355.67
LPFT speed 15953.7 318.941
LPOT speed 5161.29 60.8609
HPFP disch pr 6418.12 34.8877
HPOP disch pr 4317.02 60.9193
HPFP inlet pr 260.81 15.8503
HPOP inlet pr 383.517 7.98399
HPFP disch tmp 95.2211 1.6578
HPOP disch tmp 192.728 1.11614
HPFP inlet tmp 42.7938 0.2752
HPOP inlet tmp 169.801 0.21168
OPOV position 0.685252 0.0133
FPOV position 0.762557 0.00997
PBP disch pr 7388.36 121.458
PBP disch tmp 206.023 1.7864
LPFT inlet pr 4651.61 48.1437
LPFT inlet tmp 461.257 19.1996
main inj HG orif in pr 3366.87 31.3215
main inj Ox inlet pr 3720.01 24.3633
main inj end Pr 3126.2 0.0
*/