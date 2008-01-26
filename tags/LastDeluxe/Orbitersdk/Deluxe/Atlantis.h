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

#include "orbitersdk.h"
#include <math.h>

// ==========================================================
// Some Orbiter-related parameters
// ==========================================================

const double ORBITER_EMPTY_MASS = 81100.0;
// Orbiter empty mass [kg]

const double ORBITER_MAX_PROPELLANT_MASS = 14538.0;
// Amount of fuel the orbiter can hold in internal OMS tanks

const double ORBITER_MAIN_THRUST = 2170732.15; //Nominal "104%" thrust
// Vacuum thrust rating per main engine

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
// using MMH/N2O4 (monomethyl hydrazine/nitrogen tetroxide)

const double ORBITER_RCS_ISP0 = ORBITER_OMS_ISP0;
const double ORBITER_RCS_ISP1 = ORBITER_RCS_ISP0*0.75;
// Vacuum and sea-level fuel-specific impulse for Reaction Control System [m/s]

const double GEAR_OPERATING_SPEED = 0.3;
// Opening/closing speed of landing gear (1/sec)
// => gear cycle ~ 3 sec

const double DOOR_OPERATING_SPEED = 0.007353;
// Opening/closing speed of payload bay doors (1/sec)
// This contains the door opening sequence (63 sec for each door) and an
// interval of 10 sec between the two door operations

const double RAD_OPERATING_SPEED = 0.025;
// Deployment/stowing speed of radiators (1/sec)
// => radiator cycle = 40 sec

const double RADLATCH_OPERATING_SPEED = 0.2;
// Release/engaging speed of radiator latches (1/sec)
// => radiator latch cycle = 5 sec

const double KU_OPERATING_SPEED = 0.0435;
// Deployment speed of the Ku Band antenna (1/sec)
// cycle is 23 sec

const double SPEEDBRAKE_OPERATING_SPEED = 0.20284;
// Deployment speed of the speedbrake (1/sec)
// cycle is 4.93 sec

const double ARM_OPERATING_SPEED = 0.005;
// RMS arm joint rotation speed (rad/sec)
const double ARM_TRANSLATE_SPEED = 0.1;
// RMS IK translation speed (m/s)

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


// ==========================================================
// Some Tank-related parameters
// ==========================================================

const double TANK_MAX_PROPELLANT_MASS = 719115.0;
// Main tank propellant mass [kg]

const double TANK_EMPTY_MASS = 26535; //Super light weight tank, 58500lb, STS-91 on
                                      //STS-1 to STS-5 tank 76000lb 35425.0kg
                                      //Light weight tank 66000lb 29937kg, STS-6 to STS-90
// Main tank empty mass

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

const double SRB_THRUST = 17064000;
// Vacuum SRB thrust per unit [N]

const double MAX_ATT_LAUNCH = 1e-5;
const double MAX_ROLL_SRB = 2.5e-5;
// Max attitude thrust during launch phase (temporary)

const double SRB_STABILISATION_TIME = 4;
// MET: -SRB ignition

const double SRB_SEPARATION_TIME = 126.6;
// MET: SRB separation
const int SRB_nt = 30;

const double SRB_CUTOUT_TIME = 140.0;
// MET: engine shutdown

// ==========================================================
// Mesh offsets for various configurations
// ==========================================================

const VECTOR3 OFS_ZERO             = { 0.0, 0.0,  0.0  };
const VECTOR3 OFS_LAUNCH_ORBITER   = { 0.0, 6.22,-7.795};
const VECTOR3 OFS_LAUNCH_TANK      = { 0.0,-1.91, 5.72 };
const VECTOR3 OFS_LAUNCH_RIGHTSRB  = { 6.2,-1.91,-5.68 };
const VECTOR3 OFS_LAUNCH_LEFTSRB   = {-6.2,-1.91,-5.68 };
const VECTOR3 OFS_WITHTANK_ORBITER = { 0.0, 4.79,-9.185};
const VECTOR3 OFS_WITHTANK_TANK    = { 0.0,-3.34, 4.33 };
const VECTOR3 ORBITER_DOCKPOS      = { 0.0, 2.44,10.44 };
const VECTOR3 OFS_MMU              = {0,2.44,10.44};

