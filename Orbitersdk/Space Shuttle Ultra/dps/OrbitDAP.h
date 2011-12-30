#ifndef ORBITDAP_H_A2BBDD5A_0C8C_41E5_B501_2EF4028378C5
#define ORBITDAP_H_A2BBDD5A_0C8C_41E5_B501_2EF4028378C5
#pragma once

#include "SimpleGPCSoftware.h"
#include "../PIDControl.h"
#include "../Atlantis.h"
#include "discsignals.h"

namespace dps
{

const double RHC_SOFT_STOP = 0.75;
const double RHC_DETENT = 0.01;

struct AttManeuver
{
	bool IsValid;
	VECTOR3 radTargetAttOrbiter;
	VECTOR3 radTargetLVLHAtt; // only needed for LVLH frame
	//MATRIX3 LVLHTgtOrientationMatrix;
	enum {MNVR, TRK, ROT} Type; // at the moment, ROT is not supported
};

class OrbitDAP : public SimpleGPCSoftware
{
public:
	typedef enum {RCS, LEFT_OMS, RIGHT_OMS, BOTH_OMS} CONTROL_MODE;
	typedef enum {PRI, ALT, VERN} DAP_MODE;
	typedef enum {A, B} DAP_SELECT;
	typedef enum {AUTO, INRTL, LVLH, FREE} DAP_CONTROL_MODE;
	typedef enum {DISC_RATE, ROT_PULSE} ROT_MODE;
	typedef enum {NORM, TRANS_PULSE} TRANS_MODE;
private:
	PIDControl OMSTVCControlP, OMSTVCControlY, OMSTVCControlR;
	VECTOR3 OMSTrim;
	CONTROL_MODE ControlMode;

	DAP_MODE DAPMode;
	DAP_SELECT DAPSelect;
	DAP_CONTROL_MODE DAPControlMode;
	ROT_MODE RotMode[3];
	TRANS_MODE TransMode[3]; // 0=X, 1=Y, 2=Z

	int editDAP; // -1=None, 0=A, 1=B
	DAPConfig DAPConfiguration[3]; //0=A, 1=B, 2=Edit

	// values change depending on DAP mode selected
	// initialized in Realize() function
	double degRotPulse, TransPulse;
	double degRotRate, degAttDeadband, degRateDeadband;

	bool RotatingAxis[3];
	bool RotPulseInProg[3];
	bool TransPulseInProg[3];
	VECTOR3 TransPulseDV; //negative DV for pulses along negative axes

	VECTOR3 degReqdRates;
	MATRIX3 attErrorMatrix;
	enum {MNVR_OFF, MNVR_STARTING, MNVR_IN_PROGRESS, MNVR_COMPLETE} ManeuverStatus;
	// ActiveManeuver is whatever attitude is currently being held (in AUTO, INRTL or LVLH)
	// Cur/FutManeuver are maneuvers loaded using UNIV PTG; in AUTO mode, Active and Cur maneuvers are always (check this) the same
	AttManeuver ActiveManeuver, CurManeuver, FutManeuver;
	double FutMnvrStartTime; // MET when future loaded maneuver starts
	double mnvrCompletionMET; // MET when current maneuver will be complete
	double lastUpdateTime; // time when final inertial attitude was last estimated for TRK maneuver

	VECTOR3 radCurrentOrbiterAtt;
	VECTOR3 radAngularVelocity, degAngularVelocity;
	VECTOR3 GlobalPos;
	double OrbiterMass;
	VECTOR3 PMI;
	VECTOR3 Torque;
	
	bool bFirstStep;
	double lastStepDeltaT;

	// values used in UNIV PTG to store attitude maneuvers
	int START_TIME[4];
	VECTOR3 MNVR_OPTION;
	int TGT_ID, BODY_VECT;
	double P, Y, OM;

	VECTOR3 CUR_ATT, REQD_ATT, ATT_ERR; // attitudes in degrees in M50 frame

	//PCT
	//bool PostContactThrusting[2]; //0=armed, 1=active
	//bool PCTArmed, PCTActive;
	bool PCTActive;
	double PCTStartTime;

	bool PBI_state[24];
	DiscInPort PBI_input[24];
	DiscOutPort PBI_output[24];
	/*DiscInPort DAPSelect[2]; // A or B
	DiscInPort DAPMode[3]; // PRI, ALT, VERN
	DiscInPort DAPControlMode[4]; // AUTO, INRTL, LVLH, FREE*/
	DiscInPort RHCInput[3];
	DiscInPort THCInput[3];
	DiscOutPort RotThrusterCommands[3];
	DiscOutPort TransThrusterCommands[3]; // 0=X, 1=Y, 2=Z
	DiscOutPort POMSGimbalCommand[2], YOMSGimbalCommand[2];
	DiscInPort PCTArmed;
	DiscInPort BodyFlapAuto; // used to trigger PCT
	DiscOutPort port_PCTActive[2]; // PBIs indicating is PCT is in progress
	//DiscOutPort port_PCTActive;
	DiscOutPort PitchAuto, RollYawAuto, PitchCSS, RollYawCSS; // make sure these PBIs are all OFF
public:
	OrbitDAP(SimpleGPCSystem* pGPC);
	virtual ~OrbitDAP();

