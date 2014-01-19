#ifndef ORBITTGTSOFTWARE_H_E031D277_4DA3_4827_9F8D_D2BB8BE2F2D8
#define ORBITTGTSOFTWARE_H_E031D277_4DA3_4827_9F8D_D2BB8BE2F2D8
#pragma once

#include "SimpleGPCSoftware.h"
#include "OMSBurnSoftware.h"
#include "StateVectorSoftware.h"
#include "LambertBurnTargeting.h"

namespace dps
{

/**
 * Struct for I-Load maneuvers.
 * Not used yet (I-loading maneuver data is not implemented)
 */
struct BurnTargetingData
{
	VECTOR3 finalOffset;
	double elevation;
	double T1_TIG; // relative to base time
	double transferTime; // time between T1 and T2

	BurnTargetingData& operator = (const BurnTargetingData& rhs) {
		// copy all values from other config into this one
		finalOffset = rhs.finalOffset;
		elevation = rhs.elevation;
		T1_TIG = rhs.T1_TIG;
		transferTime = rhs.transferTime;
		return *this;
	}
};

/**
 * GPC software for targeting rendezvous burns (solving Lambert/Gauss problem).
 * Implements SPEC 34 ORBIT TGT display.
 */
class OrbitTgtSoftware : public SimpleGPCSoftware
{
	// values displayed on SPEC 34 display
	double TIG_T1[4], TIG_T2[4], BASE_TIME[4]; // day,hour,min,sec
	double transferTime;
	VECTOR3 relPos_T1, relVel_T1;
	VECTOR3 relPos_T2;
	double elevation;
	//double dT_T2;
	VECTOR3 DeltaV;
	VECTOR3 t1Vel, t1Pos;

	bool bCalculatingT1Burn;
	LambertBurnTargeting burnTargeting;

	bool bMNVRDataCalculated;
	bool bValuesChanged; // indicates that values have been changed from I-Loaded defaults
	// data used for burn targeting
	BurnTargetingData targetData;

	OMSBurnSoftware* pOMSBurnSoftware;
	StateVectorSoftware* pStateVectorSoftware;
public:
	OrbitTgtSoftware(SimpleGPCSystem* _gpc);
	virtual ~OrbitTgtSoftware();

	virtual void Realize();

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);

	virtual bool OnMajorModeChange(unsigned int newMajorMode);
	virtual bool ItemInput(int spec, int item, const char* Data);
	virtual bool OnPaint(int spec, vc::MDU* pMDU) const;

	virtual bool OnParseLine(const char* keyword, const char* value);
	virtual void OnSaveState(FILEHANDLE scn) const;
private:
	void StartCalculatingT1Burn();
	void GetT1BurnData(const VECTOR3& targetEquVelocity);
};

};

#endif // ORBITTGTSOFTWARE_H_E031D277_4DA3_4827_9F8D_D2BB8BE2F2D8