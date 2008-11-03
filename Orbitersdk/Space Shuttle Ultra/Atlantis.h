// ==============================================================
//                 ORBITER MODULE: Atlantis
//                  Part of the ORBITER SDK
//          Copyright (C) 2001-2003 Martin Schweiger
//                   All rights reserved
//
// Atlantis.h
// Class interface of Atlantis (Space Shuttle) vessel class
// module and associated subclasses (SRB, tank)
// ==============================================================

#ifndef __ATLANTIS_H
#define __ATLANTIS_H

#include "CRT.h"
#include "orbitersdk.h"
#include <math.h>
#include <vector>
#include "meshres_vc.h"
#include "dps/dps_defs.h"
#include "dps/MasterTimingUnit.h"
#include "mps/SSME.h"
#include "vc/vc_defs.h"
#include "vc/PanelGroup.h"
#include "vc/BasicPanel.h"
#include "vc/PanelF7.h"
#include "vc/DAPControl.h"
#include "discsignals/Discsignals.h"
#include "eva_docking/BasicExtAirlock.h"



typedef struct {
	double P;		//Pressure (psig)
	double T;		//Temperature (°R)
	double mdot;	//mass flow (lb/s)
} FLOWSTATE;

const MATRIX3 IdentityMatrix = _M(1, 0, 0,
								  0, 1, 0,
								  0, 0, 1);


//Z
//-.5216187842e-1*sin(beta)+.5609219446*cos(alpha)
//-.2802319446*cos(alpha)*cos(beta)+.9135110136*sin(alpha)
//+.9417727780e-2*sin(alpha)*cos(beta)

const double LBM = 0.45359237;
const double MPS2FPS = 3.280839895;
const double MPS2KTS = 1.943844492;

const short VARSTATE_OK = 0;
const short VARSTATE_MISSING = 1;
const short VARSTATE_OFFSCALE_LOW = 2;
const short VARSTATE_LOW = 3;
const short VARSTATE_HIGH = 4;
const short VARSTATE_OFFSCALE_HIGH = 5;

const static char* DEFAULT_MESHNAME_VC = "SSU/VC";
const static char* DEFAULT_MESHNAME_KU = "SSU/KU";
const static char* DEFAULT_MESHNAME_COCKPIT = "SSU/Cockpit";
const static char* DEFAULT_MESHNAME_MIDDECK = "SSU/MidDeck";
const static char* DEFAULT_MESHNAME_RMS = "SSU/RMS";
const static char* DEFAULT_MESHNAME_MPM = "SSU/OBSSMPMs";
const static char* DEFAULT_MESHNAME_ORBITER = "SSU/Orbiter";
const static char* DEFAULT_MESHNAME_ET = "SSU/ET125";
const static char* DEFAULT_MESHNAME_LSRB = "SSU/LSRB";
const static char* DEFAULT_MESHNAME_RSRB = "SSU/RSRB";
const static char* ODSPANEL_MESHNAME = "SSU/ODSVC";
const static char* DEFAULT_MESHNAME_EXTAL = "SSU/ExtAL";
const static char* DEFAULT_MESHNAME_ODS = "SSU/ODS";
const static char* DEFAULT_MESHNAME_PANELA8 = "SSU/RMSVC";
const static char* DEFAULT_MESHNAME_CHUTE = "SSU/parachute";

// ==========================================================
// Some Orbiter-related parameters
// ==========================================================

const double ORBITER_EMPTY_MASS = 81100.0;
// Orbiter empty mass [kg]

const double ORBITER_MAX_PROPELLANT_MASS = 14538.0;
// Amount of fuel the orbiter can hold in internal OMS tanks

const double ORBITER_FRCS_PROPELLANT_MASS = 1464 * LBM + 923*LBM;
// Amount of fuel in forward RCS tanks

const double SSME_RATED_THRUST = 2090664.159; //100% thrust
const double ORBITER_MAIN_THRUST = 2170732.15; //Nominal "104%" thrust
const double ORBITER_MAX_THRUST = 2275094.273; //109% thrust
// Vacuum thrust rating per main engine [N]

const double ORBITER_OMS_THRUST = 26700.0;
// Vacuum thrust per unit for Orbital Maneuvering System [N] (x2 for total)

const double ORBITER_RCS_THRUST = 7740.0;
// Vacuum thrust rating for attitude thrusters (Reaction Control System) [N]

const double ORBITER_MAIN_ISP0 = 453*9.80665;
const double ORBITER_MAIN_ISP1 = 363*9.80665;
// Vacuum and sea-level fuel-specific impulse for orbiter main engines [m/s]
// using H2/O2 (hydrogen/oxygen)

const double ORBITER_OMS_ISP0 = 316*9.80665;
const double ORBITER_OMS_ISP1 = ORBITER_OMS_ISP0*0.75;
// Vacuum and sea-level fuel-specific impulse for Orbital Maneuvering System [m/s]

const double ORBITER_RCS_ISP0 = ORBITER_OMS_ISP0;
const double ORBITER_RCS_ISP1 = ORBITER_RCS_ISP0*0.75;
// Vacuum and sea-level fuel-specific impulse for Reaction Control System [m/s]

const double ORBITER_OMS_PITCH = 15.0089;
const double ORBITER_OMS_YAW = 5.7000;
// Null position of OMS engines

const double ORBITER_PITCH_TORQUE = 239900.00;
const double ORBITER_YAW_TORQUE = 239900.00;
const double ORBITER_ROLL_TORQUE = 41800.00;
// Vacuum RCS Torque [N]

const double OMS_HELIUM_TANK_MASS = 21.0;
//Helium tank mass [kg]

const double APU_FUEL_TANK_MASS = 325.0*0.45359237;
// Max APU Fuel tank mass
const double APU_FUEL_TANK_FLOWRATE[2] = {(3.25*0.45359237)/60.0, (1.5*0.45359237)/60.0};
// APU fuel consumption rate(kg/sec)

const double CHUTE_DEPLOY_TIME = 1.5;
// Time from chute deploy command to deployment of chute (s)
const double CHUTE_INFLATE_TIME = 6.3;
// Time from chute deploy command to full inflation of chute (s)
const double CHUTE_DEPLOY_RATE = 0.25;
// Rate at which chute deploys to reefed state (1/s)
const double CHUTE_INFLATE_RATE = 1.0;
// Rate at which chute inflates (1/s)
const double CHUTE_SPIN_RATE = 0.5;
// Rate at which chute spins (1/s)
const double CHUTE_DEPLOY_SPEED = 195.0/MPS2KTS;
// Speed at which chute is deployed (m/s)
const double CHUTE_JETTISON_SPEED = 60.0/MPS2KTS;
// Speed at which chute is jettisoned (m/s)

const double GEAR_OPERATING_SPEED = 0.3;
// Opening/closing speed of landing gear (1/sec)
// => gear cycle ~ 3 sec

const double DOOR_OPERATING_SPEED = 0.007353;
// Opening/closing speed of payload bay doors (1/sec)
// This contains the door opening sequence (63 sec for each door) and an
// interval of 10 sec between the two door operations

const double DOORLATCH_OPERATING_SPEED = 0.05;
// Opening/closing speed of payload bay door latch gang (1/sec)
// 20 sec for each gang

const double RAD_OPERATING_SPEED = 0.023255814;
// Deployment/stowing speed of radiators (1/sec)
// => radiator cycle = 43 sec

const double RADLATCH_OPERATING_SPEED = 0.0384615385;
// Release/engaging speed of radiator latches (1/sec)
// => radiator latch cycle = 26 sec

const double KU_OPERATING_SPEED = 0.0435;
// Deployment speed of the Ku Band antenna (1/sec)
// cycle is 23 sec

const double SPEEDBRAKE_OPERATING_SPEED = 0.20284;
// Deployment speed of the speedbrake (1/sec)
// cycle is 4.93 sec

const double ET_UMB_DOOR_OPERATING_SPEED = 0.0416667;
const double LATCH_OPERATING_SPEED = 0.166666667;

const double AIR_DATA_PROBE_SPEED = 0.06666667;

const double ARM_OPERATING_SPEED = 0.005;
// RMS arm joint rotation speed (rad/sec)
const double ARM_TRANSLATE_SPEED = 0.1;
// RMS IK translation speed (m/s)
const double ARM_DEPLOY_SPEED = 0.0294117647;
// RMS rollout speed
const double ARM_GRAPPLE_SPEED = 0.4;
// Time to fully grapple an object (1/s)
const double ARM_RIGID_SPEED = 0.25;
// Time to regidize/derigidize arm (1/s)
const double ARM_EXTEND_SPEED = 0.142857;
// Time to extend/retract EE (1/s)
const double SHOULDER_BRACE_SPEED = 0.11765;
// shoulder brace speed
const VECTOR3 ARM_WRIST_CAM_OFFSET = {-0.091886, 0.276656, 0.666001};
// Wrist camera offset from grapple point (assuming wrist roll angle of 0.0)

const double DYNP_MAX = 100e3;
// Max. allowed dynamic pressure [Pa]
const double GEAR_MAX_DEPLOY_SPEED = 450/1.943844;
// Max. safe gear deploy speed [m/s]

const VECTOR3 ORBITER_CS = {234.8,389.1,68.2};
// Orbiter cross sections (projections into principal axes) [m^2]

