// ==============================================================
//                 ORBITER MODULE: Atlantis
//                  Part of the ORBITER SDK
//          Copyright (C) 2001-2003 Martin Schweiger
//                   All rights reserved
//
// Atlantis_Tank.cpp
// Reference implementation of Atlantis Tank vessel class module
// Note: The propellant tanks associated with the ET are defined
// by the Atlantis class. This module is attached to the orbiter
// during launch and takes control of the tank after separation
// from the orbiter.
// ==============================================================

#define ORBITER_MODULE
#include "Atlantis.h"
#include "Atlantis_Tank.h"
#include "CommonDefs.h"
#include "math.h"
#include <string>

// ==============================================================
// Specialised vessel class Atlantis_Tank
// ==============================================================

// Constructor
Atlantis_Tank::Atlantis_Tank (OBJHANDLE hObj)
: VESSEL2(hObj), bUseBurntTexture(false), scenarioMass(-1.0)
{
	// preload mesh
	mesh_idx = MESH_UNDEFINED;
	//hTankMesh = oapiLoadMeshGlobal (DEFAULT_MESHNAME_ET);

	LOXPct5LevelSensor = Sensor( 4.85, 5 );
	LOXPct98LevelSensor[0] = Sensor( 97.85, 98 );
	LOXPct98LevelSensor[1] = Sensor( 97.85, 98 );
	LOXPct100MinusLevelSensor = Sensor( 99.7, 99.85 );
	LOXPct100LevelSensor[0] = Sensor( 99.85, 100 );
	LOXPct100LevelSensor[1] = Sensor( 99.85, 100 );
	LOXPct100PlusLevelSensor = Sensor( 100, 100.15 );
	LOXOverfillLevelSensor = Sensor( 100.85, 101 );
	LH2LowLevelSensor[0] = Sensor( 0.05, 0.2 );
	LH2LowLevelSensor[1] = Sensor( 0.05, 0.2 );
	LH2LowLevelSensor[2] = Sensor( 0.05, 0.2 );
	LH2LowLevelSensor[3] = Sensor( 0.05, 0.2 );
	LH2Pct5LevelSensor = Sensor( 4.85, 5 );
	LH2Pct98LevelSensor[0] = Sensor( 97.85, 98 );
	LH2Pct98LevelSensor[1] = Sensor( 97.85, 98 );
	LH2Pct100MinusLevelSensor = Sensor( 99.7, 99.85 );
	LH2Pct100LevelSensor[0] = Sensor( 99.85, 100 );
	LH2Pct100LevelSensor[1] = Sensor( 99.85, 100 );
	LH2Pct100PlusLevelSensor = Sensor( 100, 100.15 );
	LH2OverfillLevelSensor = Sensor( 100.85, 101 );
	LOXUllagePressureSensor[0] = Sensor( 0, 50, 0.005 );
	LOXUllagePressureSensor[1] = Sensor( 0, 50, 0.005 );
	LOXUllagePressureSensor[2] = Sensor( 0, 50, 0.005 );
	LOXUllagePressureSensor[3] = Sensor( 0, 50, 0.005 );
	LH2UllagePressureSensor[0] = Sensor( 12, 52, 0.02 );
	LH2UllagePressureSensor[1] = Sensor( 12, 52, 0.02 );
	LH2UllagePressureSensor[2] = Sensor( 12, 52, 0.02 );
	LH2UllagePressureSensor[3] = Sensor( 12, 52, 0.02 );

	sensorsconnected = false;
	postsep = false;
	GO2VentReliefValveOpen = false;
	GH2VentReliefValveOpen = false;
}

void Atlantis_Tank::UseBurntETTexture()
{
	bUseBurntTexture = true;
	if(strScenarioBurnTex.empty()) UseETTexture(DEFAULT_SCORCHED_ET_TEXTURE);
	else UseETTexture(strScenarioBurnTex.c_str());
}

