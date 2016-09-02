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
#include "dps/SSME_SOP.h"
#include "dps/ATVC_SOP.h"
#include "dps/RSLS_old.h"
#include "mps/SSME.h"
#include "mps/EIU.h"
#include "mps/HeliumSystem.h"
#include "mps/MPS.h"
#include "PanelGroup.h"
#include "vc/AtlantisPanel.h"
#include "vc/PanelF7.h"
#include "vc\7SegDisp_RCSOMS_PRPLT_QTY.h"
#include "APU.h"
#include <EngConst.h>
#include "Discsignals.h"
#include "eva_docking/BasicExtAirlock.h"
#include "eva_docking\TunnelAdapterAssembly.h"
#include "ISSUMLP.h"
#include "gnc/ATVC.h"
#include "comm\DeployedAssembly.h"
#include "Sensor.h"


const short VARSTATE_OK = 0;
const short VARSTATE_MISSING = 1;
const short VARSTATE_OFFSCALE_LOW = 2;
const short VARSTATE_LOW = 3;
const short VARSTATE_HIGH = 4;
const short VARSTATE_OFFSCALE_HIGH = 5;

static const char* STD_CLASS_NAME = "SpaceShuttleUltra";

//Sound IDs
const static char* SOUND_DIRECTORY = "Sound\\_CustomVesselsSounds\\SpaceShuttleUltra\\";
const static char* AIR_CONDITIONING_SOUND_FILE = "Orbiter_AC.wav";
const int RCS_SOUND = 1;
const static char* RCS_SOUND_FILE = "RCS_Jet_Fire.wav";
const int MASTER_ALARM = 2;
const static char* MASTER_ALARM_FILE = "Master_Alarm.wav";
const int APU_START = 3;
const static char* APU_START_FILE = "APU_start_up.wav";
const int APU_RUNNING = 4;
const static char* APU_RUNNING_FILE = "APU_continues.wav";
const int APU_SHUTDOWN = 5;
const static char* APU_SHUTDOWN_FILE = "APU_shut_down.wav";
const int SSME_START = 6;
const static char* SSME_START_FILE = "SSME_ignition.wav";
const int SSME_RUNNING = 7;
const static char* SSME_RUNNING_FILE = "SSME_sustain.wav";
const int SSME_SHUTDOWN = 8;
const static char* SSME_SHUTDOWN_FILE = "SSME_shutdown.wav";
const int SWITCH_GUARD_SOUND = 9;
const static char* SWITCH_GUARD_FILE = "switch_guard.wav";
const int SWITCH_THROW_SOUND = 10;
const static char* SWITCH_THROW_FILE = "switch_throw.wav";
const int KEY_PRESS_SOUND = 11;
const static char* KEY_PRESS_FILE = "key_press.wav";

const static char* TEXT_RCSCONTROL = "Controlling RCS";
const static char* TEXT_RMSCONTROL = "Controlling RMS";

// ==========================================================
// Mesh group indices for some components
// ==========================================================

 const int MFDGROUPS[11] = {
	GRP_CDR1_VC,GRP_CDR2_VC,GRP_PLT1_VC,GRP_PLT2_VC,
	GRP_MFD1_VC, GRP_MFD4_VC, GRP_MFD3_VC, GRP_MFD_AFT_VC, GRP_MFD2_VC, GRP_MFD5_VC,
	GRP_MFD11_VC
	};

// ==========================================================
// I-Loaded values
// ==========================================================


typedef struct {
	HINSTANCE hDLL;
	SURFHANDLE pbi_lights;
	SURFHANDLE tkbk_label;
	SURFHANDLE clock_digits;
	SURFHANDLE digits_7seg;
	SURFHANDLE odslights;
	SURFHANDLE ssme_lights;
	SURFHANDLE a8_lights;
	HBITMAP deu_characters;
	HBITMAP deu_characters_overbright;
	HBITMAP deu_characters_fault;
	HDC DeuCharBitmapDC;
	HDC DeuCharOvrBrgtBitmapDC;
	HDC DeuCharFaultBitmapDC;
	SURFHANDLE deu_charactersSH;
	SURFHANDLE deu_characters_overbrightSH;
	SURFHANDLE deu_characters_faultSH;
} GDIParams;


typedef struct {
	double leftElevon, rightElevon;
	double bodyFlap;
	double speedbrake;
	double rudder;
} AerosurfacePositions;