const VECTOR3 ORBITER_CS_GEAR = {10.0,0.0,3.0};
// Contribution of fully extended landing gear to cross sections

const double ORBITER_CW[4] = {0.13, 0.5, 1.4, 1.4};
// Orbiter wind resistance coefficients in principal directions (+z, -z, +-x, +-y)

const double ORBITER_CW_GEAR[4] = {0.04, 0.04, 0.05, 0.0};
// Contribution of fully extended landing gear to wind resistance

const double MAX_GRAPPLING_DIST = 0.5;
// max distance between RMS tip and grappling point for successful grappling
const double MAX_GRAPPLING_ANGLE = 0.087266;
// max angle between EE and grapple for successful grappling (radians)

const double CAM_HIGHRATE_SPEED = 12;
const double CAM_LOWRATE_SPEED = 1.2;
// Payload camera speed rates (deg/sec)

const int NoseRotThrusters[6] = {0, 2, 4, 6, 8, 10};
const int NosePitchThrusters[2] = {0, 2};
const int NoseYawThrusters[2] = {4, 6};
const int NoseRollThrusters[2] = {8, 10};
const int AftRotThrusters[6] = {1, 3, 5, 7, 9, 11};
const int AftPitchThrusters[2] = {1, 3};
const int AftYawThrusters[2] = {5, 7};
const int AftRollThrusters[2] = {9, 11};
//RCS Thruster Groups

//PEG
const double TMajorCycle = 1.0000;
const double TPEGStop = 7.000; //40?

//Attitude ref. frame conversion
const double AXIS_TILT = 23.4458878*RAD;
//tilt of Earth's axis (radians)

// ==========================================================
// Some Tank-related parameters
// ==========================================================

const double TANK_MAX_PROPELLANT_MASS = 719115.0;
// Main tank propellant mass [kg]

const double TANK_EMPTY_MASS = 26535; //Super light weight tank, 58500lb, STS-91 on
                                      //STS-1 to STS-5 tank 76000lb 35425.0kg
                                      //Light weight tank 66000lb 29937kg, STS-6 to STS-90
// Main tank empty mass

const double ET_SEP_TIME = 18.00000; // ET Sep time (after MECO)
const double ET_SEP_RATE = 1.2192; // Commanded ET sep rate (m/s)

// ==========================================================
// Some SRB-related parameters
// ==========================================================

const double SRB_MAX_PROPELLANT_MASS = 502146.0;
// SRB propellant mass [kg]
//total impulse is 1.32466E9 Ns

const double SRB_EMPTY_MASS = 87603.65;
// SRB empty mass [kg]

//const double SRB_ISP0 = 3574.68;
const double SRB_ISP0 = 2638.89;
//const double SRB_ISP1 = 2859.74;
const double SRB_ISP1 = 2368.79;
// SRB vacuum and sea-level fuel-specific impulse [m/s]

//const double SRB_THRUST = 17064000; //from Jenkins
/*
SRM vacuum thrust as reported by Thiokol in the report "RSRM-13 (360Q013) ballistics mass 
properties flight designation STS-41"
*/
const double SRB_THRUST = 15.12904086E6; // Vacuum SRB thrust per unit [N]

//Lift-off thrust is 15.574012 MN
//Equals vacuum thrust 17.67824672 MN 

const double MAX_ATT_LAUNCH = 1e-5;
const double MAX_ROLL_SRB = 2.5e-5;
// Max attitude thrust during launch phase (temporary)

const double SRB_STABILISATION_TIME = 6.6;
// MET: -SRB ignition

const double SRB_SEPARATION_TIME = 124.0;
// MET: SRB separation
const int SRB_nt = 30;

const double SRB_CUTOUT_TIME = 130.0;

const double BSM_MAX_PROPELLANT_MASS = 75.0 * 0.45349;
const double BSM_THRUST0 = 82.76475E3;
const double BSM_ISP0 = 2400.0;

const UINT MIDX_ORBITER = 1;
const UINT MIDX_COCKPIT = 2;
const UINT MIDX_VC = 3;
const UINT MIDX_RMS = 4;
const UINT MIDX_MIDDECK = 5;
const UINT MIDX_KU_ANTENNA = 6;
const UINT MIDX_ET = 7;
const UINT MIDX_LSRB = 8;
const UINT MIDX_RSRB = 9;
const UINT MIDX_EXTAL = 10;
const UINT MIDX_ODS = 11;
const UINT MIDX_ODS_VC = 12;
const UINT MIDX_PLPANEL_A7 = 13;
const UINT MIDX_PLPANEL_A8 = 14;
const UINT MIDX_PLPANEL_A9 = 15;


// MET: engine shutdown
const double NO_SLAG_TIME = 240.0;

const double SLAG_SPREAD = 0.2;

const double SLAG1_RATE = 5;
const double SLAG2_RATE = 11.0;
const double SLAG3_RATE = 49.0;

const VECTOR3 SLAG_DIR = _V(0.0, 0.0, -1.0);

// ==========================================================
// Unit conversion
// ==========================================================
const double fps_to_ms=0.3048;
const double kg_to_pounds=2.205;

// ==========================================================
// Mesh offsets for various configurations
// ==========================================================

const VECTOR3 OFS_ZERO             = { 0.0, 0.0,  0.0  };
const VECTOR3 OFS_LAUNCH_ORBITER   = { 0.0, 6.04,-7.989};
const VECTOR3 OFS_LAUNCH_TANK      = { 0.0,-1.91, 5.72 };
//const VECTOR3 OFS_LAUNCH_RIGHTSRB  = { 6.2,-1.91,-5.68 };
//const VECTOR3 OFS_LAUNCH_LEFTSRB   = {-6.2,-1.91,-5.68 };
const VECTOR3 OFS_LAUNCH_RIGHTSRB  = { 6.88,-1.91,-5.68 };
const VECTOR3 OFS_LAUNCH_LEFTSRB   = {-6.88,-1.91,-5.68 };
const VECTOR3 OFS_WITHTANK_ORBITER = { 0.0, 4.658,-9.414};
const VECTOR3 OFS_WITHTANK_TANK    = { 0.0,-3.34, 4.33 };
const VECTOR3 ORBITER_DOCKPOS      = { 0.0, 2.3729,10.1529};
const VECTOR3 OFS_MMU              = {0,2.44,10.44};


const unsigned short MPS_SSME_NONE = 0;
const unsigned short MPS_SSME_CENTER = 1;
const unsigned short MPS_SSME_LEFT = 2;
const unsigned short MPS_SSME_RIGHT = 3;
const unsigned short MPS_SSME_ALL = 6;

const VECTOR3 SSMER_REF = _V(1.458, -0.194, -11.7875);
const VECTOR3 SSMEL_REF = _V(-1.458, -0.194, -11.7875);
const VECTOR3 SSMET_REF = _V(0.0, 1.945, -10.76250);
//const VECTOR3 SSMET_REF = _V(0.0, 3.2,-15.5);

const VECTOR3 SSMET_GOX_REF = _V(1.149, 3.166, -13.91);
const VECTOR3 SSMEL_GOX_REF = _V(-1.754, 1.274, -14.471);
const VECTOR3 SSMER_GOX_REF = _V(2.615, 0.239, -14.665);


const VECTOR3 POS_HDP = _V(0.0, -1.91, -25.8);
const VECTOR3 POS_TOW = _V(0.0, -1.91, 25.8);

const VECTOR3 UMBDOORL_REF = _V(-1.31087, -2.71022, -6.75496);
const VECTOR3 UMBDOORR_REF = _V(1.31087, -2.71022, -6.75496);
//const VECTOR3 UMBDOORR_REF = _V(1.3343, -2.8067, -7.2918);
const VECTOR3 UMBDOOR_AXIS = _V(0, -0.05, 0.99875);

const VECTOR3 STBDMPM_REF = _V(2.81, 1.60, 1.68);

const VECTOR3 PROBEL_REF = _V(-1.122688, -1.0894815, 19.4175);
const VECTOR3 PROBER_REF = _V( 1.122688, -1.0894815, 19.4175);

const VECTOR3 PROBE_AXIS = _V(0.0, cos(15 * RAD), sin(15*RAD));

const VECTOR3 CHUTE_ATTACH_POINT = _V(0, 4.6, -12.03);

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

const VECTOR3 ODS_POS = _V(0.0, 0.85, 10.1529);//080728, DaveS edit: Fixed ODS vertical offset in the payload bay

/**  
 * list of the VC positions
 */
const int VC_CDR = 0;
const int VC_PLT = 1;
const int VC_MS1 = 2;
const int VC_MS2 = 3;
const int VC_MS3 = 4;
const int VC_MS4 = 5;
const int VC_MS5 = 6;
const int VC_STBDSTATION = 7;
const int VC_AFTPILOT = 8;
const int VC_RMSSTATION = 9;
const int VC_PORTSTATION = 10;
const int VC_AFTWORKSTATION = 11;
const int VC_DOCKCAM = 12;
const int VC_PLBCAMFL = 13;
const int VC_PLBCAMFR = 14;
const int VC_PLBCAMBL = 15;
const int VC_PLBCAMBR = 16;
const int VC_RMSCAM = 17;
const int VC_LEECAM = 18;
//Beginning of Mid deck positions
const int VC_MIDDECK = 100;
//Beginning of external airlock positions
const int VC_EXT_AL = 200;

