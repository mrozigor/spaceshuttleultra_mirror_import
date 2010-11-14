#ifndef AEROJETDAP_H_77EB3AE1_0A62_4181_8370_0F9995B10FEF
#define AEROJETDAP_H_77EB3AE1_0A62_4181_8370_0F9995B10FEF
#pragma once

#include "../PIDControl.h"
#include "../AtlantisSubsystem.h"
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

	PIDControl AOA_ElevonPitch; // converts AOA error to elevon command
	PIDControl Rate_ElevonPitch; // converts pitch rate error (in degrees) to elevon command
	//PIDControl BodyFlap;
	
	DiscInPort PitchAuto, RollYawAuto;
	DiscInPort RHCInput[3];
	DiscOutPort ThrusterCommands[3];
	DiscOutPort LeftElevonCommand, RightElevonCommand;
	
	bool ThrustersActive[3]; // indicates if each set of thrusters (pitch, yaw, roll) is active
public:
	AerojetDAP(AtlantisSubsystemDirector* _director);
	virtual ~AerojetDAP();

	virtual void Realize();

	virtual void OnPostStep(double SimT, double DeltaT, double MJD);
private:
	void SetThrusterLevels();
	void CheckThrusterActivation();

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