class AtlantisSubsystemDirector;
class OMSSubsystem;
class AirDataProbeSystem;
class RMSSystem;
class StbdMPMSystem;
class ASE_IUS;
class CISS;
class DragChute;
class ActiveLatchGroup;
class MCA;
class MechActuator;
class PayloadBay;

class Atlantis_Tank;
class Atlantis_SRB;


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

using discsignals::DiscreteBundleManager;
using dps::ShuttleBusManager;
using dps::MDM;

//======================================
// RCS table indices for each RCS module
// ordered as in ODB
//======================================
//Forward RCS
const int RCS_F2F = 0;
const int RCS_F3F = 1;
const int RCS_F1F = 2;
const int RCS_F1L = 3;
const int RCS_F3L = 4;
const int RCS_F2R = 5;
const int RCS_F4R = 6;
const int RCS_F2U = 7;
const int RCS_F3U = 8;
const int RCS_F1U = 9;
const int RCS_F2D = 10;
const int RCS_F1D = 11;
const int RCS_F4D = 12;
const int RCS_F3D = 13;
const int RCS_F5R = 14;
const int RCS_F5L = 15;

//Left RCS
const int RCS_L3A = 0;
const int RCS_L1A = 1;
const int RCS_L4L = 2;
const int RCS_L2L = 3;
const int RCS_L3L = 4;
const int RCS_L1L = 5;
const int RCS_L4U = 6;
const int RCS_L2U = 7;
const int RCS_L1U = 8;
const int RCS_L4D = 9;
const int RCS_L2D = 10;
const int RCS_L3D = 11;
const int RCS_L5D = 12;
const int RCS_L5L = 13;
//Right RCS
const int RCS_R3A = 0;
const int RCS_R1A = 1;
const int RCS_R4R = 2;
const int RCS_R2R = 3;
const int RCS_R3R = 4;
const int RCS_R1R= 5;
const int RCS_R4U = 6;
const int RCS_R2U = 7;
const int RCS_R1U = 8;
const int RCS_R4D = 9;
const int RCS_R2D = 10;
const int RCS_R3D = 11;
const int RCS_R5D = 12;
const int RCS_R5R = 13;

// ==========================================================
// Interface for derived vessel class: Atlantis
// ==========================================================

class Atlantis: public VESSEL4 {
	friend class vc::_7SegDisp_RCSOMS_PRPLT_QTY;
	friend class Keyboard;
	friend class CRT;
	friend class vc::MDU;
	friend class dps::IDP;
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
	dps::SSME_SOP* pSSME_SOP;
	dps::RSLS_old* pRSLS;
	dps::ATVC_SOP* pATVC_SOP;
	mps::HeSysEng* pHeEng[3];
	mps::HeSysPneu* pHePneu;
	mps::MPS* pMPS;
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
	gnc::ATVC* pATVC;

	dps::SimpleGPCSystem *pSimpleGPC;

	MechActuator* pSTYDoorMotor;
	MechActuator* pSTZDoorMotor;
	/**
	 * Strategy pattern for the external airlock subsystem
	 */
	eva_docking::BasicExternalAirlock* pExtAirlock;

	eva_docking::TunnelAdapterAssembly* pTAA;
	
	AirDataProbeSystem* pADPS;
	RMSSystem* pRMS;
	StbdMPMSystem* pMPMs;

	ASE_IUS* pASE_IUS;

	CISS* pCISS;
	PayloadBay* pPayloadBay;

	comm::DeployedAssembly* pDeployedAssembly;

	DragChute* pDragChute;

	AnimState::Action spdb_status;
	int ___iCurrentManifold;

	// Actual Virtual Cockpit Mode
	int VCMode;
	int scnVCMode; // VC view loaded from scenario
	/**
	 * Structural configuration
	 * - 0 launch configuration
	 * - 1 SRB's engaged
	 * - 2 SRB's separated
	 * - 3 Tank separated/orbiter only
	 */
	int status;

	double t0;          // reference time: designated liftoff time
	double met;

	/* **************************************************************
	 * Mesh indices for use in objects  
	 ****************************************************************/

	UINT mesh_orbiter;                         // index for orbiter mesh
	UINT mesh_cockpit;                         // index for cockpit mesh for external view
	UINT mesh_vc;                              // index for virtual cockpit mesh
	UINT mesh_middeck;                         // index for mid deck mesh
	UINT mesh_kuband;						   // index for KU band antenna mesh
	UINT mesh_SILTS;
	UINT mesh_cargo_static;					   // index for static cargo mesh
	UINT mesh_heatshield;					   //index for heat shield mesh

