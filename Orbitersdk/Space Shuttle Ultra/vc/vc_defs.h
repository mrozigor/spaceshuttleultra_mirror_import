/****************************************************************************
  This file is part of Space Shuttle Ultra

  VC definitions



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
#if !defined(___SPACE_SHUTTLE_ULTRA_VC_DEFINITIONS_H_INCLUDED___)
#define ___SPACE_SHUTTLE_ULTRA_VC_DEFINITIONS_H_INCLUDED___

// ==========================================================
// panel area identifiers
// ==========================================================


// F
#define AID_F2_MIN		1000
#define AID_F2			1000
#define AID_F2_MAX		1009

#define AID_F3_MIN		1010
#define AID_F3			1010
#define AID_F3_MAX		1019

#define AID_F4_MIN		1020
#define AID_F4			1020
#define AID_F4_MAX		1029

#define AID_F6_MIN		1030
#define AID_F6			1030
#define AID_F6_MAX		1039

#define AID_F7_MIN		1040
#define AID_F7			1040
#define AID_F7_MAX		1049

#define AID_F8_MIN		1050
#define AID_F8			1050
#define AID_F8_MAX		1059

// L
#define AID_L1_MIN		2000
#define AID_L1			2000
#define AID_L1_MAX		2009

#define AID_L10_MIN		2010
#define AID_L10			2010
#define AID_L10_MAX		2019

#define AID_L12U_CENTAUR_MIN	2020
#define AID_L12U_CENTAUR	2020
#define AID_L12U_CENTAUR_MAX	2029

#define AID_L12U_IUS_MIN	2030
#define AID_L12U_IUS		2030
#define AID_L12U_IUS_MAX	2039

// C
#define AID_C2_MIN		3000
#define AID_C2			3000
#define AID_C2_WND0		3001
#define AID_C2_WND1		3002
#define AID_C2_WND2		3003
#define AID_C2_WND3		3004
#define AID_C2_MAX		3009

#define AID_C3_MIN		3010
#define AID_C3			3010
#define AID_C3_MAX		3019

// R
#define AID_R1_MIN		4000
#define AID_R1			4000
#define AID_R1_MAX		4009

#define AID_R2_MIN		4010
#define AID_R2			4010
#define AID_R2_MAX		4019

#define AID_R11_MIN		4020
#define AID_R11			4020
#define AID_R11_MAX		4029

#define AID_R13L_MIN		4030
#define AID_R13L		4030
#define AID_R13L_MAX		4039

// O
#define AID_O1_MIN		5000
#define AID_O1			5000
#define AID_O1_MAX		5009

#define AID_O2_MIN		5010
#define AID_O2			5010
#define AID_O2_MAX		5019

#define AID_O3_MIN		5020
#define AID_O3			5020
#define AID_O3_MAX		5029

#define AID_O6_MIN		5030
#define AID_O6			5030
#define AID_O6_MAX		5039

#define AID_O8_MIN		5040
#define AID_O8			5040
#define AID_O8_MAX		5049

#define AID_O17_MIN		5050
#define AID_O17			5050
#define AID_O17_MAX		5059

// A
#define AID_A1U_MIN		6000
#define AID_A1U			6000
#define AID_A1U_MAN		6009

#define AID_A2_MIN		6010
#define AID_A2			6010
#define AID_A2_MAX		6019

#define AID_A4_MIN		6020
#define AID_A4			6020
#define AID_A4_MAX		6029

#define AID_A6U_MIN		6030
#define AID_A6U			6030
#define AID_A6U_WND0		6031
#define AID_A6U_WND1		6032
#define AID_A6U_WND2		6033
#define AID_A6U_WND3		6034
#define AID_A6U_MAX		6039

#define AID_A7U_MIN		6040
#define AID_A7U			6040
#define AID_A7U_MAX		6049

#define AID_A7A3_MIN		6050
#define AID_A7A3		6050
#define AID_A7A3_MAX		6059

#define AID_A8_MIN		6060
#define AID_A8			6060
#define AID_A8_MAX		6069

#define AID_A8A3_MIN		6070
#define AID_A8A3		6070
#define AID_A8A3_MAX		6079

#define AID_AFTMDU_MIN		6080
#define AID_AFTMDU		6080
#define AID_AFTMDU_MAX		6089


// time until a switch gets pushed to the next position by a spring for spring-loaded switches
const double SWITCH_HOLD_TIME = 0.5;

/**
 * Namespace for all VC related classes, representing visual objects 
 * and switches.
 * Define the public contents of the VC namespace.
 */
namespace vc {
	const int TB_BARBERPOLE = 0;
	const int TB_STO = 1;
	const int TB_LAT = 2;
	const int TB_CL = 3;
	const int TB_OP = 4;
	const int TB_ON = 5;
	const int TB_OFF = 6;
	const int TB_X10 = 7;
	const int TB_GRAY = 8;
	const int TB_DPY = 9;
	const int TB_REL = 10;
	const int TB_DN = 11;
	const int TB_UP = 12;
	const int TB_RUN = 13;
	const int TB_IPL = 14;
	const int TB_ICH = 15;
	const int TB_RAD = 16;
	const int TB_PL = 17;
	const int TB_BYP = 18;

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
	const int MDUID_AFD1 = 10;


	class MDU;
	typedef MDU* PMDU;
};

#endif
