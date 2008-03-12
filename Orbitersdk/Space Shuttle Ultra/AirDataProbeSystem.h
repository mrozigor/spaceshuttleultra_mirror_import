// AirDataProbeSystem.h: Schnittstelle für die Klasse AirDataProbeSystem.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIRDATAPROBESYSTEM_H__74A5598A_2DB5_47F6_AC02_B106E9C86F81__INCLUDED_)
#define AFX_AIRDATAPROBESYSTEM_H__74A5598A_2DB5_47F6_AC02_B106E9C86F81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtlantisSubsystem.h"

class AirDataProbeSystem : public AtlantisSubsystem  
{
	typedef enum {ADPS_DEPLOYED, ADPS_DEPLOYING, ADPS_STOWING, ADPS_STOWED} DEPLOY_STATE;
	typedef enum {ADPS_STOW_ENABLE, ADPS_STOW_INHIBIT} STOW_ENABLE_STATE;
public:
	virtual void Propagate(double fSimT, double fDeltaT);
	double GetTotalTemperature(int side) const;
	double GetDynamicPressure(int side) const;
	virtual double GetStaticPressure(int side) const;
	typedef enum {ADPS_DEPLOY, ADPS_STOW} DEPLOY_MODE;
	virtual void PostStep(double fSimT, double fDeltaT);
	bool IsDeployed(int side) const;
	void SetDeployMode(int side, DEPLOY_MODE mode);
	virtual void PreStep(double fSimT, double fDeltaT);
	AirDataProbeSystem(SubsystemDirector* _director);
	virtual ~AirDataProbeSystem();

private:
	double right_deploy[2];
	double left_deploy[2];
	DEPLOY_MODE left_mode[2];
	DEPLOY_MODE right_mode[2];
	DEPLOY_STATE left_state[2];
	DEPLOY_STATE right_state[2];
	STOW_ENABLE_STATE left_stow_state[2];
	STOW_ENABLE_STATE right_stow_state[2];
};

#endif // !defined(AFX_AIRDATAPROBESYSTEM_H__74A5598A_2DB5_47F6_AC02_B106E9C86F81__INCLUDED_)
