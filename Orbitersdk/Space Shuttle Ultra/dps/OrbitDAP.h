#ifndef ORBITDAP_H_A2BBDD5A_0C8C_41E5_B501_2EF4028378C5
#define ORBITDAP_H_A2BBDD5A_0C8C_41E5_B501_2EF4028378C5
#pragma once

#include "SimpleGPCSoftware.h"
#include "StateVectorSoftware.h"
#include <PIDControl.h>
#include "../Atlantis.h"
#include "discsignals.h"

namespace dps
{

const double RHC_SOFT_STOP = 0.75;
const double RHC_DETENT = 0.01;

struct AttManeuver
{
	typedef enum {MNVR, TRK, ROT} TYPE; // at the moment, ROT is not supported
	bool IsValid;
	MATRIX3 tgtMatrix; // target attitude (rotation matrix) in right-handed frame (LVLH or M50, as appropriate)
	TYPE Type; // at the moment, ROT is not supported
};

/**
 * Controls shuttle's attitude during orbital flight.
 * Also performs OMS TVC.
 * All calculations are done using right-handed matrices in M50 frame.
 */
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
	bool NullingRates[3];
	bool RotPulseInProg[3];
	bool TransPulseInProg[3];
	VECTOR3 TransPulseDV; //negative DV for pulses along negative axes

	VECTOR3 degReqdRates;
	VECTOR3 degNullRates; // rates (in body axis frame) required to null rotation
	MATRIX3 attErrorMatrix;
	enum {MNVR_OFF, MNVR_IN_PROGRESS, MNVR_COMPLETE} ManeuverStatus;
	// ActiveManeuver is whatever attitude is currently being held (in AUTO, INRTL or LVLH)
	// Cur/FutManeuver are maneuvers loaded using UNIV PTG; in AUTO mode, Active and Cur maneuvers are always (check this) the same
	AttManeuver ActiveManeuver, CurManeuver, FutManeuver;
	double FutMnvrStartTime; // MET when future loaded maneuver starts
	double mnvrCompletionMET; // MET when current maneuver will be complete
	double lastUpdateTime; // time when null rates and final inertial attitude was last estimated for TRK maneuver

	//VECTOR3 radCurrentOrbiterAtt;
	MATRIX3 curM50Matrix;
	VECTOR3 radAngularVelocity, degAngularVelocity; // in Orbiter body axis frame
	//VECTOR3 GlobalPos;
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
	
	StateVectorSoftware* pStateVector;
public:
	OrbitDAP(SimpleGPCSystem* pGPC);
	virtual ~OrbitDAP();

	void UseOMSTVC(CONTROL_MODE Mode, const VECTOR3& Trim);
	void UseRCS();

	DAP_CONTROL_MODE GetDAPMode() const;
	
	/**
	 * Starts maneuver to INRTL attitude.
	 * Maneuver will start as soon as AUTO is selected.
	 * Currently only needed for OMSBurnSoftware class (to manuver to burn att).
	 * \param degINRTLAtt Attitude (in degrees) in M50 frame in standard PRY order
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
	/**
	 * Updates variables with current attitude data.
	 * Data from Orbitersim is converted to appropriate right-handed frame.
	 */
	void GetAttitudeData();

	void PaintUNIVPTGDisplay(vc::MDU* pMDU) const;
	void PaintDAPCONFIGDisplay(vc::MDU* pMDU) const;

	void LoadCurLVLHManeuver(const MATRIX3& tgtMatrixLVLH);
	void LoadFutLVLHManeuver(const MATRIX3& tgtMatrixLVLH);
	void LoadCurINRTLManeuver(const MATRIX3& tgtMatrixM50);
	void LoadFutINRTLManeuver(const MATRIX3& tgtMatrixM50);
	void StartCurManeuver();
	void StartManeuver(const MATRIX3& tgtAtt, AttManeuver::TYPE type);

	/**
	 * Determines rates due to RHC input.
	 * Returns true if RHC is out of detent.
	 */
	bool GetRHCRequiredRates();
	/**
	 * Fires translation thrusters based on THC input
	 */
	void HandleTHCInput(double DeltaT);

	/**
	 * Calculates required rotation rates based on rotation around Euler axis.
	 * Does not compensate for rotation of reference frame.
	 */
	void CalcEulerAxisRates();
	/**
	 * Calculates rotation rates based on errors in each individual axis.
	 * Adjusts rates to compenstate for rotation of reference frame.
	 * \param degNullRatesLocal Rotation rate (in Orbiter body frame) required to maintain constant attitude.
	 */
	void CalcMultiAxisRates(const VECTOR3& degNullRatesLocal);
	
	void UpdateNullRates();

	void SetRates(const VECTOR3 &degRates, double DeltaT);
	//void OMSTVC(const VECTOR3& degAngleErrors);
	void OMSTVC(const VECTOR3 &Rates, double SimDT);
	/**
	 * Sets OMS gimbal angles.
	 * Returns false if angles passed to functions are out of range.
	 * If angles are out of range, uses maximum possible gimbal angle.
	 */
	bool GimbalOMS(SIDE side, double pitch, double yaw);

	void StartPCT();
	void StopPCT();
	void PCTControl(double SimT);

	/**
	 * Updates DAP parameters with values from currently selected DAP configuration
	 */
	void UpdateDAPParameters();

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

	double CalcManeuverCompletionTime(const MATRIX3& curM50Matrix, const MATRIX3& tgtLVLHMatrix, const MATRIX3& curLVLHMatrix, double degOrbitalRate) const;

	/*** Vector/Matrix manipulation functions ***/
	/**
	 * Returns rotation matrix to convert from LVLH to M50 frame
	 */
	MATRIX3 GetCurrentLVLHRefMatrix() const;
	/**
	 * Returns rotation matrix representing current attitude in LVLH frame
	 */
	MATRIX3 GetCurrentLVLHAttMatrix() const;
};

};

#endif
