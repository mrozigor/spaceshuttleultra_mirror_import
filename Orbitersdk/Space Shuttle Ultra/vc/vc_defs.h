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

// D. Beachy: define power buttons
#define AID_CDR1_PWR      12
#define AID_CDR2_PWR      13
#define AID_PLT1_PWR      14
#define AID_PLT2_PWR      15
#define AID_CRT1_PWR      16
#define AID_CRT2_PWR      17
#define AID_CRT3_PWR      18
#define AID_CRT4_PWR      19
#define AID_MFD1_PWR      20
#define AID_MFD2_PWR      21
#define AID_AFD_PWR		  22
// MFD brightness buttons
#define AID_CDR1_BRT      23
#define AID_CDR2_BRT      24
#define AID_PLT1_BRT      25
#define AID_PLT2_BRT      26
#define AID_CRT1_BRT      27
#define AID_CRT2_BRT      28
#define AID_CRT3_BRT      29
#define AID_CRT4_BRT      30
#define AID_MFD1_BRT      31
#define AID_MFD2_BRT      32
#define AID_AFD_BRT       33
// Panel R13L (payload bay operations)
#define AID_R13L_MIN     100
#define AID_R13L         100
#define AID_R13L_MAX     120
// Panel F6 (commander's panel)
#define AID_F6_MIN     200
#define AID_F6         200
#define AID_F6_MAX     219
// Panel F7 (event timer and MPS status)
#define AID_F7_MIN	     260
#define AID_F7	         260
#define AID_F7_MAX	     279
// Panel C2 (event timer and CRT assignments)
#define AID_C2_MIN     280
#define AID_C2		   280
#define AID_C2_WND0    281
#define AID_C2_WND1    282
#define AID_C2_WND2    283
#define AID_C2_WND3    284
#define AID_C2_MAX     289
// Keyboards
#define AID_KYBD_CDR   290
#define AID_KYBD_PLT   291
// Panel C3
#define AID_C3_MIN     300
#define AID_C3         300
#define AID_C3_MAX     309

#define AID_O1_MIN	   320
#define AID_O1		   320
#define AID_O1_MAX	   329

#define AID_O2_MIN	   330
#define AID_O2		   330
#define AID_O2_MAX	   339

// Panel O3
#define AID_O3_MIN	   340
#define AID_O3		   340
#define AID_O3_MAX	   349

#define AID_O6_MIN	   350
#define AID_O6		   350
#define AID_O6_MAX	   359

#define AID_O8_MIN	   360
#define AID_O8		   360
#define AID_O8_MAX	   369

#define AID_R1_MIN	   370
#define AID_R1		   370
#define AID_R1_MAX	   379
// Panel R2
#define AID_R2_MIN     400
#define AID_R2         400
#define AID_R2_MAX     419

#define AID_A4_MIN	   420
#define AID_A4		   420
#define AID_A4_MAX	   429
// Panel A8
#define AID_A8_MIN     430
#define AID_A8         430
#define AID_A8_MAX     459

#define AID_F8_MIN	   460
#define AID_F8		   460
#define AID_F8_MAX	   470

#define AID_A6_MIN     500
#define AID_A6         500
#define AID_A6_WND0  501
#define AID_A6_WND1  502
#define AID_A6_WND2  503
#define AID_A6_WND3  504
#define AID_A6_MAX     509

#define AID_R11_MIN		540
#define AID_R11			540
#define AID_R11_MAX		560

#define AID_F2_MIN		580
#define AID_F2			580
#define AID_F2_MAX		589

#define AID_F4_MIN		600
#define AID_F4			600
#define AID_F4_MAX		609

#define AID_A7U_MIN		640
#define AID_A7U			640
#define AID_A7U_MAX		660

#define AID_O17 		661

#define AID_F3_MIN	700
#define AID_F3		700
#define AID_F3_MAX	710

#define AID_L10_MIN	800
#define AID_L10		800
#define AID_L10_MAX	810

#define AID_L12U_CENTAUR_MIN	900
#define AID_L12U_CENTAUR	900
#define AID_L12U_CENTAUR_MAX	910

#define AID_L12U_IUS_MIN	920
#define AID_L12U_IUS		920
#define AID_L12U_IUS_MAX	930

#define AID_MDU_CDR1	1000
#define AID_MDU_CDR2	1001
#define AID_MDU_CRT1	1002
#define AID_MDU_CRT2	1003
#define AID_MDU_CRT3	1004
#define AID_MDU_CRT4	1005
#define AID_MDU_MFD1	1006
#define AID_MDU_MFD2	1007
#define AID_MDU_PLT1	1008
#define AID_MDU_PLT2	1009
#define AID_MDU_AFD1	1010

#define AID_A1U         1011

#define AID_A2_MIN	1020
#define AID_A2		1020
#define AID_A2_MAX	1030


const unsigned int AID_CUSTOM_PANELS_MIN = 10000;
const unsigned int AID_CUSTOM_PANELS_MAX = 30000;

const unsigned int AID_A7A3 = 11000;
const unsigned int AID_A8A3 = 11010;


#define SWITCH1		0
#define SWITCH2		1
#define SWITCH3		2
#define SWITCH4		3
#define SWITCH5		4
#define SWITCH6		5
#define SWITCH7		6
#define SWITCH8		7
#define SWITCH9		8
#define SWITCH10	9
#define SWITCH11	10
#define SWITCH12	11
#define SWITCH13	12
#define SWITCH14	13
#define SWITCH15	14
#define SWITCH16	15
#define SWITCH17	16
#define SWITCH18	17
#define SWITCH19	18
#define SWITCH20	19

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
	class AtlantisVCComponent;
	class BasicSwitch;
	class AtlantisPanel;
	class PanelR2;
	class DAPControl;

	typedef MDU* PMDU;
};

#endif
