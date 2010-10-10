// ==============================================================
//                 ORBITER MODULE: Atlantis
//                  Part of the ORBITER SDK
//          Copyright (C) 2001-2003 Martin Schweiger
//                   All rights reserved
//
// Atlantis_SRB.cpp
// Reference implementation of Atlantis SRB(Space Shuttle - Solid
// Rocket Booster) vessel class module
// Note: Druing launch, the thrusters associated with this vessel
// do not provide any thrust; similar resources are created in
// the Atlantis module. This module takes control of the SRB after
// separation from the Shuttle's main tank.
// ==============================================================

#define ORBITER_MODULE
//#include "Atlantis.h"
#include "Atlantis_SRB.h"
#include "CommonDefs.h"
#include "math.h"
#include "stdio.h"

// ==============================================================
// Specialised vessel class Atlantis_SRB
// ==============================================================

// Constructor
Atlantis_SRB::Atlantis_SRB (OBJHANDLE hObj)
: VESSEL3(hObj), bMainEngine(false), bSeparationEngine(false), usSectionCount(SRB_SECTION_COUNT)
{
	// preload mesh
	//mesh_idx = MESH_UNDEFINED;
	hSRBMesh_Left = oapiLoadMeshGlobal ("SSU/LSRB");
	hSRBMesh_Right = oapiLoadMeshGlobal ("SSU/RSRB");
	for(unsigned short i=0;i<SRB_SECTION_COUNT;i++) {
		//hSRMSegmentMesh_Left[i] = NULL;
		//hSRMSegmentMesh_Right[i] = NULL;
		hSRMSegmentMesh[i] = NULL;
	}

	hVis = NULL;

	slag1 = 0.0;
	slag2 = 0.0;
	slag3 = 0.0;
}

void Atlantis_SRB::SetPostSeparationState(double launch_time, double thrust_level, double prop_level)
{
	t0 = launch_time;
	SetPropellantMass(ph_main, prop_level*SRB_MAX_PROPELLANT_MASS);
	SetThrusterLevel(th_main, thrust_level);

	bMainEngine = true;
	bSeparationEngine = true;
	srb_separation_time = oapiGetSimTime();
}

// ==============================================================
// Callback functions
// ==============================================================

void Atlantis_SRB::clbkVisualCreated(VISHANDLE vis, int refcount)
{
	hVis = vis;
}

void Atlantis_SRB::clbkVisualDestroyed(VISHANDLE vis, int refcount)
{
	if(vis == hVis) hVis = NULL;
}