const static char* VC_LBL_CDR = "Commander seat";
const VECTOR3 VC_POS_CDR = _V(-0.671257, 2.629396, 14.1);
const VECTOR3 VC_HUDPOS_CDR = _V(-0.653, 2.589, 14.614);
const static char* VC_LBL_PLT = "Pilot seat";
const VECTOR3 VC_POS_PLT = _V(0.671257, 2.629396, 14.1);
const VECTOR3 VC_HUDPOS_PLT = _V(0.652, 2.589, 14.614);

const static char* VC_LBL_MS1 = "MS1 seat";
const VECTOR3 VC_POS_MS1 = _V(0.2, 2.629396, 13.5); //0804025, DaveS edit: Moved position forward to a more correct position
const VECTOR3 VC_DIR_MS1 = _V(0.0, 0, 1.0);

const static char* VC_LBL_MS2 = "MS2 seat";
const VECTOR3 VC_POS_MS2 = _V(0.7, 2.75, 13.5); //0804025, DaveS edit: Moved position forward to a more correct position
const VECTOR3 VC_DIR_MS2 = _V(0.0, 0.0, 1.0);

const static char* VC_LBL_COAS = "COAS";
const VECTOR3 VC_POS_COAS = _V( 0.4, 3.15, 12.6);
const VECTOR3 VC_DIR_COAS = _V( 0.0, 0.45399, -0.891007);

const static char* VC_LBL_PORTSTATION = "Port workstation";
const VECTOR3 VC_POS_PORTSTATION = _V(-0.6, 2.95, 13.0);
const VECTOR3 VC_DIR_PORTSTATION = _V(-cos(25.0 * RAD), -sin(25.0 * RAD), 0);

const static char* VC_LBL_AFTPILOT = "Aft pilot station";
const VECTOR3 VC_POS_AFTPILOT = _V(0.4, 3.15, 12.7); //080509, DaveS edit: Moved the position 20 cm forward for better panel readability
const VECTOR3 VC_DIR_AFTPILOT = _V(0, 0, -1);
const VECTOR3 VC_OFSFWD_AFTPILOT = VC_POS_COAS - VC_POS_AFTPILOT;

const static char* VC_LBL_RMSSTATION = "RMS work station"; //080509, DaveS edit: Moved the position 20 cm forward for better panel readability
const VECTOR3 VC_POS_RMSSTATION = _V(-0.4, 3.15, 12.7);
const VECTOR3 VC_DIR_RMSSTATION = _V(0.0, 0.0, -1.0);

const static char* VC_LBL_STBDSTATION = "Starboard workstation";
const VECTOR3 VC_POS_STBDSTATION = _V(0.6, 2.95, 13.0);
const VECTOR3 VC_DIR_STBDSTATION = _V(cos(25.0 * RAD), -sin(25.0 * RAD), 0);

const static char* VC_LBL_AFTWORKSTATION = "Aft Workstation";
const VECTOR3 VC_POS_AFTWORKSTATION = _V(0.0, 2.95, 13.2);
const VECTOR3 VC_DIR_AFTWORKSTATION = _V(0.0, 0.0, -1.0);

const static char* VC_LBL_DOCKCAM = "ODS centerline camera";
const static char* VC_LBL_LEECAM = "RMS EE camera";
const static char* VC_LBL_ELBOWCAM = "RMS Elbow camera";
const static char* VC_LBL_PLBCAMFL = "Payload bay FL camera";
const static char* VC_LBL_PLBCAMFR = "Payload bay FR camera";
const static char* VC_LBL_PLBCAMBL = "Payload bay BL camera";
const static char* VC_LBL_PLBCAMBR = "Payload bay BR camera";

const static char* VC_LBL_MIDDECK = "Mid Deck";
const VECTOR3 VC_POS_MIDDECK = _V(-1.44, 0.0, 13.353);
const VECTOR3 VC_DIR_MIDDECK = _V(0.0, -sin(24.5*RAD), cos(24.5 * RAD));


const static char* VC_LBL_EXT_AL = "External Airlock";
const VECTOR3 VC_POS_EXT_AL = ODS_POS + _V(0.0, 0.1, 0.25);
const VECTOR3 VC_DIR_EXT_AL = _V(0.0, -sin(24.5*RAD), cos(24.5 * RAD));

const double PL_ATTACH_CENTER_Y = -1.80;
const double PL_ATTACH_SIDE_Y = 0.80;
const double PL_ATTACH_SIDE_X = 2.45;

const VECTOR3 DIR_CENTERPL = _V(0.0, 1.0, 0.0);
const VECTOR3 ROT_CENTERPL = _V(0.0, 0.0, 1.0);


const VECTOR3 DIR_PORTPL = _V(1.0, 0.0, 0.0);
const VECTOR3 ROT_PORTPL = _V(0.0, -1.0, 0.0);

const VECTOR3 DIR_STBDPL = _V(-1.0, 0.0, 0.0);
const VECTOR3 ROT_STBDPL = _V(0.0, -1.0, 0.0);

const VECTOR3 OFS_PORTMMU = _V(-PL_ATTACH_SIDE_X, PL_ATTACH_SIDE_Y, 8.0);
const VECTOR3 OFS_STBDMMU = _V( PL_ATTACH_SIDE_X, PL_ATTACH_SIDE_Y, 8.0);


static const char* PAYLOADTYPE[6] = {"XS1P", "XS3P", "XS5P",
	"XS1A", "XS3A", "XS5A"};

const double DEFAULT_PAYLOAD_ZPOS[16] = 
{ 8.0, 0.0, -8.0,		//Center active
  4.0, 2.0, -6.0,		//Center passive
  7.0, 3.0, -2.0, -8.0,	//Port
  7.0, 3.0, -2.0, -8.0,	//Starboard
  0.0, 0.0};			//spare

//Sound IDs
const int RCS_SOUND = 1;
const static char* RCS_SOUND_FILE = "Sound\\Vessel\\attsustain.wav";
const int MASTER_ALARM = 2;
const static char* MASTER_ALARM_FILE = "Sound\\ShuttleUltra\\Master_Alarm.wav";
const int APU_START = 3;
const static char* APU_START_FILE = "Sound\\ShuttleUltra\\APU_start_up.wav";
const int APU_RUNNING = 4;
const static char* APU_RUNNING_FILE = "Sound\\ShuttleUltra\\APU_continues.wav";
const int APU_SHUTDOWN = 5;
const static char* APU_SHUTDOWN_FILE = "Sound\\ShuttleUltra\\APU_shut_down.wav";
const int SSME_START = 6;
const static char* SSME_START_FILE = "Sound\\ShuttleUltra\\SSME_ignition.wav";
const int SSME_RUNNING = 7;
const static char* SSME_RUNNING_FILE = "Sound\\ShuttleUltra\\SSME_sustain.wav";

// ==========================================================
// Mesh group indices for some components
// ==========================================================

const UINT MESH_UNDEFINED = (UINT)-1;

 const int MFDGROUPS[11] = {
    GRP_CDR1_VC,GRP_CDR2_VC,GRP_PLT1_VC,GRP_PLT2_VC,
    GRP_MFD1_VC, GRP_MFD4_VC, GRP_MFD3_VC, GRP_MFD_aft_VC, GRP_MFD2_VC, GRP_MFD5_VC,
	GRP_MFD11_VC
    };

// ==========================================================
// I-Loaded values
// ==========================================================

const double defaultStage1Guidance[2][8] = {{0.0, 136.855, 219.456, 363.3216, 562.9656, 882.0912, 1236.8784, 1516.38}, //speed
											{90.0, 78.0, 68.0, 61.0, 53.0, 39.0, 30.0, 20.5}}; //pitch

const double LAUNCH_SITE[2] = {28.608, 34.581}; // 0=KSC, 1=VAFB



// ==========================================================
// panel area identifiers
// ==========================================================

// define MFD function buttons
#define AID_CDR1_BUTTONS   1
#define AID_CDR2_BUTTONS   2
#define AID_PLT1_BUTTONS   3
#define AID_PLT2_BUTTONS   4
#define AID_CRT1_BUTTONS   5
#define AID_CRT2_BUTTONS   6
#define AID_CRT3_BUTTONS   7
#define AID_CRT4_BUTTONS   8
#define AID_MFD1_BUTTONS   9
#define AID_MFD2_BUTTONS   10
#define AID_AFD_BUTTONS  11
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
#define AID_R13L_TKBK1   101
#define AID_R13L_TKBK2   102
#define AID_R13L_TKBK3   103
#define AID_R13L_TKBK4   104
#define AID_R13L_TKBK5   105
#define AID_R13L_TKBK6   106
#define AID_R13L_MAX     120
// Panel F6 (commander's panel)
#define AID_F6_MIN     200
#define AID_F6         200
#define AID_F6_TKBK1   201
#define AID_F6_TKBK2   202
#define AID_F6_TKBK3   203
#define AID_F6_MAX     220
// Panel F7 (event timer and MPS status)
#define AID_F7_MIN	   270
#define AID_F7	       270
#define AID_F7_EVTTMR1 271
#define AID_F7_EVTTMR2 272
#define AID_F7_EVTTMR3 273
#define AID_F7_EVTTMR4 274
#define AID_F7_MAX	   279
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
#define AID_O3_METTMR1 331
#define AID_O3_METTMR2 332
#define AID_O3_RCS     333
#define AID_O3_MAX	   349
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
#define AID_A4_EVTTMR  421
#define AID_A4_METTMR1 422
#define AID_A4_METTMR2 423
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
#define AID_A8_MAX     459

