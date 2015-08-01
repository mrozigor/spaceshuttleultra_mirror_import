#ifndef _PARAMETERVALUES_H_795497C2_2F91_464b_9F03_19F48E9366EE
#define _PARAMETERVALUES_H_795497C2_2F91_464b_9F03_19F48E9366EE
#pragma once
#include "OrbiterAPI.h"
#include "EngConst.h"

const double ORBITER_EMPTY_MASS_OV102 = 80739.44;
const double ORBITER_EMPTY_MASS_OV099 = 79429.01;
const double ORBITER_EMPTY_MASS_OV103 = 77564.3;
const double ORBITER_EMPTY_MASS_OV104 = 77564.3;
const double ORBITER_EMPTY_MASS_OV105 = 78017.89;
// Orbiter empty masses [kg]

const double ORBITER_MAX_PROPELLANT_MASS = 11284.23 + 2162.622;
// Per SODB Vol 1, 3.4.3.3, maximum propellant load in the OMS tanks is 4711.5 lbs (2134.3095 kg) of fuel and 7743.5 lbs (3507.8055 kg) of oxidizer per pod. Min load is 2038 (923.214 kg) lbs of fuel and 3362 lbs (1522.986 kg) of oxidizer per pod.
// This parameter for now includes the propellant mass of the Aft RCS fuel and oxidizer

const double ORBITER_FRCS_PROPELLANT_MASS = 1464 * LBM + 923*LBM;

const double ORBITER_LEFT_ARCS_PROPELLANT_MASS = 1464 * LBM + 923 * LBM;

const double ORBITER_RIGHT_ARCS_PROPELLANT_MASS = 1464 * LBM + 923 * LBM;

// Amount of fuel in forward RCS tanks

//const double SSME_RATED_THRUST = 2090664.159; //100% thrust
const double ORBITER_MAIN_THRUST = 2170732.15; //Nominal "104%" thrust
//const double ORBITER_MAX_THRUST = 2275094.273; //109% thrust
// Vacuum thrust rating per main engine [N]
const double SSME_MAX_POWER_LEVEL = 109.0;

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

const double ORBITER_WING_AREA = 2690*0.3048*0.3048; // [m^2]
const double ORBITER_SPAN = 78.056*0.3048; //Turns out span is not used...
const double ORBITER_CHORD_LENGTH=39.56*0.3048; //...but chord is
const double ORBITER_WING_ASPECT_RATIO = ORBITER_SPAN*ORBITER_SPAN/ORBITER_WING_AREA; //Has to be passed, doesn't have to be correct

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
const double CHUTE_FAIL_SPEED = 230.0/MPS2KTS;
// Lowest speed at which chute breaks off if deployed (m/s)

const double GEAR_OPERATING_SPEED = 0.3;
// Opening/closing speed of landing gear (1/sec)
// => gear cycle ~ 3 sec

const double SPEEDBRAKE_OPERATING_SPEED = 0.20284;
// Deployment speed of the speedbrake (1/sec)
// cycle is 4.93 sec

//const double AIR_DATA_PROBE_SPEED = 0.06666667;

//const double DYNP_MAX = 100e3;
// Max. allowed dynamic pressure [Pa]
//const double GEAR_MAX_DEPLOY_SPEED = 450/1.943844;
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

const double KU_OPERATING_SPEED = 0.0275;
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

#endif //_PARAMETERVALUES_H_795497C2_2F91_464b_9F03_19F48E9366EE
