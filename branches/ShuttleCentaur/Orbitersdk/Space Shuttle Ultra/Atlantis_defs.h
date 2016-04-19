#ifndef _ATLANTISDEFS_H_B4CB100F_2A86_4D6A_ADAA_746D20EACCFF
#define _ATLANTISDEFS_H_B4CB100F_2A86_4D6A_ADAA_746D20EACCFF
#pragma once

#include "Orbitersdk.h"
#include "EngConst.h"

const static char* DEFAULT_MESHNAME_VC = "SSU/VC";
const static char* DEFAULT_MESHNAME_KU = "SSU/Ku_band_DA";
const static char* DEFAULT_MESHNAME_COCKPIT = "SSU/Cockpit";
const static char* DEFAULT_MESHNAME_MIDDECK = "SSU/MidDeck";
const static char* DEFAULT_MESHNAME_ORBITER = "SSU/Orbiter";
const static char* ODSPANEL_MESHNAME = "SSU/ODSVC";
const static char* DEFAULT_MESHNAME_PANELA8 = "SSU/RMSVC";
const static char* DEFAULT_MESHNAME_PANELL12U = "SSU\\L12U_SSP_Centaur";
const static char* DEFAULT_MESHNAME_CHUTE = "SSU/parachute";
const static char* SILTS_MESHNAME = "SSU\\SILTSpod";


const VECTOR3 OFS_ZERO             = { 0.0, 0.0,  0.0  };
const VECTOR3 OFS_MMU              = {0,2.44,10.44};
const VECTOR3 VC_OFFSET = _V(0.0, -1.32, -2.22);

// mesh offsets
// Orbiter mesh is always at (0,0,0)
const VECTOR3 ET_OFFSET = _V(0.0, -8.975, 3.925); // offset between ET and shuttle meshes
const VECTOR3 LSRB_OFFSET = _V(-6.4, -8.9, 0.95); // offset between SRB and shuttle mesh
const VECTOR3 RSRB_OFFSET = _V(6.4, -8.9, 0.95); // offset between SRB and shuttle mesh
const VECTOR3 SILTS_OFFSET = _V( 0, 0, 0 );
const VECTOR3 KU_OFFSET = _V(0.0, 0, 0.0);
const VECTOR3 ENTRY_OFFSET = _V(0.0, -1.5, -2.0);


const double MPS_MANIFOLD_MASS_LOX = 2306;// kg
const double MPS_MANIFOLD_MASS_LH2 = 144;// kg
const double MPS_MANIFOLD_MASS_TOTAL = MPS_MANIFOLD_MASS_LOX + MPS_MANIFOLD_MASS_LH2;// estimated 5400 lbs LOX/LH2 in mps manifold

const unsigned short MPS_SSME_NONE = 0;
const unsigned short MPS_SSME_CENTER = 1;
const unsigned short MPS_SSME_LEFT = 2;
const unsigned short MPS_SSME_RIGHT = 3;
const unsigned short MPS_SSME_ALL = 6;

const VECTOR3 SSMER_REF = _V(1.4484, -1.5694, -14.6918);
const VECTOR3 SSMEL_REF = _V(-1.4484, -1.5694, -14.6918);
const VECTOR3 SSMET_REF = _V(0.0, 1.3289, -14.1236);

const VECTOR3 SSMET_INSTALLED_NULL_POS = _V( 0, -0.275637355816999, 0.961261695938319 );
const VECTOR3 SSMEL_INSTALLED_NULL_POS = _V( 0.060121075043999, -0.173648177666930, 0.982970888037132 );
const VECTOR3 SSMER_INSTALLED_NULL_POS = _V( -0.060121075043999, -0.173648177666930, 0.982970888037132 );

const VECTOR3 SSMET_GOX_REF = _V(1.17425, 1.97127, -16.1533);
const VECTOR3 SSMEL_GOX_REF = _V(-1.62164, -0.02995, -16.5744);
const VECTOR3 SSMER_GOX_REF = _V(2.76173, -1.14272, -16.7011);

const VECTOR3 SRB_THRUST_DIR = _V(0.0, 0.0, 1.0);


const VECTOR3 POS_HDP = _V(0.0, -10.25, -19.6);
const VECTOR3 POS_TOW = _V(0.0, -5.64, 14.73);

