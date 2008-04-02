#ifndef __SPACE_SHUTTLE_ULTRA_KUBAND_SYSTEM_H_INCLUDED___
#define __SPACE_SHUTTLE_ULTRA_KUBAND_SYSTEM_H_INCLUDED___

#include "Atlantis.h"
#include "AtlantisSubsystem.h"

class KuBandSystem: public AtlantisSubsystem
{
	unsigned short usGPCMode[2];
	double alpha_angle[2];
	double beta_angle[2];
	double fDeployPos[2];
	AnimState DeployState[2];
	AnimState GimbalLockState[2];
	double fGimbalLockState[2];
	bool bHeatersOn[2];
public:
	virtual double GetAzimuth();
	virtual bool SetSlew(double fAzimuth, double fElevation);
	virtual double GetSignalStrength();
	KuBandSystem(SubsystemDirector* _director);
	virtual ~KuBandSystem();
	virtual bool ParseLine(const char* line);
	virtual void SaveState(FILEHANDLE scn);
	virtual void PreStep(double fSimT, double fDeltaT);
	virtual void PostStep(double fSimT, double fDeltaT);
	virtual void Propagate(double fSimT, double fDeltaT);
};

#endif