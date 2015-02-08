#ifndef _ATLANTISDEFS_H_B4CB100F_2A86_4D6A_ADAA_746D20EACCFF
#define _ATLANTISDEFS_H_B4CB100F_2A86_4D6A_ADAA_746D20EACCFF
#pragma once

#include "Orbitersdk.h"

const static char* DEFAULT_MESHNAME_VC = "SSU/VC";
const static char* DEFAULT_MESHNAME_KU = "SSU/Ku_band_DA";
const static char* DEFAULT_MESHNAME_COCKPIT = "SSU/Cockpit";
const static char* DEFAULT_MESHNAME_MIDDECK = "SSU/MidDeck";
//const static char* DEFAULT_MESHNAME_RMS = "SSU/RMS";
//const static char* DEFAULT_MESHNAME_MPM = "SSU/OBSSMPMs";
const static char* DEFAULT_MESHNAME_ORBITER = "SSU/Orbiter";
//const static char* DEFAULT_MESHNAME_LSRB = "SSU/LSRB";
//const static char* DEFAULT_MESHNAME_RSRB = "SSU/RSRB";
const static char* ODSPANEL_MESHNAME = "SSU/ODSVC";
const static char* DEFAULT_MESHNAME_EXTAL = "SSU/ExtAL";
const static char* DEFAULT_MESHNAME_ODS = "SSU/ODS";
const static char* DEFAULT_MESHNAME_PANELA8 = "SSU/RMSVC";
const static char* DEFAULT_MESHNAME_CHUTE = "SSU/parachute";
const static char* SILTS_MESHNAME = "SSU\\SILTSpod";


const VECTOR3 OFS_ZERO             = { 0.0, 0.0,  0.0  };
//const VECTOR3 OFS_LAUNCH_ORBITER   = { 0.0, 6.04,-7.989};
//const VECTOR3 OFS_LAUNCH_TANK      = { 0.0,-1.91, 5.72 };
//const VECTOR3 OFS_LAUNCH_RIGHTSRB  = { 6.2,-1.91,-5.68 };
//const VECTOR3 OFS_LAUNCH_LEFTSRB   = {-6.2,-1.91,-5.68 };
//const VECTOR3 OFS_LAUNCH_RIGHTSRB  = { 6.27,-1.91,-5.68 };
//const VECTOR3 OFS_LAUNCH_LEFTSRB   = {-6.27,-1.91,-5.68 };
//const VECTOR3 OFS_WITHTANK_ORBITER = { 0.0, 4.658,-9.414};
//const VECTOR3 OFS_WITHTANK_TANK    = { 0.0,-3.34, 4.33 };
const VECTOR3 OFS_MMU              = {0,2.44,10.44};
const VECTOR3 VC_OFFSET = _V(0.0, -1.32, -2.22);

const VECTOR3 ODS_POS = _V(0.0, -1.35, 7);// Only X and Y axes used. Z pos is set in Mission.cpp (fODSZPos)
const VECTOR3 ODS_DOCKPOS_OFFSET = _V(0.0, 2.1, -0.242); // offset between ODS mesh position and docking port position

// mesh offsets
// Orbiter mesh is always at (0,0,0)
const VECTOR3 ET_OFFSET = _V(0.0, -8.975, 3.925); // offset between ET and shuttle meshes
const VECTOR3 LSRB_OFFSET = _V(-6.4, -9.46, 0.59); // offset between SRB and shuttle mesh
const VECTOR3 RSRB_OFFSET = _V(6.4, -9.46, 0.59); // offset between SRB and shuttle mesh
const VECTOR3 SILTS_OFFSET = _V( 0, 0, 0 );
const VECTOR3 KU_OFFSET = _V(0.035, 1.01, -0.05);

const double MPS_MANIFOLD_MASS_LOX = 2306;// kg
const double MPS_MANIFOLD_MASS_LH2 = 144;// kg
const double MPS_MANIFOLD_MASS_TOTAL = MPS_MANIFOLD_MASS_LOX + MPS_MANIFOLD_MASS_LH2;// estimated 5400 lbs LOX/LH2 in mps manifold

