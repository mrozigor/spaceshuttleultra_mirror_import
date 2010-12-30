#ifndef ORBITDAP_H_A2BBDD5A_0C8C_41E5_B501_2EF4028378C5
#define ORBITDAP_H_A2BBDD5A_0C8C_41E5_B501_2EF4028378C5
#pragma once

#include "SimpleGPCSoftware.h"
#include "../PIDControl.h"
#include "../Atlantis.h"
#include "discsignals.h"

namespace dps
{

struct AttManeuver
{
	bool bValid;
	VECTOR3 TargetAttM50;
	MATRIX3 LVLHTgtOrientationMatrix;
	enum {MNVR, TRK, ROT} Type; // at the moment, ROT is not supported

	AttManeuver& operator = (const AttManeuver& rhs)
	{
		bValid = rhs.bValid;
		TargetAttM50 = rhs.TargetAttM50;
		LVLHTgtOrientationMatrix = rhs.LVLHTgtOrientationMatrix;
		Type = rhs.Type;
		return *this;
	}
};

class OrbitDAP : public SimpleGPCSoftware
{
public:
	typedef enum {RCS, LEFT_OMS, RIGHT_OMS, BOTH_OMS} CONTROL_MODE;
	typedef enum {AUTO, INRTL, LVLH, FREE} DAP_MODE;
private:
	PIDControl OMSTVCControlP, OMSTVCControlY, OMSTVCControlR;
	VECTOR3 OMSTrim;
	CONTROL_MODE ControlMode;

	// ActiveManeuver is whatever attitude is currently being held (in AUTO, INRTL or LVLH)
	// Cur/FutManeuver are maneuvers loaded using UNIV PTG; in AUTO mode, Active and Cur maneuvers are always (check this) the same
	AttManeuver ActiveManeuver, CurManeuver, FutManeuver;
	double FutMnvrStartTime; // MET when future loaded maneuver starts

	VECTOR3 radAngularVelocity;

	DiscInPort DAPMode[4];
	DiscOutPort POMSGimbalCommand[2], YOMSGimbalCommand[2];
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
	
	virtual void Realize();

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);

	virtual bool OnMajorModeChange(unsigned int newMajorMode);
	//virtual bool ItemInput(int spec, int item, const char* Data);
	//virtual bool ExecPressed(int spec);
	//virtual bool OnPaint(int spec, vc::MDU* pMDU) const;

	//virtual bool OnParseLine(const char* keyword, const char* value);
	//virtual void OnSaveState(FILEHANDLE scn) const;
private:
	void GetAttitudeData();

	void LoadCurLVLHManeuver(const MATRIX3& RotMatrix);
	void LoadCurINRTLManeuver(const VECTOR3& degTargetAtt);

	//void OMSTVC(const VECTOR3& degAngleErrors);
	void OMSTVC(const VECTOR3 &Rates, double SimDT); // temporary; used unitl attitude control code is moved to this class
	/**
	 * Sets OMS gimbal angles.
	 * Returns false if angles passed to functions are out of range.
	 * If angles are out of range, uses maximum possible gimbal angle.
	 */
	bool GimbalOMS(SIDE side, double pitch, double yaw);
};

};

#endif