// ==========================================================
// Mesh group indices for some components
// ==========================================================

const UINT MESH_UNDEFINED = (UINT)-1;

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
#define AID_F6_BTN1    204
#define AID_F6_BTN2    205
#define AID_F6_MAX     220

typedef struct {
	HINSTANCE hDLL;
	SURFHANDLE tkbk_label;
	HFONT font[1];
} GDIParams;

// ==========================================================
// Interface for derived vessel class: Atlantis
// ==========================================================

class Atlantis: public VESSEL2 {
	friend class PayloadBayOp;
	friend class GearOp;
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
	void OperateSpeedbrake (AnimState::Action action);
	void RevertSpeedbrake ();
	void SetAnimationArm (UINT anim, double state);
	void SetAnimationIKArm(VECTOR3 arm_wrist_dpos);
	VECTOR3 CalcAnimationFKArm();
    void PaintMarkings (SURFHANDLE tex);
    char WingName[256];
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
	WORD srb_id1, srb_id2;

	//double kubd_proc; // Ku-band antenna deployment state (0=retracted, 1=deployed)
	double spdb_proc; // Speedbrake deployment state (0=retracted, 1=deployed)
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
	MESHHANDLE hOrbiterMesh, hOrbiterCockpitMesh, hOrbiterVCMesh, hTankMesh, hSRBMesh; // mesh handles
	char cargo_static_mesh_name[256];
	ATTACHMENTHANDLE sat_attach, rms_attach;
	VECTOR3 arm_tip[3];

	// Overloaded callback functions
	void clbkSetClassCaps (FILEHANDLE cfg);
	void clbkSetStateEx (const void *status);
	void clbkLoadStateEx (FILEHANDLE scn, void *vs);
	void clbkSaveState (FILEHANDLE scn);
	void clbkFocusChanged (bool getfocus, OBJHANDLE hNewVessel, OBJHANDLE hOldVessel);
	void clbkPostStep (double simt, double simdt, double mjd);
	bool clbkPlaybackEvent (double simt, double event_t, const char *event_type, const char *event);
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
	bool PitchActive,YawActive,RollActive;     // Are RCS channels active?

private:
	void AutoMainGimbal();
	void SteerGimbal();
	void LaunchClamps();
	void CreateAttControls_RCS(VECTOR3 center);
	void DisableAllRCS();
	void EnableAllRCS();
	bool SatGrappled() const { return GetAttachmentStatus (rms_attach) != 0; }
	bool SatStowed() const { return GetAttachmentStatus (sat_attach) != 0; }
	ATTACHMENTHANDLE CanArrest() const;

	UINT anim_door;                            // handle for cargo door animation
	UINT anim_rad;                             // handle for radiator animation
	UINT anim_kubd;                            // handle for Ku-band antenna animation
	UINT anim_elev;                            // handle for elevator animation
	UINT anim_bf;                              // handle for body flap animation
	UINT anim_laileron;						   // handle for left aileron animation
	UINT anim_raileron;						   // handle for right aileron animation
	UINT anim_rudder;						   // handle for rudder animation
	UINT anim_spdb;                            // handle for speed brake animation
	UINT mesh_orbiter;                         // index for orbiter mesh
	UINT mesh_cockpit;                         // index for cockpit mesh for external view
	UINT mesh_vc;                              // index for virtual cockpit mesh
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


	
	
	
	PROPELLANT_HANDLE ph_oms, ph_tank, ph_srb; // handles for propellant resources
	THRUSTER_HANDLE th_main[3];                // handles for orbiter main engines
	THRUSTER_HANDLE th_srb[2];                 // handles for SRB engines
    THRUSTER_HANDLE th_att_rcs[18];            // 12 for rotation, 6 virtual
	bool RCSEnabled;
	THGROUP_HANDLE thg_main, thg_srb;          // handles for thruster groups

	// RMS arm animation status
	ANIMATIONCOMPONENT_HANDLE hAC_arm, hAC_sat, hAC_satref;
	MGROUP_TRANSFORM *rms_anim[6];
	UINT anim_arm_sy, anim_arm_sp, anim_arm_ep, anim_arm_wp, anim_arm_wy, anim_arm_wr;
	double arm_sy, arm_sp, arm_ep, arm_wp, arm_wy, arm_wr;
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