#define AID_F8_MIN	   460
#define AID_F8		   460
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
#define AID_A6_MAX     529

#define AID_R11_MIN		540
#define AID_R11			540
#define AID_R11_MAX		560

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

const UINT AID_CUSTOM_PANELS_MIN = 10000;
const UINT AID_CUSTOM_PANELS_MAX = 30000;

const UINT AID_A7A3 = 11000;
const UINT AID_A8A3 = 11010;


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







typedef struct {
	HINSTANCE hDLL;
	SURFHANDLE pbi_lights;
	SURFHANDLE tkbk_label;
	SURFHANDLE clock_digits;
	SURFHANDLE digits_7seg;
	HBITMAP deu_characters;
	HDC DeuCharBitmapDC;
	HFONT font[1];
} GDIParams;

typedef struct {
	VECTOR3 Pitch;
	VECTOR3 Yaw;
	VECTOR3 Roll;			
} RefPoints;

typedef struct {
	double PRI_ROT_RATE, PRI_ATT_DB, PRI_RATE_DB, PRI_ROT_PLS, PRI_COMP, PRI_TRAN_PLS;
	int PRI_P_OPTION, PRI_Y_OPTION; //0=ALL, 1=NOSE, 2=TAIL
	double ALT_RATE_DB, ALT_ON_TIME, ALT_DELAY;
	int ALT_JET_OPT, ALT_JETS;
	double VERN_ROT_RATE, VERN_ATT_DB, VERN_RATE_DB, VERN_ROT_PLS, VERN_COMP;
	int VERN_CNTL_ACC;
} DAPConfig;

typedef struct {
	int START_TIME[4]; // day,hour,min,sec
	VECTOR3 TargetAttM50;
	MATRIX3 LVLHTgtOrientationMatrix;
	enum {OFF, MNVR, TRK, ROT} Type;
} AttManeuver;

typedef struct {
	bool OPS, ITEM, SPEC, EXEC, PRO;
	bool NewEntry; //used by CRT MFD to output scratch pad
	char input[255];
	int InputSize; //number of chars used
} KeyboardInput;

enum AXIS {PITCH, YAW, ROLL};

class PanelA4;
class PanelC2;
//class PanelF7;
class PanelO3;
class SubsystemDirector;
class OMSSubsystem;
class AirDataProbeSystem;

typedef enum {
	LT_LATCHED = 0,
	LT_OPENING,
	LT_CLOSING,
	LT_RELEASED
} LATCH_STATE;

typedef enum {
	OMS_LEFT = 0,
	OMS_KIT,
	OMS_RIGHT
} OMS_REF;

class CommModeHandler;
using discsignals::DiscreteBundleManager;


// ==========================================================
// Interface for derived vessel class: Atlantis
// ==========================================================

class Atlantis: public VESSEL2 {
	friend class PayloadBayOp;
	friend class GearOp;
	friend class PanelA4;
	friend class PanelA8;
	friend class PanelC2;
	friend class PanelC3;
	friend class vc::PanelF7;
	friend class PanelO3;
	friend class PanelR2;
	friend class Keyboard;
	friend class CRT;
	friend class vc::MDU;
	friend class vc::DAPControl;
	friend class dps::GNCSoftware; //temporary
	friend BOOL CALLBACK RMS_DlgProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	friend BOOL CALLBACK PAYCAM_DlgProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	/* **************************************************
	 * Subsystem short cuts
	 * **************************************************/
	dps::MasterTimingUnit* pMTU;		//just quick reference. Don't ever delete this, yourself.
	dps::IDP* pIDP[4];
	dps::GPC* pGPC[5];
	OMSSubsystem* pOMS;
	mps::SSME* pSSME[3];
	/**
	 * Strategy pattern for the external airlock subsystem
	 */
	eva_docking::BasicExternalAirlock* pExtAirlock;
	AirDataProbeSystem* pADPS;

	AnimState::Action spdb_status;
	int ___iCurrentManifold;
    char WingName[256];

	// Actual Virtual Cockpit Mode
	int VCMode;
	/**
	 * Structural configuration
	 * - 0 launch configuration
	 * - 1 SRB's engaged
	 * - 2 SRB's separated
	 * - 3 Tank separated/orbiter only
	 */
	int status;  
	bool bManualSeparate; // flag for user-induced booster or tank separation

	double t0;          // reference time: designated liftoff time
	double met;
	int MET[4], Launch_time[4], MET_Add[4]; // day,hour,min,sec
	WORD srb_id1, srb_id2;

	enum {
		VCM_FLIGHTDECK = 0,
		VCM_MIDDECK,
		VCM_AIRLOCK
	} vcDeckMode;

	/* **************************************************************
	 * Mesh indices for use in objects  
	 ****************************************************************/

	UINT mesh_orbiter;                         // index for orbiter mesh
	UINT mesh_cockpit;                         // index for cockpit mesh for external view
	UINT mesh_vc;                              // index for virtual cockpit mesh
	UINT mesh_panela8;						   // index for Panel A8 mesh
	UINT mesh_middeck;                         // index for mid deck mesh
	UINT mesh_rms;							   // index for RMS mesh
	UINT mesh_mpm;							   // index for STBD MPM mesh
	UINT mesh_tank;                            // index for external tank mesh
	UINT mesh_srb[2];                          // index for SRB meshes
	UINT mesh_kuband;						   // index for KU band antenna mesh
	UINT mesh_extal;						   // index for external airlock mesh
	UINT mesh_ods;							   // index for	ODS outside mesh
	UINT mesh_cargo_static;					   // index for static cargo mesh
	UINT mesh_dragchute;					   // index for drag chute mesh

public:
	//**********************************************************
	//* public methods
	//**********************************************************
	Atlantis (OBJHANDLE hObj, int fmodel);
	~Atlantis();
	void AddOrbiterVisual (const VECTOR3 &ofs);
	void AddSRBVisual (int which, const VECTOR3 &ofs);
	void AddTankVisual (const VECTOR3 &ofs);
	virtual DiscreteBundleManager* BundleManager() const;
	VECTOR3 CalcAnimationFKArm();
	void CalcAnimationFKArm(VECTOR3 &pos, VECTOR3 &dir);
	// Overloaded callback functions
	void clbkAnimate (double simt);
	int  clbkConsumeBufferedKey (DWORD key, bool down, char *kstate);
	void clbkDrawHUD (int mode, const HUDPAINTSPEC *hps, HDC hDC);
	void clbkFocusChanged (bool getfocus, OBJHANDLE hNewVessel, OBJHANDLE hOldVessel);
	bool clbkLoadGenericCockpit ();
	void clbkLoadStateEx (FILEHANDLE scn, void *vs);
	bool clbkLoadVC (int id);
	void clbkMFDMode (int mfd, int mode);
	bool clbkPlaybackEvent (double simt, double event_t, const char *event_type, const char *event);
	void clbkPostCreation ();
	void clbkPostStep (double simt, double simdt, double mjd);
	void clbkPreStep (double simT, double simDT, double mjd);
	void clbkSaveState (FILEHANDLE scn);
	void clbkSetClassCaps (FILEHANDLE cfg);
	void clbkSetStateEx (const void *status);
	bool clbkVCMouseEvent (int id, int event, VECTOR3 &p);
	bool clbkVCRedrawEvent (int id, int event, SURFHANDLE surf);
	void clbkVisualCreated (VISHANDLE vis, int refcount);
	void clbkVisualDestroyed (VISHANDLE vis, int refcount);
	void ClearMeshes ();
	void DefineAnimations (void);
	void DefineAttachments (const VECTOR3& ofs0);
	/* **********************************************************
	 * Getters
	 * **********************************************************/
	virtual short GetETPropellant(unsigned short usGPCID);
	virtual unsigned short GetGPCMET(unsigned short usGPCID, unsigned short &usDay, unsigned short &usHour, unsigned short& usMin, unsigned short &usSec);
	virtual short GetGPCRefHDot(unsigned short usGPCID, double& fRefHDot);
	virtual unsigned short GetGPCLVLHVel(unsigned short usGPCID, VECTOR3& vel);
	virtual dps::IDP* GetIDP(unsigned short usIDPNumber) const;
	virtual short GetLastCreatedMFD() const;
	virtual bool GetLiftOffFlag() const;
	virtual vc::MDU* GetMDU(unsigned short usMDUID) const;
	virtual double GetOMSPressure(OMS_REF oms_ref, unsigned short tank_id);
	virtual const VECTOR3& GetOrbiterCoGOffset() const;
	virtual short GetSRBChamberPressure(unsigned short which_srb);
	virtual bool HasExternalAirlock() const;
	virtual bool IsValidSPEC(int gpc, int spec);
	double GetThrusterGroupMaxThrust(THGROUP_HANDLE thg);
	double GetPropellantLevel(PROPELLANT_HANDLE ph);
	void Jettison ();
	void OperateSpeedbrake (AnimState::Action action);
	void PaintMarkings (SURFHANDLE tex);
	virtual bool RegisterMDU(unsigned short usMDUID, vc::MDU* pMDU);
	void RevertSpeedbrake ();
	void SeparateBoosters (double srb_time);
	void SeparateMMU (void);
	void SeparateTank (void);
	/* ***************************************************************
	 * Setters
	 *****************************************************************/
	virtual void SetAirDataProbeDeployment(int side, double position);
	void SetAnimationArm (UINT anim, double state);
	void SetAnimationIKArm(VECTOR3 arm_dpos);
	void SetBayDoorLatchPosition (int gang, double pos);
	void SetBayDoorPosition (double pos);
	void SetETUmbDoorPosition(double pos, int door);
	void SetStbdMPMPosition(double pos);
	void SetKuAntennaPosition (double pos);
	virtual void SetKuGimbalAngles(double fAlpha, double fbeta);
	void SetLastCreatedMFD(unsigned short usMDU);
	void SetLaunchConfiguration (void);
	void SetOrbiterConfiguration (void);
	void SetOrbiterTankConfiguration (void);
	void SetPostLaunchConfiguration (double srbtime);
	void SetRadiatorPosition (double pos);
	void SetRadLatchPosition (double pos) {}
	void SetSpeedbrake (double tgt);
	virtual void SetExternalAirlockVisual(bool fExtAl, bool fODS);
	/**
	 * @param usMPSNo numerical ID of the SSME
	 * @param fThrust0 Vacuum thrust
	 * @param fISP0 Vacuum ISP
	 * @param fISP1 Sealevel ISP
	 * @return false for failure, true for success
	 */
	bool SetSSMEParams(unsigned short usMPSNo, double fThrust0, double fISP0, double fISP1);
	/**
	 * @param usMPSNo numerical ID of the SSME
	 * @param dir direction vector of the SSME force vector
	 * @return false for failure, true for success
	 */
	bool SetSSMEDir(unsigned short usMPSNo, const VECTOR3& dir);