const VECTOR3 PROBEL_REF = _V(-1.12,-2.346,17.092);
const VECTOR3 PROBER_REF = _V( 1.12,-2.346,17.092);

const VECTOR3 PROBE_AXIS = _V(-0.0918181,0.94801,0.304708);

// ============================================================
// Star Tracker Door Animation Constants
// ============================================================
const VECTOR3 STZD_REF = _V(-1.1169,0.4198,13.5551);
const VECTOR3 STZD_AXIS = _V(0.372101, -0.864888, -0.336913);
const VECTOR3 STYD_REF = _V(-1.9407,-0.2329,13.5354);
const VECTOR3 STYD_AXIS = _V(-0.849865, 0.354425, 0.390016);
const float STAR_TRACKER_DOOR_ANIMATION_ANGLE = static_cast<float>(77.0 * RAD);


const VECTOR3 CHUTE_ATTACH_POINT = _V(0, 3.1655, -14.3915);

const VECTOR3 L_OMS_REF = _V(-2.15, 1.986, -14.269);
const VECTOR3 R_OMS_REF = _V(2.15, 1.986, -14.269);
const VECTOR3 L_OMS_DIR = _V(0.1132032138, -0.272280247041, 0.955535713334);
const VECTOR3 R_OMS_DIR = _V(-0.1132032138, -0.272280247041, 0.955535713334);

const int STATE_PRELAUNCH = 0;
const int STATE_STAGE1 = 1;	//SRBs ignited
const int STATE_STAGE2 = 2;	//ET only
const int STATE_ORBITER = 3; //post ET separation
const int STATE_DESTROYED = 99;	//don't use!
//



//==================================================
// Real RCS thruster offsets and directions
//Source: ODB, p 429-430
//==================================================


//const VECTOR3 RCS_F2F_OFS = _V(0.4, 0.7, 19.0);			//previous coordinates
const VECTOR3 STATION_TO_CG0 = _V(0.0, -10.31553, 24.4715)		//Position by old exhaust coordinates
								+ _V(0.0, 0.0, -0.4);			//Correction factor

const VECTOR3 RCS_F2F_OFS = _V(0.3721, 9.9811, -7.7907) + STATION_TO_CG0;				//Real coordinates
const VECTOR3 RCS_F2F_DIR = _V(-0.29597827, -0.135037729, -0.99042685);						//Direction from ODB
const double  RCS_F2F_THRUST0 = 887.9 * LBF;												//Thrust from ODB

//const VECTOR3 RCS_F3F_OFS = _V(0.0, 0.75, 19.0);
const VECTOR3 RCS_F3F_OFS = _V(0.0, 10.0190, -7.7907) + STATION_TO_CG0;
const VECTOR3 RCS_F3F_DIR = _V(0.0, -0.138175676, -0.990427928);				//Direction from ODB
const double  RCS_F3F_THRUST0 = 888.0 * LBF;									//Thrust from ODB

//const VECTOR3 RCS_F1F_OFS = _V(-0.4, 0.7, 19.0);
const VECTOR3 RCS_F1F_OFS = _V(-0.3721, 9.9811, -7.7907) + STATION_TO_CG0;
const VECTOR3 RCS_F1F_DIR = _V(0.29597827, -0.135037729, -0.99042685);			//Direction from ODB
const double  RCS_F1F_THRUST0 = 887.9 * LBF;									//Thrust from ODB

//const VECTOR3 RCS_F1L_OFS = _V(-1.7 ,-0.15, 17.40);
const VECTOR3 RCS_F1L_OFS = _V(-1.7653, 9.4927, -9.2112) + STATION_TO_CG0;
const VECTOR3 RCS_F1L_DIR = _V(0.999313658, -0.020819035,-0.030084648);			//Direction from ODB
const double  RCS_F1L_THRUST0 = 874.2 * LBF;									//Thrust from ODB

//const VECTOR3 RCS_F3L_OFS = _V(-1.65, 0.25, 17.45);
const VECTOR3 RCS_F3L_OFS = _V(-1.8199, 9.1250, -9.2636) + STATION_TO_CG0;
const VECTOR3 RCS_F3L_DIR = _V(0.99965541, -0.000574317, -0.024121296);			//Direction from ODB
const double  RCS_F3L_THRUST0 = 870.6 * LBF;									//Thrust from ODB

