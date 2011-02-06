#ifndef AEROJETDAP_H_77EB3AE1_0A62_4181_8370_0F9995B10FEF
#define AEROJETDAP_H_77EB3AE1_0A62_4181_8370_0F9995B10FEF
#pragma once

#include "../PIDControl.h"
//#include "../AtlantisSubsystem.h"
#include "SimpleGPCSoftware.h"
#include <EngConst.h>
#include <discsignals.h>

namespace dps
{

using namespace discsignals;

/**
 * Offset between OGS aimpoint and rwy threshold
 */
const double OGS_AIMPOINT = -7500/MPS2FPS;

/**
 * DAP during entry, TAEM and A/L phases (MM304 and MM305)
 * At the moment, only AOA autopilot during entry is working
 */
class AerojetDAP : public SimpleGPCSoftware
{
private:	
	typedef enum {HDG, PRFNL, OGS, FLARE} GUIDANCE_MODE;
	typedef enum {LOVHD, ROVHD, STIN} HAC_DIRECTION;

	class LandingSiteData
	{
	public:
		double radPriLat, radPriLong;
		double radSecLat, radSecLong;
		double degPriHeading; // add 180 to get sec heading
		std::string strPri, strSec;

		LandingSiteData(double _radPriLat, double _radPriLong, double _radSecLat, double _radSecLong, double _degPriHeading, const char* pri, const char* sec)
			: radPriLat(_radPriLat), radPriLong(_radPriLong), radSecLat(_radSecLat), radSecLong(_radSecLong),
			degPriHeading(_degPriHeading),
			strPri(pri), strSec(sec)
		{
		}

		~LandingSiteData()
		{
		}
	};

	bool bFirstStep, bSecondStep;

	// Orbiter data
	double OrbiterMass;
	VECTOR3 PMI;
	VECTOR3 RCSTorque;

	//PIDControl AOA_ElevonPitch; // converts AOA error to elevon command
	PIDControl ElevonPitch; // converts angle error (AOA or pitch, in degrees) to elevon command
	//PIDControl Rate_ElevonPitch; // converts pitch rate error (in degrees) to elevon command
	PIDControl Pitch_ElevonPitch; // converts pitch angle error (in degrees) to elevon command
	PIDControl Roll_AileronRoll; // converts roll angle error (in degrees) to aileron command
	PIDControl Yaw_AileronYaw; // converts roll angle error (in degrees) to aileron command
	//PIDControl BodyFlap;
	
	DiscInPort PitchAuto, RollYawAuto;
	DiscInPort RHCInput[3];
	DiscOutPort ThrusterCommands[3];
	//DiscOutPort LeftElevonCommand, RightElevonCommand;
	DiscOutPort ElevonCommand, AileronCommand;
	
	bool ThrustersActive[3]; // indicates if each set of thrusters (pitch, yaw, roll) is active
	bool AerosurfacesActive[3];
	bool CSSInitialized[3];

	// attitude and rates relative to velocity vector (AOA, sideslip, bank)
	// values in degrees or deg/sec
	VECTOR3 degTargetAttitude;
	VECTOR3 degCurrentAttitude;
	VECTOR3 degCurrentRates;
	bool RotatingAxis[3]; // indicates if Orbiter is maneuvering aronud an axis
	
	OBJHANDLE hEarth;

	GUIDANCE_MODE GuidanceMode;

	//HAC_DIRECTION HACDirection;
	MATRIX3 RwyRotMatrix;
	VECTOR3 RwyPos;
	double degRwyHeading;
	//double TargetGlideslope;
	//double TargetPitchRate;
	double NZCommand;
	//double TargetPitch, TargetBank;
	double TargetBank;
	double prfnlBankFader;
	double HAC_TurnRadius;
	double gravity_force;

	std::vector<LandingSiteData> vLandingSites;

	int SITE_ID;
	bool SEC; // true if secondary runway is used
public:
	AerojetDAP(SimpleGPCSystem* _gpc);
	virtual ~AerojetDAP();

	virtual void Realize();

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);

	virtual bool OnMajorModeChange(unsigned int newMajorMode);
	virtual bool ItemInput(int spec, int item, const char* Data);
	//virtual bool ExecPressed(int spec);
	virtual bool OnPaint(int spec, vc::MDU* pMDU) const;
	virtual bool OnDrawHUD(const HUDPAINTSPEC* hps, oapi::Sketchpad* skp) const;
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

	/**
	 * Calculates thruster command (1.0, -1.0 or 0.0) to rotate to target angle in given axis.
	 * Computes target rotation rate and returns thruster command to establish correct rate.
	 */
	double GetThrusterCommand(AXIS axis, double DeltaT);

	/**
	 * Uses lookup table to calculate target AOA for current mach number.
	 */
	double CalculateTargetAOA(double mach) const;

	void SetAerosurfaceCommands(double DeltaT);
	/**
	 * Uses RHC input to set elevon position.
	 */
	void CSSPitchGuidance(double DeltaT);
	void CSSRollGuidance(double DeltaT);

	void CalculateHACGuidance(double DeltaT);
	/**
	 * \param RwyPos position relative to runway threshold in rwy-relative frame
	 * \return target bank angle for shuttle on OGS
	 * \note X=distance from runway (-ve) Y=offset from centerline Z=height (-ve)
	 */
	double CalculatePrefinalBank(const VECTOR3 &RwyPos);
	/**
	 * \param RelPos position relative to runway threshold in rwt-relative frame
	 * \return glideslope in degrees
	 * \note X=distance from runway (-ve) Y=offset from centerline Z=height (-ve)
	 */
	void CalculateTargetGlideslope(const VECTOR3& TgtPos, double DeltaT);
	/**
	 * \param RwyPos position relative to runway threshold in rwy-relative frame
	 * \return glideslope for shuttle on OGS
	 * \note X=height Y=distance from runway (-ve) Z=offset from centerline
	 */
	double CalculateOGS(const VECTOR3 &RwyPos) const;
	/**
	 * \param RwyPos position relative to runway threshold in rwy-relative frame
	 * \note X=height Y=distance from runway (-ve) Z=offset from centerline
	 * \note Calculates target glideslope rate for preflare
	 */
	void InitiatePreflare();
	/**
	 * \param RwyPos position relative to runway threshold in rwy-relative frame
	 * \return glideslope for shuttle performing preflare
	 */
	double CalculatePreflareGlideslope(const VECTOR3 &RwyPos) const;
	double CalculateNZCommand(const VECTOR3& velocity, double predRange, double curAlt, double DeltaT) const;

	/**
	 * Called every time the target runway is changed.
	 * Updates internal variables used for calculations.
	 */
	void InitializeRunwayData();
	VECTOR3 GetRunwayRelPos() const;

	void LoadLandingSiteList();

	void PaintHORIZSITDisplay(vc::MDU* pMDU) const;
};

};

#endif