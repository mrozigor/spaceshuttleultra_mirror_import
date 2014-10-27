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


	const double PC_100 = 2746.788990825688073394495412844;
	const double PC_100_B = 274.6788990825688073394495412844; // /10
	const double PC_100_C = 27.46788990825688073394495412844; // /100
	const unsigned short PC_100_D = 2747; // round

	const int MPL = 67;
	const int FPL = 109;

	const double SINGLE_CH_STDN_TIMER = 512.86;// seconds

	// ESW
	// Self-Test Status
	const unsigned short ESW_EngineOK = 1;
	const unsigned short ESW_MCF = 2;
	const unsigned short ESW_LimitExceeded = 3;

	// Mode (Checkout)
	const unsigned short ESW_Checkout_HydraulicConditioning = 0;
	const unsigned short ESW_Checkout_Standby = 1;
	const unsigned short ESW_Checkout_ActuatorCheckout = 2;
	const unsigned short ESW_Checkout_EngineLeakDetection = 3;
	const unsigned short ESW_Checkout_IgniterCheckout = 4;
	const unsigned short ESW_Checkout_PneumaticCheckout = 5;
	const unsigned short ESW_Checkout_SensorCheckout = 6;
	const unsigned short ESW_Checkout_ControllerCheckout = 7;

	// Mode (Start Prep)
	const unsigned short ESW_StartPrep_PSN1 = 1;
	const unsigned short ESW_StartPrep_PSN2 = 2;
	const unsigned short ESW_StartPrep_PSN3 = 3;
	const unsigned short ESW_StartPrep_PSN4 = 4;
	const unsigned short ESW_StartPrep_EngineReady = 5;

	// Mode (Start)
	const unsigned short ESW_Start_StartInitiation = 1;
	const unsigned short ESW_Start_ThrustBuildup = 2;

	// Mode (Mainstage)
	const unsigned short ESW_Mainstage_NormalControl = 1;
	const unsigned short ESW_Mainstage_FixedDensity = 2;
	const unsigned short ESW_Mainstage_ThrustLimiting = 3;
	const unsigned short ESW_Mainstage_HydraulicLockup = 4;
	const unsigned short ESW_Mainstage_ElectricalLockup = 5;

	// Mode (Shutdown)
	const unsigned short ESW_Shutdown_ThrottleTo0 = 1;
	const unsigned short ESW_Shutdown_PropellantValvesClosed = 2;
	const unsigned short ESW_Shutdown_FailSafePneumatic = 3;

	// Mode (Post-Shutdown)
	const unsigned short ESW_PostShutdown_Standby = 1;
	const unsigned short ESW_PostShutdown_OxidizerDump = 2;
	const unsigned short ESW_PostShutdown_TerminateSequence = 3;

	// Phase
	const unsigned short ESW_Checkout = 1;
	const unsigned short ESW_StartPrep = 2;
	const unsigned short ESW_Start = 3;
	const unsigned short ESW_Mainstage = 4;
	const unsigned short ESW_Shutdown = 5;
	const unsigned short ESW_PostShutdown = 6;

	// Limit Control Status
	const unsigned short ESW_Inhibit = 0;
	const unsigned short ESW_Enable = 1;

	// FRT Status
	const unsigned short ESW_NormOperation = 0;
	const unsigned short ESW_FRT = 1;

	// Channel Status
	const unsigned short ESW_OK = 0;
	const unsigned short ESW_CHA_ERROR = 1;
	const unsigned short ESW_CHB_ERROR = 2;
	const unsigned short ESW_CHA_CHB_ERROR = 3;
	const unsigned short ESW_CHC_ERROR = 4;
	const unsigned short ESW_CHA_CHC_ERROR = 5;
	const unsigned short ESW_CHB_CHC_ERROR = 6;
	const unsigned short ESW_CHA_CHB_CHC_ERROR = 7;

	// Command Status
	const unsigned short ESW_NoCommand = 0;
	const unsigned short ESW_CommandRejected_A = 1;
	const unsigned short ESW_CommandRejected_B = 2;
	const unsigned short ESW_Accepted = 3;

	// Load Mode
	const unsigned short ESW_No = 0;

	// FID list
	const unsigned short FID_LossOfControllerRedundancy = 1;
	const unsigned short FID_RedlineExceeded = 13;
	const unsigned short FID_SensorDesqualified = 111;

	// Delimiter list
	const unsigned short Delimiter_DCUA = 1;
	const unsigned short Delimiter_DCUB = 2;
	const unsigned short Delimiter_IEA = 3;
	const unsigned short Delimiter_IEB = 4;
	const unsigned short Delimiter_OEA = 5;
	const unsigned short Delimiter_OEB = 6;
	const unsigned short Delimiter_CCVSEIIA = 7;
	const unsigned short Delimiter_CCVSEIIB = 8;
	const unsigned short Delimiter_MFVSEIIA = 9;
	const unsigned short Delimiter_MFVSEIIB = 10;
	const unsigned short Delimiter_MOVSEIIA = 11;
	const unsigned short Delimiter_MOVSEIIB = 12;
	const unsigned short Delimiter_FPOVSEIIA = 13;
	const unsigned short Delimiter_FPOVSEIIB = 14;
	const unsigned short Delimiter_OPOVSEIIA = 15;
	const unsigned short Delimiter_OPOVSEIIB = 16;

	const unsigned short Delimiter_HPFTDischargeTemperature = 1;
	const unsigned short Delimiter_HPOTDischargeTemperature = 2;
	const unsigned short Delimiter_HPOTPIntermediateSealPressure = 3;
	const unsigned short Delimiter_MCCPC = 4;
	const unsigned short Delimiter_OPBPurgePressure = 414;

	const unsigned short Delimiter_FuelFlowrateIntraChannelTestChannelA = 101;

	// RAM addresses
	const int RAM_AD08_CH = 0;
	const int RAM_AD08_TREF1 = 1;
	const int RAM_AD08_TREF2 = 2;

	const int RAM_AD08_ESW = 3;

	const int RAM_AD08_CMD_RCVD = 4;
	const int RAM_AD08_VALIDCMD = 5;
	const int RAM_AD08_CMD1 = 6;
	const int RAM_AD08_CMD2 = 7;
	const int RAM_AD08_CMD3 = 8;

	const int RAM_AD08_CURCMD = 9;
	const int RAM_AD08_PRVCMD = 10;

	const int RAM_AD08_PC_CMD = 12;
	const int RAM_AD08_PC_REF = 13;

	const int RAM_AD08_TIME_ESC = 15;
	const int RAM_AD08_TIME_STDN = 16;

	const int RAM_AD08_START_ENA = 18;
	const int RAM_AD08_SHUTDOWN_ENA = 19;

	const int RAM_AD08_DCU_A_STATUS = 26;
	const int RAM_AD08_DCU_B_STATUS = 27;
	const int RAM_AD08_IE_A_STATUS = 28;
	const int RAM_AD08_IE_B_STATUS = 29;
	const int RAM_AD08_OE_A_STATUS = 30;
	const int RAM_AD08_OE_B_STATUS = 31;

	const int RAM_AD08_FID_BUFFER = 33;

	const int RAM_AD08_WDT1 = 42;
	const int RAM_AD08_WDT2 = 43;

	const int RAM_AD08_NXT_PHASE = 45;
	const int RAM_AD08_NXT_MODE = 46;


	const int RAM_AD08_SENSOR_A = 180;
	const int RAM_AD08_SENSOR_B = 270;

	const int RAM_AD08_CCV_POS = 360;
	const int RAM_AD08_MFV_POS = 361;
	const int RAM_AD08_MOV_POS = 362;
	const int RAM_AD08_FPOV_POS = 363;
	const int RAM_AD08_OPOV_POS = 364;
	const int RAM_AD08_FBV_POS = 365;
	const int RAM_AD08_OBV_POS = 366;
	const int RAM_AD08_AFV_POS = 367;
	const int RAM_AD08_RIV_POS = 368;
	const int RAM_AD08_CCV_SH = 369;
	const int RAM_AD08_MFV_SH = 370;
	const int RAM_AD08_MOV_SH = 371;
	const int RAM_AD08_FPOV_SH = 372;
	const int RAM_AD08_OPOV_SH = 373;

	const int RAM_AD08_OE_A_ONOFF_REG_1 = 375;
	const int RAM_AD08_OE_A_ONOFF_REG_2 = 376;
	const int RAM_AD08_OE_B_ONOFF_REG_1 = 377;
	const int RAM_AD08_OE_B_ONOFF_REG_2 = 378;

	const int RAM_AD08_CCV_SEII_A_STATUS = 380;
	const int RAM_AD08_CCV_SEII_B_STATUS = 381;
	const int RAM_AD08_MFV_SEII_A_STATUS = 382;
	const int RAM_AD08_MFV_SEII_B_STATUS = 383;
	const int RAM_AD08_MOV_SEII_A_STATUS = 384;
	const int RAM_AD08_MOV_SEII_B_STATUS = 385;
	const int RAM_AD08_FPOV_SEII_A_STATUS = 386;
	const int RAM_AD08_FPOV_SEII_B_STATUS = 387;
	const int RAM_AD08_OPOV_SEII_A_STATUS = 388;
	const int RAM_AD08_OPOV_SEII_B_STATUS = 389;

	const int RAM_AD08_MCC_PC_QUAL_AVGR = 400;

	/*INFO Each controller normally requires two of three valid command paths
	from the GPC’s to control the SSME (start commands require three of three functional command
	paths). SSME controller software change RCN 4354 implemented in version OI-6 and subs added the
	capability for the engine to accept a shutdown enable/shutdown command pair on a single channel
	under special circumstances: an internal timer has expired (currently set at 512.86 seconds from engine
	start); limits have never been inhibited; the shutdown enable/shutdown command pair come in on the
	same channel, sequentially (with no other command in-between); and a valid command is not
	concurrently being received on the other two channels.*/
	const int RAM_AD08_IGNT_TIME1 = 430;
	const int RAM_AD08_IGNT_TIME2 = 431;
	const int RAM_AD08_LIMITSINHIBITED = 432;
	const int RAM_AD08_STEN_STDN_CH = 433;

	const int RAM_AD08_CCV_CMD = 450;
	const int RAM_AD08_MFV_CMD = 451;
	const int RAM_AD08_MOV_CMD = 452;
	const int RAM_AD08_FPOV_CMD = 453;
	const int RAM_AD08_OPOV_CMD = 454;

	const int RAM_AD08_MCC_IGNITER_CMD = 456;
	const int RAM_AD08_FPB_IGNITER_CMD = 457;
	const int RAM_AD08_OPB_IGNITER_CMD = 458;
	const int RAM_AD08_FUELSYSTEMPURGE_CMD = 459;
	const int RAM_AD08_BLEEDVALVESCONTROL_CMD = 460;
	const int RAM_AD08_EMERGENCYSHUTDOWN_CMD = 461;
	const int RAM_AD08_SHUTDOWNPURGE_CMD = 462;
	const int RAM_AD08_HPOTPISPURGE_CMD = 463;
	const int RAM_AD08_AFV_CMD = 464;
	const int RAM_AD08_HPV_CMD = 465;

	const int RAM_AD08_CCV_FO_SS_CMD = 470;
	const int RAM_AD08_MFV_FO_SS_CMD = 471;
	const int RAM_AD08_MOV_FO_SS_CMD = 472;
	const int RAM_AD08_FPOV_FO_SS_CMD = 473;
	const int RAM_AD08_OPOV_FO_SS_CMD = 474;
	const int RAM_AD08_CCV_FS_SS_CMD = 475;
	const int RAM_AD08_MFV_FS_SS_CMD = 476;
	const int RAM_AD08_MOV_FS_SS_CMD = 477;
	const int RAM_AD08_FPOV_FS_SS_CMD = 478;
	const int RAM_AD08_OPOV_FS_SS_CMD = 479;

	const int RAM_AD08_IGNT_CCV_POS = 500;
	const int RAM_AD08_IGNT_MFV_POS = 548;
	const int RAM_AD08_IGNT_MOV_POS = 596;
	const int RAM_AD08_IGNT_FPOV_POS = 644;
	const int RAM_AD08_IGNT_OPOV_POS = 692;
	const int RAM_AD08_STDN_CCV_POS = 740;
	const int RAM_AD08_STDN_MFV_POS = 788;
	const int RAM_AD08_STDN_MOV_POS = 836;
	const int RAM_AD08_STDN_FPOV_POS = 884;
	const int RAM_AD08_STDN_OPOV_POS = 932;


	const int RAM_AD08_VRC_1 = 50;
	const int RAM_AD08_VRC_2 = 51;
	const int RAM_AD08_VRC_3 = 52;
	const int RAM_AD08_VRC_4 = 53;
	const int RAM_AD08_VRC_5 = 54;
	const int RAM_AD08_VRC_6 = 55;
	const int RAM_AD08_VRC_7 = 56;
	const int RAM_AD08_VRC_8 = 57;
	const int RAM_AD08_VRC_9 = 58;
	const int RAM_AD08_VRC_10 = 59;
	const int RAM_AD08_VRC_11 = 60;
	const int RAM_AD08_VRC_12 = 61;
	const int RAM_AD08_VRC_13 = 62;
	const int RAM_AD08_VRC_14 = 63;
	const int RAM_AD08_VRC_15 = 64;
	const int RAM_AD08_VRC_16 = 65;
	const int RAM_AD08_VRC_17 = 66;
	const int RAM_AD08_VRC_18 = 67;
	const int RAM_AD08_VRC_19 = 68;
	const int RAM_AD08_VRC_20 = 69;
	const int RAM_AD08_VRC_21 = 70;
	const int RAM_AD08_VRC_22 = 71;
	const int RAM_AD08_VRC_23 = 72;
	const int RAM_AD08_VRC_24 = 73;
	const int RAM_AD08_VRC_25 = 74;
	const int RAM_AD08_VRC_26 = 75;
	const int RAM_AD08_VRC_27 = 76;
	const int RAM_AD08_VRC_28 = 77;
	const int RAM_AD08_VRC_29 = 78;
	const int RAM_AD08_VRC_30 = 79;
	const int RAM_AD08_VRC_31 = 80;
	const int RAM_AD08_VRC_32 = 81;
	const int RAM_AD08_VRC_33 = 82;
	const int RAM_AD08_VRC_34 = 83;
	const int RAM_AD08_VRC_35 = 84;
	const int RAM_AD08_VRC_36 = 85;
	const int RAM_AD08_VRC_37 = 86;
	const int RAM_AD08_VRC_38 = 87;
	const int RAM_AD08_VRC_39 = 88;
	const int RAM_AD08_VRC_40 = 89;
	const int RAM_AD08_VRC_41 = 90;
	const int RAM_AD08_VRC_42 = 91;
	const int RAM_AD08_VRC_43 = 92;
	const int RAM_AD08_VRC_44 = 93;
	const int RAM_AD08_VRC_45 = 94;
	const int RAM_AD08_VRC_46 = 95;
	const int RAM_AD08_VRC_47 = 96;
	const int RAM_AD08_VRC_48 = 97;
	const int RAM_AD08_VRC_49 = 98;
	const int RAM_AD08_VRC_50 = 99;
	const int RAM_AD08_VRC_51 = 100;
	const int RAM_AD08_VRC_52 = 101;
	const int RAM_AD08_VRC_53 = 102;
	const int RAM_AD08_VRC_54 = 103;
	const int RAM_AD08_VRC_55 = 104;
	const int RAM_AD08_VRC_56 = 105;
	const int RAM_AD08_VRC_57 = 106;
	const int RAM_AD08_VRC_58 = 107;
	const int RAM_AD08_VRC_59 = 108;
	const int RAM_AD08_VRC_60 = 109;
	const int RAM_AD08_VRC_61 = 110;
	const int RAM_AD08_VRC_62 = 111;
	const int RAM_AD08_VRC_63 = 112;
	const int RAM_AD08_VRC_64 = 113;
	const int RAM_AD08_VRC_65 = 114;
	const int RAM_AD08_VRC_66 = 115;
	const int RAM_AD08_VRC_67 = 116;
	const int RAM_AD08_VRC_68 = 117;
	const int RAM_AD08_VRC_69 = 118;
	const int RAM_AD08_VRC_70 = 119;
	const int RAM_AD08_VRC_71 = 120;
	const int RAM_AD08_VRC_72 = 121;
	const int RAM_AD08_VRC_73 = 122;
	const int RAM_AD08_VRC_74 = 123;
	const int RAM_AD08_VRC_75 = 124;
	const int RAM_AD08_VRC_76 = 125;
	const int RAM_AD08_VRC_77 = 126;
	const int RAM_AD08_VRC_78 = 127;
	const int RAM_AD08_VRC_79 = 128;
	const int RAM_AD08_VRC_80 = 129;
	const int RAM_AD08_VRC_81 = 130;
	const int RAM_AD08_VRC_82 = 131;
	const int RAM_AD08_VRC_83 = 132;
	const int RAM_AD08_VRC_84 = 133;
	const int RAM_AD08_VRC_85 = 134;
	const int RAM_AD08_VRC_86 = 135;
	const int RAM_AD08_VRC_87 = 136;
	const int RAM_AD08_VRC_88 = 137;
	const int RAM_AD08_VRC_89 = 138;
	const int RAM_AD08_VRC_90 = 139;
	const int RAM_AD08_VRC_91 = 140;
	const int RAM_AD08_VRC_92 = 141;
	const int RAM_AD08_VRC_93 = 142;
	const int RAM_AD08_VRC_94 = 143;
	const int RAM_AD08_VRC_95 = 144;
	const int RAM_AD08_VRC_96 = 145;
	const int RAM_AD08_VRC_97 = 146;
	const int RAM_AD08_VRC_98 = 147;
	const int RAM_AD08_VRC_99 = 148;
	const int RAM_AD08_VRC_100 = 149;
	const int RAM_AD08_VRC_101 = 150;
	const int RAM_AD08_VRC_102 = 151;
	const int RAM_AD08_VRC_103 = 152;
	const int RAM_AD08_VRC_104 = 153;
	const int RAM_AD08_VRC_105 = 154;
	const int RAM_AD08_VRC_106 = 155;
	const int RAM_AD08_VRC_107 = 156;
	const int RAM_AD08_VRC_108 = 157;
	const int RAM_AD08_VRC_109 = 158;
	const int RAM_AD08_VRC_110 = 159;
	const int RAM_AD08_VRC_111 = 160;
	const int RAM_AD08_VRC_112 = 161;
	const int RAM_AD08_VRC_113 = 162;
	const int RAM_AD08_VRC_114 = 163;
	const int RAM_AD08_VRC_115 = 164;
	const int RAM_AD08_VRC_116 = 165;
	const int RAM_AD08_VRC_117 = 166;
	const int RAM_AD08_VRC_118 = 167;
	const int RAM_AD08_VRC_119 = 168;
	const int RAM_AD08_VRC_120 = 169;
	const int RAM_AD08_VRC_121 = 170;
	const int RAM_AD08_VRC_122 = 171;
	const int RAM_AD08_VRC_123 = 172;
	const int RAM_AD08_VRC_124 = 173;
	const int RAM_AD08_VRC_125 = 174;
	const int RAM_AD08_VRC_126 = 175;
	const int RAM_AD08_VRC_127 = 176;
	const int RAM_AD08_VRC_128 = 177;

	//////

	const int SENSOR_COUNT = 30;

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
		private:
			int (SSMEControllerSW_AD08::*fptrSensorInput)( void );

			int (SSMEControllerSW_AD08::*fptrSensorScale)( void );

			int (SSMEControllerSW_AD08::*fptrVehicleCommands)( void );

			int (SSMEControllerSW_AD08::*fptrMonitorSDLimits)( void );

			int (SSMEControllerSW_AD08::*fptrEngineOperations)( void );

			int (SSMEControllerSW_AD08::*fptrCommandActuators)( void );

			int (SSMEControllerSW_AD08::*fptrCommandONOFFDevices)( void );

			int (SSMEControllerSW_AD08::*fptrOutputVDT)( void );
		public:
			SSMEControllerSW_AD08( DigitalComputerUnit* DCU );
			~SSMEControllerSW_AD08( void );

			int GetConfig( void );
			void SetConfig( int config );

			void Executive( void );

			int SelfTest( void );

			void ChangePhaseMode( void );

			int SensorInput( void );

			int SensorScale( void );
			void SensorQualification_Upper( unsigned short addr_snsr, unsigned short qual_limit_upper );
			void SensorQualification_Lower( unsigned short addr_snsr, unsigned short qual_limit_lower );
			void SensorQualification_UpperLower( unsigned short addr_snsr, unsigned short qual_limit_upper, unsigned short qual_limit_lower );
			void SensorsDesqualifyAll( int ch );
			//unsigned short CalcSensorAverage2( unsigned short addr_snsr_A, unsigned short addr_snsr_B );
			unsigned short CalcSensorAverage4( unsigned short addr_snsr_A, unsigned short addr_snsr_B, unsigned short addr_snsr_C, unsigned short addr_snsr_D );

			int VehicleCommands_Checkout_Standby( void );
			int VehicleCommands_StartPrep_PSN1( void );
			int VehicleCommands_StartPrep_PSN2( void );
			int VehicleCommands_StartPrep_PSN3( void );
			int VehicleCommands_StartPrep_PSN4( void );
			int VehicleCommands_StartPrep_EngineReady( void );
			int VehicleCommands_Start_StartInitiation( void );
			int VehicleCommands_Start_ThrustBuildup( void );
			int VehicleCommands_Mainstage_NormalControl( void );
			int VehicleCommands_Mainstage_FixedDensity( void );
			int VehicleCommands_Mainstage_ThrustLimiting( void );
			int VehicleCommands_Mainstage_HydraulicLockup( void );
			int VehicleCommands_Mainstage_ElectricalLockup( void );
			int VehicleCommands_Shutdown_ThrottleTo0( void );
			int VehicleCommands_Shutdown_PropellantValvesClosed( void );
			int VehicleCommands_Shutdown_FailSafePneumatic( void );
			int VehicleCommands_PostShutdown_Standby( void );
			int VehicleCommands_PostShutdown_OxidizerDump( void );
			int VehicleCommands_PostShutdown_TerminateSequence( void );

			int MonitorSDLimits_Mainstage_NormalControl( void );

			int EngineOperations_Checkout_Standby( void );
			int EngineOperations_StartPrep_PSN1( void );
			int EngineOperations_StartPrep_PSN2( void );
			int EngineOperations_StartPrep_PSN3( void );
			int EngineOperations_StartPrep_PSN4( void );
			int EngineOperations_StartPrep_EngineReady( void );
			int EngineOperations_Start_StartInitiation( void );
			int EngineOperations_Start_ThrustBuildup( void );
			int EngineOperations_Mainstage_NormalControl( void );
			int EngineOperations_Mainstage_FixedDensity( void );
			int EngineOperations_Mainstage_ThrustLimiting( void );
			int EngineOperations_Mainstage_HydraulicLockup( void );
			int EngineOperations_Mainstage_ElectricalLockup( void );
			int EngineOperations_Shutdown_ThrottleTo0( void );
			int EngineOperations_Shutdown_PropellantValvesClosed( void );
			int EngineOperations_Shutdown_FailSafePneumatic( void );
			int EngineOperations_PostShutdown_Standby( void );
			int EngineOperations_PostShutdown_OxidizerDump( void );
			int EngineOperations_PostShutdown_TerminateSequence( void );

			int CommandActuators( void );

			int CommandONOFFDevices( void );

			int OutputVDT( void );


			void Set_ESW_SelfTestStatus( unsigned short SelfTestStatus );
			unsigned short Get_ESW_SelfTestStatus( void ) const;
			void Set_ESW_Mode( unsigned short Mode );
			unsigned short Get_ESW_Mode( void ) const;
			void Set_ESW_Phase( unsigned short Phase );
			unsigned short Get_ESW_Phase( void ) const;
			void Set_ESW_LimitControlStatus( unsigned short LimitControlStatus );
			unsigned short Get_ESW_LimitControlStatus( void ) const;
			void Set_ESW_FRTStatus( unsigned short FRTStatus );
			unsigned short Get_ESW_FRTStatus( void ) const;
			void Set_ESW_ChannelStatus( unsigned short ChannelStatus );
			unsigned short Get_ESW_ChannelStatus( void ) const;
			void Set_ESW_CommandStatus( unsigned short CommandStatus );
			unsigned short Get_ESW_CommandStatus( void ) const;

			////////////////////////////////////////////////


			void GetTime( void );
			void RotateCommand( void );// TODO inline this?
			void AddFID( unsigned short FID, unsigned short Delimiter );
			unsigned short GetFID( void );
			int CommandVoting( void );

			void ValveSchedule( int tableaddr, int cmdaddr, int timeaddr, int currentposaddr );

			void LoadStartValveSchedule( void );
			void LoadShutdownValveSchedule( void );
			void UpdateShutdownValveSchedule( double dPC );

			void Interrupt( int num );
	};
}


#endif// _mps_AD08_H_