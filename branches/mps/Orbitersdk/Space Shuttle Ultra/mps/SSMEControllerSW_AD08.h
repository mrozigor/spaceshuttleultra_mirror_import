/****************************************************************************
  This file is part of Space Shuttle Ultra

  Space Shuttle Main Engine Controller Software version AD08 definition



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
#ifndef _mps_AD08_H_
#define _mps_AD08_H_


#include "SSMEControllerSW.h"


namespace mps
{
	// sw version AD08
	// for SSME Controller Block II
	// used in mission STS-125

	// commands
	const unsigned short NOP = 0x0000;
	const unsigned short SDCA = 0x0400;
	const unsigned short SDCB = 0x0800;
	const unsigned short EGND = 0x0C00;
	const unsigned short EFLT = 0x1000;
	const unsigned short EFRT = 0x1400;
	const unsigned short ETWO = 0x1800;
	const unsigned short RVRC = 0x1C00;
	const unsigned short SVRC = 0x2000;
	const unsigned short AFRT = 0x2400;
	const unsigned short XFRT = 0x2800;
	const unsigned short HYDC = 0x2C00;
	const unsigned short ERCK = 0x3000;
	const unsigned short RSYS = 0x3400;
	const unsigned short RESM = 0x3800;
	const unsigned short RSCT = 0x3C00;
	const unsigned short MLDA = 0x4000;
	const unsigned short MLDB = 0x4400;
	const unsigned short RSCA = 0x4800;
	const unsigned short RSCB = 0x4C00;
	const unsigned short HELA = 0x5000;
	const unsigned short HELB = 0x5400;
	const unsigned short PSCA = 0x5800;
	const unsigned short PSCB = 0x5C00;
	const unsigned short IOHA = 0x6000;
	const unsigned short IOHB = 0x6400;
	const unsigned short IOLA = 0x6800;
	const unsigned short IOLB = 0x6C00;
	const unsigned short IOSA = 0x7000;
	const unsigned short IOSB = 0x7400;
	const unsigned short ENFA = 0x7800;
	const unsigned short ENFB = 0x7C00;
	const unsigned short ROFA = 0x8000;
	const unsigned short ROFB = 0x8400;
	const unsigned short RWRA = 0x8800;
	const unsigned short RWRB = 0x8C00;
	const unsigned short RSKA = 0x9000;
	const unsigned short RSKB = 0x9400;
	const unsigned short EXPM = 0x9800;
	const unsigned short MRC1 = 0x9C00;
	const unsigned short MRC2 = 0xA000;
	const unsigned short LOXD = 0xA400;
	const unsigned short APOW = 0xA800;
	const unsigned short XPOW = 0xAC00;
	const unsigned short PSN1 = 0xB000;
	const unsigned short PSN2 = 0xB400;
	const unsigned short PSN3 = 0xB800;
	const unsigned short PSN4 = 0xBC00;
	const unsigned short STEN = 0xC000;
	const unsigned short IGNT = 0xC400;
	const unsigned short SDEN = 0xC800;
	const unsigned short STDN = 0xCC00;
	const unsigned short TMSQ = 0xD000;
	const unsigned short THRT = 0xD400;
	const unsigned short COSY = 0xD800;
	const unsigned short DAVL = 0xDC00;
	const unsigned short ENLS = 0xE000;
	const unsigned short INLS = 0xE400;
	const unsigned short INFA = 0xE800;
	const unsigned short INFB = 0xEC00;


	const double PC_100 = 27.46788990825688073394495412844;

	// ESW
	// Self-Test Status
	const unsigned short ESW_Engine_OK = 0x0000;
	const unsigned short ESW_MCF = 0x8000;

	// Mode (Checkout)
	const unsigned short ESW_Hydraulic_Conditioning = 0x0000;
	const unsigned short ESW_Standby = 0x1000;
	const unsigned short ESW_Actuator_Checkout = 0x2000;
	const unsigned short ESW_Engine_Leak_Detection = 0x3000;
	const unsigned short ESW_Igniter_Checkout = 0x4000;
	const unsigned short ESW_Pneumatic_Checkout = 0x5000;
	const unsigned short ESW_Sensor_Checkout = 0x6000;
	const unsigned short ESW_Controller_Checkout = 0x7000;

	// Mode (Start Prep)
	const unsigned short ESW_PSN_1 = 0x1000;
	const unsigned short ESW_PSN_2 = 0x2000;
	const unsigned short ESW_PSN_3 = 0x3000;
	const unsigned short ESW_PSN_4 = 0x4000;
	const unsigned short ESW_Engine_Ready = 0x5000;

	// Mode (Start)
	const unsigned short ESW_Start_Initiation = 0x1000;
	const unsigned short ESW_Thrust_Buildup = 0x2000;

	// Mode (Mainstage)
	const unsigned short ESW_Normal_Control = 0x1000;
	const unsigned short ESW_Fixed_Density = 0x2000;
	const unsigned short ESW_Thrust_Limiting = 0x3000;
	const unsigned short ESW_Hydraulic_Lockup = 0x4000;
	const unsigned short ESW_Electrical_Lockup = 0x5000;

	// Mode (Shutdown)
	const unsigned short ESW_Throttle_To_0 = 0x1000;
	const unsigned short ESW_Propellant_Valves_Closed = 0x2000;
	const unsigned short ESW_Fail_Safe_Pneumatic = 0x3000;

	// Mode (Post-Shutdown)
	//const unsigned short ESW_Standby = 0x1000;
	const unsigned short ESW_Oxidizer_Dump = 0x2000;
	const unsigned short ESW_Terminate_Sequence = 0x3000;

	// Mode (PROM)
	//const unsigned short ESW_Standby = 0x1000;

	// Phase
	const unsigned short ESW_Checkout = 0x0200;
	const unsigned short ESW_Start_Prep = 0x0400;
	const unsigned short ESW_Start = 0x0600;
	const unsigned short ESW_Mainstage = 0x0800;
	const unsigned short ESW_Shutdown = 0x0A00;
	const unsigned short ESW_Post_Shutdown = 0x0C00;
	const unsigned short ESW_PROM = 0x0E00;

	// Limit Control Status
	const unsigned short ESW_Enable = 0x0000;
	const unsigned short ESW_Inhibit = 0x0100;

	// Command Status
	const unsigned short ESW_Accepted = 0x0000;
	const unsigned short ESW_Rejected = 0x0004;


	// RAM addresses
	const int RAM_AD08_CH = 0;
	const int RAM_AD08_TREF1 = 1;
	const int RAM_AD08_TREF2 = 2;

	const int RAM_AD08_ESW = 3;

	const int RAM_AD08_VALIDCMD = 5;
	const int RAM_AD08_CMD1 = 6;
	const int RAM_AD08_CMD2 = 7;
	const int RAM_AD08_CMD3 = 8;

	const int RAM_AD08_CURCMD = 9;
	const int RAM_AD08_PRVCMD = 10;

	const int RAM_AD08_PC_CMD = 11;
	const int RAM_AD08_PC_REF = 12;

	const int RAM_AD08_TIME_ESC = 13;
	const int RAM_AD08_TIME_STDN = 14;

	const int RAM_AD08_START_ENA = 16;
	const int RAM_AD08_SHUTDOWN_ENA = 17;



	const int RAM_AD08_SENSOR_A = 150;
	const int RAM_AD08_SENSOR_B = 220;

	const int RAM_AD08_WDT1 = 292;
	const int RAM_AD08_WDT2 = 293;

	const int RAM_AD08_CCV_POS = 320;
	const int RAM_AD08_MFV_POS = 321;
	const int RAM_AD08_MOV_POS = 322;
	const int RAM_AD08_FPOV_POS = 323;
	const int RAM_AD08_OPOV_POS = 324;

	const int RAM_AD08_CCV_SH = 325;
	const int RAM_AD08_MFV_SH = 326;
	const int RAM_AD08_MOV_SH = 327;
	const int RAM_AD08_FPOV_SH = 328;
	const int RAM_AD08_OPOV_SH = 329;

	const int RAM_AD08_CCV_CMD = 330;
	const int RAM_AD08_MFV_CMD = 331;
	const int RAM_AD08_MOV_CMD = 332;
	const int RAM_AD08_FPOV_CMD = 333;
	const int RAM_AD08_OPOV_CMD = 334;


	const int RAM_AD08_IGNT_CCV_POS = 370;
	const int RAM_AD08_IGNT_MFV_POS = 418;
	const int RAM_AD08_IGNT_MOV_POS = 466;
	const int RAM_AD08_IGNT_FPOV_POS = 514;
	const int RAM_AD08_IGNT_OPOV_POS = 562;
	const int RAM_AD08_STDN_CCV_POS = 610;
	const int RAM_AD08_STDN_MFV_POS = 658;
	const int RAM_AD08_STDN_MOV_POS = 706;
	const int RAM_AD08_STDN_FPOV_POS = 754;
	const int RAM_AD08_STDN_OPOV_POS = 802;


	const int RAM_AD08_VRC_1 = 21;
	const int RAM_AD08_VRC_2 = 22;
	const int RAM_AD08_VRC_3 = 23;
	const int RAM_AD08_VRC_4 = 24;
	const int RAM_AD08_VRC_5 = 25;
	const int RAM_AD08_VRC_6 = 26;
	const int RAM_AD08_VRC_7 = 27;
	const int RAM_AD08_VRC_8 = 28;
	const int RAM_AD08_VRC_9 = 29;
	const int RAM_AD08_VRC_10 = 30;
	const int RAM_AD08_VRC_11 = 31;
	const int RAM_AD08_VRC_12 = 32;
	const int RAM_AD08_VRC_13 = 33;
	const int RAM_AD08_VRC_14 = 34;
	const int RAM_AD08_VRC_15 = 35;
	const int RAM_AD08_VRC_16 = 36;
	const int RAM_AD08_VRC_17 = 37;
	const int RAM_AD08_VRC_18 = 38;
	const int RAM_AD08_VRC_19 = 39;
	const int RAM_AD08_VRC_20 = 40;
	const int RAM_AD08_VRC_21 = 41;
	const int RAM_AD08_VRC_22 = 42;
	const int RAM_AD08_VRC_23 = 43;
	const int RAM_AD08_VRC_24 = 44;
	const int RAM_AD08_VRC_25 = 45;
	const int RAM_AD08_VRC_26 = 46;
	const int RAM_AD08_VRC_27 = 47;
	const int RAM_AD08_VRC_28 = 48;
	const int RAM_AD08_VRC_29 = 49;
	const int RAM_AD08_VRC_30 = 50;
	const int RAM_AD08_VRC_31 = 51;
	const int RAM_AD08_VRC_32 = 52;
	const int RAM_AD08_VRC_33 = 53;
	const int RAM_AD08_VRC_34 = 54;
	const int RAM_AD08_VRC_35 = 55;
	const int RAM_AD08_VRC_36 = 56;
	const int RAM_AD08_VRC_37 = 57;
	const int RAM_AD08_VRC_38 = 58;
	const int RAM_AD08_VRC_39 = 59;
	const int RAM_AD08_VRC_40 = 60;
	const int RAM_AD08_VRC_41 = 61;
	const int RAM_AD08_VRC_42 = 62;
	const int RAM_AD08_VRC_43 = 63;
	const int RAM_AD08_VRC_44 = 64;
	const int RAM_AD08_VRC_45 = 65;
	const int RAM_AD08_VRC_46 = 66;
	const int RAM_AD08_VRC_47 = 67;
	const int RAM_AD08_VRC_48 = 68;
	const int RAM_AD08_VRC_49 = 69;
	const int RAM_AD08_VRC_50 = 70;
	const int RAM_AD08_VRC_51 = 71;
	const int RAM_AD08_VRC_52 = 72;
	const int RAM_AD08_VRC_53 = 73;
	const int RAM_AD08_VRC_54 = 74;
	const int RAM_AD08_VRC_55 = 75;
	const int RAM_AD08_VRC_56 = 76;
	const int RAM_AD08_VRC_57 = 77;
	const int RAM_AD08_VRC_58 = 78;
	const int RAM_AD08_VRC_59 = 79;
	const int RAM_AD08_VRC_60 = 80;
	const int RAM_AD08_VRC_61 = 81;
	const int RAM_AD08_VRC_62 = 82;
	const int RAM_AD08_VRC_63 = 83;
	const int RAM_AD08_VRC_64 = 84;
	const int RAM_AD08_VRC_65 = 85;
	const int RAM_AD08_VRC_66 = 86;
	const int RAM_AD08_VRC_67 = 87;
	const int RAM_AD08_VRC_68 = 88;
	const int RAM_AD08_VRC_69 = 89;
	const int RAM_AD08_VRC_70 = 90;
	const int RAM_AD08_VRC_71 = 91;
	const int RAM_AD08_VRC_72 = 92;
	const int RAM_AD08_VRC_73 = 93;
	const int RAM_AD08_VRC_74 = 94;
	const int RAM_AD08_VRC_75 = 95;
	const int RAM_AD08_VRC_76 = 96;
	const int RAM_AD08_VRC_77 = 97;
	const int RAM_AD08_VRC_78 = 98;
	const int RAM_AD08_VRC_79 = 99;
	const int RAM_AD08_VRC_80 = 100;
	const int RAM_AD08_VRC_81 = 101;
	const int RAM_AD08_VRC_82 = 102;
	const int RAM_AD08_VRC_83 = 103;
	const int RAM_AD08_VRC_84 = 104;
	const int RAM_AD08_VRC_85 = 105;
	const int RAM_AD08_VRC_86 = 106;
	const int RAM_AD08_VRC_87 = 107;
	const int RAM_AD08_VRC_88 = 108;
	const int RAM_AD08_VRC_89 = 109;
	const int RAM_AD08_VRC_90 = 110;
	const int RAM_AD08_VRC_91 = 111;
	const int RAM_AD08_VRC_92 = 112;
	const int RAM_AD08_VRC_93 = 113;
	const int RAM_AD08_VRC_94 = 114;
	const int RAM_AD08_VRC_95 = 115;
	const int RAM_AD08_VRC_96 = 116;
	const int RAM_AD08_VRC_97 = 117;
	const int RAM_AD08_VRC_98 = 118;
	const int RAM_AD08_VRC_99 = 119;
	const int RAM_AD08_VRC_100 = 120;
	const int RAM_AD08_VRC_101 = 121;
	const int RAM_AD08_VRC_102 = 122;
	const int RAM_AD08_VRC_103 = 123;
	const int RAM_AD08_VRC_104 = 124;
	const int RAM_AD08_VRC_105 = 125;
	const int RAM_AD08_VRC_106 = 126;
	const int RAM_AD08_VRC_107 = 127;
	const int RAM_AD08_VRC_108 = 128;
	const int RAM_AD08_VRC_109 = 129;
	const int RAM_AD08_VRC_110 = 130;
	const int RAM_AD08_VRC_111 = 131;
	const int RAM_AD08_VRC_112 = 132;
	const int RAM_AD08_VRC_113 = 133;
	const int RAM_AD08_VRC_114 = 134;
	const int RAM_AD08_VRC_115 = 135;
	const int RAM_AD08_VRC_116 = 136;
	const int RAM_AD08_VRC_117 = 137;
	const int RAM_AD08_VRC_118 = 138;
	const int RAM_AD08_VRC_119 = 139;
	const int RAM_AD08_VRC_120 = 140;
	const int RAM_AD08_VRC_121 = 141;
	const int RAM_AD08_VRC_122 = 142;
	const int RAM_AD08_VRC_123 = 143;
	const int RAM_AD08_VRC_124 = 144;
	const int RAM_AD08_VRC_125 = 145;
	const int RAM_AD08_VRC_126 = 146;
	const int RAM_AD08_VRC_127 = 147;
	const int RAM_AD08_VRC_128 = 148;

	//////

	const int SENSOR_COUNT = 23;

	// must match engine model sensor output
	const unsigned short FPB_PP_IEA = 0x1F00;// 0001 1111 0000 0000
	const unsigned short OPB_PP_IEA = 0x1F01;// 0001 1111 0000 0001
	const unsigned short FPB_PP_IEB = 0x2F00;// 0010 1111 0000 0000
	const unsigned short OPB_PP_IEB = 0x2F01;// 0010 1111 0000 0001
	const unsigned short LPOT_DP_A = 0x1F02;// 0001 1111 0000 0010
	const unsigned short LPOT_DP_B = 0x2F02;// 0010 1111 0000 0010
	const unsigned short LPFT_DP_A = 0x1F03;// 0001 1111 0000 0011
	const unsigned short LPFT_DP_B = 0x2F03;// 0010 1111 0000 0011
	const unsigned short HPOTP_DP_A = 0x1F04;// 0001 1111 0000 0100
	const unsigned short HPOTP_DP_B = 0x2F04;// 0010 1111 0000 0100
	const unsigned short HPFTP_DP_A = 0x1F05;// 0001 1111 0000 0101
	const unsigned short HPFTP_DP_B = 0x2F05;// 0010 1111 0000 0101
	const unsigned short HPOTPBP_DP_A = 0x1F06;// 0001 1111 0000 0110
	const unsigned short HPOTPBP_DP_B = 0x2F06;// 0010 1111 0000 0110
	const unsigned short FS_PP_A = 0x1F07;// 0001 1111 0000 0111
	const unsigned short FS_PP_B = 0x2F07;// 0010 1111 0000 0111
	const unsigned short MCC_P_A1 = 0x1F08;// 0001 1111 0000 1000
	const unsigned short MCC_P_B1 = 0x2F08;// 0010 1111 0000 1000
	const unsigned short MCC_P_A2 = 0x1F09;// 0001 1111 0000 1001
	const unsigned short MCC_P_B2 = 0x2F09;// 0010 1111 0000 1001
	const unsigned short HPOTP_IS_A = 0x1F0A;// 0001 1111 0000 1010
	const unsigned short HPOTP_IS_B = 0x2F0A;// 0010 1111 0000 1010
	const unsigned short HPFT_DT_A2 = 0x1F0F;// 0001 1111 0000 1111
	const unsigned short HPFT_DT_B2 = 0x2F0F;// 0010 1111 0000 1111
	const unsigned short HPFT_DT_A3 = 0x1F10;// 0001 1111 0001 0000
	const unsigned short HPFT_DT_B3 = 0x2F10;// 0010 1111 0001 0000
	const unsigned short HPOT_DT_A2 = 0x1F11;// 0001 1111 0001 0001
	const unsigned short HPOT_DT_B2 = 0x2F11;// 0010 1111 0001 0001
	const unsigned short HPOT_DT_A3 = 0x1F12;// 0001 1111 0001 0010
	const unsigned short HPOT_DT_B3 = 0x2F12;// 0010 1111 0001 0010
	const unsigned short PBP_DT_A = 0x1F13;// 0001 1111 0001 0011
	const unsigned short PBP_DT_B = 0x2F13;// 0010 1111 0001 0011
	const unsigned short LPFP_DT_A = 0x1F14;// 0001 1111 0001 0100
	const unsigned short LPFP_DT_B = 0x2F14;// 0010 1111 0001 0100
	const unsigned short MOV_HT_A = 0x1F15;// 0001 1111 0001 0101
	const unsigned short MOV_HT_B = 0x2F15;// 0010 1111 0001 0101
	const unsigned short MFV_HT_A = 0x1F16;// 0001 1111 0001 0110
	const unsigned short MFV_HT_B = 0x2F16;// 0010 1111 0001 0110
	const unsigned short FF_A1 = 0x1000;// 0001 0000 0000 0000
	const unsigned short FF_B1 = 0x2000;// 0010 0000 0000 0000
	const unsigned short FF_A2 = 0x1001;// 0001 0001 0000 0000
	const unsigned short FF_B2 = 0x2001;// 0010 0001 0000 0000
	const unsigned short LPFTP_SS_A = 0x1004;// 0001 0100 0000 0000
	const unsigned short LPFTP_SS_B = 0x2004;// 0010 0100 0000 0000
	const unsigned short HPFTP_SS_A = 0x1005;// 0001 0101 0000 0000
	const unsigned short HPFTP_SS_B = 0x2005;// 0010 0101 0000 0000


	class DigitalComputerUnit;

	class SSMEControllerSW_AD08:public SSMEControllerSW
	{
		public:
			SSMEControllerSW_AD08( DigitalComputerUnit* DCU );
			~SSMEControllerSW_AD08( void );

			void Checkout_HydraulicConditioning( void );
			void Checkout_Standby( void );
			void Checkout_ActuatorCheckout( void );
			void Checkout_EngineLeakDetection( void );
			void Checkout_IgniterCheckout( void );
			void Checkout_PneumaticCheckout( void );
			void Checkout_SensorCheckout( void );
			void Checkout_ControllerCheckout( void );

			void StartPrep_PSN1( void );
			void StartPrep_PSN2( void );
			void StartPrep_PSN3( void );
			void StartPrep_PSN4( void );
			void StartPrep_EngineReady( void );

			void Start_StartInitiation( void );
			void Start_ThrustBuildup( void );

			void Mainstage_NormalControl( void );
			void Mainstage_FixedDensity( void );
			void Mainstage_ThrustLimiting( void );
			void Mainstage_HydraulicLockup( void );
			void Mainstage_ElectricalLockup( void );

			void Shutdown_ThrottleTo0( void );
			void Shutdown_PropellantValvesClosed( void );
			void Shutdown_FailSafePneumatic( void );

			void PostShutdown_Standby( void );
			void PostShutdown_OxidizerDump( void );
			void PostShutdown_TerminateSequence( void );

			//////

			void Checkout_HydraulicConditioning_B( void );
			void Checkout_Standby_B( void );
			void Checkout_ActuatorCheckout_B( void );
			void Checkout_EngineLeakDetection_B( void );
			void Checkout_IgniterCheckout_B( void );
			void Checkout_PneumaticCheckout_B( void );
			void Checkout_SensorCheckout_B( void );
			void Checkout_ControllerCheckout_B( void );

			void StartPrep_PSN1_B( void );
			void StartPrep_PSN2_B( void );
			void StartPrep_PSN3_B( void );
			void StartPrep_PSN4_B( void );
			void StartPrep_EngineReady_B( void );

			void Start_StartInitiation_B( void );
			void Start_ThrustBuildup_B( void );

			void Mainstage_NormalControl_B( void );
			void Mainstage_FixedDensity_B( void );
			void Mainstage_ThrustLimiting_B( void );
			void Mainstage_HydraulicLockup_B( void );
			void Mainstage_ElectricalLockup_B( void );

			void Shutdown_ThrottleTo0_B( void );
			void Shutdown_PropellantValvesClosed_B( void );
			void Shutdown_FailSafePneumatic_B( void );

			void PostShutdown_Standby_B( void );
			void PostShutdown_OxidizerDump_B( void );
			void PostShutdown_TerminateSequence_B( void );

			void GetTime( void );
			void BuiltVDT( void );
			void RotateCommand( void );// TODO inline this?

			void ValveSchedule( int tableaddr, int cmdaddr, int timeaddr, int currentposaddr );

			void LoadStartValveSchedule( void );
			void LoadShutdownValveSchedule( void );
			void UpdateShutdownValveSchedule( double dPC );

			void PowerFailureSense( void );
	};
}


#endif// _mps_AD08_H_