	//**********************************************************
	//* public methods
	//**********************************************************
	Atlantis (OBJHANDLE hObj, int fmodel);
	~Atlantis();
	void AddOrbiterVisual();
	virtual DiscreteBundleManager* BundleManager() const;
	virtual dps::ShuttleBusManager* BusManager() const;
	mission::Mission* GetMissionData() const;
	// Overloaded callback functions
	void clbkAnimate (double simt);
	int  clbkConsumeBufferedKey (DWORD key, bool down, char *kstate);
	bool clbkDrawHUD (int mode, const HUDPAINTSPEC *hps, oapi::Sketchpad *skp);
	void clbkFocusChanged (bool getfocus, OBJHANDLE hNewVessel, OBJHANDLE hOldVessel);
	virtual int clbkGeneric(int msgid = 0, int prm = 0, void *context = 0);
	bool clbkLoadGenericCockpit ();
	void clbkLoadStateEx (FILEHANDLE scn, void *vs);
	bool clbkLoadVC (int id);
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
	virtual short GetETPropellant() const;
	virtual double GetETPropellant_B( void ) const;
	virtual double GetETLOXUllagePressure( void ) const;
	virtual double GetETLH2UllagePressure( void ) const;
	virtual unsigned short GetGPCMET(unsigned short usGPCID, unsigned short &usDay, unsigned short &usHour, unsigned short& usMin, unsigned short &usSec);
	virtual double GetMET() const;
	virtual short GetGPCRefHDot(unsigned short usGPCID, double& fRefHDot);
	virtual unsigned short GetGPCLVLHVel(unsigned short usGPCID, VECTOR3& vel);
	virtual dps::IDP* GetIDP(unsigned short usIDPNumber) const;
	virtual bool GetLiftOffFlag() const;
	virtual vc::MDU* GetMDU(unsigned short usMDUID) const;
	virtual const VECTOR3& GetOrbiterCoGOffset() const;
	virtual short GetSRBChamberPressure(unsigned short which_srb);
	virtual unsigned int GetGPCMajorMode() const;
	virtual double GetTgtSpeedbrakePosition() const;
	virtual double GetActSpeedbrakePosition() const;
	virtual double GetKEAS() const;
	virtual AnimState::Action GetGearState() const;
	int GetSoundID() const;
	double GetThrusterGroupMaxThrust(THGROUP_HANDLE thg) const;
	double GetPropellantLevel(PROPELLANT_HANDLE ph) const;
	void OperateSpeedbrake (AnimState::Action action);
	virtual bool RegisterMDU(unsigned short usMDUID, vc::MDU* pMDU);
	/* ***************************************************************
	 * Setters
	 *****************************************************************/
	virtual void SetAirDataProbeDeployment(int side, double position);
	void SetBayDoorLatchPosition (int gang, double pos);
	void SetBayDoorPosition (double pos);
	void SetETUmbDoorPosition(double pos, int door);
	void SetKuAntennaDAPosition (double pos);
	virtual void SetKuGimbalAngles(double fAlpha, double fbeta);
	void SetLaunchConfiguration (void);
	void SetOrbiterConfiguration (void);
	void SetOrbiterTankConfiguration (void);
	void SetPostLaunchConfiguration (double srbtime);
	void SetRadiatorPosition (double pos, int side);
	void SetSpeedbrake (double tgt);
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
	 * Sets SSME thrust direction.
	 * Function limits gimbal angles to correct range (10.5 degrees pitch, 8.5 degrees yaw)
	 * Null direction passes through stack c.g.
	 * @paramusMPSNo numerical ID of the SSME
	 * @param degPitch pitch gimbal angle (degrees)
	 * @param degYaw yaw gimbal angle (degrees)
	 * @return false for failure, true for success
	 */
	bool SetSSMEGimbalAngles(unsigned usMPSNo, double degPitch, double degYaw);

	/** 
	 *	@param fThrustLevel Thrust level (in %) from 0.0 to 109.0 (max power level)
	 */
	bool SetSSMEThrustLevel(unsigned short usMPSNo, double fThrustLevel);

	/**
	 * @param usMPSNo numerical ID of the SSME
	 * @return 0 < SSME PC < 109.0
	 */
	double GetSSMEThrustLevel( unsigned short usMPSNo );

	/**
	 * Sets SRB gimbal direction based on specified gimbal angles.
	 * Function limits angles to +/- 5 degrees
	 */
	void SetSRBGimbalAngles(SIDE SRB, double degPitch, double degYaw);