// Set SRB class specs
void Atlantis_SRB::clbkSetClassCaps (FILEHANDLE cfg)
{
	bool Left;
	oapiReadItem_bool(cfg, "Left", Left);


	if(Left) 
	{
		hSRBMesh = hSRBMesh_Left;
	//	oapiWriteLog("Create Left SRB\n");
		hSRMSegmentMesh[0] = oapiLoadMeshGlobal("SSU/STS1_LSRB_aft_segment");
		hSRMSegmentMesh[1] = oapiLoadMeshGlobal("SSU/STS1_LSRB_aft_center_segment");
		hSRMSegmentMesh[2] = oapiLoadMeshGlobal("SSU/STS1_LSRB_fwd_center_segment");
		hSRMSegmentMesh[3] = oapiLoadMeshGlobal("SSU/STS1_LSRB_fwd_segment");
		hSRMSegmentMesh[4] = oapiLoadMeshGlobal("SSU/STS1_LSRB_fwd_skirt_assy");
	}
	else 
	{
		hSRBMesh = hSRBMesh_Right;
	//	oapiWriteLog("Create Right SRB\n");
		hSRMSegmentMesh[0] = oapiLoadMeshGlobal("SSU/STS1_RSRB_aft_segment");
		hSRMSegmentMesh[1] = oapiLoadMeshGlobal("SSU/STS1_RSRB_aft_center_segment");
		hSRMSegmentMesh[2] = oapiLoadMeshGlobal("SSU/STS1_RSRB_fwd_center_segment");
		hSRMSegmentMesh[3] = oapiLoadMeshGlobal("SSU/STS1_RSRB_fwd_segment");
		hSRMSegmentMesh[4] = oapiLoadMeshGlobal("SSU/STS1_RSRB_fwd_skirt_assy");
	}

	extern PARTICLESTREAMSPEC srb_contrail, srb_exhaust, srb_slag1, srb_slag2, srb_slag3;
	extern PARTICLESTREAMSPEC bsm_exhaust, bsm_contrail, bsm_residual;
	/*PARTICLESTREAMSPEC srb_bolt = {
		0, 8.0, 20, 0.0, 0.1, 0.3, 16, 3.0, PARTICLESTREAMSPEC::EMISSIVE,
		PARTICLESTREAMSPEC::LVL_LIN, 0, 1,
		PARTICLESTREAMSPEC::ATM_FLAT, 1, 1
	};*/

	

	SetEnableFocus (false);
	// SRB cannot receive input focus

	// *********************** physical parameters *********************************

	SetSize (23.0);
	SetEmptyMass (SRB_EMPTY_MASS);
	SetCW (0.1, 0.3, 1.4, 1.4);
	SetCrossSections (_V(167.42,172.64,22.92));
	SetRotDrag (_V(0.7,0.7,0.1));
	SetPMI (_V(128.72,128.64,2.31));
	//SetGravityGradientDamping (10.0);
	SetTouchdownPoints (_V(0,-2,-21.1), _V(-2,1,-21.1), _V(2,1,-21.1));
	SetLiftCoeffFunc (0);

	ahToOrbiter = CreateAttachment(true, _V(0, 0, 0), _V(1, 0, 0), _V(0, 0, 1), "SSU_SRB");

	// ************************* propellant specs **********************************

	ph_main = CreatePropellantResource (SRB_MAX_PROPELLANT_MASS);

	//75 pounds of fuel unburned

	phBSM = CreatePropellantResource(BSM_MAX_PROPELLANT_MASS * 8.0);
	// Note that the SRB instance is only created after separation from
	// the main assembly, so the actual fuel mass will always be
	// much smaller

	// *********************** thruster definitions ********************************

	// main engine
	th_main = CreateThruster (_V(0.484,0.434,-21), _V(0,0,1), SRB_THRUST, ph_main, SRB_ISP0, SRB_ISP1);
	SURFHANDLE tex = oapiRegisterExhaustTexture ("Exhaust2");
	bsm_exhaust.tex = srb_exhaust.tex = oapiRegisterParticleTexture ("SSU\\SRB_exhaust");
	
	AddExhaust (th_main, 16.0, 2.0, tex);
	AddExhaustStream (th_main, _V(0.484,0.434,-25), &srb_exhaust);
	AddExhaustStream (th_main, _V(0.484,0.434,-100), &srb_contrail);

	// separation bolts
	//th_bolt = CreateThruster (_V(0,0,1.3), _V(1,0,0), 3e6, ph_main, 1e7);

	//BSM
	if(Left)
	{
		thBSM[0] = CreateThruster(_V(0.752, 3.15, -19.5), _V(-0.219, -0.604, 0.765), 4*BSM_THRUST0, phBSM, BSM_ISP0);
		thBSM[1] = CreateThruster(_V(0.445, 1.22, 21), _V(-0.262, -0.719, 0.642), 4*BSM_THRUST0, phBSM, BSM_ISP0);
	} else 
	{
		thBSM[0] = CreateThruster(_V(-0.752, 2.06, -20.5), _V(0.219, -0.604, 0.765), 4*BSM_THRUST0, phBSM, BSM_ISP0);
		thBSM[1] = CreateThruster(_V(-0.445, 1.22, 21), _V(0.262, -0.719, 0.642), 4*BSM_THRUST0, phBSM, BSM_ISP0);
	}
	// for simplicity, the separation bolts directly use SRB propellant. We give
	// them an insanely high ISP to avoid significant propellant drainage

	//AddExhaust (th_bolt, 0.7, 0.1, _V(-2.1,0,-8), _V(-1,0,0));
	//AddExhaust (th_bolt, 0.7, 0.1, _V(-2.1,0,11), _V(-1,0,0));

	AddExhaustStream (thBSM[0], &bsm_exhaust);
	AddExhaustStream (thBSM[1], &bsm_exhaust);
	//AddExhaustStream (thBSM[0], &bsm_contrail);
	//AddExhaustStream (thBSM[1], &bsm_contrail);

	//Add slag effect streams
	AddParticleStream(&srb_slag1, _V(0,0,-22), SLAG_DIR, &slag1);
	AddParticleStream(&srb_slag2, _V(0,0,-22), SLAG_DIR, &slag2);
	AddParticleStream(&srb_slag3, _V(0,0,-22), SLAG_DIR, &slag3);

	// ************************ visual parameters **********************************

	//mesh_idx = AddMesh (hSRBMesh);
}

