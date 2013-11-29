#ifndef __SPACE_SHUTTLE_ULTRA_KUBAND_SYSTEM_H_INCLUDED___
#define __SPACE_SHUTTLE_ULTRA_KUBAND_SYSTEM_H_INCLUDED___

#include "Atlantis.h"
#include "AtlantisSubsystem.h"

namespace comm {

/**
 * Structural and mechanical model of the deployed assembly.
 */
class DeployedAssembly: public AtlantisSubsystem
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
	virtual bool SetSlew(double fAlpha, double fBeta);
	virtual double GetSignalStrength();
	virtual int GetAlphaResolverAngle() const;
	virtual int GetBetaResolverAngle() const;

	DeployedAssembly(AtlantisSubsystemDirector* _director);
	virtual ~DeployedAssembly();
	virtual bool ParseLine(const char* line);
	virtual void SaveState(FILEHANDLE scn);
	virtual void PreStep(double fSimT, double fDeltaT);
	virtual void PostStep(double fSimT, double fDeltaT);
	virtual void Propagate(double fSimT, double fDeltaT);
	
};

};

#endif