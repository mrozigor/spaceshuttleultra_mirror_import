#ifndef OMSBURNSOFTWARE_H_874C017A_A729_44E5_A7BD_E4126667306F
#define	OMSBURNSOFTWARE_H_874C017A_A729_44E5_A7BD_E4126667306F
#pragma once

#include "SimpleGPCSoftware.h"
#include "StateVectorPropagator.h"
#include "discsignals.h"

namespace dps
{

class OMSBurnSoftware : public SimpleGPCSoftware
{
	int OMS; //0=BOTH, 1=LEFT, 2=RIGHT, 3=RCS
	double tig; // TIG in seconds 
	double TIG[4]; // day,hour,min,sec
	double IgnitionTime; //MET at ignition
	bool BurnInProg, BurnCompleted;
	double WT;
	VECTOR3 PEG7, DeltaV;
	double C1, C2, HT, ThetaT; // PEG4 Targets
	VECTOR3 Trim; // 0=P, 1=LY, 2=RY
	double TV_ROLL;
	double DeltaVTot;
	double BurnTime;
	VECTOR3 VGO;
	double vgoTot;
	bool MnvrLoad, MnvrExecute, MnvrToBurnAtt;
	VECTOR3 BurnAtt;
	//double OMSGimbal[2][2]; //0=LOMS/PITCH, 1=ROMS/YAW

	//ORBITPARAM oparam;
	//ELEMENTS el;
	double ApD, ApT, PeD, PeT;
	double lastUpdateSimTime;
	StateVectorPropagator propagator;

	DiscOutPort omsEngineCommand[2], omsPitchCommand[2], omsYawCommand[2];
	DiscInPort omsPitchGimbal[2], omsYawGimbal[2];

	OrbitDAP* pOrbitDAP;
public:
	OMSBurnSoftware(SimpleGPCSystem* _gpc);
	virtual ~OMSBurnSoftware();

	virtual void Realize();

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);

	virtual bool OnMajorModeChange(unsigned int newMajorMode);
	virtual bool ItemInput(int spec, int item, const char* Data);
	virtual bool ExecPressed(int spec);
	virtual bool OnPaint(int spec, vc::MDU* pMDU) const;

	virtual bool OnParseLine(const char* keyword, const char* value);
	virtual void OnSaveState(FILEHANDLE scn) const;
private:
	void LoadManeuver();
};

};

#endif