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
#define AID_F6_PB1   204
#define AID_F6_PB2   205
#define AID_F6_MAX     219
// Panel F7 (event timer and MPS status)
#define AID_F7_MIN	     260
#define AID_F7	         260
#define AID_F7_SSME_C_R  265
#define AID_F7_SSME_C_Y  266
#define AID_F7_SSME_L_R  267
#define AID_F7_SSME_L_Y  268
#define AID_F7_SSME_R_R  269
#define AID_F7_SSME_R_Y  270
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
#define AID_C3_PBI1	   301
#define AID_C3_PBI2	   302
#define AID_C3_PBI3	   303
#define AID_C3_PBI4	   304
#define AID_C3_PBI5	   305
#define AID_C3_PBI6	   306
#define AID_C3_PBI7	   307
#define AID_C3_PBI8	   308
#define AID_C3_PBI9	   309
#define AID_C3_PBI10   310
#define AID_C3_PBI11   311
#define AID_C3_PBI12   312
#define AID_C3_PBI13   313
#define AID_C3_PBI14   314
#define AID_C3_PBI15   315
#define AID_C3_PBI16   316
#define AID_C3_PBI17   317
#define AID_C3_PBI18   318
#define AID_C3_PBI19   319
#define AID_C3_PBI20   320
#define AID_C3_PBI21   321
#define AID_C3_PBI22   322
#define AID_C3_PBI23   323
#define AID_C3_PBI24   324
#define AID_C3_MAX     329
// Panel O3
#define AID_O3_MIN	   330
#define AID_O3		   330
#define AID_O3_MAX	   349

#define AID_O6_MIN	   350
#define AID_O6		   350
#define AID_O6_MAX	   362

#define AID_O8_MIN	   380
#define AID_O8		   380
#define AID_O8_MAX	   390
// Panel R2
#define AID_R2_MIN     400
#define AID_R2         400
#define AID_R2_TKBK1   401
#define AID_R2_TKBK2   402
#define AID_R2_TKBK3   403
#define AID_R2_TKBK4   404
#define AID_R2_TKBK5   405
#define AID_R2_TKBK6   406
#define AID_R2_TKBK7   407
#define AID_R2_TKBK8   408
#define AID_R2_MAX     419

#define AID_A4_MIN	   420
#define AID_A4		   420
#define AID_A4_MAX	   429
// Panel A8
#define AID_A8_MIN     430
#define AID_A8         430
#define AID_A8_TKBK1   431
#define AID_A8_TKBK2   432
#define AID_A8_TKBK3   433
#define AID_A8_TKBK4   434
#define AID_A8_TKBK5   435
#define AID_A8_TKBK6   436
#define AID_A8_TKBK7   437
#define AID_A8_TKBK8   438
#define AID_A8_TKBK9   439
#define AID_A8_TKBK10  440
#define AID_A8_TKBK11  441
#define AID_A8_TKBK12  442
#define AID_A8_TKBK13  443
#define AID_A8_TKBK14  444
#define AID_A8_TKBK15  445
#define AID_A8_TKBK16  446
#define AID_A8_TKBK17  447
#define AID_A8_TKBK18  448
#define AID_A8_TKBK19  449
#define AID_A8_TKBK20  451
#define AID_A8_TKBK21  452
#define AID_A8_LED1	   453
#define AID_A8_LED2	   454
#define AID_A8_LED3	   455
#define AID_A8_MAX     459

#define AID_F8_MIN	   460
#define AID_F8		   460
#define AID_F8_PB1	464
#define AID_F8_PB2	465
#define AID_F8_MAX	   470

#define AID_A6_MIN     500
#define AID_A6         500
#define AID_A6_PBI1	   501
#define AID_A6_PBI2	   502
#define AID_A6_PBI3	   503
#define AID_A6_PBI4	   504
#define AID_A6_PBI5	   505
#define AID_A6_PBI6	   506
#define AID_A6_PBI7	   507
#define AID_A6_PBI8	   508
#define AID_A6_PBI9	   509
#define AID_A6_PBI10   510
#define AID_A6_PBI11   511
#define AID_A6_PBI12   512
#define AID_A6_PBI13   513
#define AID_A6_PBI14   514
#define AID_A6_PBI15   515
#define AID_A6_PBI16   516
#define AID_A6_PBI17   517
#define AID_A6_PBI18   518
#define AID_A6_PBI19   519
#define AID_A6_PBI20   520
#define AID_A6_PBI21   521
#define AID_A6_PBI22   522
#define AID_A6_PBI23   523
#define AID_A6_PBI24   524
#define AID_A6_TKBK1   525
#define AID_A6_TKBK2   526
#define AID_A6_TKBK3   527
#define AID_A6_TKBK4   528
#define AID_A6_TKBK5   529
#define AID_A6_TKBK6   530
#define AID_A6_TKBK7   531
#define AID_A6_TKBK8   532
#define AID_A6_TKBK9   533
#define AID_A6_TKBK10  534
#define AID_A6_WND0  535
#define AID_A6_WND1  536
#define AID_A6_WND2  537
#define AID_A6_WND3  538
#define AID_A6_MAX     539

#define AID_R11_MIN		540
#define AID_R11			540
#define AID_R11_MAX		560

#define AID_F2_MIN		580
#define AID_F2			580
#define AID_F2_BF_AUTO	581
#define AID_F2_BF_MAN	582
#define AID_F2_ST_AUTO	583
#define AID_F2_ST_MAN	584
#define AID_F2_P_CSS	585
#define AID_F2_RY_CSS	586
#define AID_F2_P_AUTO	587
#define AID_F2_RY_AUTO	588
#define AID_F2_DC_ARM	589
#define AID_F2_DC_DPY	590
#define AID_F2_MAX		599

#define AID_F4_MIN		600
#define AID_F4			600		
#define AID_F4_BF_AUTO	601
#define AID_F4_BF_MAN	602
#define AID_F4_ST_AUTO	603
#define AID_F4_ST_MAN	604
#define AID_F4_P_CSS	605
#define AID_F4_RY_CSS	606
#define AID_F4_P_AUTO	607
#define AID_F4_RY_AUTO	608
#define AID_F4_DC_JETT	609
#define AID_F4_MAX		620

#define AID_A7U_MIN		640
#define AID_A7U			640
#define AID_A7U_PBI1	641
#define AID_A7U_PBI2	642
#define AID_A7U_PBI3	643
#define AID_A7U_PBI4	644
#define AID_A7U_PBI5	645
#define AID_A7U_PBI6	646
#define AID_A7U_PBI7	647
#define AID_A7U_MAX		660

#define AID_O17 		661

#define AID_F3_MIN	700
#define AID_F3		700		
#define AID_F3_DC_ARM	701
#define AID_F3_DC_DPY	702
#define AID_F3_DC_JETT	703
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
const unsigned int AID_ODSLIGHTS = 11001;
const unsigned int AID_A8A3 = 11010;
const unsigned int AID_A8LIGHTS = 11100;


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
	const int MDUID_AFD  = 10;


	class MDU;
	class AtlantisVCComponent;
	class BasicSwitch;
	class AtlantisPanel;
	class PanelR2;
	class DAPControl;

	typedef MDU* PMDU;
};

#endif
