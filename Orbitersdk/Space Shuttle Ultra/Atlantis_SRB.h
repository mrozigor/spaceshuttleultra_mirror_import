#ifndef ATLANTIS_SRB_H
#define ATLANTIS_SRB_H
#pragma once

#include "orbitersdk.h"
#include <vector>

const double SRB_MAX_PROPELLANT_MASS = 502146.0;
const double FWC_SRB_MAX_PROPELLANT_MASS = 501440.015;
// SRB propellant mass [kg]
//total impulse is 1.32466E9 Ns

const double SRB_EMPTY_MASS = 87603.65;
const double FWC_SRB_EMPTY_MASS = 53103.8731;
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
const double FWC_SRB_THRUST = 14808129.8; // Vacuum SRB thrust per unit [N]

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

const double NO_SLAG_TIME = 240.0;

const double SLAG_SPREAD = 0.2;

const double SLAG1_RATE = 5;
const double SLAG2_RATE = 11.0;
const double SLAG3_RATE = 49.0;

const VECTOR3 SLAG_DIR = _V(0.0, 0.0, -1.0);

const unsigned short SRB_SECTION_COUNT = 5; // 4 fueled segments and FWD skirt

const double SRB_SECTION_ZPOS[SRB_SECTION_COUNT] = {
	-15.2,
	SRB_SECTION_ZPOS[0]+10.6,
	SRB_SECTION_ZPOS[1]+8.3,
	SRB_SECTION_ZPOS[2]+8.24,
	SRB_SECTION_ZPOS[3]+8.2,
};

// ==========================================================
// Interface for derived vessel class: Atlantis_SRB
// ==========================================================

class Atlantis_SRB: public VESSEL3 {
public:
	Atlantis_SRB (OBJHANDLE hObj);
	// Construct interface from existing object

	//void SetRefTime (void);
	/**
	 * @param launch_time simtime at which SRB ignition occured (seconds)
	 * @param thrust_level thrust (between 0 and 1) of booster
	 * @param prop_level amount of fuel remaining (between 0 and 1)
	 */
	virtual void SetPostSeparationState(double launch_time, double thrust_level, double prop_level);

	// Overloaded callback functions
	void clbkVisualCreated(VISHANDLE vis, int refcount);
	void clbkVisualDestroyed(VISHANDLE vis, int refcount);
	void clbkSetClassCaps (FILEHANDLE cfg);
	void clbkPostStep (double simt, double simdt, double mjd);
	void clbkPostCreation ();

	void clbkLoadStateEx(FILEHANDLE scn, void* status);
	void clbkSaveState(FILEHANDLE scn);

private:
	void UpdateVisual() const;

	MESHHANDLE hSRBMesh_Left;
	MESHHANDLE hSRBMesh_Right;
	MESHHANDLE hSRMSegmentMesh[SRB_SECTION_COUNT];
	//MESHHANDLE hSRMSegmentMesh_Right[SRB_SECTION_COUNT];
	//UINT mesh_idx;

	VISHANDLE hVis;

	unsigned short usSectionCount; // number of sections; if usSectionCount is 5, SRB is fully assembled

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
	ATTACHMENTHANDLE ahToOrbiter;
};

#endif