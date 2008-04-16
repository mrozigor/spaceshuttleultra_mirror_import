// ==============================================================
//                 ORBITER MODULE: Atlantis
//                  Part of the ORBITER SDK
//          Copyright (C) 2001-2003 Martin Schweiger
//                   All rights reserved
//
// Atlantis.cpp
// Reference implementation of Atlantis (Space Shuttle) vessel
// class module
//
// RMS, grappling and MMU capabilities by Robert Conley
// ==============================================================

#define STRICT 1
#define ORBITER_MODULE
#include "Atlantis.h"
#include "PlBayOp.h"
#include "GearOp.h"
#include "PanelA4.h"
#include "PanelA8.h"
#include "PanelC2.h"
#include "PanelC3.h"
#include "PanelF7.h"
#include "PanelO3.h"
#include "PanelR2.h"
#include "Keyboard.h"
#include "DlgCtrl.h"
#include "meshres.h"
#include "meshres_vc.h"
#include "meshres_RMS.h"
#include "meshres_KU.h"
#include "meshres_vc_additions.h"
#include "resource.h"
#include "SubsystemDirector.h"
#include "dps/MasterTimingUnit.h"
#include "AirDataProbeSystem.h"
//#include "PayloadBaySystem.h"
#ifdef INCLUDE_OMS_CODE
#include "OMSSubsystem.h"
#endif
#include "CommModeHandler.h"


#include "MLP/MLP.h"

#include <stdio.h>
#include <fstream>

//#include "StopWatch.h"

#ifdef _DEBUG
    // D. Beachy: for BoundsChecker debugging
    extern int GrowStack();
#endif

#define LOADBMP(id) (LoadBitmap (g_Param.hDLL, MAKEINTRESOURCE (id)))

// ==============================================================
// Global (class-wide) parameters

GDIParams g_Param;

char *ActionString[5] = {"STOPPED", "ISCLOSED", "ISOPEN", "CLOSE", "OPEN"};

HELPCONTEXT g_hc = {
  "html/vessels/Atlantis.chm",
  0,
  "html/vessels/Atlantis.chm::/Atlantis.hhc",
  "html/vessels/Atlantis.chm::/Atlantis.hhk"
};

// ==============================================================
// Local prototypes