void Atlantis_Tank::UseETTexture(const char* pszTexName)
{
	SURFHANDLE texture = oapiLoadTexture(pszTexName);
	if(!oapiSetTexture(hDevTankMesh, 1, texture))
		oapiWriteLog("(Atlantis_Tank) ERROR: Could not update texture");
}

void Atlantis_Tank::PropellantFlow( double& LOXmass, double& LH2mass )
{
	this->LOXmass -= LOXmass;
	if (this->LOXmass < 0)
	{
		LOXmass += this->LOXmass;
		this->LOXmass = 0;
	}

	this->LH2mass -= LH2mass;
	if (this->LH2mass < 0)
	{
		LH2mass += this->LH2mass;
		this->LH2mass = 0;
	}

	SetPropellantMass( phET, this->LOXmass + this->LH2mass );
	return;
}

void Atlantis_Tank::PressurantFlow( double GOXmass, double GH2mass )
{
	// HACK take the masses directly from the ET, as it's more simple and the small error this
	// gives only appear when ET is empty (just before MECO on rare occasions)
	PropellantFlow( GOXmass, GH2mass );

	// add masses (put ullage mass as emtpy mass)
	SetEmptyMass( GetEmptyMass() + GOXmass + GH2mass );

	this->GOXmass += GOXmass * 1000;
	this->GH2mass += GH2mass * 1000;
	return;
}

double Atlantis_Tank::GetPropellantLevel( void ) const
{
	return 100.0 * (GetPropellantMass( phET ) / TANK_MAX_PROPELLANT_MASS);
}

double Atlantis_Tank::GetLOXUllagePressure( void ) const
{
	return LOXullagepress;
}

double Atlantis_Tank::GetLH2UllagePressure( void ) const
{
	return LH2ullagepress;
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

	// see what type of tank this is
	char pszBuffer[255];
	bool bFoundData = oapiReadItem_string(cfg, "Type", pszBuffer);
	if(!bFoundData || !_strnicmp(pszBuffer, "SLWT", 4)) { // default to SLWT tank
		SetEmptyMass (SLWT_EMPTY_MASS);
		hTankMesh = oapiLoadMeshGlobal (SLWT_MESHNAME_ET);
	}
	else if(!_strnicmp(pszBuffer, "LWT", 3)) {
		SetEmptyMass (LWT_EMPTY_MASS);
		hTankMesh = oapiLoadMeshGlobal (LWT_MESHNAME_ET);
	}
	else if(!_strnicmp(pszBuffer, "SWT", 3)) {
		SetEmptyMass (SWT_EMPTY_MASS);
		hTankMesh = oapiLoadMeshGlobal (SWT_MESHNAME_ET);
	}

	phET = CreatePropellantResource( TANK_MAX_PROPELLANT_MASS );

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

	// vents (initially with very high ISP so it uses little prop)
	// LOX
	thGOXventNE = CreateThruster( _V( 0, 0, 30.4 ), _V( -0.735112, 0.677946, 0 ), 600, phET, 99999999, 99999999 );
	thGOXventSW = CreateThruster( _V( 0, 0, 30.4 ), _V( 0.973112, -0.230333, 0 ), 600, phET, 99999999, 99999999 );
	PARTICLESTREAMSPEC psGOXvent = {
		0,
		0.1,
		50,
		20,
		0.1,
		0.4,
		6,
		5,
		PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_PLIN,
		0, 1,
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0};
	AddExhaustStream( thGOXventNE, &psGOXvent );
	AddExhaustStream( thGOXventSW, &psGOXvent );

	// LH2
	thGH2vent = CreateThruster( _V( -3.12321, -2.36732, 13.4955 ), _V( 0.788011, 0.615661, 0 ), 600, phET, 99999999, 99999999 );
	PARTICLESTREAMSPEC psGH2vent = {
		0,
		0.1,
		50,
		20,
		0,
		0.5,
		20,
		5,
		PARTICLESTREAMSPEC::DIFFUSE,
		PARTICLESTREAMSPEC::LVL_PLIN,
		0, 1,
		PARTICLESTREAMSPEC::ATM_FLAT,
		1, 1,
		0};
	AddExhaustStream( thGH2vent, &psGH2vent );

	mesh_idx = AddMesh (hTankMesh);

	//ahToOrbiter = CreateAttachment(true, _V(0, -0.25, 7.5), _V(0, -1, 0), _V(0, 0, 1), "SSU_ET");
	ahToOrbiter = CreateAttachment(true, _V(0, 0.0, 0.0), _V(0, -1, 0), _V(0, 0, 1), "SSU_ET");
}