	/**
	 * Allows control of the following MPS vents:
	 * 0 = SSME-1
	 * 1 = SSME-2
	 * 2 = SSME-3
	 * 3 = LH2 B/U
	 * 4 = LH2 F/D
	 * 5 = LOX F/D
	 * 6 = LH2 FDLN Relief
	 * 7 = LOX FDLN Relief
	 * @param[in]	vent	identification of vent to control
	 * @param[in]	level	level of vent (between 0 and 1)
	 */
	void SetMPSDumpLevel( int vent, double level );

	/**
	 * Allows control of the SSME GH2 burning effect at ignition and shutdown.
	 * @param[in]	eng	number of SSME
	 * @param[in]	burn	controls if burn effect is on or off
	 */
	void SetSSMEGH2burn( int eng, bool burn );

	double CalcNetSSMEThrust() const;
	double GetSSMEISP() const;
	void CalcSSMEThrustAngles(int eng, double& degAngleP, double& degAngleY) const;

	//Thruster Control; called from OrbitDAP class
	void EnableThrusters(const int Thrusters[], int nThrusters);
	void DisableThrusters(const int Thrusters[], int nThrusters);

	/**
	 * Calls VESSEL::AttachChild and adds mass of child to shuttle mass
	 * Should always be called instead of AttachChild.
	 */
	bool AttachChildAndUpdateMass(OBJHANDLE child, ATTACHMENTHANDLE attachment, ATTACHMENTHANDLE child_attachment);
	/**
	 * Calls VESSEL::DetachChild and subtracts mass of child from shuttle mass
	 * Should always be called instead of DetachChild.
	 */
	bool DetachChildAndUpdateMass(ATTACHMENTHANDLE attachment, double vel = 0.0);

	bool AreMCADebugMessagesEnabled() const throw();

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

	void SeparateBoosters (double srb_time);
	void SeparateTank (void);

	/**
	 * Return true if ET is attached to shuttle
	 */
	bool HasTank() const;

	//Communication with LCC
	virtual void SynchronizeCountdown(double launch_mjd);
	virtual void StartRSLSSequence();
	virtual bool GetRSLSAbortFlag() const;

	virtual bool HydraulicsOK();

	virtual void PSN4( void );
	virtual void SetSSMEActPos( int num, double Ppos, double Ypos );

	virtual int GetSSMEPress( int eng );
	virtual int GetHeTankPress( int sys ) const;
	virtual int GetHeRegPress( int sys ) const;
	virtual void HeFillTank( int sys, double mass );
	virtual PROPELLANT_HANDLE GetLH2Tank( void ) const;
	virtual PROPELLANT_HANDLE GetLOXTank( void ) const;
	virtual double GetLOXManifPress( void ) const;
	virtual double GetLH2ManifPress( void ) const;

	void ToggleGrapple (void);
	void UpdateMesh ();
	void UpdateSSMEGimbalAnimations();

	//**********************************************************
	// RCS Thruster interface functions
	//**********************************************************
	
	/**
	 * Pointer to the A7A8 custom panel region
	 */
	vc::AtlantisPanel* pA7A8Panel;
	/**
	 * Pointer to the A8 (RMS) panel region
	 */
	vc::AtlantisPanel* pPanelA8;

	vc::AtlantisPanel* pPanelL10;
	vc::AtlantisPanel* pPanelL12U_IUS;
	vc::AtlantisPanel* pPanelL12U_Centaur;


	AerosurfacePositions aerosurfaces;
	double spdb_proc, spdb_tgt; // Speedbrake deployment state (0=fully closed, 1=fully open)
	double ldoor_drag, rdoor_drag; // drag components from open cargo doors
	bool do_cargostatic;
	VECTOR3 orbiter_ofs;
	VECTOR3 cargo_static_ofs;
	VISHANDLE vis;      // handle for visual - note: we assume that only one visual per object is created!
	MESHHANDLE hOrbiterMesh, hOrbiterCockpitMesh, hOrbiterVCMesh, 
		hMidDeckMesh; // mesh handles
	MESHHANDLE hKUBandMesh;
	MESHHANDLE hSILTSMesh;
	MESHHANDLE hHeatShieldMesh;
	DEVMESHHANDLE hDevHeatShieldMesh;
	DEVMESHHANDLE hDevOrbiterMesh;

	SURFHANDLE hOVTexture;

	char cargo_static_mesh_name[256];

	//C-P attachments
	ATTACHMENTHANDLE ahHDP;
	ATTACHMENTHANDLE ahTow;
	//P-C attachments
	//ATTACHMENTHANDLE ahCenterActive[3];
	ATTACHMENTHANDLE ahCenterPassive[4];
	ATTACHMENTHANDLE ahStbdPL[4];
	ATTACHMENTHANDLE ahPortPL[4];
	ATTACHMENTHANDLE ahET, ahLeftSRB, ahRightSRB;
	