	virtual void TurnOnPadLights();
	virtual void TurnOffPadLights();
	void ToggleGrapple (void);
	void AutoGrappleSequence();
	void AutoReleaseSequence();
	void ToggleArrest (void);
	void UpdateMesh ();
	void UpdateRMSPositions();
	void UpdateRMSAngles();
	void UpdateSSMEGimbalAnimations();
	//VECTOR3 CalcAnimationFKArm2();

	/*
	void RegisterVC_CdrMFD ();
	void RegisterVC_PltMFD ();
	void RegisterVC_CntMFD ();
	void RegisterVC_AftMFD ();
	void RedrawPanel_MFDButton (SURFHANDLE surf, int mfd);
	*/

	
	//mission::Mission* the_mission;

	//Extended SRB smoke effects
	
	/**
	 * Pointer to the A7A8 custom panel region
	 */
	vc::BasicPanel* pA7A8Panel;
	


	//double kubd_proc; // Ku-band antenna deployment state (0=retracted, 1=deployed)
	double spdb_proc, spdb_tgt; // Speedbrake deployment state (0=fully closed, 1=fully open)
	double ldoor_drag, rdoor_drag; // drag components from open cargo doors
	bool center_arm;
	bool arm_moved;
	double center_arm_t;
	bool do_eva;
	bool do_plat;
	bool do_cargostatic;
	VECTOR3 orbiter_ofs;
	VECTOR3 ofs_sts_sat;
	VECTOR3 cargo_static_ofs;
	VISHANDLE vis;      // handle for visual - note: we assume that only one visual per object is created!
	MESHHANDLE hOrbiterMesh, hOrbiterCockpitMesh, hOrbiterVCMesh, 
		hMidDeckMesh, hOrbiterRMSMesh, hOBSSMPMMesh, hTankMesh, hSRBMesh[2],
		hODSMesh, hPanelA8Mesh, hDragChuteMesh; // mesh handles
	MESHHANDLE hKUBandMesh;
	MESHHANDLE hExtALMesh;
	char cargo_static_mesh_name[256];

	double fPayloadZPos[16];
	double fPayloadMass[16];
	unsigned short usPayloadType[16];
	//C-P attachments
	ATTACHMENTHANDLE ahHDP;
	ATTACHMENTHANDLE ahTow;
	//P-C attachments
	/**
	 * @deprecated
	 */
	ATTACHMENTHANDLE ahRMS, ahOBSS;
	ATTACHMENTHANDLE ahMMU[2];
	ATTACHMENTHANDLE ahDockAux;
	ATTACHMENTHANDLE ahExtAL[2];
	ATTACHMENTHANDLE ahCenterActive[3];
	ATTACHMENTHANDLE ahCenterPassive[4];
	ATTACHMENTHANDLE ahStbdPL[4];
	ATTACHMENTHANDLE ahPortPL[4];
	
	VECTOR3 arm_tip[3];
	//VECTOR3 wrist_yaw_joint[2];

	

	

	SubsystemDirector* psubsystems;
	
	PayloadBayOp *plop; // control and status of payload bay operations
	//GearOp *gop; // control and status of landing gear
	PanelA4 *panela4;
	PanelA8 *panela8;
	PanelC2 *panelc2;
	PanelC3 *panelc3; // PanelC3 operations
	//PanelF7 *panelf7;
	PanelO3 *panelo3;
	PanelR2 *r2d2; // PanelR2 operations
	vc::DAPControl *dapcontrol;
	Keyboard *CDRKeyboard;
	Keyboard *PLTKeyboard;
	bool PitchActive,YawActive,RollActive;     // Are RCS channels active?

	OBJHANDLE ThisVessel;

	

private:
	double slag1, slag2, slag3;
	PSTREAM_HANDLE pshSlag1[2], pshSlag2[2], pshSlag3[2];

	bool bSRBCutoffFlag;
	bool bLiftOff;
	bool bHasKUBand;
	bool bHasODS;
	bool bMidDeckVisible;

	CommModeHandler* pCommModeHandler;

	/* *************************************************
	 * Panel groups
	 * *************************************************/
	vc::PanelGroup pgForward;
	vc::PanelGroup pgLeft;
	vc::PanelGroup pgCenter;
	vc::PanelGroup pgRight;
	vc::PanelGroup pgOverhead;
	vc::PanelGroup pgAftStbd;
	vc::PanelGroup pgAft;
	vc::PanelGroup pgAftPort;


	//GPC programs
	dps::GNCSoftware *gncsoftware;

	SURFHANDLE tex_rcs;
	void StopAllManifolds();
	void FireAllNextManifold();
	void AddPrimaryRCSExhaust(THRUSTER_HANDLE thX);
	void CreateRightARCS(const VECTOR3& ref_pos);
	void CreateLeftARCS(const VECTOR3& ref_pos);
	void CreateFRCS(const VECTOR3& ref_pos);
	void CreateMPSGOXVents(const VECTOR3& ref_pos);
	bool bUseRealRCS;
	void CreateOrbiterTanks();
	unsigned short usCurrentPlayerChar;
	bool bCommMode;
	void DefineSSMEExhaust();
	void SignalGSEBreakHDP();
	void SignalGSEStart();
	//-----------------------------------
	void ShowMidDeck();
	void HideMidDeck();
	void ShowODS();
	void HideODS();
	/*void IlluminateMesh(UINT idx);
	void IlluminateMesh(UINT idx, vector<DWORD> vExclude); //NOTE: vExclude MUST be in ascending order
	void DisableIllumination(UINT idx, MESHHANDLE GlobalMesh);*/
	//-----------------------------------
	void DefineKUBandAnimations();
	void LaunchClamps();
	void CreateAttControls_RCS(VECTOR3 center);
	void DisableAllRCS();
	void EnableAllRCS();
	void DisableControlSurfaces();
	void EnableControlSurfaces();

	//Landing Gear
	void DeployLandingGear();
	void RetractLandingGear();
	void ArmGear();
	void DefineTouchdownPoints();
	bool GearArmed() const;
	void DeployDragChute();
	void JettisonDragChute();

	//
	void SavePayloadState(FILEHANDLE scn) const;
	bool ParsePayloadLine(const char* pszLine);

	/**
	 * React on Key "V", switching the view from flight deck to Mid Deck
	 * and back.
	 */
	void ToggleVCMode();
	
	//RMS
	bool ArmCradled();
	void UpdateMPMMicroswitches();
	void UpdateMRLMicroswitches();
	bool SatGrappled() const { return GetAttachmentStatus (ahRMS) != 0; }
	bool SatStowed() const;
	ATTACHMENTHANDLE CanArrest() const;

	//Launch
	void AutoMainGimbal();
	void SteerGimbal();
	void RateCommand();
	void Throttle(double dt);
	void FailEngine(int engine);
	void InitializeAutopilot();
	double CalcNetThrust();
	void CalcThrustAngles();
	double CalculateAzimuth();
	//PEG
	void MajorCycle();
	void Navigate();
	void Estimate();
	void Guide();

	//GPC
	void GPC(double dt);
	void Maneuver(double dt);
	void SetILoads();
	bool Input(int mfd, int change, char *Name, char *Data=NULL);
	//void Test();

