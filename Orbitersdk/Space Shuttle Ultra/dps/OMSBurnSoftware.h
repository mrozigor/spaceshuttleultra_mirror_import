#ifndef OMSBURNSOFTWARE_H_874C017A_A729_44E5_A7BD_E4126667306F
#define	OMSBURNSOFTWARE_H_874C017A_A729_44E5_A7BD_E4126667306F
#pragma once

#include "SimpleGPCSoftware.h"
#include "StateVectorSoftware.h"
#include "discsignals.h"
#include <EngConst.h>
#include <PEG4Targeting.h>

namespace dps
{
const double EI_ALT = 400e3/MPS2FPS; // 400,000 feet

class OMSBurnSoftware : public SimpleGPCSoftware
{
	int OMS; //0=BOTH, 1=LEFT, 2=RIGHT, 3=RCS
	double tig; // TIG in seconds 
	double TIG[4]; // day,hour,min,sec
	double IgnitionTime; //MET at ignition
	bool BurnInProg, BurnCompleted;
	double WT;
	VECTOR3 PEG7; // DV in fps (values entered on CRT display; LVLH at TIG frame)
	VECTOR3 DeltaV; // DV in m/s (LVLH at TIG frame)
	double C1, C2, HT, ThetaT; // PEG4 Targets
	VECTOR3 Trim; // 0=P, 1=LY, 2=RY
	int TV_ROLL;
	double DeltaVTot;
	double BurnTime;
	VECTOR3 VGO; // fps, body vector frame (VGO values displayed on CRT display)
	VECTOR3 VGO_Global; // m/s, Orbitersim global frame
	double vgoTot;
	bool MnvrLoad, MnvrExecute, MnvrToBurnAtt;
	bool bShowTimer;
	VECTOR3 BurnAtt;
	//double OMSGimbal[2][2]; //0=LOMS/PITCH, 1=ROMS/YAW

	bool bCalculatingPEG4Burn;
	PEG4Targeting peg4Targeting;

	OBJHANDLE hEarth;
	//ORBITPARAM oparam;
	//ELEMENTS el;
	double ApD, ApT, PeD, PeT;
	double metAt400KFeet;
	double lastUpdateSimTime;
	double lastMET;
	StateVectorPropagator propagator;
	OMSBurnPropagator omsBurnPropagator;
	double tgtApD, tgtPeD;

	DiscOutPort omsEngineCommand[2], omsPitchCommand[2], omsYawCommand[2];
	DiscInPort omsPitchGimbal[2], omsYawGimbal[2];

	OrbitDAP* pOrbitDAP;
	StateVectorSoftware* pStateVector;
public:
	OMSBurnSoftware(SimpleGPCSystem* _gpc);
	virtual ~OMSBurnSoftware();

	virtual void Realize();

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);

	virtual bool OnMajorModeChange(unsigned int newMajorMode);
	virtual bool ItemInput(int spec, int item, const char* Data, bool &IllegalEntry );
	virtual bool ExecPressed(int spec);
	virtual bool OnPaint(int spec, vc::MDU* pMDU) const;

	virtual bool OnParseLine(const char* keyword, const char* value);
	virtual void OnSaveState(FILEHANDLE scn) const;

	/**
	 * Sets maneuver data (TIG and DV).
	 * Used for other programs (i.e. Orbit Tgt) to set burn data.
	 * \param maneuverTIG TIG of burn (MET, in seconds)
	 * \param maneuverDV DeltaV of burn in m/s
	 */
	void SetManeuverData(double maneuverTIG, const VECTOR3& maneuverDV);

	VECTOR3 GetAttitudeCommandErrors() const;
private:
	void StartBurn();
	void TerminateBurn();
	
	void StartCalculatingPEG4Targets();
	void LoadManeuver(bool calculateBurnAtt = true);
	void CalculateEIMinus5Att(VECTOR3& degAtt) const;
	void UpdateOrbitData();
	void UpdateBurnPropagator();
};

};

#endif