	AtlantisSubsystemDirector* psubsystems;
	
	vc::PanelR2 *panelr2; // temporary

	/**
	 * Bridge function between MPS and ET to "deliver" GO2 and GH2 for
	 * tank pressurization. Note that no actual mass is sent from the
	 * MPS to the ET, but the mass is directly decreased from the
	 * propellant in the ET.
	 * @param[in]	GOXmass	mass of gaseous oxygen (in kg)
	 * @param[in]	GH2mass mass of gaseous hydrogen (in kg)
	 */
	void ETPressurization( double GOXmass, double GH2mass );

	/**
	 * Function called by MPS to "flow" mass from the ET to the MPS manifold.
	 */
	void UpdateMPSManifold( void );


private:
	int mfdID;

	double slag1, slag2, slag3;
	PSTREAM_HANDLE pshSlag1[2], pshSlag2[2], pshSlag3[2];

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
	vc::PanelGroup<Atlantis> pgOverheadAft;
	vc::PanelGroup<Atlantis> pgAftStbd;
	vc::PanelGroup<Atlantis> pgAft;
	vc::PanelGroup<Atlantis> pgAftPort;

	/**
	 * all animations that need to be deleted in destructor
	 */
	std::vector<MGROUP_TRANSFORM*> vpAnimations;

	std::vector<ActiveLatchGroup*> pActiveLatches;

	void DetachSRB(SIDE side, double thrust, double prop);
	void loadMDMConfiguration(void);
	/**
	 * Copies settings (thrust & ISP) from one thruster to another
	 * \param th Thruster to change settings on
	 * \param v Vessel containing th_ref
	 * \param th_ref Thruster to copy settings from
	 */
	void CopyThrusterSettings(THRUSTER_HANDLE th, const VESSEL* v, THRUSTER_HANDLE th_ref);
	
	void OMSEngControl(unsigned short usEng);

	void StopAllManifolds();
	void FireAllNextManifold();

	
	//Helper functions for RCS creation
	//
	void AddPrimaryRCSExhaust(THRUSTER_HANDLE thX);
	void AddVernierRCSExhaust(THRUSTER_HANDLE thX);
	void AddRCSExhaust(THRUSTER_HANDLE thX, const VECTOR3& pos, const VECTOR3& dir);
	void AddVRCSExhaust(THRUSTER_HANDLE thX, const VECTOR3& pos, const VECTOR3& dir);


	inline void CreateOrRedefineRCSThruster(THRUSTER_HANDLE *thX, const VECTOR3& pos, const VECTOR3& dir, double vacThrust, PROPELLANT_HANDLE phY, double isp0, double ispsl)
	{
		if (*thX == NULL)
		{
			*thX = CreateThruster(pos, dir, vacThrust, phY, isp0, ispsl);
		}
		else
		{
			SetThrusterRef(*thX, pos);
		}
		if (vacThrust < 50.0 * LBF)
		{
			AddVernierRCSExhaust(*thX);
		}
		else
		{
			AddPrimaryRCSExhaust(*thX);
		}
	}
	//Functions for creating real RCS
	void CreateRightARCS(const VECTOR3& ref_pos);
	void CreateLeftARCS(const VECTOR3& ref_pos);
	void CreateFRCS(const VECTOR3& ref_pos);
	//-------------------------------------------------
	void CreateSSMEs(const VECTOR3& ofs);
	void CreateMPSGOXVents(const VECTOR3& ref_pos);
	void CreateMPSDumpVents( void );
	bool bUseRealRCS;
	void CreateOrbiterTanks();
	bool bCommMode;
	void DefineSSMEExhaust();
	//-----------------------------------
	void ShowMidDeck();
	void HideMidDeck();
	int Lua_InitInterpreter (void *context);
	int Lua_InitInstance (void *context);
	//-----------------------------------
	void DefineKUBandAnimations();
	void CreateAttControls_RCS(VECTOR3 center);

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

	/**
	 * Calculates direction in which SSMEs/SRBs must thrust to point through CoG
	 */
	void UpdateNullDirections();

	//Landing Gear
	void DeployLandingGear();
	void RetractLandingGear();
	void ArmGear();
	void DefineTouchdownPoints();
	bool GearArmed() const;

	void RealizeSubsystemConnections();

