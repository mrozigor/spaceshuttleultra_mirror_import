/****************************************************************************
  This file is part of Space Shuttle Ultra

  STS Orbiter vessel class definition



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

//Original copyright notice of Atlantis example
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
#include <vector>
#include "meshres_vc.h"
#include "dps/dps_defs.h"
#include "dps/MasterTimingUnit.h"
#include "dps/MMU.h"
#include "eps/eps_defs.h"
#include "gnc/IMU.h"
#include "mission/Mission.h"
#include "mps/SSME.h"
#include "mps/EIU.h"
#include "PanelGroup.h"
#include "vc/AtlantisPanel.h"
#include "vc/PanelF7.h"
#include "APU.h"
#include <EngConst.h>
#include "Discsignals.h"
#include "eva_docking/BasicExtAirlock.h"
#include "PIDControl.h"
#include "ISSUMLP.h"



typedef struct {
	double P;		//Pressure (psig)
	double T;		//Temperature (°R)
	double mdot;	//mass flow (lb/s)
} FLOWSTATE;




//Z
//-.5216187842e-1*sin(beta)+.5609219446*cos(alpha)
//-.2802319446*cos(alpha)*cos(beta)+.9135110136*sin(alpha)
//+.9417727780e-2*sin(alpha)*cos(beta)



const short VARSTATE_OK = 0;
const short VARSTATE_MISSING = 1;
const short VARSTATE_OFFSCALE_LOW = 2;
const short VARSTATE_LOW = 3;
const short VARSTATE_HIGH = 4;
const short VARSTATE_OFFSCALE_HIGH = 5;

static const char* STD_CLASS_NAME = "SpaceShuttleUltra";

//Sound IDs
const int RCS_SOUND = 1;
const static char* RCS_SOUND_FILE = "Sound\\ShuttleUltra\\RCS_Jet_Fire.wav";
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

const static char* TEXT_RCSCONTROL = "Controlling RCS";
const static char* TEXT_RMSCONTROL = "Controlling RMS";

// ==========================================================
// Mesh group indices for some components
// ==========================================================

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



const unsigned int convert[69] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};




typedef struct {
	HINSTANCE hDLL;
	SURFHANDLE pbi_lights;
	SURFHANDLE tkbk_label;
	SURFHANDLE clock_digits;
	SURFHANDLE digits_7seg;
	SURFHANDLE odslights;
	HBITMAP deu_characters;
	HDC DeuCharBitmapDC;
	HFONT font[1];
} GDIParams;

typedef struct {
	VECTOR3 Pitch;
	VECTOR3 Yaw;
	VECTOR3 Roll;			
} RefPoints;

struct DAPConfig {
	double PRI_ROT_RATE, PRI_ATT_DB, PRI_RATE_DB, PRI_ROT_PLS, PRI_COMP, PRI_TRAN_PLS;
	int PRI_P_OPTION, PRI_Y_OPTION; //0=ALL, 1=NOSE, 2=TAIL
	double ALT_RATE_DB, ALT_ON_TIME, ALT_DELAY;
	int ALT_JET_OPT, ALT_JETS;
	double VERN_ROT_RATE, VERN_ATT_DB, VERN_RATE_DB, VERN_ROT_PLS, VERN_COMP;
	int VERN_CNTL_ACC;

	DAPConfig& operator = (const DAPConfig& rhs) {
		// copy all values from other config into this one
		PRI_ROT_RATE=rhs.PRI_ROT_RATE;
		PRI_ATT_DB=rhs.PRI_ATT_DB;
		PRI_RATE_DB=rhs.PRI_RATE_DB;
		PRI_ROT_PLS=rhs.PRI_ROT_PLS;
		PRI_COMP=rhs.PRI_COMP;
		PRI_TRAN_PLS=rhs.PRI_TRAN_PLS;
		PRI_P_OPTION=rhs.PRI_P_OPTION;
		PRI_Y_OPTION=rhs.PRI_Y_OPTION;
		ALT_RATE_DB=rhs.ALT_RATE_DB;
		ALT_ON_TIME=rhs.ALT_ON_TIME;
		ALT_DELAY=rhs.ALT_DELAY;
		ALT_JET_OPT=rhs.ALT_JET_OPT;
		ALT_JETS=rhs.ALT_JETS;
		VERN_ROT_RATE=rhs.VERN_ROT_RATE;
		VERN_ATT_DB=rhs.VERN_ATT_DB;
		VERN_RATE_DB=rhs.VERN_RATE_DB;
		VERN_ROT_PLS=rhs.VERN_ROT_PLS;
		VERN_COMP=rhs.VERN_COMP;
		VERN_CNTL_ACC=rhs.VERN_CNTL_ACC;

		return *this;
	}

};

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

typedef struct {
	double leftElevon, rightElevon;
	double bodyFlap;
	double speedbrake;
	double rudder;
} AerosurfacePositions;


class PanelA4;
class PanelC2;
//class PanelF7;
class PanelO3;
class AtlantisSubsystemDirector;
class OMSSubsystem;
class AirDataProbeSystem;
class RMSSystem;
class StbdMPMSystem;
class ActiveLatchGroup;
class MCA;
class MechActuator;

class Atlantis_Tank;
class Atlantis_SRB;



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

typedef enum {
	LEFT = 0,
	RIGHT
} SIDE;

class CommModeHandler;
class SSUOptions;

using class discsignals::DiscreteBundleManager;
using class dps::ShuttleBusManager;
using class dps::MDM;

// ==========================================================
// Interface for derived vessel class: Atlantis
// ==========================================================

class Atlantis: public VESSEL3 {
	friend class PayloadBayOp;
	friend class GearOp;
	friend class PanelA4;
	friend class PanelA8;
	friend class PanelC2;
	friend class vc::PanelF7;
	friend class PanelO3;
	friend class Keyboard;
	friend class CRT;
	friend class vc::MDU;
	friend class vc::DAPControl;
public:
	SSUOptions* options;
	/* **************************************************
	 * Mission data reference
	 * **************************************************/
	mission::Mission* pMission;
	/* **************************************************
	 * Subsystem short cuts
	 * **************************************************/
	eps::ACBusSystem *pACBusSystem;
	dps::MasterTimingUnit* pMTU;		//just quick reference. Don't ever delete this, yourself.
	dps::IDP* pIDP[4];
	/**
	 * Flight forward MDMs
	 * Card 3 is SIO
	 * * Channel 1 connects to MTU (FF4 not connected)
	 * Card 8 is Analog output
	 * * Channel 6 is Nosewheel Steering CMD
	 */
	dps::MDM* pFF[4];
	/** 
	 * Flight Aft MDM
	 * Card 0 is Analog out
	 * * Channels 0-11 connect to ASA
	 * 
	 */
	dps::MDM* pFA[4];
	/**
	 * Payload MDMs
	 * Card 2 is Discrete output
	 * Card 10 is Discrete output
	 * Card 12 is Analog input
	 * * Channel 3 is APU 3 Fuel quantity
	 * * Channel 8 is APU 1 Fuel quantity
	 * * Channel 10 is APU 2 Fuel quantity
	 */
	dps::MDM* pPL[2];
	dps::MDM* pLF1;
	dps::MDM* pLA1;
	dps::MDM* pLM1;
	dps::MDM* pOF[4];
	dps::MDM* pOA[3];
	dps::MDM* pLL[2];
	dps::MDM* pLR[2];
	dps::MDM* pFMDM[2];
	dps::GPC* pGPC[5];
	dps::MMU* pMMU[2];
	OMSSubsystem* pOMS;
	gnc::IMU* pIMU[3];
	mps::SSME* pSSME[3];
	mps::EIU* pEIU[3];
	eps::Inverter* pInverter[3];
	APU* pAPU[3];
	MCA* pFMC1;
	MCA* pFMC2;
	MCA* pFMC3;
	MCA* pMMC1;
	MCA* pMMC2;
	MCA* pMMC3;
	MCA* pMMC4;
	MCA* pAMC1;
	MCA* pAMC2;
	MCA* pAMC3;

	dps::SimpleGPCSystem *pSimpleGPC;

	MechActuator* pSTYDoorMotor;
	MechActuator* pSTZDoorMotor;
	/**
	 * Strategy pattern for the external airlock subsystem
	 */
	eva_docking::BasicExternalAirlock* pExtAirlock;
	AirDataProbeSystem* pADPS;
	RMSSystem* pRMS;
	StbdMPMSystem* pMPMs;

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
	//int MET[4], Launch_time[4], MET_Add[4]; // day,hour,min,sec
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
	//UINT mesh_rms;							   // index for RMS mesh
	//UINT mesh_mpm;							   // index for STBD MPM mesh
	//UINT mesh_tank;                            // index for external tank mesh
	//UINT mesh_srb[2];                          // index for SRB meshes
	UINT mesh_kuband;						   // index for KU band antenna mesh
	UINT mesh_extal;						   // index for external airlock mesh
	UINT mesh_ods;							   // index for	ODS outside mesh
	UINT mesh_cargo_static;					   // index for static cargo mesh
	UINT mesh_dragchute;					   // index for drag chute mesh
	UINT mesh_heatshield;					   //index for heat shield mesh

	//**********************************************************
	//* public methods
	//**********************************************************
	Atlantis (OBJHANDLE hObj, int fmodel);
	~Atlantis();
	void AddOrbiterVisual (const VECTOR3 &ofs);
	virtual DiscreteBundleManager* BundleManager() const;
	virtual ShuttleBusManager* BusManager() const;
	// Overloaded callback functions
	void clbkAnimate (double simt);
	int  clbkConsumeBufferedKey (DWORD key, bool down, char *kstate);
	void clbkDrawHUD (int mode, const HUDPAINTSPEC *hps, HDC hDC);
	bool clbkDrawHUD (int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp);
	void clbkFocusChanged (bool getfocus, OBJHANDLE hNewVessel, OBJHANDLE hOldVessel);
	virtual int clbkGeneric(int msgid = 0, int prm = 0, void *context = 0);
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
	virtual double GetETGOXMassFlow() const;
	virtual double GetETGH2MassFlow() const;
	virtual short GetETPropellant() const;
	virtual unsigned short GetGPCMET(unsigned short usGPCID, unsigned short &usDay, unsigned short &usHour, unsigned short& usMin, unsigned short &usSec);
	virtual double GetMET() const;
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
	virtual bool IsValidSPEC(int gpc, int spec) const;
	virtual unsigned int GetGPCMajorMode() const;
	virtual double GetTgtSpeedbrakePosition() const;
	virtual double GetActSpeedbrakePosition() const;
	virtual AnimState::Action GetGearState() const;
	int GetSoundID() const;
	double GetThrusterGroupMaxThrust(THGROUP_HANDLE thg) const;
	double GetPropellantLevel(PROPELLANT_HANDLE ph) const;
	void Jettison ();
	void OperateSpeedbrake (AnimState::Action action);
	void PaintMarkings (SURFHANDLE tex);
	virtual bool RegisterMDU(unsigned short usMDUID, vc::MDU* pMDU);
	void RevertSpeedbrake ();
	/* ***************************************************************
	 * Setters
	 *****************************************************************/
	virtual void SetAirDataProbeDeployment(int side, double position);
	void SetBayDoorLatchPosition (int gang, double pos);
	void SetBayDoorPosition (double pos);
	void SetETUmbDoorPosition(double pos, int door);
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
	void SetGPCMajorMode (unsigned int newMajorMode);
	//virtual void SetExternalAirlockVisual(bool fExtAl, bool fODS);
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

	/** 
	 *	@param fThrustLevel Thrust level from 0.0 to 1.0
	 */
	bool SetSSMEThrustLevel(unsigned short usMPSNo, double fThrustLevel);

	/**
	 * @param usMPSNo numerical ID of the SSME
	 * @return 0 < SSME PC < 1
	 */
	double GetSSMEThrustLevel( unsigned short usMPSNo );

	//Thruster Control; called from OrbitDAP class
	void EnableThrusters(const int Thrusters[], int nThrusters);
	void DisableThrusters(const int Thrusters[], int nThrusters);

	/**
	 * Calls VESSEL::AttachChild and adds mass of child to shuttle mass
	 * Should always be called instead of AttachChild.
	 */
	bool AttachChildAndUpdateMass(OBJHANDLE child, ATTACHMENTHANDLE attachment, ATTACHMENTHANDLE child_attachment) const;
	/**
	 * Calls VESSEL::DetachChild and subtracts mass of child from shuttle mass
	 * Should always be called instead of DetachChild.
	 */
	bool DetachChildAndUpdateMass(ATTACHMENTHANDLE attachment, double vel = 0.0) const;

	bool AreMCADebugMessagesEnabled() const throw();

	virtual void UpdateODSAttachment(const VECTOR3& pos, const VECTOR3& dir, const VECTOR3& up);
	virtual ATTACHMENTHANDLE GetODSAttachment() const;

	/**
	 * If no docking port yet defined, create new only docking port at that location
	 * If docking port already defined and no vessel docked, move to new position.
	 * Otherwise, the function fails.
	 * 
	 * @param pos The desired position of the docking port in body coordinates
	 * @return true if successful and docking port at the desired location, false if failed. 
	 */
	virtual bool CreateDockingPort(const VECTOR3& pos);

	/**
	 * Wrapper for AddAnimationComponent
	 * MGROUP_TRANSFORM passed MUST be allocated with new and will be deleted by Atlantis destructor
	 */
	ANIMATIONCOMPONENT_HANDLE AddManagedAnimationComponent(UINT anim, double state0, double state1,
		MGROUP_TRANSFORM *trans, ANIMATIONCOMPONENT_HANDLE parent = NULL);

	void IgniteSRBs();
	void GLSAutoSeqStart();
	void SignalGSEBreakHDP();
	void TriggerLiftOff();
	void StartROFIs();

	//Communication with LCC
	virtual void SynchronizeCountdown(double launch_mjd);
	virtual void StartRSLSSequence();

	virtual bool HydraulicsOK();

	void ToggleGrapple (void);
	void ToggleArrest (void);
	void UpdateMesh ();
	void UpdateSSMEGimbalAnimations();

	/*
	void RegisterVC_CdrMFD ();
	void RegisterVC_PltMFD ();
	void RegisterVC_CntMFD ();
	void RegisterVC_AftMFD ();
	void RedrawPanel_MFDButton (SURFHANDLE surf, int mfd);
	*/

	
	//mission::Mission* the_mission;
	
	/**
	 * Pointer to the A7A8 custom panel region
	 */
	vc::AtlantisPanel* pA7A8Panel;
	/**
	 * Pointer to the A8 (RMS) panel region
	 */
	vc::AtlantisPanel* pPanelA8;
	


	AerosurfacePositions aerosurfaces;
	//double kubd_proc; // Ku-band antenna deployment state (0=retracted, 1=deployed)
	double spdb_proc, spdb_tgt; // Speedbrake deployment state (0=fully closed, 1=fully open)
	double ldoor_drag, rdoor_drag; // drag components from open cargo doors
	//bool center_arm;
	//bool arm_moved;
	bool do_eva;
	bool do_plat;
	bool do_cargostatic;
	VECTOR3 orbiter_ofs;
	VECTOR3 ofs_sts_sat;
	VECTOR3 cargo_static_ofs;
	VISHANDLE vis;      // handle for visual - note: we assume that only one visual per object is created!
	MESHHANDLE hOrbiterMesh, hOrbiterCockpitMesh, hOrbiterVCMesh, 
		hMidDeckMesh, /*hOrbiterRMSMesh,*/ /*hOBSSMPMMesh, hTankMesh, hSRBMesh[2],*/
		hODSMesh, hPanelA8Mesh, hDragChuteMesh; // mesh handles
	MESHHANDLE hKUBandMesh;
	MESHHANDLE hExtALMesh;
	MESHHANDLE hHeatShieldMesh;
	DEVMESHHANDLE hDevHeatShieldMesh;
	DEVMESHHANDLE hDevOrbiterMesh;
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
	//ATTACHMENTHANDLE ahRMS, ahOBSS;
	ATTACHMENTHANDLE ahMMU[2];
	ATTACHMENTHANDLE ahDockAux;
	ATTACHMENTHANDLE ahExtAL[2];
	ATTACHMENTHANDLE ahCenterActive[3];
	ATTACHMENTHANDLE ahCenterPassive[4];
	ATTACHMENTHANDLE ahStbdPL[4];
	ATTACHMENTHANDLE ahPortPL[4];
	ATTACHMENTHANDLE ahET, ahLeftSRB, ahRightSRB;
	
	AtlantisSubsystemDirector* psubsystems;
	
	PayloadBayOp *plop; // control and status of payload bay operations
	//GearOp *gop; // control and status of landing gear
	PanelA4 *panela4;
	//PanelA8 *panela8;
	PanelC2 *panelc2;
	//PanelC3 *panelc3; // PanelC3 operations
	//PanelF7 *panelf7;
	PanelO3 *panelo3;
	vc::PanelR2 *panelr2; // temporary
	Keyboard *CDRKeyboard;
	Keyboard *PLTKeyboard;
	//bool PitchActive,YawActive,RollActive;     // Are RCS channels active?

	//OBJHANDLE ThisVessel;

	//-----------------------------------------------
	//PLB Lights
	//-----------------------------------------------
	LightEmitter* PLBLight[6];
	void ControlPLBLights();
	VECTOR3 PLBLightPosition[6];
	BEACONLIGHTSPEC bspec[6];
	bool bPLBLights;




