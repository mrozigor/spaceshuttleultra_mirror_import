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
//Beginning of Mid deck positions
const int VC_MIDDECK = 100;
//Beginning of external airlock positions
const int VC_EXT_AL = 200;

const static char* VC_LBL_CDR = "Commander seat";
const VECTOR3 VC_POS_CDR = _V(-0.671257, 2.629396, 14.1);
//const VECTOR3 VC_HUDPOS_CDR = _V(-0.653, 2.589, 14.614);
const VECTOR3 VC_HUDPOS_CDR = _V(-0.6515, 2.568, 14.614);
const static char* VC_LBL_PLT = "Pilot seat";
const VECTOR3 VC_POS_PLT = _V(0.671257, 2.629396, 14.1);
//const VECTOR3 VC_HUDPOS_PLT = _V(0.652, 2.589, 14.614);
const VECTOR3 VC_HUDPOS_PLT = _V(0.6515, 2.568, 14.614);

const static char* VC_LBL_MS1 = "MS2/FE seat";
const VECTOR3 VC_POS_MS1 = _V(0, 2.629396, 13.25);
const VECTOR3 VC_DIR_MS1 = _V(0.0, 0, 1.0);

const static char* VC_LBL_MS2 = "MS seat";
const VECTOR3 VC_POS_MS2 = _V(0.7, 2.75, 13.25);
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
const VECTOR3 VC_POS_EXT_AL = ODS_POS + _V(0.0, 0.1, 0.25);
const VECTOR3 VC_DIR_EXT_AL = _V(0.0, -sin(24.5*RAD), cos(24.5 * RAD));

#endif //__ATLANTIS_VC_DEFS_H