	//DAP
	void GimbalOMS(VECTOR3 Targets);
	void LoadManeuver();
	void UpdateDAP(); //updates rot rates, torques
	void TransControl(double SimdT);
	void AttControl(double SimdT);
	void AerojetDAP(double SimdT);
	void StartAttManeuver(); //initiates maneuver loaded into CurManeuver
	void LoadInertialManeuver();
	void LoadTrackManeuver();
	void LoadRotationManeuver();
	void CalcManeuverTargets(VECTOR3 NullRates);
	void SetRates(VECTOR3 &Rates);
	void CalcRequiredRates(VECTOR3 &Rates);
	void CalcRequiredRates(VECTOR3 &Rates, const VECTOR3 &NullRates);
	//change ref. frames
	VECTOR3 ConvertAnglesBetweenM50AndOrbiter(const VECTOR3 &Angles, bool ToOrbiter=false);
	MATRIX3 ConvertMatrixBetweenM50AndOrbiter(const MATRIX3 &RotMatrix, bool ToOrbiter=false);
	VECTOR3 ConvertOrbiterAnglesToLocal(const VECTOR3 &Angles);
	VECTOR3 ConvertVectorBetweenOrbiterAndM50(const VECTOR3 &Input);
	VECTOR3 ConvertLVLHAnglesToM50(const VECTOR3 &Input);
	MATRIX3 ConvertLVLHAnglesToM50Matrix(const VECTOR3 &Input);
	//calc attitude/attitude error
	VECTOR3 CalcLVLHAttitude();
	VECTOR3 CalcRelLVLHAttitude(VECTOR3 &Target);
	VECTOR3 CalcPitchYawRollAngles(VECTOR3 &RelAttitude);
	MATRIX3 CalcPitchYawRollRotMatrix(); //handles M50 coordinates
	VECTOR3 CalcPitchYawRollAngles(); //handles M50 coordinates
	//bool CheckLimits(double dNum1, double dNum2, double dLim);
	double NullStartAngle(double Rates, AXIS Axis);

	//Thruster Control
	void EnableThrusters(const int Thrusters[], int nThrusters);
	void DisableThrusters(const int Thrusters[], int nThrusters);
	void UpdateTranslationForces();
	bool RCSThrustersFiring();

	//Math
	VECTOR3 GetPYR(VECTOR3 Pitch, VECTOR3 YawRoll);
	VECTOR3 GetPYR2(VECTOR3 Pitch, VECTOR3 YawRoll);
	//returns rotation axis and angle of rotation (in radians)
	double CalcEulerAngle(const MATRIX3 &RefAngles, const MATRIX3 &TargetAngles, VECTOR3 &Axis);
	double CalcEulerAngle(const VECTOR3 &RefAngles, const VECTOR3 &TargetAngles, VECTOR3 &Axis);
	void RotateVector(const VECTOR3 &Initial, const VECTOR3 &Angles, VECTOR3 &Result);
	void RotateVectorPYR(const VECTOR3 &Initial, const VECTOR3 &Angles, VECTOR3 &Result);
	VECTOR3 GetAnglesFromMatrix(MATRIX3 RotMatrix); //returns angles in radians
	void GetRotMatrixX(double Angle, MATRIX3 &RotMatrixX);
	void GetRotMatrixY(double Angle, MATRIX3 &RotMatrixY);
	void GetRotMatrixZ(double Angle, MATRIX3 &RotMatrixZ);
	void MultiplyByMatrix(const VECTOR3 &Initial, const MATRIX3 &RotMatrix, VECTOR3 &Result);
	inline VECTOR3 Normalize(const VECTOR3 &a)
	{
		return (a/length(a));
	}
	inline VECTOR3 Projection(const VECTOR3 &u, const VECTOR3 &v) //Proj u onto v
	{
		return v*(dotp(u, v)/dotp(v, v));
	}
	inline VECTOR3 ToRad(const VECTOR3 &Input)
	{
		VECTOR3 Output;
		for(int i=0;i<3;i++) {
			Output.data[i]=Input.data[i]*RAD;
		}
		return Output;
	}
	inline VECTOR3 ToDeg(const VECTOR3 &Input)
	{
		VECTOR3 Output;
		for(int i=0;i<3;i++) {
			Output.data[i]=Input.data[i]*DEG;
		}
		return Output;
	}
	inline VECTOR3 NormZ(VECTOR3 &v)
	{
		VECTOR3 o=v;
		o.z=sqrt(1.0-pow(v.x,2)-pow(v.y,2));
		return o;
	}
	inline VECTOR3 RotateVectorX(const VECTOR3 &v, double angle) //rotates about angle in X-axis
	{
		VECTOR3 Output;
		Output.x=v.x;
		Output.z=v.z*cos(angle*RAD)-v.y*sin(angle*RAD);
		Output.y=v.y*cos(angle*RAD)+v.z*sin(angle*RAD);
		return Output;
	}
	inline VECTOR3 RotateVectorY(const VECTOR3 &v, double angle) //rotates about angle in Y-axis
	{
		VECTOR3 Output;
		Output.y=v.y;
		Output.x=v.x*cos(angle*RAD)-v.z*sin(angle*RAD);
		Output.z=v.z*cos(angle*RAD)+v.x*sin(angle*RAD);
		return Output;
	}
	inline VECTOR3 RotateVectorZ(const VECTOR3 &v, double angle) //rotates about angle in Z-axis
	{
		VECTOR3 Output;
		Output.x=v.x*cos(angle*RAD)-v.y*sin(angle*RAD);
		Output.y=v.x*sin(angle*RAD)+v.y*cos(angle*RAD);
		Output.z=v.z;
		return Output;
	}
	inline bool Eq(const double d1, const double d2, double dDiff=0.00001)
	{
		if(fabs(d1-d2)>dDiff) return false;
		return true;
	}
	inline double range(double min, double value, double max)
	{
		if(value<min) return min;
		if(value>max) return max;
		return value;
	}
	inline double a(double t) {
		return a0/(1-t/tau);
	}
	inline double b0(double TT) {
		return -Isp*log(1-TT/tau);
	}
	inline double bn(double TT, int n) {
		if(n==0) return b0(TT);
		return bn(TT,n-1)*tau-Isp*pow(TT,n)/n;
	}
	inline double c0(double TT) {
		return b0(TT)*TT-bn(TT,1);
	}
	inline double cn(double TT, int n) {
		if(n==0) return c0(TT);
		return cn(TT,n-1)*tau-Isp*pow(TT,n+1)/(n*(n+1));
	}

	// *******************************************************************************
	// Animations
	// *******************************************************************************
	UINT anim_door;                            // handle for cargo door animation
	UINT anim_rad;                             // handle for radiator animation
	UINT anim_clatch[4];					   // handle for center line latch gangs
	/*UINT anim_clatch1_4;					   // handle for center line latches 1-4
	UINT anim_clatch5_8;						// handle for center line latches 5-8
	UINT anim_clatch9_12;						// handle for center line latches 9-12
	UINT anim_clatch13_16;						// handle for center line latches 13-16*/

	UINT anim_portTS;							//Port Torque Shaft animation (0°...135°)

	UINT anim_kubd;                            // handle for Ku-band antenna animation
	UINT anim_elev;                            // handle for elevator animation
	UINT anim_bf;                              // handle for body flap animation
	UINT anim_laileron;						   // handle for left aileron animation
	UINT anim_raileron;						   // handle for right aileron animation
	UINT anim_rudder;						   // handle for rudder animation
	UINT anim_spdb;                            // handle for speed brake animation
	UINT anim_dummy;						   // handle for dummy animation
	UINT anim_letumbdoor;					   // handle for left ET umbilical door animation
	UINT anim_retumbdoor;					   // handle for right ET umbilical door animation
	UINT anim_gear;                            // handle for landing gear animation
	UINT anim_chute_deploy;					   // handle for drag chute deploy animation
	UINT anim_chute_spin;					   // handle for chute spinning
	
	//SSME GIMBAL ANIMATIONS
	UINT anim_ssmeTyaw;
	UINT anim_ssmeTpitch;
	UINT anim_ssmeRyaw;
	UINT anim_ssmeRpitch;
	UINT anim_ssmeLyaw;
	UINT anim_ssmeLpitch;

	//=======================================================
	// Air data probe animations
	//=======================================================
	UINT anim_adpl;
	UINT anim_adpr;

	//=======================================================
	// KU Band antenna gimbal animations
	//=======================================================
	UINT anim_kualpha;
	UINT anim_kubeta;


	// PAYLOAD CAMERAS ANIMATIONS
	UINT anim_camFLyaw;						   // handle for front-left payload camera yaw animation 
	UINT anim_camFLpitch;					   // handle for front-left payload camera pitch animation 
	UINT anim_camFRyaw;						   // handle for front-right payload camera yaw animation 
	UINT anim_camFRpitch;					   // handle for front-right payload camera pitch animation 
	UINT anim_camBLyaw;						   // handle for back-left payload camera yaw animation 
	UINT anim_camBLpitch;					   // handle for back-left payload camera pitch animation 
	UINT anim_camBRyaw;						   // handle for back-right payload camera yaw animation 
	UINT anim_camBRpitch;					   // handle for back-right payload camera pitch animation 
	UINT anim_camRMSElbowPan;
	UINT anim_camRMSElbowTilt;
	