//const VECTOR3 RCS_F2R_OFS = _V(1.75, 0.25, 17.45);
const VECTOR3 RCS_F2R_OFS = _V(1.7653, 9.4927, -9.2112) + STATION_TO_CG0;
const VECTOR3 RCS_F2R_DIR = _V(-0.999313658, -0.020819035, -0.030084648);		//Direction from ODB
const double  RCS_F2R_THRUST0 = 874.62 * LBF;									//Thrust from ODB

//const VECTOR3 RCS_F4R_OFS = _V(1.8, -0.15, 17.40);
const VECTOR3 RCS_F4R_OFS = _V(1.8199, 9.1250, -9.2636) + STATION_TO_CG0;
const VECTOR3 RCS_F4R_DIR = _V(-0.99965541, -0.000574317, -0.024121296);		//Direction from ODB
const double  RCS_F4R_THRUST0 = 870.6 * LBF;									//Thrust from ODB

//const VECTOR3 RCS_F2U_OFS = _V(0.4, 1.10, 17.9);
const VECTOR3 RCS_F2U_OFS = _V(0.3655, 10.501, -8.9136) + STATION_TO_CG0;
const VECTOR3 RCS_F2U_DIR = _V(-0.013369901, -0.999200091, -0.036910067);		//Direction from ODB
const double  RCS_F2U_THRUST0 = 875.1 * LBF;									//Thrust from ODB

const VECTOR3 RCS_F3U_OFS = _V(0.0 , 10.5291, -8.9134 ) + STATION_TO_CG0;		
const VECTOR3 RCS_F3U_DIR = _V(0.0, -0.99931358, -0.03649468);					//Direction from ODB
const double  RCS_F3U_THRUST0 = 874.1 * LBF;									//Thrust from ODB

const VECTOR3 RCS_F1U_OFS = _V(-0.3655, 10.501, -8.9136) + STATION_TO_CG0;		
const VECTOR3 RCS_F1U_DIR = _V(0.013369901, -0.999200091, -0.036910067);		//Direction from ODB
const double  RCS_F1U_THRUST0 = 875.1 * LBF;									//Thrust from ODB

const VECTOR3 RCS_F2D_OFS = _V(1.560068, 9.06653, -8.479536) + STATION_TO_CG0;
const VECTOR3 RCS_F2D_DIR = _V(-0.693675445, 0.719671393, -0.031510241);			//Direction from ODB
const double  RCS_F2D_THRUST0 = 888.6 * LBF;									//Thrust from ODB

const VECTOR3 RCS_F1D_OFS = _V(-1.560068, 9.06653, -8.479536) + STATION_TO_CG0;	
const VECTOR3 RCS_F1D_DIR = _V(0.693675445, 0.719671393, -0.031510241);		//Direction from ODB
const double  RCS_F1D_THRUST0 = 888.6 * LBF;									//Thrust from ODB

const VECTOR3 RCS_F4D_OFS = _V(1.6822, 9.1044, -8.8504) + STATION_TO_CG0;		
const VECTOR3 RCS_F4D_DIR = _V(-0.691500169, 0.721751891, -0.02799413);			//Direction from ODB
const double  RCS_F4D_THRUST0 = 885.9 * LBF;									//Thrust from ODB

const VECTOR3 RCS_F3D_OFS = _V(-1.6822, 9.1044, -8.8504) + STATION_TO_CG0;		
const VECTOR3 RCS_F3D_DIR = _V(0.691500169, 0.721751891, -0.02799413);			//Direction from ODB
const double  RCS_F3D_THRUST0 = 885.9 * LBF;									//Thrust from ODB

const VECTOR3 RCS_F5R_OFS = _V(1.5164, 8.8931, -8.2385) + STATION_TO_CG0;		
const VECTOR3 RCS_F5R_DIR = _V(-0.693877551, 0.718367347, -0.032653061);		//Direction from ODB
const double  RCS_F5R_THRUST0 = 24.5 * LBF;										//Thrust from ODB

const VECTOR3 RCS_F5L_OFS = _V(-1.5164, 8.8931, -8.2385) + STATION_TO_CG0;		
const VECTOR3 RCS_F5L_DIR = _V(0.693877551, 0.718367347, -0.032653061);			//Direction from ODB
const double  RCS_F5L_THRUST0 = 24.5 * LBF;										//Thrust from ODB