BOOL CALLBACK Atlantis_DlgProc (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK RMS_DlgProc (HWND, UINT, WPARAM, LPARAM);
extern void GetSRB_State (double met, double &thrust_level, double &prop_level);


// ==============================================================
// Airfoil coefficient functions
// Return lift, moment and zero-lift drag coefficients as a
// function of angle of attack (alpha or beta)
// ==============================================================

// 1. vertical lift component (wings and body)

int tpir(const double* list, int n_items, double target) {
 // char buf[64];
  if(target<list[0]) return 0;
  if(target>=list[n_items-1]) return n_items-2;
//  sprintf(oapiDebugString(),"target %f n_items %d ",target,n_items);
  for(int i=1;i<n_items;i++) {
	  //if(i>10) {
	  //    sprintf(buf,"list[%d] %.2f ",i,list[i]);
	  //  strcat(oapiDebugString(),buf);
	  //}
	  if(list[i]>=target) {
		  //sprintf(buf,"result %d",i-1);
		//strcat(oapiDebugString(),buf);
		return i-1;
	  }
  }
 // sprintf(buf,"result %d",-46);
 // strcat(oapiDebugString(),buf);
  return -46;
}

double linterp(double x0, double y0, double x1, double y1, double x) {
  double t=(x-x0)/(x1-x0);
  return y0*(1-t)+y1*t;
}

double listerp(const double* listx, const double* listy, int n_items, double x) {
  int i=tpir(listx,n_items,x);
//  sprintf(oapiDebugString(),"i %d x0 %f y0 %f x1 %f y1 %f x %f",i,listx[i],listy[i],listx[i+1],listy[i+1],x);
  return linterp(listx[i],listy[i],listx[i+1],listy[i+1],x);
}

double tableterp(const double* table, const double* listrow, int n_row, const double* listcol, int n_col, double rowlookup, double collookup) {
  int i_row=tpir(listrow,n_row,rowlookup);
  double row0=listerp(listcol,&table[i_row*n_col],n_col,collookup);
  double row1=listerp(listcol,&table[(i_row+1)*n_col],n_col,collookup);
  double t=(rowlookup-listrow[i_row])/(listrow[i_row+1]-listrow[i_row]);
  double result=row0*(1-t)+row1*t;
//  sprintf(oapiDebugString(),"rowlookup %f collookup %f i_row %d row0 %f row1 %f t %f result %f",rowlookup,collookup,i_row,row0,row1,t,result);
  return result;
//  return 0;
}

const double RollOff=10*47.880259;
const double PitchOff=40*47.880259;
const double YawOff=1.0;

const int n_mach=21;
const double mach[n_mach]={0.25,0.4,0.6,0.8,0.85,0.9,0.92,0.95,0.98,1.05,1.1,1.2,1.3,1.5,2,2.5,3,4,5,8,10};
const int n_aoa1=19;
const double aoa1[n_aoa1]={-10,-5,-2.5,0,2.5,5,7.5,10,12.5,15,17.5,20,22.5,25,30,35,40,45,50};
const double clBase[n_aoa1][n_mach]={{-0.50280,-0.51097,-0.52957,-0.58454,-0.59773,-0.61005,-0.62831,-0.65514,-0.65616,-0.64808,-0.64225,-0.61158,-0.57681,-0.48844,-0.36885,-0.31514,-0.29383,-0.22874,-0.19854,-0.18641,-0.18641},
{-0.25906,-0.27397,-0.28670,-0.32761,-0.33790,-0.34601,-0.35039,-0.35897,-0.36172,-0.36027,-0.35685,-0.33449,-0.30454,-0.25188,-0.19695,-0.16705,-0.16922,-0.13893,-0.12424,-0.12402,-0.12402},
{-0.15202,-0.16199,-0.17186,-0.19155,-0.19632,-0.20588,-0.21553,-0.21485,-0.19888,-0.20310,-0.19292,-0.18275,-0.16776,-0.12989,-0.09888,-0.10456,-0.10514,-0.10050,-0.09081,-0.08913,-0.08913},
{-0.05000,-0.05000,-0.05500,-0.06500,-0.06200,-0.06000,-0.06000,-0.06000,-0.05500,-0.04700,-0.03600,-0.02500,-0.02300,-0.01500,-0.01700,-0.04290,-0.03700,-0.04430,-0.04724,-0.05540,-0.05540},
{0.06214,0.06212,0.07201,0.06176,0.06652,0.07101,0.07563,0.08498,0.09414,0.10315,0.10796,0.10460,0.10295,0.09317,0.08416,0.02995,0.02048,0.01111,0.00642,-0.02023,-0.02023},
{0.17453,0.17448,0.17928,0.18859,0.19295,0.20356,0.21579,0.23947,0.24762,0.25552,0.25513,0.25014,0.24043,0.21606,0.16125,0.10468,0.08600,0.06174,0.04301,0.01774,0.01774},
{0.29705,0.29691,0.30657,0.31963,0.31838,0.32413,0.34997,0.38783,0.38955,0.39173,0.39121,0.38627,0.37178,0.32805,0.24173,0.18906,0.15609,0.11881,0.09656,0.05965,0.05965},
{0.40448,0.40924,0.41674,0.44384,0.44172,0.44572,0.47690,0.51887,0.52899,0.53602,0.53054,0.51767,0.49146,0.42874,0.33103,0.27135,0.22528,0.18185,0.15220,0.10604,0.10604},
{0.51599,0.52757,0.54905,0.56480,0.56566,0.56794,0.58585,0.64660,0.66026,0.66393,0.66357,0.63927,0.61835,0.54671,0.41958,0.35229,0.29513,0.24730,0.21351,0.16115,0.16115},
{0.63581,0.65002,0.67426,0.68162,0.67309,0.68323,0.70567,0.76535,0.77783,0.78377,0.77863,0.75952,0.72567,0.64381,0.50777,0.43483,0.36604,0.31897,0.27892,0.22632,0.22632},
{0.77676,0.78385,0.79709,0.79087,0.77995,0.79401,0.81577,0.86501,0.89069,0.89596,0.89086,0.85608,0.83073,0.74311,0.60244,0.52033,0.44227,0.39229,0.35079,0.29998,0.29998},
{0.91657,0.92529,0.92851,0.89578,0.87690,0.89485,0.91619,0.96428,0.99805,1.00286,1.00255,0.95636,0.92538,0.83834,0.68871,0.60168,0.52215,0.46559,0.42351,0.36492,0.35587},
{1.00634,1.01512,1.01007,0.93184,0.91266,0.92074,0.95994,1.00227,1.04837,1.06699,1.06660,1.03541,1.00114,0.91858,0.76904,0.67886,0.60751,0.53159,0.49654,0.44356,0.43474},
{1.08364,1.08846,1.04489,0.92283,0.90795,0.93837,0.96751,1.02232,1.07579,1.10328,1.11188,1.09985,1.07176,0.99205,0.84561,0.76551,0.68386,0.61884,0.57019,0.51805,0.50936},
{1.30045,1.30623,1.25395,1.10747,1.08961,1.12612,1.16109,1.22686,1.29103,1.32402,1.33434,1.31990,1.28619,1.19054,0.99835,0.91867,0.84307,0.76946,0.72044,0.66845,0.64465},
{1.51313,1.51986,1.45902,1.28858,1.26781,1.31028,1.35097,1.42751,1.50217,1.54055,1.55256,1.53576,1.49654,1.38524,1.16162,1.06891,0.97219,0.89530,0.84607,0.79881,0.78382},
{1.69584,1.70339,1.63520,1.44418,1.42090,1.46850,1.51411,1.59988,1.68356,1.72658,1.74004,1.72121,1.67725,1.55251,1.30189,1.19799,1.07919,1.00341,0.96129,0.91017,0.88400},
{1.82367,1.83178,1.75845,1.55304,1.52800,1.57919,1.62823,1.72047,1.81045,1.85672,1.87119,1.85095,1.80367,1.66953,1.40002,1.28828,1.15575,1.07904,1.03683,0.98747,0.96803},
{1.91170,1.92021,1.84334,1.62801,1.60176,1.65542,1.70683,1.80353,1.89785,1.94635,1.96152,1.94030,1.89074,1.75012,1.46761,1.35047,1.21154,1.13113,1.08629,1.03514,1.02007}};

const double cdBase[n_aoa1][n_mach]={{0.11485,0.11975,0.13521,0.16897,0.18654,0.19703,0.20867,0.22925,0.25227,0.26933,0.27358,0.27477,0.27037,0.25326,0.21167,0.18859,0.17640,0.15782,0.14843,0.14122,0.14122},
{0.07985,0.08119,0.08489,0.09953,0.10785,0.11831,0.12552,0.14223,0.16817,0.18470,0.18942,0.19259,0.19067,0.18445,0.15837,0.14100,0.13105,0.11936,0.11165,0.10501,0.10501},
{0.07170,0.07263,0.07517,0.08323,0.08675,0.09887,0.10710,0.12269,0.14461,0.16251,0.16647,0.17013,0.16958,0.16572,0.14245,0.12598,0.11599,0.10538,0.09795,0.09067,0.09067},
{0.06740,0.06610,0.07020,0.07630,0.08000,0.09070,0.09900,0.11450,0.13460,0.15410,0.15840,0.16140,0.16110,0.15770,0.13490,0.11860,0.10680,0.09430,0.08760,0.08000,0.08000},
{0.06687,0.05747,0.07011,0.07557,0.08048,0.09309,0.10180,0.11702,0.13644,0.16935,0.16376,0.16502,0.16395,0.15852,0.13500,0.11632,0.10399,0.08937,0.08226,0.07359,0.07359},
{0.07038,0.07096,0.07350,0.08175,0.08916,0.10574,0.11515,0.13127,0.15286,0.17724,0.18173,0.18139,0.17763,0.16938,0.14189,0.12068,0.10700,0.09063,0.08096,0.07142,0.07140},
{0.08056,0.06145,0.08464,0.10038,0.10990,0.12750,0.13806,0.15696,0.18221,0.20398,0.20794,0.20709,0.20226,0.18364,0.15579,0.13301,0.11677,0.09815,0.08634,0.07402,0.07348},
{0.09599,0.09775,0.10146,0.13088,0.14389,0.16155,0.17477,0.19588,0.22487,0.24256,0.24485,0.24298,0.23613,0.21755,0.17850,0.15244,0.13324,0.11238,0.09853,0.08216,0.08059},
{0.10625,0.12444,0.13381,0.17827,0.19090,0.21031,0.22196,0.24823,0.27902,0.29479,0.29645,0.29055,0.28325,0.25989,0.20958,0.17951,0.15649,0.13380,0.11760,0.09780,0.09544},
{0.15784,0.16278,0.18832,0.23803,0.25555,0.27066,0.28350,0.31191,0.34859,0.35909,0.35895,0.35301,0.34037,0.30958,0.24932,0.21476,0.18711,0.16342,0.14482,0.12245,0.11931},
{0.21346,0.21758,0.26637,0.30939,0.31973,0.34168,0.35577,0.38252,0.41746,0.43401,0.43355,0.42081,0.40809,0.37008,0.30079,0.25990,0.22679,0.20107,0.18106,0.15697,0.15302},
{0.26859,0.29389,0.35519,0.38857,0.39759,0.42137,0.43584,0.46452,0.50331,0.51932,0.52016,0.50090,0.48367,0.43922,0.35911,0.31286,0.27582,0.24672,0.22523,0.19614,0.19178},
{0.36651,0.37144,0.43592,0.45493,0.46003,0.48172,0.50467,0.53313,0.57864,0.59902,0.59994,0.58377,0.56200,0.51308,0.42462,0.37380,0.33639,0.29758,0.27765,0.24878,0.24394},
{0.45537,0.45790,0.50401,0.50538,0.51675,0.54316,0.56348,0.59974,0.65071,0.67457,0.67979,0.67010,0.64751,0.59368,0.49836,0.44700,0.40275,0.36614,0.33893,0.30876,0.30372},
{0.62879,0.63228,0.69595,0.69784,0.71354,0.75001,0.77807,0.82814,0.89852,0.93146,0.93867,0.92529,0.89410,0.81977,0.67501,0.61723,0.56977,0.52265,0.49216,0.45821,0.44343},
{0.85388,0.85863,0.94509,0.94766,0.96898,1.01850,1.05660,1.12460,1.22017,1.26491,1.27470,1.25653,1.21417,1.11323,0.91665,0.83819,0.76478,0.70975,0.67287,0.63575,0.62404},
{1.11596,1.12216,1.23516,1.23852,1.26638,1.33111,1.38090,1.46976,1.59467,1.65315,1.66594,1.64219,1.58683,1.45491,1.19800,1.09545,0.99301,0.92759,0.89082,0.84440,0.82113},
{1.40622,1.41403,1.55642,1.56065,1.59576,1.67732,1.74007,1.85204,2.00944,2.08312,2.09924,2.06932,1.99956,1.83333,1.50959,1.38037,1.24741,1.16885,1.12550,1.07445,1.05430},
{1.74092,1.75059,1.92688,1.93211,1.97558,2.07655,2.15424,2.29286,2.48772,2.57894,2.59890,2.56185,2.47549,2.26969,1.86890,1.70892,1.54432,1.44706,1.38996,1.33019,1.31104}};

const double cmBase[n_aoa1][n_mach]={{0.03200,0.03400,0.04000,0.05900,0.06800,0.09000,0.10100,0.12600,0.14000,0.15800,0.16250,0.16200,0.14800,0.01300,0.04200,0.00450,-0.01800,-0.02940,-0.03370,-0.03500,-0.03500},
{0.03000,0.03200,0.03650,0.04900,0.05400,0.06100,0.06700,0.08200,0.09800,0.11400,0.11800,0.11200,0.09200,0.06000,0.01750,-0.00800,-0.01800,-0.02700,-0.03070,-0.03280,-0.03280},
{0.02900,0.03050,0.03470,0.04450,0.04700,0.04500,0.05300,0.06200,0.07800,0.08400,0.08200,0.07200,0.05800,0.03700,0.00800,-0.01144,-0.01850,-0.02550,-0.02800,-0.03000,-0.03000},
{0.02800,0.29000,0.03300,0.04000,0.04000,0.03500,0.03800,0.04200,0.05500,0.05500,0.04700,0.03400,0.02500,0.01400,-0.01300,-0.01480,-0.01900,-0.02310,-0.02400,-0.02700,-0.02700},
{0.02700,0.02750,0.03050,0.03600,0.03800,0.03500,0.02200,0.02000,0.03100,0.02400,0.01500,0.00600,-0.00100,-0.00700,-0.01300,-0.01940,-0.01970,-0.02100,-0.02200,-0.02350,-0.02350},
{0.02600,0.02600,0.02800,0.03150,0.03200,0.02250,0.01600,0.00000,0.00500,-0.00300,-0.01050,-0.02000,-0.02350,-0.02500,-0.02380,-0.02300,-0.02050,-0.01880,-0.02000,-0.01900,-0.01900},
{0.02400,0.02400,0.02400,0.02250,0.02200,0.02000,0.01200,-0.01600,-0.01500,-0.02200,-0.02900,-0.04000,-0.04400,-0.04100,-0.03360,-0.02480,-0.02150,-0.01820,-0.01810,-0.01560,-0.01550},
{0.02300,0.02200,0.02050,0.01800,0.01700,0.01500,0.00400,-0.02700,-0.03000,-0.03400,-0.04000,-0.05400,-0.06000,-0.05500,-0.03850,-0.02740,-0.02250,-0.01830,-0.01660,-0.01250,-0.01200},
{0.02300,0.02250,0.02100,0.01700,0.01200,0.00750,-0.00600,-0.04000,-0.04200,-0.04500,-0.04900,-0.06100,-0.07200,-0.07000,-0.04300,-0.03100,-0.02400,-0.01910,-0.01560,-0.00990,-0.00970},
{0.02400,0.02200,0.01600,0.00600,0.00050,-0.00250,-0.01800,-0.05000,-0.05200,-0.05800,-0.06200,-0.07200,-0.08300,-0.08200,-0.04720,-0.03510,-0.02700,-0.02010,-0.01520,-0.00725,-0.00780},
{0.01900,0.01600,0.00900,-0.00600,-0.01100,-0.01000,-0.02800,-0.05650,-0.06300,-0.07000,-0.07400,-0.08200,-0.09000,-0.09100,-0.05160,-0.03950,-0.03000,-0.02150,-0.01510,-0.00493,-0.00600},
{0.01400,0.00800,-0.00400,-0.00500,-0.01000,-0.00300,-0.03000,-0.06000,-0.07200,-0.07700,-0.08100,-0.08800,-0.09300,-0.09700,-0.05810,-0.04400,-0.03400,-0.02310,-0.01550,-0.00300,-0.00600},
{0.01050,0.01000,0.00600,0.00260,0.02000,0.02000,-0.01200,-0.03500,-0.04700,-0.06100,-0.06800,-0.08000,-0.09100,-0.09500,-0.06150,-0.05000,-0.03700,-0.02350,-0.01700,-0.00400,-0.00620},
{0.10000,0.01200,0.03300,0.08000,0.09000,0.09000,0.08700,0.07000,0.02000,-0.02300,-0.04200,-0.06400,-0.07700,-0.08200,-0.06490,-0.05530,-0.04200,-0.02780,-0.01840,-0.00590,-0.00800},
{0.12007,0.01441,0.03962,0.09606,0.10807,0.10807,0.10446,0.08405,0.02401,-0.02762,-0.05043,-0.07685,-0.09246,-0.09846,-0.05180,-0.06640,-0.05400,-0.03720,-0.02700,-0.01350,-0.01500},
{0.15655,0.01879,0.05166,0.12524,0.14089,0.14089,0.13619,0.10958,0.03131,-0.03601,-0.06575,-0.10019,-0.12054,-0.12837,-0.06753,-0.08657,-0.06600,-0.04850,-0.03940,-0.02570,-0.02600},
{0.19818,0.02378,0.06540,0.15855,0.17837,0.17837,0.17242,0.13873,0.03964,-0.04558,-0.08324,-0.12684,-0.15260,-0.16251,-0.08550,-0.10960,-0.08300,-0.06140,-0.05300,-0.04290,-0.04100},
{0.24466,0.02936,0.08074,0.19573,0.22020,0.22020,0.21286,0.17126,0.04893,-0.05627,-0.10276,-0.15658,-0.18839,-0.20062,-0.10555,-0.13530,-0.10000,-0.07580,-0.06750,-0.06150,-0.06100},
{0.32582,0.03910,0.10752,0.26066,0.29324,0.29324,0.28346,0.22807,0.06516,-0.07494,-0.13684,-0.20852,-0.25088,-0.26717,-0.14056,-0.18018,-0.13317,-0.10094,-0.08330,-0.08190,-0.08200}};

const int n_trimExt=3;
const double trimExt[n_trimExt]={-11.7,0,22.5};
const double cmTrim[n_trimExt][n_mach]={{0.0158,0.016,0.0179,0.0204,0.020165,0.01993,0.020866,0.02149,0.01803,0.01574,0.0136,0.0148,0.01141,0.00981,0.00578,0.004215,0.00265,0.00603,0.00791,0.012,0.015},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{-0.0424,-0.0442,-0.0488,-0.05562,-0.0559,-0.05618,-0.058004,-0.05922,-0.05222,-0.05241,-0.04894,-0.04195,-0.03722,-0.02979,-0.02112,-0.016505,-0.01189,-0.0328,-0.0442,-0.061,-0.065}};

const double OrbiterS=2690*0.3048*0.3048;
const double Orbiterb=78.056*0.3048; //Turns out span is not used...
const double Orbiterc=39.56*0.3048; //...but chord is
const double OrbiterA=Orbiterb*Orbiterb/OrbiterS; //Has to be passed, doesn't have to be correct
void FlatPlateCoeff (double aoa, double *cl, double *cm, double *cd) {
  *cl=1.2*sin(aoa*2);
  *cd=2*sin(aoa);
  *cm=0;
}

/*
void VLiftCoeff (VESSEL *v, double aoa, double M, double Re, void* lv, double *cl, double *cm, double *cd) {
  if(M<mach[0])M=mach[0];
  if(M>mach[n_mach-1])M=mach[n_mach-1];
  static const int nabsc=11;
  static const double cmAoA[nabsc]={    -75.0,  -60.0,  -45.0,  -30.0,  -15.0,   0.0,   15.0,      30.0,     45.0,  60.0,  75.0 };
  static const double CM[nabsc] = {    0,     0,  0.002,  0.004, 0.0025,0.0012,      0,-0.0012,-0.0007,    0,    0};
  double bfDeploy=-22.5*v->GetControlSurfaceLevel(AIRCTRL_ELEVATORTRIM); //In data book, aft end of body flap up
                                                                         //"up trim" is negative. In Orbiter, this is positive.
  if(aoa*180.0/PI<aoa1[0] || aoa*180.0/PI>aoa1[n_aoa1-1]) {
    FlatPlateCoeff(aoa,cl,cm,cd);
    sprintf(oapiDebugString(),"FlatPlate AoA: %f Cl: %f Cd: %f Cm: %f",aoa*180.0/PI,*cl,*cd,*cm);
  } else {
    *cl=tableterp(&clBase[0][0], aoa1, n_aoa1, mach, n_mach, aoa*180.0/PI,M);
    *cd=tableterp(&cdBase[0][0], aoa1, n_aoa1, mach, n_mach, aoa*180.0/PI,M);
  *cm=listerp(cmAoA,CM,nabsc,aoa);
//    *cm=tableterp(&cmBase[0][0], aoa1, n_aoa1, mach, n_mach, aoa*180.0/PI,M);
//  *cm+=tableterp(&cmTrim[0][0], trimExt, n_trimExt, mach, n_mach, bfDeploy,M);
  sprintf(oapiDebugString(),"TableM: %f TableAoA: %f Cl: %f Cd: %f Cm: %f bf: %f",M,aoa*180.0/PI,*cl,*cd,*cm,bfDeploy);
  }
}
*/
void VLiftCoeff (VESSEL* vv, double aoa, double M, double Re, void* stuff, double *cl, double *cm, double *cd){
  Atlantis* v=(Atlantis*)vv;
    if(M<mach[0])M=mach[0];
    if(M>mach[n_mach-1])M=mach[n_mach-1];
    double Tablecl=tableterp(&clBase[0][0], aoa1, n_aoa1, mach, n_mach, aoa*180.0/PI,M);
    double Tablecd=tableterp(&cdBase[0][0], aoa1, n_aoa1, mach, n_mach, aoa*180.0/PI,M);
  static const double step = RAD*15.0;
  static const double istep = 1.0/step;
  static const int nabsc = 25;
  //                              -180 -165  -150 -135  -120 -105 -90 -75.0  -60.0  -45.0  -30.0  -15.0   0.0   15.0      30.0     45.0  60.0  75.0  90.0 105.0  120.0  135.0  150.0  165.0 180.0
  static const double CL[nabsc] = {0.1, 0.17, 0.2, 0.2, 0.17, 0.1, 0, -0.11, -0.24, -0.38,  -0.5,  -0.5, -0.02, 0.6355,    0.63,   0.46, 0.28, 0.13, 0.0, -0.16, -0.26, -0.29, -0.24, -0.1, 0.1};
  static const double CM[nabsc] = {  0,    0,   0,   0,    0,   0, 0,     0,     0,  0.002,  0.004, 0.0025,0.0012,      0,-0.0012,-0.0007,    0,    0,   0,     0,     0,     0,     0,    0,   0};
  // lift and moment coefficients from -180 to 180 in 15 degree steps.
  // This uses a documented lift slope of 0.0437/deg, everything else is rather ad-hoc

  // change for drag increase at high Mach:
  // Subsonic Equivalents

  double Cl0, Cl1, Cd0, Cd1, Cd2;
  if(M>=5.0) {
    Cl0 = -0.207040;
    Cl1 =  1.675600;
    Cd0 =  0.078540;
//    Cd1 = -0.352900;
//    Cd2 =  2.039960;

// at 40 deg Alpha, Cd = .078540 - .24637 + .994252 = .826422 Cl = .96275
// want to reduce it 15%
// at 25 deg, it's ok (Cd = .3129)
// then, Cd1p = (.1142-.1904*(Cd40p - Cd0))/.0798
// Cd2p = (.2344-.4363*Cd1p)/.1904
// Cl1p = (Cl40p-.5241)/.2618
// Cl0p = .5241 - Cl1p*.4363

// Cd40p = .7025 (Baseline -15%)

    Cd1 = -0.05767;
    Cd2 =  1.3632;

// add 5% (Cd40p = .86774)

//    Cd1 = -0.4519;
//    Cd2 = 2.2666;

    *cl = Cl0 + Cl1*aoa;
    *cd = Cd0 + Cd1*aoa + Cd2*aoa* aoa;

  }
  int idx = max (0, min (23, (int)((aoa+PI)*istep)));
  double d = (aoa+PI)*istep - idx;
  *cm = (CM[idx] + (CM[idx+1]-CM[idx])*d);
  if(M<1.25) {
    *cl = CL[idx] + (CL[idx+1]-CL[idx])*d;
//    *cd = 0.05 + oapiGetInducedDrag (*cl, 2.266, 0.75);
    *cd = 0.05 + oapiGetInducedDrag (*cl, 2.266, 0.75);
  }
  else if(M<5.0) {
    *cl = CL[idx] + (CL[idx+1]-CL[idx])*d;
//    *cd = 0.09 + oapiGetInducedDrag (*cl, 2.266, 0.5);
    *cd = 0.09 + oapiGetInducedDrag (*cl, 2.266, 0.75);

  }
//  sprintf(oapiDebugString(),"P%d Y%d R%d TableM: %f TableAoA: %f Table Cl: %f AFCS Cl: %f Table Cd: %f AFCS Cd: %f",v->PitchActive,v->YawActive,v->RollActive,M,aoa*180.0/PI,Tablecl,*cl,Tablecd,*cd);
  else {
	*cl=Tablecl;
	*cd=Tablecd;
  }
//    *cm=tableterp(&cmBase[0][0], aoa1, n_aoa1, mach, n_mach, aoa*180.0/PI,M);
//  *cm+=tableterp(&cmTrim[0][0], trimExt, n_trimExt, mach, n_mach, bfDeploy,M);
}

// 2. horizontal lift component (vertical stabiliser and body)

void HLiftCoeff (double beta, double M, double Re, double *cl, double *cm, double *cd)
{
  static const double step = RAD*22.5;
  static const double istep = 1.0/step;
  static const int nabsc = 17;
  static const double CL[nabsc] = {0, 0.2, 0.3, 0.2, 0, -0.2, -0.3, -0.2, 0, 0.2, 0.3, 0.2, 0, -0.2, -0.3, -0.2, 0};

  beta += PI;
  int idx = max (0, min (15, (int)(beta*istep)));
  double d = beta*istep - idx;
  *cl = CL[idx] + (CL[idx+1]-CL[idx])*d;
  *cm = 0.0;
  *cd = 0.02 + oapiGetInducedDrag (*cl, 1.5, 0.6);
}

// ==============================================================
// Specialised vessel class Atlantis
// ==============================================================

// --------------------------------------------------------------
// Constructor
// --------------------------------------------------------------
Atlantis::Atlantis (OBJHANDLE hObj, int fmodel)
: VESSEL2 (hObj, fmodel)
{
#ifdef _DEBUG
        // D. Beachy: for BoundsChecker debugging
        GrowStack();
#endif
  int i;

  plop            = new PayloadBayOp (this);
  gop             = new GearOp (this);
  panela4		  = new PanelA4(this);
  panela8		  = new PanelA8(this);
  c3po            = new PanelC3(this);
  r2d2            = new PanelR2(this);
  panelc2		  = new PanelC2(this);
  panelf7		  = new PanelF7(this);
  panelo3		  = new PanelO3(this);
  CDRKeyboard     = new Keyboard(this, 0);
  PLTKeyboard     = new Keyboard(this, 1);

  pCommModeHandler= new CommModeHandler(this);

  psubsystems	  = new SubsystemDirector(this);

  psubsystems->AddSubsystem(pMTU = new dps::MasterTimingUnit(psubsystems));
  psubsystems->AddSubsystem(pADPS = new AirDataProbeSystem(psubsystems));
#ifdef INCLUDE_OMS_CODE
  psubsystems->AddSubsystem(pOMS = new OMSSubsystem(psubsystems));
#else
  pOMS = NULL;
#endif
  status          = 3;
  ldoor_drag      = rdoor_drag = 0.0;
  spdb_status     = AnimState::CLOSED;
  spdb_proc       = 0.0;
  mesh_orbiter    = MESH_UNDEFINED;
  mesh_cockpit    = MESH_UNDEFINED;
  mesh_vc         = MESH_UNDEFINED;
  mesh_tank       = MESH_UNDEFINED;
  mesh_srb[0] = mesh_srb[1] = MESH_UNDEFINED;
  mesh_kuband	  = MESH_UNDEFINED;

  vis             = NULL;

  reset_mmu       = false;
  reset_sat       = false;
  jettison_time   = 0.0;
  render_cockpit  = false;
  bSRBCutoffFlag  = false;
  bLiftOff		  = false;
  bHasKUBand	  = true;
  bUseRealRCS	  = true;

  ___iCurrentManifold = 0;

  //SRB slag effects
  slag1 = 0.0;
  slag2 = 0.0;
  slag3 = 0.0;

  pshSlag1[0] = pshSlag1[1] = NULL;
  pshSlag2[0] = pshSlag2[1] = NULL;
  pshSlag3[0] = pshSlag3[1] = NULL;

  /*
  int mfdgrp[10] = {
    GRP_CDR1_VC,GRP_CDR2_VC,GRP_PLT1_VC,GRP_PLT2_VC,
    GRP_MFD1_VC, GRP_MFD2_VC, GRP_MFD3_VC, GRP_MFD4_VC, GRP_MFD5_VC,
    GRP_MFD_aft_VC};
  */

  int mfdgrp[11] = {
    GRP_CDR1_VC,GRP_CDR2_VC,GRP_PLT1_VC,GRP_PLT2_VC,
    GRP_MFD1_VC, GRP_MFD4_VC, GRP_MFD3_VC, GRP_MFD_aft_VC, GRP_MFD2_VC, GRP_MFD5_VC,
	NULL
    };

  for (i = 0; i < 11; i++) {
    mfds[i].ngroup   = mfdgrp[i];
    mfds[i].flag     = MFD_SHOWMODELABELS;
    mfds[i].nbt1     = 5;
    mfds[i].nbt2     = 0;
    mfds[i].bt_yofs  = 256/6;
    mfds[i].bt_ydist = 256/7;
  }
  for (i = 0; i < 11; i++)
    mfdbright[i] =  1.0;
  huds.ngroup       = GRP_VirtualHUD_VC;
  huds.size         = 0.176558;

  // propellant resources
  ph_oms          = NULL;
  ph_frcs		  = NULL;
  ph_lrcs		  = NULL;
  ph_rrcs		  = NULL;
  ph_tank         = NULL;
  ph_srb          = NULL;
  thg_main        = NULL;
  thg_retro		  = NULL;
  thg_srb         = NULL;


  for(i=0;i<4;i++)
  {
	thManFRCS1[i] = NULL;
	thManFRCS2[i] = NULL;
	thManFRCS3[i] = NULL;
  }

  thManFRCS4[0] = thManFRCS4[1] = NULL;
  thManFRCS5[0] = thManFRCS5[1] = NULL;

  for(i=0;i<3;i++)
  {
	thManLRCS1[i] = NULL;
	thManLRCS2[i] = NULL;
	thManLRCS3[i] = NULL;
	thManLRCS4[i] = NULL;
	thManRRCS1[i] = NULL;
	thManRRCS2[i] = NULL;
	thManRRCS3[i] = NULL;
	thManRRCS4[i] = NULL;
  }
  thManLRCS5[0] = thManLRCS5[1] = NULL;
  thManRRCS5[0] = thManRRCS5[1] = NULL;

  oms_helium_tank[0] = NULL;
  oms_helium_tank[1] = NULL;
  for(i=0;i<3;i++) {
	  apu_tank[i] = NULL;
	  mps_helium_tank[i] = NULL;
	  ex_main[i] = NULL;
  }

  //Control Surfaces
  hrudder	= NULL;
  hbodyflap	= NULL;
  helevator	= NULL;
  hlaileron	= NULL;
  hraileron	= NULL;

  // preload meshes
  hOrbiterMesh        = oapiLoadMeshGlobal (DEFAULT_MESHNAME_ORBITER);
  hOrbiterCockpitMesh = oapiLoadMeshGlobal (DEFAULT_MESHNAME_COCKPIT);
  hOrbiterVCMesh      = oapiLoadMeshGlobal (DEFAULT_MESHNAME_VC);
  hOrbiterRMSMesh	  = oapiLoadMeshGlobal (DEFAULT_MESHNAME_RMS);
  hKUBandMesh		  = oapiLoadMeshGlobal (DEFAULT_MESHNAME_KU);
  hTankMesh           = oapiLoadMeshGlobal (DEFAULT_MESHNAME_ET);
  hSRBMesh[0]		  = oapiLoadMeshGlobal (DEFAULT_MESHNAME_RSRB);
  hSRBMesh[1]		  = oapiLoadMeshGlobal (DEFAULT_MESHNAME_LSRB);

   tex_rcs = oapiRegisterExhaustTexture ("Exhaust_atrcs");
  
  //hSRBMesh            = oapiLoadMeshGlobal ("Atlantis_srb");

  strcpy(WingName,"Atlantis");

  DefineAnimations();
  center_arm      = false;
  arm_moved       = false;
  bManualSeparate = false;
  ofs_sts_sat     = _V(0,0,0);
  do_eva          = false;
  do_plat         = false;
  do_cargostatic  = false;
  vis             = NULL;
  sat_attach      = NULL;
  rms_attach      = NULL;
  //080415, DaveS add: Added temporary OBSS MPM attachment point
  obss_attach     = NULL;
  ahHDP			  = NULL;
  cargo_static_ofs   =_V(0,0,0);

  // default arm status: deployed
  DisplayJointAngles=false;
  RMS=false;
  RMSRollout.Set(AnimState::OPEN, 1);
  shoulder_brace = 0.0; //released
  //MRL[0]=MRL[1] = 0.0; //released
  for(i=0;i<2;i++) {
	  MRL[i]=0.0; //released
	  MRL_FwdMicroswitches[i][0]=MRL_MidMicroswitches[i][0]=MRL_AftMicroswitches[i][0]=0;
	  MRL_FwdMicroswitches[i][1]=MRL_MidMicroswitches[i][1]=MRL_AftMicroswitches[i][1]=0;
	  MRL_FwdMicroswitches[i][2]=MRL_MidMicroswitches[i][2]=MRL_AftMicroswitches[i][2]=1;
	  MPM_Microswitches[i][0]=0;
	  MPM_Microswitches[i][1]=1;
  }
  arm_sy = 0.5;
  arm_sp = 0.0136;
  arm_ep = 0.014688;
  arm_wp = 0.5;
  arm_wy = 0.5;
  arm_wr = 0.5;
  //wrist_yaw_joint[0] = _V(-2.87, 2.03, -4.88);
  //wrist_yaw_joint[1] = _V(-2.87, 2.03, -4.88)-_V(0.314338082679218, -0.949311102735849, 0);
  //wrist_yaw_joint[1] = _V(-2.87, 2.03, -4.88)+RotateVectorZ(_V(0.0, 1.0, 0.0), 18.435);
  arm_tip[0] = _V(-2.77, 2.13, -6.049);
  arm_tip[1] = _V(-2.77, 2.13, -7.049);
  arm_tip[2] = _V(-2.77, 3.13, -6.049);
  arm_ee_dir = _V(1.0, 0.0, 0.0);
  arm_ee_pos = _V(15.069, 0.0, 0.0);

  // default camera positions
  camFLyaw = 0;
  camFLpitch = 0;
  camFRyaw = 0;
  camFRpitch = 0;
  camBLyaw = 0;
  camBLpitch = 0;
  camBRyaw = 0;
  camBRpitch = 0;

  cameraLowRate = false;
  cameraMoved = false;
  cameraControl = 0;

  pl_mass=0.0;

  newmfd=NULL;

  //Launch Guidance
  MaxThrust=104.5; //104.5% thrust
  Throttle_Bucket[0]=834*fps_to_ms;
  Throttle_Bucket[1]=1174*fps_to_ms;
  OMS_Assist[0]=0.0;
  OMS_Assist[1]=0.0;
  RollToHeadsUp=100000.0;
  bAutopilot=false;
  bThrottle=true;
  bMECO=false;
  bZThrust=false;
  bEngineFail=false;
  bCommMode = false;
  tSRBSep=SRB_SEPARATION_TIME;
  TLastMajorCycle=0.0;

  // gpc
  SMOps=201;
  ops=101;
  last_mfd=0;
  //bFirstStep=true;
  ThrAngleP=-13.20;
  ThrAngleY=0.0;
  //Displays
  CRT_SEL[0]=2; //CRT3
  CRT_SEL[1]=1; //CRT2
  //MNVR
  MNVRLOAD=false;
  TIG[0]=TIG[1]=TIG[2]=TIG[3]=0.0;
  OMSGimbal[0][0]=OMSGimbal[0][1]=0;
  OMSGimbal[1][0]=OMSGimbal[1][1]=0;
  TV_ROLL=0;
  BurnInProg=false;
  BurnCompleted=false;
  for(i=0; i<3; i++) {
	  //MNVR
	  PEG7.data[i]=0.0;
	  //UNIV PTG
	  MNVR_OPTION.data[i]=0.0;
	  TRKROT_OPTION.data[i]=0.0;
	  REQD_ATT.data[i]=0.0;
	  LVLHOrientationReqd.data[i]=0.0;
	  TargetAtt.data[i]=0.0;
	  //Initialize Keyboard Input
	  DataInput[i].OPS=false;
	  DataInput[i].ITEM=false;
	  DataInput[i].SPEC=false;
	  DataInput[i].PRO=false;
	  DataInput[i].EXEC=false;
	  DataInput[i].NewEntry=false;
	  //DataInput[i].input="";
	  sprintf(DataInput[i].input, "");
	  DataInput[i].InputSize=0;
	  Display[i]=NULL;
	  //Initialize DAP Config
	  DAP[i].PRI_ROT_RATE=RotRate=1.0;
	  DAP[i].PRI_ATT_DB=AttDeadband=0.5;
	  DAP[i].PRI_RATE_DB=RateDeadband=0.1;
	  DAP[i].PRI_ROT_PLS=0.1;
	  DAP[i].PRI_COMP=0.0;
  	  DAP[i].PRI_TRAN_PLS=0.2;
	  DAP[i].PRI_P_OPTION=0;
	  DAP[i].PRI_Y_OPTION=0;
	  DAP[i].ALT_RATE_DB=0.2;
	  DAP[i].ALT_ON_TIME=0.5;
	  DAP[i].ALT_DELAY=0.5;
	  DAP[i].ALT_JET_OPT=0;
	  DAP[i].ALT_JETS=2;
	  DAP[i].VERN_ROT_RATE=0.25;
	  DAP[i].VERN_ATT_DB=0.1;
	  DAP[i].VERN_RATE_DB=0.01;
	  DAP[i].VERN_ROT_PLS=0.1;
	  DAP[i].VERN_COMP=0.0;
	  DAP[i].VERN_CNTL_ACC=0;
  }
  TGT_ID=2;
  BODY_VECT=1;
  P=0;
  Y=0;
  OM=-1;
  MNVR=false;
  ROT=false;
  TRK=false;
  JetsEnabled=3;
  Thrusters=true;
  NoseThrusters=true;
  TailThrusters=true;
  Torque.data[PITCH]=ORBITER_PITCH_TORQUE;
  Torque.data[YAW]=ORBITER_YAW_TORQUE;
  Torque.data[ROLL]=ORBITER_ROLL_TORQUE;
  ManeuverinProg=false;

  //I-loads
  stage1guidance_size=0;

  pCommModeHandler->DefineAnnotations();

  fTimeCameraLabel = 0.0;
  nhCameraLabel = oapiCreateAnnotation(true, 1.0, _V(1.0, 1.0, 1.0));
  oapiAnnotationSetPos(nhCameraLabel, 0.4, 0.05, 0.6, 0.15);
}

// --------------------------------------------------------------
// Destructor
// --------------------------------------------------------------
Atlantis::~Atlantis () {
	int i;

	delete psubsystems;
	delete pCommModeHandler;

	delete plop;
	delete gop;
	delete panela4;
	delete panela8;
	delete c3po;
	delete r2d2;
	delete panelf7;
	delete panelo3;
	delete panelc2;
	delete CDRKeyboard;
	delete PLTKeyboard;
  
  for (i = 0; i < 7; i++) delete rms_anim[i];
  
  delete CameraFLYaw;
  delete CameraFLPitch;
  delete CameraFRYaw;
  delete CameraFRPitch;
  delete CameraBLYaw;
  delete CameraBLPitch;
  delete CameraBRYaw;
  delete CameraBRPitch;

  delete [] stage1guidance[0];
  delete [] stage1guidance[1];
}

// --------------------------------------------------------------
// launch configuration setup (orbiter + tank + SRB)
// --------------------------------------------------------------
void Atlantis::SetLaunchConfiguration (void)
{
  int i;
  extern PARTICLESTREAMSPEC srb_contrail, srb_exhaust, srb_slag1, srb_slag2, srb_slag3;

  // *********************** physical parameters *********************************

  SetSize (30.0);
  SetEmptyMass (ORBITER_EMPTY_MASS + TANK_EMPTY_MASS + 2*SRB_EMPTY_MASS + pl_mass);
  SetCW (0.2, 0.5, 1.5, 1.5);
  SetWingAspect (0.7);
  SetWingEffectiveness (2.5);
  SetCrossSections (_V(687.4,849.5,189.4));
  SetRotDrag (_V(0.7,0.1,0.3));
  SetPMI (_V(179.1,176.8,29.3));
  SetTrimScale (0.05);
  SetLiftCoeffFunc (0); // simplification: we assume no lift during launch phase
  SetTouchdownPoints (_V(0,-10,-55.8), _V(-7,7,-55.8), _V(7,7,-55.8));

  // ************************* propellant specs **********************************

  
  if (!ph_tank) ph_tank = CreatePropellantResource (TANK_MAX_PROPELLANT_MASS);    // main tank
  if (!ph_srb)  ph_srb  = CreatePropellantResource (SRB_MAX_PROPELLANT_MASS*2.0); // SRB's
  
  CreateOrbiterTanks();
  
  SetDefaultPropellantResource (ph_tank); // display main tank level in generic HUD

  // *********************** thruster definitions ********************************

  // The main and SRB thrusters are defined so as to minimise the angular momentum
  // when engaged at maximum thrust. Dynamic gimbaling is not required until the SRBs
  // start to lose power since the centre of gravity is assumed static.
  // However the resulting linear force vector has a component in +y ("up") direction

  ClearThrusterDefinitions();

  // orbiter main thrusters
  /*th_main[0] = CreateThruster (OFS_LAUNCH_ORBITER+_V( 0.0, 3.2,-15.5), _V( 0.0,-0.13,1), ORBITER_MAIN_THRUST, ph_tank, ORBITER_MAIN_ISP0, ORBITER_MAIN_ISP1);
  th_main[1] = CreateThruster (OFS_LAUNCH_ORBITER+_V(-1.6,-0.2,-16.0), _V( 0.04994,0.0,0.99875), ORBITER_MAIN_THRUST, ph_tank, ORBITER_MAIN_ISP0, ORBITER_MAIN_ISP1);
  th_main[2] = CreateThruster (OFS_LAUNCH_ORBITER+_V( 1.6,-0.2,-16.0), _V(-0.04994,0.0,0.99875), ORBITER_MAIN_THRUST, ph_tank, ORBITER_MAIN_ISP0, ORBITER_MAIN_ISP1);*/
  //th_main[0] = CreateThruster (OFS_LAUNCH_ORBITER+_V( 0.0, 3.2,-15.5), _V(0.0, -0.37489, 0.92707), ORBITER_MAIN_THRUST, ph_tank, ORBITER_MAIN_ISP0, ORBITER_MAIN_ISP1);
  //th_main[1] = CreateThruster (OFS_LAUNCH_ORBITER+_V(-1.6,-0.2,-16.0), _V(0.065, -0.2447, 0.9674), ORBITER_MAIN_THRUST, ph_tank, ORBITER_MAIN_ISP0, ORBITER_MAIN_ISP1);
  //th_main[2] = CreateThruster (OFS_LAUNCH_ORBITER+_V( 1.6,-0.2,-16.0), _V(-0.065, -0.2447, 0.9674), ORBITER_MAIN_THRUST, ph_tank, ORBITER_MAIN_ISP0, ORBITER_MAIN_ISP1);
  
  th_main[0] = CreateThruster (OFS_LAUNCH_ORBITER + SSMET_REF, _V(0.0, -0.37489, 0.92707), ORBITER_MAIN_THRUST, ph_tank, ORBITER_MAIN_ISP0, ORBITER_MAIN_ISP1);
  th_main[1] = CreateThruster (OFS_LAUNCH_ORBITER + SSMEL_REF, _V(0.065, -0.2447, 0.9674), ORBITER_MAIN_THRUST, ph_tank, ORBITER_MAIN_ISP0, ORBITER_MAIN_ISP1);
  th_main[2] = CreateThruster (OFS_LAUNCH_ORBITER + SSMER_REF, _V(-0.065, -0.2447, 0.9674), ORBITER_MAIN_THRUST, ph_tank, ORBITER_MAIN_ISP0, ORBITER_MAIN_ISP1);
  
  
  thg_main = CreateThrusterGroup (th_main, 3, THGROUP_MAIN);
  DefineSSMEExhaust();
  //SURFHANDLE tex_main = oapiRegisterExhaustTexture ("Exhaust_atsme");
  for(i=0;i<3;i++) {
	  //AddExhaust(th_main[i], 30.0, 2.0, 5, tex_main);
	  GetThrusterRef(th_main[i], EngineNullPosition[i]);
	  EngineNullPosition[i]=Normalize(-EngineNullPosition[i]);
	  r2d2->CheckMPSArmed(i);
  }

  // SRBs
  /*th_srb[0] = CreateThruster (OFS_LAUNCH_LEFTSRB+_V(0.0,0.0,-21.8), _V(0,0.023643,0.999720), SRB_THRUST, ph_srb, SRB_ISP0, SRB_ISP1);
  th_srb[1] = CreateThruster (OFS_LAUNCH_RIGHTSRB +_V(0.0,0.0,-21.8), _V(0,0.023643,0.999720), SRB_THRUST, ph_srb, SRB_ISP0, SRB_ISP1);*/
  //th_srb[0] = CreateThruster (OFS_LAUNCH_LEFTSRB+_V(0.0,0.0,-21.8), _V(0.21958, 0.06765, 0.97325), SRB_THRUST, ph_srb, SRB_ISP0, SRB_ISP1);
  //th_srb[1] = CreateThruster (OFS_LAUNCH_RIGHTSRB +_V(0.0,0.0,-21.8), _V(-0.21958, 0.06765, 0.97325), SRB_THRUST, ph_srb, SRB_ISP0, SRB_ISP1);
  th_srb[0] = CreateThruster (OFS_LAUNCH_LEFTSRB+_V(0.0,0.0,-21.8), _V(0.0, 0.069338, 0.99759), SRB_THRUST, ph_srb, SRB_ISP0, SRB_ISP1);
  th_srb[1] = CreateThruster (OFS_LAUNCH_RIGHTSRB +_V(0.0,0.0,-21.8), _V(0.0, 0.069338, 0.99759), SRB_THRUST, ph_srb, SRB_ISP0, SRB_ISP1);
  thg_srb = CreateThrusterGroup (th_srb, 2, THGROUP_USER);
  SURFHANDLE tex = oapiRegisterExhaustTexture ("Exhaust2");
  srb_exhaust.tex = oapiRegisterParticleTexture ("Contrail2");
  srb_contrail.tex = oapiRegisterParticleTexture ("Contrail4");
  for (i = 0; i < 2; i++) AddExhaust (th_srb[i], 16.0, 2.0, tex);
  AddExhaustStream (th_srb[0], OFS_LAUNCH_RIGHTSRB+_V(0,1,/*-30*/-135), &srb_contrail); //DaveS edit: Modified Zofs from 50 to 135 to take in account longer exhaust
  AddExhaustStream (th_srb[1], OFS_LAUNCH_LEFTSRB+_V(0,1,/*-30*/-135), &srb_contrail); //DaveS edit: Modified Zofs from 50 to 135 to take in account longer exhaust
  AddExhaustStream (th_srb[0], OFS_LAUNCH_RIGHTSRB+_V(-0.5,1,-23), &srb_exhaust); //DaveS edit: Modified Zofs from -25 to -23 to better line up the exhaust PSTREAM with SRB nozzle, also corrected xofs and yofs to fix alignment issue
  AddExhaustStream (th_srb[1], OFS_LAUNCH_LEFTSRB+_V(0.5,1,-23), &srb_exhaust); //DaveS edit: Modified Zofs from -25 to -23 to better line up the exhaust PSTREAM with SRB nozzle, also corrected xofs and yofs to fix alignment issue

  //Add slag effect streams
  if(pshSlag1[0] == NULL) {
	pshSlag1[0] = AddParticleStream(&srb_slag1, OFS_LAUNCH_RIGHTSRB+_V(-0.5,1,-23), SLAG_DIR, &slag1);
  }
  if(pshSlag1[1] == NULL)
  {
	pshSlag1[1] = AddParticleStream(&srb_slag1, OFS_LAUNCH_LEFTSRB+_V(0.5,1,-23),  SLAG_DIR, &slag1);
  }

  //Add slag effect streams
  if(pshSlag2[0] == NULL) {
	pshSlag2[0] = AddParticleStream(&srb_slag2, OFS_LAUNCH_RIGHTSRB+_V(-0.5,1,-23), SLAG_DIR, &slag2);
  }
  if(pshSlag2[1] == NULL)
  {
	pshSlag2[1] = AddParticleStream(&srb_slag2, OFS_LAUNCH_LEFTSRB+_V(0.5,1,-23),  SLAG_DIR, &slag2);
  }

  
  //Add slag effect streams
  if(pshSlag3[0] == NULL) {
	pshSlag3[0] = AddParticleStream(&srb_slag3, OFS_LAUNCH_RIGHTSRB+_V(-0.5,1,-23), SLAG_DIR, &slag3);
  }
  if(pshSlag3[1] == NULL)
  {
	pshSlag3[1] = AddParticleStream(&srb_slag3, OFS_LAUNCH_LEFTSRB+_V(0.5,1,-23),  SLAG_DIR, &slag3);
  }

  //OMS
  // DaveS edit: Fixed OMS position to line up with OMS nozzles on the scaled down orbiter mesh
  VECTOR3 OMS_POS=OFS_LAUNCH_ORBITER+_V(0,3.55,-13.04);
  th_oms[0] = CreateThruster (OMS_POS-_V(2.313,0,0), -OMS_POS/length(OMS_POS), ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  th_oms[1] = CreateThruster (OMS_POS+_V(2.313,0,0), -OMS_POS/length(OMS_POS), ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  for(i=0;i<2;i++) {
	  AddExhaust (th_oms[i], 0.0, 0.5);
	  c3po->EngControl(i);
  }

  // attitude
  CreateAttControls_RCS(OFS_LAUNCH_ORBITER);

  // ************************* aerodynamics **************************************

  ClearVariableDragElements ();
  CreateVariableDragElement (&spdb_proc, 5, OFS_LAUNCH_ORBITER+_V(0, 7.5, -14)); // speedbrake drag
  CreateVariableDragElement (&(gop->gear_proc), 2, OFS_LAUNCH_ORBITER+_V(0,-3,0));      // landing gear drag
  CreateVariableDragElement (&rdoor_drag, 7, OFS_LAUNCH_ORBITER+_V(2.9,0,10));   // right cargo door drag
  CreateVariableDragElement (&ldoor_drag, 7, OFS_LAUNCH_ORBITER+_V(-2.9,0,10));  // right cargo door drag

  // ************************ visual parameters **********************************

  AddOrbiterVisual (OFS_LAUNCH_ORBITER);
  AddTankVisual    (OFS_LAUNCH_TANK);
  AddSRBVisual     (0, OFS_LAUNCH_RIGHTSRB);
  AddSRBVisual     (1, OFS_LAUNCH_LEFTSRB);

  status = 0;
}

// --------------------------------------------------------------
// Configuration after launch, before SRB separation
// --------------------------------------------------------------
void Atlantis::SetPostLaunchConfiguration (double met)
{
  SetLaunchConfiguration();
  t0 = -met; // reference time (liftoff)
  status = 1;
}

// --------------------------------------------------------------
// Configuration after booster separation (orbiter + tank)
// --------------------------------------------------------------
void Atlantis::SetOrbiterTankConfiguration (void)
{
  int i;
  VECTOR3 ofs;

  // *********************** physical parameters *********************************

  SetSize (28.8);
  SetEmptyMass (ORBITER_EMPTY_MASS + TANK_EMPTY_MASS + pl_mass);
  SetCW (0.2, 0.5, 1.5, 1.5);
  SetWingAspect (0.7);
  SetWingEffectiveness (2.5);
  SetCrossSections (_V(646.1,603.0,141.5));
  SetRotDrag (_V(0.7,0.1,0.3));
  SetPMI (_V(173.3,161.0,24.0));
  SetTrimScale (0.05);
  SetLiftCoeffFunc (0); // simplification: we assume no lift during launch phase
  SetTouchdownPoints (_V(0,-5,30), _V(-10,-10,-30), _V(10,0,-30));

  // ************************* propellant specs **********************************

  if (!ph_tank) ph_tank = CreatePropellantResource (TANK_MAX_PROPELLANT_MASS);    // main tank

  CreateOrbiterTanks();

  SetDefaultPropellantResource (ph_tank); // display main tank level in generic HUD

  // *********************** thruster definitions ********************************

  // Orbiter main engines
  // The thruster directions are adjusted so as to generate no angular moment
  // i.e. sum_{i=1}^3 thrusterpos_i x thrusterdir_i = 0
  // This assumes that all 3 main engines generate the same amount of thrust at
  // all times

  ofs = OFS_WITHTANK_ORBITER;
  if (thg_main) { // main engines already defined - just modify parameters
	  /*
	SetThrusterRef (th_main[1], ofs+_V(-1.6,-0.2,-16.0));
    SetThrusterDir (th_main[1], _V( 0.0624,-0.1789,0.9819));
    SetThrusterRef (th_main[2], ofs+_V( 1.6,-0.2,-16.0));
    SetThrusterDir (th_main[2], _V(-0.0624,-0.1789,0.9819));
    SetThrusterRef (th_main[0], ofs+_V( 0.0, 3.2,-15.5));
    SetThrusterDir (th_main[0], _V( 0.0,-0.308046,0.951372));
	*/

	DefineSSMEExhaust();
  } 
  else {        // create main engines
    th_main[0] = CreateThruster (ofs+_V( 0.0, 3.2,-15.5), _V( 0.0,-0.308046,0.951372), SSME_RATED_THRUST*(MaxThrust/100.0), ph_tank, ORBITER_MAIN_ISP0, ORBITER_MAIN_ISP1);
	th_main[1] = CreateThruster (ofs+_V(-1.6,-0.2,-16.0), _V( 0.0624,-0.1789,0.9819), SSME_RATED_THRUST*(MaxThrust/100.0), ph_tank, ORBITER_MAIN_ISP0, ORBITER_MAIN_ISP1);
	th_main[2] = CreateThruster (ofs+_V( 1.6,-0.2,-16.0), _V(-0.0624,-0.1789,0.9819), SSME_RATED_THRUST*(MaxThrust/100.0), ph_tank, ORBITER_MAIN_ISP0, ORBITER_MAIN_ISP1);
	thg_main = CreateThrusterGroup (th_main, 3, THGROUP_MAIN);
    //SURFHANDLE tex_main = oapiRegisterExhaustTexture ("Exhaust_atsme");
	//sprintf(oapiDebugString(), "Creating main engines");
    //for (i = 0; i < 3; i++) AddExhaust (th_main[i], 30.0, 2.0, tex_main);

	DefineSSMEExhaust();
  }
  for(i=0;i<3;i++) {
	  //AddExhaust(th_main[i], 30.0, 2.0, 1, tex_main);
	  GetThrusterRef(th_main[i], EngineNullPosition[i]);
	  EngineNullPosition[i]=Normalize(-EngineNullPosition[i]);
	  SetThrusterDir(th_main[i], EngineNullPosition[i]);
	  r2d2->CheckMPSArmed(i);
  }
  // DaveS edit: Fixed OMS position to line up with OMS nozzles on the scaled down orbiter mesh
  if(th_oms[0]==NULL) {
    VECTOR3 OMS_POS=ofs+_V(0,3.55,-13.04);
	th_oms[0] = CreateThruster (OMS_POS-_V(2.313,0,0), -OMS_POS/length(OMS_POS), ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
	th_oms[1] = CreateThruster (OMS_POS+_V(2.313,0,0), -OMS_POS/length(OMS_POS), ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
	for(i=0;i<2;i++) {
		AddExhaust (th_oms[i], 0.0, 0.5);
		c3po->EngControl(i);
	}
  }
  //if (!ThrusterGroupDefined (THGROUP_ATT_PITCHUP))
    CreateAttControls_RCS(ofs);

  // ************************* aerodynamics **************************************

  ClearVariableDragElements ();
  CreateVariableDragElement (&spdb_proc, 5, ofs+_V(0, 7.5, -14)); // speedbrake drag
  CreateVariableDragElement (&(gop->gear_proc), 2, ofs+_V(0,-3,0));      // landing gear drag
  CreateVariableDragElement (&rdoor_drag, 7, ofs+_V(2.9,0,10));   // right cargo door drag
  CreateVariableDragElement (&ldoor_drag, 7, ofs+_V(-2.9,0,10));  // right cargo door drag

  // ************************ visual parameters **********************************

  AddOrbiterVisual (OFS_WITHTANK_ORBITER);
  AddTankVisual    (OFS_WITHTANK_TANK);

  status = 2;
}

// --------------------------------------------------------------
// Configuration after tank separation (orbiter only)
// --------------------------------------------------------------
void Atlantis::SetOrbiterConfiguration (void)
{
  int i;

  // *********************** physical parameters *********************************

  SetSize (19.6);
  SetEmptyMass (ORBITER_EMPTY_MASS + pl_mass);
  VECTOR3 r[2] = {{0,0,10},{0,0,-8}};
  SetPMI (_V(104.2,108.8,13.497));
  SetGravityGradientDamping (20.0);
  SetTrimScale (0.05);
  SetCW (ORBITER_CW[0], ORBITER_CW[1], ORBITER_CW[2], ORBITER_CW[3]);
  SetCrossSections (ORBITER_CS);
  gop->SetGearParameters(gop->gear_proc);

  // ************************* aerodynamics **************************************

  SetRotDrag (_V(0.43,0.43,0.29)); // angular drag
  CreateAirfoil3 (LIFT_VERTICAL,   _V(0,0,-0.5), VLiftCoeff, NULL,Orbiterc, OrbiterS, OrbiterA);
  CreateAirfoil (LIFT_HORIZONTAL, _V(0,0,-4), HLiftCoeff, 20,  50, 1.5);

  helevator = CreateControlSurface2 (AIRCTRL_ELEVATOR, 5, 1.5, _V( 0, 0,  -15), AIRCTRL_AXIS_XPOS, anim_elev);
  hbodyflap = CreateControlSurface2 (AIRCTRL_ELEVATORTRIM, 5, 1.75, _V( 0, 0,  -17), AIRCTRL_AXIS_XPOS, anim_bf);
  hrudder = CreateControlSurface2 (AIRCTRL_RUDDER,   2, 1.5, _V( 0, 3,  -16), AIRCTRL_AXIS_YPOS, anim_rudder);
  hraileron = CreateControlSurface2 (AIRCTRL_AILERON,  3, 1.5, _V( 7,-0.5,-15), AIRCTRL_AXIS_XPOS, anim_raileron);
  hlaileron = CreateControlSurface2 (AIRCTRL_AILERON,  3, 1.5, _V(-7,-0.5,-15), AIRCTRL_AXIS_XNEG, anim_laileron);
  ControlSurfacesEnabled=true;

  ClearVariableDragElements ();
  CreateVariableDragElement (&spdb_proc, 12, _V(0, 7.5, -14)); // speedbrake drag
  CreateVariableDragElement (&(gop->gear_proc), 2, _V(0,-3,0));      // landing gear drag
  CreateVariableDragElement (&rdoor_drag, 7, _V(2.9,0,10));   // right cargo door drag
  CreateVariableDragElement (&ldoor_drag, 7, _V(-2.9,0,10));  // right cargo door drag

  SetADCtrlMode (7);

  // ************************* particle streams **********************************

  PARTICLESTREAMSPEC rps = {
    0, 20, 20, 0, 0.03, 0.5, 100, 3, PARTICLESTREAMSPEC::DIFFUSE,
    PARTICLESTREAMSPEC::LVL_FLAT, 1, 1, PARTICLESTREAMSPEC::ATM_PLIN, 6e7, 12e7
  };
  AddReentryStream (&rps);

  // ************************* propellant specs **********************************

  CreateOrbiterTanks();

  // *********************** thruster definitions ********************************

  // OMS (Orbital Manouevering System)
  // DaveS edit: Fixed OMS position to line up with OMS nozzles on the scaled down orbiter mesh
  VECTOR3 OMS_POS=_V(0,3.55,-13.04);
  th_oms[0] = CreateThruster (OMS_POS-_V(2.313,0,0), -OMS_POS/length(OMS_POS), ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  th_oms[1] = CreateThruster (OMS_POS+_V(2.313,0,0), -OMS_POS/length(OMS_POS), ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  thg_main = CreateThrusterGroup (th_oms, 2, THGROUP_MAIN);
  for(i=0;i<2;i++) {
	  AddExhaust (th_oms[i], 0.0, 0.5);
	  c3po->EngControl(i);
  }

  //Don't deactivate main engines... keep them existing for gimbal code
  //for(i=0;i<3;i++) th_main[i]=NULL; //deactivate mains

  /*VECTOR3 L_ENG_POS=_V(-1.6,-0.2,-16.0);
  VECTOR3 R_ENG_POS=_V( 1.6,-0.2,-16.0);
  VECTOR3 C_ENG_POS=_V( 0.0, 3.2,-15.5);
  /*th_main[0] = CreateThruster (_V(-1.6,-0.2,-16.0), _V( 0.099496, 0.012437, 0.99496), ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  th_main[1] = CreateThruster (_V( 1.6,-0.2,-16.0), _V(-0.099496, 0.012437, 0.99496), ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  th_main[2] = CreateThruster (_V( 0.0, 3.2,-15.5), _V( 0.0, -0.20219, 0.97935), ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);*/
  //th_main[0] = CreateThruster (C_ENG_POS, -C_ENG_POS/length(C_ENG_POS), ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  //th_main[1] = CreateThruster (L_ENG_POS, -L_ENG_POS/length(L_ENG_POS), ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  //th_main[2] = CreateThruster (R_ENG_POS, -R_ENG_POS/length(R_ENG_POS), ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  /*th_main[0] = CreateThruster (C_ENG_POS, -C_ENG_POS/length(C_ENG_POS), ORBITER_OMS_THRUST, NULL, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  th_main[1] = CreateThruster (L_ENG_POS, -L_ENG_POS/length(L_ENG_POS), ORBITER_OMS_THRUST, NULL, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  th_main[2] = CreateThruster (R_ENG_POS, -R_ENG_POS/length(R_ENG_POS), ORBITER_OMS_THRUST, NULL, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  //th_main[0] = CreateThruster (_V(-1.6,-0.2,-16.0), _V(0.0995, 0.0124, 0.9950), ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  //th_main[1] = CreateThruster (_V( 0.0, 3.2,-15.5), _V( 0.0, -0.0248, 0.9997), ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  //th_main[2] = CreateThruster (_V( 1.6,-0.2,-16.0), _V(-0.0995, 0.0124, 0.9950), ORBITER_OMS_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  thg_main = CreateThrusterGroup (th_main, 3, THGROUP_MAIN);
  SURFHANDLE tex_main = oapiRegisterExhaustTexture ("Exhaust_atsme");
  for(i=0;i<3;i++) {
	  AddExhaust(th_main[i], 30.0, 2.0, tex_main);
	  r2d2->CheckMPSArmed(i);
  }
  //SURFHANDLE tex_main = oapiRegisterExhaustTexture ("Exhaust_atsme");
  //for (i = 0; i < 3; i++) AddExhaust (th_main[i], 30.0, 2.0, tex_main);
  //AddExhaust (th_main[0], 30.0, 2.0, _V( 0.0, 3.2,-15.5), _V( 0.0, 0.308046,-0.951372), tex_main); 
  //AddExhaust (th_main[1], 30.0, 2.0, _V(-1.6,-0.2,-16.0), _V( -0.0624,0.1789,-0.9819), tex_main);
  //AddExhaust (th_main[2], 30.0, 2.0, _V( 1.6,-0.2,-16.0), _V(0.0624,0.1789,-0.9819), tex_main);
  
  //Retro (OMS)
  VECTOR3 L_OMS_POS=_V(-2.7,3.7,-13.8);
  VECTOR3 R_OMS_POS=_V(2.7,3.7,-13.8);
  //th_retro[0] = CreateThruster (OMS_POS-_V(2.7,0,0), -OMS_POS/length(OMS_POS), ORBITER_RETRO_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  //th_retro[1] = CreateThruster (OMS_POS+_V(2.7,0,0), -OMS_POS/length(OMS_POS), ORBITER_RETRO_THRUST, ph_oms, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  th_retro[0] = CreateThruster (L_OMS_POS, -L_OMS_POS/length(L_OMS_POS), ORBITER_RETRO_THRUST, NULL, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  th_retro[1] = CreateThruster (R_OMS_POS, -R_OMS_POS/length(R_OMS_POS), ORBITER_RETRO_THRUST, NULL, ORBITER_OMS_ISP0, ORBITER_OMS_ISP1);
  thg_retro = CreateThrusterGroup (th_retro, 2, THGROUP_RETRO);
  for(i=0;i<2;i++) {
	  AddExhaust (th_retro[i], 4.0, 0.5);
	  c3po->EngControl(i);
  }*/

  // RCS (Reaction Control System)
  CreateAttControls_RCS (_V(0,0,0));
  EnableAllRCS();

  // ************************ visual parameters **********************************

  AddOrbiterVisual (OFS_ZERO);
  

  status = 3;
}

// --------------------------------------------------------------
// Attitude controls (RCS) during orbital phase
// --------------------------------------------------------------
void Atlantis::CreateAttControls_RCS(VECTOR3 center) {
 // SURFHANDLE tex_rcs = oapiRegisterExhaustTexture ("Exhaust_atrcs");
  const double eh = 6.0;             // exhaust length scale
  const double ew1 = 0.4, ew2 = 0.8; // exhaust width scales

  THRUSTER_HANDLE thTmp[10];
  PitchActive=true;
  YawActive=true;
  RollActive=true;

  if(bUseRealRCS)
  {
	  CreateFRCS(center);
	  CreateLeftARCS(center);
	  CreateRightARCS(center);

	  
	  thTmp[0] = thManFRCS1[RCS_F1D];
	  thTmp[1] = thManFRCS1[RCS_F2D];
	  thTmp[2] = thManFRCS1[RCS_F3D];
	  thTmp[3] = thManFRCS1[RCS_F4D];
	  CreateThrusterGroup(thTmp, 4, THGROUP_ATT_PITCHUP);

	  return;
  }

  // set of attitude thrusters (idealised). The arrangement is such that no angular
  // momentum is created in linear mode, and no linear momentum is created in rotational mode.
  th_att_rcs[0] = CreateThruster (center+_V(0,0, 15.5), _V(0, 1,0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_rcs[1] = CreateThruster (center+_V(0,0,-15.5), _V(0,-1,0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_rcs[2] = CreateThruster (center+_V(0,0, 15.5), _V(0,-1,0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_rcs[3] = CreateThruster (center+_V(0,0,-15.5), _V(0, 1,0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  
  th_att_lin[0] = CreateThruster (center+_V(0,0, 15.5), _V(0, 1,0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_lin[3] = CreateThruster (center+_V(0,0,-15.5), _V(0,-1,0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_lin[2] = CreateThruster (center+_V(0,0, 15.5), _V(0,-1,0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_lin[1] = CreateThruster (center+_V(0,0,-15.5), _V(0, 1,0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  CreateThrusterGroup (th_att_lin,   2, THGROUP_ATT_UP);
  CreateThrusterGroup (th_att_lin+2, 2, THGROUP_ATT_DOWN);

  // DaveS edit: Fixed RCS exhaust defs to line up with nozzles on the scaled down orbiter mesh
  AddExhaust (th_att_rcs[0], eh, ew1, center+_V( 1.60,-0.20, 18.25), _V( 0.4339,-0.8830,-0.1793), tex_rcs);//F2D, fixed
  AddExhaust (th_att_rcs[0], eh, ew1, center+_V( 1.68,-0.18, 17.9), _V( 0.4339,-0.8830,-0.1793), tex_rcs);//F4D, fixed
  AddExhaust (th_att_rcs[0], eh, ew1, center+_V(-1.55,-0.20, 18.25), _V(-0.4339,-0.8830,-0.1793), tex_rcs);//F1D, fixed
  AddExhaust (th_att_rcs[0], eh, ew1, center+_V(-1.63,-0.18, 17.9), _V(-0.4339,-0.8830,-0.1793), tex_rcs);//F3D, fixed

  AddExhaust (th_att_rcs[1], eh, ew1, center+_V(-3.46, 3.20,-11.9), _V(0, 1,0), tex_rcs);//L4U, fixed
  AddExhaust (th_att_rcs[1], eh, ew1, center+_V(-3.46, 3.20,-12.3), _V(0, 1,0), tex_rcs);//L2U, fixed
  AddExhaust (th_att_rcs[1], eh, ew1, center+_V(-3.46, 3.20,-12.7), _V(0, 1,0), tex_rcs);//L1U, fixed

  AddExhaust (th_att_rcs[1], eh, ew1, center+_V( 3.43, 3.20,-11.9), _V(0, 1,0), tex_rcs);//R4U, fixed
  AddExhaust (th_att_rcs[1], eh, ew1, center+_V( 3.43, 3.20,-12.3), _V(0, 1,0), tex_rcs);//R2U, fixed
  AddExhaust (th_att_rcs[1], eh, ew1, center+_V( 3.43, 3.20,-12.7), _V(0, 1,0), tex_rcs);//R1U, fixed

  AddExhaust (th_att_rcs[2], eh, ew1, center+_V(-0.4 , 1.10, 17.9 ), _V(0, 1,0), tex_rcs);//F1U, fixed
  AddExhaust (th_att_rcs[2], eh, ew1, center+_V( 0.0 , 1.15 ,17.9 ), _V(0, 1,0), tex_rcs);//F3U, fixed
  AddExhaust (th_att_rcs[2], eh, ew1, center+_V( 0.4 , 1.10, 17.9 ), _V(0, 1,0), tex_rcs);//F2U, fixed

  AddExhaust (th_att_rcs[3], eh, ew1, center+_V(-3.1 , 1.55,-12.08), _V(-0.2844,-0.9481,-0.1422), tex_rcs);//L4D, fixed
  AddExhaust (th_att_rcs[3], eh, ew1, center+_V(-3.1 , 1.6 ,-12.43 ), _V(-0.2844,-0.9481,-0.1422), tex_rcs);//L2D, fixed
  AddExhaust (th_att_rcs[3], eh, ew1, center+_V(-3.1 , 1.65,-12.78), _V(-0.2844,-0.9481,-0.1422), tex_rcs);//L3D, fixed

  AddExhaust (th_att_rcs[3], eh, ew1, center+_V( 3.15, 1.55,-12.08), _V( 0.2844,-0.9481,-0.1422), tex_rcs);//R4D, fixed
  AddExhaust (th_att_rcs[3], eh, ew1, center+_V( 3.15, 1.6 ,-12.43 ), _V( 0.2844,-0.9481,-0.1422), tex_rcs);//R2D, fixed
  AddExhaust (th_att_rcs[3], eh, ew1, center+_V( 3.15, 1.65,-12.78), _V( 0.2844,-0.9481,-0.1422), tex_rcs);//R3D, fixed

  AddExhaust (th_att_lin[0], eh, ew1, center+_V( 1.60,-0.20, 18.25), _V( 0.4339,-0.8830,-0.1793), tex_rcs);//F2D, fixed
  AddExhaust (th_att_lin[0], eh, ew1, center+_V( 1.68,-0.18, 17.9), _V( 0.4339,-0.8830,-0.1793), tex_rcs);//F4D, fixed
  AddExhaust (th_att_lin[0], eh, ew1, center+_V(-1.55,-0.20, 18.25), _V(-0.4339,-0.8830,-0.1793), tex_rcs);//F1D, fixed
  AddExhaust (th_att_lin[0], eh, ew1, center+_V(-1.63,-0.18, 17.9), _V(-0.4339,-0.8830,-0.1793), tex_rcs);//F3D, fixed

  AddExhaust (th_att_lin[3], eh, ew1, center+_V(-3.46, 3.20,-11.9), _V(0, 1,0), tex_rcs);//L4U, fixed
  AddExhaust (th_att_lin[3], eh, ew1, center+_V(-3.46, 3.20,-12.3), _V(0, 1,0), tex_rcs);//L2U, fixed
  AddExhaust (th_att_lin[3], eh, ew1, center+_V(-3.46, 3.20,-12.7), _V(0, 1,0), tex_rcs);//L1U, fixed

  AddExhaust (th_att_lin[3], eh, ew1, center+_V( 3.43, 3.20,-11.9), _V(0, 1,0), tex_rcs);//R4U, fixed
  AddExhaust (th_att_lin[3], eh, ew1, center+_V( 3.43, 3.20,-12.3), _V(0, 1,0), tex_rcs);//R2U, fixed
  AddExhaust (th_att_lin[3], eh, ew1, center+_V( 3.43, 3.20,-12.7), _V(0, 1,0), tex_rcs);//R1U, fixed

  AddExhaust (th_att_lin[2], eh, ew1, center+_V(-0.4 , 1.10, 17.9 ), _V(0, 1,0), tex_rcs);//F1U, fixed
  AddExhaust (th_att_lin[2], eh, ew1, center+_V( 0.0 , 1.15 ,17.9 ), _V(0, 1,0), tex_rcs);//F3U, fixed
  AddExhaust (th_att_lin[2], eh, ew1, center+_V( 0.4 , 1.10, 17.9 ), _V(0, 1,0), tex_rcs);//F2U, fixed

  AddExhaust (th_att_lin[1], eh, ew1, center+_V(-3.1 , 1.55,-12.08), _V(-0.2844,-0.9481,-0.1422), tex_rcs);//L4D, fixed
  AddExhaust (th_att_lin[1], eh, ew1, center+_V(-3.1 , 1.6 ,-12.43 ), _V(-0.2844,-0.9481,-0.1422), tex_rcs);//L2D, fixed
  AddExhaust (th_att_lin[1], eh, ew1, center+_V(-3.1 , 1.65,-12.78), _V(-0.2844,-0.9481,-0.1422), tex_rcs);//L3D, fixed

  AddExhaust (th_att_lin[1], eh, ew1, center+_V( 3.15, 1.55,-12.08), _V( 0.2844,-0.9481,-0.1422), tex_rcs);//R4D, fixed
  AddExhaust (th_att_lin[1], eh, ew1, center+_V( 3.15, 1.6 ,-12.43 ), _V( 0.2844,-0.9481,-0.1422), tex_rcs);//R2D, fixed
  AddExhaust (th_att_lin[1], eh, ew1, center+_V( 3.15, 1.65,-12.78), _V( 0.2844,-0.9481,-0.1422), tex_rcs);//R3D, fixed

  th_att_rcs[4] = CreateThruster (center+_V(0,0, 15.5), _V(-1,0,0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_rcs[5] = CreateThruster (center+_V(0,0,-15.5), _V( 1,0,0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_rcs[6] = CreateThruster (center+_V(0,0, 15.5), _V( 1,0,0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_rcs[7] = CreateThruster (center+_V(0,0,-15.5), _V(-1,0,0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);

  th_att_lin[4] = CreateThruster (center+_V(0,0, 15.5), _V(-1,0,0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_lin[7] = CreateThruster (center+_V(0,0,-15.5), _V( 1,0,0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_lin[6] = CreateThruster (center+_V(0,0, 15.5), _V( 1,0,0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_lin[5] = CreateThruster (center+_V(0,0,-15.5), _V(-1,0,0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  CreateThrusterGroup (th_att_lin+4,   2, THGROUP_ATT_LEFT);
  CreateThrusterGroup (th_att_lin+6, 2, THGROUP_ATT_RIGHT);

  AddExhaust (th_att_rcs[4], eh, ew2, center+_V( 1.8 ,-0.15 , 17.40 ), _V( 1,0,0), tex_rcs);//F4R, fixed
  AddExhaust (th_att_rcs[4], eh, ew2, center+_V( 1.75, 0.25 , 17.45), _V( 1,0,0), tex_rcs);//F2R, fixed
  AddExhaust (th_att_rcs[6], eh, ew2, center+_V(-1.7 ,-0.15 , 17.40 ), _V(-1,0,0), tex_rcs);//F1L, fixed
  AddExhaust (th_att_rcs[6], eh, ew2, center+_V(-1.65, 0.25 , 17.45), _V(-1,0,0), tex_rcs);//F3L, fixed

  AddExhaust (th_att_rcs[5], eh, ew2, center+_V(-3.8 , 2.35,-11.9), _V(-1,0,0), tex_rcs);//L4L, fixed
  AddExhaust (th_att_rcs[5], eh, ew2, center+_V(-3.8 , 2.35,-12.15 ), _V(-1,0,0), tex_rcs);//L2L, fixed
  AddExhaust (th_att_rcs[5], eh, ew2, center+_V(-3.8 , 2.35,-12.55 ), _V(-1,0,0), tex_rcs);//L3L, fixed
  AddExhaust (th_att_rcs[5], eh, ew2, center+_V(-3.8 , 2.35,-12.9), _V(-1,0,0), tex_rcs);//L1L, fixed

  AddExhaust (th_att_rcs[7], eh, ew2, center+_V( 3.8 , 2.35,-11.9), _V( 1,0,0), tex_rcs);//R4R, fixed
  AddExhaust (th_att_rcs[7], eh, ew2, center+_V( 3.8 , 2.35,-12.15 ), _V( 1,0,0), tex_rcs);//R2R, fixed
  AddExhaust (th_att_rcs[7], eh, ew2, center+_V( 3.8 , 2.35,-12.55 ), _V( 1,0,0), tex_rcs);//R3R, fixed
  AddExhaust (th_att_rcs[7], eh, ew2, center+_V( 3.8,  2.35,-12.9), _V( 1,0,0), tex_rcs);//R1R, fixed

  AddExhaust (th_att_lin[4], eh, ew2, center+_V( 1.8 ,-0.15 , 17.40 ), _V( 1,0,0), tex_rcs);//F4R, fixed
  AddExhaust (th_att_lin[4], eh, ew2, center+_V( 1.75, 0.25 , 17.45), _V( 1,0,0), tex_rcs);//F2R, fixed
  AddExhaust (th_att_lin[6], eh, ew2, center+_V(-1.7 ,-0.15 , 17.40 ), _V(-1,0,0), tex_rcs);//F1L, fixed
  AddExhaust (th_att_lin[6], eh, ew2, center+_V(-1.65, 0.25 , 17.45), _V(-1,0,0), tex_rcs);//F3L, fixed

  AddExhaust (th_att_lin[7], eh, ew2, center+_V(-3.8 , 2.35,-11.9), _V(-1,0,0), tex_rcs);//L4L, fixed
  AddExhaust (th_att_lin[7], eh, ew2, center+_V(-3.8 , 2.35,-12.15 ), _V(-1,0,0), tex_rcs);//L2L, fixed
  AddExhaust (th_att_lin[7], eh, ew2, center+_V(-3.8 , 2.35,-12.55 ), _V(-1,0,0), tex_rcs);//L3L, fixed
  AddExhaust (th_att_lin[7], eh, ew2, center+_V(-3.8 , 2.35,-12.9), _V(-1,0,0), tex_rcs);//L1L, fixed

  AddExhaust (th_att_lin[5], eh, ew2, center+_V( 3.8 , 2.35,-11.9), _V( 1,0,0), tex_rcs);//R4R, fixed
  AddExhaust (th_att_lin[5], eh, ew2, center+_V( 3.8 , 2.35,-12.15 ), _V( 1,0,0), tex_rcs);//R2R, fixed
  AddExhaust (th_att_lin[5], eh, ew2, center+_V( 3.8 , 2.35,-12.55 ), _V( 1,0,0), tex_rcs);//R3R, fixed
  AddExhaust (th_att_lin[5], eh, ew2, center+_V( 3.8,  2.35,-12.9), _V( 1,0,0), tex_rcs);//R1R, fixed

  th_att_rcs[8] = CreateThruster (center+_V( 2.7,0,0), _V(0, 1,0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_rcs[9] = CreateThruster (center+_V(-2.7,0,0), _V(0,-1,0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_rcs[10] = CreateThruster (center+_V(-2.7,0,0), _V(0, 1,0), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_rcs[11] = CreateThruster (center+_V( 2.7,0,0), _V(0,-1,0), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  //The virtual thrusters
  th_att_rcs[12] = CreateThruster (_V(0,0,0), _V(0,-1,0), 0, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_rcs[13] = CreateThruster (_V(0,0,0), _V(0,-1,0), 0, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_rcs[14] = CreateThruster (_V(0,0,0), _V(0,-1,0), 0, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_rcs[15] = CreateThruster (_V(0,0,0), _V(0,-1,0), 0, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_rcs[16] = CreateThruster (_V(0,0,0), _V(0,-1,0), 0, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_rcs[17] = CreateThruster (_V(0,0,0), _V(0,-1,0), 0, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);

  AddExhaust (th_att_rcs[8], eh, ew1, center+_V( 1.60,-0.20, 18.78), _V( 0.4339,-0.8830,-0.1793), tex_rcs);//F2D
  AddExhaust (th_att_rcs[8], eh, ew1, center+_V( 1.68,-0.18, 18.40), _V( 0.4339,-0.8830,-0.1793), tex_rcs);//F4D
  AddExhaust (th_att_rcs[10], eh, ew1, center+_V(-1.55,-0.20, 18.78), _V(-0.4339,-0.8830,-0.1793), tex_rcs);//F1D
  AddExhaust (th_att_rcs[10], eh, ew1, center+_V(-1.63,-0.18, 18.40), _V(-0.4339,-0.8830,-0.1793), tex_rcs);//F3D

  AddExhaust (th_att_rcs[9], eh, ew1, center+_V(-3.46, 3.20,-12.30), _V(0, 1,0), tex_rcs);//L4U
  AddExhaust (th_att_rcs[9], eh, ew1, center+_V(-3.46, 3.20,-12.70), _V(0, 1,0), tex_rcs);//L2U
  AddExhaust (th_att_rcs[9], eh, ew1, center+_V(-3.46, 3.20,-13.10), _V(0, 1,0), tex_rcs);//L1U

  AddExhaust (th_att_rcs[11], eh, ew1, center+_V( 3.43, 3.20,-12.30), _V(0, 1,0), tex_rcs);//R4U
  AddExhaust (th_att_rcs[11], eh, ew1, center+_V( 3.43, 3.20,-12.70), _V(0, 1,0), tex_rcs);//R2U
  AddExhaust (th_att_rcs[11], eh, ew1, center+_V( 3.43, 3.20,-13.10), _V(0, 1,0), tex_rcs);//R1U

  AddExhaust (th_att_rcs[10], eh, ew1, center+_V(-3.1 , 1.55,-12.45), _V(-0.2844,-0.9481,-0.1422), tex_rcs);//L4D
  AddExhaust (th_att_rcs[10], eh, ew1, center+_V(-3.1 , 1.6 ,-12.8 ), _V(-0.2844,-0.9481,-0.1422), tex_rcs);//L2D
  AddExhaust (th_att_rcs[10], eh, ew1, center+_V(-3.1 , 1.65,-13.15), _V(-0.2844,-0.9481,-0.1422), tex_rcs);//L3D

  AddExhaust (th_att_rcs[8], eh, ew1, center+_V( 3.15, 1.55,-12.45), _V( 0.2844,-0.9481,-0.1422), tex_rcs);//R4D
  AddExhaust (th_att_rcs[8], eh, ew1, center+_V( 3.15, 1.6 ,-12.8 ), _V( 0.2844,-0.9481,-0.1422), tex_rcs);//R2D
  AddExhaust (th_att_rcs[8], eh, ew1, center+_V( 3.15, 1.65,-13.15), _V( 0.2844,-0.9481,-0.1422), tex_rcs);//R3D

  th_att_lin[8] = CreateThruster (center+_V(0,0,-16), _V(0,0, 1), ORBITER_RCS_THRUST, ph_oms, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  th_att_lin[9] = CreateThruster (center+_V(0,0, 16), _V(0,0,-1), ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, ORBITER_RCS_ISP1);
  CreateThrusterGroup (th_att_lin+8, 1, THGROUP_ATT_FORWARD);
  CreateThrusterGroup (th_att_lin+9, 1, THGROUP_ATT_BACK);

  AddExhaust (th_att_lin[8], eh, ew1, center+_V(-3.59, 2.8 ,-13.4 ), _V(0,0,-1), tex_rcs);//L1A, fixed
  AddExhaust (th_att_lin[8], eh, ew1, center+_V(-3.27, 2.8 ,-13.4 ), _V(0,0,-1), tex_rcs);//L3A, fixed
  AddExhaust (th_att_lin[8], eh, ew1, center+_V( 3.64, 2.8 ,-13.4 ), _V(0,0,-1), tex_rcs);//R1A, fixed
  AddExhaust (th_att_lin[8], eh, ew1, center+_V( 3.27, 2.8 ,-13.4 ), _V(0,0,-1), tex_rcs);//R3A, fixed

  AddExhaust (th_att_lin[9], eh, ew1, center+_V( 0.0 , 0.75, 19.0 ), _V(0, 0.0499, 0.9988), tex_rcs);//F3F, fixed
  AddExhaust (th_att_lin[9], eh, ew1, center+_V(-0.4 , 0.7 , 19.0 ), _V(0, 0.0499, 0.9988), tex_rcs);//F1F, fixed
  AddExhaust (th_att_lin[9], eh, ew1, center+_V( 0.4 , 0.7 , 19.0 ), _V(0, 0.0499, 0.9988), tex_rcs);//F2F, fixed
  RCSEnabled=true;
  CreateThrusterGroup (th_att_rcs,   2, THGROUP_ATT_PITCHUP);
  CreateThrusterGroup (th_att_rcs+2, 2, THGROUP_ATT_PITCHDOWN);
  CreateThrusterGroup (th_att_rcs+4,   2, THGROUP_ATT_YAWLEFT);
  CreateThrusterGroup (th_att_rcs+6, 2, THGROUP_ATT_YAWRIGHT);
  CreateThrusterGroup (th_att_rcs+8,   2, THGROUP_ATT_BANKLEFT);
  CreateThrusterGroup (th_att_rcs+10, 2, THGROUP_ATT_BANKRIGHT);
  
}

void Atlantis::DisableAllRCS() {
  if(bUseRealRCS)
		return;

  if(!RCSEnabled) return;
  DelThrusterGroup(THGROUP_ATT_PITCHDOWN);
  DelThrusterGroup(THGROUP_ATT_PITCHUP);
  DelThrusterGroup(THGROUP_ATT_YAWLEFT);
  DelThrusterGroup(THGROUP_ATT_YAWRIGHT);
  DelThrusterGroup(THGROUP_ATT_BANKLEFT);
  DelThrusterGroup(THGROUP_ATT_BANKRIGHT);
  CreateThrusterGroup (th_att_rcs+12,   1, THGROUP_ATT_PITCHUP);
  CreateThrusterGroup (th_att_rcs+13,   1, THGROUP_ATT_PITCHDOWN);
  CreateThrusterGroup (th_att_rcs+14,   1, THGROUP_ATT_YAWLEFT);
  CreateThrusterGroup (th_att_rcs+15,   1, THGROUP_ATT_YAWRIGHT);
  CreateThrusterGroup (th_att_rcs+16,   1, THGROUP_ATT_BANKLEFT);
  CreateThrusterGroup (th_att_rcs+17,   1, THGROUP_ATT_BANKRIGHT);
  RCSEnabled=false;
}

void Atlantis::EnableAllRCS() {
  if(bUseRealRCS)
		return;

  if(RCSEnabled) return;
  DelThrusterGroup(THGROUP_ATT_PITCHDOWN);
  DelThrusterGroup(THGROUP_ATT_PITCHUP);
  DelThrusterGroup(THGROUP_ATT_YAWLEFT);
  DelThrusterGroup(THGROUP_ATT_YAWRIGHT);
  DelThrusterGroup(THGROUP_ATT_BANKLEFT);
  DelThrusterGroup(THGROUP_ATT_BANKRIGHT);
  CreateThrusterGroup (th_att_rcs,   2, THGROUP_ATT_PITCHUP);
  CreateThrusterGroup (th_att_rcs+2, 2, THGROUP_ATT_PITCHDOWN);
  CreateThrusterGroup (th_att_rcs+4,   2, THGROUP_ATT_YAWLEFT);
  CreateThrusterGroup (th_att_rcs+6, 2, THGROUP_ATT_YAWRIGHT);
  CreateThrusterGroup (th_att_rcs+8,   2, THGROUP_ATT_BANKLEFT);
  CreateThrusterGroup (th_att_rcs+10, 2, THGROUP_ATT_BANKRIGHT);
  RCSEnabled=true;
}

void Atlantis::PaintMarkings (SURFHANDLE tex) {
	HDC hDC = oapiGetDC (tex);
	HFONT hFont = CreateFont(47, 0, 1800, 1800, 700, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
	HFONT pFont = (HFONT)SelectObject (hDC, hFont);
	SetTextColor (hDC, 0x202020);
	SetBkMode (hDC, TRANSPARENT);
	char cbuf[256];
//	strncpy (cbuf, "Kwan's Excellent Space Shuttle Adventure", 256);
	//strncpy (cbuf, this->GetName(), 256);
	//
	strncpy (cbuf, WingName, 256);
	int len = strlen(cbuf);
	TextOut (hDC, 566, 296, cbuf, len); //DaveS edit: Fixed incorret location of wing name marking
	SelectObject (hDC, pFont);
	DeleteObject (hFont);
	hFont = CreateFont(26, 0, 900, 900, 700, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
	pFont = (HFONT)SelectObject (hDC, hFont);
	SetTextAlign (hDC, TA_CENTER);
	TextOut (hDC, 1800, 493, cbuf, len);
	SelectObject (hDC, pFont);
	DeleteObject (hFont);
	hFont = CreateFont(26, 0, 2700, 2700, 700, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
	pFont = (HFONT)SelectObject (hDC, hFont);
	TextOut (hDC, 1400, 493, cbuf, len);
	SelectObject (hDC, pFont);
	DeleteObject (hFont);
	oapiReleaseDC (tex, hDC);
}



void Atlantis::DisableControlSurfaces()
{
	if(!ControlSurfacesEnabled) return;
	ClearControlSurfaceDefinitions();
	hrudder		= NULL;
	hbodyflap	= NULL;
	helevator	= NULL;
	hlaileron	= NULL;
	hraileron	= NULL;
	ControlSurfacesEnabled=false;
}

void Atlantis::EnableControlSurfaces()
{
	if(ControlSurfacesEnabled) return;
	helevator = CreateControlSurface2 (AIRCTRL_ELEVATOR, 5, 1.5, _V( 0, 0,  -15), AIRCTRL_AXIS_XPOS, anim_elev);
    hbodyflap = CreateControlSurface2 (AIRCTRL_ELEVATORTRIM, 5, 1.75, _V( 0, 0,  -17), AIRCTRL_AXIS_XPOS, anim_bf);
	hrudder = CreateControlSurface2 (AIRCTRL_RUDDER,   2, 1.5, _V( 0, 3,  -16), AIRCTRL_AXIS_YPOS, anim_rudder);
	hraileron = CreateControlSurface2 (AIRCTRL_AILERON,  3, 1.5, _V( 7,-0.5,-15), AIRCTRL_AXIS_XPOS, anim_raileron);
	hlaileron = CreateControlSurface2 (AIRCTRL_AILERON,  3, 1.5, _V(-7,-0.5,-15), AIRCTRL_AXIS_XNEG, anim_laileron);
	ControlSurfacesEnabled=true;
}

// --------------------------------------------------------------
// Define animation sequences for moving parts
// --------------------------------------------------------------
void Atlantis::DefineAnimations (void)
{
  UINT midx = 1; // mesh index for all external animations
  UINT vidx = 2; // mesh index for all VC animations
  UINT ridx = 3; // mesh index for all RMS animations

  ANIMATIONCOMPONENT_HANDLE parent;

  // ***** 1. Cargo door and radiator animations *****
  // DaveS edit: Updated animations to work with the new scaled down orbiter mesh

  static UINT RCargoDoorGrp[3] = {GRP_CARGODOOROUTR, GRP_CARGODOORINR, GRP_RADIATORBR};
  static MGROUP_ROTATE RCargoDoor (midx, RCargoDoorGrp, 3,
    _V(2.82, 1.39, 0), _V(0, 0, 1), (float)(-170.5*RAD));
  static UINT LCargoDoorGrp[3] = {GRP_CARGODOOROUTL,GRP_CARGODOORINL, GRP_RADIATORBL};
  static MGROUP_ROTATE LCargoDoor (midx, LCargoDoorGrp, 3,
    _V(-2.80, 1.39, 0), _V(0, 0, 1), (float)(170.5*RAD));

  static UINT RRadiatorGrp[1] = {GRP_RADIATORFR};
  static MGROUP_ROTATE RRadiator (midx, RRadiatorGrp, 1,
    _V(2.71, 1.7, 0), _V(0, 0, 1), (float)(35.5*RAD));
  static UINT LRadiatorGrp[1] = {GRP_RADIATORFL};
  static MGROUP_ROTATE LRadiator (midx, LRadiatorGrp, 1,
    _V(-2.71, 1.7, 0), _V(0, 0, 1), (float)(-35.5*RAD));

  static UINT CLatch1_4Grp[] = {64};
  static MGROUP_ROTATE CLatch1_4 (midx, CLatch1_4Grp, 1, _V(0.05,3.47,0.0), _V(0,0,1), 90 * RAD);

  static UINT CLatch5_8Grp[] = {63};
  static MGROUP_ROTATE CLatch5_8 (midx, CLatch5_8Grp, 1, _V(0.05,3.47,0.0), _V(0,0,1), 90 * RAD);

  static UINT CLatch9_12Grp[] = {62};
  static MGROUP_ROTATE CLatch9_12 (midx, CLatch9_12Grp, 1, _V(0.05,3.47,0.0), _V(0,0,1), 90 * RAD);

  static UINT CLatch13_16Grp[] = {61};
  static MGROUP_ROTATE CLatch13_16 (midx, CLatch13_16Grp, 1, _V(0.05,3.47,0.0), _V(0,0,1), 90 * RAD);

  anim_door = CreateAnimation (0);
  anim_rad = CreateAnimation (0);
  anim_clatch1_4 = CreateAnimation(0);
  anim_clatch5_8 = CreateAnimation(0);
  anim_clatch9_12 = CreateAnimation(0);
  anim_clatch13_16 = CreateAnimation(0);
  //right side
  parent = AddAnimationComponent (anim_door, 0.0, 0.4632, &RCargoDoor);
  AddAnimationComponent (anim_rad, 0, 1, &RRadiator, parent);
  AddAnimationComponent (anim_clatch1_4, 0, 1, &CLatch1_4, parent);
  AddAnimationComponent (anim_clatch5_8, 0, 1, &CLatch5_8, parent);
  AddAnimationComponent (anim_clatch9_12, 0, 1, &CLatch9_12, parent);
  AddAnimationComponent (anim_clatch13_16, 0, 1, &CLatch13_16, parent);
  //left side
  parent = AddAnimationComponent (anim_door, 0.5368, 1.0, &LCargoDoor);
  AddAnimationComponent (anim_rad, 0, 1, &LRadiator, parent);

  // ***** 2. Landing gear animation *****
  gop->DefineAnimations();
  //In GearOp.cpp
  if(bHasKUBand)
		DefineKUBandAnimations();
  
  // ***** 4. Elevator animation of elevons *****

  static UINT ElevGrp_up[4] = {GRP_FLAPR,GRP_FLAPL,GRP_AILERONL,GRP_AILERONR};
  static MGROUP_ROTATE Elevator_up (midx, ElevGrp_up, 4,
    _V(-3.058,-2.137,-8.776), _V(1,0,0), (float)(34.0*RAD));
  static UINT ElevGrp_down[4] = {GRP_FLAPR,GRP_FLAPL,GRP_AILERONL,GRP_AILERONR};
  static MGROUP_ROTATE Elevator_down (midx, ElevGrp_down, 4,
    _V(-3.058,-2.137,-8.776), _V(1,0,0), (float)(18.0*RAD));
  static UINT ElevDoorGrp_up[4] = {GRP_LEFTFLAPPERIN,GRP_LEFTFLAPPEROUT,GRP_RIGHTFLAPPERIN,GRP_RIGHTFLAPPEROUT};
  static MGROUP_ROTATE ElevatorDoor_up (midx, ElevDoorGrp_up, 4,
    _V(0,-1.798,-8.246), _V(1,0,0), (float)(15.0*RAD));
  static UINT ElevDoorGrp_down[4] = {GRP_LEFTFLAPPERIN,GRP_LEFTFLAPPEROUT,GRP_RIGHTFLAPPERIN,GRP_RIGHTFLAPPEROUT};
  static MGROUP_ROTATE ElevatorDoor_down (midx, ElevDoorGrp_down, 4,
    _V(0,-1.798,-8.246), _V(1,0,0), (float)(2.5*RAD));
  anim_elev = CreateAnimation (0.5);
  AddAnimationComponent (anim_elev, 0.5, 1, &Elevator_up);
  AddAnimationComponent (anim_elev, 0, 0.5, &Elevator_down);
  AddAnimationComponent (anim_elev, 0.5, 1, &ElevatorDoor_up);
  AddAnimationComponent (anim_elev, 0, 0.5, &ElevatorDoor_down);

  static UINT bfGrp[1] = {GRP_BODYFLAP};
  static MGROUP_ROTATE BodyFlap (midx, bfGrp, 1,
    _V(0,-2.034,-12.256), _V(1,0,0), (float)(22.5*RAD));
  anim_bf = CreateAnimation (0.5);
  AddAnimationComponent (anim_bf, 0, 1, &BodyFlap);

  // ***** 5. Aileron animation of elevons *****

  // ***** Elevon upward animation *****
  static UINT LAileronGrp_up[2] = {GRP_FLAPL,GRP_AILERONL};
  static MGROUP_ROTATE LAileron_up (midx, LAileronGrp_up, 2,
    _V(0,-2.137,-8.776), _V(-1,0,0), (float)(34.0*RAD));
  static UINT LAileronDoorGrp_up[2] = {GRP_LEFTFLAPPERIN,GRP_LEFTFLAPPEROUT};
  static MGROUP_ROTATE LAileronDoor_up (midx, LAileronDoorGrp_up, 2,
    _V(0,-1.798,-8.246), _V(-1,0,0), (float)(15.0*RAD));

  static UINT RAileronGrp_up[2] = {GRP_FLAPR,GRP_AILERONR};
  static MGROUP_ROTATE RAileron_up (midx, RAileronGrp_up, 2,
    _V(0,-2.137,-8.776), _V(1,0,0), (float)(34.0*RAD));
  static UINT RAileronDoorGrp_up[2] = {GRP_RIGHTFLAPPERIN,GRP_RIGHTFLAPPEROUT};
  static MGROUP_ROTATE RAileronDoor_up (midx, RAileronDoorGrp_up, 2,
    _V(0,-1.798,-8.246), _V(1,0,0), (float)(15.0*RAD));
  
  // ***** Elevon downward animation *****
  static UINT LAileronGrp_down[2] = {GRP_FLAPL,GRP_AILERONL};
  static MGROUP_ROTATE LAileron_down (midx, LAileronGrp_down, 2,
    _V(0,-2.137,-8.776), _V(-1,0,0), (float)(18.0*RAD));
  static UINT LAileronDoorGrp_down[2] = {GRP_LEFTFLAPPERIN,GRP_LEFTFLAPPEROUT};
  static MGROUP_ROTATE LAileronDoor_down (midx, LAileronDoorGrp_down, 2,
    _V(0,-1.798,-8.246), _V(-1,0,0), (float)(2.5*RAD));

  static UINT RAileronGrp_down[2] = {GRP_FLAPR,GRP_AILERONR};
  static MGROUP_ROTATE RAileron_down (midx, RAileronGrp_down, 2,
    _V(0,-2.137,-8.776), _V(1,0,0), (float)(18.0*RAD));
  static UINT RAileronDoorGrp_down[2] = {GRP_RIGHTFLAPPERIN,GRP_RIGHTFLAPPEROUT};
  static MGROUP_ROTATE RAileronDoor_down (midx, RAileronDoorGrp_down, 2,
    _V(0,-1.798,-8.246), _V(1,0,0), (float)(2.5*RAD));

  anim_laileron = CreateAnimation (0.5);
  AddAnimationComponent (anim_laileron, 0, 0.5, &LAileron_up);
  AddAnimationComponent (anim_laileron, 0, 0.5, &LAileronDoor_up);
  AddAnimationComponent (anim_laileron, 0.5, 1, &LAileron_down);
  AddAnimationComponent (anim_laileron, 0.5, 1, &LAileronDoor_down);
  anim_raileron = CreateAnimation (0.5);
  AddAnimationComponent (anim_raileron, 0, 0.5, &RAileron_down);
  AddAnimationComponent (anim_raileron, 0, 0.5, &RAileronDoor_down);
  AddAnimationComponent (anim_raileron, 0.5, 1, &RAileron_up);
  AddAnimationComponent (anim_raileron, 0.5, 1, &RAileronDoor_up);

  // ***** 6. Rudder animation *****

  static UINT RudderGrp[2] = {GRP_RUDDERR,GRP_RUDDERL};
  static MGROUP_ROTATE Rudder (midx, RudderGrp, 2,
    _V(0,5.611325,-11.835325), _V(0.037,-0.833,0.552), (float)(54.2*RAD));
  anim_rudder = CreateAnimation (0.5);
  AddAnimationComponent (anim_rudder, 0, 1, &Rudder);

  // ***** 7. Speedbrake animation *****

  static UINT SB1Grp[1] = {GRP_RUDDERR};
  static MGROUP_ROTATE SB1 (midx, SB1Grp, 1,
    _V(0.3112,5.611325,-11.835325), _V(-0.037,0.833,-0.552), (float)(-49.3*RAD));
  static UINT SB2Grp[1] = {GRP_RUDDERL};
  static MGROUP_ROTATE SB2 (midx, SB2Grp, 1,
    _V(-0.3112,5.611325,-11.835325), _V(0.037,0.833,-0.552), (float)(49.3*RAD));

  anim_spdb = CreateAnimation (0);
  AddAnimationComponent (anim_spdb, 0, 1, &SB1);
  AddAnimationComponent (anim_spdb, 0, 1, &SB2);

  // ***** 8. RMS arm animation *****
  // Note that the animation components can't be declared static here, since
  // their rotation parameters are modified by the respective parent transforms

  //080415, DaveS edit: Fixed RMS rollout animation as the end effector meshgroup didn't animate.
  static UINT RMSRolloutGrp[11] = {GRP_RMS_MPMs, GRP_base, GRP_Shoulder_Yaw, GRP_Humerus, GRP_Radii, GRP_elbowcam, GRP_camswivel, GRP_cambase, GRP_Wristpitch, GRP_Wrist_Yaw, GRP_Endeffector};
  rms_rollout_anim = new MGROUP_ROTATE (ridx, RMSRolloutGrp, 11,
    _V(-2.643, 1.282, 0.0), _V(0, 0, 1), (float)(31.36*RAD)); //1.05 or 1.10
  anim_rollout = CreateAnimation(1.0);
  AddAnimationComponent(anim_rollout, 0, 1, rms_rollout_anim);

  //DaveS edit: Fixed animation
  static UINT RMSShoulderYawGrp[1] = {GRP_Shoulder_Yaw};
  rms_anim[0] = new MGROUP_ROTATE (ridx, RMSShoulderYawGrp, 1,
    _V(-2.61,1.65,9.02), _V(-0.321040041302228, 0.947065621739415, 0), (float)(-360*RAD)); // -180 .. +180
  anim_arm_sy = CreateAnimation (0.5);
  parent = AddAnimationComponent (anim_arm_sy, 0, 1, rms_anim[0]);

  //DaveS edit: Fixed animation
  static UINT RMSShoulderPitchGrp[1] = {GRP_Humerus};
  rms_anim[1] = new MGROUP_ROTATE (ridx, RMSShoulderPitchGrp, 1,
    _V(-2.77,2.13,9.02), _V(0.948683298050514, 0.316227766016838, 0), (float)(147*RAD)); // -2 .. +145
  anim_arm_sp = CreateAnimation (0.0136);
  parent = AddAnimationComponent (anim_arm_sp, 0, 1, rms_anim[1], parent);

  //DaveS edit: Fixed animation. 080317 edit: Added elbow camera meshgroups
  static UINT RMSElbowPitchGrp[4] = {GRP_Radii,GRP_elbowcam ,GRP_camswivel, GRP_cambase};
  rms_anim[2] = new MGROUP_ROTATE (ridx, RMSElbowPitchGrp, 4,
    _V(-2.71,1.97,2.74), _V(0.948683598, 0.316226863954669, 0), (float)(-163.4*RAD));
  anim_arm_ep = CreateAnimation (0.014688);
  parent = AddAnimationComponent (anim_arm_ep, 0, 1, rms_anim[2], parent);

  //DaveS edit: Fixed animation
  static UINT RMSWristPitchGrp[1] = {GRP_Wristpitch};
  rms_anim[3] = new MGROUP_ROTATE (ridx, RMSWristPitchGrp, 1,
    _V(-2.77,2.13,-4.19), _V(0.949637404032871, 0.313350922867173, 0), (float)(242.8*RAD)); // -121.4 .. +121.4
  anim_arm_wp = CreateAnimation (0.5);
  parent = AddAnimationComponent (anim_arm_wp, 0, 1, rms_anim[3], parent);

  //DaveS edit: Fixed animation
  static UINT RMSWristYawGrp[1] = {GRP_Wrist_Yaw};
  rms_anim[4] = new MGROUP_ROTATE (ridx, RMSWristYawGrp, 1,
    _V(-2.77,2.13,-4.69), _V(0.314338082679218, -0.949311102735849, 0), (float)(-242.6*RAD)); // -121.3 .. +121.3
  anim_arm_wy = CreateAnimation (0.5);
  parent = AddAnimationComponent (anim_arm_wy, 0, 1, rms_anim[4], parent);

  //DaveS edit: Can't get the animation to line up properly. The coords used below is supposedly good ones from GMAX
  static UINT RMSEndEffectorGrp[1] = {GRP_Endeffector};
  rms_anim[5] = new MGROUP_ROTATE (ridx, RMSEndEffectorGrp, 1,
	  _V(-2.77,2.13,-5.52), _V(0, 0, 1), (float)(894*RAD));  // -447 .. +447
  anim_arm_wr = CreateAnimation (0.5);
  parent = AddAnimationComponent (anim_arm_wr, 0, 1, rms_anim[5], parent);

  rms_anim[6] = new MGROUP_ROTATE (LOCALVERTEXLIST, MAKEGROUPARRAY(arm_tip), 3,
    _V(-2.77, 2.13, -6.049), _V(0,0,1), (float)(0.0));
  anim_arm_ee = CreateAnimation (0.0);
  hAC_arm = AddAnimationComponent (anim_arm_ee, 0, 1, rms_anim[6], parent);

  //IK setup
  /*VECTOR3 shoulder_pos=_V(-10,-2.26,1.8); //wrong
  VECTOR3 elbow_pos=_V(-3.3,-2.26,1.7); //wrong
  VECTOR3 wrist_pos=_V(3.55,-2.26,1.7); //wrong*/
  VECTOR3 shoulder_pos=_V(-9.02, -2.77, 2.13);
  VECTOR3 elbow_pos=_V(-2.74, -2.71, 1.97);
  //VECTOR3 elbow_pos=_V(-2.76, -2.87, 2.03);
  VECTOR3 wrist_pos=_V(4.19, -2.77, 2.13);
  VECTOR3 wrist_yaw_pos=_V(4.69, -2.77, 2.13);
  VECTOR3 ee_pos=_V(6.049, -2.77, 2.13);
  elbow_pos-=shoulder_pos;
  wrist_pos-=shoulder_pos;
  wrist_yaw_pos-=shoulder_pos;
  ee_pos-=shoulder_pos;
  //shoulder_pos-=shoulder_pos;
  lu=length(elbow_pos);
  sp_null=DEG*acos(elbow_pos.x/lu);
  VECTOR3 temp=wrist_pos-elbow_pos;
  ll=length(temp);
  ep_null=DEG*acos(temp.x/ll);
  wp_wy=length(wrist_yaw_pos-wrist_pos);
  wy_ee=length(ee_pos-wrist_yaw_pos);
  shoulder_neutral=0.0136; //In anim coordinate
  shoulder_range=147;      //in deg
  shoulder_min=shoulder_range*-shoulder_neutral; //Min angle, deg
  shoulder_max=shoulder_range*(1-shoulder_neutral); //Max angle, deg
  elbow_neutral=0.014688;
  elbow_range=163.4;
  elbow_min=elbow_range*-elbow_neutral; //Min angle, deg
  elbow_max=elbow_range*(1-elbow_neutral); //Max angle, deg
  wrist_neutral=0.5; //In anim coordinate
  wrist_range=242.8;      //in deg
  wrist_min=wrist_range*-wrist_neutral; //Min angle, deg
  wrist_max=wrist_range*(1-wrist_neutral); //Max angle, deg
  wrist_yaw_range=242.6; //in deg
  wrist_yaw_min=wrist_yaw_range*-wrist_neutral; //Min angle, deg
  wrist_yaw_max=wrist_yaw_range*(1-wrist_neutral); //Max angle, deg
  wrist_roll_range=894.0; //in deg
  wrist_roll_min=wrist_roll_range*-wrist_neutral; //Min angle, deg
  wrist_roll_max=wrist_roll_range*(1-wrist_neutral); //Max angle, deg
  arm_wrist_pos=wrist_pos;

  // ***** 9 Payload cameras animation *****
  // DaveS edit: realigned with the scaled down orbiter mesh
  // FRONT LEFT
  static UINT camFLYawGrp[1] = {GRP_CAMERAPIVOTFL};
  CameraFLYaw = new MGROUP_ROTATE (midx, camFLYawGrp, 1,
    _V(orbiter_ofs.x-1.759,orbiter_ofs.y+1.656,orbiter_ofs.z+11.902), _V(0,1,0), (float)(340*RAD));
  anim_camFLyaw = CreateAnimation (0.5);
  parent = AddAnimationComponent (anim_camFLyaw, 0, 1, CameraFLYaw);

  static UINT camFLPitchGrp[1] = {GRP_CAMERAFL};
  CameraFLPitch = new MGROUP_ROTATE (midx, camFLPitchGrp, 1,
    _V(orbiter_ofs.x-1.759,orbiter_ofs.y+1.671,orbiter_ofs.z+11.874), _V(1,0,0), (float)(340*RAD));
  anim_camFLpitch = CreateAnimation (0.5);
  parent = AddAnimationComponent(anim_camFLpitch, 0, 1, CameraFLPitch, parent);

  // FRONT RIGHT
  static UINT camFRYawGrp[1] = {GRP_CAMERAPIVOTFR};
  CameraFRYaw = new MGROUP_ROTATE (midx, camFRYawGrp, 1,
    _V(orbiter_ofs.x+1.759,orbiter_ofs.y+1.656,orbiter_ofs.z+11.902), _V(0,1,0), (float)(340*RAD));
  anim_camFRyaw = CreateAnimation (0.5);
  parent = AddAnimationComponent (anim_camFRyaw, 0, 1, CameraFRYaw);

  static UINT camFRPitchGrp[1] = {GRP_CAMERAFR};
  CameraFRPitch = new MGROUP_ROTATE (midx, camFRPitchGrp, 1,
    _V(orbiter_ofs.x+1.759,orbiter_ofs.y+1.671,orbiter_ofs.z+11.874), _V(1,0,0), (float)(340*RAD));
  anim_camFRpitch = CreateAnimation (0.5);
  AddAnimationComponent (anim_camFRpitch, 0, 1, CameraFRPitch, parent);

  // BACK LEFT
  static UINT camBLYawGrp[1] = {GRP_CAMERAPIVOTBL};
  CameraBLYaw = new MGROUP_ROTATE (midx, camBLYawGrp, 1,
    _V(orbiter_ofs.x-2.263,orbiter_ofs.y+1.656,orbiter_ofs.z-6.42), _V(0,1,0), (float)(180*RAD));
  anim_camBLyaw = CreateAnimation (0.5);
  parent = AddAnimationComponent (anim_camBLyaw, 0, 1, CameraBLYaw);

  static UINT camBLPitchGrp[1] = {GRP_CAMERABL};
  CameraBLPitch = new MGROUP_ROTATE (midx, camBLPitchGrp, 1,
    _V(orbiter_ofs.x-2.263,orbiter_ofs.y+1.822,orbiter_ofs.z-6.413), _V(1,0,0), (float)(-340*RAD));
  anim_camBLpitch = CreateAnimation (0.5);
  AddAnimationComponent (anim_camBLpitch, 0, 1, CameraBLPitch, parent);

  // BACK RIGHT
  static UINT camBRYawGrp[1] = {GRP_CAMERAPIVOTBR};
  CameraBRYaw = new MGROUP_ROTATE (midx, camBRYawGrp, 1,
    _V(orbiter_ofs.x+2.263,orbiter_ofs.y+1.656,orbiter_ofs.z-6.42), _V(0,1,0), (float)(340*RAD));
  anim_camBRyaw = CreateAnimation (0.5);
  parent = AddAnimationComponent (anim_camBRyaw, 0, 1, CameraBRYaw);

  static UINT camBRPitchGrp[1] = {GRP_CAMERABR};
  CameraBRPitch = new MGROUP_ROTATE (midx, camBRPitchGrp, 1,
    _V(orbiter_ofs.x+2.263,orbiter_ofs.y+1.822,orbiter_ofs.z-6.413), _V(1,0,0), (float)(-340*RAD));
  anim_camBRpitch = CreateAnimation (0.5);
  AddAnimationComponent (anim_camBRpitch, 0, 1, CameraBRPitch, parent);

  // ***** 10 Dummy animation *****
  static MGROUP_ROTATE Dummy (midx, ElevGrp_up, 4,
    _V(-3.058,-2.137,-8.776), _V(1,0,0), (float)(0.0*RAD));
  anim_dummy = CreateAnimation (0.5);
  AddAnimationComponent (anim_dummy, 0, 1, &Dummy);

  // ***** 11 ET Umb Door animation *****
  // DaveS edit: still to be realigned
  static UINT ETUmbLGrp[1] = {GRP_ETUMBDOORL};
  static UINT ETUmbRGrp[1] = {GRP_ETUMBDOORR};
  static MGROUP_ROTATE EtumbdoorL (midx, ETUmbLGrp, 1,
	  UMBDOORL_REF, UMBDOOR_AXIS, (float)(+180.0*RAD));
  static MGROUP_ROTATE EtumbdoorR (midx, ETUmbRGrp, 1,
	  UMBDOORR_REF, UMBDOOR_AXIS, (float)(-180.0*RAD));
  anim_letumbdoor = CreateAnimation(0);
  anim_retumbdoor = CreateAnimation(0);
  AddAnimationComponent(anim_letumbdoor, 0, 1, &EtumbdoorL);
  AddAnimationComponent(anim_retumbdoor, 0, 1, &EtumbdoorR);

  // ***** SSME Gimbal Animations *****


  static UINT SSMET_Grp[1] = {GRP_SSMET};
  static UINT SSMEL_Grp[1] = {GRP_SSMEL};
  static UINT SSMER_Grp[1] = {GRP_SSMER};
  static VECTOR3 ssmet_dummy_vec;
  static VECTOR3 ssmer_dummy_vec;
  static VECTOR3 ssmel_dummy_vec;

  static MGROUP_ROTATE SSMETYaw (LOCALVERTEXLIST, MAKEGROUPARRAY(&ssmet_dummy_vec), 1, SSMET_REF, _V(0.0, 1.0, 0.0), (float)(21.0 * RAD));
  static MGROUP_ROTATE SSMETPitch (midx, SSMET_Grp, 1, SSMET_REF, _V(1.0, 0.0, 0.0), (float)(17.0 * RAD));

  anim_ssmeTyaw = CreateAnimation(0.5);
  anim_ssmeTpitch = CreateAnimation(0.5);

  parent = AddAnimationComponent(anim_ssmeTyaw, 0.0, 1.0, &SSMETYaw, NULL);
  AddAnimationComponent(anim_ssmeTpitch, 0.0, 1.0, &SSMETPitch, parent);

  static MGROUP_ROTATE SSMELYaw (LOCALVERTEXLIST, MAKEGROUPARRAY(&ssmel_dummy_vec), 1, SSMEL_REF, _V(0.0, 1.0, 0.0), (float)(21.0 * RAD));
  static MGROUP_ROTATE SSMELPitch (midx, SSMEL_Grp, 1, SSMEL_REF, _V(1.0, 0.0, 0.0), (float)(17.0 * RAD));

  anim_ssmeLyaw = CreateAnimation(0.5);
  anim_ssmeLpitch = CreateAnimation(0.5);

  parent = AddAnimationComponent(anim_ssmeLyaw, 0.0, 1.0, &SSMELYaw, NULL);
  AddAnimationComponent(anim_ssmeLpitch, 0.0, 1.0, &SSMELPitch, parent);


  static MGROUP_ROTATE SSMERYaw (LOCALVERTEXLIST, MAKEGROUPARRAY(&ssmer_dummy_vec), 1, SSMEL_REF, _V(0.0, 1.0, 0.0), (float)(21.0 * RAD));
  static MGROUP_ROTATE SSMERPitch (midx, SSMER_Grp, 1, SSMER_REF, _V(1.0, 0.0, 0.0), (float)(17.0 * RAD));

  anim_ssmeRyaw = CreateAnimation(0.5);
  anim_ssmeRpitch = CreateAnimation(0.5);

  parent = AddAnimationComponent(anim_ssmeRyaw, 0.0, 1.0, &SSMERYaw, NULL);
  AddAnimationComponent(anim_ssmeRpitch, 0.0, 1.0, &SSMERPitch, parent);

  // ======================================================
  // Air Data Probe Assembly Animations
  // ======================================================

  static UINT ADPL_Grp[1] = {GRP_PROBEL};
  static UINT ADPR_Grp[1] = {GRP_PROBER};

  static MGROUP_ROTATE ADPL_Deploy (midx, ADPL_Grp, 1, PROBEL_REF, PROBE_AXIS, (float)(180.0 * RAD));
  static MGROUP_ROTATE ADPR_Deploy (midx, ADPR_Grp, 1, PROBER_REF, PROBE_AXIS, (float)(-180.0 * RAD));

  anim_adpl = CreateAnimation(1.0);
  AddAnimationComponent(anim_adpl, 0.0, 1.0, &ADPL_Deploy);

  anim_adpr = CreateAnimation(1.0);
  AddAnimationComponent(anim_adpr, 0.0, 1.0, &ADPR_Deploy);



  // ======================================================
  // VC animation definitions
  // ======================================================
  plop->DefineAnimations (vidx);
  gop->DefineVCAnimations (vidx);
  panela4->DefineVCAnimations (vidx);
  panela8->DefineVCAnimations (vidx);
  panelc2->DefineVCAnimations (vidx);
  panelf7->DefineVCAnimations (vidx);
  panelo3->DefineVCAnimations (vidx);
  // ======================================================
  c3po->DefineVCAnimations (vidx);
  r2d2->DefineVCAnimations (vidx);
}

void Atlantis::AddOrbiterVisual (const VECTOR3 &ofs)
{
  orbiter_ofs = ofs;
  huds.hudcnt = _V(ofs.x-0.671257, ofs.y+2.523535, ofs.z+14.969);

  if (mesh_orbiter == MESH_UNDEFINED) {

    // ***** Load meshes

    mesh_cockpit = AddMesh (hOrbiterCockpitMesh, &ofs);
    SetMeshVisibilityMode (mesh_cockpit, MESHVIS_EXTERNAL);

    mesh_orbiter = AddMesh (hOrbiterMesh, &ofs);
    SetMeshVisibilityMode (mesh_orbiter, MESHVIS_EXTERNAL|MESHVIS_VC|MESHVIS_EXTPASS);

	SURFHANDLE insignia_tex = oapiGetTextureHandle (hOrbiterMesh, TEX_ATLANTIS);
	PaintMarkings (insignia_tex);

    mesh_vc = AddMesh (hOrbiterVCMesh, &ofs);
    SetMeshVisibilityMode (mesh_vc, MESHVIS_VC);

	if(RMS) {
		mesh_rms = AddMesh (hOrbiterRMSMesh, &ofs);
		SetMeshVisibilityMode (mesh_rms, MESHVIS_EXTERNAL|MESHVIS_VC|MESHVIS_EXTPASS);
	}

	AddKUBandVisual(ofs);

    for (int i = 0; i < 10; i++) mfds[i].nmesh = mesh_vc;
    huds.nmesh = mesh_vc;

    if (do_cargostatic) {
      VECTOR3 cofs = ofs + cargo_static_ofs;
      AddMesh (cargo_static_mesh_name, &cofs);
    }
    if (do_plat) {
      VECTOR3 plat_ofs = _V(-2.59805, 1.69209, -5.15524);
      AddMesh("shuttle_eva_plat", &plat_ofs);
    }

    // ***** Docking definitions

    SetDockParams (ofs+ORBITER_DOCKPOS, _V(0,1,0), _V(0,0,-1));

    // ***** Attachment definitions

    if (!sat_attach) sat_attach = CreateAttachment (false, ofs+ofs_sts_sat, _V(0,1,0), _V(0,0,1), "X");
	//080415, DaveS add: Added temporary OBSS MPM attachment point
	if (!obss_attach) obss_attach = CreateAttachment (false, _V(2.83, 1.05, 1.68), _V(0,1,0), _V(0,0,1), "OBSS");
    if (!rms_attach) rms_attach = CreateAttachment (false, ofs+arm_tip[0], arm_tip[1]-arm_tip[0], arm_tip[2]-arm_tip[0], "G", true);

    // ***** Cockpit camera definition

    SetCameraOffset (_V(ofs.x-0.67,ofs.y+2.55,ofs.z+14.4));
    oapiVCRegisterHUD (&huds); // register changes in HUD parameters

//	DefineAnimations();

  }
}

void Atlantis::AddTankVisual (const VECTOR3 &ofs)
{
  if (mesh_tank == MESH_UNDEFINED) {

    // ***** Load mesh
    mesh_tank = AddMesh (hTankMesh, &ofs);
    SetMeshVisibilityMode (mesh_tank, MESHVIS_ALWAYS|MESHVIS_EXTPASS);
  }
}

void Atlantis::AddSRBVisual (int which, const VECTOR3 &ofs)
{
  if (mesh_srb[which] == MESH_UNDEFINED) {

    // ***** Load mesh
    //mesh_srb[which] = AddMesh (hSRBMesh, &ofs);
    mesh_srb[which] = AddMesh (hSRBMesh[which], &ofs);
    SetMeshVisibilityMode (mesh_srb[which], MESHVIS_ALWAYS|MESHVIS_EXTPASS);
    int id = AddExhaustRef (EXHAUST_CUSTOM, _V(ofs.x, ofs.y, ofs.z-21.8), 0, 0);
    if (which) srb_id1 = id;
    else       srb_id2 = id;
  }
  if(ahHDP == NULL)
  {
	  ahHDP = CreateAttachment(true, POS_HDP, _V(0.0, 0.0, -1.0), _V(0.0, 1.0, 0.0), "XHDP");
  }
}

void Atlantis::SeparateBoosters (double met)
{
	int i;
	char buffer[120];
  // Create SRB's as individual objects
  VESSELSTATUS2 vs;
  VESSELSTATUS2::FUELSPEC fuel;
  VESSELSTATUS2::THRUSTSPEC thrust;
  memset (&vs, 0, sizeof(vs));
  vs.version = 2;
  GetStatusEx (&vs);
  vs.flag = VS_FUELLIST | VS_THRUSTLIST;
  vs.fuel = &fuel;
  vs.nfuel = 1;
  vs.fuel->idx = 0;
  vs.thruster = &thrust;
  vs.nthruster = 1;
  vs.thruster->idx = 0;
  GetSRB_State (met, vs.thruster->level, vs.fuel->level);
  Local2Rel (OFS_LAUNCH_RIGHTSRB, vs.rpos);
  //vs.arot.z += 0.25*PI;
  vs.status = 0;
  char name[256];
  strcpy (name, GetName()); strcat (name, "-SRB1");
  oapiCreateVesselEx (name, "Atlantis_RSRB", &vs);
  Local2Rel (OFS_LAUNCH_LEFTSRB, vs.rpos);
  //vs.arot.z -= 1.5*PI;
  name[strlen(name)-1] = '2';
  oapiCreateVesselEx (name, "Atlantis_LSRB", &vs);

  sprintf(buffer, "MG_Atlantis: Residual SRB propellant mass is %f kg\n", GetPropellantMass(ph_srb));
  oapiWriteLog(buffer);

  // Remove SRB's from Shuttle instance
  DelPropellantResource (ph_srb);
  DelThrusterGroup (thg_srb, THGROUP_USER, true);
  for(i = 0; i<2; i++)
  {
	DelExhaustStream(pshSlag1[i]);
    DelExhaustStream(pshSlag2[i]);
	DelExhaustStream(pshSlag3[i]);
  }

  // remove srb meshes and shift cg
  DelMesh(mesh_srb[1], true);
  DelMesh(mesh_srb[0], true);
  ShiftCG (OFS_LAUNCH_ORBITER-OFS_WITHTANK_ORBITER);


  // reconfigure
  RecordEvent ("JET", "SRB");
  SetOrbiterTankConfiguration();
  char cbuf[255];
  sprintf(cbuf, "Boosters separated");
  oapiWriteLog(cbuf);
}

void Atlantis::SeparateTank (void)
{
  // Create Tank as individual object
  VESSELSTATUS2 vs;
  memset (&vs, 0, sizeof (vs));
  vs.version = 2;
  GetStatusEx (&vs);
  vs.flag = VS_FUELRESET | VS_THRUSTRESET;
  VECTOR3 ofs = OFS_WITHTANK_TANK;
  if (Playback()) // necessary because during playback the CG shift occurs before separation
    ofs -= OFS_WITHTANK_ORBITER;
  VECTOR3 rofs, rvel = {vs.rvel.x, vs.rvel.y, vs.rvel.z};
  VECTOR3 vel = {0,-1,0};
  Local2Rel (ofs, vs.rpos);
  GlobalRot (vel, rofs);
  vs.rvel.x = rvel.x+rofs.x;
  vs.rvel.y = rvel.y+rofs.y;
  vs.rvel.z = rvel.z+rofs.z;
  vs.vrot.x = -0.02;
  vs.status = 0;
  char name[256];
  strcpy (name, GetName()); strcat (name, "-Tank");
  oapiCreateVesselEx (name, "Atlantis_Tank", &vs);

  // Remove Tank from shuttle instance
  DelPropellantResource (ph_tank);

  // main engines are done
  DelThrusterGroup (thg_main, THGROUP_MAIN, true);

  // clear launch attitude control system
  DelThrusterGroup (THGROUP_ATT_PITCHUP, true);
  DelThrusterGroup (THGROUP_ATT_PITCHDOWN, true);
  DelThrusterGroup (THGROUP_ATT_BANKLEFT, true);
  DelThrusterGroup (THGROUP_ATT_BANKRIGHT, true);
  DelThrusterGroup (THGROUP_ATT_YAWLEFT, true);
  DelThrusterGroup (THGROUP_ATT_YAWRIGHT, true);

  // remove tank mesh and shift cg
  //Test keeping animations - which are not defined on the ET.
  DelMesh (mesh_tank, true);
  ShiftCG (OFS_WITHTANK_ORBITER);

  // reconfigure
  RecordEvent ("JET", "ET"); 
  SetOrbiterConfiguration ();
}

void Atlantis::ToggleGrapple (void)
{
  HWND hDlg;
  OBJHANDLE hV = GetAttachmentStatus (rms_attach);

  if(!RMS) return; //no arm
  if (hV) {  // release satellite

    ATTACHMENTHANDLE hAtt = CanArrest();
    DetachChild (rms_attach);
    if (hDlg = oapiFindDialog (g_Param.hDLL, IDD_RMS)) {
      SetWindowText (GetDlgItem (hDlg, IDC_GRAPPLE), "Grapple");
      EnableWindow (GetDlgItem (hDlg, IDC_STOW), TRUE);
    }
    // check whether the object being ungrappled is ready to be clamped into the payload bay
    if (hAtt) {
      AttachChild (hV, sat_attach, hAtt);
      if (hDlg) {
        SetWindowText (GetDlgItem (hDlg, IDC_PAYLOAD), "Purge");
        EnableWindow (GetDlgItem (hDlg, IDC_PAYLOAD), TRUE);
      }
    }

#ifdef UNDEF
    VECTOR3 pos, dir, rot, gbay, gpos;
    GetAttachmentParams (sat_attach, pos, dir, rot);
    Local2Global (pos, gbay);
    VESSEL *v = oapiGetVesselInterface (hV);
    DWORD nAttach = v->AttachmentCount (true);
    for (DWORD j = 0; j < nAttach; j++) { // now scan all attachment points
      ATTACHMENTHANDLE hAtt = v->GetAttachmentHandle (true, j);
      v->GetAttachmentParams (hAtt, pos, dir, rot);
      v->Local2Global (pos, gpos);
      if (dist (gpos, gbay) < MAX_GRAPPLING_DIST) {
        AttachChild (hV, sat_attach, hAtt);
        return;
      }
    }
#endif

  } else {             // grapple satellite

    VECTOR3 gpos, grms, pos, dir, rot;
    Local2Global (orbiter_ofs+arm_tip[0], grms);  // global position of RMS tip

    // Search the complete vessel list for a grappling candidate.
    // Not very scalable ...
    for (DWORD i = 0; i < oapiGetVesselCount(); i++) {
      OBJHANDLE hV = oapiGetVesselByIndex (i);
      if (hV == GetHandle()) continue; // we don't want to grapple ourselves ...
      oapiGetGlobalPos (hV, &gpos);
      if (dist (gpos, grms) < oapiGetSize (hV)) { // in range
        VESSEL *v = oapiGetVesselInterface (hV);
        DWORD nAttach = v->AttachmentCount (true);
        for (DWORD j = 0; j < nAttach; j++) { // now scan all attachment points of the candidate
          ATTACHMENTHANDLE hAtt = v->GetAttachmentHandle (true, j);
          const char *id = v->GetAttachmentId (hAtt);
          if (strncmp (id, "GS", 2)) continue; // attachment point not compatible
          v->GetAttachmentParams (hAtt, pos, dir, rot);
          v->Local2Global (pos, gpos);
          if (dist (gpos, grms) < MAX_GRAPPLING_DIST) { // found one!
            // check whether satellite is currently clamped into payload bay
            if (hV == GetAttachmentStatus (sat_attach))
              DetachChild (sat_attach);
            AttachChild (hV, rms_attach, hAtt);
            if (hDlg = oapiFindDialog (g_Param.hDLL, IDD_RMS)) {
              SetWindowText (GetDlgItem (hDlg, IDC_GRAPPLE), "Release");
              EnableWindow (GetDlgItem (hDlg, IDC_STOW), FALSE);
            }
            return;
          }
        }
      }
    }

  }
}

void Atlantis::ToggleArrest (void)
{
  HWND hDlg;
  if (SatStowed()) { // purge satellite
    DetachChild (sat_attach, 0.1);
    if (hDlg = oapiFindDialog (g_Param.hDLL, IDD_RMS)) {
      SetWindowText (GetDlgItem (hDlg, IDC_PAYLOAD), "Arrest");
      EnableWindow (GetDlgItem (hDlg, IDC_PAYLOAD), CanArrest() ? TRUE:FALSE);
    }
  } else if (CanArrest()) {           // try to arrest satellite
    ToggleGrapple();
  }
}

// check whether the currently grappled object can be stowed in the cargo bay
ATTACHMENTHANDLE Atlantis::CanArrest (void) const
{
  OBJHANDLE hV = GetAttachmentStatus (rms_attach);
  if (!hV) return 0;
  VESSEL *v = oapiGetVesselInterface (hV);
  DWORD nAttach = v->AttachmentCount (true);
  VECTOR3 pos, dir, rot, gpos, gbay;
  GetAttachmentParams (sat_attach, pos, dir, rot);
  Local2Global (pos, gbay);
  for (DWORD j = 0; j < nAttach; j++) {
    ATTACHMENTHANDLE hAtt = v->GetAttachmentHandle (true, j);
    if (strncmp (v->GetAttachmentId (hAtt), "XS", 2)) continue; // attachment point not compatible
    v->GetAttachmentParams (hAtt, pos, dir, rot);
    v->Local2Global (pos, gpos);
    if (dist (gpos, gbay) < MAX_GRAPPLING_DIST) {
      return hAtt;
    }
  }
  return 0;
}

bool Atlantis::ArmCradled()
{
	if(!RMS) return true;
	if(!Eq(arm_sy, 0.5)) return false;
	if(!Eq(arm_sp, shoulder_neutral)) return false;
	if(!Eq(arm_ep, elbow_neutral)) return false;
	if(!Eq(arm_wp, wrist_neutral)) return false;
	if(!Eq(arm_wy, wrist_neutral)) return false;
	if(!Eq(arm_wr, wrist_neutral)) return false;
	return true;
}

void Atlantis::UpdateMRLMicroswitches()
{
	//int i;
	if(!RMS) return;
	MRL_FwdMicroswitches[0][2]=MRL_MidMicroswitches[0][2]=MRL_AftMicroswitches[0][2]=0;
	if(Eq(arm_sy, 0.5) && Eq(arm_sp, shoulder_neutral)) {
		MRL_FwdMicroswitches[0][2]=1;
		if(Eq(arm_ep, elbow_neutral)) {
			MRL_MidMicroswitches[0][2]=1;
			if(Eq(arm_wp, wrist_neutral) && Eq(arm_wy, wrist_neutral) && Eq(arm_wr, wrist_neutral)) {
				MRL_AftMicroswitches[0][2]=1;
			}
		}
	}

	if(Eq(MRL[0], 1.0)) {
		MRL_FwdMicroswitches[0][0]=MRL_MidMicroswitches[0][0]=MRL_AftMicroswitches[0][0]=1;
		MRL_FwdMicroswitches[0][1]=MRL_MidMicroswitches[0][1]=MRL_AftMicroswitches[0][1]=0;
	}
	else if(Eq(MRL[0], 0.0)){
		MRL_FwdMicroswitches[0][0]=MRL_MidMicroswitches[0][0]=MRL_AftMicroswitches[0][0]=0;
		MRL_FwdMicroswitches[0][1]=MRL_MidMicroswitches[0][1]=MRL_AftMicroswitches[0][1]=1;
	}
	else {
		MRL_FwdMicroswitches[0][0]=MRL_MidMicroswitches[0][0]=MRL_AftMicroswitches[0][0]=0;
		MRL_FwdMicroswitches[0][1]=MRL_MidMicroswitches[0][1]=MRL_AftMicroswitches[0][1]=0;
	}
}

void Atlantis::UpdateMPMMicroswitches()
{
	if(RMSRollout.action==AnimState::OPEN) {
		MPM_Microswitches[0][0]=0;
		MPM_Microswitches[0][1]=1;
	}
	else if(RMSRollout.action==AnimState::CLOSED) {
		MPM_Microswitches[0][0]=1;
		MPM_Microswitches[0][1]=0;
	}
	else {
		MPM_Microswitches[0][0]=0;
		MPM_Microswitches[0][1]=0;
	}
	panela8->UpdateVC();
}

void Atlantis::SeparateMMU (void)
{
  // Create Tank as individual object
  VESSELSTATUS vs;
  GetStatus (vs);
  char name[256];
  strcpy (name, GetName()); strcat (name, "-MMU");
  hMMU = VESSEL::Create (name, "Nasa_MMU", vs);
  jettison_time = oapiGetSimTime();
  reset_mmu = true;
  // Remove Tank from shuttle instance
  SetOrbiterConfiguration ();
}

void Atlantis::SteerGimbal() {
	int i;

  //Use the left and right main engines to steer (after SRBs are gone)
	VECTOR3 pitchcorrect, yawcorrect, rollcorrect;
	VECTOR3 RateDeltas;
	GetAngularVel(AngularVelocity);
	for(i=0;i<3;i++) {
		RateDeltas.data[i]=ReqdRates.data[i]-(DEG*AngularVelocity.data[i]);
	}
	if(!r2d2->bHydraulicPressure) {
		for(int i=0;i<3;i++) {
			pitchcorrect.data[i]=0.0;
			yawcorrect.data[i]=0.0;
			rollcorrect.data[i]=0.0;
		}
	}
	else {
		pitchcorrect.data[1]=pitchcorrect.data[2]=range(-0.18, -0.03*RateDeltas.data[PITCH], 0.18);
		pitchcorrect.data[0]=range(-0.18, -0.06*RateDeltas.data[PITCH], 0.18);
		yawcorrect.data[1]=yawcorrect.data[2]=range(-0.15, 0.03*RateDeltas.data[YAW], 0.15);
		yawcorrect.data[0]=range(-0.15, 0.07*RateDeltas.data[YAW], 0.15);
		rollcorrect.data[0]=0.0;
		rollcorrect.data[1]=range(-0.18, 0.03*RateDeltas.data[ROLL], 0.18);
		rollcorrect.data[2]=range(-0.18, -0.03*RateDeltas.data[ROLL], 0.18);
	}
	//SetThrusterDir(th_main[0], NormZ(_V( 0.0+yawcorrect.data[0],-0.308046+pitchcorrect.data[0]+rollcorrect.data[0],0.951372)));
	//SetThrusterDir(th_main[1], NormZ(_V( 0.0624+yawcorrect.data[1],-0.1789+pitchcorrect.data[1]+rollcorrect.data[1],0.9819)));
	//SetThrusterDir(th_main[2], NormZ(_V(-0.0624+yawcorrect.data[2],-0.1789+pitchcorrect.data[2]+rollcorrect.data[2],0.9819)));
	for(i=0;i<3;i++) {
		SetThrusterDir(th_main[i], NormZ(EngineNullPosition[i]+_V(yawcorrect.data[i], pitchcorrect.data[i]+rollcorrect.data[i], 0.0)));
	}
	//sprintf(oapiDebugString(), "%f %f %f", EngineNullPosition[0].x, EngineNullPosition[0].y, EngineNullPosition[0].z);

	UpdateSSMEGimbalAnimations();
}

void Atlantis::AutoMainGimbal () {
  //Steer with the SRBs and lower SSMEs
	VECTOR3 pitchcorrect, yawcorrect, rollcorrect;
	VECTOR3 RateDeltas;
	int i;
	
	GetAngularVel(AngularVelocity);
	for(i=0;i<3;i++) {
		RateDeltas.data[i]=ReqdRates.data[i]-(DEG*AngularVelocity.data[i]);
	}
	if(!r2d2->bHydraulicPressure) {
		for(i=0;i<3;i++) {
			pitchcorrect.data[i]=0.0;
			yawcorrect.data[i]=0.0;
			rollcorrect.data[i]=0.0;
		}
	}
	else {
		pitchcorrect.data[1]=pitchcorrect.data[2]=range(-0.18, -0.03*RateDeltas.data[PITCH], 0.18);
		pitchcorrect.data[0]=range(-0.18, -0.06*RateDeltas.data[PITCH], 0.18);
		yawcorrect.data[1]=yawcorrect.data[2]=range(-0.15, 0.03*RateDeltas.data[YAW], 0.15);
		yawcorrect.data[0]=range(-0.15, 0.07*RateDeltas.data[YAW], 0.15);
		rollcorrect.data[0]=0.0;
		rollcorrect.data[1]=range(-0.18, 0.03*RateDeltas.data[ROLL], 0.18);
		rollcorrect.data[2]=range(-0.18, -0.03*RateDeltas.data[ROLL], 0.18);
	}
	SetThrusterDir(th_srb[0], NormZ(_V(0.0+yawcorrect.data[1],0.069338+pitchcorrect.data[1]+rollcorrect.data[1],0.99759)));
	SetThrusterDir(th_srb[1], NormZ(_V(0.0+yawcorrect.data[2],0.069338+pitchcorrect.data[2]+rollcorrect.data[2],0.99759)));
	for(i=0;i<3;i++) {
		SetThrusterDir(th_main[i], NormZ(EngineNullPosition[i]+_V(yawcorrect.data[i], pitchcorrect.data[i]+rollcorrect.data[i], 0.0)));
	}
	/*SetThrusterDir(th_main[0], NormZ(_V( 0.0+yawcorrect.data[0],-0.37489+pitchcorrect.data[0]+rollcorrect.data[0],0.92707)));
	SetThrusterDir(th_main[1], NormZ(_V( 0.065+yawcorrect.data[1],-0.2447+pitchcorrect.data[1]+rollcorrect.data[1],0.9674)));
	SetThrusterDir(th_main[2], NormZ(_V(-0.065+yawcorrect.data[2],-0.2447+pitchcorrect.data[2]+rollcorrect.data[2],0.9674)));*/

	UpdateSSMEGimbalAnimations();
}

double Atlantis::CalcNetThrust()
{
	VECTOR3 N=_V(0, 0, 0), F, M;
	for(int i=0;i<3;i++) {
		GetThrusterMoment(th_main[i], F, M);
		N+=F;
	}
	return length(N);
}

void Atlantis::CalcThrustAngles()
{
	VECTOR3 N=_V(0, 0, 0);
	N+=_V( 0.0,-0.308046,0.951372)*GetThrusterLevel(th_main[0]);
	N+=_V( 0.0624,-0.1789,0.9819)*GetThrusterLevel(th_main[1]);
	N+=_V(-0.0624,-0.1789,0.9819)*GetThrusterLevel(th_main[2]);
	ThrAngleP=DEG*asin(N.y/N.z);
	ThrAngleY=DEG*asin(N.x/N.z);
}

void Atlantis::FailEngine(int engine)
{
	SetThrusterResource(th_main[engine], NULL);
	//should check throttle increase
	MaxThrust=109.0;
	for(int i=0;i<3;i++)
	{
		SetThrusterMax0(th_main[i], SSME_RATED_THRUST*(MaxThrust/100.0));
	}
	CalcThrustAngles();
	bEngineFail=false;
}

void Atlantis::LaunchClamps ()
{
  VECTOR3 F, T, r = _V(0,0,0), Fc = _V(0,0,0), Tc = _V(0,0,0);
  GetThrusterMoment (th_srb[0], F, T);
  Fc.z = -2*F.z;
  Tc.x =  2*T.x;
  GetThrusterMoment (th_main[0], F, T);
  Fc.z -= 2*F.z;
  Fc.y -= 2*F.y;
  Tc.x += 2*T.x;
  GetThrusterMoment (th_main[2], F, T);
  Fc.z -= F.z;
  Fc.y -= F.y;
  Tc.x += T.x;
  r.z = (Fc.y ? Tc.x/Fc.y : 0);
  AddForce (Fc, r);
}

void Atlantis::Jettison ()
{
  switch (status) {
  case 0:
  case 3:               // nothing to do
    break;
  case 1:               // abandon boosters
    SeparateBoosters (oapiGetSimTime()-t0);
    break;
  case 2:               // abandon tank
    SeparateTank();
    break;
  }
}

// Update moving parts of the orbiter's visual: payload bay doors and gear
// This should only be called when the visual exists, e.g. from within
// clbkVisualCreated or clbkAnimate

void Atlantis::UpdateMesh ()
{
  // update animation states
  gop->UpdateMesh();
  SetAnimation (anim_spdb, spdb_proc);
  SetAnimation (anim_door, plop->BayDoorStatus.pos);
  SetAnimation (anim_rad,  plop->RadiatorStatus.pos);
  SetAnimation (anim_kubd, plop->KuAntennaStatus.pos);

  SetAnimationArm (anim_arm_sy, arm_sy);
  SetAnimationArm (anim_arm_sp, arm_sp);
  SetAnimationArm (anim_arm_ep, arm_ep);
  SetAnimationArm (anim_arm_wp, arm_wp);
  SetAnimationArm (anim_arm_wy, arm_wy);
  SetAnimationArm (anim_arm_wr, arm_wr);
  UpdateRMSPositions();

  // update MFD brightness
  if (vis) {
    int i;
    MESHHANDLE hMesh = GetMesh (vis, mesh_vc);
    for (i = 0; i < 10; i++) {
      MATERIAL *mat = oapiMeshMaterial (hMesh, 10+i);
      mat->emissive.r = mat->emissive.g = mat->emissive.b = (float)mfdbright[i];
    }
  }
}

void Atlantis::ClearMeshes ()
{
  VESSEL::ClearMeshes();
  mesh_orbiter = MESH_UNDEFINED;
  mesh_kuband  = MESH_UNDEFINED;
  mesh_rms     = MESH_UNDEFINED;
  mesh_cockpit = MESH_UNDEFINED;
  mesh_vc      = MESH_UNDEFINED;
  mesh_tank    = MESH_UNDEFINED;
  mesh_srb[0] = mesh_srb[1] = MESH_UNDEFINED;
}

void Atlantis::SetBayDoorPosition (double pos)
{
  SetAnimation (anim_door, pos);
  rdoor_drag = sqrt (min (1.0, pos*3.0));
  ldoor_drag = sqrt (min (1.0, max(0.0, pos-0.3656)*3.0));
}

void Atlantis::SetRadiatorPosition (double pos)
{
  SetAnimation (anim_rad, pos);
}

void Atlantis::SetKuAntennaPosition (double pos)
{
  SetAnimation (anim_kubd, pos);
}

void Atlantis::SetETUmbDoorPosition (double pos, int door)
{
	if(door==0) SetAnimation (anim_letumbdoor, pos);
	else SetAnimation (anim_retumbdoor, pos);
}

void Atlantis::OperateSpeedbrake (AnimState::Action action)
{
  spdb_status = action;
  RecordEvent ("SPEEDBRAKE", action == AnimState::CLOSING ? "CLOSE" : "OPEN");
}

void Atlantis::RevertSpeedbrake (void)
{
  if(spdb_status == AnimState::CLOSED || spdb_status == AnimState::CLOSING) spdb_tgt=1.0;
  else if(spdb_status == AnimState::OPEN || spdb_status == AnimState::OPENING) spdb_tgt=0.0;
  OperateSpeedbrake (spdb_status == AnimState::CLOSED || spdb_status == AnimState::CLOSING ?
    AnimState::OPENING : AnimState::CLOSING);
}

void Atlantis::SetSpeedbrake(double tgt)
{
	spdb_tgt=tgt;
	if(spdb_tgt<spdb_proc) OperateSpeedbrake(AnimState::CLOSING);
	else if(spdb_tgt>spdb_proc) OperateSpeedbrake(AnimState::OPENING);
}

void Atlantis::SetAnimationArm (UINT anim, double state)
{
  if(!RMS) return;
  if(RMSRollout.action!=AnimState::OPEN || !Eq(shoulder_brace, 0.0) || !Eq(MRL[0], 0.0)) return;
  SetAnimation (anim, state);
  arm_moved = true;
  UpdateMRLMicroswitches();
  panela8->UpdateVC();

  HWND hDlg;
  if (!SatStowed() && (hDlg = oapiFindDialog (g_Param.hDLL, IDD_RMS))) {
    SetWindowText (GetDlgItem (hDlg, IDC_PAYLOAD), "Arrest");
    EnableWindow (GetDlgItem (hDlg, IDC_PAYLOAD), CanArrest() ? TRUE : FALSE);
  }
  //CalcAnimationFKArm();
}

void Atlantis::SetAnimationCameras() {
  double a = 0;
  double b = 0;

  // FRONT LEFT
  double anim_yaw = linterp(-170, 0, 170, 1, camFLyaw);
  SetAnimation(anim_camFLyaw, anim_yaw);

    double anim_pitch = linterp(-170, 0, 170, 1, camFLpitch);
  SetAnimation(anim_camFLpitch, anim_pitch);

  // FRONT RIGHT
    anim_yaw = linterp(-170, 0, 170, 1, camFRyaw);
  SetAnimation(anim_camFRyaw, anim_yaw);

    anim_pitch = linterp(-170, 0, 170, 1, camFRpitch);
  SetAnimation(anim_camFRpitch, anim_pitch);

  // BACK LEFT
    anim_yaw = linterp(-170, 0, 170, 1, camBLyaw);
  SetAnimation(anim_camBLyaw, anim_yaw);

    anim_pitch = linterp(-170, 0, 170, 1, camBLpitch);
  SetAnimation(anim_camBLpitch, anim_pitch);

  // BACK RIGHT
    anim_yaw = linterp(-170, 0, 170, 1, camBRyaw);
  SetAnimation(anim_camBRyaw, anim_yaw);

    anim_pitch = linterp(-170, 0, 170, 1, camBRpitch);
  SetAnimation(anim_camBRpitch, anim_pitch);

    switch (VCMode) {
  case VC_PLBCAMFL:
    a = ((-camFLyaw+90)*RAD);
    b = ((camFLpitch-90)*RAD);
    break;
  case VC_PLBCAMFR:
    a = ((-camFRyaw+90)*RAD);
    b = ((camFRpitch-90)*RAD);
    break;
  case VC_PLBCAMBL:
    a = ((-camBLyaw-90)*RAD);
    b = ((camBLpitch-90)*RAD);
    break;
  case VC_PLBCAMBR:
    a = ((-camBRyaw-90)*RAD);
    b = ((camBRpitch-90)*RAD);
    break;
    }

  SetCameraDefaultDirection (_V(cos(a)*sin(b), cos(b), sin(a)*sin(b)));
}

void Atlantis::EnableThrusters(const int Thrusters[], int nThrusters)
{
	if(bUseRealRCS)
		return;

	for(int i=0;i<nThrusters;i++) {
		if(Thrusters[i]%2==1)
			SetThrusterResource(th_att_rcs[Thrusters[i]], ph_oms);
		else
			SetThrusterResource(th_att_rcs[Thrusters[i]], ph_frcs);
	}
}

void Atlantis::DisableThrusters(const int Thrusters[], int nThrusters)
{
	if(bUseRealRCS)
		return;

	for(int i=0;i<nThrusters;i++) {
		SetThrusterResource(th_att_rcs[Thrusters[i]], NULL);
	}
}

bool Atlantis::Input(int mfd, int change, char *Name, char *Data)
{
	//int item;
	int nNew;
	double dNew;
	int id=Display[mfd]->id;

	if (change == 9)
	{
		//Resume key pressed
		if(Display[mfd]->display >= 0) {
			Display[mfd]->display = -1;
			return true;
		} else if(Display[mfd]->spec >= 0) {
			Display[mfd]->spec = -1;
			return true;
		} else
			return false;
	}

	if(id>2 || (id<2 && panelc2->switch_state[SWITCH2+2*id]==0)) //GNC
	{
		if(change==0) {
			nNew=atoi(Name);
			if(nNew==104 && ops==103) {
				ops=104;
				WT=GetMass()*kg_to_pounds;
				BurnInProg=false;
				BurnCompleted=false;
				MNVRLOAD=false;
				Display[mfd]->bTIMER=false;
			}
			else if(nNew==105 && ops==104) {
				ops=105;
				WT=GetMass()*kg_to_pounds;
				BurnInProg=false;
				BurnCompleted=false;
				MNVRLOAD=false;
				Display[mfd]->bTIMER=false;
			}
			else if(nNew==106 && ops==105) {
				ops=106;
			}
			else if(nNew==201 && (ops==202 || ops==106))
			{
				ops=201;
				MNVR=false;
				TRK=false;
				ROT=false;
			}
			else if(nNew==202 && ops==201)
			{
				ops=202;
				WT=GetMass()*kg_to_pounds;
				BurnInProg=false;
				BurnCompleted=false;
				MNVRLOAD=false;
				Display[mfd]->bTIMER=false;
			}
			else if(nNew==301 && ops==201)
			{
				ops=301;
				WT=GetMass()*kg_to_pounds;
				BurnInProg=false;
				BurnCompleted=false;
				MNVRLOAD=false;
				Display[mfd]->bTIMER=false;
			}
			else if(nNew==302 && ops==301)
			{
				ops=302;
			}
			else if(nNew==303 && ops==302)
			{
				ops=303;
			}
			return true;
		}
		else if(change==1) {
			nNew=atoi(Name);
			if(ops==201) {
				switch(Display[mfd]->spec) {
					case 0:
						if(nNew<=17) {
							item=nNew;
							return Input(mfd, 3, Name, Data);
							//data=1;
							//return true;
						}
						else if(nNew==18) {
							if(MNVR==false) 
							{
								MNVR=true;
								ROT=false;
								TRK=false;
								REQD_ATT.x=MNVR_OPTION.x;
								REQD_ATT.y=MNVR_OPTION.y;
								REQD_ATT.z=MNVR_OPTION.z;
								for(int i=0;i<3;i++) {
									if(REQD_ATT.data[i]>180.0) TargetAtt.data[i]=REQD_ATT.data[i]-360.0;
									else TargetAtt.data[i]=REQD_ATT.data[i];
								}
								TargetAtt=ToRad(TargetAtt);
							}
							else MNVR=false;
							Yaw=false;
							Pitch=false;
							Roll=false;
							return true;
						}
						else if(nNew==19) {
							if(TRK==false) {
								TRK=true;
								MNVR=false;
								ROT=false;
								if(TGT_ID==2) {
									if(BODY_VECT==1) {
										LVLHOrientationReqd.data[PITCH]=270.0;
										LVLHOrientationReqd.data[YAW]=0.0;
										if((OM)<=0.0) {
											LVLHOrientationReqd.data[ROLL]=0.0;
											LVLHRateVector.data[PITCH]=-1.0;
											LVLHRateVector.data[YAW]=0.0;
											LVLHRateVector.data[ROLL]=0.0;
										}
										else {
											LVLHOrientationReqd.data[ROLL]=OM;
											LVLHRateVector.data[PITCH]=-1.0*cos(RAD*OM);
											LVLHRateVector.data[YAW]=-1.0*sin(RAD*OM);
											LVLHRateVector.data[ROLL]=0.0;
										}
									}
									else if(BODY_VECT==2) {
										LVLHOrientationReqd.data[PITCH]=90.0;
										LVLHOrientationReqd.data[YAW]=0.0;
										if(OM<=0.0) {
											LVLHOrientationReqd.data[ROLL]=0.0;
											LVLHRateVector.data[PITCH]=-1.0;
											LVLHRateVector.data[YAW]=0.0;
											LVLHRateVector.data[ROLL]=0.0;
										}
										else {
											LVLHOrientationReqd.data[ROLL]=OM;
											LVLHRateVector.data[PITCH]=-1.0*cos(RAD*OM);
											LVLHRateVector.data[YAW]=-1.0*sin(RAD*OM);
											LVLHRateVector.data[ROLL]=0.0;
										}
									}
									else if(BODY_VECT==3) {
										LVLHOrientationReqd.data[PITCH]=180.0;
										LVLHOrientationReqd.data[ROLL]=0.0;
										if(OM<=0.0) {
											LVLHOrientationReqd.data[YAW]=0.0;
											LVLHRateVector.data[PITCH]=-1.0;
											LVLHRateVector.data[YAW]=0.0;
											LVLHRateVector.data[ROLL]=0.0;
										}
										else {
											LVLHOrientationReqd.data[YAW]=OM;
											LVLHRateVector.data[PITCH]=-1.0*cos(RAD*OM);
											LVLHRateVector.data[ROLL]=1.0*sin(RAD*OM);
											LVLHRateVector.data[YAW]=0.0;
										}
									}
									else if(BODY_VECT==5) {
										LVLHOrientationReqd.data[PITCH]=270-P;
										if(LVLHOrientationReqd.data[PITCH]==0.0)
										{
											LVLHOrientationReqd.data[ROLL]=Y;
											if(OM<=0.0) {
												LVLHOrientationReqd.data[YAW]=0.0;
												if(LVLHOrientationReqd.data[ROLL]==0.0) {
													LVLHRateVector.data[PITCH]=-1.0;
													LVLHRateVector.data[YAW]=0.0;
													LVLHRateVector.data[ROLL]=0.0;
												}
											}
											else LVLHOrientationReqd.data[YAW]=OM;
										}
										else
										{
											LVLHOrientationReqd.data[YAW]=(270.0-P)*sin(RAD*Y);
											LVLHOrientationReqd.data[ROLL]=Y*cos(RAD*LVLHOrientationReqd.data[PITCH]);
											if(OM>0.0) LVLHOrientationReqd.data[YAW]+=OM;
										}
										LVLHRateVector.data[PITCH]=-1.0*cos(RAD*LVLHOrientationReqd.data[ROLL])*cos(RAD*LVLHOrientationReqd.data[YAW]);
										LVLHRateVector.data[ROLL]=1.0*sin(RAD*LVLHOrientationReqd.data[YAW]);
										LVLHRateVector.data[YAW]=-1.0*sin(RAD*LVLHOrientationReqd.data[ROLL])*cos(RAD*LVLHOrientationReqd.data[YAW]);
									}
								}
							}
							else TRK=false;
							Yaw=false;
							Pitch=false;
							Roll=false;
							return true;
						}
						else if(nNew==20) {
							if(ROT==false) {
								ROT=true;
								MNVR=false;
								TRK=false;
							}
							else ROT=false;
							return true;
						}
						else if(nNew==21) {
							MNVR=false;
							ROT=false;
							TRK=false;
							SetThrusterGroupLevel(THGROUP_ATT_PITCHUP, 0.0);
							SetThrusterGroupLevel(THGROUP_ATT_PITCHDOWN, 0.0);
							SetThrusterGroupLevel(THGROUP_ATT_YAWLEFT, 0.0);
							SetThrusterGroupLevel(THGROUP_ATT_YAWRIGHT, 0.0);
							SetThrusterGroupLevel(THGROUP_ATT_BANKLEFT, 0.0);
							SetThrusterGroupLevel(THGROUP_ATT_BANKRIGHT, 0.0);
							REQD_ATT.x=MNVR_OPTION.x;
							REQD_ATT.y=MNVR_OPTION.y;
							REQD_ATT.z=MNVR_OPTION.z;
							for(int i=0;i<3;i++) {
								if(REQD_ATT.data[i]>180.0) TargetAtt.data[i]=REQD_ATT.data[i]-360.0;
								else TargetAtt.data[i]=REQD_ATT.data[i];
							}
							TargetAtt=ToRad(TargetAtt);
							return true;
						}
						break;
					case 20:
						if(nNew==1 || nNew==2) {
							DAPMode[0]=nNew-1;
							UpdateDAP();
							return true;
						}
						else if(nNew>=3 && nNew<=5) {
							DAPMode[1]=nNew-3;
							UpdateDAP();
							return true;
						}
						else if(nNew==6 || nNew==7) {
							edit=nNew-5;
							int i=edit-1;
							DAP[2].PRI_ROT_RATE=DAP[i].PRI_ROT_RATE;
							DAP[2].PRI_ATT_DB=DAP[i].PRI_ATT_DB;
							DAP[2].PRI_RATE_DB=DAP[i].PRI_RATE_DB;
							DAP[2].PRI_ROT_PLS=DAP[i].PRI_ROT_PLS;
							DAP[2].PRI_COMP=DAP[i].PRI_COMP;
							DAP[2].PRI_TRAN_PLS=DAP[i].PRI_TRAN_PLS;
							DAP[2].PRI_P_OPTION=DAP[i].PRI_P_OPTION;
							DAP[2].PRI_Y_OPTION=DAP[i].PRI_Y_OPTION;
							DAP[2].ALT_RATE_DB=DAP[i].ALT_RATE_DB;
							DAP[2].ALT_ON_TIME=DAP[i].ALT_ON_TIME;
							DAP[2].ALT_DELAY=DAP[i].ALT_DELAY;
							DAP[2].ALT_JET_OPT=DAP[i].ALT_JET_OPT;
							DAP[2].ALT_JETS=DAP[i].ALT_JETS;
							DAP[2].VERN_ROT_RATE=DAP[i].VERN_ROT_RATE;
							DAP[2].VERN_ATT_DB=DAP[i].VERN_ATT_DB;
							DAP[2].VERN_RATE_DB=DAP[i].VERN_RATE_DB;
							DAP[2].VERN_ROT_PLS=DAP[i].VERN_ROT_PLS;
							DAP[2].VERN_COMP=DAP[i].VERN_COMP;
							DAP[2].VERN_CNTL_ACC=DAP[i].VERN_CNTL_ACC;
							return true;
						}
						else if(nNew==8) {
							int i=edit-1;
							DAP[i].PRI_ROT_RATE=DAP[2].PRI_ROT_RATE;
							DAP[i].PRI_ATT_DB=DAP[2].PRI_ATT_DB;
							DAP[i].PRI_RATE_DB=DAP[2].PRI_RATE_DB;
							DAP[i].PRI_ROT_PLS=DAP[2].PRI_ROT_PLS;
							DAP[i].PRI_COMP=DAP[2].PRI_COMP;
							DAP[i].PRI_TRAN_PLS=DAP[2].PRI_TRAN_PLS;
							DAP[i].PRI_P_OPTION=DAP[2].PRI_P_OPTION;
							DAP[i].PRI_Y_OPTION=DAP[2].PRI_Y_OPTION;
							DAP[i].ALT_RATE_DB=DAP[2].ALT_RATE_DB;
							DAP[i].ALT_ON_TIME=DAP[2].ALT_ON_TIME;
							DAP[i].ALT_DELAY=DAP[2].ALT_DELAY;
							DAP[i].ALT_JET_OPT=DAP[2].ALT_JET_OPT;
							DAP[i].ALT_JETS=DAP[2].ALT_JETS;
							DAP[i].VERN_ROT_RATE=DAP[2].VERN_ROT_RATE;
							DAP[i].VERN_ATT_DB=DAP[2].VERN_ATT_DB;
							DAP[i].VERN_RATE_DB=DAP[2].VERN_RATE_DB;
							DAP[i].VERN_ROT_PLS=DAP[2].VERN_ROT_PLS;
							DAP[i].VERN_COMP=DAP[2].VERN_COMP;
							DAP[i].VERN_CNTL_ACC=DAP[2].VERN_CNTL_ACC;
							edit=0;
							return true;
						}
						else if(nNew==15 || nNew==35 || nNew==55) {
							if(DAP[convert[nNew]].PRI_P_OPTION<2) {
								DAP[convert[nNew]].PRI_P_OPTION++;
								if(DAPMode[1]==0) {
									if(DAP[DAPMode[0]].PRI_P_OPTION==1) {
										DisableThrusters(AftPitchThrusters, 2);
										UpdateDAP();
									}
									else if(DAP[DAPMode[0]].PRI_P_OPTION==2) {
										EnableThrusters(AftPitchThrusters, 2);
										DisableThrusters(NosePitchThrusters, 2);
										UpdateDAP();
									}
								}
							}
							else {
								DAP[convert[nNew]].PRI_P_OPTION=0;
								if(DAP[DAPMode[0]].PRI_P_OPTION==0) {
									EnableThrusters(NosePitchThrusters, 2);
									UpdateDAP();
								}
							}
							return true;
						}
						else if(nNew==16 || nNew==36 || nNew==56) {
							if(DAP[convert[nNew]].PRI_Y_OPTION<2) {
								DAP[convert[nNew]].PRI_Y_OPTION++;
								if(DAPMode[1]==0) {
									if(DAP[DAPMode[0]].PRI_Y_OPTION==1) {
										DisableThrusters(AftYawThrusters, 2);
										UpdateDAP();
									}
									else if(DAP[DAPMode[0]].PRI_Y_OPTION==2) {
										EnableThrusters(AftYawThrusters, 2);
										DisableThrusters(NoseYawThrusters, 2);
										UpdateDAP();
									}
								}
							}
							else {
								DAP[convert[nNew]].PRI_Y_OPTION=0;
								if(DAP[DAPMode[0]].PRI_Y_OPTION==0) {
									EnableThrusters(NoseYawThrusters, 2);
									UpdateDAP();
								}
							}
							return true;
						}
						else if(nNew==19 || nNew==39 || nNew==59) {
							if(DAP[convert[nNew]].ALT_JET_OPT==0) {
								DAP[convert[nNew]].ALT_JET_OPT=2;
								if(DAPMode[1]==1) {
									if(DAP[DAPMode[0]].ALT_JET_OPT==2) {
										DisableThrusters(NoseRotThrusters, 6);
										EnableThrusters(AftRotThrusters, 6);
										UpdateDAP();
									}
								}
							}
							else {
								DAP[convert[nNew]].ALT_JET_OPT=0;
								if(DAPMode[1]==1) {
									if(DAP[DAPMode[0]].ALT_JET_OPT==0) {
										EnableThrusters(NoseRotThrusters, 6);
										UpdateDAP();
									}
								}
							}
							return true;
						}
						else if(nNew>=10 && nNew<=28 || nNew>=30 && nNew<=48 || nNew>=50 && nNew<=68) {
							item=nNew;
							return Input(mfd, 3, Name, Data);
							//data=1;
							//return true;
						}
						break;
				}
			}
			else if(ops==104 || ops==105 || ops==202 || ops==301 || ops==302) {
				if(nNew>=1 && nNew<=4) {
					OMS=nNew-1;
					return true;
				}
				else if(nNew>=5 && nNew<=21) {
					item=nNew;
					return Input(mfd, 3, Name, Data);
					//data=1;
					//return true;
				}
				else if(nNew==22) {
					LoadManeuver();
					return true;
				}
				else if(nNew==23) {
					Display[mfd]->bTIMER=true;
					return true;
				}
				else if(nNew==27) {
					if(!TRK) {
						TRK=true;
						MNVR=false;
						ROT=false;
						Yaw=false;
						Pitch=false;
						Roll=false;
						TGT_ID=2;
						if((Eq(BurnAtt.data[YAW], 90.00) || Eq(BurnAtt.data[YAW], -90.00)) && !Eq(BurnAtt.data[PITCH], 0.0)) {
							LVLHOrientationReqd.data[PITCH]=90.0;
							LVLHOrientationReqd.data[YAW]=BurnAtt.data[YAW]-BurnAtt.data[PITCH];
							LVLHOrientationReqd.data[ROLL]=BurnAtt.data[YAW]+BurnAtt.data[ROLL];
						}
						else {
							LVLHOrientationReqd.data[PITCH]=BurnAtt.data[PITCH];
							LVLHOrientationReqd.data[YAW]=BurnAtt.data[YAW];
							LVLHOrientationReqd.data[ROLL]=BurnAtt.data[ROLL];
						}
						LVLHRateVector.data[PITCH]=-1.0*cos(RAD*LVLHOrientationReqd.data[ROLL])*cos(RAD*LVLHOrientationReqd.data[YAW]);
						LVLHRateVector.data[ROLL]=1.0*sin(RAD*LVLHOrientationReqd.data[YAW]);
						LVLHRateVector.data[YAW]=-1.0*sin(RAD*LVLHOrientationReqd.data[ROLL])*cos(RAD*LVLHOrientationReqd.data[YAW]);
						AttDeadband=0.05;
						for(int i=0;i<4;i++) START_TIME[i]=MET[i];
					}
					else {
						MNVR=false;
						ROT=false;
						TRK=false;
						SetThrusterGroupLevel(THGROUP_ATT_PITCHUP, 0.0);
						SetThrusterGroupLevel(THGROUP_ATT_PITCHDOWN, 0.0);
						SetThrusterGroupLevel(THGROUP_ATT_YAWLEFT, 0.0);
						SetThrusterGroupLevel(THGROUP_ATT_YAWRIGHT, 0.0);
						SetThrusterGroupLevel(THGROUP_ATT_BANKLEFT, 0.0);
						SetThrusterGroupLevel(THGROUP_ATT_BANKRIGHT, 0.0);
					}
					return true;
				}
				return false;
			}
		}
		else if(change==2) {
			nNew=atoi(Name);
			if(ops==201) {
				if(nNew==20 || nNew==0) {
					Display[mfd]->spec=nNew;
					//InvalidateDisplay();
					return true;
				}
				else return false;
			}
		}
		else if(change==3) {
			//item=atoi(Name);
			if(ops==201) {
				switch(Display[mfd]->spec) {
					case 0:
						if(item>=1 && item<=4) {
							nNew=atoi(Data);
							if((item==1 && nNew<365) || (item==2 && nNew<24) || (item>2 && nNew<60)) {
								START_TIME[item-1]=nNew;
								return true;
							}
							return false;
						}
						else if(item==5 || item==6) {
							dNew=atof(Data);
							if(dNew>=0.0 && dNew<360.0) {
								if(item==5) MNVR_OPTION.data[ROLL]=dNew;
								else MNVR_OPTION.data[PITCH]=dNew;
								return true;
							}
							return false;
						}
						else if(item==7) {
							dNew=atof(Data);
							if((dNew>=0.0 && dNew<=90.0) || (dNew>=270.0 && dNew<360.0)) {
								MNVR_OPTION.data[YAW]=dNew;
								return true;
							}
							return false;
						}
						if(item==8) {
							nNew=atoi(Data);
							if(nNew==2 || nNew==5) {
								TGT_ID=nNew;
								return true;
							}
							return false;
						}
						if(item==14) {
							nNew=atoi(Data);
							if(nNew>=1 && nNew<=5) {
								BODY_VECT=nNew;
								if(BODY_VECT==1) {
									P=0.0;
									Y=0.0;
								}
								else if(BODY_VECT==2) {
									P=180.0;
									Y=0.0;
								}
								else if(BODY_VECT==3) {
									P=90.0;
									Y=0.0;
								}
								return true;
							}
							return false;
						}
						if(item==15 && BODY_VECT==5) {
							dNew=atof(Data);
							if(dNew>=0.0 && dNew<360.0) {
								P=dNew;
								return true;
							}
							return false;
						}
						if(item==16 && BODY_VECT==5) {
							dNew=atof(Data);
							if((dNew>=0.0 && dNew<=90.0) || (dNew>=270.0 && dNew<360.0)) {
								Y=dNew;
								return true;
							}
							return false;
						}
						if(item==17) {
							dNew=atof(Data);
							if(dNew>=0.0 && dNew<360.0) {
								OM=dNew;
								return true;
							}
							return false;
						}
						return false;
						break;
					case 20:
						if(item==10 || item==30 || item==50) {
							dNew=atof(Data);
							if(dNew>0.0 && dNew<10.0) {
								DAP[convert[item]].PRI_ROT_RATE=dNew;
								if(convert[item]==DAPMode[0] && DAPMode[1]==0) UpdateDAP();
								return true;
							}
							return false;
						}
						else if(item==11 || item==31 || item==51) {
							dNew=atof(Data);
							if(dNew>0.0 && dNew<100.0) {
								DAP[convert[item]].PRI_ATT_DB=dNew;
								if(convert[item]==DAPMode[0] && DAPMode[1]==0) UpdateDAP();
								return true;
							}
							return false;
						}
						else if(item==12 || item==32 || item==52) {
							dNew=atof(Data);
							if(dNew>0.0 && dNew<10.0) {
								DAP[convert[item]].PRI_RATE_DB=dNew;
								if(convert[item]==DAPMode[0] && DAPMode[1]==0) UpdateDAP();
								return true;
							}
							return false;
						}
						else if(item==13 || item==33 || item==53) {
							dNew=atof(Data);
							if(dNew>0.0 && dNew<10.0) {
								DAP[convert[item]].PRI_ROT_PLS=dNew;
								if(convert[item]==DAPMode[0] && DAPMode[1]==0) UpdateDAP();
								return true;
							}
							return false;
						}
						else if(item==14 || item==34 || item==54) {
							dNew=atof(Data);
							if(dNew>0.0 && dNew<1.0) {
								DAP[convert[item]].PRI_COMP=dNew;
								if(convert[item]==DAPMode[0] && DAPMode[1]==0) UpdateDAP();
								return true;
							}
							return false;
						}
						else if(item==17 || item==37 || item==57) {
							dNew=atof(Data);
							if(dNew>0.0 && dNew<10.0) {
								DAP[convert[item]].PRI_ROT_PLS=dNew;
								if(convert[item]==DAPMode[0] && DAPMode[1]==0) UpdateDAP();
								return true;
							}
							return false;
						}
						else if(item==18 || item==38 || item==58) {
							dNew=atof(Data);
							if(dNew>0.0 && dNew<10.0) {
								DAP[convert[item]].ALT_RATE_DB=dNew;
								if(convert[item]==DAPMode[0] && DAPMode[1]==1) UpdateDAP();
								return true;
							}
							return false;
						}
						else if(item==20 || item==40 || item==60) {
							nNew=atoi(Data);
							if(nNew>=1 && nNew<=3) {
								DAP[convert[item]].ALT_JETS=nNew;
								if(convert[item]==DAPMode[0] && DAPMode[1]==1) UpdateDAP();
								return true;
							}
							return false;
						}
						else if(item==21 || item==41 || item==61) {
							dNew=atof(Data);
							if(dNew>0.0 && dNew<10.0) {
								DAP[convert[item]].ALT_ON_TIME=dNew;
								if(convert[item]==DAPMode[0] && DAPMode[1]==1) UpdateDAP();
								return true;
							}
							return false;
						}
						else if(item==22 || item==42 || item==62) {
							dNew=atof(Data);
							if(dNew>0.0 && dNew<100.0) {
								DAP[convert[item]].ALT_DELAY=dNew;
								if(convert[item]==DAPMode[0] && DAPMode[1]==1) UpdateDAP();
								return true;
							}
							return false;
						}
						else if(item==23 || item==43 || item==63) {
							dNew=atof(Data);
							if(dNew>0.0 && dNew<10.0) {
								DAP[convert[item]].VERN_ROT_RATE=dNew;
								if(convert[item]==DAPMode[0] && DAPMode[1]==2) UpdateDAP();
								return true;
							}
							return false;
						}
						else if(item==24 || item==44 || item==64) {
							dNew=atof(Data);
							if(dNew>0.0 && dNew<100.0) {
								DAP[convert[item]].VERN_ATT_DB=dNew;
								if(convert[item]==DAPMode[0] && DAPMode[1]==2) UpdateDAP();
								return true;
							}
							return false;
						}
						else if(item==25 || item==45 || item==65) {
							dNew=atof(Data);
							if(dNew>0.0 && dNew<1.0) {
								DAP[convert[item]].VERN_RATE_DB=dNew;
								if(convert[item]==DAPMode[0] && DAPMode[1]==2) UpdateDAP();
								return true;
							}
							return false;
						}
						else if(item==26 || item==46 || item==66) {
							dNew=atof(Data);
							if(dNew>0.0 && dNew<10.0) {
								DAP[convert[item]].VERN_ROT_PLS=dNew;
								if(convert[item]==DAPMode[0] && DAPMode[1]==2) UpdateDAP();
								return true;
							}
							return false;
						}
						else if(item==27 || item==47 || item==67) {
							dNew=atof(Data);
							if(dNew>0.0 && dNew<10.0) {
								DAP[convert[item]].VERN_COMP=dNew;
								if(convert[item]==DAPMode[0] && DAPMode[1]==2) UpdateDAP();
								return true;
							}
							return false;
						}
						else if(item==28 || item==48 || item==68) {
							nNew=atoi(Data);
							if(nNew>=0 && nNew<=9) {
								DAP[convert[item]].VERN_CNTL_ACC=nNew;
								if(convert[item]==DAPMode[0] && DAPMode[1]==2) UpdateDAP();
								return true;
							}
							return false;
						}
						return false;
						break;
				}
			}
			else if(ops==104 || ops==105 || ops==202 || ops==301 || ops==302) {
				if(item==5) {
					TV_ROLL=atoi(Data);
					return true;
				}
				else if(item>=6 && item<=8) {
					dNew=atof(Data);
					if(fabs(dNew)<=6.0) 
					{
						Trim.data[item-6]=dNew;
						return true;
					}
				}
				else if(item==9) {
					//dNew=atof(Data);
					WT=atof(Data);
					return true;
				}
				else if(item>=10 && item<=13) {
					if(item==13) dNew=atof(Data);
					else dNew=atoi(Data);
					//sprintf(oapiDebugString(), "%f", dNew);
					if((item==10 && dNew<365.0) || (item==11 && dNew<24.0) || (item>11 && dNew<60.0)) {
						TIG[item-10]=dNew;
						return true;
					}
					return false;
				}
				else if(item==14 && ops!=202) {
					C1=atof(Data);
					return true;
				}
				else if(item==15 && ops!=202) {
					dNew=atof(Data);
					if(fabs(dNew)<10.0) {
						C2=dNew;
						return true;
					}
				}
				else if(item==16 && ops!=202) {
					HT=atof(Data);
					return true;
				}
				else if(item==17 && ops!=202) {
					ThetaT=atof(Data);
					return true;
				}
				else if(item>=19 && item<=21) {
					dNew=atof(Data);
					PEG7.data[item-19]=dNew;
					return true;
				}
				return false;
			}
			item=0;
			return true;
		}
	}

	else if(panelc2->switch_state[SWITCH2+2*id]==1) //SM
	{
		sprintf(oapiDebugString(), "SM Item Entry");
		if(change==1) {
			nNew=atoi(Name);
			if(SMOps==201) {
				if(nNew==1) {
					Display[mfd]->RMS_SEL=0;
					return true;
				}
				else if(nNew==2) {
					Display[mfd]->RMS_SEL=1;
					return true;
				}
			}
		}
	}
	return false;
}

void Atlantis::RedrawPanel_MFDButton (SURFHANDLE surf, int mfd)
{
  HDC hDC = oapiGetDC (surf);

  // D. Beachy: BUGFIX: if MFD powered off, cover separator lines and do not paint buttons
    if (oapiGetMFDMode(mfd) == MFD_NONE) {
        RECT r = { 0,0,255,13 };
        FillRect(hDC, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));
    } else {   // MFD powered on
    HFONT pFont = (HFONT)SelectObject (hDC, g_Param.font[0]);
    SetTextColor (hDC, RGB(0,255,216));
    SetTextAlign (hDC, TA_CENTER);
    SetBkMode (hDC, TRANSPARENT);
    const char *label;
    int x = 24;

    for (int bt = 0; bt < 5; bt++) {
      if (label = oapiMFDButtonLabel (mfd, bt)) {
        TextOut (hDC, x, 1, label, strlen(label));
        x += 42;
      } else break;
    }
    TextOut (hDC, 234, 1, "PG", 2);
    SelectObject (hDC, pFont);
  }

  oapiReleaseDC (surf, hDC);
}

void Atlantis::SetILoads()
{
	stage1guidance[0]=new double[8];
	stage1guidance[1]=new double[8];
	for(int i=0;i<8;i++) {
		stage1guidance[0][i]=defaultStage1Guidance[0][i];
		stage1guidance[1][i]=defaultStage1Guidance[1][i];
	}
	stage1guidance_size=8;
	return;
}

void Atlantis::CalcLVLHAttitude(VECTOR3 &Output)
{
	VECTOR3 H;
	H = crossp(Status.rpos, Status.rvel);
	TargetAtt=GetPYR2(Status.rvel, H);
	//Output=CalcPitchYawRollAngles(ToRad(LVLHOrientationReqd));
	Output=CalcPitchYawRollAngles(_V(0.0, 0.0, 0.0));
	return;
}

VECTOR3 Atlantis::CalcRelLVLHAttitude(VECTOR3 &Target)
{
	RefPoints GlobalPts, LocalPts;
	VECTOR3 PitchUnit = {0, 0, 1.0}, YawRollUnit = {1.0, 0, 0};
	VECTOR3 H = crossp(Status.rpos, Status.rvel);
	VECTOR3 RefAttitude = GetPYR2(Status.rvel, H);

	RotateVector(PitchUnit, Target, PitchUnit);
	RotateVector(YawRollUnit, Target, YawRollUnit);
	RotateVector(PitchUnit, RefAttitude, GlobalPts.Pitch);
	RotateVector(YawRollUnit, RefAttitude, GlobalPts.Yaw);
	GlobalPts.Pitch = GVesselPos + GlobalPts.Pitch;
	GlobalPts.Yaw = GVesselPos + GlobalPts.Yaw;	
	Global2Local(GlobalPts.Pitch, LocalPts.Pitch);
	Global2Local(GlobalPts.Yaw, LocalPts.Yaw);
	return GetPYR(LocalPts.Pitch, LocalPts.Yaw);
}

VECTOR3 Atlantis::CalcPitchYawRollAngles(VECTOR3 &RelAttitude)
{
	RefPoints GlobalPts, LocalPts;
	VECTOR3 PitchUnit = {0, 0, 1.0}, YawRollUnit = {1.0, 0, 0};
	RotateVector(PitchUnit, RelAttitude, PitchUnit);
	RotateVector(YawRollUnit, RelAttitude, YawRollUnit);
	RotateVector(PitchUnit, TargetAtt, GlobalPts.Pitch);
	RotateVector(YawRollUnit, TargetAtt, GlobalPts.Yaw);
	GlobalPts.Pitch = GVesselPos + GlobalPts.Pitch;
	GlobalPts.Yaw = GVesselPos + GlobalPts.Yaw;	
	Global2Local(GlobalPts.Pitch, LocalPts.Pitch);
	Global2Local(GlobalPts.Yaw, LocalPts.Yaw);
	return GetPYR(LocalPts.Pitch, LocalPts.Yaw);
}

//Math
double Atlantis::NullStartAngle(double Rates, AXIS Axis)
{
	double Time, Angle;
	//TorqueReq = -(Mass * PMI.data[Axis] * Rate) / TimeStep;
	if(Rates!=0.0) {
		Time = (Mass*PMI.data[Axis]*Rates)/Torque.data[Axis];
		//Acceleration=Rates/Time;
		//Angle=Acceleration*Time*Time;
		Angle=0.5*Rates*Time;
		//if(Axis==YAW) sprintf(oapiDebugString(), "%f %f %f", Rates, Angle, PMI.data[Axis]);
		return DEG*Angle;
	}
	else return 0.0;
}

VECTOR3 Atlantis::GetPYR(VECTOR3 Pitch, VECTOR3 YawRoll)
{	
	VECTOR3 Res = { 0, 0, 0 };

	// Normalize the vectors
	Pitch = Normalize(Pitch);
	YawRoll = Normalize(YawRoll);
	VECTOR3 H = Normalize(crossp(Pitch, YawRoll));

	Res.data[YAW] = -asin(YawRoll.z);

	Res.data[ROLL] = atan2(YawRoll.y, YawRoll.x);

	Res.data[PITCH] = atan2(H.z, Pitch.z);

	return Res;

}

VECTOR3 Atlantis::GetPYR2(VECTOR3 Pitch, VECTOR3 YawRoll)
{	
	VECTOR3 Res = { 0, 0, 0 };
	// Normalize the vectors
	Pitch = Normalize(Pitch);
	YawRoll = Normalize(YawRoll);
	VECTOR3 H = Normalize(crossp(Pitch, YawRoll));
	Res.data[YAW] = -asin(Pitch.x);
	Res.data[ROLL] = atan2(H.x, YawRoll.x);
	Res.data[PITCH] = atan2(Pitch.y, Pitch.z);
	return Res;
}

void Atlantis::RotateVector(const VECTOR3 &Initial, const VECTOR3 &Angles, VECTOR3 &Result)
{
	MATRIX3 RotMatrixX, RotMatrixY, RotMatrixZ;
	VECTOR3 AfterZ, AfterZY;					// Temporary variables


	GetRotMatrixX(Angles.x, RotMatrixX);
	GetRotMatrixY(Angles.y, RotMatrixY);
	GetRotMatrixZ(Angles.z, RotMatrixZ);
	
	MultiplyByMatrix(Initial, RotMatrixZ, AfterZ);
	MultiplyByMatrix(AfterZ, RotMatrixY, AfterZY);
	MultiplyByMatrix(AfterZY, RotMatrixX, Result);
}

// Returns the rotation matrix for a rotation of a given angle around the X axis (Pitch)
void Atlantis::GetRotMatrixX(double Angle, MATRIX3 &RotMatrixX)
{
	RotMatrixX.m11 = 1;
	RotMatrixX.m12 = 0;
	RotMatrixX.m13 = 0;
	RotMatrixX.m21 = 0;
	RotMatrixX.m22 = cos(Angle);
	RotMatrixX.m23 = sin(Angle);
	RotMatrixX.m31 = 0;
	RotMatrixX.m32 = -sin(Angle);
	RotMatrixX.m33 = cos(Angle);
}

// Returns the rotation matrix for a rotation of a given angle around the Y axis (Yaw)
void Atlantis::GetRotMatrixY(double Angle, MATRIX3 &RotMatrixY)
{
	RotMatrixY.m11 = cos(Angle);
	RotMatrixY.m12 = 0;
	RotMatrixY.m13 = -sin(Angle);
	RotMatrixY.m21 = 0;
	RotMatrixY.m22 = 1;
	RotMatrixY.m23 = 0;
	RotMatrixY.m31 = sin(Angle);
	RotMatrixY.m32 = 0;
	RotMatrixY.m33 = cos(Angle);
}

// Returns the rotation matrix for a rotation of a given angle around the Z axis (Roll)
void Atlantis::GetRotMatrixZ(double Angle, MATRIX3 &RotMatrixZ)
{
	RotMatrixZ.m11 = cos(Angle);
	RotMatrixZ.m12 = sin(Angle);
	RotMatrixZ.m13 = 0;
	RotMatrixZ.m21 = -sin(Angle);
	RotMatrixZ.m22 = cos(Angle);
	RotMatrixZ.m23 = 0;
	RotMatrixZ.m31 = 0;
	RotMatrixZ.m32 = 0;
	RotMatrixZ.m33 = 1;
}

void Atlantis::MultiplyByMatrix(const VECTOR3 &Initial, const MATRIX3 &RotMatrix, VECTOR3 &Result)
{

	Result.x =	(Initial.x * RotMatrix.m11) 
				+ (Initial.y * RotMatrix.m12) 
				+ (Initial.z * RotMatrix.m13);	
	Result.y =	(Initial.x * RotMatrix.m21) 
				+ (Initial.y * RotMatrix.m22) 
				+ (Initial.z * RotMatrix.m23);	
	Result.z =	(Initial.x * RotMatrix.m31) 
				+ (Initial.y * RotMatrix.m32) 
				+ (Initial.z * RotMatrix.m33);
}

// ==============================================================
// Overloaded callback functions
// ==============================================================

// --------------------------------------------------------------
// Set vessel class capabilities from config file
// --------------------------------------------------------------
void Atlantis::clbkSetClassCaps (FILEHANDLE cfg)
{
  if (!oapiReadItem_bool (cfg, "RenderCockpit", render_cockpit))
    render_cockpit = false;

  if (!oapiReadItem_bool (cfg, "UseRealRCS", bUseRealRCS))
    bUseRealRCS = false;


  psubsystems->SetClassCaps(cfg);
}

// --------------------------------------------------------------
// Set status from a VESSELSTATUS2 structure
// --------------------------------------------------------------
void Atlantis::clbkSetStateEx (const void *status)
{
  // default parameter initialisation
  DefSetStateEx (status);

  // reset vessel-specific parameters to defaults
  //status = 3;
  SetOrbiterConfiguration();
}

// --------------------------------------------------------------
// Read status from scenario file
// --------------------------------------------------------------
void Atlantis::clbkLoadStateEx (FILEHANDLE scn, void *vs)
{
  int action;
  char *line;
  double srbtime = 0.0;
  double sts_sat_x = 0.0;
  double sts_sat_y = 0.0;
  double sts_sat_z = 0.0;
  spdb_status = AnimState::CLOSED; spdb_proc = 0.0;

  while (oapiReadScenario_nextline (scn, line)) {
        if (!strnicmp (line, "CONFIGURATION", 13)) {
            sscanf (line+13, "%d", &status);
    } else if (!strnicmp (line, "MET", 3)) {
		sscanf (line+3, "%lf", &met);
    } else if (!strnicmp (line, "SPEEDBRAKE", 10)) {
		sscanf (line+10, "%d%lf", &action, &spdb_proc);
		spdb_status = (AnimState::Action)(action+1);
    } else if (!strnicmp (line, "WING_NAME", 9)) {
      strncpy(WingName,line+10,256);
    } else if (!strnicmp (line, "SRB_IGNITION_TIME", 17)) {
		sscanf (line+17, "%lf", &srbtime);
    } else if (!strnicmp (line, "SAT_OFS_X", 9)) {
		sscanf (line+9, "%lf", &sts_sat_x);
    } else if (!strnicmp (line, "SAT_OFS_Y", 9)) {
		sscanf (line+9, "%lf", &sts_sat_y);
    } else if (!strnicmp (line, "SAT_OFS_Z", 9)) {
		sscanf (line+9, "%lf", &sts_sat_z);
	} else if (!strnicmp (line, "PAYLOAD_MASS", 12)) {
		sscanf (line+12, "%lf", &pl_mass);
	} else if (!strnicmp (line, "CARGO_STATIC_MESH", 17)) {
		sscanf (line+17, "%s", cargo_static_mesh_name);
		do_cargostatic = true;
    } else if (!strnicmp (line, "CARGO_STATIC_OFS", 16)) {
		sscanf (line+16, "%lf%lf%lf", &cargo_static_ofs.x, &cargo_static_ofs.y, &cargo_static_ofs.z);
	} else if (!strnicmp (line, "RMS", 3)) {
		RMS=true;
	} else if (!strnicmp(line, "ROLLOUT", 7)) {
		sscanf(line+7, "%d%lf", &action, &RMSRollout.pos);
		SetAnimation(anim_rollout, RMSRollout.pos);
		if(action==1) {
			if(RMSRollout.pos!=1.0) RMSRollout.action=AnimState::OPENING;
			else RMSRollout.action=AnimState::OPEN;
		}
		else {
			if(RMSRollout.pos!=0.0) RMSRollout.action=AnimState::CLOSING;
			else RMSRollout.action=AnimState::CLOSED;
		}
		UpdateMPMMicroswitches();
	} else if (!strnicmp (line, "SHOULDER_BRACE", 14)) {
		sscanf (line+14, "%lf", &shoulder_brace);
	} else if (!strnicmp (line, "MRL", 3)) {
		sscanf (line+3, "%lf%lf", &MRL[0], &MRL[1]);
		UpdateMRLMicroswitches();
	} else if (!strnicmp (line, "ARM_STATUS", 10)) {
		sscanf (line+10, "%lf%lf%lf%lf%lf%lf", &arm_sy, &arm_sp, &arm_ep, &arm_wp, &arm_wy, &arm_wr);
	} else if(!strnicmp(line, "OPS", 3)) {
		sscanf(line+3, "%d", &ops);
	} else if(!strnicmp(line, "PEG7", 4)) {
		sscanf(line+4, "%lf%lf%lf", &PEG7.x, &PEG7.y, &PEG7.z);
	} else if(!strnicmp(line, "WT", 2)) {
		sscanf(line+2, "%lf", &WT);
	} else if(!strnicmp(line, "TIG", 3)) {
		sscanf(line+3, "%lf%lf%lf%lf", &TIG[0], &TIG[1], &TIG[2], &TIG[3]);
	} else if(!strnicmp(line, "ASSIST", 6)) {
		sscanf(line+6, "%lf%lf", &OMS_Assist[0], &OMS_Assist[1]);
	} else if(!strnicmp(line, "THROTTLE_BUCKET", 15)) {
		sscanf(line+15, "%lf%lf", &Throttle_Bucket[0], &Throttle_Bucket[1]);
		Throttle_Bucket[0]=Throttle_Bucket[0]*fps_to_ms;
		Throttle_Bucket[1]=Throttle_Bucket[1]*fps_to_ms;
	} else if(!strnicmp(line, "HEADS_UP", 8)) {
		sscanf(line+8, "%lf", &RollToHeadsUp);
		RollToHeadsUp=RollToHeadsUp*fps_to_ms;
	} else if(!strnicmp(line, "AUTOPILOT", 9)) {
		sscanf(line+9, "%lf%lf%lf%lf%lf", &TgtInc, &TgtLAN, &TgtAlt, &TgtSpd, &TgtFPA);
		bAutopilot=true;
	} else if(!strnicmp(line, "ENGINE FAIL", 11)) {
		sscanf(line+11, "%d%lf", &EngineFail, &EngineFailTime);
		bEngineFail=true;
	} else if(!strnicmp(line, "TGT_ID", 6)) {
		sscanf(line+6, "%d", &TGT_ID);
	} else if(!strnicmp(line, "BODY_VECT", 9)) {
		sscanf(line+9, "%d", &BODY_VECT);
	} else if(!strnicmp(line, "ROLL", 4)) {
		sscanf(line+4, "%lf", &MNVR_OPTION.data[ROLL]);
	} else if(!strnicmp(line, "PITCH", 5)) {
		sscanf(line+5, "%lf", &MNVR_OPTION.data[PITCH]);
	} else if(!strnicmp(line, "YAW", 3)) {
		sscanf(line+3, "%lf", &MNVR_OPTION.data[YAW]);
	} else {
      if (plop->ParseScenarioLine (line)) continue; // offer the line to bay door operations
      if (gop->ParseScenarioLine (line)) continue; // offer the line to gear operations
	  if (c3po->ParseScenarioLine (line)) continue; // offer line to c3po
	  if (r2d2->ParseScenarioLine (line)) continue; // offer line to r2d2
	  if (panela4->ParseScenarioLine (line)) continue; // offer line to panel A4
	  if (panela8->ParseScenarioLine (line)) continue;
	  if (panelc2->ParseScenarioLine (line)) continue; // offer line to panel C2
	  if (panelf7->ParseScenarioLine (line)) continue; // offer line to panel F7
	  if (psubsystems->ParseScenarioLine(line)) continue; // offer line to subsystem simulation
      ParseScenarioLineEx (line, vs);
      // unrecognised option - pass to Orbiter's generic parser
    }
  }
  ofs_sts_sat.x=sts_sat_x;
  ofs_sts_sat.y=sts_sat_y;
  ofs_sts_sat.z=sts_sat_z;

  ClearMeshes();
  switch (status) {
  case 0:
    SetLaunchConfiguration();
    break;
  case 1:
    SetPostLaunchConfiguration (met);
    break;
  case 2:
    SetOrbiterTankConfiguration();
    break;
  case 3:
    SetOrbiterConfiguration();
    break;
  }

  UpdateMesh ();
  SetILoads();
}

// --------------------------------------------------------------
// Write status to scenario file
// --------------------------------------------------------------
void Atlantis::clbkSaveState (FILEHANDLE scn)
{
  char cbuf[256];

  // default vessel parameters
  VESSEL2::clbkSaveState (scn);

  // custom parameters
  oapiWriteScenario_int (scn, "CONFIGURATION", status);

  if (status == 1)
    oapiWriteScenario_float (scn, "MET", oapiGetSimTime()-t0);
  else oapiWriteScenario_float (scn, "MET", met);

  if (spdb_status != AnimState::CLOSED) {
    sprintf (cbuf, "%d %0.4f", spdb_status-1, spdb_proc);
    oapiWriteScenario_string (scn, "SPEEDBRAKE", cbuf);
  }
  oapiWriteScenario_string (scn, "WING_NAME", WingName);

  if(RMS) {
	  oapiWriteLine(scn, "  RMS");
	  sprintf (cbuf, "%0.6f %0.6f %0.6f %0.6f %0.6f %0.6f", arm_sy, arm_sp, arm_ep, arm_wp, arm_wy, arm_wr);
	  oapiWriteScenario_string (scn, "ARM_STATUS", cbuf);
	  if(RMSRollout.action==AnimState::OPEN || RMSRollout.action==AnimState::OPENING)
		  sprintf(cbuf, "1 %f", RMSRollout.pos);
	  else sprintf(cbuf, "0 %f", RMSRollout.pos);
	  oapiWriteScenario_string(scn, "ROLLOUT", cbuf);
	  oapiWriteScenario_float(scn, "SHOULDER_BRACE", shoulder_brace);
	  sprintf(cbuf, "%f %f", MRL[0], MRL[1]);
	  oapiWriteScenario_string(scn, "MRL", cbuf);
  }

  oapiWriteScenario_float (scn, "SAT_OFS_X", ofs_sts_sat.x);
  oapiWriteScenario_float (scn, "SAT_OFS_Y", ofs_sts_sat.y);
  oapiWriteScenario_float (scn, "SAT_OFS_Z", ofs_sts_sat.z);

  if (do_cargostatic) {
    oapiWriteScenario_string (scn, "CARGO_STATIC_MESH", cargo_static_mesh_name);
    if (cargo_static_ofs.x || cargo_static_ofs.y || cargo_static_ofs.z)
      oapiWriteScenario_vec (scn, "CARGO_STATIC_OFS", cargo_static_ofs);
  } 
  if(pl_mass!=0.0) oapiWriteScenario_float(scn, "PAYLOAD_MASS", pl_mass);

  //GPC
  oapiWriteScenario_int (scn, "OPS", ops);
  if(bAutopilot) {
	  sprintf(cbuf, "%f %f% f% f% f", TgtInc, TgtLAN, TgtAlt, TgtSpd, TgtFPA);
	  oapiWriteScenario_string(scn, "AUTOPILOT", cbuf);
  }
  if(status<3) {
	  sprintf(cbuf, "%f %f", OMS_Assist[0], OMS_Assist[1]);
	  oapiWriteScenario_string(scn, "ASSIST", cbuf);
	  sprintf(cbuf, "%f %f", Throttle_Bucket[0]/fps_to_ms, Throttle_Bucket[1]/fps_to_ms);
	  oapiWriteScenario_string(scn, "THROTTLE_BUCKET", cbuf);
	  oapiWriteScenario_float(scn, "HEADS_UP", RollToHeadsUp/fps_to_ms);
  }
  //MNVR
  if(ops==302 || ops==301 || ops==202 || ops==104 || ops==105) {
	  oapiWriteScenario_vec(scn, "PEG7", PEG7);
	  oapiWriteScenario_float(scn, "WT", WT);
	  sprintf(cbuf, "%0.0f %0.0f %0.0f %0.1f", TIG[0], TIG[1], TIG[2], TIG[3]);
	  oapiWriteScenario_string(scn, "TIG", cbuf);
  }
  //DAP
  oapiWriteScenario_int (scn, "TGT_ID", TGT_ID);
  oapiWriteScenario_int (scn, "BODY_VECT", BODY_VECT);
  oapiWriteScenario_float (scn, "ROLL", MNVR_OPTION.data[ROLL]);
  oapiWriteScenario_float (scn, "PITCH", MNVR_OPTION.data[PITCH]);
  oapiWriteScenario_float (scn, "YAW", MNVR_OPTION.data[YAW]);
  oapiWriteScenario_float (scn, "P", P);
  oapiWriteScenario_float (scn, "Y", Y);
  oapiWriteScenario_float (scn, "OM", OM);

  // save bay door operations status
  plop->SaveState (scn);
  gop->SaveState (scn);
  panela4->SaveState(scn);
  panela8->SaveState(scn);
  panelc2->SaveState(scn);
  c3po->SaveState (scn);
  panelf7->SaveState(scn);
  r2d2->SaveState (scn);

  psubsystems->SaveState(scn);
  
}

// --------------------------------------------------------------
// Vessel gains or loses input focus
// --------------------------------------------------------------
void Atlantis::clbkFocusChanged (bool getfocus, OBJHANDLE newv, OBJHANDLE oldv)
{
  if (getfocus) {
    oapiDisableMFDMode (MFD_LANDING);
    // no VTOL MFD mode for Atlantis
	//Enable communication overlays
  } else 
  {
	  //Disable all Communication overlays
  }
}

// --------------------------------------------------------------
// Simulation time step
// --------------------------------------------------------------

void Atlantis::clbkPreStep (double simT, double simDT, double mjd)
{
	double dThrust;
	double steerforce, airspeed;
	int i;

	psubsystems->PreStep(simT, simDT, mjd);
	/*if(bFirstStep)
	{
		if(bAutopilot) InitializeAutopilot();
		bFirstStep=false;
	}*/
	//throttle limits
	for(i=0;i<2;i++)
	{
		if(c3po->OMS_Eng[i]<2) {
			if(GetThrusterLevel(th_oms[i])>0.0 && GetThrusterLevel(th_oms[i])<1) {
				SetThrusterLevel(th_oms[i],1.0);
				dThrust=GetPropellantMass(oms_helium_tank[i])-0.01*simDT;
				if(dThrust<=0.0) {
					SetThrusterLevel(th_oms[i],0.0);
					dThrust=0.0;
				}
				SetPropellantMass(oms_helium_tank[i],dThrust);
			}
			else if(GetThrusterLevel(th_oms[i])==1.0) {
				dThrust=GetPropellantMass(oms_helium_tank[i])-0.01*simDT;
				if(dThrust<=0.0) {
					SetThrusterLevel(th_oms[i],0.0);
					dThrust=0.0;
				}
				SetPropellantMass(oms_helium_tank[i],dThrust);
			 }
		}
	}
	if(status==3) {
		//Nosewheel steering
		if(GroundContact()) {
			airspeed=GetAirspeed();
			if(airspeed<25.0 && airspeed>1.0)
			{
				steerforce = (25-airspeed);
				if(airspeed<6.0) steerforce*=(airspeed/6);
				steerforce = 250000*steerforce*GetControlSurfaceLevel(AIRCTRL_RUDDER);
				AddForce (_V(steerforce, 0, 0), _V(0, 0, 12.0));
				AddForce (_V(-steerforce, 0, 0), _V(0, 0, -12.0));
			}
		}
	}
}

void Atlantis::clbkPostStep (double simt, double simdt, double mjd)
{
  //double met;
  double airspeed;
  int i;
  OBJHANDLE hvessel;

  psubsystems->PostStep(simt, simdt, mjd);

  switch (status) {
  case 0: // launch configuration
    if (GetEngineLevel (ENGINE_MAIN) > 0.95) 
	{
      status = 1; // launch
	  SignalGSEStart();
      t0 = simt + SRB_STABILISATION_TIME;   // store designated liftoff time
      RecordEvent ("STATUS", "SSME_IGNITION");
	  if(bAutopilot) 
		  InitializeAutopilot(); //setup autopilot for ascent
	} else {
		//AutoMainGimbal();
		RateCommand();
	}
    break;
  case 1: // SRB's ignited
    met = simt-t0;
	if(met >= 0.0 && !GetLiftOffFlag())
	{
		SignalGSEBreakHDP();
		TriggerLiftOff();	
	}
    //sprintf(oapiDebugString(),"met: %f",met);
    if (met > SRB_SEPARATION_TIME && !Playback() || bManualSeparate) { // separate boosters
      SeparateBoosters (met);
	  tSRBSep=met;
      bManualSeparate = false;
	  ops=103;		//Replace by signal to GPC
	  CalcThrustAngles();
    }
	else {
	  if(GetPropellantMass(ph_srb) == 0.0 && !bSRBCutoffFlag)
	  {
		  char buffer[100];
		  sprintf(buffer, "MG_Atlantis: CRITICAL ERROR! SRB BURN OUT AT %f s\n", met);
		  oapiWriteLog(buffer);
		  bSRBCutoffFlag = true;
	  }


      // extract current thrust level and propellant level as a function of time
      DisableAllRCS(); //Don't need RCS, SRB gimbal works fine
      double thrust_level, prop_level;
      GetSRB_State (met, thrust_level, prop_level);
      for (i = 0; i < 2; i++)
        SetThrusterLevel (th_srb[i], thrust_level);

	  if(met > 15.0)
	  {
		slag1 = pow(1.0 - thrust_level, 3);
		slag2 = pow(1.0 - thrust_level, 2);
		slag3 = 1.0 - thrust_level;
	  }
	  if (met < 0.0) {
		  LaunchClamps ();
	  }
	  else if(ops==101) ops=102;
    }
	if(bEngineFail && met>=EngineFailTime) FailEngine(EngineFail);
	GPC(simdt);
    break;

  case 2: // post SRB separation
	  met+=simdt;
    if (bManualSeparate) {
	  SetThrusterGroupLevel(THGROUP_MAIN, 0.00);
	  SetThrusterLevel(th_oms[0], 0.00);
	  SetThrusterLevel(th_oms[1], 0.00);
	  bMECO=true;
	  EnableAllRCS();
      SeparateTank();
      bManualSeparate = false;
	  bZThrust=false;
	  //ops=104;
    }
	if (GetEngineLevel (ENGINE_MAIN) > 0.05) {
		DisableAllRCS();
	}
	else if(!bMECO)
	{
		bMECO=true;
		tMECO=met;
		EnableAllRCS();
		SetThrusterLevel(th_oms[0], 0.00);
		SetThrusterLevel(th_oms[1], 0.00);
	}
	else EnableAllRCS();
	if(bEngineFail && met>=EngineFailTime) FailEngine(EngineFail);
	GPC(simdt);
    break;
  case 3: // post tank separation
	  EnableAllRCS();
    //On entry, start shutting down RCS channels as appropriate
  if(RollActive && GetDynPressure()>RollOff) {
    SetThrusterGroupLevel(THGROUP_ATT_BANKLEFT,0);
    SetThrusterGroupLevel(THGROUP_ATT_BANKRIGHT,0);
    DelThrusterGroup(THGROUP_ATT_BANKLEFT);
    DelThrusterGroup(THGROUP_ATT_BANKRIGHT);
    RollActive=false;
  }
  if(PitchActive && GetDynPressure()>PitchOff) {
    SetThrusterGroupLevel(THGROUP_ATT_PITCHUP,0);
    SetThrusterGroupLevel(THGROUP_ATT_PITCHDOWN,0);
    DelThrusterGroup(THGROUP_ATT_PITCHUP);
    DelThrusterGroup(THGROUP_ATT_PITCHDOWN);
    PitchActive=false;
  }
  if(YawActive && GetMachNumber()<YawOff && GetDynPressure()>100) {
    SetThrusterGroupLevel(THGROUP_ATT_YAWLEFT,0);
    SetThrusterGroupLevel(THGROUP_ATT_YAWRIGHT,0);
    DelThrusterGroup(THGROUP_ATT_YAWLEFT);
    DelThrusterGroup(THGROUP_ATT_YAWRIGHT);
    YawActive=false;
  }
  //Check if Control Surfaces are usable
  if(ControlSurfacesEnabled && !r2d2->bHydraulicPressure)
  {
	  DisableControlSurfaces();
  }
  else if(!ControlSurfacesEnabled && r2d2->bHydraulicPressure)
  {
	  EnableControlSurfaces();
  }
  met+=simdt;
  GPC(simdt); //perform GPC functions

    if (bManualSeparate && GetAttachmentStatus (sat_attach)) {
      DetachChild (sat_attach, 0.1);
      bManualSeparate = false;
    }

    if (do_eva) {
      char name[256];
      strcpy (name, GetName()); strcat (name, "-MMU");
      hvessel=oapiGetVesselByName(name);
      if (hvessel == 0)
      {
        SeparateMMU();
        hvessel=oapiGetVesselByName(name);
        if (hvessel != 0)
          oapiSetFocusObject(hvessel);
      }
      else
      {
        hvessel=oapiGetVesselByName(name);
        if (hvessel != 0)
          oapiSetFocusObject(hvessel);
      };
      do_eva = false;
    };
    break;
  }

  //update MET
  MET[0]=met/86400;
  MET[1]=(met-86400*MET[0])/3600;
  MET[2]=(met-86400*MET[0]-3600*MET[1])/60;
  MET[3]=met-86400*MET[0]-3600*MET[1]-60*MET[2];
  //sprintf(oapiDebugString(), "%i", last_mfd);
  //deploy gear
  if(status==3) {
		airspeed=GetAirspeed();
		if(GetAltitude()<92.44 && gop->GetGearAction()==AnimState::CLOSED) {
		  if(GetAltitude()<10 || airspeed<=GEAR_MAX_DEPLOY_SPEED-75) gop->RevertLandingGear();
		}
		else if(GetAltitude()<609.6) gop->ArmGear();
		else if(gop->GetGearAction()!=AnimState::CLOSED && airspeed>GEAR_MAX_DEPLOY_SPEED && GetDynPressure()>10000.0)
		{
		  gop->DamageGear();
		}
  }

  VESSEL *aVessel;
  VESSELSTATUS vs;

  if (reset_mmu && simt-jettison_time > .01)
  {
    GetStatus(vs);
    vs.eng_main = vs.eng_hovr = 0.0;
    VECTOR3 ofs = OFS_ZERO;
    ofs.x += OFS_MMU.x;
    ofs.y += OFS_MMU.y;
    ofs.z += OFS_MMU.z;
    VECTOR3 rofs, rvel = {vs.rvel.x, vs.rvel.y, vs.rvel.z};
    VECTOR3 vel = {0,0,0};
    Local2Rel (ofs, vs.rpos);
    GlobalRot (vel, rofs);
    vs.rvel.x = rvel.x+rofs.x;
    vs.rvel.y = rvel.y+rofs.y;
    vs.rvel.z = rvel.z+rofs.z;
    aVessel = new VESSEL (hMMU, 1);
    aVessel->DefSetState(&vs);
    reset_mmu=false;
  }

  // Execute payload bay operations
  plop->Step (simt, simdt);
  gop->Step (simt, simdt);
  panela4->Step(simt, simdt);
  panela8->Step(simt, simdt);
  panelc2->Step(simt, simdt);
  c3po->Step (simt, simdt);
  panelf7->Step(simt, simdt);
  panelo3->Step(simt, simdt);
  r2d2->Step (simt, simdt);
  


  // ***** Animate speedbrake *****

  if (spdb_status >= AnimState::CLOSING) {
    double da = simdt * SPEEDBRAKE_OPERATING_SPEED;
    if (spdb_status == AnimState::CLOSING) { // retract brake
      if (spdb_proc > spdb_tgt) spdb_proc = max (spdb_tgt, spdb_proc-da);
      else                 spdb_status = AnimState::CLOSED;
    } else {                           // deploy antenna
      if (spdb_proc < spdb_tgt) spdb_proc = min (spdb_tgt, spdb_proc+da);
      else                 spdb_status = AnimState::OPEN;
    }
    SetAnimation (anim_spdb, spdb_proc);
  }

  // ***** RMS Rollout *****
  if(RMSRollout.Moving() && plop->MechPwr[0]==PayloadBayOp::MP_ON && plop->MechPwr[1]==PayloadBayOp::MP_ON && ArmCradled() && plop->BayDoorStatus.pos==1.0) {
	  double da = simdt*ARM_DEPLOY_SPEED;
	  if(RMSRollout.Closing()) {
		  RMSRollout.pos=max(0.0, RMSRollout.pos-da);
		  if(RMSRollout.pos<=0.0) {
			  RMSRollout.action=AnimState::CLOSED;
			  //panela8->UpdateVC();
		  }
	  }
	  else {
		  RMSRollout.pos=min(1.0, RMSRollout.pos+da);
		  if(RMSRollout.pos>=1.0) {
			  RMSRollout.action=AnimState::OPEN;
			  //panela8->UpdateVC();
		  }
	  }
	  SetAnimation(anim_rollout, RMSRollout.pos);
	  UpdateMPMMicroswitches();
  }
  
  // ***** Stow RMS arm *****

  if (center_arm && RMSRollout.action==AnimState::OPEN && Eq(shoulder_brace, 0.0) && Eq(MRL[0], 0.0)) {
    double t0 = oapiGetSimTime();
    double dt = t0 - center_arm_t;       // time step
    double da = ARM_OPERATING_SPEED*dt;  // total rotation angle

    // work from the wrist down to the shoulder
	if (da && (arm_wr != 0.5)) {    // zero wrist roll
      if (da >= fabs(arm_wr-0.5)) // finished
        arm_wr = 0.5, da -= fabs(arm_wr-0.5);
      else
        arm_wr -= (arm_wr > 0.5 ? da:-da), da = 0;
      SetAnimationArm (anim_arm_wr, arm_wr);
    }
    if (da && (arm_wy != 0.5)) {    // zero wrist yaw
      if (da >= fabs(arm_wy-0.5)) // finished
        arm_wy = 0.5, da -= fabs(arm_wy-0.5);
      else
        arm_wy -= (arm_wy > 0.5 ? da:-da), da = 0;
      SetAnimationArm (anim_arm_wy, arm_wy);
    }
    if (da && (arm_wp != wrist_neutral)) {    // zero wrist pitch
      if (da >= fabs(arm_wp-wrist_neutral)) // finished
        arm_wp = wrist_neutral, da -= fabs(arm_wp-wrist_neutral);
      else
        arm_wp -= (arm_wp > wrist_neutral ? da:-da), da = 0;
      SetAnimationArm (anim_arm_wp, arm_wp);
    }
    if (da && (arm_ep != elbow_neutral)) {    // zero elbow pitch
      if (da >= fabs(arm_ep-elbow_neutral)) // finished
        arm_ep = elbow_neutral, da -= fabs(arm_ep-elbow_neutral);
      else
        arm_ep -= (arm_ep > elbow_neutral ? da:-da), da = 0;
      SetAnimationArm (anim_arm_ep, arm_ep);
    }
    if (da && (arm_sy != 0.5)) {    // zero shoulder yaw
      if (da >= fabs(arm_sy-0.5)) // finished
        arm_sy = 0.5, da -= fabs(arm_sy-0.5);
      else
        arm_sy -= (arm_sy > 0.5 ? da:-da), da = 0;
      SetAnimationArm (anim_arm_sy, arm_sy);
    }
    if (da && (arm_sp != shoulder_neutral)) {    // zero shoulder pitch
      if (da >= fabs(arm_sp-shoulder_neutral)) // finished
        arm_sp = shoulder_neutral, da -= fabs(arm_sp-shoulder_neutral);
      else
        arm_sp -= (arm_sp > shoulder_neutral ? da:-da), da = 0;
      SetAnimationArm (anim_arm_sp, arm_sp);
    }
    center_arm_t = t0;
    if (da) {
      center_arm = false; // finished stowing
      HWND hDlg = oapiFindDialog (g_Param.hDLL, IDD_RMS);
      if (hDlg) EnableWindow (GetDlgItem (hDlg, IDC_GRAPPLE), TRUE);
    }
	UpdateRMSPositions();
  }

  if (arm_moved) {
    SetAttachmentParams (rms_attach, orbiter_ofs+arm_tip[0], Normalize(arm_tip[1]-arm_tip[0]), Normalize(arm_tip[2]-arm_tip[0]));
	//sprintf(oapiDebugString(), "%f %f", length(arm_tip[1]-arm_tip[0]), length(arm_tip[2]-arm_tip[0]));
	
	//calculate joint angles
	sy_angle=360.0*arm_sy-180.0;
	sp_angle=shoulder_range*arm_sp+shoulder_min;
	ep_angle=-elbow_range*arm_ep-elbow_min;
	wp_angle=wrist_range*arm_wp+wrist_min;
	wy_angle=wrist_yaw_range*arm_wy+wrist_yaw_min;
	wr_angle=wrist_roll_range*arm_wr+wrist_roll_min;
	
	// If the current camera mode is the RMS_EFFECTOR move camera position to match
	// the position and direction of the wrist
	if (VCMode == VC_LEECAM) {
		double tilt = wr_angle;
		if(tilt<-180.0) tilt+=360.0;
		else if(tilt>180.0) tilt-=360.0;
		SetCameraOffset (_V(orbiter_ofs.x,orbiter_ofs.y,orbiter_ofs.z)+arm_tip[0]+RotateVectorZ(ARM_WRIST_CAM_OFFSET, wr_angle));
		SetCameraDefaultDirection (arm_tip[1]-arm_tip[0], -tilt*RAD);
	}

    arm_moved = false;
  }
  if(DisplayJointAngles) {
	  sprintf(oapiDebugString(), "SY:%f SP:%f EP:%f WY:%f WP:%f WR:%f", sy_angle, sp_angle, ep_angle,
		wy_angle, wp_angle, wr_angle);
	  /*VECTOR3 pos, pos2, dir, dir2;
	  CalcAnimationFKArm(pos2, dir2);
	  pos=orbiter_ofs+arm_tip[0];
	  pos=RotateVectorX(_V(-pos.z, pos.x, pos.y)-shoulder_pos, 18.435);
	  //pos=CalcAnimationFKArm();
	  dir=arm_tip[1]-arm_tip[0];
	  dir=RotateVectorX(_V(-dir.z, dir.x, dir.y), 18.435);
	  //pos2=RotateVectorX(pos2, -18.435)+shoulder_pos;
	  //SetAttachmentParams (rms_attach, _V(pos2.y, pos2.z, -pos2.x), Normalize(arm_tip[1]-arm_tip[0]), Normalize(arm_tip[2]-arm_tip[0]));
	  sprintf(oapiDebugString(), "%f %f %f %f %f %f %f %f %f %f %f %f %f %f", pos.x, pos.y, pos.z, pos2.x, pos2.y, pos2.z,
		  dir.x, dir.y, dir.z, dir2.x, dir2.y, dir2.z, sp_null, ep_null);*/

  }

  // Animate payload bay cameras.
  if (cameraMoved) {
  SetAnimationCameras();
  cameraMoved = false;
  }

  pCommModeHandler->PostStep(simt, simdt);

  if(fTimeCameraLabel > 0)
  {
	  fTimeCameraLabel -= simdt;
	  if(fTimeCameraLabel < 0)
		  fTimeCameraLabel = 0;
	  if(0 == fTimeCameraLabel)
	  {
		  oapiAnnotationSetText(nhCameraLabel, NULL);
	  }
  }
}

// --------------------------------------------------------------
// Respond to playback event
// --------------------------------------------------------------
bool Atlantis::clbkPlaybackEvent (double simt, double event_t, const char *event_type, const char *event)
{
  if (!stricmp (event_type, "JET")) {
    if (!stricmp (event, "SRB")) {
      bManualSeparate = true;
      return true;
    }
    else if (!stricmp (event, "ET")) {
      bManualSeparate = true;
      return true;
    }
  } else if (!_stricmp (event_type, "STATUS")) {
    if (!_stricmp (event, "SRB_IGNITION")) {
      status = 1;
      t0 = event_t + SRB_STABILISATION_TIME;
      return true;
    }
  } else if (!_stricmp (event_type, "CARGODOOR")) {
    plop->SetDoorAction (!stricmp (event, "CLOSE") ? AnimState::CLOSING : AnimState::OPENING);
    return true;
  } else if (!_stricmp (event_type, "GEAR")) {
    gop->OperateLandingGear (!stricmp (event, "UP") ? AnimState::CLOSING : AnimState::OPENING);
    return true;
  } else if (!_stricmp (event_type,"SPEEDBRAKE")) {
    OperateSpeedbrake (!stricmp (event, "CLOSE") ? AnimState::CLOSING : AnimState::OPENING);
    return true;
  } else if (!_stricmp (event_type, "KUBAND")) {
    plop->SetKuAntennaAction (!stricmp (event, "CLOSE") ? AnimState::CLOSING : AnimState::OPENING);
    return true;
  } else if(psubsystems->PlaybackEvent(simt, event_t, event_type, event)) {
	  return true;
  }

  return false;
}

void Atlantis::clbkDrawHUD(int mode, const HUDPAINTSPEC *hps, HDC hDC)
{
	//int i;
	char cbuf[255];
	VECTOR3 Velocity;
	double dOut;
	int commanded, act;
	// draw the default HUD
	VESSEL2::clbkDrawHUD (mode, hps, hDC);

	// show gear deployment status
	if (gop->GetGearAction()==AnimState::OPEN) {
		TextOut (hDC, 10, (hps->H)/2, "GR-DN", 5);
	}
	else if (gop->GetGearAction()==AnimState::OPENING || gop->GetGearAction()==AnimState::CLOSING) {
		TextOut (hDC, 10, (hps->H)/2, "//GR//", 6);
	}
	if(gop->SwitchPush[0]==gop->SW_PUSH)
	{
		TextOut(hDC, hps->W-25, (hps->H)/2, "ARM", 3);
	}
	if(c3po->CheckProbesDeployed()==true && GetAltitude()<50000)
	{
		GetHorizonAirspeedVector(Velocity);
		sprintf(cbuf,"VSPEED:%.2f",Velocity.y);
		TextOut(hDC,10,(hps->H)/2-35,cbuf,strlen(cbuf));
		//dOut=GetAirspeed()*1.943844;
		const double EAS_EXP = 2.0/7.0;
		const double P0 = 1.01325E6;
		dOut = 661.48 * sqrt(5.0 * GetAtmPressure()/ATMP * (pow(GetDynPressure() / GetAtmPressure() + 1, EAS_EXP) - 1.0));
		sprintf(cbuf, "KEAS:%.0f", dOut);
		//sprintf(cbuf,"KEAS:%.0f",dOut);
		TextOut(hDC,hps->W-100,(hps->H)/2-25,cbuf,strlen(cbuf));
		dOut=(GetAltitude()*3.280833)-17;
		sprintf(cbuf,"ALT:%.0f",dOut);
		TextOut(hDC,10,(hps->H)/2-25,cbuf,strlen(cbuf));
	}
	//speedbrakes
	//sprintf(oapiDebugString(), "%f", GetDrag());
	if(GetAltitude()<50000 && status==3)
	{
		//sprintf(oapiDebugString(), "%f", GetDrag());
		MoveToEx(hDC, (hps->W/2)-25, hps->H-85, NULL);
		LineTo(hDC, (hps->W/2)+25, hps->H-85);
		for(int i=-25;i<=25;i+=10)
		{
			MoveToEx(hDC, (hps->W/2)+i, hps->H-80, NULL);
			LineTo(hDC, (hps->W/2)+i, hps->H-90);
		}
		commanded=(int)(spdb_tgt*50)-25;
		act=(int)(spdb_proc*50)-25;
		//actual
		MoveToEx(hDC, (hps->W/2)+act, hps->H-85, NULL);
		LineTo(hDC, (hps->W/2)+act-5, hps->H-90);
		LineTo(hDC, (hps->W/2)+act+5, hps->H-90);
		LineTo(hDC, (hps->W/2)+act, hps->H-85);
		//commanded
		MoveToEx(hDC, (hps->W/2)+commanded, hps->H-85, NULL);
		LineTo(hDC, (hps->W/2)+commanded-5, hps->H-80);
		LineTo(hDC, (hps->W/2)+commanded+5, hps->H-80);
		LineTo(hDC, (hps->W/2)+commanded, hps->H-85);
	}
}

// --------------------------------------------------------------
// Atlantis mesh loaded
// --------------------------------------------------------------
void Atlantis::clbkVisualCreated (VISHANDLE _vis, int refcount)
{
  if (refcount > 1) return; // we don't support more than one visual per object
  vis = _vis;

#ifdef UNDEF
  // note: orbiter re-applies the animations to the mesh before calling
  // clbkVisualCreated, so updating the mesh here is not necessary

  // reset grappling point
  //wrist_yaw_joint[0] = _V(-2.87, 2.03, -4.88);
  //wrist_yaw_joint[1] = _V(-2.87, 2.03, -4.88)+RotateVectorZ(_V(0.0, 1.0, 0.0), 18.435);
  arm_tip[0] = _V(-2.87, 2.03, -6.27);
  arm_tip[1] = _V(-2.87, 2.03, -7.27);
  arm_tip[2] = _V(-2.87, 3.03, -6.27);

  UpdateMesh ();
#endif
}

// --------------------------------------------------------------
// Atlantis mesh discarded
// --------------------------------------------------------------
void Atlantis::clbkVisualDestroyed (VISHANDLE _vis, int refcount)
{
  if (vis == _vis) vis = NULL;
}

// --------------------------------------------------------------
// Update mesh animation state
// --------------------------------------------------------------
void Atlantis::clbkAnimate (double simt)
{
  UpdateMesh ();
}

// --------------------------------------------------------------
// Respond to MFD mode change
// --------------------------------------------------------------
void Atlantis::clbkMFDMode (int mfd, int mode)
{
	oapiVCTriggerRedrawArea (-1, AID_CDR1_BUTTONS+mfd-MFD_LEFT);
	
	//get pointer to CRT MFD as required
	if(newmfd!=NULL) {
		if(mfd==4) newmfd->id=0;
		else if(mfd==7) newmfd->id=1;
		else if(mfd==6) newmfd->id=2;
		else if(mfd==9) newmfd->id=3;
		else {
			//newmfd->id=-1;
			if(mfd<4) newmfd->id=mfd+4;
			else if(mfd==5) newmfd->id=8;
			else if(mfd>=8) newmfd->id=mfd+1;
		}
		if((newmfd->id)<3) {
			Display[newmfd->id]=newmfd;
		}
		newmfd->UpdateStatus=true;
		newmfd=NULL;
	}
	//sprintf(oapiDebugString(), "%d", mfd);
}

// --------------------------------------------------------------
// Load generic glass cockpit mode
// --------------------------------------------------------------
bool Atlantis::clbkLoadGenericCockpit ()
{
  SetCameraOffset (_V(orbiter_ofs.x-0.67,orbiter_ofs.y+2.55,orbiter_ofs.z+14.4));
  SetCameraDefaultDirection (_V(0,0,1));
  return true;
}

// --------------------------------------------------------------
// register VC buttons for the 2 commander MFDs
// (accessible from commander position only)
// --------------------------------------------------------------
void Atlantis::RegisterVC_CdrMFD ()
{

  // -0.9049484  2.118143 14.7288
  // -0.8917124  2.114834
  // -0.7282474  2.104245
  //2.121287  14.70349
  // activate MFD function buttons
  oapiVCSetAreaClickmode_Quadrilateral (AID_CDR1_BUTTONS, 
	  _V(-0.900,  2.11, 14.703)+orbiter_ofs, _V(-0.725, 2.11, 14.703)+orbiter_ofs,  
	  _V(-0.900, 2.09, 14.703)+orbiter_ofs, _V(-0.725, 2.09, 14.703)+orbiter_ofs);

  //-0.6275758
  //-0.4694053
  oapiVCSetAreaClickmode_Quadrilateral (AID_CDR2_BUTTONS, 
	  _V(-0.630, 2.11, 14.703)+orbiter_ofs,  _V(-0.469, 2.11, 14.703)+orbiter_ofs,
	  _V(-0.630, 2.09, 14.703)+orbiter_ofs, _V(-0.469, 2.09, 14.703)+orbiter_ofs);

    // D. Beachy: register+activate MFD power buttons
  const double powerButtonRadius = 0.0075; // radius of power button on each MFD

  oapiVCRegisterArea (AID_CDR1_PWR, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_ONREPLAY);
  oapiVCRegisterArea (AID_CDR2_PWR, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_ONREPLAY);

  oapiVCSetAreaClickmode_Spherical(AID_CDR1_PWR, _V(-0.92, 2.123, 14.700)+orbiter_ofs, powerButtonRadius);	
  oapiVCSetAreaClickmode_Spherical(AID_CDR2_PWR, _V(-0.655, 2.123, 14.700)+orbiter_ofs, powerButtonRadius);

  // register+activate MFD brightness buttons
  oapiVCRegisterArea (AID_CDR1_BRT, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY);
  oapiVCRegisterArea (AID_CDR2_BRT, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY);
  oapiVCSetAreaClickmode_Spherical (AID_CDR1_BRT, _V(-0.707, 2.123, 14.700)+orbiter_ofs, powerButtonRadius);
  oapiVCSetAreaClickmode_Spherical (AID_CDR2_BRT, _V(-0.441, 2.123, 14.700)+orbiter_ofs, powerButtonRadius);
}

// --------------------------------------------------------------
// register VC buttons for the 2 pilot MFDs
// (accessible from pilot position only)
// --------------------------------------------------------------
void Atlantis::RegisterVC_PltMFD ()
{
  // activate MFD function buttons
  oapiVCSetAreaClickmode_Quadrilateral (AID_PLT1_BUTTONS, 
	  _V(0.469, 2.11, 14.703) + orbiter_ofs, _V(0.630, 2.11, 14.703) + orbiter_ofs, 
	  _V(0.469, 2.09, 14.703) + orbiter_ofs, _V(0.630, 2.09, 14.703) + orbiter_ofs);
  oapiVCSetAreaClickmode_Quadrilateral (AID_PLT2_BUTTONS, 
	  _V(0.725, 2.11, 14.703) + orbiter_ofs, _V(0.900,  2.11, 14.703) + orbiter_ofs, 
	  _V(0.725, 2.09, 14.703) + orbiter_ofs, _V(0.900, 2.09, 14.703) + orbiter_ofs);

    // D. Beachy: register+activate MFD power buttons
    const double powerButtonRadius = 0.0075; // radius of power button on each MFD
  oapiVCRegisterArea (AID_PLT1_PWR, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_ONREPLAY);
  oapiVCRegisterArea (AID_PLT2_PWR, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_ONREPLAY);
    oapiVCSetAreaClickmode_Spherical(AID_PLT1_PWR, _V( 0.441, 2.123, 14.700)+orbiter_ofs, powerButtonRadius);
    oapiVCSetAreaClickmode_Spherical(AID_PLT2_PWR, _V( 0.707, 2.123, 14.700)+orbiter_ofs, powerButtonRadius);

  // register+activate MFD brightness buttons
  oapiVCRegisterArea (AID_PLT1_BRT, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY);
  oapiVCRegisterArea (AID_PLT2_BRT, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY);
  oapiVCSetAreaClickmode_Spherical (AID_PLT1_BRT, _V(0.655, 2.123, 14.700)+orbiter_ofs, powerButtonRadius);
  oapiVCSetAreaClickmode_Spherical (AID_PLT2_BRT, _V(0.92, 2.123, 14.700)+orbiter_ofs, powerButtonRadius);
}

// --------------------------------------------------------------
// register VC buttons for the 5 MFDs on the central panel
// (accessible from commander and pilot positions)
// --------------------------------------------------------------
void Atlantis::RegisterVC_CntMFD ()
{
  // activate MFD function buttons
  oapiVCSetAreaClickmode_Quadrilateral (AID_CRT1_BUTTONS, 
	  _V(-0.346, 2.212, 14.728) + orbiter_ofs, _V(-0.176, 2.212, 14.728) + orbiter_ofs, 
	  _V(-0.346, 2.2, 14.724) + orbiter_ofs, _V(-0.176, 2.2, 14.724) + orbiter_ofs);
  oapiVCSetAreaClickmode_Quadrilateral (AID_CRT2_BUTTONS, 
	  _V( 0.176, 2.212, 14.728) + orbiter_ofs, _V(0.346, 2.212, 14.728) + orbiter_ofs, 
	  _V( 0.176, 2.2, 14.724) + orbiter_ofs, _V(0.346, 2.2, 14.724) + orbiter_ofs);
  oapiVCSetAreaClickmode_Quadrilateral (AID_CRT3_BUTTONS, 
	  _V(-0.082, 2.099, 14.696) + orbiter_ofs, _V(0.085, 2.099,14.696) + orbiter_ofs,
	  _V(-0.082, 2.09, 14.693) + orbiter_ofs, _V(0.085, 2.09, 14.693) + orbiter_ofs);

  oapiVCSetAreaClickmode_Quadrilateral (AID_MFD1_BUTTONS, 
	  _V(-0.346, 1.987, 14.466)+orbiter_ofs, _V(-0.176, 1.987, 14.466)+orbiter_ofs, 
	  _V(-0.346, 1.977, 14.462)+orbiter_ofs, _V(-0.176, 1.977, 14.462)+orbiter_ofs);
  oapiVCSetAreaClickmode_Quadrilateral (AID_MFD2_BUTTONS, 
	  _V( 0.176, 1.987, 14.466) + orbiter_ofs, _V(0.346, 1.987, 14.466) + orbiter_ofs, 
	  _V( 0.176, 1.977, 14.462) + orbiter_ofs, _V(0.346, 1.977, 14.464) + orbiter_ofs);

    // D. Beachy: register+activate MFD power buttons
    const double powerButtonRadius = 0.0075; // radius of power button on each MFD
  oapiVCRegisterArea (AID_CRT1_PWR, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_ONREPLAY);
  oapiVCRegisterArea (AID_CRT2_PWR, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_ONREPLAY);
  oapiVCRegisterArea (AID_CRT3_PWR, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_ONREPLAY);
  oapiVCRegisterArea (AID_MFD1_PWR, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_ONREPLAY);
  oapiVCRegisterArea (AID_MFD2_PWR, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_ONREPLAY);

  oapiVCSetAreaClickmode_Spherical(AID_CRT1_PWR, _V(-0.366, 2.216, 14.729)+orbiter_ofs, powerButtonRadius);
  oapiVCSetAreaClickmode_Spherical(AID_CRT2_PWR, _V( 0.155, 2.216, 14.729)+orbiter_ofs, powerButtonRadius);
  oapiVCSetAreaClickmode_Spherical(AID_CRT3_PWR, _V(-0.103, 2.103, 14.697)+orbiter_ofs, powerButtonRadius);
  oapiVCSetAreaClickmode_Spherical(AID_MFD1_PWR, _V(-0.366, 1.992, 14.667)+orbiter_ofs, powerButtonRadius);
  oapiVCSetAreaClickmode_Spherical(AID_MFD2_PWR, _V( 0.155, 1.992, 14.667)+orbiter_ofs, powerButtonRadius);

  // register+activate MFD brightness buttons
  oapiVCRegisterArea (AID_CRT1_BRT, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY);
  oapiVCRegisterArea (AID_CRT2_BRT, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY);
  oapiVCRegisterArea (AID_CRT3_BRT, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY);
  oapiVCRegisterArea (AID_MFD1_BRT, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY);
  oapiVCRegisterArea (AID_MFD2_BRT, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY);

  oapiVCSetAreaClickmode_Spherical (AID_CRT1_BRT, _V(-0.155, 2.216, 14.729)+orbiter_ofs, powerButtonRadius);
  oapiVCSetAreaClickmode_Spherical (AID_CRT2_BRT, _V(0.366, 2.216, 14.729)+orbiter_ofs, powerButtonRadius);
  oapiVCSetAreaClickmode_Spherical (AID_CRT3_BRT, _V(0.103, 2.103, 14.697)+orbiter_ofs, powerButtonRadius);
  oapiVCSetAreaClickmode_Spherical (AID_MFD1_BRT, _V(-0.155,1.992, 14.667)+orbiter_ofs, powerButtonRadius);
  oapiVCSetAreaClickmode_Spherical (AID_MFD2_BRT, _V(0.366,1.992, 14.667)+orbiter_ofs, powerButtonRadius);
}

// --------------------------------------------------------------
// register VC buttons for the aft MFD at the starbord panel
// (accessible from payload control position only)
// --------------------------------------------------------------
void Atlantis::RegisterVC_AftMFD ()
{
  // register+activate aft MFD function buttons
  //SURFHANDLE tex1 = oapiGetTextureHandle (hOrbiterVCMesh, TEX_LABEL_VC);
  //oapiVCRegisterArea (AID_CRT4_BUTTONS, _R(0,127,255,140), PANEL_REDRAW_USER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBUP|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY, PANEL_MAP_BACKGROUND, tex1);
  
  oapiVCSetAreaClickmode_Quadrilateral (AID_CRT4_BUTTONS, 
	  _V(1.352, 2.319, 13.54) + orbiter_ofs, _V(1.352, 2.319, 13.376) + orbiter_ofs, 
	  _V(1.348, 2.315, 13.54) + orbiter_ofs, _V(1.348, 2.315, 13.376) + orbiter_ofs);

  // register+activate MFD power button
    const double powerButtonRadius = 0.0075; // radius of power button on each MFD
  oapiVCRegisterArea (AID_CRT4_PWR, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_ONREPLAY);
    oapiVCSetAreaClickmode_Spherical(AID_CRT4_PWR, _V(1.356, 2.321,13.563) + orbiter_ofs, powerButtonRadius);

  // register+activate MFD brightness buttons
  oapiVCRegisterArea (AID_CRT4_BRT, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY);
  oapiVCSetAreaClickmode_Spherical (AID_CRT4_BRT, _V(1.356, 2.321,13.35)+orbiter_ofs, powerButtonRadius);
}

// --------------------------------------------------------------
// Load virtual cockpit mode
// --------------------------------------------------------------
bool Atlantis::clbkLoadVC (int id)
{
  bool ok = false;
  double tilt = 0.0;

    // Get the VC Mode.
  VCMode = id;


  // register MFD function buttons
  // this needs to be done globally, so that the labels are correctly updated from all VC positions
  SURFHANDLE tex1 = oapiGetTextureHandle (hOrbiterVCMesh, TEX_LABEL_VC);
  
  // commander MFD function buttons
  oapiVCRegisterArea (AID_CDR1_BUTTONS, _R(0,1,255,14), PANEL_REDRAW_USER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBUP|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY, PANEL_MAP_BACKGROUND, tex1);
  oapiVCRegisterArea (AID_CDR2_BUTTONS, _R(0,15,255,28), PANEL_REDRAW_USER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBUP|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY, PANEL_MAP_BACKGROUND, tex1);
  // pilot MFD function buttons
  oapiVCRegisterArea (AID_PLT1_BUTTONS, _R(0,29,255,42), PANEL_REDRAW_USER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBUP|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY, PANEL_MAP_BACKGROUND, tex1);
  oapiVCRegisterArea (AID_PLT2_BUTTONS, _R(0,43,255,56), PANEL_REDRAW_USER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBUP|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY, PANEL_MAP_BACKGROUND, tex1);
  // central console MFD function buttons
  oapiVCRegisterArea (AID_CRT1_BUTTONS, _R(0, 57,255, 70), PANEL_REDRAW_USER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBUP|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY, PANEL_MAP_BACKGROUND, tex1);
  oapiVCRegisterArea (AID_CRT2_BUTTONS, _R(0, 99,255,112), PANEL_REDRAW_USER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBUP|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY, PANEL_MAP_BACKGROUND, tex1);
  oapiVCRegisterArea (AID_CRT3_BUTTONS, _R(0, 85,255, 98), PANEL_REDRAW_USER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBUP|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY, PANEL_MAP_BACKGROUND, tex1);
  oapiVCRegisterArea (AID_CRT4_BUTTONS, _R(0,127,255,140), PANEL_REDRAW_USER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBUP|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY, PANEL_MAP_BACKGROUND, tex1);
  oapiVCRegisterArea (AID_MFD1_BUTTONS, _R(0, 71,255, 84), PANEL_REDRAW_USER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBUP|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY, PANEL_MAP_BACKGROUND, tex1);
  oapiVCRegisterArea (AID_MFD2_BUTTONS, _R(0,113,255,126), PANEL_REDRAW_USER, PANEL_MOUSE_LBDOWN|PANEL_MOUSE_LBUP|PANEL_MOUSE_LBPRESSED|PANEL_MOUSE_ONREPLAY, PANEL_MAP_BACKGROUND, tex1);
  

  // VC Cockpit not visible from Payload cameras or RMS camera.
  // 080415, DaveS edit: Fixed VC being seen from the BR PLB camera. Original line below in comment
  // Original line: if ((id > VC_PLBCAMFL && id < VC_PLBCAMBR) || id == VC_LEECAM || id == VC_DOCKCAM) {
  if ((id == VC_PLBCAMBL || id == VC_PLBCAMBR) || id == VC_LEECAM || id == VC_DOCKCAM) {
  SetMeshVisibilityMode (mesh_vc, MESHVIS_EXTERNAL);
  } else {
  SetMeshVisibilityMode (mesh_vc, MESHVIS_VC);
  }

  switch (id) {
  case VC_CDR: // commander position
    DisplayCameraLabel(VC_LBL_CDR);
    SetCameraOffset (orbiter_ofs + VC_POS_CDR);
    SetCameraDefaultDirection (_V(0,0,1));
    SetCameraMovement (_V(0,0,0.3), 0, 0, _V(-0.3,0,0), 75*RAD, -5*RAD, _V(0.3,0,0), -20*RAD, -27*RAD);
    huds.hudcnt = orbiter_ofs + VC_POS_CDR;
    oapiVCSetNeighbours (VC_PORTSTATION, VC_PLT, VC_DOCKCAM, VC_MS2);

  // Default camera rotarion
  SetCameraRotationRange(144*RAD, 144*RAD, 72*RAD, 72*RAD);

    RegisterVC_CdrMFD (); // activate commander MFD controls
    RegisterVC_CntMFD (); // activate central panel MFD controls
    gop->RegisterVC ();  // register panel F6 interface
	panela4->RegisterVC();
	c3po->RegisterVC();
	panelc2->RegisterVC();
	CDRKeyboard->RegisterVC();
	PLTKeyboard->RegisterVC();
	panelf7->RegisterVC();
	panelo3->RegisterVC();
    ok = true;
    break;
  case VC_PLT: // pilot position
	DisplayCameraLabel(VC_LBL_PLT);
    SetCameraOffset (orbiter_ofs + VC_POS_PLT);
    SetCameraDefaultDirection (_V(0,0,1));
    SetCameraMovement (_V(0,0,0.3), 0, 0, _V(-0.3,0,0), 20*RAD, -27*RAD, _V(0.3,0,0), -75*RAD, -5*RAD);
    huds.hudcnt = orbiter_ofs + VC_POS_PLT;
    oapiVCSetNeighbours (VC_CDR, VC_STBDSTATION, VC_DOCKCAM, VC_MS1);

  // Default camera rotarion
  SetCameraRotationRange(144*RAD, 144*RAD, 72*RAD, 72*RAD);

    RegisterVC_PltMFD (); // activate pilot MFD controls
    RegisterVC_CntMFD (); // activate central panel MFD controls
	c3po->RegisterVC();
	r2d2->RegisterVC();
	panelo3->RegisterVC();
	panela4->RegisterVC();
	panelc2->RegisterVC();
	CDRKeyboard->RegisterVC();
	PLTKeyboard->RegisterVC();
	panelf7->RegisterVC();
    ok = true;
    break;
  case VC_STBDSTATION: 
	  DisplayCameraLabel(VC_LBL_STBDSTATION);
    SetCameraOffset (VC_POS_STBDSTATION + orbiter_ofs);
    SetCameraDefaultDirection (VC_DIR_STBDSTATION);
    //SetCameraMovement (_V(0,0.20,0.20), 0, 40.0*RAD, _V(0.3,-0.3,0.15), 60.0*RAD, -50.0*RAD, _V(-0.8,0,0), 0, 0);

    // Outside cameras neighbours
	oapiVCSetNeighbours(VC_PLT, VC_AFTPILOT, VC_DOCKCAM, VC_PORTSTATION);
    //oapiVCSetNeighbours (1, 0, 3, 0);

    // Default camera rotarion
    SetCameraRotationRange(144*RAD, 144*RAD, 72*RAD, 72*RAD);

    RegisterVC_AftMFD (); // activate aft MFD controls
    plop->RegisterVC ();  // register panel R13L interface
	panela4->RegisterVC();
	panela8->RegisterVC();
	panelo3->RegisterVC();
    ok = true;
    break;
  case VC_LEECAM: //RMS End Effector Camera
	  DisplayCameraLabel(VC_LBL_LEECAM);
	tilt = wr_angle;
	if(tilt<-180.0) tilt+=360.0;
	else if(tilt>180.0) tilt-=360.0;

    //SetCameraOffset (_V(orbiter_ofs.x,orbiter_ofs.y,orbiter_ofs.z)+arm_tip[0]);
    SetCameraOffset (_V(orbiter_ofs.x,orbiter_ofs.y,orbiter_ofs.z)+arm_tip[0]+RotateVectorZ(ARM_WRIST_CAM_OFFSET, wr_angle));
	//SetCameraDefaultDirection (arm_tip[1]-arm_tip[0]);
	SetCameraDefaultDirection (arm_tip[1]-arm_tip[0], -tilt*RAD);
    oapiVCSetNeighbours (-1, -1, -1, VC_RMSSTATION);

    // No rotation allowed for RMS wrist camera.
    SetCameraRotationRange(0,0,0,0);
    // No lean for RMS wrist camera
    SetCameraMovement (_V(0,0,0), 0, 0, _V(0,0,0), 0, 0, _V(0,0,0), 0, 0);

    ok = true;
    break;
  case VC_PLBCAMFL: //FL Payload Bay Camera
	  DisplayCameraLabel(VC_LBL_PLBCAMFL);
    SetCameraOffset (_V(orbiter_ofs.x-1.759,orbiter_ofs.y+1.656,orbiter_ofs.z+11.902));
    oapiVCSetNeighbours (VC_PLBCAMBL, VC_PLBCAMFR, VC_LEECAM, VC_DOCKCAM);

    ok = true;
    break;
  case VC_PLBCAMFR: //FR Payload Bay Camera
	  DisplayCameraLabel(VC_LBL_PLBCAMFR);
    SetCameraOffset (_V(orbiter_ofs.x+1.759,orbiter_ofs.y+1.656,orbiter_ofs.z+11.902));
    oapiVCSetNeighbours (VC_PLBCAMFL, VC_PLBCAMBR, VC_LEECAM, VC_DOCKCAM);

    ok = true;
    break;
  case VC_PLBCAMBL: //BL Payload Bay Camera
	  DisplayCameraLabel(VC_LBL_PLBCAMBL);
    SetCameraOffset (_V(orbiter_ofs.x-2.263,orbiter_ofs.y+1.656,orbiter_ofs.z-6.42));
    oapiVCSetNeighbours (VC_PLBCAMBR, VC_PLBCAMFL, VC_LEECAM, VC_DOCKCAM);

    ok = true;
    break;
  case VC_PLBCAMBR: //BR Payload Bay Camera
	  DisplayCameraLabel(VC_LBL_PLBCAMBR);
    SetCameraOffset (_V(orbiter_ofs.x+2.263,orbiter_ofs.y+1.656,orbiter_ofs.z-6.42));
    oapiVCSetNeighbours (VC_PLBCAMFR, VC_PLBCAMBL, VC_LEECAM, VC_DOCKCAM);

    ok = true;
    break;
  case VC_DOCKCAM: //Docking camera
	  DisplayCameraLabel(VC_LBL_DOCKCAM);
	  SetCameraOffset (_V(orbiter_ofs.x,orbiter_ofs.y+1.20,orbiter_ofs.z+10.1529));
	  SetCameraDefaultDirection (_V(0.0, 1.0, 0.0));
	  SetCameraRotationRange(0, 0, 0, 0);
	  oapiVCSetNeighbours(-1, -1, VC_PLBCAMFL, VC_AFTPILOT);
	  ok = true;
	  break;
  case VC_AFTPILOT: //Aft Flight Deck
	  DisplayCameraLabel(VC_LBL_AFTPILOT);
	SetCameraOffset (VC_POS_AFTPILOT + orbiter_ofs);
    SetCameraDefaultDirection (VC_DIR_AFTPILOT);
	SetCameraMovement (VC_OFSFWD_AFTPILOT, 0, 90.0*RAD, _V(0.8,0,0), 0, 0, _V(0,-0.6,0.15), 0, 0);
	// Default camera rotarion
    SetCameraRotationRange(144*RAD, 144*RAD, 95*RAD, 72*RAD);

    // Outside cameras neighbours
	oapiVCSetNeighbours(VC_STBDSTATION, VC_RMSSTATION, VC_DOCKCAM, VC_AFTWORKSTATION);

    plop->RegisterVC ();  // register panel R13L interface
	panela4->RegisterVC();
	panela8->RegisterVC();
	panelo3->RegisterVC();
	ok = true;
	break;
  case VC_RMSSTATION: 
	  DisplayCameraLabel(VC_LBL_RMSSTATION);
	SetCameraOffset (orbiter_ofs + VC_POS_RMSSTATION);
    SetCameraDefaultDirection (VC_DIR_RMSSTATION);
    //SetCameraMovement (_V(0,0,0.3), 0, 0, _V(-0.3,0,0), 20*RAD, -27*RAD, _V(0.3,0,0), -75*RAD, -5*RAD);
    oapiVCSetNeighbours (VC_AFTPILOT, VC_PORTSTATION, VC_DOCKCAM, VC_AFTWORKSTATION);

	// Default camera rotation
	SetCameraRotationRange(144*RAD, 144*RAD, 72*RAD, 72*RAD);

	plop->RegisterVC ();  // register panel R13L interface
	panela4->RegisterVC();
	panela8->RegisterVC();
	panelo3->RegisterVC();
	ok = true;
	break;
  case VC_PORTSTATION:
	  DisplayCameraLabel(VC_LBL_PORTSTATION);
	SetCameraOffset (orbiter_ofs + VC_POS_PORTSTATION);
    SetCameraDefaultDirection (VC_DIR_PORTSTATION);
    //SetCameraMovement (_V(0,0,0.3), 0, 0, _V(-0.3,0,0), 20*RAD, -27*RAD, _V(0.3,0,0), -75*RAD, -5*RAD);
    oapiVCSetNeighbours (VC_RMSSTATION, VC_CDR, VC_DOCKCAM, VC_STBDSTATION);

	// Default camera rotation
	SetCameraRotationRange(144*RAD, 144*RAD, 72*RAD, 72*RAD);

	plop->RegisterVC ();  // register panel R13L interface
	panela4->RegisterVC();
	panela8->RegisterVC();
	panelo3->RegisterVC();
	ok = true;
	break;
  case VC_AFTWORKSTATION:
	  DisplayCameraLabel(VC_LBL_AFTWORKSTATION);
	SetCameraOffset (orbiter_ofs + VC_POS_AFTWORKSTATION);
    SetCameraDefaultDirection (VC_DIR_AFTWORKSTATION);
    //SetCameraMovement (_V(0,0,0.3), 0, 0, _V(-0.3,0,0), 20*RAD, -27*RAD, _V(0.3,0,0), -75*RAD, -5*RAD);
    oapiVCSetNeighbours (VC_STBDSTATION, VC_MS1, VC_RMSSTATION, VC_PORTSTATION);

	// Default camera rotation
	SetCameraRotationRange(144*RAD, 144*RAD, 72*RAD, 72*RAD);

	SetCameraRotationRange(144*RAD, 144*RAD, 72*RAD, 72*RAD);
	plop->RegisterVC ();  // register panel R13L interface
	panela4->RegisterVC();
	panela8->RegisterVC();
	panelo3->RegisterVC();
	ok = true;
	break;

  case VC_MS1:
	 DisplayCameraLabel(VC_LBL_MS1);
	SetCameraOffset (orbiter_ofs + VC_POS_MS1);
    SetCameraDefaultDirection (VC_DIR_MS1);
    //SetCameraMovement (_V(0,0,0.3), 0, 0, _V(-0.3,0,0), 20*RAD, -27*RAD, _V(0.3,0,0), -75*RAD, -5*RAD);
    oapiVCSetNeighbours (VC_PORTSTATION, VC_MS2, VC_PLT, VC_DOCKCAM);

	// Default camera rotation
	SetCameraRotationRange(144*RAD, 144*RAD, 72*RAD, 72*RAD);

    RegisterVC_CdrMFD();
	RegisterVC_PltMFD (); // activate pilot MFD controls
    RegisterVC_CntMFD (); // activate central panel MFD controls
	panela4->RegisterVC();
	c3po->RegisterVC();
	r2d2->RegisterVC();
	panelo3->RegisterVC();
	panelc2->RegisterVC();
	CDRKeyboard->RegisterVC();
	PLTKeyboard->RegisterVC();
	panelf7->RegisterVC();
    ok = true;
    break;
  case VC_MS2:
	  DisplayCameraLabel(VC_LBL_MS2);
	SetCameraOffset (orbiter_ofs + VC_POS_MS2);
    SetCameraDefaultDirection (VC_DIR_MS2);
    //SetCameraMovement (_V(0,0,0.3), 0, 0, _V(-0.3,0,0), 20*RAD, -27*RAD, _V(0.3,0,0), -75*RAD, -5*RAD);
    oapiVCSetNeighbours (VC_MS1, VC_STBDSTATION, VC_CDR, VC_DOCKCAM);

	// Default camera rotation
	SetCameraRotationRange(144*RAD, 144*RAD, 72*RAD, 72*RAD);
    RegisterVC_CdrMFD();
	RegisterVC_PltMFD (); // activate pilot MFD controls
    RegisterVC_CntMFD (); // activate central panel MFD controls
	panela4->RegisterVC();
	c3po->RegisterVC();
	r2d2->RegisterVC();
	panelo3->RegisterVC();
	panelc2->RegisterVC();
	CDRKeyboard->RegisterVC();
	PLTKeyboard->RegisterVC();
	panelf7->RegisterVC();
    ok = true;
    break;

  }

  // Common action for external payload cameras
  if (id >= VC_PLBCAMFL && id <= VC_PLBCAMBR) {
    // Pan and tilt from camera control not from alt + arrow but from the dialog
    SetCameraRotationRange(0,0,0,0);
    // No lean for payload camera
    SetCameraMovement (_V(0,0,0), 0, 0, _V(0,0,0), 0, 0, _V(0,0,0), 0, 0);

    // Refresh camera meshes and view positions
    SetAnimationCameras();
  }

  if (ok) {
    // register the HUDs (synced)
    oapiVCRegisterHUD (&huds);
    // register all MFD displays
    for (int i = 0; i < 10; i++)
      oapiRegisterMFD (MFD_LEFT+i, mfds+i);
    // update panels
    plop->UpdateVC();
    gop->UpdateVC();
	panela4->UpdateVC();
	panela8->UpdateVC();
	panelc2->UpdateVC();
	c3po->UpdateVC();
	panelf7->UpdateVC();
	panelo3->UpdateVC();
	r2d2->UpdateVC();
  }
  return ok;
}

// --------------------------------------------------------------
// Respond to virtual cockpit mouse event
// --------------------------------------------------------------
bool Atlantis::clbkVCMouseEvent (int id, int event, VECTOR3 &p)
{
  static bool counting = false;
  static double t0 = 0.0;

  //sprintf(oapiDebugString(), "VCMouseEvent: id %d event %d p %f %f %f",id,event,p.x,p.y,p.z);

  switch (id) 
  {
  // handle MFD selection buttons
  case AID_CDR1_BUTTONS:
  case AID_CDR2_BUTTONS:
  case AID_PLT1_BUTTONS:
  case AID_PLT2_BUTTONS:
  case AID_CRT1_BUTTONS:
  case AID_CRT2_BUTTONS:
  case AID_CRT3_BUTTONS:
  case AID_CRT4_BUTTONS:
  case AID_MFD1_BUTTONS:
  case AID_MFD2_BUTTONS:
  case AID_AFD_BUTTONS: 
	  {
    int mfd = id-AID_CDR1_BUTTONS+MFD_LEFT;
    int bt = (int)(p.x*5.99);
    if (bt < 5) oapiProcessMFDButton (mfd, bt, event);
    else {
      if (event & PANEL_MOUSE_LBDOWN) {
        t0 = oapiGetSysTime();
        counting = true;
      } else if ((event & PANEL_MOUSE_LBUP) && counting) {
        oapiSendMFDKey (mfd, OAPI_KEY_F2);
        counting = false;
      } else if ((event & PANEL_MOUSE_LBPRESSED) && counting && (oapiGetSysTime()-t0 >= 1.0)) {
        oapiSendMFDKey (mfd, OAPI_KEY_F1);
        counting = false;
      }
    }
	sprintf(oapiDebugString(), "MDU %d EDGE KEY %d", mfd, bt);
    } return true;

  // D. Beachy: handle power buttons
  case AID_CDR1_PWR:
  case AID_CDR2_PWR:
  case AID_PLT1_PWR:
  case AID_PLT2_PWR:
  case AID_CRT1_PWR:
  case AID_CRT2_PWR:
  case AID_CRT3_PWR:
  case AID_CRT4_PWR:
  case AID_MFD1_PWR:
  case AID_MFD2_PWR:
  case AID_AFD_PWR: 
	  {
		
        int mfd = id - AID_CDR1_PWR+MFD_LEFT;
		sprintf(oapiDebugString(), "POWER BUTTON %d", mfd);
        oapiSendMFDKey(mfd, OAPI_KEY_ESCAPE);
       } 
	  return true;

  // handle MFD brightness buttons
  case AID_CDR1_BRT:
  case AID_CDR2_BRT:
  case AID_PLT1_BRT:
  case AID_PLT2_BRT:
  case AID_CRT1_BRT:
  case AID_CRT2_BRT:
  case AID_CRT3_BRT:
  case AID_CRT4_BRT:
  case AID_MFD1_BRT:
  case AID_MFD2_BRT:
  case AID_AFD_BRT: 
	  {
		static double t0, brt0;
		static bool up;
		int mfd = id-AID_CDR1_BRT;
		sprintf(oapiDebugString(), "BRT BUTTON %d", mfd);

		/*
		if (event & PANEL_MOUSE_LBDOWN) {
			up = (p.x >= 0.5);
			t0 = oapiGetSysTime();
			brt0 = mfdbright[mfd];
		} else if (event & PANEL_MOUSE_LBPRESSED) 
		{
			double dt = oapiGetSysTime()-t0;
			double brt, dbrt = dt * 0.2;
			if (up) 
				brt = min (1.0, brt0 + dbrt);
			else    
				brt = max (0.25, brt0 - dbrt);
			mfdbright[mfd] = brt;
			if (vis) {
				MESHHANDLE hMesh = GetMesh (vis, mesh_vc);
				MATERIAL *mat = oapiMeshMaterial (hMesh, 10+mfd);
				mat->emissive.r = mat->emissive.g = mat->emissive.b = (float)brt;
			}
		}
		*/
	} 
	return false;
  // handle panel R13L events (payload bay operations)
  case AID_R13L:
    return plop->VCMouseEvent (id, event, p);
  case AID_A4:
	return panela4->VCMouseEvent (id, event, p);
  case AID_A8:
	return panela8->VCMouseEvent (id, event, p);
  case AID_F6:
    return gop->VCMouseEvent (id, event, p);
  case AID_F7:
	return panelf7->VCMouseEvent(id, event, p);
  case AID_C2:
	return panelc2->VCMouseEvent(id, event, p);
  case AID_C3:
	return c3po->VCMouseEvent (id, event, p);
  case AID_O3:
	return panelo3->VCMouseEvent(id, event, p);
  case AID_KYBD_CDR:
	//sprintf(oapiDebugString(), "AID_KYBD_CDR event");
    return CDRKeyboard->VCMouseEvent(id, event, p);
  case AID_KYBD_PLT:
    return PLTKeyboard->VCMouseEvent(id, event, p);
  case AID_R2:
	return r2d2->VCMouseEvent (id, event, p);
  }
  return false;
}

// --------------------------------------------------------------
// Respond to virtual cockpit area redraw request
// --------------------------------------------------------------
bool Atlantis::clbkVCRedrawEvent (int id, int event, SURFHANDLE surf)
{
  switch (id) {
  case AID_CDR1_BUTTONS:
  case AID_CDR2_BUTTONS:
  case AID_PLT1_BUTTONS:
  case AID_PLT2_BUTTONS:
  case AID_CRT1_BUTTONS:
  case AID_CRT2_BUTTONS:
  case AID_CRT3_BUTTONS:
  case AID_CRT4_BUTTONS:
  case AID_MFD1_BUTTONS:
  case AID_MFD2_BUTTONS:
  case AID_AFD_BUTTONS:
   {
    int mfd = id-AID_CDR1_BUTTONS+MFD_LEFT;
    RedrawPanel_MFDButton (surf, mfd);
    } return true;
  default:
	if (id >= AID_A4_MIN && id <= AID_A4_MAX)
	  return panela4->VCRedrawEvent (id, event, surf);
	if (id >= AID_A8_MIN && id <= AID_A8_MAX)
	  return panela8->VCRedrawEvent (id, event, surf);
	if (id >= AID_R2_MIN && id <= AID_R2_MAX)
	  return r2d2->VCRedrawEvent (id, event, surf);
    if (id >= AID_R13L_MIN && id <= AID_R13L_MAX)
      return plop->VCRedrawEvent (id, event, surf);
    if (id >= AID_F6_MIN && id <= AID_F6_MAX)
      return gop->VCRedrawEvent (id, event, surf);
	if (id >= AID_F7_MIN && id <= AID_F7_MAX)
      return panelf7->VCRedrawEvent (id, event, surf);
	if (id >= AID_C3_MIN && id <= AID_C3_MAX)
		return c3po->VCRedrawEvent (id, event, surf);
	if (id >= AID_C2_MIN && id <= AID_C2_MAX)
      return panelc2->VCRedrawEvent (id, event, surf);
	if (id >= AID_O3_MIN && id <= AID_O3_MAX)
      return panelo3->VCRedrawEvent (id, event, surf);
	
	
    break;
  }
  return false;
}

void Atlantis::UpdateRMSPositions()
{
	CalcAnimationFKArm(arm_ee_pos, arm_ee_dir);
	arm_ee_rot=arm_tip[2]-arm_tip[0];
	arm_ee_rot=_V(-arm_ee_rot.z, arm_ee_rot.x, arm_ee_rot.y);
	//sprintf(oapiDebugString(), "%f %f %f %f %f %f", arm_ee_pos.x, arm_ee_pos.y, arm_ee_pos.z,
		//arm_ee_dir.x, arm_ee_dir.y, arm_ee_dir.z);
}

void Atlantis::CalcAnimationFKArm(VECTOR3 &pos, VECTOR3 &dir)
{
	double current_phi_s=linterp(0,shoulder_min,1,shoulder_max,arm_sp);
	double current_phi_e=linterp(0,elbow_min,1,elbow_max,arm_ep);
	double current_phi_w=linterp(0, wrist_min, 1, wrist_max, arm_wp);
	double current_beta_s=linterp(0,-180,1,180,arm_sy);
	double current_beta_w=linterp(0, wrist_yaw_min, 1, wrist_yaw_max, arm_wy);

	VECTOR3 temp=RotateVectorZ(_V(1.0, 0.0, 0.0), current_beta_w+current_beta_s);
	dir=RotateVectorY(temp, current_phi_w-current_phi_e+current_phi_s);

	//sprintf(oapiDebugString(), "%f %f %f %f %f %f", temp.x, temp.y, temp.z, dir.x, dir.y, dir.z);
	//sprintf(oapiDebugString(), "%f %f %f %f", current_beta_w, current_beta_s, arm_wy, arm_sy);

	temp=RotateVectorY(_V(lu, 0.0, 0.0), current_phi_s-sp_null)+RotateVectorY(_V(ll, 0.0, 0.0), current_phi_s-current_phi_e+ep_null)
		+RotateVectorY(_V(wp_wy, 0.0, 0.0), current_phi_w-current_phi_e+current_phi_s);
	pos=RotateVectorZ(temp, current_beta_s)+dir*wy_ee;
}

VECTOR3 Atlantis::CalcAnimationFKArm() {
  //Do forward kinematics to get the current position of the wrist joint
  //Return value is in MPM ref. frame
  double current_phi_s=linterp(0,shoulder_min,1,shoulder_max,arm_sp);
  double current_phi_e=linterp(0,elbow_min,1,elbow_max,arm_ep);
  double current_beta_s=linterp(0,-180,1,180,arm_sy);
  double current_phi_l=current_phi_s-current_phi_e;
  double rho_e=lu*cos(RAD*current_phi_s);
  double z_e=lu*sin(RAD*current_phi_s);
  double rho_w=rho_e+ll*cos(RAD*current_phi_l);
  double z_w=z_e+ll*sin(RAD*current_phi_l);
  double x_w=rho_w*cos(RAD*current_beta_s);
  double y_w=rho_w*sin(RAD*current_beta_s);
  return _V(x_w,y_w,z_w);
}

void Atlantis::SetAnimationIKArm(VECTOR3 arm_dpos) {
  if(!RMS) return;
  if(RMSRollout.action!=AnimState::OPEN || !Eq(shoulder_brace, 0.0) || !Eq(MRL[0], 0.0)) return;
  
  //VECTOR3 rot=arm_tip[2]-arm_tip[0];
  VECTOR3 rot=arm_ee_rot;

  //sprintf(oapiDebugString(), "%f %f %f %f %f %f", arm_ee_pos.x, arm_ee_pos.y, arm_ee_pos.z,
	  //arm_ee_dir.x, arm_ee_dir.y, arm_ee_dir.z);

  //Candidate position. Calculate the joints on it...
  VECTOR3 arm_cpos=arm_ee_pos+RotateVectorX(arm_dpos, 18.435);
  //VECTOR3 arm_cpos=arm_ee_pos+arm_dpos;
  VECTOR3 arm_wrist_yaw_cpos=arm_cpos-arm_ee_dir*wy_ee;
  //VECTOR3 dpos=-dir*wy_ee;
  //sprintf(oapiDebugString(), "%f %f %f", dpos.x, dpos.y, dpos.z);
  //sprintf(oapiDebugString(), "%f %f %f %f %f %f", pos.x, pos.y, pos.z, posOrig.x, posOrig.y, posOrig.z);

  //get wrist_pitch boom direction
  VECTOR3 wrist_rot=RotateVectorX(rot, wr_angle);
  double phi=DEG*asin(wrist_rot.x/length(wrist_rot));
  double beta=DEG*asin(wrist_rot.y/length(wrist_rot));
  //wrist_rot=RotateVectorY(wrist_rot, phi);
  //wrist_rot=RotateVectorX(wrist_rot, -beta);
  //sprintf(oapiDebugString(), "%f %f %f", wrist_rot.x, wrist_rot.y, wrist_rot.z);
  //VECTOR3 boom_plane = RotateVectorY(Normalize(arm_wrist_yaw_cpos), phi);
  VECTOR3 boom_plane = Normalize(arm_wrist_yaw_cpos);
  //boom_plane=RotateVectorZ(boom_plane, -beta);
  //boom_plane=RotateVectorX(boom_plane, -beta);
  VECTOR3 wrist_pitch_dir = RotateVectorY(arm_ee_dir, phi);
  wrist_pitch_dir=RotateVectorX(wrist_pitch_dir, -beta);
  //sprintf(oapiDebugString(), "%f %f", length(boom_plane), length(wrist_pitch_dir));
  double beta_w=DEG*(asin(boom_plane.y)-asin(arm_ee_dir.y));
  //sprintf(oapiDebugString(), "%f %f %f", phi, beta, beta_w);
  wrist_pitch_dir=RotateVectorZ(wrist_pitch_dir, beta_w);
  //wrist_pitch_dir=_V(boom_plane.x, boom_plane.y, wrist_pitch_dir.z);
  //sprintf(oapiDebugString(), "%f %f", length(boom_plane), length(wrist_pitch_dir));
  wrist_pitch_dir=RotateVectorX(wrist_pitch_dir, beta);
  wrist_pitch_dir=RotateVectorY(wrist_pitch_dir, -phi)*wp_wy;
  //wrist_pitch_dir=RotateVectorX(wrist_pitch_dir, beta)*wp_wy;
  sprintf(oapiDebugString(), "%f %f %f %f %f %f %f %f", wrist_pitch_dir.x, wrist_pitch_dir.y, wrist_pitch_dir.z,
	  phi, beta, wrist_rot.x, wrist_rot.y, wrist_rot.z);

  VECTOR3 arm_wrist_cpos=arm_wrist_yaw_cpos-wrist_pitch_dir;
  //sprintf(oapiDebugString(), "%f %f %f", arm_cpos.x, arm_cpos.y, arm_cpos.z);
  //VECTOR3 temp=CalcAnimationFKArm();
  //sprintf(oapiDebugString(), "%f %f %f %f %f %f", arm_wrist_cpos.x, arm_wrist_cpos.y, arm_wrist_cpos.z, temp.x, temp.y, temp.z);
  //VECTOR3 temp=RotateVectorX(arm_wrist_dpos, 18.435);
  double r=length(arm_wrist_cpos);
  double beta_s=DEG*atan2(arm_wrist_cpos.y,arm_wrist_cpos.x);
  double rho=sqrt(arm_wrist_cpos.x*arm_wrist_cpos.x+arm_wrist_cpos.y*arm_wrist_cpos.y);
  double cos_phibar_e=(r*r-lu*lu-ll*ll)/(-2*lu*ll);
  if (fabs(cos_phibar_e)>1) return;//Can't reach new point with the elbow
  double phi_e=180-DEG*acos(cos_phibar_e)+ep_null+sp_null;
  double cos_phi_s2=(ll*ll-lu*lu-r*r)/(-2*lu*r);
  if(fabs(cos_phi_s2)>1) return; //Can't reach with shoulder
  double phi_s=DEG*(atan2(arm_wrist_cpos.z,rho)+acos(cos_phi_s2))+sp_null;
  double anim_phi_s=linterp(shoulder_min,0,shoulder_max,1,phi_s);
  double anim_phi_e=linterp(elbow_min,0,elbow_max,1,phi_e);
  double anim_beta_s=linterp(-180,0,180,1,beta_s);
  if(anim_beta_s<0 || anim_beta_s>1) return;
  if(anim_phi_s<0 || anim_phi_s>1) return;
  if(anim_phi_e<0 || anim_phi_e>1) return;
  //but only keep it and set the joints if no constraints are violated.
  double phi_w=DEG*asin(wrist_pitch_dir.z/length(wrist_pitch_dir))+phi_e-phi_s;
  double anim_phi_w=linterp(wrist_min,0,wrist_max,1,phi_w);
  double anim_beta_w=linterp(wrist_yaw_min,0,wrist_yaw_max,1,beta_w);
  //sprintf(oapiDebugString(), "%f %f %f %f", anim_phi_w, arm_wp, anim_beta_w, arm_wy);
  //VECTOR3 temp=Normalize(pos-CalcAnimationFKArm());
  //VECTOR3 temp=pos-arm_wrist_cpos;
  //VECTOR3 temp2=dir;
  //sprintf(oapiDebugString(), "%f %f %f %f %f %f %f %f", temp.x, temp.y, temp.z, temp2.x, temp2.y, temp2.z, length(temp), wp_wy+wy_ee);
  //sprintf(oapiDebugString(), "%f", DEG*asin(wrist_pitch_dir.z/length(wrist_pitch_dir)));

  arm_sy=anim_beta_s;
  SetAnimationArm(anim_arm_sy,arm_sy);
  arm_sp=anim_phi_s;
  SetAnimationArm(anim_arm_sp,arm_sp);
  arm_ep=anim_phi_e;
  SetAnimationArm(anim_arm_ep,arm_ep);
  arm_wp=anim_phi_w;
  SetAnimationArm(anim_arm_wp,arm_wp);
  arm_wy=anim_beta_w;
  SetAnimationArm(anim_arm_wy,arm_wy);
  arm_ee_pos=arm_cpos;

  //sprintf(oapiDebugString(), "%f %f %f %f", arm_ee_dir.x, arm_ee_dir.y, arm_ee_dir.z, beta_w);
  //CalcAnimationFKArm(pos, dir);
  //sprintf(oapiDebugString(), "%f %f %f %f %f %f", pos.x, pos.y, pos.z, arm_cpos.x, arm_cpos.y, arm_cpos.z);
}

// --------------------------------------------------------------
// Keyboard interface handler (buffered key events)
// --------------------------------------------------------------
int Atlantis::clbkConsumeBufferedKey (DWORD key, bool down, char *kstate)
{
  if (!down) return 0; // only process keydown events

  if(pCommModeHandler->IsInCommMode())
	  return pCommModeHandler->ConsumeBufferedKey(key, down, kstate);

    if (KEYMOD_CONTROL (kstate)) {
	switch (key) {
    case OAPI_KEY_SPACE: // open RMS control dialog
      oapiOpenDialogEx (g_Param.hDLL, IDD_CTRL, Atlantis_DlgProc, DLG_CAPTIONCLOSE, this);
      return 1;
    case OAPI_KEY_B: // deploy/retract speedbrake
      if (!Playback()) RevertSpeedbrake ();
      return 1;
	case OAPI_KEY_G:
		gop->ArmGear();
		return 1;
	case OAPI_KEY_X: //temporary
		if(RMS && !Playback() && plop->MechPwr[0]==PayloadBayOp::MP_ON && plop->MechPwr[1]==PayloadBayOp::MP_ON && ArmCradled() && plop->BayDoorStatus.pos==1.0 ) {
			if(RMSRollout.action==AnimState::CLOSED) {
				RMSRollout.action=AnimState::OPENING;
			}
			else {
				RMSRollout.action=AnimState::CLOSING;
			}
		}
		return 1;
	case OAPI_KEY_1: //temporary
		if(DisplayJointAngles) DisplayJointAngles=false;
		else DisplayJointAngles=true;
		return 1;
	case OAPI_KEY_2:
		FireAllNextManifold();
		return 1;
	}
  } else { // unmodified keys
    switch (key) {
	case OAPI_KEY_TAB:
		pCommModeHandler->EnterCommMode();
		break;
	case OAPI_KEY_B:
	  bAutopilot=false;
	  return 1;
	case OAPI_KEY_C:
	  if(bThrottle) bThrottle=false;
	  else bThrottle=true;
	  return 1;
    case OAPI_KEY_J:  // "Jettison"
      if (!Playback()) bManualSeparate = true;
      return 1;
    case OAPI_KEY_8:
      ToggleGrapple();
      return 1;
    case OAPI_KEY_9:
      center_arm = true;
      return 1;
    case OAPI_KEY_E:
      do_eva = true;
      return 1;
	case OAPI_KEY_COMMA:
		if(!Playback() && r2d2->bHydraulicPressure) SetSpeedbrake(min(1.0, spdb_tgt+0.05));
		return 1;
	case OAPI_KEY_PERIOD:
		if(!Playback() && r2d2->bHydraulicPressure) SetSpeedbrake(max(0.0, spdb_tgt-0.05));
		return 1;
	case OAPI_KEY_G:
		gop->RevertLandingGear();
		return 1;
    }
  }
  return 0;
}

// ==============================================================
// API callback interface
// ==============================================================

// --------------------------------------------------------------
// Module initialisation
// --------------------------------------------------------------
DLLCLBK void InitModule (HINSTANCE hModule)
{
  g_Param.hDLL = hModule;
  oapiRegisterCustomControls (hModule);
  g_Param.tkbk_label = oapiCreateSurface (LOADBMP (IDB_TKBKLABEL));
  g_Param.clock_digits = oapiCreateSurface (LOADBMP (IDB_CLOCKDIGITS));
  if(g_Param.clock_digits == NULL) {
	  oapiWriteLog("Loading bitmap \"CLOCK_DIGITS\" failed.");
  }
  g_Param.digits_7seg = oapiCreateSurface (LOADBMP (IDB_7SEGDIGITS));
  if(g_Param.digits_7seg == NULL) {
	  oapiWriteLog("Loading bitmap \"DIGITS_7SEG\" failed.");
  }

  // allocate GDI resources
  g_Param.font[0] = CreateFont (-11, 0, 0, 0, 400, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
}

DLLCLBK void ExitModule (HINSTANCE hModule)
{
  oapiUnregisterCustomControls (hModule);
  if(g_Param.tkbk_label)
  {
	oapiDestroySurface (g_Param.tkbk_label);
  }
  if(g_Param.clock_digits)
  {
	  oapiDestroySurface (g_Param.clock_digits);
  }
  if(g_Param.digits_7seg)
  {
	oapiDestroySurface (g_Param.digits_7seg);
  }

  // deallocate GDI resources
  DeleteObject (g_Param.font[0]);
}

// --------------------------------------------------------------
// Vessel initialisation
// --------------------------------------------------------------
DLLCLBK VESSEL *ovcInit (OBJHANDLE hvessel, int flightmodel)
{
  return new Atlantis (hvessel, flightmodel);
}

// --------------------------------------------------------------
// Vessel cleanup
// --------------------------------------------------------------
DLLCLBK void ovcExit (VESSEL *vessel)
{
  if (vessel) delete (Atlantis*)vessel;
}

// ==============================================================
// Message callback function for Atlantis control dialog box
// ==============================================================

BOOL CALLBACK Atlantis_DlgProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  Atlantis *sts = (uMsg == WM_INITDIALOG ? (Atlantis*)lParam : (Atlantis*)oapiGetDialogContext (hWnd));
  // pointer to vessel instance was passed as dialog context

  switch (uMsg) {
  case WM_COMMAND:
    switch (LOWORD(wParam)) {
    case IDCANCEL:
      oapiCloseDialog (hWnd);
      return TRUE;
    case IDC_PLBAYOP:
      sts->plop->OpenDialog ();
      break;
    case IDC_RMSOP:
      oapiOpenDialogEx (g_Param.hDLL, IDD_RMS, RMS_DlgProc, DLG_CAPTIONCLOSE, sts);
      break;
  case IDC_PAYCAM:
      oapiOpenDialogEx (g_Param.hDLL, IDD_PAYLOADCAM, PAYCAM_DlgProc, DLG_CAPTIONCLOSE, sts);
      break;
    }
    break;
  }
  return oapiDefDialogProc (hWnd, uMsg, wParam, lParam);
}

// ==============================================================
// Message callback function for RMS control dialog box
// ==============================================================

BOOL CALLBACK RMS_DlgProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  Atlantis *sts = (uMsg == WM_INITDIALOG ? (Atlantis*)lParam : (Atlantis*)oapiGetDialogContext (hWnd));
  // pointer to vessel instance was passed as dialog context

  const double step = 0.05*RAD;
  static double t0;
  double t1;
  HICON hIcon;

  switch (uMsg) {
  case WM_INITDIALOG:
    hIcon = LoadIcon (g_Param.hDLL, MAKEINTRESOURCE(IDI_UP));
    SendDlgItemMessage (hWnd, IDC_WRIST_PITCHUP, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
    SendDlgItemMessage (hWnd, IDC_ELBOW_PITCHUP, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
    SendDlgItemMessage (hWnd, IDC_SHOULDER_PITCHUP, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
    hIcon = LoadIcon (g_Param.hDLL, MAKEINTRESOURCE(IDI_DOWN));
    SendDlgItemMessage (hWnd, IDC_WRIST_PITCHDOWN, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
    SendDlgItemMessage (hWnd, IDC_ELBOW_PITCHDOWN, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
    SendDlgItemMessage (hWnd, IDC_SHOULDER_PITCHDOWN, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
    hIcon = LoadIcon (g_Param.hDLL, MAKEINTRESOURCE(IDI_LEFT));
    SendDlgItemMessage (hWnd, IDC_WRIST_YAWLEFT, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
    SendDlgItemMessage (hWnd, IDC_SHOULDER_YAWLEFT, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
    hIcon = LoadIcon (g_Param.hDLL, MAKEINTRESOURCE(IDI_RIGHT));
    SendDlgItemMessage (hWnd, IDC_WRIST_YAWRIGHT, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
    SendDlgItemMessage (hWnd, IDC_SHOULDER_YAWRIGHT, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
    hIcon = LoadIcon (g_Param.hDLL, MAKEINTRESOURCE(IDI_RRIGHT));
    SendDlgItemMessage (hWnd, IDC_WRIST_ROLLRIGHT, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
    hIcon = LoadIcon (g_Param.hDLL, MAKEINTRESOURCE(IDI_RLEFT));
    SendDlgItemMessage (hWnd, IDC_WRIST_ROLLLEFT, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
    SendDlgItemMessage (hWnd, IDC_SHOWGRAPPLE, BM_SETCHECK, oapiGetShowGrapplePoints() ? BST_CHECKED:BST_UNCHECKED, 0);
    SetWindowText (GetDlgItem (hWnd, IDC_GRAPPLE), sts->SatGrappled() ? "Release" : "Grapple");
    EnableWindow (GetDlgItem (hWnd, IDC_STOW), sts->SatGrappled() ? FALSE : TRUE);
    SetWindowText (GetDlgItem (hWnd, IDC_PAYLOAD), sts->SatStowed() ? "Purge" : "Arrest");
    EnableWindow (GetDlgItem (hWnd, IDC_PAYLOAD), sts->SatStowed() || sts->CanArrest() ? TRUE:FALSE);
    SetTimer (hWnd, 1, 50, NULL);
    t0 = oapiGetSimTime();
    return FALSE;
  case WM_DESTROY:
    KillTimer (hWnd, 1);
    return 0;
  case WM_TIMER:
    if (wParam == 1) {
      t1 = oapiGetSimTime();
	  if(sts->RMSRollout.action!=AnimState::OPEN || sts->shoulder_brace!=0.0 || sts->MRL[0]!=0.0) break;
      if (SendDlgItemMessage (hWnd, IDC_SHOULDER_YAWLEFT, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->arm_sy = min (1.0, sts->arm_sy + (t1-t0)*ARM_OPERATING_SPEED);
        sts->SetAnimationArm (sts->anim_arm_sy, sts->arm_sy);
		sts->UpdateRMSPositions();
      } else if (SendDlgItemMessage (hWnd, IDC_SHOULDER_YAWRIGHT, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->arm_sy = max (0.0, sts->arm_sy - (t1-t0)*ARM_OPERATING_SPEED);
        sts->SetAnimationArm (sts->anim_arm_sy, sts->arm_sy);
		sts->UpdateRMSPositions();
      } else if (SendDlgItemMessage (hWnd, IDC_SHOULDER_PITCHUP, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->arm_sp = min (1.0, sts->arm_sp + (t1-t0)*ARM_OPERATING_SPEED);
        sts->SetAnimationArm (sts->anim_arm_sp, sts->arm_sp);
		sts->UpdateRMSPositions();
      } else if (SendDlgItemMessage (hWnd, IDC_SHOULDER_PITCHDOWN, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->arm_sp = max (0.0, sts->arm_sp - (t1-t0)*ARM_OPERATING_SPEED);
        sts->SetAnimationArm (sts->anim_arm_sp, sts->arm_sp);
		sts->UpdateRMSPositions();
      } else if (SendDlgItemMessage (hWnd, IDC_ELBOW_PITCHUP, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->arm_ep = max (0.0, sts->arm_ep - (t1-t0)*ARM_OPERATING_SPEED);
        sts->SetAnimationArm (sts->anim_arm_ep, sts->arm_ep);
		sts->UpdateRMSPositions();
      } else if (SendDlgItemMessage (hWnd, IDC_ELBOW_PITCHDOWN, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->arm_ep = min (1.0, sts->arm_ep + (t1-t0)*ARM_OPERATING_SPEED);
        sts->SetAnimationArm (sts->anim_arm_ep, sts->arm_ep);
		sts->UpdateRMSPositions();
      } else if (SendDlgItemMessage (hWnd, IDC_WRIST_PITCHUP, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->arm_wp = min (1.0, sts->arm_wp + (t1-t0)*ARM_OPERATING_SPEED);
        sts->SetAnimationArm (sts->anim_arm_wp, sts->arm_wp);
		sts->UpdateRMSPositions();
      } else if (SendDlgItemMessage (hWnd, IDC_WRIST_PITCHDOWN, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->arm_wp = max (0.0, sts->arm_wp - (t1-t0)*ARM_OPERATING_SPEED);
        sts->SetAnimationArm (sts->anim_arm_wp, sts->arm_wp);
		sts->UpdateRMSPositions();
      } else if (SendDlgItemMessage (hWnd, IDC_WRIST_YAWLEFT, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->arm_wy = min (1.0, sts->arm_wy + (t1-t0)*ARM_OPERATING_SPEED);
        sts->SetAnimationArm (sts->anim_arm_wy, sts->arm_wy);
		sts->UpdateRMSPositions();
      } else if (SendDlgItemMessage (hWnd, IDC_WRIST_YAWRIGHT, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->arm_wy = max (0.0, sts->arm_wy - (t1-t0)*ARM_OPERATING_SPEED);
        sts->SetAnimationArm (sts->anim_arm_wy, sts->arm_wy);
		sts->UpdateRMSPositions();
      } else if (SendDlgItemMessage (hWnd, IDC_WRIST_ROLLLEFT, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->arm_wr = max (0.0, sts->arm_wr - (t1-t0)*ARM_OPERATING_SPEED);
        sts->SetAnimationArm (sts->anim_arm_wr, sts->arm_wr);
		sts->UpdateRMSPositions();
      } else if (SendDlgItemMessage (hWnd, IDC_WRIST_ROLLRIGHT, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->arm_wr = min (1.0, sts->arm_wr + (t1-t0)*ARM_OPERATING_SPEED);
        sts->SetAnimationArm (sts->anim_arm_wr, sts->arm_wr);
		sts->UpdateRMSPositions();
      } else if (SendDlgItemMessage (hWnd, IDC_TRANS_PX, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->SetAnimationIKArm (_V(+1,0,0)*(t1-t0)*ARM_TRANSLATE_SPEED);
      } else if (SendDlgItemMessage (hWnd, IDC_TRANS_PY, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->SetAnimationIKArm (_V(0,+1,0)*(t1-t0)*ARM_TRANSLATE_SPEED);
      } else if (SendDlgItemMessage (hWnd, IDC_TRANS_PZ, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->SetAnimationIKArm (_V(0,0,+1)*(t1-t0)*ARM_TRANSLATE_SPEED);
      } else if (SendDlgItemMessage (hWnd, IDC_TRANS_MX, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->SetAnimationIKArm (_V(-1,0,0)*(t1-t0)*ARM_TRANSLATE_SPEED);
      } else if (SendDlgItemMessage (hWnd, IDC_TRANS_MY, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->SetAnimationIKArm (_V(0,-1,0)*(t1-t0)*ARM_TRANSLATE_SPEED);
      } else if (SendDlgItemMessage (hWnd, IDC_TRANS_MZ, BM_GETSTATE, 0, 0) & BST_PUSHED) {
        sts->SetAnimationIKArm (_V(0,0,-1)*(t1-t0)*ARM_TRANSLATE_SPEED);
    }
      t0 = t1;
    }
    if (!sts->center_arm) EnableWindow (GetDlgItem (hWnd, IDC_GRAPPLE), TRUE);
    break;
  case WM_COMMAND:
    switch (LOWORD(wParam)) {
    case IDCANCEL:
      oapiCloseDialog (hWnd);
      return TRUE;
    case IDC_STOW:
      if (sts->center_arm = !sts->center_arm) {
        sts->center_arm_t = oapiGetSimTime();
        EnableWindow (GetDlgItem (hWnd, IDC_GRAPPLE), FALSE);
      }
      return 0;
    case IDC_GRAPPLE:
      sts->ToggleGrapple();
      return 0;
    case IDC_PAYLOAD:
      sts->ToggleArrest();
      return 0;
    case IDC_SHOWGRAPPLE:
      oapiSetShowGrapplePoints (SendDlgItemMessage (hWnd, IDC_SHOWGRAPPLE, BM_GETCHECK, 0, 0) == BST_CHECKED ? true : false);
      return 0;
    }
    break;
  }
  return oapiDefDialogProc (hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK PAYCAM_DlgProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

  Atlantis *sts = (uMsg == WM_INITDIALOG ? (Atlantis*)lParam : (Atlantis*)oapiGetDialogContext (hWnd));

  static double t0;
  double t1;

  double *camYaw;
  double *camPitch;
  double rate = CAM_HIGHRATE_SPEED;

  switch (uMsg) {
    case WM_INITDIALOG:
    // Get Camera rate from atlantis and set the dialog
    if (!sts->cameraLowRate) {
      SendDlgItemMessage (hWnd, IDC_CAM_HIGHRATE, BM_SETCHECK, BST_CHECKED, 0);
    } else {
      SendDlgItemMessage (hWnd, IDC_CAM_LOWRATE, BM_SETCHECK, BST_CHECKED, 0);
    }

    // Get camera selected from atlantis and set the dialog
    switch (sts->cameraControl) {
      case 0:
      SendDlgItemMessage (hWnd, IDC_CAM_FL, BM_SETCHECK, BST_CHECKED, 0);
      break;
      case 1:
      SendDlgItemMessage (hWnd, IDC_CAM_FR, BM_SETCHECK, BST_CHECKED, 0);
      break;
      case 2:
      SendDlgItemMessage (hWnd, IDC_CAM_BL, BM_SETCHECK, BST_CHECKED, 0);
      break;
      case 3:
      SendDlgItemMessage (hWnd, IDC_CAM_BR, BM_SETCHECK, BST_CHECKED, 0);
      break;

    }

    SetTimer (hWnd, 1, 50, NULL);
      t0 = oapiGetSimTime();
    return FALSE;
    break;

    case WM_TIMER:
      if (wParam == 1) {
        t1 = oapiGetSimTime();

    // Set Atlantis camera selected from dialog
    if (SendDlgItemMessage (hWnd, IDC_CAM_FL, BM_GETCHECK, 0, 0) == BST_CHECKED) {
      sts->cameraControl = 0;
      camYaw =& sts->camFLyaw;
      camPitch =& sts->camFLpitch;
    }
    if (SendDlgItemMessage (hWnd, IDC_CAM_FR, BM_GETCHECK, 0, 0) == BST_CHECKED) {
      sts->cameraControl = 1;
      camYaw =& sts->camFRyaw;
      camPitch =& sts->camFRpitch;
    }
    if (SendDlgItemMessage (hWnd, IDC_CAM_BL, BM_GETCHECK, 0, 0) == BST_CHECKED) {
      sts->cameraControl = 2;
      camYaw =& sts->camBLyaw;
      camPitch =& sts->camBLpitch;
    }
    if (SendDlgItemMessage (hWnd, IDC_CAM_BR, BM_GETCHECK, 0, 0) == BST_CHECKED) {
      sts->cameraControl = 3;
      camYaw =& sts->camBRyaw;
      camPitch =& sts->camBRpitch;
    }

    // Set Atlantis camera Rate from dialog
    if (SendDlgItemMessage (hWnd, IDC_CAM_LOWRATE, BM_GETCHECK, 0, 0) == BST_CHECKED) {
      sts->cameraLowRate = true;
      rate = CAM_LOWRATE_SPEED;
    } else {
      sts->cameraLowRate = false;
      rate = CAM_HIGHRATE_SPEED;
    }

    // Sets the current selected camera orientation and flag camera moved;
        if (SendDlgItemMessage (hWnd, IDC_CAM_LEFT, BM_GETSTATE, 0, 0) & BST_PUSHED) {
          *camYaw = max(-170, *camYaw - (t1-t0)*rate);
      sts->cameraMoved = true;
    } else if (SendDlgItemMessage (hWnd, IDC_CAM_RIGHT, BM_GETSTATE, 0, 0) & BST_PUSHED) {
      *camYaw = min(170, *camYaw + (t1-t0)*rate);
      sts->cameraMoved = true;
        } else if (SendDlgItemMessage (hWnd, IDC_CAM_UP, BM_GETSTATE, 0, 0) & BST_PUSHED) {
      *camPitch = min(170, *camPitch + (t1-t0)*rate);
      sts->cameraMoved = true;
    } else if (SendDlgItemMessage (hWnd, IDC_CAM_DOWN, BM_GETSTATE, 0, 0) & BST_PUSHED) {
      *camPitch = max(-170, *camPitch - (t1-t0)*rate);
      sts->cameraMoved = true;
    }

    t0 = t1;
      }

    case WM_COMMAND:
      if (LOWORD(wParam) == IDCANCEL) {
        oapiCloseDialog (hWnd);
        return TRUE;
    }
      break;
  }
  // pointer to vessel instance was passed as dialog context
  return oapiDefDialogProc (hWnd, uMsg, wParam, lParam);

}

DLLCLBK bool gpcReadValue(VESSEL* pVessel, UINT gpc, UINT val_index, DWORD* value)
{
	return false;
}

DLLCLBK bool gpcSetValue(VESSEL* pVessel, UINT gpc, UINT val_index, const DWORD value)
{
	return false;
}

bool Atlantis::GetLiftOffFlag() const
{
	return bLiftOff;
}

void Atlantis::TriggerLiftOff()
{
	bLiftOff = true;
	pMTU->StartMET();
}

short Atlantis::GetSRBChamberPressure(unsigned short which_srb)
{
	if(which_srb < 2 && status < 2)
	{
		return (short)(1000 * GetThrusterLevel(th_srb[which_srb]));
	}
	else
		return -1;
}

unsigned short Atlantis::GetGPCMET(unsigned short usGPCID, unsigned short &usDay, unsigned short &usHour, unsigned short &usMin, unsigned short &usSec)
{
	usDay = pMTU->GetMETDay(0);
	usHour = pMTU->GetMETHour(0);
	usMin = pMTU->GetMETMin(0);
	usSec = pMTU->GetMETSec(0);
	return 0;
}

short Atlantis::GetETPropellant(unsigned short usGPCID)
{
	if(status < 3)
	{
		return min((short)(100.0*GetPropellantMass(ph_tank)/TANK_MAX_PROPELLANT_MASS), 99);
	} else
	 return -1;
}

unsigned short Atlantis::GetGPCLVLHVel(unsigned short usGPCID, VECTOR3 &vel)
{
	VECTOR3 tmpVel;
	this->GetRelativeVel(GetGravityRef(), tmpVel);
	this->GetRelativePos(GetGravityRef(), LVLH_Z);
	
	double magVel = length(tmpVel);

	if(magVel != 0.0)
	{
		LVLH_X = tmpVel/length(tmpVel);
		LVLH_Z = -LVLH_Z/length(LVLH_Z);
	
		LVLH_Y = crossp(LVLH_X, LVLH_Z);

		vel.x = dotp(LVLH_X,tmpVel) * MPS2FPS;
		vel.y = dotp(LVLH_Y, tmpVel)* MPS2FPS;
		vel.z = dotp(LVLH_Z, tmpVel) * MPS2FPS;

		return 0;
	} else 
	{
		return 1;
	}
}

short Atlantis::GetGPCRefHDot(unsigned short usGPCID, double &fRefHDot)
{
	switch(ops)
	{
	case 102:
		if(met < 30.0)
		{
			fRefHDot = met * 21.33;
		} else if(met < 50.0)
		{
			fRefHDot = 640 + (met-30.0) * 16.7;
		} else if(met < 70.0)
		{
			fRefHDot = 974 + (met-50.0) * 21.65;
		} else if(met < 90)
		{
			fRefHDot = 1407 + (met-70.0) * 23.4;
		} else if (met < 110.0)
		{
			fRefHDot = 1875 + (met-90.0) * 15.6;
		} else {
			fRefHDot = 2187 + (met-110.0) * 15.6;
		}
		//TODO: Generate VSpeed Table.
		return VARSTATE_OK;
	}
	return VARSTATE_MISSING;
}

inline double angle(const VECTOR3 dir, const VECTOR3 dir0)
{
	return acos(dotp(dir, dir0)/
		(length(dir) * length(dir0)));
}

void Atlantis::UpdateSSMEGimbalAnimations()
{

	const double YAWS = 2 * sin(8.5 * RAD);
	const double PITCHS = 2 * sin(10 * RAD);
	
	VECTOR3 SSME_DIR;
	double fDeflYaw, fDeflPitch;

	GetThrusterDir(th_main[0], SSME_DIR);

	//fDeflYaw = 0.5+angle(SSME_DIR, SSMET_DIR0)/YAWS;

	//fDeflYaw = acos(SSME_DIR.x);
	//fDeflPitch = acos(SSME_DIR.y/sin(fDeflYaw));

	fDeflPitch = asin(-SSME_DIR.y);
	fDeflYaw = asin(SSME_DIR.x / cos(fDeflPitch));

	//sprintf(oapiDebugString(), "SSMET %f° %f° (%f, %f, %f)", fDeflPitch*DEG, fDeflYaw*DEG, SSME_DIR.x, SSME_DIR.y, SSME_DIR.z);


	//fDeflPitch = 0.5+acos((SSME_DIR.y * SSMET_DIR0.y + SSME_DIR.z * SSMET_DIR0.z)/
	//	(sqrt(pow(SSME_DIR.y,2)+pow(SSME_DIR.z, 2)) * sqrt(pow(SSMET_DIR0.y, 2) + pow(SSMET_DIR0.z, 2))))/PITCHS;
	SetAnimation(anim_ssmeTyaw, fDeflYaw/YAWS + 0.5);
	SetAnimation(anim_ssmeTpitch, (fDeflPitch - 16.0 *RAD)/PITCHS + 0.5);

	GetThrusterDir(th_main[1], SSME_DIR);

	fDeflPitch = asin(-SSME_DIR.y);
	fDeflYaw = asin(SSME_DIR.x / cos(fDeflPitch));

	SetAnimation(anim_ssmeLyaw, (fDeflYaw - 3.5 * RAD)/YAWS + 0.5);
	SetAnimation(anim_ssmeLpitch, (fDeflPitch - 10 * RAD)/PITCHS + 0.5);


	GetThrusterDir(th_main[2], SSME_DIR);

	fDeflPitch = asin(-SSME_DIR.y);
	fDeflYaw = asin(SSME_DIR.x / cos(fDeflPitch));

	SetAnimation(anim_ssmeRyaw, (fDeflYaw + 3.5 * RAD)/YAWS + 0.5);
	SetAnimation(anim_ssmeRpitch, (fDeflPitch - 10 * RAD)/PITCHS + 0.5);
	
}

void Atlantis::AddKUBandVisual(const VECTOR3 ofs)
{
	if (mesh_kuband == MESH_UNDEFINED && bHasKUBand) 
	{
		
		mesh_kuband = AddMesh(hKUBandMesh, &ofs);
		
		SetMeshVisibilityMode(mesh_kuband, MESHVIS_EXTERNAL|MESHVIS_VC|MESHVIS_EXTPASS);
		
	}

}

void Atlantis::DefineKUBandAnimations()
{
  UINT kidx = 4;
	  // ***** 3. Ku-band antenna animation *****
      // DaveS edit: Animations have now been realigned. Someone better doublecheck the gimbal and dish animations though.

  static UINT KuBand1Grp[1] = {GRP_KUBAND_BOX_KU};
  static MGROUP_ROTATE KuBand1 (kidx, KuBand1Grp, 1,
    _V(2.626,1.350,11.793), _V(0,1,0), (float)(-118*RAD));

  static UINT KuBand2Grp[1] = {GRP_KUGIMBAL_KU};
  static MGROUP_ROTATE KuBand2 (kidx, KuBand2Grp, 1,
    _V(2.549,1.878,10.469), _V(0,0,1), (float)(-360*RAD));


  static UINT KuBand3Grp[1] = {GRP_KUDISH_KU};
  static MGROUP_ROTATE KuBand3 (kidx, KuBand3Grp, 1,
    _V(2.549,1.874,10.280), _V(0,1,0), (float)(-162*RAD));

  anim_kubd = CreateAnimation (0);
  ANIMATIONCOMPONENT_HANDLE parent = AddAnimationComponent (anim_kubd, 0,     1, &KuBand1);

  anim_kualpha = CreateAnimation(0.0);
  parent = AddAnimationComponent (anim_kualpha, 0, 1, &KuBand2, parent);

  anim_kubeta = CreateAnimation(0.5);
  AddAnimationComponent (anim_kubeta, 0.0, 0.1, &KuBand3, parent);

}

void Atlantis::SignalGSEStart()
{
	if(ahHDP)
	{
		OBJHANDLE hMLP = GetAttachmentStatus(ahHDP);
		if(hMLP)
		{
			VESSEL* pV = oapiGetVesselInterface(hMLP);
			if(pV && !_stricmp(pV->GetClassName(), "Atlantis_MLP"))
			{
				static_cast<MLP*>(pV)->SignalGSEStart();
			}
		}
	}
}

void Atlantis::SignalGSEBreakHDP()
{

	if(ahHDP)
	{
		OBJHANDLE hMLP = GetAttachmentStatus(ahHDP);
		if(hMLP)
		{
			VESSEL* pV = oapiGetVesselInterface(hMLP);
			if(pV && !_stricmp(pV->GetClassName(), "Atlantis_MLP"))
			{
				static_cast<MLP*>(pV)->TriggerHDP();
			}
		}
	}
}

void Atlantis::DefineSSMEExhaust()
{
	int i;

	SURFHANDLE tex_main = oapiRegisterExhaustTexture ("Exhaust_atsme");
  	for(i = 0; i<3; i++)
	{
		if(ex_main[i])
		{
			DelExhaust(ex_main[i]);
		}
		ex_main[i] = AddExhaust(th_main[i], 30.0, 2.0, 5, tex_main);
	}
}

void Atlantis::SetKuGimbalAngles(double fAlpha, double fbeta)
{
	//No checking of subsystem state or latches, only animation
	if(bHasKUBand)
	{
		SetAnimation(anim_kualpha, fAlpha);
		SetAnimation(anim_kubeta, fbeta);
	}
}

double Atlantis::GetOMSPressure(OMS_REF oms_ref, unsigned short tank_id)
{
	return 50.0;
}

bool Atlantis::IsValidSPEC(int gpc, int spec)
{
	switch(ops/100)
	{
	case 0:
		switch(spec)
		{
		case 1:
		case 2:
		case 6:
			return true;
		}
		break;
	case 1:
		switch(spec)
		{
		case 0:
		case 1:
		case 6:
		case 18:
		case 23:
		case 50:
		case 51:
		case 53:
			return true;
		default:
			return false;
		}
		break;
	case 2:
		switch(spec)
		{
		case 0:
		case 1:
		case 2:
		case 6:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 25:
		case 33:
		case 34:
		case 55:
			return true;
		default:
			return false;
		}
		break;
	case 3:
		switch(spec)
		{
		case 0:
		case 1:
		case 6:
		case 18:
		case 21:
		case 22:
		case 23:
		case 50:
		case 51:
		case 53:
		case 55:
			return true;
		default:
			return false;
		}
		break;
	case 6:
		switch(spec)
		{
		case 0:
		case 1:
		case 6:
		case 18:
		case 23:
		case 50:
		case 51:
		case 53:
			return true;
		default:
			return false;
		}
		break;
	case 8:
		switch(spec)
		{
		case 0:
		case 1:
		case 2:
		case 6:
		case 18:
		case 19:
		case 23:
		case 40:
		case 41:
		case 42:
		case 43:
		case 44:
		case 45:
		case 55:
			return true;
		default:
			return false;
		}
	case 9:
		switch(spec)
		{
		case 0:
		case 1:
		case 2:
		case 6:
		case 55:
		case 62:
		case 100:
		case 101:
		case 102:
		case 104:
		case 105:
		case 106:
		case 112:
		case 113:
			return true;
		default:
			return false;
		}
		break;
	

	}
	return false;
}

void Atlantis::CreateOrbiterTanks()
{
	int i;
	if (!ph_oms)  ph_oms  = CreatePropellantResource (ORBITER_MAX_PROPELLANT_MASS); // OMS propellant
	for(i=0;i<3;i++) {
	  if(!apu_tank[i]) apu_tank[i]=CreatePropellantResource(APU_FUEL_TANK_MASS);
	}
	for(i=0;i<2;i++) {
	  if(!oms_helium_tank[i]) oms_helium_tank[i]=CreatePropellantResource(OMS_HELIUM_TANK_MASS);
	}
	if(!ph_frcs) 
		ph_frcs = CreatePropellantResource(ORBITER_FRCS_PROPELLANT_MASS);
	if(!ph_lrcs) 
		ph_lrcs = CreatePropellantResource(ORBITER_FRCS_PROPELLANT_MASS);
	if(!ph_rrcs) 
		ph_rrcs = CreatePropellantResource(ORBITER_FRCS_PROPELLANT_MASS);
}

void Atlantis::CreateFRCS(const VECTOR3 &ref_pos)
{
	//=================================================================================================
	// Downwards firing thrusters 
	//=================================================================================================
	if(thManFRCS1[RCS_F1D] == NULL) {
		thManFRCS1[RCS_F1D] = CreateThruster (ref_pos + RCS_F1D_OFS, RCS_F1D_DIR, ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, 
			ORBITER_RCS_ISP1);
		AddPrimaryRCSExhaust(thManFRCS1[RCS_F1D]);
	}
	if(thManFRCS2[RCS_F2D] == NULL) {
		thManFRCS2[RCS_F2D] = CreateThruster (ref_pos + RCS_F2D_OFS, RCS_F2D_DIR, ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, 
			ORBITER_RCS_ISP1);
		AddPrimaryRCSExhaust(thManFRCS2[RCS_F2D]);
	}
	if(thManFRCS3[RCS_F3D] == NULL)	{
		thManFRCS3[RCS_F3D] = CreateThruster (ref_pos + RCS_F3D_OFS, RCS_F3D_DIR, ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, 
			ORBITER_RCS_ISP1);
		AddPrimaryRCSExhaust(thManFRCS3[RCS_F3D]);
	}
	if(thManFRCS4[RCS_F4D] == NULL)	{
		thManFRCS4[RCS_F4D] = CreateThruster (ref_pos + RCS_F4D_OFS, RCS_F4D_DIR, ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, 
			ORBITER_RCS_ISP1);
		AddPrimaryRCSExhaust(thManFRCS4[RCS_F4D]);
	}
	//=================================================================================================
	// Upwards firing thrusters 
	//=================================================================================================
	if(thManFRCS1[RCS_F1U] == NULL) {
		thManFRCS1[RCS_F1U] = CreateThruster (ref_pos + RCS_F1U_OFS, RCS_F1U_DIR, ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, 
			ORBITER_RCS_ISP1);
		AddPrimaryRCSExhaust(thManFRCS1[RCS_F1U]);
	}
	if(thManFRCS2[RCS_F2U] == NULL) {
		thManFRCS2[RCS_F2U] = CreateThruster (ref_pos + RCS_F2U_OFS, RCS_F2U_DIR, ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, 
			ORBITER_RCS_ISP1);
		AddPrimaryRCSExhaust(thManFRCS2[RCS_F2U]);
	}
	if(thManFRCS3[RCS_F3U] == NULL)	{
		thManFRCS3[RCS_F3U] = CreateThruster (ref_pos + RCS_F3U_OFS, RCS_F3U_DIR, ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, 
			ORBITER_RCS_ISP1);
		AddPrimaryRCSExhaust(thManFRCS3[RCS_F3U]);
	}
	//=================================================================================================
	// Left firing thrusters 
	//=================================================================================================
	if(thManFRCS1[RCS_F1L] == NULL) {
		thManFRCS1[RCS_F1L] = CreateThruster (ref_pos + RCS_F1L_OFS, RCS_F1L_DIR, ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, 
			ORBITER_RCS_ISP1);
		AddPrimaryRCSExhaust(thManFRCS1[RCS_F1L]);
	}
	if(thManFRCS2[RCS_F3L] == NULL) {
		thManFRCS2[RCS_F3L] = CreateThruster (ref_pos + RCS_F3L_OFS, RCS_F3L_DIR, ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, 
			ORBITER_RCS_ISP1);
		AddPrimaryRCSExhaust(thManFRCS3[RCS_F3L]);
	}
	//=================================================================================================
	// Right firing thrusters 
	//=================================================================================================
	if(thManFRCS1[RCS_F2R] == NULL) {
		thManFRCS1[RCS_F2R] = CreateThruster (ref_pos + RCS_F2R_OFS, RCS_F2R_DIR, ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, 
			ORBITER_RCS_ISP1);
		AddPrimaryRCSExhaust(thManFRCS2[RCS_F2R]);
	}
	if(thManFRCS2[RCS_F4R] == NULL) {
		thManFRCS2[RCS_F4R] = CreateThruster (ref_pos + RCS_F4R_OFS, RCS_F4R_DIR, ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, 
			ORBITER_RCS_ISP1);
		AddPrimaryRCSExhaust(thManFRCS4[RCS_F4R]);
	}
	//=================================================================================================
	// Forwards firing thrusters 
	//=================================================================================================
	if(thManFRCS1[RCS_F1F] == NULL) {
		thManFRCS1[RCS_F1F] = CreateThruster (ref_pos + RCS_F1F_OFS, RCS_F1F_DIR, ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, 
			ORBITER_RCS_ISP1);
		AddPrimaryRCSExhaust(thManFRCS1[RCS_F1F]);
	}
	if(thManFRCS2[RCS_F2F] == NULL) {
		thManFRCS2[RCS_F2F] = CreateThruster (ref_pos + RCS_F2F_OFS, RCS_F2F_DIR, ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, 
			ORBITER_RCS_ISP1);
		AddPrimaryRCSExhaust(thManFRCS2[RCS_F2F]);
	}
	if(thManFRCS3[RCS_F3F] == NULL)	{
		thManFRCS3[RCS_F3F] = CreateThruster (ref_pos + RCS_F3F_OFS, RCS_F3F_DIR, ORBITER_RCS_THRUST, ph_frcs, ORBITER_RCS_ISP0, 
			ORBITER_RCS_ISP1);
		AddPrimaryRCSExhaust(thManFRCS3[RCS_F3F]);
	}
}

void Atlantis::CreateLeftARCS(const VECTOR3 &ref_pos)
{

}

void Atlantis::CreateRightARCS(const VECTOR3 &ref_pos)
{

}

void Atlantis::AddPrimaryRCSExhaust(THRUSTER_HANDLE thX)
{
	const double eh = 6.0;             // exhaust length scale
	const double ew1 = 0.4; // exhaust width scales
	AddExhaust (thX, eh, ew1, tex_rcs);
}

void Atlantis::FireAllNextManifold()
{
	StopAllManifolds();
	int i;
	switch(___iCurrentManifold)
	{
	case 0:
		//Fire none
		return;
	case 1:
		//Fire Manifold 1
		for(i = 0; i<4; i++)
		{
			if(thManFRCS1[i])
			{
				SetThrusterLevel(thManFRCS1[i], 1.0);
			}
	
		}
		___iCurrentManifold++;
		return;
	case 2:
		//Fire manifold 2
		for(i = 0; i<4; i++)
		{
			if(thManFRCS2[i])
			{
				SetThrusterLevel(thManFRCS2[i], 1.0);
			}
	
		}
		___iCurrentManifold++;
		return;
	case 3:
		//Fire manifold 3
		for(i = 0; i<4; i++)
		{
			if(thManFRCS3[i])
			{
				SetThrusterLevel(thManFRCS3[i], 1.0);
			}
	
		}
		___iCurrentManifold++;
		return;
	case 4:
		//Fire manifold 4
		for(i = 0; i<2; i++)
		{
			if(thManFRCS4[i])
			{
				SetThrusterLevel(thManFRCS4[i], 1.0);
			}
	
		}
		___iCurrentManifold++;
		return;
	case 5:
		//Fire manifold 5 (vernier)
		for(i = 0; i<2; i++)
		{
			if(thManFRCS5[i])
			{
				SetThrusterLevel(thManFRCS5[i], 1.0);
			}
	
		}
		___iCurrentManifold++;
		return;
	default:
		___iCurrentManifold = 0;
		return;
	}
}

void Atlantis::StopAllManifolds()
{
	int i;
	for(i = 0; i<4; i++)
	{
		if(thManFRCS1[i])
		{
			SetThrusterLevel(thManFRCS1[i], 0.0);
		}

		if(thManFRCS2[i])
		{
			SetThrusterLevel(thManFRCS2[i], 0.0);
		}

		if(thManFRCS3[i])
		{
			SetThrusterLevel(thManFRCS3[i], 0.0);
		}

		
	}
	for(i=0; i<2; i++)
	{
		if(thManFRCS4[i])
		{
			SetThrusterLevel(thManFRCS4[i], 0.0);
		}
		if(thManFRCS5[i])
		{
			SetThrusterLevel(thManFRCS5[i], 0.0);
		}
	}
}

void Atlantis::SetAirDataProbeDeployment(int side, double position)
{
	if(side == 1) {
		SetAnimation(anim_adpr, position);
	} else if(side == 0) {
		SetAnimation(anim_adpl, position);
	}
}

void Atlantis::DisplayCameraLabel(const char* pszLabel)
{
	strcpy(pszCameraLabelBuffer, pszLabel);
	oapiAnnotationSetText(nhCameraLabel, pszCameraLabelBuffer);
	fTimeCameraLabel = 5.0;
}