const unsigned short MPS_SSME_NONE = 0;
const unsigned short MPS_SSME_CENTER = 1;
const unsigned short MPS_SSME_LEFT = 2;
const unsigned short MPS_SSME_RIGHT = 3;
const unsigned short MPS_SSME_ALL = 6;

const VECTOR3 SSMER_REF = _V(1.3757, -1.7002, -14.1162);
const VECTOR3 SSMEL_REF = _V(-1.3757, -1.7002, -14.1162);
const VECTOR3 SSMET_REF = _V(0.0, 1.0201, -13.4180);
//const VECTOR3 SSMET_REF = _V(0.0, 3.2,-15.5);

const VECTOR3 SSMET_INSTALLED_NULL_POS = _V( 0, -0.275637355816999, 0.961261695938319 );
const VECTOR3 SSMEL_INSTALLED_NULL_POS = _V( 0.060121075043999, -0.173648177666930, 0.982970888037132 );
const VECTOR3 SSMER_INSTALLED_NULL_POS = _V( -0.060121075043999, -0.173648177666930, 0.982970888037132 );

const VECTOR3 SSMET_GOX_REF = _V(1.230, 1.8034, -15.9772);
const VECTOR3 SSMEL_GOX_REF = _V(-1.4787, 0.0362, -16.5415);
const VECTOR3 SSMER_GOX_REF = _V(2.7507, -1.1756, -16.7006);

//const VECTOR3 SRB_THRUST_DIR = _V(0.0, 0.069338, 0.99759);
const VECTOR3 SRB_THRUST_DIR = _V(0.0, 0.0, 1.0);


const VECTOR3 POS_HDP = _V(0.0, -10.25, -19.6);
const VECTOR3 POS_TOW = _V(0.0, -5.64, 14.73);

//const VECTOR3 STBDMPM_REF = _V(2.81, 1.60, 1.68);

const VECTOR3 PROBEL_REF = _V(-1.134,-2.346,17.092);
const VECTOR3 PROBER_REF = _V( 1.134,-2.346,17.092);

const VECTOR3 PROBE_AXIS = _V(-0.0918181,0.94801,0.304708);

// ============================================================
// Star Tracker Door Animation Constants
// ============================================================
const VECTOR3 STZD_REF = _V(-1.178,0.404,13.55);
const VECTOR3 STZD_AXIS = _V(-0.241377,0.928995,0.280545);
const VECTOR3 STYD_REF = _V(-1.965,-0.244,13.509);
const VECTOR3 STYD_AXIS = _V(0.874892,-0.359366,-0.324684);
const float STAR_TRACKER_DOOR_ANIMATION_ANGLE = static_cast<float>(95.0 * RAD);


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

//======================================
// RCS table indices for each manifold
//======================================
//Forward RCS, Manifold 1 indices
const int RCS_F1L = 0;
const int RCS_F1U = 1;
const int RCS_F1D = 2;
const int RCS_F1F = 3;
//Forward RCS, Manifold 2 indices
const int RCS_F2R = 0;
const int RCS_F2U = 1;
const int RCS_F2D = 2;
const int RCS_F2F = 3;
//Forward RCS, Manifold 3 indices
const int RCS_F3L = 0;
const int RCS_F3U = 1;
const int RCS_F3D = 2;
const int RCS_F3F = 3;
//Forward RCS, Manifold 4 indices
const int RCS_F4R = 0;
const int RCS_F4D = 1;
//Forward RCS, Manifold 5(Vernier) indices
const int RCS_F5L = 0;
const int RCS_F5R = 1;
//Left RCS, Manifold 1 indices
const int RCS_L1L = 0;
const int RCS_L1U = 1;
const int RCS_L1A = 2;
//Left RCS, Manifold 2 indices
const int RCS_L2L = 0;
const int RCS_L2U = 1;
const int RCS_L2D = 2;
//Left RCS, Manifold 3 indices
const int RCS_L3L = 0;
const int RCS_L3D = 1;
const int RCS_L3A = 2;
//Left RCS, Manifold 4 indices
const int RCS_L4L = 0;
const int RCS_L4U = 1;
const int RCS_L4D = 2;
//Right RCS, Manifold 5(vernier) indices
const int RCS_L5R = 0;
const int RCS_L5D = 1;
//Right RCS, Manifold 1 indices
const int RCS_R1R = 0;
const int RCS_R1U = 1;
const int RCS_R1A = 2;
//Right RCS, Manifold 2 indices
const int RCS_R2R = 0;
const int RCS_R2U = 1;
const int RCS_R2D = 2;
//Right RCS, Manifold 3 indices
const int RCS_R3R = 0;
const int RCS_R3D = 1;
const int RCS_R3A = 2;
//Right RCS, Manifold 4 indices
const int RCS_R4R = 0;
const int RCS_R4U = 1;
const int RCS_R4D = 2;
//Right RCS, Manifold 5(vernier) indices
const int RCS_R5R = 0;
const int RCS_R5D = 1;