	void CreateETAndSRBAttachments(const VECTOR3 &ofs);

	Atlantis_Tank* GetTankInterface() const;
	Atlantis_SRB* GetSRBInterface(SIDE side) const;
	ISSUMLP* GetMLPInterface() const;

	double GetMassAndCoGOfAttachedObject(ATTACHMENTHANDLE ah, VECTOR3& CoG) const;
	/**
	 * Updates shuttle CoG.
	 * Estimates center of gravity relative to center of Orbiter mesh, then calls ShiftCG to update CG.
	 */
	void UpdateMassAndCoG(bool bUpdateAttachedVessels = false);

	void Twang(double timeToLaunch) const;

	//OMS
	/**
	 * @param engine 0 for Left, 1 for Right
	 * @param pitch angle in degrees (relative to null position)
	 * @param yaw angle in degrees (relative to null position)
	 * @return false if gimbal values are out of range, true otherwise
	 */
	void GimbalOMS(int engine, double pitch, double yaw);
	void OMSTVC(const VECTOR3 &Rates, double SimDT);

	void UpdateTranslationForces();

	void UpdateOrbiterTexture(const std::string& strTextureName);

	// *******************************************************************************
	// Animations
	// *******************************************************************************
	UINT anim_door;                            // handle for cargo door animation
	UINT anim_rad[2];                             // handle for radiator animation
	UINT anim_clatch[4];					   // handle for center line latch gangs

	UINT anim_kubd;                            // handle for Ku-band antenna animation
	UINT anim_lelevon;                         // handle for left elevator animation
	UINT anim_relevon;                         // handle for right elevator animation
	UINT anim_bf;                              // handle for body flap animation
	UINT anim_rudder;						   // handle for rudder animation
	UINT anim_spdb;                            // handle for speed brake animation
	UINT anim_letumbdoor;					   // handle for left ET umbilical door animation
	UINT anim_retumbdoor;					   // handle for right ET umbilical door animation
	UINT anim_gear;                            // handle for landing gear animation
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
	
	typedef enum {CAM_A=0, CAM_B=1, CAM_C=2, CAM_D=3} PLB_CAM;
	double camYaw[4], camPitch[4];
	VECTOR3 plbdCamPos[4];
	
	// Selected camera must be moved at low rate (if false at high rate)
	bool cameraLowRate;

	// If a camera have been moved (used for reposition camera animations)
	bool cameraMoved;
	
	// Selected camera for control
	int cameraControl;  // 0:FL 1:FR 2:BL 3:BR 4:RMS Elbow
	
	// Sets the camera positions and animations.
	void SetAnimationCameras();

	//-----------------------------------------------
	//PLB Lights
	//-----------------------------------------------
	/**
	 * Defines payload bay light (LightEmitter and associated beacon)
	 * \param pos position of light
	 * \param dir direction of beam
	 * \param degWidth angular width of umbra (in degrees); penumbra is 1.1 times specified width
	 * \param bspec Beacon spec to be initialized with data
	 * \returns LightEmitter pointer
	 */
	LightEmitter* AddPayloadBayLight(VECTOR3& pos, VECTOR3& dir, double degWidth, BEACONLIGHTSPEC& bspec);
	LightEmitter* PLBLight[6];
	LightEmitter* FwdBulkheadLight;
	LightEmitter* DockingLight[2];
	VECTOR3 PLBLightPosition[6];
	VECTOR3 FwdBulkheadLightPos, DockingLightPos;
	BEACONLIGHTSPEC PLB_bspec[6];
	BEACONLIGHTSPEC FwdBulkhead_bspec, Docking_bspec[2];
	
	LightEmitter* SRBLight[2];
	LightEmitter* SSMELight;

	//gear
	AnimState gear_status;
	bool gear_armed;
	
	PROPELLANT_HANDLE ph_oms, ph_mps, ph_srb, ph_frcs; // handles for propellant resources
	PROPELLANT_HANDLE ph_lrcs, ph_rrcs, ph_controller;
	THRUSTER_HANDLE th_main[3];                // handles for orbiter main (SSME) engines
	THRUSTER_HANDLE th_oms[2];               // handles for orbiter OMS engines
	THRUSTER_HANDLE th_srb[2];                 // handles for SRB engines
	THRUSTER_HANDLE th_att_rcs[18];            // 12 for rotation, 6 virtual
	THRUSTER_HANDLE th_att_lin[10];

	bool bSSMEsDefined, bOMSDefined, bRCSDefined, bControllerThrustersDefined;

