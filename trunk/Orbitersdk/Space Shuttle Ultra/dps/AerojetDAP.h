#ifndef AEROJETDAP_H_77EB3AE1_0A62_4181_8370_0F9995B10FEF
#define AEROJETDAP_H_77EB3AE1_0A62_4181_8370_0F9995B10FEF
#pragma once

#include "../PIDControl.h"
//#include "../AtlantisSubsystem.h"
#include "SimpleGPCSoftware.h"
#include <EngConst.h>
#include <discsignals.h>
#include <Averager.h>
#include "../ReentryDragTable.h"

namespace dps
{

using namespace discsignals;

/**
* Offset between IGS aimpoint and rwy threshold
*/
const double IGS_AIMPOINT = 1000.0/MPS2FPS;
/**
 * Offset between OGS aimpoint and rwy threshold
 */
const double OGS_AIMPOINT = -7500/MPS2FPS;
const double AL_GS = tan(-20.0*RAD);
const double Y_AL_INTERCEPT = 10018.0/MPS2FPS;
const double X_AL_INTERCEPT = OGS_AIMPOINT + Y_AL_INTERCEPT/AL_GS;

const double HAC_CENTER_X = OGS_AIMPOINT - 33020.0/MPS2FPS;
const double FINAL_RADIUS = 14000.0/MPS2FPS; // center of HAC in Y direction is +/- FINAL_RADIUS
const double R1 = 0.0;
const double R2 = 0.093/MPS2FPS;

const int NZ_VALUE_COUNT = 5;
const double NZ_UPDATE_INTERVAL = 0.1;

/**
 * DAP during entry, TAEM and A/L phases (MM304 and MM305)
 * At the moment, only AOA autopilot during entry is working
 */
class AerojetDAP : public SimpleGPCSoftware
{
private:	
	typedef enum {PREENTRY, TEMP_CONTROL, EQU_GLIDE, CONST_DRAG, TRANSITION} ENTRY_GUIDANCE_MODE;
	typedef enum {ACQ, HDG, PRFNL, OGS, FLARE, FNLFL} TAEM_GUIDANCE_MODE;
	typedef enum {L, R} HAC_SIDE;
	typedef enum {OVHD, STRT} HAC_DIRECTION;

	class LandingSiteData
	{
		double radPriLat, radPriLong;
		double radSecLat, radSecLong;
		double degPriHeading; // add 180 to get sec heading
		std::string strPri, strSec;

	public:
		LandingSiteData(double _radPriLat, double _radPriLong, double _radSecLat, double _radSecLong, double _degPriHeading, const char* pri, const char* sec)
			: radPriLat(_radPriLat), radPriLong(_radPriLong), radSecLat(_radSecLat), radSecLong(_radSecLong),
			degPriHeading(_degPriHeading),
			strPri(pri), strSec(sec)
		{
		}

		~LandingSiteData()
		{
		}

		void GetRwyPosition(bool pri, double& radLat, double& radLong) const
		{
			if(pri) {
				radLat = radPriLat;
				radLong = radPriLong;
			}
			else {
				radLat = radSecLat;
				radLong = radSecLong;
			}
		}

		double GetRwyHeading(bool pri) const
		{
			if(pri) return degPriHeading;
			else {
				double heading = degPriHeading + 180.0;
				if(heading >= 360.0) heading-=360.0;
				return heading;
			}
		}

		const std::string& GetPriRwyName() const
		{
			return strPri;
		}

		const std::string& GetSecRwyName() const
		{
			return strSec;
		}
	};

	bool bFirstStep, bSecondStep;

	bool bWONG;

	// Orbiter data
	double OrbiterMass;
	VECTOR3 PMI;
	VECTOR3 RCSTorque;

	//PIDControl AOA_ElevonPitch; // converts AOA error to elevon command
	PIDControl ElevonPitch; // converts angle error (AOA or pitch, in degrees) to elevon command
	//PIDControl Rate_ElevonPitch; // converts pitch rate error (in degrees) to elevon command
	PIDControl Pitch_ElevonPitch; // converts pitch angle error (in degrees) to elevon command
	PIDControl Roll_AileronRoll; // converts roll angle error (in degrees) to aileron command
	PIDControl Yaw_RudderYaw; // converts roll angle error (in degrees) to aileron command
	PIDControl QBar_Speedbrake; // converts qbar error (in kPa) to speedbrake command; used in TAEM phase
	PIDControl Vel_Speedbrake; // converts speed error (KEAS) to speedbrake command; used in A/L phase
	//PIDControl BodyFlap;
	
	DiscInPort PitchAuto, RollYawAuto;
	DiscInPort SpeedbrakeAuto;
	DiscInPort SpdbkThrotPort;
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
	//VECTOR3 degTargetRates; // used in aerosurface control to ramp rotation rates
	bool RotatingAxis[3]; // indicates if Orbiter is maneuvering aronud an axis
	
	OBJHANDLE hEarth;

	ENTRY_GUIDANCE_MODE EntryGuidanceMode;
	VECTOR3 HAC_Center;
	double HAC_Lat, HAC_Long;
	double refDrag; // DREFP
	double referenceDrag23; // D23
	double constDragLevel; // T2
	double constDragStartVel; // VCG
	double lastTgtAltitude;
	double lastRefVSpeed;
	//double lastVAccSum;
	//std::queue<double> lastVAccs;
	//double lastVspeedSum;
	//std::queue<double> lastVspeeds;
	//double lastTargetAltitudeSum;
	//std::queue<double> lastTargetAltitudes;
	Averager tgtAltAveraging;
	Averager vspeedAveraging;
	Averager vaccAveraging;
	double tgtBankSign; // changed at each roll reversal
	bool performedFirstRollReversal;