const VECTOR3 RCS_R3A_OFS = _XYZO(1555.29, 137.0, 473.06) + STATION_TO_CG0;
const VECTOR3 RCS_R3A_DIR = _FXYZB_TO_DIRECTION(856.8, 0.0, 151.1);
const double  RCS_R3A_THRUST0 = 870.0 * LBF;

const VECTOR3 RCS_R1A_OFS = _XYZO(1555.29, 124.00, 473.06) + STATION_TO_CG0;
const VECTOR3 RCS_R1A_DIR = _FXYZB_TO_DIRECTION(856.8, 0.0, 151.1);
const double  RCS_R1A_THRUST0 = 870.0 * LBF;

const VECTOR3 RCS_L3A_OFS = _XYZO(1555.29, -137.0, 473.06) + STATION_TO_CG0;
const VECTOR3 RCS_L3A_DIR = _FXYZB_TO_DIRECTION(856.8, 0.0, 151.1);
const double  RCS_L3A_THRUST0 = 870.0 * LBF;

const VECTOR3 RCS_L1A_OFS = _XYZO(1555.29, -124.00, 473.06) + STATION_TO_CG0;
const VECTOR3 RCS_L1A_DIR = _FXYZB_TO_DIRECTION(856.8, 0.0, 151.1);
const double  RCS_L1A_THRUST0 = 870.0 * LBF;

const VECTOR3 RCS_L4L_OFS = _XYZO(1516.00, -149.87, 459.00) + STATION_TO_CG0;
const VECTOR3 RCS_L4L_DIR = _FXYZB_TO_DIRECTION(0.0, 870.5, -22.4);
const double  RCS_L4L_THRUST0 = 870.8 * LBF;

const VECTOR3 RCS_L2L_OFS = _XYZO(1529.00, -149.87, 459.00) + STATION_TO_CG0;
const VECTOR3 RCS_L2L_DIR = _FXYZB_TO_DIRECTION(0.0, 870.5, -22.4);
const double  RCS_L2L_THRUST0 = 870.8 * LBF;

const VECTOR3 RCS_L3L_OFS = _XYZO(1542.00, -149.87, 459.00) + STATION_TO_CG0;
const VECTOR3 RCS_L3L_DIR = _FXYZB_TO_DIRECTION(0.0, 870.5, -22.4);
const double  RCS_L3L_THRUST0 = 870.8 * LBF;

const VECTOR3 RCS_L1L_OFS = _XYZO(1555.00, -149.87, 459.00) + STATION_TO_CG0;
const VECTOR3 RCS_L1L_DIR = _FXYZB_TO_DIRECTION(0.0, 870.5, -22.4);
const double  RCS_L1L_THRUST0 = 870.8 * LBF;

const VECTOR3 RCS_R4R_OFS = _XYZO(1516.00, 149.87, 459.00) + STATION_TO_CG0;
const VECTOR3 RCS_R4R_DIR = _FXYZB_TO_DIRECTION(0.0, -870.5, -22.4);
const double  RCS_R4R_THRUST0 = 870.8 * LBF;

const VECTOR3 RCS_R2R_OFS = _XYZO(1529.00, 149.87, 459.00) + STATION_TO_CG0;
const VECTOR3 RCS_R2R_DIR = _FXYZB_TO_DIRECTION(0.0, -870.5, -22.4);
const double  RCS_R2R_THRUST0 = 870.8 * LBF;

const VECTOR3 RCS_R3R_OFS = _XYZO(1542.00, 149.87, 459.00) + STATION_TO_CG0;
const VECTOR3 RCS_R3R_DIR = _FXYZB_TO_DIRECTION(0.0, -870.5, -22.4);
const double  RCS_R3R_THRUST0 = 870.8 * LBF;

const VECTOR3 RCS_R1R_OFS = _XYZO(1555.00, 149.87, 459.00) + STATION_TO_CG0;
const VECTOR3 RCS_R1R_DIR = _FXYZB_TO_DIRECTION(0.0, -870.5, -22.4);
const double  RCS_R1R_THRUST0 = 870.8 * LBF;