void Atlantis_SRB::UpdateVisual() const
{
	ClearMeshes();

	if(usSectionCount == SRB_SECTION_COUNT) AddMesh(hSRBMesh);
	else {
		for(int i=0;i<usSectionCount;i++) {
			VECTOR3 ofs = _V(0.0, 0.0, SRB_SECTION_ZPOS[i]);
			AddMesh(hSRMSegmentMesh[i], &ofs);
		}
	}
}

// Finish setup
void Atlantis_SRB::clbkPostCreation ()
{
	//SetRefTime ();	// reconstruct ignition time from fuel level
	SetPropellantMass(ph_main, 0.0);
	SetPropellantMass(phBSM, GetPropellantMaxMass(phBSM));

	// show meshes
	UpdateVisual();
}

// Simulation time step
void Atlantis_SRB::clbkPostStep (double simt, double simdt, double mjd)
{
	extern void GetSRB_State (double, double&, double&);
	//sprintf (oapiDebugString(), "SRB mass = %f", GetMass());
	if (bMainEngine) {
		double met = simt-t0;
		if (met >= NO_SLAG_TIME)
		{
			slag1 = 0.0;
			slag2 = 0.0;
			slag3 = 0.0;
		} else 
		{
			slag1 = 1.0 - met/NO_SLAG_TIME;
			slag2 = 1.0 - met/NO_SLAG_TIME;
			slag3 = 1.0 - met/NO_SLAG_TIME;
		}

		if (met >= SRB_CUTOUT_TIME) {
			SetThrusterLevel (th_main, 0);
			bMainEngine = false;
			// After the propellant is burnt out we should be airborne.
			// Now we can prepare touchdown points for "landing"
			SetTouchdownPoints (_V(0,9,3), _V(-1,1,-3), _V(1,1,-3));
		} else {
			double thrust_level, prop_level;
			GetSRB_State (met, thrust_level, prop_level);
			SetThrusterLevel (th_main, thrust_level);
		}
		if (bSeparationEngine) {
			static double bolt_t = 0.5;
			double srb_dt = simt - srb_separation_time;

			if(srb_dt < 1.2)
			{
				for(int i=0;i<2;i++) SetThrusterLevel(thBSM[i], 1.0-(srb_dt*0.583));
			} else {
				for(int i=0;i<2;i++) SetThrusterLevel(thBSM[i], 0.0);
			}
			/*
			if (srb_dt > bolt_t) {
				DelThruster (th_bolt);
				bSeparationEngine = false;
			} else {
				SetThrusterLevel (th_bolt, sqrt (1.0-srb_dt/bolt_t));
				
			}
			*/
		}
	}
	//increased altitude for self delete
	if (GetAltitude() < 250.0 && !GetAttachmentStatus(ahToOrbiter)) oapiDeleteVessel (GetHandle());
}

void Atlantis_SRB::clbkLoadStateEx(FILEHANDLE scn, void *status)
{
	char* line;

	while(oapiReadScenario_nextline(scn, line)) {
		if(!_strnicmp(line, "SECTION_COUNT", 13)) {
			int temp;
			sscanf_s(line+14, "%d", &temp);
			usSectionCount = static_cast<int>(temp);
		}
		else ParseScenarioLineEx(line, status);
	}
}

void Atlantis_SRB::clbkSaveState(FILEHANDLE scn)
{
	VESSEL3::clbkSaveState(scn);

	oapiWriteScenario_int(scn, "SECTION_COUNT", static_cast<int>(usSectionCount));
}

// ==============================================================
// API interface
// ==============================================================

// Initialisation
DLLCLBK VESSEL *ovcInit (OBJHANDLE hvessel, int flightmodel)
{
	return new Atlantis_SRB (hvessel);
}

// Cleanup
DLLCLBK void ovcExit (VESSEL *vessel)
{
	if (vessel) delete (Atlantis_SRB*)vessel;
}
