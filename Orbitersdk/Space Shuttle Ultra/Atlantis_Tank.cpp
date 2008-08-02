// ==============================================================
//                 ORBITER MODULE: Atlantis
//                  Part of the ORBITER SDK
//          Copyright (C) 2001-2003 Martin Schweiger
//                   All rights reserved
//
// Atlantis_Tank.cpp
// Reference implementation of Atlantis Tank vessel class module
// Note: This module takes control of the tank after separation
// from the orbiter.
// ==============================================================

#define ORBITER_MODULE
#include "Atlantis.h"
#include "math.h"

// ==============================================================
// Specialised vessel class Atlantis_Tank
// ==============================================================

// Constructor
Atlantis_Tank::Atlantis_Tank (OBJHANDLE hObj)
: VESSEL2(hObj)
{
	// preload mesh
	hTankMesh = oapiLoadMeshGlobal (DEFAULT_MESHNAME_ET);

	//////////////////////// ET vent ////////////////////////
	timer = -1;
	counter = 50;
	pos = 0;
	//////////////////////// ET vent ////////////////////////
}

// ==============================================================
// Callback functions
// ==============================================================

// Set Tank class specs
void Atlantis_Tank::clbkSetClassCaps (FILEHANDLE cfg)
{
	SetEnableFocus (false);
	// Tank cannot receive input focus

	SetSize (24.0);
	SetEmptyMass (TANK_EMPTY_MASS);

	//SetMaxFuelMass (TANK_MAX_PROPELLANT_MASS);
	// Note that the Tank instance is only created after separation from
	// the orbiter, so the actual fuel mass will always be much smaller

	SetISP (5000.0);

	SetMaxThrust (ENGINE_MAIN, 0);
	SetMaxThrust (ENGINE_RETRO, 0);
	SetMaxThrust (ENGINE_HOVER, 0);
	SetMaxThrust (ENGINE_ATTITUDE, 0);
	// Tank has no engines of its own

	SetCW (0.2, 0.3, 1.2, 1.2);
	VECTOR3 cs = {412.1,411.8,72.7};
	SetCrossSections (cs);
	VECTOR3 rd = {0.5,0.5,0.1};
	SetRotDrag (rd);
	VECTOR3 pmi = {145.6,145.6,10.5};
	SetPMI (pmi);
	SetPitchMomentScale (1e-4);
	SetBankMomentScale (1e-4);

	VECTOR3 co = {0,0,0};
	SetCameraOffset (co);
	// Note that the camera offset should not be required
	// since the Tank doesn't define a 'cockpit'

	SetCOG_elev (-5.0);
	SetTouchdownPoints (_V(0,9,3), _V(-1,1,-3), _V(1,1,-3));
	SetLiftCoeffFunc (0);

	//////////////////////// ET vent ////////////////////////
	phLOXtank = CreatePropellantResource( 400 );
	thLOXvent = CreateThruster( _V( -3.5, -3.5, 6 ), _V( 0.719, 0.695, 0 ), 600, phLOXtank, 300, 300 );
	PARTICLESTREAMSPEC psLOXvent = {
		0,
		0.1,
		2,
		50,
		0,
		1,
		25,
		1,
		PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_PLIN,
		0.75, 1,// match the #1 value with the pulse % below...
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0};
	
	AddExhaustStream( thLOXvent, &psLOXvent );
	//////////////////////// ET vent ////////////////////////

	AddMesh (hTankMesh);
}

// Simulation time step
void Atlantis_Tank::clbkPostStep (double simt, double simdt, double mjd)
{
	if (GetAltitude() < 0.0) oapiDeleteVessel (GetHandle());

	//////////////////////// ET vent ////////////////////////
	if (timer == -1) timer = simt + 120;// start vent in 120 sec

	if (counter >= (int)((1 - pos) * 50))
	{
		pos = (simt * 10) - (int)(simt * 10);// new valve position
		counter = 0;
	}

	if ((simt - timer) > 0)
	{
		counter++;
		if (pos <= 0.75)// close it if under 75% so it pulses... looks cool!
		{
			SetThrusterLevel( thLOXvent, 0 );
		}
		else
		{
			SetThrusterLevel( thLOXvent, pos );
		}
	}
	else
	{
		SetThrusterLevel( thLOXvent, 0 );
	}
	//////////////////////// ET vent ////////////////////////
}

// ==============================================================
// API interface
// ==============================================================

// Initialisation
DLLCLBK VESSEL *ovcInit (OBJHANDLE hvessel, int flightmodel)
{
	return new Atlantis_Tank (hvessel);
}

// Cleanup
DLLCLBK void ovcExit (VESSEL *vessel)
{
	if (vessel) delete (Atlantis_Tank*)vessel;
}

// Vessel status initialisation
DLLCLBK void ovcSetState (VESSEL *vessel, const VESSELSTATUS *status)
{
	vessel->DefSetState (status);	        // default initialisation
}