const VECTOR3 RCS_L4U_OFS = _XYZO(1516.00, -132.00, 480.50) + STATION_TO_CG0;
const VECTOR3 RCS_L4U_DIR = _FXYZB_TO_DIRECTION(0.0, 0.0, 870.0);
const double  RCS_L4U_THRUST0 = 870.0 * LBF;

const VECTOR3 RCS_L2U_OFS = _XYZO(1529.00, -132.00, 480.50) + STATION_TO_CG0;
const VECTOR3 RCS_L2U_DIR = _FXYZB_TO_DIRECTION(0.0, 0.0, 870.0);
const double  RCS_L2U_THRUST0 = 870.0 * LBF;

const VECTOR3 RCS_L1U_OFS = _XYZO(1542.00, -132.00, 480.50) + STATION_TO_CG0;
const VECTOR3 RCS_L1U_DIR = _FXYZB_TO_DIRECTION(0.0, 0.0, 870.0);
const double  RCS_L1U_THRUST0 = 870.0 * LBF;

const VECTOR3 RCS_R4U_OFS = _XYZO(1516.00, 132.00, 480.50) + STATION_TO_CG0;
const VECTOR3 RCS_R4U_DIR = _FXYZB_TO_DIRECTION(0.0, 0.0, 870.0);
const double  RCS_R4U_THRUST0 = 870.0 * LBF;

const VECTOR3 RCS_R2U_OFS = _XYZO(1529.00, 132.00, 480.50) + STATION_TO_CG0;
const VECTOR3 RCS_R2U_DIR = _FXYZB_TO_DIRECTION(0.0, 0.0, 870.0);
const double  RCS_R2U_THRUST0 = 870.0 * LBF;

const VECTOR3 RCS_R1U_OFS = _XYZO(1542.00, 132.00, 480.50) + STATION_TO_CG0;
const VECTOR3 RCS_R1U_DIR = _FXYZB_TO_DIRECTION(0.0, 0.0, 870.0);
const double  RCS_R1U_THRUST0 = 870.0 * LBF;

const VECTOR3 RCS_L4D_OFS = _XYZO(1516.00, -111.95, 437.40) + STATION_TO_CG0;
const VECTOR3 RCS_L4D_DIR = _FXYZB_TO_DIRECTION(170.4, 291.8, -801.7);
const double  RCS_L4D_THRUST0 = 870.0 * LBF;

const VECTOR3 RCS_L2D_OFS = _XYZO(1529.00, -111.00, 440.00) + STATION_TO_CG0;
const VECTOR3 RCS_L2D_DIR = _FXYZB_TO_DIRECTION(170.4, 291.8, -801.7);
const double  RCS_L2D_THRUST0 = 870.0 * LBF;

const VECTOR3 RCS_L3D_OFS = _XYZO(1542.00, -110.06, 442.60) + STATION_TO_CG0;
const VECTOR3 RCS_L3D_DIR = _FXYZB_TO_DIRECTION(170.4, 291.8, -801.7);
const double  RCS_L3D_THRUST0 = 870.0 * LBF;

const VECTOR3 RCS_R4D_OFS = _XYZO(1516.00, 111.95, 437.40) + STATION_TO_CG0;
const VECTOR3 RCS_R4D_DIR = _FXYZB_TO_DIRECTION(170.4, -291.8, -801.7);
const double  RCS_R4D_THRUST0 = 870.0 * LBF;

const VECTOR3 RCS_R2D_OFS = _XYZO(1529.00, 111.00, 440.00) + STATION_TO_CG0;
const VECTOR3 RCS_R2D_DIR = _FXYZB_TO_DIRECTION(170.4, -291.8, -801.7);
const double  RCS_R2D_THRUST0 = 870.0 * LBF;

const VECTOR3 RCS_R3D_OFS = _XYZO(1542.00, 110.06, 442.60) + STATION_TO_CG0;
const VECTOR3 RCS_R3D_DIR = _FXYZB_TO_DIRECTION(170.4, -291.8, -801.7);
const double  RCS_R3D_THRUST0 = 870.0 * LBF;

const VECTOR3 RCS_L5D_OFS = _XYZO(1565.00, -118.00, 445.44) + STATION_TO_CG0;
const VECTOR3 RCS_L5D_DIR = _FXYZB_TO_DIRECTION(0.0, 0.0, -24.0);
const double  RCS_L5D_THRUST0 = 24.0 * LBF;

