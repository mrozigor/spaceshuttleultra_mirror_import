#ifndef _PARAMETERVALUES_H_795497C2_2F91_464b_9F03_19F48E9366EE
#define _PARAMETERVALUES_H_795497C2_2F91_464b_9F03_19F48E9366EE
#pragma once

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

const double OMS_PITCH_RANGE = 6.0;
const double OMS_YAW_RANGE = 7.0;
// max gimbal angles in degrees

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

const double ORBITER_PITCH_TORQUE = 239900.00;
const double ORBITER_YAW_TORQUE = 239900.00;
const double ORBITER_ROLL_TORQUE = 41800.00;
// Vacuum RCS Torque [N]

const double OMS_HELIUM_TANK_MASS = 21.0;
//Helium tank mass [kg]

const double CHUTE_DEPLOY_TIME = 2.5;
// Time from chute deploy command to deployment of chute (s)
const double CHUTE_INFLATE_TIME = 6.3;
// Time from chute deploy command to full inflation of chute (s)
const double CHUTE_DEPLOY_RATE = 0.25;
// Rate at which chute deploys to reefed state (1/s)
const double CHUTE_INFLATE_RATE = 1.0;
// Rate at which chute inflates (1/s)
const double CHUTE_SPIN_RATE = 0.5;
// Rate at which chute spins (1/s)
const double CHUTE_DEPLOY_SPEED = 165.0/MPS2KTS;
// Speed at which chute is deployed (m/s)
const double CHUTE_JETTISON_SPEED = 60.0/MPS2KTS;
// Speed at which chute is jettisoned (m/s)

const double GEAR_OPERATING_SPEED = 0.3;
// Opening/closing speed of landing gear (1/sec)
// => gear cycle ~ 3 sec

const double SPEEDBRAKE_OPERATING_SPEED = 0.20284;
// Deployment speed of the speedbrake (1/sec)
// cycle is 4.93 sec

const double AIR_DATA_PROBE_SPEED = 0.06666667;

const double DYNP_MAX = 100e3;
// Max. allowed dynamic pressure [Pa]
const double GEAR_MAX_DEPLOY_SPEED = 450/1.943844;
// Max. safe gear deploy speed [m/s]

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

const double KU_OPERATING_SPEED = 0.055;
// Deployment speed of the Ku Band antenna (1/sec)
// Specified cycle is 23 sec, although actual observed cycle is ~19 sec

const VECTOR3 ORBITER_CS = {304.62,457.57,149.27};
//DaveS: Fixed the CS values as they were off bit great deal.
// Orbiter cross sections (projections into principal axes) [m^2]

const VECTOR3 ORBITER_CS_GEAR = {10.0,0.0,3.0};
// Contribution of fully extended landing gear to cross sections

const double ORBITER_CW[4] = {0.13, 0.5, 1.4, 1.4};
// Orbiter wind resistance coefficients in principal directions (+z, -z, +-x, +-y)

const double ORBITER_CW_GEAR[4] = {0.04, 0.04, 0.05, 0.0};
// Contribution of fully extended landing gear to wind resistance

const double PTU_HIGHRATE_SPEED = 12;
const double PTU_LOWRATE_SPEED = 1.2;
// Payload camera speed rates (deg/sec)
const double MAX_PLBD_CAM_TILT = 170.0;
const double MAX_PLBD_CAM_PAN = 170.0;

//Post Contact Thrusting
const double PCT_STAGE1 = 0.56;
//period (s) for initial jet firing
const double PCT_STAGE2 = 1.52;
//period (s) between jet firings
const double PCT_STAGE3 = 2.40;
//period (s) for second jet firing

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

// ==========================================================
// Some Tank-related parameters
// ==========================================================

const double ET_SEP_TIME = 18.00000; // ET Sep time (after MECO)
const double ET_SEP_RATE = 1.2192; // Commanded ET sep rate (m/s)

#endif //_PARAMETERVALUES_H_795497C2_2F91_464b_9F03_19F48E9366EE