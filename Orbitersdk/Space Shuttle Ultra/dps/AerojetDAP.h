#ifndef AEROJETDAP_H_77EB3AE1_0A62_4181_8370_0F9995B10FEF
#define AEROJETDAP_H_77EB3AE1_0A62_4181_8370_0F9995B10FEF
#pragma once

#include "../PIDControl.h"
#include "../AtlantisSubsystem.h"
#include "../SSUEngConst.h"
#include "discsignals.h"

namespace dps
{

using namespace discsignals;

/**
 * DAP during entry, TAEM and A/L phases (MM304 and MM305)
 * At the moment, only AOA autopilot during entry is working
 */
class AerojetDAP : public AtlantisSubsystem
{
	bool bFirstStep;

	// Orbiter data
	double OrbiterMass;
	VECTOR3 PMI;
	VECTOR3 RCSTorque;

	PIDControl AOA_ElevonPitch; // converts AOA error to elevon command
	PIDControl Rate_ElevonPitch; // converts pitch rate error (in degrees) to elevon command
	PIDControl Pitch_ElevonPitch; // converts pitch angle error (in degrees) to elevon command
	//PIDControl BodyFlap;
	
	DiscInPort PitchAuto, RollYawAuto;
	DiscInPort RHCInput[3];
	DiscOutPort ThrusterCommands[3];
	DiscOutPort LeftElevonCommand, RightElevonCommand;
	
	bool ThrustersActive[3]; // indicates if each set of thrusters (pitch, yaw, roll) is active
	bool AerosurfacesActive[3];
	bool CSSInitialized[3];

	// attitude and rates relative to velocity vector (AOA, sideslip, bank)
	// values in degrees or deg/sec
	VECTOR3 degTargetAttitude;
	VECTOR3 degCurrentAttitude;
	VECTOR3 degCurrentRates;
	bool RotatingAxis[3]; // indicates if Orbiter is maneuvering aronud an axis
public:
	AerojetDAP(AtlantisSubsystemDirector* _director);
	virtual ~AerojetDAP();

	virtual void Realize();

	virtual void OnPostStep(double SimT, double DeltaT, double MJD);
private:
	void SetThrusterLevels();
	/**
	 * Checks if any active thruster groups should be disabled.
	 * Checks if any inactive control surfaces should be enabled.
	 */
	void CheckControlActivation();

	/**
	 * Get AOA, bank and sideslip values and rates.
	 */
	void GetAttitudeData(double DeltaT);
	void UpdateOrbiterData();

	double GetThrusterCommand(AXIS axis);

	/**
	 * Uses lookup table to calculate target AOA for current mach number.
	 */
	double CalculateTargetAOA(double mach) const;
	/**
	 * Uses RHC input to set elevon position.
	 */
	void ControllerInputGuidance(double DeltaT);
};

};

#endif