// Simulation time step
void Atlantis_Tank::clbkPostStep (double simt, double simdt, double mjd)
{
	if (GetAltitude() < 0.0) oapiDeleteVessel (GetHandle());

	if (sensorsconnected == false)
	{
		// connect sensors
		OBJHANDLE hOV = GetAttachmentStatus( ahToOrbiter );

		if (hOV != NULL)
		{
			Atlantis* OV = static_cast<Atlantis*> (oapiGetVesselInterface( hOV ));

			DiscreteBundle *pBundle = OV->BundleManager()->CreateBundle( "ET_LOX_SENSORS", 16 );
			LOXPct5LevelSensor.Connect( pBundle, 4 );
			LOXPct98LevelSensor[0].Connect( pBundle, 5 );
			LOXPct98LevelSensor[1].Connect( pBundle, 6 );
			LOXPct100MinusLevelSensor.Connect( pBundle, 7 );
			LOXPct100LevelSensor[0].Connect( pBundle, 8 );
			LOXPct100LevelSensor[1].Connect( pBundle, 9 );
			LOXPct100PlusLevelSensor.Connect( pBundle, 10 );
			LOXOverfillLevelSensor.Connect( pBundle, 11 );
			LOXUllagePressureSensor[0].Connect( pBundle, 12 );
			LOXUllagePressureSensor[1].Connect( pBundle, 13 );
			LOXUllagePressureSensor[2].Connect( pBundle, 14 );
			LOXUllagePressureSensor[3].Connect( pBundle, 15 );

			pBundle = OV->BundleManager()->CreateBundle( "ET_LH2_SENSORS", 16 );
			LH2LowLevelSensor[0].Connect( pBundle, 0 );
			LH2LowLevelSensor[1].Connect( pBundle, 1 );
			LH2LowLevelSensor[2].Connect( pBundle, 2 );
			LH2LowLevelSensor[3].Connect( pBundle, 3 );
			LH2Pct5LevelSensor.Connect( pBundle, 4 );
			LH2Pct98LevelSensor[0].Connect( pBundle, 5 );
			LH2Pct98LevelSensor[1].Connect( pBundle, 6 );
			LH2Pct100MinusLevelSensor.Connect( pBundle, 7 );
			LH2Pct100LevelSensor[0].Connect( pBundle, 8 );
			LH2Pct100LevelSensor[1].Connect( pBundle, 9 );
			LH2Pct100PlusLevelSensor.Connect( pBundle, 10 );
			LH2OverfillLevelSensor.Connect( pBundle, 11 );
			LH2UllagePressureSensor[0].Connect( pBundle, 12 );
			LH2UllagePressureSensor[1].Connect( pBundle, 13 );
			LH2UllagePressureSensor[2].Connect( pBundle, 14 );
			LH2UllagePressureSensor[3].Connect( pBundle, 15 );

			sensorsconnected = true;
		}
	}

	double LOXullagevol = ((1 - (LOXmass / LOX_MAX_PROPELLANT_MASS)) * LOX_MAX_VOLUME) + LOX_MIN_ULLAGE_VOLUME;
	double LH2ullagevol = ((1 - (LH2mass / LH2_MAX_PROPELLANT_MASS)) * LH2_MAX_VOLUME) + LH2_MIN_ULLAGE_VOLUME;
	
	// using van der waals now as it's easy to solve for pressure
	LOXullagepress = ((GOXmass * 8.314 * GOX_TEMPERATURE) / ((15.999 * LOXullagevol) - (GOXmass * O2_VDW_B))) - ((GOXmass * GOXmass * O2_VDW_A) / (15.999 * 15.999 * LOXullagevol * LOXullagevol));
	LH2ullagepress = ((GH2mass * 8.314 * GH2_TEMPERATURE) / ((1.008 * LH2ullagevol) - (GH2mass * H2_VDW_B))) - ((GH2mass * GH2mass * H2_VDW_A) / (1.008 * 1.008 * LH2ullagevol * LH2ullagevol));

	UpdateSensors();

	// vent/relief valves
	// HACK far from perfect but should only be used post sep, and only few care about the ET by then
	// GOX
	if (GO2VentReliefValveOpen == true)// opens at 31psig, closes at 29psig 
	{// HACK using psia for now
		// check if should close
		if ((LOXullagepress / PSI2PA) < 43.7)
		{
			GO2VentReliefValveOpen = false;
			// close
			SetThrusterLevel( thGOXventNE, 0 );
			SetThrusterLevel( thGOXventSW, 0 );
		}
		else
		{
			// keep open
			SetThrusterLevel( thGOXventNE, min( (LOXullagepress / PSI2PA) - 45, 1 ) );
			SetThrusterLevel( thGOXventSW, min( (LOXullagepress / PSI2PA) - 45, 1 ) );
			// decrease mass
			GOXmass -= 6000 * simdt;
		}
	}
	else
	{
		// check if should open
		if ((LOXullagepress / PSI2PA) > 45.7)
		{
			GO2VentReliefValveOpen = true;
			// open
			SetThrusterLevel( thGOXventNE, min( (LOXullagepress / PSI2PA) - 45, 1 ) );
			SetThrusterLevel( thGOXventSW, min( (LOXullagepress / PSI2PA) - 45, 1 ) );
			// decrease mass
			GOXmass -= 6000 * simdt;
		}
		else
		{
			// keep closed
			SetThrusterLevel( thGOXventNE, 0 );
			SetThrusterLevel( thGOXventSW, 0 );
		}
	}

	// GH2
	if (GH2VentReliefValveOpen == true)// opens at 50.7psia, closes at 48.7psia
	{
		// check if should close
		if ((LH2ullagepress / PSI2PA) < 48.7)
		{
			GH2VentReliefValveOpen = false;
			// close
			SetThrusterLevel( thGH2vent, 0 );
		}
		else
		{
			// keep open
			SetThrusterLevel( thGH2vent, min( (LH2ullagepress / PSI2PA) - 48, 1 ) );
			// decrease mass
			GH2mass -= 2000 * simdt;
		}
	}
	else
	{
		// check if should open
		if ((LH2ullagepress / PSI2PA) > 50.7)
		{
			GH2VentReliefValveOpen = true;
			// open
			SetThrusterLevel( thGH2vent, min( (LH2ullagepress / PSI2PA) - 48.7, 1 ) );
			// decrease mass
			GH2mass -= 2000 * simdt;
		}
		else
		{
			// keep closed
			SetThrusterLevel( thGH2vent, 0 );
		}
	}

	// post sep "reconfigure"
	if (!GetAttachmentStatus(ahToOrbiter))
	{
		if (postsep == false)// runs once after sep
		{
			// add ullage mass back to propellant resource to be used for venting
			SetPropellantMass( phET, GetPropellantMass( phET ) + ((GOXmass + GH2mass) / 1000) );
			SetEmptyMass( GetEmptyMass() - ((GOXmass + GH2mass) / 1000) );

			// change vent isp to sane values
			SetThrusterIsp( thGOXventNE, 100, 100 );
			SetThrusterIsp( thGOXventSW, 100, 100 );
			SetThrusterIsp( thGH2vent, 300, 300 );

			// disconnect sensors
			LOXPct5LevelSensor.Disconnect();
			LOXPct98LevelSensor[0].Disconnect();
			LOXPct98LevelSensor[1].Disconnect();
			LOXPct100MinusLevelSensor.Disconnect();
			LOXPct100LevelSensor[0].Disconnect();
			LOXPct100LevelSensor[1].Disconnect();
			LOXPct100PlusLevelSensor.Disconnect();
			LOXOverfillLevelSensor.Disconnect();
			LOXUllagePressureSensor[0].Disconnect();
			LOXUllagePressureSensor[1].Disconnect();
			LOXUllagePressureSensor[2].Disconnect();
			LOXUllagePressureSensor[3].Disconnect();
			LH2LowLevelSensor[0].Disconnect();
			LH2LowLevelSensor[1].Disconnect();
			LH2LowLevelSensor[2].Disconnect();
			LH2LowLevelSensor[3].Disconnect();
			LH2Pct5LevelSensor.Disconnect();
			LH2Pct98LevelSensor[0].Disconnect();
			LH2Pct98LevelSensor[1].Disconnect();
			LH2Pct100MinusLevelSensor.Disconnect();
			LH2Pct100LevelSensor[0].Disconnect();
			LH2Pct100LevelSensor[1].Disconnect();
			LH2Pct100PlusLevelSensor.Disconnect();
			LH2OverfillLevelSensor.Disconnect();
			LH2UllagePressureSensor[0].Disconnect();
			LH2UllagePressureSensor[1].Disconnect();
			LH2UllagePressureSensor[2].Disconnect();
			LH2UllagePressureSensor[3].Disconnect();

			postsep = true;
		}

		// "vaporize" 0.005% LOX mass remaining per second, and 0.05% LH2
		double tmp = LOXmass * 0.00005 * oapiRand() * simdt;
		GOXmass += tmp * 1000;
		LOXmass -= tmp;

		tmp = LH2mass * 0.0005 * oapiRand() * simdt;
		GH2mass += tmp * 1000;
		LH2mass -= tmp;
	}
	return;
}

