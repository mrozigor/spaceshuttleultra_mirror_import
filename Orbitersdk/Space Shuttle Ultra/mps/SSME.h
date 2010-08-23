/****************************************************************************
  This file is part of Space Shuttle Ultra

  SSME definition



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/
#ifndef _mps_SSME_H_
#define _mps_SSME_H_


#include <orbitersdk.h>
#include "..\AtlantisSubsystem.h"
#include "..\ValveTypeBool.h"


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

	typedef struct _VDT_128
	{
		float DW1;
		float DW2;
		float DW3;
		float DW4;
		float DW5;
		float DW6;
		float DW7;
		float DW8;
		float DW9;
		float DW10;
		float DW11;
		float DW12;
		float DW13;
		float DW14;
		float DW15;
		float DW16;
		float DW17;
		float DW18;
		float DW19;
		float DW20;
		float DW21;
		float DW22;
		float DW23;
		float DW24;
		float DW25;
		float DW26;
		float DW27;
		float DW28;
		float DW29;
		float DW30;
		float DW31;
		float DW32;
		float DW33;
		float DW34;
		float DW35;
		float DW36;
		float DW37;
		float DW38;
		float DW39;
		float DW40;
		float DW41;
		float DW42;
		float DW43;
		float DW44;
		float DW45;
		float DW46;
		float DW47;
		float DW48;
		float DW49;
		float DW50;
		float DW51;
		float DW52;
		float DW53;
		float DW54;
		float DW55;
		float DW56;
		float DW57;
		float DW58;
		float DW59;
		float DW60;
		float DW61;
		float DW62;
		float DW63;
		float DW64;
		float DW65;
		float DW66;
		float DW67;
		float DW68;
		float DW69;
		float DW70;
		float DW71;
		float DW72;
		float DW73;
		float DW74;
		float DW75;
		float DW76;
		float DW77;
		float DW78;
		float DW79;
		float DW80;
		float DW81;
		float DW82;
		float DW83;
		float DW84;
		float DW85;
		float DW86;
		float DW87;
		float DW88;
		float DW89;
		float DW90;
		float DW91;
		float DW92;
		float DW93;
		float DW94;
		float DW95;
		float DW96;
		float DW97;
		float DW98;
		float DW99;
		float DW100;
		float DW101;
		float DW102;
		float DW103;
		float DW104;
		float DW105;
		float DW106;
		float DW107;
		float DW108;
		float DW109;
		float DW110;
		float DW111;
		float DW112;
		float DW113;
		float DW114;
		float DW115;
		float DW116;
		float DW117;
		float DW118;
		float DW119;
		float DW120;
		float DW121;
		float DW122;
		float DW123;
		float DW124;
		float DW125;
		float DW126;
		float DW127;
		float DW128;
	} VDT_128;

	typedef struct _VDT_32
	{
		float DW1;
		float DW2;
		float DW3;
		float DW4;
		float DW5;
		float DW6;
		float DW7;
		float DW8;
		float DW9;
		float DW10;
		float DW11;
		float DW12;
		float DW13;
		float DW14;
		float DW15;
		float DW16;
		float DW17;
		float DW18;
		float DW19;
		float DW20;
		float DW21;
		float DW22;
		float DW23;
		float DW24;
		float DW25;
		float DW26;
		float DW27;
		float DW28;
		float DW29;
		float DW30;
		float DW31;
		float DW32;
	} VDT_32;

	typedef struct _VDT_6
	{
		float DW1;
		float DW2;
		float DW3;
		float DW4;
		float DW5;
		float DW6;
	} VDT_6;


	class SSME:public AtlantisSubsystem
	{
	protected:
		BasicValve* ptrCCV;
		BasicValve* ptrMOV;
		BasicValve* ptrMFV;
		BasicValve* ptrFPOV;
		BasicValve* ptrOPOV;
		ValveTypeBool* ptrOBV;
		ValveTypeBool* ptrFBV;

		// internal data
		VDT_128* VDT;
		int ID;
		ENGINE_STATUS STATUSWORD;
		DCU activeDCU;
		//THRUSTER_HANDLE thSSME;

		double ESCtime;// simtime
		double COtime;// simtime

		bool StartEnable;
		bool ShutdownEnable;

		//PROPELLANT_HANDLE phET;

		//VECTOR3 pos;
		//VECTOR3 dir;

		double ISP0;
		double ISP1;
		double FPL_THRUST;
		double RPL_PC_PRESS;
		int MPL;// % RPL
		int FPL;// % RPL

		double COtimecoef;// phasing out...
		double ThrottleCmdTme;
		double PSN4time;
		double PC_REF;
		double PC_CMD;


		// valve position tables
		short* CCVSIindex;
		double* CCVScheduleIgnition_Time;
		double* CCVScheduleIgnition_Position;
		double* CCVScheduleIgnition_Rate;

		short* MOVSIindex;
		double* MOVScheduleIgnition_Time;
		double* MOVScheduleIgnition_Position;
		double* MOVScheduleIgnition_Rate;

		short* MFVSIindex;
		double* MFVScheduleIgnition_Time;
		double* MFVScheduleIgnition_Position;
		double* MFVScheduleIgnition_Rate;

		short* FPOVSIindex;
		double* FPOVScheduleIgnition_Time;
		double* FPOVScheduleIgnition_Position;
		double* FPOVScheduleIgnition_Rate;

		short* OPOVSIindex;
		double* OPOVScheduleIgnition_Time;
		double* OPOVScheduleIgnition_Position;
		double* OPOVScheduleIgnition_Rate;

		short* CCVSSindex;
		double* CCVScheduleShutdown_Time;
		double* CCVScheduleShutdown_Position;
		double* CCVScheduleShutdown_Rate;

		short* MOVSSindex;
		double* MOVScheduleShutdown_Time;
		double* MOVScheduleShutdown_Position;
		double* MOVScheduleShutdown_Rate;

		short* MFVSSindex;
		double* MFVScheduleShutdown_Time;
		double* MFVScheduleShutdown_Position;
		double* MFVScheduleShutdown_Rate;

		short* FPOVSSindex;
		double* FPOVScheduleShutdown_Time;
		double* FPOVScheduleShutdown_Position;
		double* FPOVScheduleShutdown_Rate;

		short* OPOVSSindex;
		double* OPOVScheduleShutdown_Time;
		double* OPOVScheduleShutdown_Position;
		double* OPOVScheduleShutdown_Rate;

		double PRESS_MCC_A1;
		double PRESS_MCC_A2;
		double PRESS_MCC_B1;
		double PRESS_MCC_B2;
		

		void Ignition( double );
		void Shutdown( double );
		void Throttling( double, double );
		void SetCOTime( void );

		/**
		 * Converts engine level from Orbiter convention to STS convention
		 * @param pcOSFS engine level (Orbiter convention)
		 * @return engine level (STS convention)
		 */
		double PCfromOSFStoSTS( double pcOSFS );

		/**
		 * Converts engine level from STS convention to Orbiter convention
		 * @param pcSTS engine level (STS convention)
		 * @return engine level (Orbiter convention)
		 */
		double PCfromSTStoOSFS( double pcSTS );

		/**
		 * Converts engine level from percentage to chamber pressure (psi)
		 * @param pcPCT engine level (percentage)
		 * @return engine level (chamber pressure)
		 */
		double PCfromPCTtoPSI( double pcPCT );

		/**
		 * Converts engine level from chamber pressure (psi) to percentage
		 * @param pcPSI engine level (chamber pressure)
		 * @return engine level (percentage)
		 */
		double PCfromPSItoPCT( double pcPSI );

		/**
		 * Moves valves through ignition schedule
		 * @param time time from ignition command
		 */
		void ValveScheduleIgnition( double time );

		/**
		 * Moves valves through throttle schedule
		 * @param tgtpc commanded engine level (percentage)
		 */
		virtual void ValveScheduleThrottle( double tgtpc ) = 0;

		/**
		 * Moves valves through shutdown schedule
		 * @param time time from shutdown command
		 */
		void ValveScheduleShutdown( double time );

		/**
		 * Updates valve shutdown schedule according to current engine level
		 * @param dPC difference between RPL and current engine level in percentage (-9 (109%) to 33 (67%))
		 */
		virtual void ValveShutdownTableUpdate( double dPC ) = 0;

		virtual void VDTUpdate( double ) = 0;// ????

		void ReadSensors( void );

		// data cookup fncts
		virtual double dcPC_ESC( double ) = 0;
		virtual double dcPC_CO( double ) = 0;
		virtual double dcPC_MS( double ) = 0;

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
		//bool cmdDeactivateAllValves( void );
		
		// data
		int dataGetPrimaryData( VDT_32* );
		int dataGetSecondaryData( VDT_6* );

		/**
		 * Create a new SSME object.
		 * @param _director reference to the subsystem director
		 * @param _ident identification of the subsystem inside the model
		 * @param nID identification number of the SSME
		 */
		SSME( AtlantisSubsystemDirector* _director, const string& _ident, int nID );
		~SSME( void );

		/** 
		 * Create the thruster reference of the SSME 
		 * and link the cable connections of the SSME.
		 * @sa AtlantisSubsystem::Realize
		 */
		virtual void Realize( void );

		//virtual THRUSTER_HANDLE GetHandle( void ) const;

		// heart beat
		void OnPostStep( double, double, double );
		//void OnPreStep( double, double, double );
		//void OnPropagate( double, double, double );

		virtual void OnSaveState( FILEHANDLE ) const = 0;
		virtual bool OnParseLine( const char* ) = 0;
		};
}


#endif// _mps_SSME_H_
