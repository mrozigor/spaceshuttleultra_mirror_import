#ifndef ATLANTIS_TANK_H
#define ATLANTIS_TANK_H
#pragma once

#include "orbitersdk.h"
#include "discsignals.h"
#include "Sensor.h"


using namespace discsignals;


//const static char* DEFAULT_MESHNAME_ET = "SSU/SLWT";
const static char* SWT_MESHNAME_ET = "SSU/SWT";
const static char* LWT_MESHNAME_ET = "SSU/LWT";
const static char* SLWT_MESHNAME_ET = "SSU/SLWT";

//const static char* DEFAULT_NORMAL_ET_TEXTURE = "SSU/SLWT_ETtex.dds";
const static char* DEFAULT_SCORCHED_ET_TEXTURE = "SSU/ETburntex.dds";

// HACK assuming constant temperatures during powered flight, using values are at SSME exit minus 100K
const double GOX_TEMPERATURE = 448.7056 - 100;// K
const double GH2_TEMPERATURE = 255.3722 - 100;// K

// constants for van der waals equation
const double O2_VDW_A = 0.1382;// Pa m^3
const double O2_VDW_B = 0.0000319;// m^3/mol
const double H2_VDW_A = 0.02452;// Pa m^3
const double H2_VDW_B = 0.0000265;// m^3/mol

const double LOX_TANK_VOLUME = 553.9625;// m^3
const double LOX_MAX_VOLUME = 548.3434;// m^3
const double LOX_MIN_ULLAGE_VOLUME = LOX_TANK_VOLUME - LOX_MAX_VOLUME;// m^3
const double LH2_TANK_VOLUME = 1515.461;// m^3
const double LH2_MAX_VOLUME = 1475.44;// m^3
const double LH2_MIN_ULLAGE_VOLUME = LH2_TANK_VOLUME - LH2_MAX_VOLUME;// m^3

const double LOX_MAX_PROPELLANT_MASS = 624252.0;
const double LH2_MAX_PROPELLANT_MASS = 104463.23;// seems to already include a 2145.485 lbs fuel bias, for 6.032 MR
const double TANK_MAX_PROPELLANT_MASS = LOX_MAX_PROPELLANT_MASS+LH2_MAX_PROPELLANT_MASS;
// Main tank propellant mass [kg]

// Main tank empty mass
const double SWT_EMPTY_MASS = 35425.0;	//STS-1 to STS-5 tank 76000lb 35425.0kg
const double LWT_EMPTY_MASS = 29937.0;	//Light weight tank 66000lb 29937kg, STS-6 to STS-90
const double SLWT_EMPTY_MASS = 26535.0;	//Super light weight tank, 58500lb, STS-91 on

// ==========================================================
// Interface for derived vessel class: Atlantis_Tank
// ==========================================================

class Atlantis_Tank: public VESSEL2 {
public:
	Atlantis_Tank (OBJHANDLE hObj);
	// Construct interface from existing object

	// Overloaded callback functions
	void clbkVisualCreated(VISHANDLE vis, int refcount);
	void clbkVisualDestroyed(VISHANDLE vis, int refcount);
	void clbkSetClassCaps (FILEHANDLE cfg);
	void clbkPostStep (double simt, double simdt, double mjd);
	void clbkLoadStateEx(FILEHANDLE scn, void* status);	
	void clbkSaveState (FILEHANDLE scn);
	void clbkPostCreation( void );
	
	virtual void UseBurntETTexture();

	/**
	 * Simulated propellant flow from the External Tank.
	 * @param[in,out]	LOXmass	mass of liquid oxygen (in kg) to subtract from the ET, returns actual mass subtracted
	 * @param[in,out]	LH2mass	mass of liquid hydrogen (in kg) to subtract from the ET, returns actual mass subtracted
	 */
	virtual void PropellantFlow( double& LOXmass, double& LH2mass );

	/**
	 * Simulated pressurant flow into the External Tank.
	 * @param[in]	GOXmass	mass of gaseous oxygen pressurant (in kg) to add to the ET
	 * @param[in]	GH2mass	mass of gaseous hydrogen pressurant (in kg) to add to the ET
	 */
	virtual void PressurantFlow( double GOXmass, double GH2mass );

	/**
	 * Returns the remaining propellant mass level in the ET. No distinction is made between LOX and LH2 masses.
	 * @return		remaining propellant mass (in percent)
	 */
	virtual double GetPropellantLevel( void ) const;

	/**
	 * Returns the ullage pressure inside the LOX tank.
	 * This function should only be used for physics purposes.
	 * @return		LOX tank ullage pressure (in pascal)
	 * @see GetLH2UllagePressure
	 */
	virtual double GetLOXUllagePressure( void ) const;

	/**
	 * Returns the ullage pressure inside the LH2 tank.
	 * This function should only be used for physics purposes.
	 * @return		LH2 tank ullage pressure (in pascal)
	 * @see GetLOXUllagePressure
	 */
	virtual double GetLH2UllagePressure( void ) const;

private:
	virtual void UseETTexture(const char* pszTexName);

	/**
	 * This function updates the output of the pressure and level sensors.
	 */
	void UpdateSensors( void );

	VISHANDLE hVis;

	MESHHANDLE hTankMesh;
	DEVMESHHANDLE hDevTankMesh;
	UINT mesh_idx;
	bool bUseBurntTexture;

	// values specified in scenario
	double scenarioMass;
	char pszScenarioTexture[255];

	ATTACHMENTHANDLE ahToOrbiter;

	PROPELLANT_HANDLE phET;
	double LOXmass;// kg
	double LH2mass;// kg
	double LOXullagepress;// Pa
	double LH2ullagepress;// Pa
	double GOXmass;// g
	double GH2mass;// g

	// level sensors (wet = true/5v, dry = false/0v)
	// considering level sensors are 0.15% "tall"
	Sensor LOXPct5LevelSensor;
	Sensor LOXPct98LevelSensor[2];
	Sensor LOXPct100MinusLevelSensor;//99.85%
	Sensor LOXPct100LevelSensor[2];
	Sensor LOXPct100PlusLevelSensor;// 100.15%
	Sensor LOXOverfillLevelSensor;// HACK using 101%

	Sensor LH2LowLevelSensor[4];// HACK using 0.2-0.05%
	Sensor LH2Pct5LevelSensor;
	Sensor LH2Pct98LevelSensor[2];
	Sensor LH2Pct100MinusLevelSensor;//99.85%
	Sensor LH2Pct100LevelSensor[2];
	Sensor LH2Pct100PlusLevelSensor;// 100.15%
	Sensor LH2OverfillLevelSensor;// HACK using 101%

	// pressure sensors
	Sensor LOXUllagePressureSensor[4];

	Sensor LH2UllagePressureSensor[4];

	bool sensorsconnected;
	bool postsep;

	// vents
	THRUSTER_HANDLE thGOXventNE;
	THRUSTER_HANDLE thGOXventSW;
	THRUSTER_HANDLE thGH2vent;
};

#endif
