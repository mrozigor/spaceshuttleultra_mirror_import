/****************************************************************************
  This file is part of Space Shuttle Ultra

  Constants used for defining VC views/positions



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
#ifndef __ATLANTIS_VC_DEFS_H
#define __ATLANTIS_VC_DEFS_H
#pragma once

#include "orbitersdk.h"


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
const int VC_PLBCAMFL = 13; // A
const int VC_PLBCAMBL = 14; // B
const int VC_PLBCAMBR = 15; // C
const int VC_PLBCAMFR = 16; // D
const int VC_RMSCAM = 17;
const int VC_LEECAM = 18;
//New panel positions for pilot and commander
const int VC_PANELR4 = 20;
const int VC_PANELL4 = 21;
//Beginning of Mid deck positions
const int VC_MIDDECK = 100;
//Beginning of external airlock positions
const int VC_EXT_AL = 200;

const static char* VC_LBL_CDR = "Commander seat";
const VECTOR3 VC_POS_CDR = _V( -0.671257, 2.629396, 14.1 );
const VECTOR3 VC_DIR_CDR = _V( 0, 0, 1 );
const VECTOR3 VC_OFS_CDR_F = _V( 0, 0, 0.3 );// HUD
const double VC_AZ_CDR_F = 0;
const double VC_EL_CDR_F = 0;
const VECTOR3 VC_OFS_CDR_L = _V( -0.3, 0, 0 );// center panels
const double VC_AZ_CDR_L = 75 * RAD;
const double VC_EL_CDR_L = -5 * RAD;
const VECTOR3 VC_OFS_CDR_R = _V( 0.3, 0, 0 );// window 1
const double VC_AZ_CDR_R = -20 * RAD;
const double VC_EL_CDR_R = -27 * RAD;
const VECTOR3 VC_HUDPOS_CDR = _V( -0.6527025, 2.59925, 14.614 );

const static char* VC_LBL_PLT = "Pilot seat";
const VECTOR3 VC_POS_PLT = _V( 0.671257, 2.629396, 14.1 );
const VECTOR3 VC_DIR_PLT = _V( 0, 0, 1 );
const VECTOR3 VC_OFS_PLT_F = _V( 0, 0, 0.3 );// HUD
const double VC_AZ_PLT_F = 0;
const double VC_EL_PLT_F = 0;
const VECTOR3 VC_OFS_PLT_L = _V( -0.3, 0, 0 );// center panels
const double VC_AZ_PLT_L = 20 * RAD;
const double VC_EL_PLT_L = -27 * RAD;
const VECTOR3 VC_OFS_PLT_R = _V( 0.2, -0.1, 0.25 );// panel R2
const double VC_AZ_PLT_R = -90 * RAD;
const double VC_EL_PLT_R = -72 * RAD;
const VECTOR3 VC_HUDPOS_PLT = _V(0.6528925, 2.59925, 14.614);

const static char* VC_LBL_MS2 = "MS2/FE seat";
const VECTOR3 VC_POS_MS2 = _V( 0, 2.629396, 13.25 );
const VECTOR3 VC_DIR_MS2 = _V( 0.0, 0, 1.0 );
const VECTOR3 VC_OFS_MS2_F = _V( 0, 0, 0.3 );
const double VC_AZ_MS2_F = 0;
const double VC_EL_MS2_F = 0;
const VECTOR3 VC_OFS_MS2_L = _V( -0.3, 0, 0.1 );// panels O14/O15
const double VC_AZ_MS2_L = 0;
const double VC_EL_MS2_L = 70 * RAD;
const VECTOR3 VC_OFS_MS2_R = _V( 0.3, 0, 0.1 );// panels O15/O16
const double VC_AZ_MS2_R = 0;
const double VC_EL_MS2_R = 70 * RAD;

const static char* VC_LBL_MS1 = "MS1 seat";
const VECTOR3 VC_POS_MS1 = _V( 0.7, 2.75, 13.25 );
const VECTOR3 VC_DIR_MS1 = _V( 0.0, 0.0, 1.0 );
const VECTOR3 VC_OFS_MS1_F = _V( 0.1, -0.1, 0.2 );
const double VC_AZ_MS1_F = -20 * RAD;
const double VC_EL_MS1_F = 0;
const VECTOR3 VC_OFS_MS1_L = _V( -0.1, -0.1, 0.1 );
const double VC_AZ_MS1_L = 45 * RAD;
const double VC_EL_MS1_L = -5 * RAD;
const VECTOR3 VC_OFS_MS1_R = _V( 0.1, 0, 0.1 );
const double VC_AZ_MS1_R = -70 * RAD;
const double VC_EL_MS1_R = -40 * RAD;

//const static char* VC_LBL_COAS = "COAS";
const VECTOR3 VC_POS_COAS = _V( 0.4, 3.15, 12.6);
//const VECTOR3 VC_DIR_COAS = _V( 0.0, 0.45399, -0.891007);

const static char* VC_LBL_PORTSTATION = "Port Work Station";
const VECTOR3 VC_POS_PORTSTATION = _V( -0.6, 2.95, 13.0 );
const VECTOR3 VC_DIR_PORTSTATION = _V( -cos( 25.0 * RAD ), -sin( 25.0 * RAD ), 0 );
const VECTOR3 VC_OFS_PORTSTATION_F = _V( -0.3, 0, 0 );// panel L11
const double VC_AZ_PORTSTATION_F = 0;
const double VC_EL_PORTSTATION_F = -30 * RAD;
const VECTOR3 VC_OFS_PORTSTATION_L = _V( -0.45, -0.2, -0.35 );// panel L12
const double VC_AZ_PORTSTATION_L = 20 * RAD;
const double VC_EL_PORTSTATION_L = -25 * RAD;
const VECTOR3 VC_OFS_PORTSTATION_R = _V( -0.3, 0, 0.3 );// panel L10
const double VC_AZ_PORTSTATION_R = 0;
const double VC_EL_PORTSTATION_R = -30 * RAD;

const static char* VC_LBL_AFTPILOT = "Aft Pilot Station";
const VECTOR3 VC_POS_AFTPILOT = _V( 0.4, 3.15, 12.7 );
const VECTOR3 VC_DIR_AFTPILOT = _V( 0, 0, -1 );
const VECTOR3 VC_OFS_AFTPILOT_F = VC_POS_COAS - VC_POS_AFTPILOT;
const double VC_AZ_AFTPILOT_F = 0;
const double VC_EL_AFTPILOT_F = 90.0 * RAD;
const VECTOR3 VC_OFS_AFTPILOT_L = _V( 0.5, -0.5, 0.0 );// panel A1
const double VC_AZ_AFTPILOT_L = 20.0 * RAD;
const double VC_EL_AFTPILOT_L = -10.0 * RAD;
const VECTOR3 VC_OFS_AFTPILOT_R = _V( -0.4, 0.0, 0.0 );// panel A2
const double VC_AZ_AFTPILOT_R = 0;
const double VC_EL_AFTPILOT_R = 0;

const static char* VC_LBL_RMSSTATION = "RMS Work Station";
const VECTOR3 VC_POS_RMSSTATION = _V( -0.4, 3.15, 12.7 );
const VECTOR3 VC_DIR_RMSSTATION = _V( 0.0, 0.0, -1.0 );
const VECTOR3 VC_OFS_RMSSTATION_F = VC_POS_COAS - VC_POS_AFTPILOT;
const double VC_AZ_RMSSTATION_F = 0;
const double VC_EL_RMSSTATION_F = 90.0 * RAD;
const VECTOR3 VC_OFS_RMSSTATION_L = _V( -0.07, -0.05, -0.35 );// window 10
const double VC_AZ_RMSSTATION_L = 0;
const double VC_EL_RMSSTATION_L = 0;
const VECTOR3 VC_OFS_RMSSTATION_R = _V( 0, -0.3, 0.15 );// panel A8
const double VC_AZ_RMSSTATION_R = 0;
const double VC_EL_RMSSTATION_R = 0;

const static char* VC_LBL_STBDSTATION = "Starboard Work Station";
const VECTOR3 VC_POS_STBDSTATION = _V( 0.6, 2.95, 13.0 );
const VECTOR3 VC_DIR_STBDSTATION = _V( cos( 25.0 * RAD ), -sin( 25.0 * RAD ), 0 );
const VECTOR3 VC_OFS_STBDSTATION_F = _V( 0.3, 0, 0 );
const double VC_AZ_STBDSTATION_F = 0;
const double VC_EL_STBDSTATION_F = -30.0 * RAD;
const VECTOR3 VC_OFS_STBDSTATION_L = _V( 0.3, 0, 0.2 );// panel L11
const double VC_AZ_STBDSTATION_L = 15.0 * RAD;
const double VC_EL_STBDSTATION_L = -30.0 * RAD;
const VECTOR3 VC_OFS_STBDSTATION_R = _V( 0.3, 0, -0.2 );// panel L13
const double VC_AZ_STBDSTATION_R = -25.0 * RAD;
const double VC_EL_STBDSTATION_R = -30.0 * RAD;

const static char* VC_LBL_AFTWORKSTATION = "Aft Work Station";
const VECTOR3 VC_POS_AFTWORKSTATION = _V( 0.0, 2.95, 13.2 );
const VECTOR3 VC_DIR_AFTWORKSTATION = _V( 0.0, 0.0, -1.0 );

const static char* VC_LBL_DOCKCAM = "ODS centerline camera";

const static char* VC_LBL_LEECAM = "RMS EE camera";

const static char* VC_LBL_ELBOWCAM = "RMS Elbow camera";

const static char* VC_LBL_PLBCAMFL = "Payload bay A camera";
const VECTOR3 CAM_A_POS = _V(-1.7765, 0.965, 9.268);

const static char* VC_LBL_PLBCAMFR = "Payload bay D camera";
const VECTOR3 CAM_D_POS = _V(1.7765, 0.965, 9.268);

const static char* VC_LBL_PLBCAMBL = "Payload bay B camera";
const VECTOR3 CAM_B_POS = _V(-2.2495, 0.9655, -8.21);

const static char* VC_LBL_PLBCAMBR = "Payload bay C camera";
const VECTOR3 CAM_C_POS = _V(2.2495, 0.9655, -8.21);

const static char* VC_LBL_MIDDECK = "Mid Deck";
const VECTOR3 VC_POS_MIDDECK = _V(-1.44, 0.0, 13.353);
const VECTOR3 VC_DIR_MIDDECK = _V(0.0, -sin(24.5*RAD), cos(24.5 * RAD));

const static char* VC_LBL_EXT_AL = "External Airlock";
const VECTOR3 VC_POS_EXT_AL = _V(0.0, 0.1, 2.0);
const VECTOR3 VC_DIR_EXT_AL = _V(0.0, -sin(24.5*RAD), cos(24.5 * RAD));

const static char* VC_LBL_PANELR4 = "Pilot - R4";
const VECTOR3 VC_POS_PANELR4 = _V( 1.271257, 2.229396, 14.2 );
const VECTOR3 VC_DIR_PANELR4 = _V( 0, -sin( 15 * RAD ), -cos( 15 * RAD ) );
const double VC_TILT_PANELR4 = 90.0 * RAD;
const VECTOR3 VC_OFS_PANELR4_F = _V( 0, 0, 0.3 );
const double VC_AZ_PANELR4_F = 0;
const double VC_EL_PANELR4_F = 0;
const VECTOR3 VC_OFS_PANELR4_L = _V( -0.3, 0, 0 );
const double VC_AZ_PANELR4_L = 20 * RAD;
const double VC_EL_PANELR4_L = -27 * RAD;
const VECTOR3 VC_OFS_PANELR4_R = _V( 0.2, -0.1, 0.25 );
const double VC_AZ_PANELR4_R = -90 * RAD;
const double VC_EL_PANELR4_R = -72 * RAD;

const static char* VC_LBL_PANELL4 = "CDR - L4";
const VECTOR3 VC_POS_PANELL4 = _V( -1.271257, 2.229396, 14.2 );
const VECTOR3 VC_DIR_PANELL4 = _V( 0, 0, -1 );
const double VC_TILT_PANELL4 = -90.0 * RAD;
const VECTOR3 VC_OFS_PANELL4_F = _V( 0, 0, 0.3 );
const double VC_AZ_PANELL4_F = 0;
const double VC_EL_PANELL4_F = 0;
const VECTOR3 VC_OFS_PANELL4_L = _V( -0.3, 0, 0 );
const double VC_AZ_PANELL4_L = 20 * RAD;
const double VC_EL_PANELL4_L = -27 * RAD;
const VECTOR3 VC_OFS_PANELL4_R = _V( 0.2, -0.1, 0.25 );
const double VC_AZ_PANELL4_R = -90 * RAD;
const double VC_EL_PANELL4_R = -72 * RAD;


#endif //__ATLANTIS_VC_DEFS_H