void Atlantis_Tank::UpdateSensors( void )
{
	// LOX
	double lvl = 100 * LOXmass / LOX_MAX_PROPELLANT_MASS;
	LOXPct5LevelSensor.SetValue( lvl );
	LOXPct98LevelSensor[0].SetValue( lvl );
	LOXPct98LevelSensor[1].SetValue( lvl );
	LOXPct100MinusLevelSensor.SetValue( lvl );
	LOXPct100LevelSensor[0].SetValue( lvl );
	LOXPct100LevelSensor[1].SetValue( lvl );
	LOXPct100PlusLevelSensor.SetValue( lvl );
	LOXOverfillLevelSensor.SetValue( lvl );
	LOXUllagePressureSensor[0].SetValue( LOXullagepress / PSI2PA );
	LOXUllagePressureSensor[1].SetValue( LOXullagepress / PSI2PA );
	LOXUllagePressureSensor[2].SetValue( LOXullagepress / PSI2PA );
	LOXUllagePressureSensor[3].SetValue( LOXullagepress / PSI2PA );

	// LH2
	lvl = 100 * LH2mass / LH2_MAX_PROPELLANT_MASS;
	LH2LowLevelSensor[0].SetValue( lvl );
	LH2LowLevelSensor[1].SetValue( lvl );
	LH2LowLevelSensor[2].SetValue( lvl );
	LH2LowLevelSensor[3].SetValue( lvl );
	LH2Pct5LevelSensor.SetValue( lvl );
	LH2Pct98LevelSensor[0].SetValue( lvl );
	LH2Pct98LevelSensor[1].SetValue( lvl );
	LH2Pct100MinusLevelSensor.SetValue( lvl );
	LH2Pct100LevelSensor[0].SetValue( lvl );
	LH2Pct100LevelSensor[1].SetValue( lvl );
	LH2Pct100PlusLevelSensor.SetValue( lvl );
	LH2OverfillLevelSensor.SetValue( lvl );
	LH2UllagePressureSensor[0].SetValue( LH2ullagepress / PSI2PA );
	LH2UllagePressureSensor[1].SetValue( LH2ullagepress / PSI2PA );
	LH2UllagePressureSensor[2].SetValue( LH2ullagepress / PSI2PA );
	LH2UllagePressureSensor[3].SetValue( LH2ullagepress / PSI2PA );
	return;
}