	// PAYLOAD CAMERAS ROTATION (-170 to 170 degrees)
	double camFLyaw;
	double camFLpitch;
	double camFRyaw;
	double camFRpitch;
	double camBLyaw;
	double camBLpitch;
	double camBRyaw;
	double camBRpitch;
	double camRMSElbowPan, camRMSElbowTilt;
	//RMS Camera rot/direction
	VECTOR3 camRMSElbowLoc[2];
	
	// Selected camera must be moved at low rate (if false at high rate)
	bool cameraLowRate;

	// If a camera have been moved (used for reposition camera animations)
	bool cameraMoved;
	
	// Selected camera for control
	int cameraControl;  // 0:FL 1:FR 2:BL 3:BR 4:RMS Elbow
	
	// Transform for the cameras
	MGROUP_TRANSFORM *CameraFLYaw;
	MGROUP_TRANSFORM *CameraFLPitch;
	MGROUP_TRANSFORM *CameraFRYaw;
	MGROUP_TRANSFORM *CameraFRPitch;
	MGROUP_TRANSFORM *CameraBLYaw;
	MGROUP_TRANSFORM *CameraBLPitch;
	MGROUP_TRANSFORM *CameraBRYaw;
	MGROUP_TRANSFORM *CameraBRPitch;
	
	// Sets the camera positions and animations.
	void SetAnimationCameras();

	//gear/drag chute
	AnimState gear_status;
	bool gear_armed;
	bool DragChuteDeploying; //used to command drag chute deploy
	enum{STOWED, DEPLOYING, REEFED, INFLATED, JETTISONED} DragChuteState;
	double DragChuteDeployTime; //time at which deploy command was received
	double DragChuteSize; //0 (Stowed/Jettisoned) or 0.4(Reefed) or 1.0(Deployed)
	AnimState DragChuteSpin;
	
	PROPELLANT_HANDLE ph_oms, ph_tank, ph_srb, ph_frcs; // handles for propellant resources
	PROPELLANT_HANDLE ph_lrcs, ph_rrcs, ph_controller;
	THRUSTER_HANDLE th_main[3];                // handles for orbiter main (SSME) engines
	THRUSTER_HANDLE th_oms[2];               // handles for orbiter OMS engines
	THRUSTER_HANDLE th_srb[2];                 // handles for SRB engines
    THRUSTER_HANDLE th_att_rcs[18];            // 12 for rotation, 6 virtual
	THRUSTER_HANDLE th_att_lin[10];

	/**
	 * Used for the preflight GOX venting of the SSMEs.
	 */
	THRUSTER_HANDLE th_ssme_gox[3];
	/**
	 * To be used for visualizing the LOX MPS dump after MECO. 
	 */
	THRUSTER_HANDLE th_ssme_loxdump[3];

	//<<<< Begin new RCS model here
	//Array collecting all primary jets
	/** Forward Manifold 1
	 */
	THRUSTER_HANDLE thManFRCS1[4];		
	/** Forward Manifold 2
	 */
	THRUSTER_HANDLE thManFRCS2[4];
	/** Forward Manifold 3
	 */
	THRUSTER_HANDLE thManFRCS3[4];		
	/** Forward Manifold 4
	 */
	THRUSTER_HANDLE thManFRCS4[2];
	/** Forward Manifold 5
	 */
	THRUSTER_HANDLE thManFRCS5[2];		

	/** Left Manifold 1
	 */
	THRUSTER_HANDLE thManLRCS1[3];		
	/** Left Manifold 2
	 */
	THRUSTER_HANDLE thManLRCS2[3];
	/** Left Manifold 3
	 */
	THRUSTER_HANDLE thManLRCS3[3];		
	/** Left Manifold 4
	 */
	THRUSTER_HANDLE thManLRCS4[3];
	/** Left Manifold 5
	 */
	THRUSTER_HANDLE thManLRCS5[2];

	/** Right Manifold 1
	 */
	THRUSTER_HANDLE thManRRCS1[3];		
	/** Right Manifold 2
	 */
	THRUSTER_HANDLE thManRRCS2[3];
	/** Right Manifold 3
	 */
	THRUSTER_HANDLE thManRRCS3[3];		
	/** Right Manifold 4
	 */
	THRUSTER_HANDLE thManRRCS4[3];
	/** Right Manifold 5
	 */
	THRUSTER_HANDLE thManRRCS5[2];
	//>>>> End of new RCS model
	THGROUP_HANDLE thg_pitchup, thg_pitchdown, thg_yawleft, thg_yawright, thg_rollleft, thg_rollright;
	THGROUP_HANDLE thg_transfwd, thg_transaft, thg_transup, thg_transdown, thg_transright, thg_transleft;
	VECTOR3 TransForce[2]; //force provided by translation groups; 0=plus-axis
	UINT ex_main[3];						   // main engine exhaust
	UINT ex_retro[2];						   // OMS exhaust
	bool RCSEnabled;
	THGROUP_HANDLE thg_main, thg_srb, thg_retro;          // handles for thruster groups
	CTRLSURFHANDLE hrudder, hlaileron, hraileron, helevator, hbodyflap;
	bool ControlSurfacesEnabled;

	bool bSSMEGOXVent;

	//CLR
	VECTOR3 SSMET_GOX_REF1;
	VECTOR3 SSMEL_GOX_REF1;
	VECTOR3 SSMER_GOX_REF1;

	// APU Fuel Tanks
	PROPELLANT_HANDLE apu_tank[3], oms_helium_tank[2], mps_helium_tank[3];
	int MPS_Manifold_Press[2]; //0=LO2, 1=LH2
	int MPS_He_Reg_Press[4], MPS_He_Tank_Press[4];
	int Hydraulic_Press[3];

	// RMS arm animation status
	ANIMATIONCOMPONENT_HANDLE hAC_arm, hAC_sat, hAC_satref;
	MGROUP_TRANSFORM *rms_anim[7], *rms_rollout_anim;
	UINT anim_arm_sy, anim_arm_sp, anim_arm_ep, anim_arm_wp, anim_arm_wy, anim_arm_wr, anim_arm_ee;
	UINT anim_rollout;
	UINT anim_stbd_mpm;
	VECTOR3 obss_attach_point[2];
	double arm_sy, arm_sp, arm_ep, arm_wp, arm_wy, arm_wr;
	double sy_angle, sp_angle, ep_angle, wp_angle, wy_angle, wr_angle;
	double sp_null, ep_null; //required to compensate for elbow joint being 'below' booms
	
	//RMS
	AnimState RMSRollout, StbdMPMRollout;
	int MPM_Microswitches[2][2]; //0=PORT/STO, 1=STBD/DPLY
	double shoulder_brace;
	double MRL[2]; //0=PORT, 1=STBD
	int MRL_FwdMicroswitches[2][3], MRL_MidMicroswitches[2][3], MRL_AftMicroswitches[2][3]; //0=PORT/LAT, 1=STBD/REL, 2=RDY
	bool RMS;
	bool STBDMPM;
	bool mpm_moved;
	bool DisplayJointAngles;	
	//Grapple/Release sequences
	bool bGrappleInProgress, bReleaseInProgress;
	AnimState Grapple, Rigidize, Extend;
	int EEGrappleMode; //0=OFF, 1=MAN, 2=AUTO
	//IK parameters
	VECTOR3 arm_wrist_pos;
	VECTOR3 arm_ee_pos, arm_ee_dir, arm_ee_rot;
	VECTOR3 arm_tgt_pos, arm_tgt_dir;
	double lu, ll, wp_wy, wy_ee;
    double shoulder_neutral;
    double shoulder_range,shoulder_min,shoulder_max;
    double elbow_neutral;
    double elbow_range,elbow_min,elbow_max;
    double wrist_neutral;
    double wrist_range,wrist_min,wrist_max;
	double wrist_yaw_neutral;
	double wrist_yaw_range, wrist_yaw_min, wrist_yaw_max;
	double wrist_roll_neutral;
	double wrist_roll_range, wrist_roll_min, wrist_roll_max;
	VECTOR3 shoulder_pos;

	//Hand controller input
	VECTOR3 THCInput, RHCInput;

	MGROUP_TRANSFORM *sat_anim, *sat_ref;

	bool reset_mmu, reset_sat;
	OBJHANDLE hMMU, hSAT;
	double jettison_time;
	bool render_cockpit;
	VCHUDSPEC huds;
	unsigned short usLastMDUID;
	//EXTMFDSPEC mfds[11];
	double mfdbright[11];
	double pl_mass;
	//double dT;
	VECTOR3 GVesselPos, GVesselVel;
	VESSELSTATUS Status;

	//base vectors;
	VECTOR3 LVLH_X, LVLH_Y, LVLH_Z;

	//Launch
	double Throttle_Bucket[2], OMS_Assist[2], RollToHeadsUp; //start/end times
	//targets in SI (angles in degrees)
	double TgtInc, TgtLAN, TgtSpd, TgtAlt, TgtFPA; //targets for guidance
	double THeading, TAp, TPe, TTrA, TEcc, TgtRad;