//==================================================
// Real RCS thruster offsets and directions
//==================================================

const VECTOR3 RCS_F1L_OFS = _V(-1.7 ,-0.15, 17.40);
const VECTOR3 RCS_F1L_DIR = _V(1,0,0);
const VECTOR3 RCS_F1U_OFS = _V(-0.4 , 1.10, 17.9 );
const VECTOR3 RCS_F1U_DIR = _V(0, -1,0);
const VECTOR3 RCS_F1D_OFS = _V(-1.55,-0.20, 18.25);
const VECTOR3 RCS_F1D_DIR = _V(0.4339,0.8830,0.1793);
const VECTOR3 RCS_F1F_OFS = _V(-0.4, 0.7, 19.0);
const VECTOR3 RCS_F1F_DIR = _V(0, -0.0499, -0.9988);

const VECTOR3 RCS_F2R_OFS = _V( 1.75, 0.25, 17.45);
const VECTOR3 RCS_F2R_DIR = _V(-1,0,0);
const VECTOR3 RCS_F2U_OFS = _V( 0.4, 1.10, 17.9 );
const VECTOR3 RCS_F2U_DIR = _V(0, -1,0);
const VECTOR3 RCS_F2D_OFS = _V( 1.60,-0.20, 18.25);
const VECTOR3 RCS_F2D_DIR = _V( -0.4339,0.8830,0.1793);
const VECTOR3 RCS_F2F_OFS = _V( 0.4, 0.7, 19.0 );
const VECTOR3 RCS_F2F_DIR = _V(0, -0.0499, -0.9988);

const VECTOR3 RCS_F3L_OFS = _V(-1.65, 0.25 , 17.45);
const VECTOR3 RCS_F3L_DIR = _V(1,0,0);
const VECTOR3 RCS_F3U_OFS = _V( 0.0 , 1.15, 17.9);
const VECTOR3 RCS_F3U_DIR = _V(0, -1,0);
const VECTOR3 RCS_F3D_OFS = _V(-1.63,-0.18, 17.9);
const VECTOR3 RCS_F3D_DIR = _V(0.4339,0.8830,0.1793);
const VECTOR3 RCS_F3F_OFS = _V( 0.0 , 0.75, 19.0 );
const VECTOR3 RCS_F3F_DIR = _V(0, -0.0499, -0.9988);

const VECTOR3 RCS_F4R_OFS = _V( 1.8 ,-0.15, 17.40);
const VECTOR3 RCS_F4R_DIR = _V(-1,0,0);
const VECTOR3 RCS_F4D_OFS = _V( 1.68,-0.18, 17.9);
const VECTOR3 RCS_F4D_DIR = _V(-0.4339,0.8830,0.1793);

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

static const char* PAYLOADTYPE[6] = {"XS1P", "XS3P", "XS5P",
	"XS1A", "XS3A", "XS5A"};

//const double DEFAULT_PAYLOAD_ZPOS[16] = 
//{ 8.0, 0.0, -8.0,		//Center active
//  4.0, 2.0, -6.0,		//Center passive
//  7.0, 3.0, -2.0, -8.0,	//Port
//  7.0, 3.0, -2.0, -8.0,	//Starboard
//  0.0, 0.0};			//spare


#endif _ATLANTISDEFS_H_B4CB100F_2A86_4D6A_ADAA_746D20EACCFF
