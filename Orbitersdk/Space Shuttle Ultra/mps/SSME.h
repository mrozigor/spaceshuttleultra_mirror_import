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
		// TODO use class for vlvs
		//VESSEL *OV;
		int ID;

		// internal data
		THRUSTER_HANDLE thSSME;

		PROPELLANT_HANDLE phET; 

		VECTOR3 pos;
		VECTOR3 dir;
		double ISP0;
		double ISP1;
		double FPL_THRUST;

		double COtimecoef;
		double OPOV_pos_CO;
		double FPOV_pos_CO;
		double PSN4time;

		int MPL;// %
		int FPL;// %

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
		bool cmdStartEnable( void );
		bool cmdStart( void );
		bool cmdControllerReset( void );
		bool cmdCheckoutStandby( void );
		bool cmdTerminateSequence( void );
		bool cmdShutdownEnable( void );
		bool cmdShutdown( void );
		bool cmdFRT1( void );
		bool cmdOxidizerDump( void );
		bool cmdExitPROM( void );
		bool cmdPowerOn( void );
		bool cmdChannelReset( void );
		bool cmdThrottle( double );
		bool cmdPowerOff( void );
		//bool cmdDeactivateAllValves( void );//
		
		// data
		int dataDataTable( DATA_TABLE* );

		/**
		 * Create a new SSME object.
		 * @param _director reference to the subsystem director
		 * @param _ident identification of the subsystem inside the model
		 * @param nID  identification number of the SSME
		 */
		SSME( SubsystemDirector* _director, const string& _ident, 
		int nID /*, PROPELLANT_HANDLE _phET, 
		const VECTOR3& _pos, const VECTOR3& _dir, 
		double ISP0, double ISP1, double FPL_THRUST  */ );

		~SSME( void );

		/** 
		 * Create the thruster reference of the SSME 
		 * and link the cable connections of the SSME.
		 * @sa AtlantisSubsystem::Realize
		 */
		virtual void Realize();

		virtual THRUSTER_HANDLE GetHandle() const;

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
-->VDT DWs

ID word 2

SSMEC failure ID word (DW 5)
MCC Pc average word six

PBP Disch Temp Ch A DW 11

HPFT Coolant Press Ch B DW14
HPFT Coolant Press Ch B DW15

HPOT I-SEAL PGE P CH A DW 20
HPOT I-SEAL PGE P CH B DW 21

HPFTP Disch Pressure DW 29
HPOTP Disch Pressure DW 30

Fuel Flowrate Sensor A1 DW 34

HPFT Disch Temp Ch A2 DW 45
HPFT Disch Temp Ch B2 DW 46
HPOT Disch Temp Ch A2 DW 47
HPOT Disch Temp Ch B2 DW 48

Fuel Sys Prg Pr Ch A DW53
Fuel Sys Prg Pr Ch B DW54
PBP Disch Temp Ch B DW 55

MOV Hyd Temp Ch A DW 59
MOV Hyd Temp Ch B DW 60
MFV Hyd Temp Ch A DW 61
MFV Hyd Temp Ch B DW 62
POGO Recirn Isln V Pos DW 63
HPFT Disch Temp Ch A3 DW 64
Oxid Bleed Vlv Position DW 65
Fuel Bleed Vlv Position DW 65

Anti-Flood Vlv Posn Ch A DW68
Anti-Flood Vlv Posn Ch B DW68
HPFT Disch Temp Ch B3 DW 69
LPOT Disch Press Ch A DW 70
LPOT Disch Press Ch B DW 71
Emerg Sht Dn Press Ch A DW 72
Emerg Sht Dn Press Ch B DW 73
Fuel Preburn Pge Press DW 74
Oxid Prebnr Pge Press DW 75

Fuel Flowrate Sensor B1 DW 89

LPFP Disch Press Ch A DW 92
LPFP Disch Temp Ch A DW 93

HPOT Disch Temp Ch A3 DW 122
HPOT Disch Temp Ch B3 DW 123
Fuel Flowrate Sensor A2 DW124
Fuel Flowrate Sensor B2 DW125

LPFP Disch Press Ch B DW 127
LPFP Disch Temp Ch B DW 128
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