	double MaxThrust;
	bool bAutopilot, bThrottle;
	bool bMECO, bZThrust;
	double tMECO, tSRBSep; //time(MET)
	double dV; // used for -Z translation
	VECTOR3 EngineNullPosition[3];
	int EngineFail;
	double EngineFailTime;
	bool bEngineFail;
	VECTOR3 Thrust;
	//PEG
	double target_pitch,last_target_pitch,CmdPDot;
	double target_yaw,last_target_yaw,CmdYDot;
	double target_roll,last_target_roll,CmdRDot;
	double TLastMajorCycle;
	//Navigation variables
	VECTOR3 rh0,rv,vv,hv;
	double r,v,h,theta,omega,phi;
	VECTOR3 rh,thetah,hh;
	VECTOR3 posMoon,velMoon,rmh;
	double vr,vtheta,vh;
	//Azimuth
	double Radius, SidDay;

	double fh;
	double pitch,yaw,roll;

	double mu,Re,g;
	double F, m, Isp, a0, tau, ThrAngleP, ThrAngleY;
	//Estimation variables
	double T, p, deltah, deltav, hT, metCutoff;
	//double rT,vrT,T,p,aOrbit,deltah,deltav,hT,metCutoff;
	double ftheta,fdottheta,fdotdottheta,rbar;
	double deltatheta,thetaT;
	double fr,fdotr;
	double d1,d2,d3,d4;
	double A,B,C,TLastAB,fhdotrh;
	double eCurrent;

	//GPC
	int ops, SMOps;
	int last_mfd;
	//bool bFirstStep; //call functions in first timestep
	//Data Input
	KeyboardInput DataInput[3];
	int CRT_SEL[2]; //0=CDR, 1=PLT
	int item;
	//CRT* Display[3];
	CRT* newmfd;

	DiscreteBundleManager* bundleManager;
	
	vc::MDU* mdus[11];
	UINT mfds[11]; //MFD index for corresponding MDUID

	//MNVR
	int OMS; //0=BOTH, 1=LEFT, 2=RIGHT, 3=RCS
	double tig; // TIG in seconds 
	double TIG[4]; // day,hour,min,sec
	double IgnitionTime; //MET at ignition
	bool BurnInProg, BurnCompleted;
	double WT;
	VECTOR3 PEG7, DeltaV;
	double C1, C2, HT, ThetaT; // PEG4 Targets
	VECTOR3 Trim; // 0=P, 1=LY, 2=RY
	int TV_ROLL;
	double DeltaVTot, DeltaVTotms;
	double BurnTime;
	VECTOR3 VGO;
	double vgoTot;
	bool MNVRLOAD;
	VECTOR3 BurnAtt;
	double OMSGimbal[2][2];

	//DAP
	bool ManeuverinProg;
	//ManueverinProg is true if attitude is controlled by autopilot
	enum {MNVR_OFF, MNVR_STARTING, MNVR_IN_PROGRESS, MNVR_COMPLETE} ManeuverStatus;
	//ManeuverStatus is used to set autopilot actions
	AttManeuver CurManeuver, FutManeuver;
	double MNVR_TIME;
	int START_TIME[4]; // day,hour,min,sec
	int TGT_ID, BODY_VECT;
	double P, Y, OM;
	bool MNVR, TRK, ROT;
	bool Pitch, Yaw, Roll;
	VECTOR3 InertialOrientationRad, AngularVelocity;
	VECTOR3 CurrentAttitude;
	VECTOR3 LVLHOrientationReqd/*, LVLHError, LVLHRateVector*/;
	MATRIX3 LVLHTgtOrientationMatrix;
	VECTOR3 RotationAxis;
	double RotationAngle;
	VECTOR3 MNVR_OPTION, TRKROT_OPTION, REQD_ATT;
	MATRIX3 ReqdAttMatrix;
	VECTOR3 PitchYawRoll, TargetAttM50, TargetAttOrbiter, ReqdRates/*, TrackRates*/;
	MATRIX3 PitchYawRollMatrix;
	ORBITPARAM oparam;
	ELEMENTS el;
	double TrkRate;	
	double Mass;
	VECTOR3 PMI, Torque;
	//DAPConfig A, B, Edit;
	DAPConfig DAP[3]; //0=A, 1=B, 2=Edit
	int edit; //0=Blank, 1=A, 2=B
	int DAPMode[2]; //0=A, 1=B && 0=PRI, 1=ALT, 2=VERN
	int RotMode[3]; //0=PITCH/DISC RATE, 1=YAW/PULSE, 2=ROLL
	int TransMode[3]; //0=X/NORM, 1=Y/PULSE, 2=Z
	enum {AUTO, INRTL, LVLH, FREE} ControlMode;
	bool RotPulseInProg[3], TransPulseInProg[3];
	VECTOR3 TransPulseDV; //negative DV for pulses along negative axes
	double RotRate, AttDeadband, RateDeadband, RotPls, TranPls;
	bool NoseThrusters, TailThrusters, Thrusters; //Enabled/Disabled
	int JetsEnabled;

	//I-Loads
	double* stage1guidance[2];
	int stage1guidance_size;
	//double* stage1Vguidance;

	double fTimeCameraLabel;
	NOTEHANDLE nhCameraLabel;
	char pszCameraLabelBuffer[80];

	bool bIlluminated;

	//sound
	int SoundID;
protected:
	void AddKUBandVisual(const VECTOR3 ofs);
	void TriggerLiftOff();
	void DisplayCameraLabel(const char* pszLabel);
};

// ==========================================================
// Interface for derived vessel class: Atlantis_SRB
// ==========================================================

class Atlantis_SRB: public VESSEL2 {
public:
	Atlantis_SRB (OBJHANDLE hObj);
	// Construct interface from existing object

	void SetRefTime (void);

	// Overloaded callback functions
	void clbkSetClassCaps (FILEHANDLE cfg);
	void clbkPostStep (double simt, double simdt, double mjd);
	void clbkPostCreation ();

private:
	MESHHANDLE hSRBMesh_Left;
	MESHHANDLE hSRBMesh_Right;
	double t0;                  // reference time: liftoff
	double srb_separation_time; // simulation time at which SRB separation was initiated
	bool bMainEngine;           // main engine firing?
	bool bSeparationEngine;     // separation thrusters firing?
	double slag1, slag2, slag3;
	MESHHANDLE hSRBMesh;
	PROPELLANT_HANDLE ph_main;  // handle for propellant resource
	PROPELLANT_HANDLE phBSM;	//separation motor fuel resource
	THRUSTER_HANDLE th_main;    // engine handle
	THRUSTER_HANDLE th_bolt;    // separation bolt
	THRUSTER_HANDLE thBSM[3];	//represent engines by a single logical thruster for each group.
};

// ==========================================================
// Interface for derived vessel class: Atlantis_Tank
// ==========================================================

class Atlantis_Tank: public VESSEL2 {
public:
	Atlantis_Tank (OBJHANDLE hObj);
	// Construct interface from existing object

	// Overloaded callback functions
	void clbkSetClassCaps (FILEHANDLE cfg);
	void clbkPostStep (double simt, double simdt, double mjd);

private:
	MESHHANDLE hTankMesh;

	//////////////////////// ET vent ////////////////////////
	PROPELLANT_HANDLE phLOXtank;
	THRUSTER_HANDLE thLOXvent;
	double timer;
	short counter;
	double pos;
	//////////////////////// ET vent ////////////////////////
};

//mesh illumination functions
static void IlluminateMesh(MESHHANDLE mesh)
{
	MATERIAL* material=NULL;

	DWORD materialCount = oapiMeshMaterialCount(mesh);
    for (DWORD mi = 0; mi < materialCount; mi++) {
        material = oapiMeshMaterial(mesh, mi);
        if (material->emissive.g <= 0.1) {
            material->emissive.r = 0.3f;
            material->emissive.g = 0.3f;
            material->emissive.b = 0.3f;
        }
    }
}

static void IlluminateMesh(MESHHANDLE mesh, vector<int> vExclude)
{
	MATERIAL* material=NULL;
	unsigned int ExCounter=0;

	int materialCount = oapiMeshMaterialCount(mesh);
    for (int mi = 0; mi < materialCount; mi++) {
		if(vExclude[ExCounter]!=mi) {
			material = oapiMeshMaterial(mesh, mi);
			if (material->emissive.g <= 0.1) {
				material->emissive.r = 0.3f;
				material->emissive.g = 0.3f;
				material->emissive.b = 0.3f;
			}
		}
		else if(ExCounter<(vExclude.size()-1)) ExCounter++;
    }
}

static void DisableIllumination(MESHHANDLE mesh, MESHHANDLE GlobalMesh)
{
	MATERIAL* MeshMaterial=NULL;
	MATERIAL* DefaultMaterial=NULL;

	DWORD materialCount = oapiMeshMaterialCount(mesh);
    for (DWORD mi = 0; mi < materialCount; mi++) {
        MeshMaterial = oapiMeshMaterial(mesh, mi);
		DefaultMaterial = oapiMeshMaterial(GlobalMesh, mi);
        
		MeshMaterial->emissive.r=DefaultMaterial->emissive.r;
		MeshMaterial->emissive.g=DefaultMaterial->emissive.g;
		MeshMaterial->emissive.b=DefaultMaterial->emissive.b;
    }
}

#endif // !__ATLANTIS_H