void Atlantis_Tank::clbkLoadStateEx(FILEHANDLE scn, void *status)
{
	char* line;

	while(oapiReadScenario_nextline(scn, line)) {
		if(!_strnicmp(line, "USE_BURNT_TEX", 13)) {
			bUseBurntTexture = true;
		}
		else if(!_strnicmp(line, "EMPTY_MASS", 10)) {
			sscanf_s(line+10, "%lf", scenarioMass);
			SetEmptyMass(scenarioMass);
		}
		else if(!_strnicmp(line, "ET_TEX_NAME", 11)) {
			strScenarioTextureName = line+12;
			strScenarioTexture = "SSU\\" + strScenarioTextureName + ".dds";
		}
		else if(!_strnicmp(line, "BURNT_TEX_NAME", 14)) {
			strScenarioBurnTexName = line+15;
			strScenarioBurnTex = "SSU\\" + strScenarioBurnTexName + ".dds";
		}
		else ParseScenarioLineEx(line, status);
	}
}

void Atlantis_Tank::clbkSaveState(FILEHANDLE scn)
{
	VESSEL2::clbkSaveState(scn);

	if(bUseBurntTexture) oapiWriteLine(scn, "  BURNT_TEX");
	if(scenarioMass>0.0) oapiWriteScenario_float(scn, "EMPTY_MASS", scenarioMass);
	if(!strScenarioTextureName.empty()) oapiWriteScenario_string(scn, "ET_TEX_NAME", const_cast<char*>(strScenarioTextureName.c_str()));
	if(!strScenarioBurnTexName.empty()) oapiWriteScenario_string(scn, "BURNT_TEX_NAME", const_cast<char*>(strScenarioBurnTexName.c_str()));
}

