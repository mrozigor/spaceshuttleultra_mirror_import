#ifndef AEROJETDAP_H_77EB3AE1_0A62_4181_8370_0F9995B10FEF
#define AEROJETDAP_H_77EB3AE1_0A62_4181_8370_0F9995B10FEF
#pragma once

#include <PIDControl.h>
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
const double OGS_AIMPOINT_NOM = -7500/MPS2FPS;
const double OGS_AIMPOINT_CLOSE = -6500/MPS2FPS;
//const double OGS_AIMPOINT = -7500/MPS2FPS;
const double AL_GS = tan(-20.0*RAD);
const double Y_AL_INTERCEPT = 10018.0/MPS2FPS;
//const double X_AL_INTERCEPT = OGS_AIMPOINT + Y_AL_INTERCEPT/AL_GS;

//const double HAC_CENTER_X = OGS_AIMPOINT - 33020.0/MPS2FPS;
const double FINAL_RADIUS = 14000.0/MPS2FPS; // center of HAC in Y direction is +/- FINAL_RADIUS
const double R1 = 0.0;
const double R2 = 0.093/MPS2FPS;

const int NZ_VALUE_COUNT = 5;
const double NZ_UPDATE_INTERVAL = 0.1;


class RHC_SOP;
class RPTA_SOP;
class SBTC_SOP;


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
	typedef enum {NOM, SHORT, ELS} SB_CONTROL_LOGIC;
	typedef enum {AUTO, MAN_CDR, MAN_PLT} SPEEDBRAKE_STATE;

	class LandingSiteData
	{
		double radPriLat, radPriLong;
		double radSecLat, radSecLong;
		double degPriHeading;
		double degSecHeading;
		std::string strPri, strSec;
		double priRwyLength, priRwyWidth; // length and width in meters
		double secRwyLength, secRwyWidth; // length and width in meters
		double priRwyAMSL;// meters above mean sea level
		double secRwyAMSL;// meters above mean sea level

	public:
		LandingSiteData(double _radPriLat, double _radPriLong, double _priRwyAMSL, double _radSecLat, double _radSecLong, double _secRwyAMSL, double _degPriHeading, double _degSecHeading, const char* pri, const char* sec, double _rwyLength = 15000.0/MPS2FPS, double _rwyWidth = 100.0)
			: radPriLat(_radPriLat), radPriLong(_radPriLong), radSecLat(_radSecLat), radSecLong(_radSecLong),
			degPriHeading(_degPriHeading), degSecHeading(_degSecHeading),
			strPri(pri), strSec(sec),
			priRwyLength(_rwyLength), priRwyWidth(_rwyWidth),
			secRwyLength(_rwyLength), secRwyWidth(_rwyWidth),
			priRwyAMSL(_priRwyAMSL), secRwyAMSL(_secRwyAMSL)
		{
		}

		~LandingSiteData()
		{
		}

		void SetPriRunwayParameters(double length, double width)
		{
			priRwyLength = length;
			priRwyWidth = width;
		}
		void SetSecRunwayParameters(double length, double width)
		{
			secRwyLength = length;
			secRwyWidth = width;
		}

		void GetRwyPosition(bool pri, double& radLat, double& radLong, double &AMSL ) const
		{
			if(pri) {
				radLat = radPriLat;
				radLong = radPriLong;
				AMSL = priRwyAMSL;
			}
			else {
				radLat = radSecLat;
				radLong = radSecLong;
				AMSL = secRwyAMSL;
			}
		}

		double GetRwyLength(bool pri) const
		{
			if(pri) return priRwyLength;
			else return secRwyLength;
		}

		double GetRwyWidth(bool pri) const
		{
			if(pri) return priRwyWidth;
			else return secRwyWidth;
		}

		double GetRwyHeading(bool pri) const
		{
			if(pri) return degPriHeading;
			else return degSecHeading;
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

	bool bWOW;
	bool bWONG;

	// Orbiter data
	double OrbiterMass;
	VECTOR3 PMI;
	VECTOR3 RCSTorque;

	//PIDControl AOA_ElevonPitch; // converts AOA error to elevon command
	PIDControl NZ_PitchRate; // converts NZ error to pitch rate
	PIDControl ElevonPitch; // converts angle error (AOA or pitch, in degrees) to elevon command
	//PIDControl Rate_ElevonPitch; // converts pitch rate error (in degrees) to elevon command
	//PIDControl Pitch_ElevonPitch; // converts pitch angle error (in degrees) to elevon command
	PIDControl Roll_AileronRoll; // converts roll angle error (in degrees) to aileron command
	PIDControl Yaw_RudderYaw; // converts roll angle error (in degrees) to aileron command
	PIDControl QBar_Speedbrake; // converts qbar error (in kPa) to speedbrake command; used in TAEM phase
	PIDControl Vel_Speedbrake; // converts speed error (KEAS) to speedbrake command; used in A/L phase
	//PIDControl BodyFlap;
	
	DiscInPort CDRPitchAuto;
	DiscInPort PLTPitchAuto;
	DiscInPort CDRPitchCSS;
	DiscInPort PLTPitchCSS;
	DiscInPort CDRRollYawAuto;
	DiscInPort PLTRollYawAuto;
	DiscInPort CDRRollYawCSS;
	DiscInPort PLTRollYawCSS;

	DiscOutPort CDRPitchAutoLT;
	DiscOutPort PLTPitchAutoLT;
	DiscOutPort CDRPitchCSSLT;
	DiscOutPort PLTPitchCSSLT;
	DiscOutPort CDRRollYawAutoLT;
	DiscOutPort PLTRollYawAutoLT;
	DiscOutPort CDRRollYawCSSLT;
	DiscOutPort PLTRollYawCSSLT;
	
	DiscInPort CDR_SPDBK_THROT;
	DiscInPort PLT_SPDBK_THROT;

	DiscOutPort CDR_SPDBK_THROT_AUTO_LT;
	DiscOutPort CDR_SPDBK_THROT_MAN_LT;
	DiscOutPort PLT_SPDBK_THROT_AUTO_LT;
	DiscOutPort PLT_SPDBK_THROT_MAN_LT;

	DiscOutPort ThrusterCommands[3];
	//DiscOutPort LeftElevonCommand, RightElevonCommand;
	DiscOutPort ElevonCommand, AileronCommand, RudderCommand;

	DiscInPort HUDPower[2];
	DiscInPort pHUDDCLT[2];
	bool dclt_sw_on[2];
	int declutter_level[2];

	/**
	 * If true, the FCS pitch channel is in AUTO mode. If false, the FCS pitch channel is in CSS mode.
	 */
	bool AutoFCSPitch;
	/**
	 * If true, the FCS roll channel is in AUTO mode. If false, the FCS roll channel is in CSS mode.
	 */
	bool AutoFCSRoll;

	SPEEDBRAKE_STATE SpeedbrakeState;
	double AutoSpeedbrakeCommand;

	double tCSS;
	double tGear;
	
	bool ThrustersActive[3]; // indicates if each set of thrusters (pitch, yaw, roll) is active
	bool AerosurfacesActive[3];

	// attitude and rates relative to velocity vector (AOA, sideslip, bank)
	// values in degrees or deg/sec
	VECTOR3 degCurrentAttitude;
	VECTOR3 degCurrentRates;
	VECTOR3 degTargetRates; // used in aerosurface control to ramp rotation rates
	bool RotatingAxis[3]; // indicates if Orbiter is maneuvering around an axis (RCS)
	
	OBJHANDLE hEarth;

	RHC_SOP* pRHC_SOP;
	RPTA_SOP* pRPTA_SOP;
	SBTC_SOP* pSBTC_SOP;

	ENTRY_GUIDANCE_MODE EntryGuidanceMode;
	VECTOR3 HAC_Center;
	double HAC_Lat, HAC_Long;
	double refDrag; // DREFP
	double referenceDrag23; // D23
	double constDragLevel; // T2
	double constDragStartVel; // VCG
	double lastTgtAltitude;
	double lastRefVSpeed;
	Averager tgtAltAveraging;
	Averager vspeedAveraging;
	Averager vaccAveraging;
	double vspeedUpdateSimT, vaccUpdateSimT;
	double tgtBankSign; // changed at each roll reversal
	bool performedFirstRollReversal;

	/** Variables used by TAEM guidance **/
	TAEM_GUIDANCE_MODE TAEMGuidanceMode;
	HAC_DIRECTION HACDirection;
	HAC_SIDE HACSide;
	SB_CONTROL_LOGIC SBControlLogic;
	VECTOR3 RwyStart_EarthLocal, RwyEnd_EarthLocal; // start and end points of runway in Orbitersim Earth local frame
	// Runway frame: Same center as Orbitersim Earth local frame, but rotated so x axis is along rwy centerline and z axis corresponds to altitude (right-handed frame)
	MATRIX3 RwyRotMatrix; // converts from Orbiter Earth local frame to runway frame
	VECTOR3 RwyPos; // position of runway in runway frame
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

	double OGS_AIMPOINT;
	double X_AL_INTERCEPT;
	double HAC_CENTER_X;

	/**
	 * Time to HAC (seconds).
	 */
	double TimeToHAC;

	/**
	 * Vehicle distance to center of HAC (m).
	 */
	double DistanceToHACCenter;

	/**
	 * Data for ENTRY TRAJ and VERT SIT displays.
	 */
	double ET_MachHistory[5];
	double ET_RangeHistory[5];
	double ET_AltitudeHistory[5];
	double ET_History_updatetime;
	double ET_Mach;
	double ETVS_Range;
	double ETVS_Altitude;

	/** Output guidance variables **/
	double NZCommand; // MM305 output
	double TargetBank;
	double NZErr;

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

	/**
	 * Gets current state of pitch commanding.
	 * @return	true if AUTO, false if CSS
	 */
	bool GetAutoPitchState( void ) const;

	/**
	 * Gets current state of roll/yaw commanding.
	 * @return	true if AUTO, false if CSS
	 */
	bool GetAutoRollYawState( void ) const;

	/**
	 * Gets current state of speedrake commanding.
	 * @return	true if AUTO, false if MAN
	 */
	bool GetAutoSpeedbrakeState( void ) const;

	/**
	 * Gets current auto speedrake command.
	 * @return	auto speedrake command, bewteen 0% and 100%
	 */
	double GetAutoSpeedbrakeCommand( void ) const;

	/**
	 * Gets current target runway.
	 * @return	string with runway name and number
	 */
	const std::string& GetSelectedRunway( void ) const;

	/**
	 * Gets current range to selected runway (NM).
	 * @return	range to runway (NM)
	 */
	double GetRangeToRunway( void ) const;

	/**
	 * Gets current delta azimuth to WP-1, in degrees, with range -180 to +180, and >0 for runway left of vehicle.
	 * @return	delta azimuth (degrees)
	 */
	double GetdeltaAZ( void ) const;
	
	/**
	 * Gets current state of vehicle in relation to the HAC.
	 * @return	true if vehicle is on, or past HAC
	 */
	bool GetOnHACState( void ) const;

	/**
	 * Gets current state of Prefinal guidance.
	 * @return	true if Prefinal guidance on
	 */
	bool GetPrefinalState( void ) const;
	
	/**
	 * Gets current state of Approach and Land guidance.
	 * @return	true if A/L guidance on
	 */
	bool GetApproachAndLandState( void ) const;

	/**
	 * Gets current vertical accelaration (fps^2).
	 * @return	vertical accelaration (fps^2)
	 */
	double GetVacc( void ) const;

	/**
	 * Gets WOW indication.
	 * @return	true if WOW indication has been set
	 */
	bool GetWOW( void ) const;

	/**
	 * Gets current vehicle attitude errors (deg).
	 * @return	attitude errors (deg) (x=pitch, y=yaw, z=roll)
	 */
	VECTOR3 GetAttitudeErrors( void ) const;

	/**
	 * Gets current vehicle Y-runway error (ft).
	 * @return	Y-runway error (ft)
	 */
	double GetYRunwayPositionError( void ) const;

	/**
	 * Gets time to HAC (seconds).
	 * @return	time to HAC (seconds)
	 */
	double GetTimeToHAC( void ) const;

	/**
	 * Gets vehicle distance to center of HAC (NM).
	 * @return	distance to center of HAC (NM)
	 */
	double GetDistanceToHACCenter( void ) const;

	/**
	 * Gets vehicle HAC radial error (ft).
	 * @return	HAC radial error (ft)
	 */
	double GetHACRadialError( void ) const;

	/**
	 * Gets vehicle NZ error (g).
	 * @return	NZ error (g)
	 */
	double GetNZError( void ) const;

	/**
	 * Gets vehicle HAC turn angle (deg).
	 * @return	HAC turn angle (deg)
	 */
	double GetHTA( void ) const;

	/**
	 * Gets vehicle distance to glide slope (ft).
	 * @return	distance to glide slope (ft) (>0 -> above)
	 */
	double GetGlideSlopeDistance( void ) const;

	/**
	 * Gets vehicle NZ (g).
	 * @return	NZ (g)
	 */
	double GetNZ( void ) const;

	/**
	 * Returns current delta azimuth limit.
	 * @return	delta azimuth limit (DEG)
	 */
	double GetdeltaAZLimit( double mach ) const;

	/**
 	 * Returns heading of selected runway
 	 * @return	heading of selected runway (deg)
 	 */
	double GetSelectedRunwayHeading( void ) const;
private:
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
	 * Returns normal bank angle at high altitude (when atmosphere is thin and there is no lift)
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
	 * Calculates thruster command (1.0, -1.0 or 0.0) to set desired rotation rate
	 */
	double GetThrusterCommand(AXIS axis, double DeltaT);

	/**
	 * Uses lookup table to calculate target AOA for current mach number.
	 */
	double CalculateTargetAOA(double mach) const;
	/**
	 * Calculates target bank in degrees
	 */
	double CalculateTargetBank(double mach, double targetAOA, double DeltaT, double SimT);
	/**
	 * Uses lookup table to calculate GALR (Gain for Ailerons in Roll Channel)
	 * Lookup table values from NASA Entry DAP Workbook 21002.
	 */
	double CalculateGALR(double mach) const;
	/**
	 * Calculates required roll/yaw rates around body axes to achieve desired bank rate around stability axes (i.e. velocity vector)
	 * Uses simplified version of roll/ywa channels described in NASA Entry DAP Workbook 21002
	 */
	void CalculateTargetRollYawRates(double mach, double radAOA, double degTgtBankRate, double& degTgtRollRate, double& degTgtYawRate) const;

	void SetAerosurfaceCommands(double DeltaT);
	/**
	 * Uses either AUTO guidance or SBTC command to set speedbrake position.
	 * Checks for manual speedbrake takeover.
	 */
	void SetSpeedbrakeCommand(double range, double DeltaT);
	/**
	 * Uses RHC input to set desired rates.
	 * Return target rates (in deg/sec) in appropriate axis
	 */
	double CSSPitchInput(double DeltaT);
	double CSSRollInput(double DeltaT);

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
	 * /param delaz updated with azimuth error to WP1 [deg], range [-180;+180], >0 for runway left of vehicle
	 */
	void CalculateRangeAndDELAZ(double& Range, double& delaz);

	void LoadLandingSiteList();

	void PaintHORIZSITDisplay(vc::MDU* pMDU) const;
	void PaintENTRYTRAJ1Display( vc::MDU* pMDU ) const;
	void PaintENTRYTRAJ2Display( vc::MDU* pMDU ) const;
	void PaintENTRYTRAJ3Display( vc::MDU* pMDU ) const;
	void PaintENTRYTRAJ4Display( vc::MDU* pMDU ) const;
	void PaintENTRYTRAJ5Display( vc::MDU* pMDU ) const;
	void PaintVERTSIT1Display( vc::MDU* pMDU ) const;
	void PaintVERTSIT2Display( vc::MDU* pMDU ) const;
};

};

#endif