private:
	double slag1, slag2, slag3;
	PSTREAM_HANDLE pshSlag1[2], pshSlag2[2], pshSlag3[2];
	DOCKHANDLE hODSDock;
	PSTREAM_HANDLE reentry_flames;
	PARTICLESTREAMSPEC PS_REENTRY;

	bool bSRBCutoffFlag;
	bool bLiftOff;
	bool bHasKUBand;
	bool bHasODS;
	bool bHasExtAL;
	bool bMidDeckVisible;

	CommModeHandler* pCommModeHandler;

	/* *************************************************
	 * Panel groups
	 * *************************************************/
	vc::PanelGroup<Atlantis> pgForward;
	vc::PanelGroup<Atlantis> pgLeft;
	vc::PanelGroup<Atlantis> pgCenter;
	vc::PanelGroup<Atlantis> pgRight;
	vc::PanelGroup<Atlantis> pgOverhead;
	vc::PanelGroup<Atlantis> pgAftStbd;
	vc::PanelGroup<Atlantis> pgAft;
	vc::PanelGroup<Atlantis> pgAftPort;

	/**
	 * all animations that need to be deleted in destructor
	 */
	std::vector<MGROUP_TRANSFORM*> vpAnimations;

	std::vector<ActiveLatchGroup*> pActiveLatches;

	//GPC programs
	//dps::RSLS *rsls;
	dps::RSLS_old *rsls;

	SURFHANDLE tex_rcs;
	
	void SeparateBoosters (double srb_time);
	void DetachSRB(SIDE side, double thrust, double prop) const;
	void SeparateMMU (void);
	void SeparateTank (void);

	/**
	 * Copies settings (thrust & ISP) from one thruster to another
	 * \param th Thruster to change settings on
	 * \param v Vessel containing th_ref
	 * \param th_ref Thruster to copy settings from
	 */
	void CopyThrusterSettings(THRUSTER_HANDLE th, const VESSEL* v, THRUSTER_HANDLE th_ref);
	
	void SSMEEngControl(unsigned short usEng) const;
	void OMSEngControl(unsigned short usEng);

	void StopAllManifolds();
	void FireAllNextManifold();
	void AddPrimaryRCSExhaust(THRUSTER_HANDLE thX);
	void CreateRightARCS(const VECTOR3& ref_pos);
	void CreateLeftARCS(const VECTOR3& ref_pos);
	void CreateFRCS(const VECTOR3& ref_pos);
	void CreateSSMEs(const VECTOR3& ofs);
	void CreateMPSGOXVents(const VECTOR3& ref_pos);
	bool bUseRealRCS;
	void CreateOrbiterTanks();
	unsigned short usCurrentPlayerChar;
	bool bCommMode;
	void DefineSSMEExhaust();
	//void SignalGSEBreakHDP();
	//-----------------------------------
	void ShowMidDeck();
	void HideMidDeck();
	void ShowODS() const;
	void HideODS() const;
	void ShowExtAL() const;
	void HideExtAL() const;
	int Lua_InitInterpreter (void *context);
	int Lua_InitInstance (void *context);
	//-----------------------------------
	void DefineKUBandAnimations();
	void LaunchClamps();
	void CreateAttControls_RCS(VECTOR3 center);
	//void DisableAllRCS();
	//void EnableAllRCS();
	void DisableControlSurfaces();
	void EnableControlSurfaces();
	/**
	 * Creates fake thrusters for THC, RHC and SPDBK/THROT input
	 */
	void CreateDummyThrusters();

	/**
	 * Updates discrete port values for all hand controllers (RHC, THC, SPDBK/THROT).
	 */
	void UpdateHandControllerSignals();

	//Landing Gear
	void DeployLandingGear();
	void RetractLandingGear();
	void ArmGear();
	void DefineTouchdownPoints();
	bool GearArmed() const;
	void DeployDragChute();
	void JettisonDragChute();

	void RealizeSubsystemConnections();

	//
	void SavePayloadState(FILEHANDLE scn) const;
	bool ParsePayloadLine(const char* pszLine);

	/**
	 * React on Key "V", switching the view from flight deck to Mid Deck
	 * and back.
	 */
	void ToggleVCMode();
	
	//RMS
	bool SatStowed() const;
	//ATTACHMENTHANDLE CanArrest() const;
	ATTACHMENTHANDLE GetAttachmentTarget(ATTACHMENTHANDLE attachment, const char* id_string, OBJHANDLE* vessel=NULL) const;

	void CreateETAndSRBAttachments(const VECTOR3 &ofs);

	Atlantis_Tank* GetTankInterface() const;
	Atlantis_SRB* GetSRBInterface(SIDE side) const;
	ISSUMLP* GetMLPInterface() const;

	/**
	 * Called from clbkPostCreation.
	 * Loops through child attachments and adds their mass to shuttle mass.
	 */
	double GetMassOfAttachedObjects() const;
	void UpdateMass() const;

	void Twang(double timeToLaunch) const;
	//Launch
	void AutoMainGimbal(double DeltaT);
	void SteerGimbal(double DeltaT);
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
	void GPC(double simt, double dt);
	void Maneuver(double dt);
	void SetILoads();
	//void Test();

	//OMS
	/**
	 * @param engine 0 for Left, 1 for Right
	 * @param pitch angle in degrees (relative to null position)
	 * @param yaw angle in degrees (relative to null position)
	 * @return false if gimbal values are out of range, true otherwise
	 */
	void GimbalOMS(int engine, double pitch, double yaw);
	void OMSTVC(const VECTOR3 &Rates, double SimDT);
	//void GimbalOMS(const VECTOR3 &Targets);

	//DAP
	//VECTOR3 ConvertOrbiterAnglesToLocal(const VECTOR3 &Angles);
	//VECTOR3 ConvertVectorBetweenOrbiterAndM50(const VECTOR3 &Input);
	//VECTOR3 ConvertLVLHAnglesToM50(const VECTOR3 &Input);
	//MATRIX3 ConvertLVLHAnglesToM50Matrix(const VECTOR3 &Input);
	//calc attitude/attitude error
	//VECTOR3 CalcLVLHAttitude();
	//VECTOR3 CalcRelLVLHAttitude(VECTOR3 &Target);
	//VECTOR3 CalcPitchYawRollAngles(VECTOR3 &RelAttitude);
	//MATRIX3 CalcPitchYawRollRotMatrix(); //handles M50 coordinates
	//VECTOR3 CalcPitchYawRollAngles(); //handles M50 coordinates
	//bool CheckLimits(double dNum1, double dNum2, double dLim);
	//double NullStartAngle(double Rates, AXIS Axis) const;

	void UpdateTranslationForces();

	void UpdateOrbiterTexture(const std::string& strTextureName);

	

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
	UINT anim_stzd;							   // handle for +Z Star Tracker Door animation
	UINT anim_styd;							   // handle for -Y Star Tracker Door animation
	
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
	//UINT anim_camRMSElbowPan;
	//UINT anim_camRMSElbowTilt;
	
	typedef enum {CAM_A=0, CAM_B=1, CAM_C=2, CAM_D=3} PLBD_CAM;
	double camYaw[4], camPitch[4];
	//double camRMSElbowPan, camRMSElbowTilt;
	//RMS Camera rot/direction
	//VECTOR3 camRMSElbowLoc[2];
	
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

	bool bSSMEsDefined, bOMSDefined, bRCSDefined, bControllerThrustersDefined;

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
	std::vector<UINT> vExRCS;				   // RCS exhaust
	//bool RCSEnabled;
	THGROUP_HANDLE thg_main, thg_srb, thg_retro;          // handles for thruster groups
	CTRLSURFHANDLE hrudder, hlaileron, hraileron, helevator, hbodyflap;
	bool ControlSurfacesEnabled;

	bool bSSMEGOXVent;

	//CLR
	VECTOR3 SSMET_GOX_REF1;
	VECTOR3 SSMEL_GOX_REF1;
	VECTOR3 SSMER_GOX_REF1;

	// Helium Tanks
	PROPELLANT_HANDLE oms_helium_tank[2], mps_helium_tank[3];
	int MPS_Manifold_Press[2]; //0=LO2, 1=LH2
	int MPS_He_Reg_Press[4], MPS_He_Tank_Press[4];
	int Hydraulic_Press[3];

	bool RMS, STBDMPM;

	bool ControlRMS;
	//Hand controller input
	VECTOR3 THCInput, RHCInput;
	VECTOR3 AltKybdInput; // uses arrow,PgUp/PgDn keys to provide translation inputs; axes correspond to RCS FWD SENSE
	//Thruster commands
	VECTOR3 TranslationCommand, RotationCommand;

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
	//VESSELSTATUS Status;

	//base vectors;
	VECTOR3 LVLH_X, LVLH_Y, LVLH_Z;

	//Launch
	double Throttle_Bucket[2], OMS_Assist[2], RollToHeadsUp; //start/end times
	//targets in SI (angles in degrees)
	double TgtInc, TgtLAN, TgtSpd, TgtAlt, TgtFPA; //targets for guidance
	double THeading, TAp, TPe, TTrA, TEcc, TgtRad;
	PIDControl SSMEGimbal[3][3], SRBGimbal[2][3];
	DiscOutPort ZTransCommand;
	bool ETSepTranslationInProg;
	double ETSepMinusZDV;

	double MaxThrust; // maximum thrust that can be commanded; usually 104.5
	bool bAutopilot, bThrottle;
	bool bMECO, bZThrust;
	double tMECO, tSRBSep; //time(MET)
	double dV; // used for -Z translation
	VECTOR3 EngineNullPosition[3];
	int EngineFail;
	double EngineFailTime;
	bool bEngineFail;
	bool bEnableMCADebug;
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

	// Entry
	PIDControl BodyFlap, ElevonPitch; // used to maintain AoA
	PIDControl PitchControl;

	//GPC
	//int ops, SMOps;
	//unsigned int ops;
	unsigned int SMOps;
	int last_mfd;
	bool firstStep; //call functions in first timestep
	//Data Input
	KeyboardInput DataInput[3];
	int CRT_SEL[2]; //0=CDR, 1=PLT
	int item;
	//CRT* Display[3];
	CRT* newmfd;

	DiscreteBundleManager* bundleManager;
	dps::ShuttleBusManager* busManager;
	
	vc::MDU* mdus[11];
	UINT mfds[11]; //stores MDUID for corresponding MFD index

	//MNVR
	double curOMSPitch[2], curOMSYaw[2];

	//DAP
	VECTOR3 ReqdRates;

	vector<double> stage1guidance[2];

	double fTimeCameraLabel;
	NOTEHANDLE nhCameraLabel;
	char pszCameraLabelBuffer[80];

	bool bIlluminated;

	//sound
	int SoundID;

	mutable bool fSSMEHandleErrorFlag;

	bool bLastCamInternal;

	bool bPLBDCamPanLeft[5], bPLBDCamPanRight[5], bPLBDCamTiltUp[5], bPLBDCamTiltDown[5];

	// used to trigger RCS sounds
	int lastRotCommand[3], lastTransCommand[3]; // -1, 0 or 1
	int lastRMSSJCommand; // -1, 0 or 1

	//DiscPorts
	DiscInPort SpdbkThrotAutoIn;
	DiscOutPort SpdbkThrotAutoOut, SpdbkThrotCDROut, SpdbkThrotPLTOut;
	DiscInPort BodyFlapAutoIn;
	DiscOutPort BodyFlapAutoOut, BodyFlapManOut;
	DiscInPort PitchAutoIn, RollYawAutoIn;
	//DiscOutPort PitchAutoOut, RollYawAutoOut;
	//DiscInPort PitchCSSIn, RollYawCSSIn;
	//DiscOutPort PitchCSSOut, RollYawCSSOut;
	DiscInPort AftSense, AftFltCntlrPwr, CdrFltCntlrPwr, PltFltCntlrPwr;

	DiscOutPort SpdbkThrotPort;
	DiscOutPort RHCInputPort[3], THCInputPort[3];
	DiscInPort RotThrusterCommands[3], TransThrusterCommands[3];
	//DiscInPort LeftElevonCommand, RightElevonCommand;
	DiscInPort ElevonCommand, AileronCommand;

	// Pan/Tilt PLBD cameras and RMS elbow cam
	// 0=A, 1=B, 2=C, 3=D, 4=RMS Elbow
	DiscInPort PLBDCamPanLeft[5], PLBDCamPanRight[5], PLBDCamTiltUp[5], PLBDCamTiltDown[5];
	DiscInPort PTULowSpeed[5];
	DiscOutPort PLBDCamPanLeft_Out[5], PLBDCamPanRight_Out[5];
	DiscOutPort PLBDCamTiltUp_Out[5], PLBDCamTiltDown_Out[5];

	DiscOutPort RMSGrapple, RMSRelease; // used by RMS dialog
	DiscOutPort RMS_RHCInput[3], RMS_THCInput[3], RMSDrivePlus, RMSDriveMinus;
	DiscInPort RMSSpeedIn;
	DiscOutPort RMSSpeedOut;
	DiscInPort MPSPwr[2][3], MPSHeIsolA[3], MPSHeIsolB[3];
	DiscInPort OMSArm[2], OMSArmPress[2], OMSFire[2], OMSPitch[2], OMSYaw[2];

	void AddKUBandVisual(const VECTOR3 ofs);
	//void TriggerLiftOff();
	void DisplayCameraLabel(const char* pszLabel);
};

VECTOR3 CalcOMSThrustDir(unsigned int side, double pitch, double yaw);
/**
* Calculates lift, drag and moment coefficients for given mach, AOA and aerosurface positions
*/
void GetShuttleVerticalAeroCoefficients(double mach, double degAOA, const AerosurfacePositions* aerosurfaces, double* cl, double* cm, double* cd);



#if defined(IS_SSU_DLL_MODULE)
DLLCLBK mission::Mission* ssuGetMission(const std::string& filename);
void ClearMissionManagementMemory();
void InitMissionManagementMemory();
#endif

#endif // !__ATLANTIS_H