	/**
	 * Particle stream handle for SSME GH2 burn effect at ignition and shutdown.
	 */
	PSTREAM_HANDLE SSMEGH2burn[3];

	/**
	 * Propellant handle for MPS LOX manifold. Used for MPS dump.
	 */
	PROPELLANT_HANDLE phLOXdump;

	/**
	 * Propellant handle for MPS LH2 manifold. Used for MPS dump.
	 */
	PROPELLANT_HANDLE phLH2dump;
	/**
	 * Used for the preflight GOX venting of the SSMEs.
	 */
	THRUSTER_HANDLE th_ssme_gox[3];
	/**
	 * Thruster handles for the various MPS dumps/vents. List follows:
	 * 0 = SSME-1
	 * 1 = SSME-2
	 * 2 = SSME-3
	 * 3 = LH2 B/U
	 * 4 = LH2 F/D
	 * 5 = LOX F/D
	 * 6 = LH2 FDLN Relief
	 * 7 = LOX FDLN Relief
	 */
	THRUSTER_HANDLE thMPSDump[8];

	//<<<< Begin new RCS model here
	//Array collecting all primary jets

	THRUSTER_HANDLE thFRCS[16];
	THRUSTER_HANDLE thLRCS[14];
	THRUSTER_HANDLE thRRCS[14];
	/** Forward Manifold 1
	 */
	//THRUSTER_HANDLE thManFRCS1[4];		
	///** Forward Manifold 2
	// */
	//THRUSTER_HANDLE thManFRCS2[4];
	///** Forward Manifold 3
	// */
	//THRUSTER_HANDLE thManFRCS3[4];		
	///** Forward Manifold 4
	// */
	//THRUSTER_HANDLE thManFRCS4[2];
	///** Forward Manifold 5
	// */
	//THRUSTER_HANDLE thManFRCS5[2];		

	///** Left Manifold 1
	// */
	//THRUSTER_HANDLE thManLRCS1[3];		
	///** Left Manifold 2
	// */
	//THRUSTER_HANDLE thManLRCS2[3];
	///** Left Manifold 3
	// */
	//THRUSTER_HANDLE thManLRCS3[3];		
	///** Left Manifold 4
	// */
	//THRUSTER_HANDLE thManLRCS4[3];
	///** Left Manifold 5
	// */
	//THRUSTER_HANDLE thManLRCS5[2];

	///** Right Manifold 1
	// */
	//THRUSTER_HANDLE thManRRCS1[3];		
	///** Right Manifold 2
	// */
	//THRUSTER_HANDLE thManRRCS2[3];
	///** Right Manifold 3
	// */
	//THRUSTER_HANDLE thManRRCS3[3];		
	///** Right Manifold 4
	// */
	//THRUSTER_HANDLE thManRRCS4[3];
	///** Right Manifold 5
	// */
	//THRUSTER_HANDLE thManRRCS5[2];
	//>>>> End of new RCS model
	THGROUP_HANDLE thg_pitchup, thg_pitchdown, thg_yawleft, thg_yawright, thg_rollleft, thg_rollright;
	THGROUP_HANDLE thg_transfwd, thg_transaft, thg_transup, thg_transdown, thg_transright, thg_transleft;
	VECTOR3 TransForce[2]; //force provided by translation groups; 0=plus-axis
	UINT ex_main[3];						   // main engine exhaust
	std::vector<UINT> vExRCS;				   // RCS exhaust
	std::vector<PSTREAM_HANDLE> vExStreamRCS;  // RCS exhaust stream
	PARTICLESTREAMSPEC RCS_PSSpec;
	SURFHANDLE RCS_Exhaust_tex;
	THGROUP_HANDLE thg_main, thg_srb, thg_retro;          // handles for thruster groups
	CTRLSURFHANDLE hrudder, hlaileron, hraileron, helevator, hbodyflap;
	AIRFOILHANDLE hStackAirfoil;
	bool ControlSurfacesEnabled;

	//CLR
	VECTOR3 SSMET_GOX_REF1;
	VECTOR3 SSMEL_GOX_REF1;
	VECTOR3 SSMER_GOX_REF1;

	/**
	 * Mass of LOX in MPS manifold.
	 */
	double LOXmass;

	/**
	 * Mass of LH2 in MPS manifold.
	 */
	double LH2mass;

	// Helium Tanks
	PROPELLANT_HANDLE oms_helium_tank[2];
	int Hydraulic_Press[3];

	bool bSSMEGOXVent;

	bool RMS, STBDMPM;