const VECTOR3 RCS_R5D_OFS = _XYZO(1565.00, 118.00, 445.44) + STATION_TO_CG0;
const VECTOR3 RCS_R5D_DIR = _FXYZB_TO_DIRECTION(0.0, 0.0, -24.0);
const double  RCS_R5D_THRUST0 = 24.0 * LBF;

const VECTOR3 RCS_L5L_OFS = _XYZO(1565.00, -149.87, 459.00) + STATION_TO_CG0;
const VECTOR3 RCS_L5L_DIR = _FXYZB_TO_DIRECTION(0.0, 24.0, -0.6);
const double  RCS_L5L_THRUST0 = 24.0 * LBF;

const VECTOR3 RCS_R5R_OFS = _XYZO(1565.00, 149.87, 459.00) + STATION_TO_CG0;
const VECTOR3 RCS_R5R_DIR = _FXYZB_TO_DIRECTION(0.0, -24.0, -0.6);
const double  RCS_R5R_THRUST0 = 24.0 * LBF;

//---------------------------------------------------------------------------------------
//RCS end
//---------------------------------------------------------------------------------------

/* RCS GROUPS:
   G-1 : F2F, F3F, F1F : 
   G-2 : F1L, F3L : + yaw, +y
   G-3 : F2R, F4R : - yaw, -y
   G-4 : F2U, F3U, F1U : 
   G-5 : F5R, F2D, F4D : -roll, +pitch, -z
   G-6 : F5L, F1D, F3D : +roll, +pitch, -z
   G-7 : R3A, R1A : +x
   G-8 : L1A, L3A : +x
   G-9 : L4L, L2L, L3L, L1L : -yaw, +y
   G-10: R1R, R3R, R2R, R4R : +yaw, -y
   G-11: L1U, L2U, L4U : -roll, +pitch, +z
   G-12: R1U, R2U, R4U : +roll, +pitch, +z
   G-13: L3D, L2D, L4D : +roll, -pitch, -z
   G-14: R3D, R2D, R4D : -roll, -pitch, -z
 */

const double PL_ATTACH_CENTER_Y = -2.933;
const double PL_ATTACH_SIDE_Y = -0.546;
const double PL_ATTACH_SIDE_X = 2.424;

const VECTOR3 DIR_CENTERPL = _V(0.0, 1.0, 0.0);
const VECTOR3 ROT_CENTERPL = _V(0.0, 0.0, 1.0);


const VECTOR3 DIR_PORTPL = _V(1.0, 0.0, 0.0);
const VECTOR3 ROT_PORTPL = _V(0.0, -1.0, 0.0);

const VECTOR3 DIR_STBDPL = _V(-1.0, 0.0, 0.0);
const VECTOR3 ROT_STBDPL = _V(0.0, -1.0, 0.0);

const VECTOR3 OFS_PORTMMU = _V(-PL_ATTACH_SIDE_X, PL_ATTACH_SIDE_Y, 8.0);
const VECTOR3 OFS_STBDMMU = _V( PL_ATTACH_SIDE_X, PL_ATTACH_SIDE_Y, 8.0);

// =====================================================================
// CG definitions (location of CG relative to center of Orbiter mesh)
// =====================================================================
const VECTOR3 ORBITER_CG = _V(0.0, -1.23562, -3.52522);// + _V(0.0, 0.28491595177, -0.5908348442); // second term is fudge factor added so OMS gimbal angles are correct (SiameseCat, 141205)
const VECTOR3 ET_EMPTY_CG = ET_OFFSET + _V(0.0, 0.62484, 4.6927);
const VECTOR3 LSRB_CG = LSRB_OFFSET + _V(0.0, 0.0, 2.61217);
const VECTOR3 RSRB_CG = RSRB_OFFSET + _V(0.0, 0.0, 2.61217);
const VECTOR3 ET_LH2_BASE = ET_OFFSET + _V(0.0, 0.0, -15.8);
const VECTOR3 ET_LOX_BASE = ET_OFFSET + _V(0.0, 0.0, 15.0); // might not be exactly correct


#endif _ATLANTISDEFS_H_B4CB100F_2A86_4D6A_ADAA_746D20EACCFF