void Atlantis_Tank::clbkPostCreation( void )
{
	// use prop level just loaded from the .scn to finish lox/lh2 mass setup
	// HACK always assuming post pre-press conditions
	double lvl = GetPropellantMass( phET ) / TANK_MAX_PROPELLANT_MASS;

	LOXmass = lvl * LOX_MAX_PROPELLANT_MASS;
	LH2mass = lvl * LH2_MAX_PROPELLANT_MASS;

	LOXullagepress = 35.7 * PSI2PA;// Pa pre-press 21+14.7=35.7 psia, in flight 20-25psig (34.7-39.7psia), relief at 31psig (45.7psia)
	LH2ullagepress = 47 * PSI2PA;// Pa pre-press 47 psia, in flight 32.8-33.2psia, relief at 36psig (50.7psia)

	// initially use the pressures above and derive GH2/GOX mass
	// HACK using the ideal gas equation because the van der waals is a little hardcore to solve for mass
	double LOXullagevol = ((1 - lvl) * LOX_MAX_VOLUME) + LOX_MIN_ULLAGE_VOLUME;
	double LH2ullagevol = ((1 - lvl) * LH2_MAX_VOLUME) + LH2_MIN_ULLAGE_VOLUME;

	GOXmass = (LOXullagepress * LOXullagevol * 15.999) / (8.314 * GOX_TEMPERATURE);
	GH2mass = (LH2ullagepress * LH2ullagevol * 1.008) / (8.314 * GH2_TEMPERATURE);

	SetEmptyMass( GetEmptyMass() + ((GOXmass + GH2mass) / 1000) );
	return;
}

void Atlantis_Tank::clbkVisualCreated(VISHANDLE vis, int refcount)
{
	hVis = vis;

	hDevTankMesh = GetDevMesh(vis, mesh_idx);

	if(bUseBurntTexture) UseBurntETTexture();
	if(!strScenarioTexture.empty()) UseETTexture(strScenarioTexture.c_str());
}

void Atlantis_Tank::clbkVisualDestroyed(VISHANDLE vis, int refcount)
{
	if(vis == hVis) hVis = NULL;
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