	void UseOMSTVC(CONTROL_MODE Mode, const VECTOR3& Trim);
	void UseRCS();

	/**
	 * Starts maneuver to LVLH attitude.
	 * Maneuver will start as soon as AUTO is selected.
	 * Currently only needed for OMSBurnSoftware class (to manuver to burn att).
	 */
	void ManeuverToLVLHAttitude(const VECTOR3& degLVLHAtt);
	/**
	 * Starts maneuver to INRTL attitude.
	 * Maneuver will start as soon as AUTO is selected.
	 * Currently only needed for OMSBurnSoftware class (to manuver to burn att).
	 */
	void ManeuverToINRTLAttitude(const VECTOR3& degINRTLAtt);
	
	virtual void Realize();

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);

	virtual bool OnMajorModeChange(unsigned int newMajorMode);
	virtual bool ItemInput(int spec, int item, const char* Data);
	//virtual bool ExecPressed(int spec);
	virtual bool OnPaint(int spec, vc::MDU* pMDU) const;

	virtual bool OnParseLine(const char* keyword, const char* value);
	virtual void OnSaveState(FILEHANDLE scn) const;
private:
	void GetAttitudeData();

	void PaintUNIVPTGDisplay(vc::MDU* pMDU) const;
	void PaintDAPCONFIGDisplay(vc::MDU* pMDU) const;

	//void LoadCurLVLHManeuver(const MATRIX3& RotMatrix);
	void LoadCurLVLHManeuver(const VECTOR3& radTargetLVLHAtt);
	void LoadFutLVLHManeuver(const VECTOR3& radTargetLVLHAtt);
	void LoadCurINRTLManeuver(const VECTOR3& radTargetAttOrbiter);
	void LoadFutINRTLManeuver(const VECTOR3& radTargetAttOrbiter);
	void StartCurManeuver();
	void StartINRTLManeuver(const VECTOR3& radTargetAttOrbiter);
	//void StartLVLHManeuver(const MATRIX3& LVLHTgtMatrix);
	void StartLVLHManeuver(const VECTOR3& radTargetLVLHAtt);

	/**
	 * Determines rates due to RHC input.
	 * Returns true if RHC is out of detent.
	 */
	bool GetRHCRequiredRates();
	void HandleTHCInput(double DeltaT);

	void CalcEulerAxisRates();
	void CalcMultiAxisRates(const VECTOR3& degNullRatesLocal);

	void SetRates(const VECTOR3 &degRates, double DeltaT);
	//void OMSTVC(const VECTOR3& degAngleErrors);
	void OMSTVC(const VECTOR3 &Rates, double SimDT); // temporary; used unitl attitude control code is moved to this class
	/**
	 * Sets OMS gimbal angles.
	 * Returns false if angles passed to functions are out of range.
	 * If angles are out of range, uses maximum possible gimbal angle.
	 */
	bool GimbalOMS(SIDE side, double pitch, double yaw);

	void StartPCT();
	void StopPCT();
	void PCTControl(double SimT);

	void UpdateDAPParameters();
	//void UpdateTorqueValues();

	/**
	* Returns true if PBI on.
	* PBIs are numbered along rows, then columns.
	* First row is 1, 2, 3,..., 2nd row is 7, 8, etc.
	*/
	bool GetPBIState(int id) const;
	/**
	 * Handles DAP PBI being pressed.
	 */
	void ButtonPress(int id);

	double CalcManeuverCompletionTime(const VECTOR3& radTargetAttOrbiter, const VECTOR3& radNullRatesOrbiter) const;

	/*** Vector/Matrix manipulation functions ***/
	/**
	 * Returns current attitude (in RADIANS) in rotatin LVLH frame.
	 */
	VECTOR3 GetCurrentLVLHAttitude() const;
	VECTOR3 ConvertOrbiterAnglesToLocal(const VECTOR3 &radAngles) const;
	MATRIX3 ConvertLVLHAnglesToM50Matrix(const VECTOR3 &radAngles) const;
	MATRIX3 CalcPitchYawRollRotMatrix(const VECTOR3& radTargetAttOrbiter) const;
};

};

#endif