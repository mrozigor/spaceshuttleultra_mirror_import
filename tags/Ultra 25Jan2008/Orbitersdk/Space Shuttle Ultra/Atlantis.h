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

//#include "CRT.h"
#include "orbitersdk.h"
#include <math.h>

// ==========================================================
// Some Orbiter-related parameters
// ==========================================================

const double ORBITER_EMPTY_MASS = 81100.0;
// Orbiter empty mass [kg]

const double ORBITER_MAX_PROPELLANT_MASS = 14538.0;
// Amount of fuel the orbiter can hold in internal OMS tanks

const double ORBITER_FRCS_PROPELLANT_MASS = 1082.0;
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

const double GEAR_OPERATING_SPEED = 0.3;
// Opening/closing speed of landing gear (1/sec)
// => gear cycle ~ 3 sec

const double DOOR_OPERATING_SPEED = 0.007353;
// Opening/closing speed of payload bay doors (1/sec)
// This contains the door opening sequence (63 sec for each door) and an
// interval of 10 sec between the two door operations

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
const VECTOR3 ROLLOUT_AXIS = {-2.589, 0.955, 0.0};

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

const double SRB_MAX_PROPELLANT_MASS = 502126.0;
// SRB propellant mass [kg]

const double SRB_EMPTY_MASS = 87543.0;
// SRB empty mass [kg]

const double SRB_ISP0 = 3574.68;
const double SRB_ISP1 = 2859.74;
// SRB vacuum and sea-level fuel-specific impulse [m/s]

const double SRB_THRUST = 17064000; //from Jenkins
// Vacuum SRB thrust per unit [N]

const double MAX_ATT_LAUNCH = 1e-5;
const double MAX_ROLL_SRB = 2.5e-5;
// Max attitude thrust during launch phase (temporary)

const double SRB_STABILISATION_TIME = 6;
// MET: -SRB ignition

const double SRB_SEPARATION_TIME = 126.6;
// MET: SRB separation
const int SRB_nt = 30;

const double SRB_CUTOUT_TIME = 140.0;
// MET: engine shutdown

// ==========================================================
// Unit conversion
// ==========================================================
const double fps_to_ms=0.3048;
const double kg_to_pounds=2.205;

// ==========================================================
// Mesh offsets for various configurations
// ==========================================================

const VECTOR3 OFS_ZERO             = { 0.0, 0.0,  0.0  };
const VECTOR3 OFS_LAUNCH_ORBITER   = { 0.0, 6.22,-7.795};
const VECTOR3 OFS_LAUNCH_TANK      = { 0.0,-1.91, 5.72 };
//const VECTOR3 OFS_LAUNCH_RIGHTSRB  = { 6.2,-1.91,-5.68 };
//const VECTOR3 OFS_LAUNCH_LEFTSRB   = {-6.2,-1.91,-5.68 };
const VECTOR3 OFS_LAUNCH_RIGHTSRB  = { 6.88,-1.91,-5.68 };
const VECTOR3 OFS_LAUNCH_LEFTSRB   = {-6.88,-1.91,-5.68 };
const VECTOR3 OFS_WITHTANK_ORBITER = { 0.0, 4.79,-9.185};
const VECTOR3 OFS_WITHTANK_TANK    = { 0.0,-3.34, 4.33 };
const VECTOR3 ORBITER_DOCKPOS      = { 0.0, 2.44,10.44 };
const VECTOR3 OFS_MMU              = {0,2.44,10.44};

// ==========================================================
// Mesh group indices for some components
// ==========================================================

const UINT MESH_UNDEFINED = (UINT)-1;

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
#define AID_MFD1_BUTTONS   5
#define AID_MFD2_BUTTONS   6
#define AID_MFD3_BUTTONS   7
#define AID_MFD4_BUTTONS   8
#define AID_MFD5_BUTTONS   9
#define AID_MFDA_BUTTONS  10
// D. Beachy: define power buttons
#define AID_CDR1_PWR      11
#define AID_CDR2_PWR      12
#define AID_PLT1_PWR      13
#define AID_PLT2_PWR      14
#define AID_MFD1_PWR      15
#define AID_MFD2_PWR      16
#define AID_MFD3_PWR      17
#define AID_MFD4_PWR      18
#define AID_MFD5_PWR      19
#define AID_MFDA_PWR      20
// MFD brightness buttons
#define AID_CDR1_BRT      21
#define AID_CDR2_BRT      22
#define AID_PLT1_BRT      23
#define AID_PLT2_BRT      24
#define AID_MFD1_BRT      25
#define AID_MFD2_BRT      26
#define AID_MFD3_BRT      27
#define AID_MFD4_BRT      28
#define AID_MFD5_BRT      29
#define AID_MFDA_BRT      30
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
// Panel C3
#define AID_C3_MIN     300
#define AID_C3         300
#define AID_C3_MAX     320
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
#define AID_R2_MAX     420