	/** Variables used by TAEM guidance **/
	TAEM_GUIDANCE_MODE TAEMGuidanceMode;
	HAC_DIRECTION HACDirection;
	HAC_SIDE HACSide;
	MATRIX3 RwyRotMatrix; // converts from Orbiter Earth local frame to runway frame
	VECTOR3 RwyPos;
	double degRwyHeading;
	double degTargetGlideslope;
	//double TargetPitchRate;
	double prfnlBankFader;
	double HAC_TurnRadius; // also used by Entry Guidance
	double TotalRange;
	/** values for calculating NZ **/
	// NZ needs to be averaged to compensate for elevon deflections
	double lastNZUpdateTime;
	Averager nzAveraging;
	double averageNZ;
	//int curNZValueCount;
	/** values related to qbar (dynamic pressure) **/
	// all values in kPa
	double filteredQBar;

	/** Output guidance variables **/
	double NZCommand; // MM305 output
	double TargetBank;

	double elevonPos, aileronPos, rudderPos;

	std::vector<LandingSiteData> vLandingSites;

	double HUDFlashTime;
	bool bHUDFlasher; // used for flashing values on HUD

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

	virtual bool OnParseLine(const char* keyword, const char* value);
	virtual void OnSaveState(FILEHANDLE scn) const;

	DragTable* dTable;
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
	/**
	 * Calculate bank angle 'seen' by lift vector
	 * This is not the same as normal bank angle, because shuttle flies at high AOA (and lift vector is perpendicular to velocity, not direction)
	 * Returns (positive) value in degrees
	 */
	double CalculateCurrentLiftBank() const;
	/**
	 * Calculate lift bank required to produce desired vertical acceleration
	 * Returns (positive) value in degrees
	 */
	double CalculateRequiredLiftBank(double tgtVAcc) const;
	/**
	 * Update mass and PMI data
	 */
	void UpdateOrbiterData();

	/**
	 * Calculates thruster command (1.0, -1.0 or 0.0) to rotate to target angle in given axis.
	 * Computes target rotation rate and returns thruster command to establish correct rate.
	 */
	double GetThrusterCommand(AXIS axis, double DeltaT);

	/**
	 * Uses lookup table to calculate target AOA for current mach number.
	 * and lookup table to manipulate roll to maintain drag profile described at that table
	 */
	double CalculateTargetAOA(double mach) const;
	double CalculateTargetVAcc(double actual_vspeed, double target_vspeed, double actual_vacc, double dT);
	bool first_roll;
	bool roll_command;
	double target_bank;
	double last_vel;
	double last_h_error;
	double target_vspeed;
	double target_vacc;

	void SetAerosurfaceCommands(double DeltaT);
	/**
	 * Uses RHC input to set elevon position.
	 */
	void CSSPitchGuidance(double DeltaT);
	void CSSRollGuidance(double DeltaT);

	/**
	 * Calculates target drag (based on current guidance mode)
	 * /param range range to rwy [m]
	 */
	double CalculateTargetDrag(double DeltaT, double range);
	/**
	 * Updates averaging for target altitude, vspeed and vacc.
	 */
	void UpdateRequiredStateAveraging(double targetAltitude, double DeltaT, double SimT);
	//double CalculateTargetVAcc(double actual_vspeed, double target_vspeed, double);
	/**
	 * Calculates roll direction (-1 or +1) for maintaing delaz within limits.
	 * Updates tgtBankSign variable.
	 */
	void UpdateRollDirection(double mach, double delaz);
	
	/**
	 * Based on HAC direction (OVHD or STRT), selects left or right HAC
	 */
	void SelectHAC();

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
	 * \note X=height Y=distance from runway (-ve) Z=offset from centerline
	 * \note Calculates target glideslope rate for preflare
	 */
	void InitiatePreflare();
	/**
	 * \param RwyPos position relative to runway threshold in rwy-relative frame
	 * \return glideslope for shuttle performing preflare
	 */
	//double CalculatePreflareGlideslope(const VECTOR3 &RwyPos) const;
	/**
	 * \param RwyPos position relative to runway threshold in rwy-relative frame
	 * \return NZ acceleration for shuttle performing preflare
	 */
	double CalculatePreflareNZ(const VECTOR3 &RwyPos, double DeltaT);
	/**
	 * Updates NZCommand variable.
	 * Also calculates qbar error for speedbrake calculation.
	 */
	double CalculateNZCommand(const VECTOR3& velocity, double predRange, double curAlt, double DeltaT) const;
	/**
	 * Returns commanded speedbrake angle in degrees.
	 * Currently implements older version of speedbrake algorithm.
	 */
	double CalculateSpeedbrakeCommand(double predRange, double DeltaT);

	/**
	 * Called every time the target runway is changed.
	 * Updates internal variables used for calculations.
	 */
	void InitializeRunwayData();
	/**
 	 * Returns position relative to runway
 	 * \note X=distance from runway (-ve) Y=offset from centerline Z=height (-ve)
 	 */
	VECTOR3 GetRunwayRelPos() const;

	/**
	 * Calculates range and azimuth error to selected runway.
	 * /param range updated with range to rwy [m]
	 * /param delaz updated with azimuth error to rwy threshold [deg]
	 */
	void CalculateRangeAndDELAZ(double& Range, double& delaz);

	void LoadLandingSiteList();

	void PaintHORIZSITDisplay(vc::MDU* pMDU) const;
};

};

#endif