	bool ControlRMS;
	//Hand controller input
	VECTOR3 THCInput, RHCInput;
	VECTOR3 AltKybdInput; // uses arrow,PgUp/PgDn keys to provide translation inputs; axes correspond to RCS FWD SENSE
	//Thruster commands
	VECTOR3 TranslationCommand, RotationCommand;

	VCHUDSPEC huds;
	double mfdbright[11];
	double pl_mass;

	VECTOR3 currentCoG; // 0,0,0 corresponds to CoG at center of Orbiter mesh
	VECTOR3 payloadCoG;
	double payloadMass;

	bool hasCISS;// if true, bay 13 covers are hidden

	//base vectors;
	VECTOR3 LVLH_X, LVLH_Y, LVLH_Z;

	bool bEnableMCADebug;

	/**
	 * Thrust direction of each SSME when at the "installed position" (0º pitch
	 * and yaw in the engine referencial).
	 */
	VECTOR3 SSMEInstalledNullPos[3];

	/**
	 * Current thrust direction of SSMEs.
	 */
	VECTOR3 SSMECurrentPos[3];
	VECTOR3 SRBNullDirection[3];


	bool firstStep; //call functions in first timestep

	DiscreteBundleManager* bundleManager;
	dps::ShuttleBusManager* busManager;
	
	vc::MDU* mdus[11];

	bool SERCstop;

	//MNVR
	double curOMSPitch[2], curOMSYaw[2];

	double fTimeCameraLabel;
	NOTEHANDLE nhCameraLabel;
	char pszCameraLabelBuffer[80];

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
	DiscOutPort SpdbkThrotCDROut, SpdbkThrotPLTOut;
	DiscInPort BodyFlapAutoIn;
	DiscOutPort BodyFlapAutoOut, BodyFlapManOut;
	DiscInPort AftSense, AftFltCntlrPwr, CdrFltCntlrPwr, PltFltCntlrPwr;

	DiscOutPort SpdbkThrotPort;
	DiscOutPort RHCInputPort[3], THCInputPort[3];
	DiscInPort RotThrusterCommands[4], TransThrusterCommands[3];
	//DiscInPort LeftElevonCommand, RightElevonCommand;
	DiscInPort ElevonCommand, AileronCommand, RudderCommand;

	// Pan/Tilt PLBD cameras and RMS elbow cam
	// 0=A, 1=B, 2=C, 3=D, 4=RMS Elbow
	DiscInPort PLBDCamPanLeft[5], PLBDCamPanRight[5], PLBDCamTiltUp[5], PLBDCamTiltDown[5];
	DiscInPort PTULowSpeed[5];
	DiscOutPort PLBDCamPanLeft_Out[5], PLBDCamPanRight_Out[5];
	DiscOutPort PLBDCamTiltUp_Out[5], PLBDCamTiltDown_Out[5];

	DiscInPort PLBDLightPower[6];
	DiscInPort FwdBulkheadLightPower, DockingLightBright, DockingLightDim;

	DiscOutPort RMSGrapple, RMSRelease; // used by RMS dialog
	DiscOutPort RMS_RHCInput[3], RMS_THCInput[3], RMSDrivePlus, RMSDriveMinus;
	DiscInPort RMSSpeedIn;
	DiscOutPort RMSSpeedOut;
	DiscInPort OMSArm[2], OMSArmPress[2], OMSFire[2], OMSPitch[2], OMSYaw[2];

	DiscOutPort LandingGearPosition[6];
	DiscOutPort LandingGearArmDeployLT[2];
	DiscInPort LandingGearArmDeployPB[4];

	DiscOutPort SSMEPBAnalog[3]; // to allow MECO to be commanded from keyboard

	/**
	 * LOX low level sensors.
	 */
	Sensor LO2LowLevelSensor[4];

	void AddKUBandVisual(const VECTOR3 ofs);
	void DisplayCameraLabel(const char* pszLabel);
};

VECTOR3 CalcOMSThrustDir(unsigned int side, double pitch, double yaw);
/**
* Calculates lift, drag and moment coefficients for given mach, AOA and aerosurface positions
*/
void GetShuttleVerticalAeroCoefficients(double mach, double degAOA, double altitude, const AerosurfacePositions* aerosurfaces, double* cl, double* cm, double* cd);



#if defined(IS_SSU_DLL_MODULE)
DLLCLBK mission::Mission* ssuGetDefaultMission();
DLLCLBK mission::Mission* ssuGetMission(const std::string& filename);
void ClearMissionManagementMemory();
void InitMissionManagementMemory();
#endif

#endif // !__ATLANTIS_H