typedef struct {
	HINSTANCE hDLL;
	SURFHANDLE tkbk_label;
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

enum AXIS {PITCH, YAW, ROLL};

// ==========================================================
// Interface for derived vessel class: Atlantis
// ==========================================================

class Atlantis: public VESSEL2 {
	friend class PayloadBayOp;
	friend class GearOp;
	friend class PanelC3;
	friend class PanelR2;
	friend class CRT;
	friend BOOL CALLBACK RMS_DlgProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	friend BOOL CALLBACK PAYCAM_DlgProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	AnimState::Action spdb_status;
	Atlantis (OBJHANDLE hObj, int fmodel);
	~Atlantis();
	void SetLaunchConfiguration (void);
	void SetPostLaunchConfiguration (double srbtime);
	void SetOrbiterTankConfiguration (void);
	void SetOrbiterConfiguration (void);
	void DefineAnimations (void);
	void AddOrbiterVisual (const VECTOR3 &ofs);
	void AddTankVisual (const VECTOR3 &ofs);
	void AddSRBVisual (int which, const VECTOR3 &ofs);
	void SeparateBoosters (double srb_time);
	void SeparateTank (void);
	void SeparateMMU (void);
	void ToggleGrapple (void);
	void ToggleArrest (void);
	void Jettison ();
	void UpdateMesh ();
	void ClearMeshes ();
	void SetBayDoorPosition (double pos);
	void SetRadiatorPosition (double pos);
	void SetRadLatchPosition (double pos) {}
	void SetKuAntennaPosition (double pos);
	void SetETUmbDoorPosition(double pos, int door);
	void OperateSpeedbrake (AnimState::Action action);
	void RevertSpeedbrake ();
	void SetSpeedbrake (double tgt);
	void SetAnimationArm (UINT anim, double state);
	void SetAnimationIKArm(VECTOR3 arm_wrist_dpos);
	VECTOR3 CalcAnimationFKArm();

	// Actual Virtual Cockpit Mode
	int VCMode;

	void RegisterVC_CdrMFD ();
	void RegisterVC_PltMFD ();
	void RegisterVC_CntMFD ();
	void RegisterVC_AftMFD ();
	void RedrawPanel_MFDButton (SURFHANDLE surf, int mfd);

	int status; // 0=launch configuration
	            // 1=SRB's engaged
	            // 2=SRB's separated
	            // 3=Tank separated (orbiter only)
	bool bManualSeparate; // flag for user-induced booster or tank separation

	double t0;          // reference time: designated liftoff time
	double met;
	int MET[4], Launch_time[4], MET_Add[4]; // day,hour,min,sec
	WORD srb_id1, srb_id2;

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
	MESHHANDLE hOrbiterMesh, hOrbiterCockpitMesh, hOrbiterVCMesh, hOrbiterRMSMesh, hTankMesh, hSRBMesh[2]; // mesh handles
	char cargo_static_mesh_name[256];
	ATTACHMENTHANDLE sat_attach, rms_attach;
	VECTOR3 arm_tip[3];

	// Overloaded callback functions
	void clbkSetClassCaps (FILEHANDLE cfg);
	void clbkSetStateEx (const void *status);
	void clbkLoadStateEx (FILEHANDLE scn, void *vs);
	void clbkSaveState (FILEHANDLE scn);
	void clbkFocusChanged (bool getfocus, OBJHANDLE hNewVessel, OBJHANDLE hOldVessel);
	void clbkPreStep (double simT, double simDT, double mjd);
	void clbkPostStep (double simt, double simdt, double mjd);
	bool clbkPlaybackEvent (double simt, double event_t, const char *event_type, const char *event);
	void clbkDrawHUD (int mode, const HUDPAINTSPEC *hps, HDC hDC);
	int  clbkConsumeBufferedKey (DWORD key, bool down, char *kstate);
	void clbkVisualCreated (VISHANDLE vis, int refcount);
	void clbkVisualDestroyed (VISHANDLE vis, int refcount);
	void clbkAnimate (double simt);
	void clbkMFDMode (int mfd, int mode);
	bool clbkLoadGenericCockpit ();
	bool clbkLoadVC (int id);
	bool clbkVCMouseEvent (int id, int event, VECTOR3 &p);
	bool clbkVCRedrawEvent (int id, int event, SURFHANDLE surf);

	PayloadBayOp *plop; // control and status of payload bay operations
	GearOp *gop; // control and status of landing gear
	PanelC3 *c3po; // PanelC3 operations
	PanelR2 *r2d2; // PanelR2 operations
	bool PitchActive,YawActive,RollActive;     // Are RCS channels active?

	OBJHANDLE ThisVessel;

private:
	void LaunchClamps();
	void CreateAttControls_RCS(VECTOR3 center);
	void DisableAllRCS();
	void EnableAllRCS();
	void DisableControlSurfaces();
	void EnableControlSurfaces();
	bool ArmCradled();
	bool SatGrappled() const { return GetAttachmentStatus (rms_attach) != 0; }
	bool SatStowed() const { return GetAttachmentStatus (sat_attach) != 0; }
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
	//PEG
	void MajorCycle();
	void Navigate();
	void Estimate();
	void Guide();

	//GPC
	void GPC(double dt);
	void Maneuver(double dt);
	void SetILoads();

	//DAP
	void AttControl(double SimdT);
	void SetRates(VECTOR3 &Rates);
	//VECTOR3 ConvertAxes(VECTOR3 Rates);
	void CalcRequiredRates(VECTOR3 &Rates);
	void CalcLVLHAttitude(VECTOR3 &Output);
	VECTOR3 CalcRelLVLHAttitude(VECTOR3 &Target);
	//bool CheckLimits(double dNum1, double dNum2, double dLim);
	double NullStartAngle(double Rates, AXIS Axis);

	//Math
	VECTOR3 CalcPitchYawRollAngles(VECTOR3 &RelAttitude);
	VECTOR3 GetPYR(VECTOR3 Pitch, VECTOR3 YawRoll);
	VECTOR3 GetPYR2(VECTOR3 Pitch, VECTOR3 YawRoll);
	void RotateVector(const VECTOR3 &Initial, const VECTOR3 &Angles, VECTOR3 &Result);
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
	inline VECTOR3 RotateVectorZ(const VECTOR3 &v, double angle) //rotates about angle in Z-axis
	{
		VECTOR3 Output;
		Output.x=v.x*cos(angle*RAD)-v.y*sin(angle*RAD);
		Output.y=v.x*sin(angle*RAD)+v.y*cos(angle*RAD);
		Output.z=v.z;
		return Output;
	}
	inline bool Eq(const double d1, const double d2)
	{
		if(abs(d1-d2)>0.00001) return false;
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

	UINT anim_door;                            // handle for cargo door animation
	UINT anim_rad;                             // handle for radiator animation
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
	UINT mesh_orbiter;                         // index for orbiter mesh
	UINT mesh_cockpit;                         // index for cockpit mesh for external view
	UINT mesh_vc;                              // index for virtual cockpit mesh
	UINT mesh_rms;							   // index for RMS mesh
	UINT mesh_tank;                            // index for external tank mesh
	UINT mesh_srb[2];                          // index for SRB meshes


	// PAYLOAD CAMERAS ANIMATIONS
	UINT anim_camFLyaw;						   // handle for front-left payload camera yaw animation 
	UINT anim_camFLpitch;					   // handle for front-left payload camera pitch animation 
	UINT anim_camFRyaw;						   // handle for front-right payload camera yaw animation 
	UINT anim_camFRpitch;					   // handle for front-right payload camera pitch animation 
	UINT anim_camBLyaw;						   // handle for back-left payload camera yaw animation 
	UINT anim_camBLpitch;					   // handle for back-left payload camera pitch animation 
	UINT anim_camBRyaw;						   // handle for back-right payload camera yaw animation 
	UINT anim_camBRpitch;					   // handle for back-right payload camera pitch animation 
	
	// PAYLOAD CAMERAS ROTATION (-170 to 170 degrees)
	double camFLyaw;
	double camFLpitch;
	double camFRyaw;
	double camFRpitch;
	double camBLyaw;
	double camBLpitch;
	double camBRyaw;
	double camBRpitch;
	
	// Selected camera must be moved at low rate (if false at high rate)
	bool cameraLowRate;

	// If a camera have been moved (used for reposition camera animations)
	bool cameraMoved;
	
	// Selected camera for control
	int cameraControl;  // 0:FL 1:FR 2:BL 3:BR
	
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
	
	PROPELLANT_HANDLE ph_oms, ph_tank, ph_srb, ph_frcs; // handles for propellant resources
	THRUSTER_HANDLE th_main[3];                // handles for orbiter main (SSME) engines
	THRUSTER_HANDLE th_oms[2];               // handles for orbiter OMS engines
	THRUSTER_HANDLE th_srb[2];                 // handles for SRB engines
    THRUSTER_HANDLE th_att_rcs[18];            // 12 for rotation, 6 virtual
	THRUSTER_HANDLE th_att_lin[10];
	UINT ex_main[3];						   // main engine exhaust
	UINT ex_retro[2];						   // OMS exhaust
	bool RCSEnabled;
	THGROUP_HANDLE thg_main, thg_srb, thg_retro;          // handles for thruster groups
	CTRLSURFHANDLE hrudder, hlaileron, hraileron, helevator, hbodyflap;
	bool ControlSurfacesEnabled;

	// APU Fuel Tanks
	PROPELLANT_HANDLE apu_tank[3], oms_helium_tank[2], mps_helium_tank[3];
	int MPS_Manifold_Press[2]; //0=LO2, 1=LH2
	int MPS_He_Reg_Press[4], MPS_He_Tank_Press[4];
	int Hydraulic_Press[3];

	// RMS arm animation status
	ANIMATIONCOMPONENT_HANDLE hAC_arm, hAC_sat, hAC_satref;
	MGROUP_TRANSFORM *rms_anim[7], *rms_rollout_anim;
	UINT anim_arm_sy, anim_arm_sp, anim_arm_ep, anim_arm_wp, anim_arm_wy, anim_arm_wr;
	UINT anim_rollout;
	double arm_sy, arm_sp, arm_ep, arm_wp, arm_wy, arm_wr;
	AnimState RMSRollout;
	bool RMS;
	//IK parameters
	VECTOR3 arm_wrist_pos;
	double lu,ll;
    double shoulder_neutral;
    double shoulder_range,shoulder_min,shoulder_max;
    double elbow_neutral;
    double elbow_range,elbow_min,elbow_max;
    double wrist_neutral;
    double wrist_range,wrist_min,wrist_max;

	MGROUP_TRANSFORM *sat_anim, *sat_ref;

	bool reset_mmu, reset_sat;
	OBJHANDLE hMMU, hSAT;
	double jettison_time;
	bool render_cockpit;
	VCHUDSPEC huds;
	EXTMFDSPEC mfds[10];
	double mfdbright[10];
	double pl_mass;
	double dT;
	VECTOR3 GVesselPos, GVesselVel;
	VESSELSTATUS Status;

	//Launch
	double Throttle_Bucket[2], OMS_Assist[2]; //start/end times
	//targets in SI (angles in degrees)
	double TgtInc, TgtLAN, TgtSpd, TgtAlt, TgtFPA; //targets for guidance
	double THeading, TAp, TPe, TTrA, TEcc, TgtRad;

	double MaxThrust;
	bool bAutopilot, bThrottle;
	bool bMECO, bZThrust;
	double tMECO, tSRBSep; //time(MET)
	double dV; // used for -Z translation
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
	int ops;
	int last_mfd;
	bool bFirstStep; //call functions in first timestep

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
	int START_TIME[4]; // day,hour,min,sec
	int TGT_ID, BODY_VECT;
	double P, Y, OM;
	bool MNVR, TRK, ROT;
	bool Pitch, Yaw, Roll;
	VECTOR3 InertialOrientation, InertialOrientationRad, AngularVelocity;
	VECTOR3 LVLHOrientation, LVLHOrientationLast, LVLHOrientationReqd, LVLHError, LVLHRateVector;
	VECTOR3 MNVR_OPTION, TRKROT_OPTION, REQD_ATT, PitchYawRoll, PitchYawRollLast, TargetAtt, ReqdRates, TrackRates;
	ORBITPARAM oparam;
	ELEMENTS el;
	double TrkRate;	
	double Mass;
	VECTOR3 PMI, Torque;
	//DAPConfig A, B, Edit;
	DAPConfig DAP[3]; //0=A, 1=B, 2=Edit
	int edit; //0=Blank, 1=A, 2=B
	int DAPMode[2]; //0=A, 1=B && 0=PRI, 1=ALT, 2=VERN
	double RotRate, AttDeadband, RateDeadband;
	bool NoseThrusters, TailThrusters, Thrusters; //Enabled/Disabled
	int JetsEnabled;

	//I-Loads
	double* stage1guidance[2];
	int stage1guidance_size;
	//double* stage1Vguidance;
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
	double t0;                  // reference time: liftoff
	double srb_separation_time; // simulation time at which SRB separation was initiated
	bool bMainEngine;           // main engine firing?
	bool bSeparationEngine;     // separation thrusters firing?
	MESHHANDLE hSRBMesh;
	PROPELLANT_HANDLE ph_main;  // handle for propellant resource
	THRUSTER_HANDLE th_main;    // engine handle
	THRUSTER_HANDLE th_bolt;    // separation bolt
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
};

#endif // !__ATLANTIS_H