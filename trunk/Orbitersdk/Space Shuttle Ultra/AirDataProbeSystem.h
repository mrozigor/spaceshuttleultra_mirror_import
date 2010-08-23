// AirDataProbeSystem.h: Schnittstelle für die Klasse AirDataProbeSystem.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIRDATAPROBESYSTEM_H__74A5598A_2DB5_47F6_AC02_B106E9C86F81__INCLUDED_)
#define AFX_AIRDATAPROBESYSTEM_H__74A5598A_2DB5_47F6_AC02_B106E9C86F81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtlantisSubsystem.h"
#include "discsignals/discsignals.h"

using namespace discsignals;

class AirDataProbeSystem : public AtlantisSubsystem  
{
	typedef enum {ADPS_DEPLOYED, ADPS_DEPLOYING, ADPS_STOWING, ADPS_STOWED} DEPLOY_STATE;
//public:
	typedef enum {ADPS_DEPLOY, ADPS_STOW} DEPLOY_MODE;
	//typedef enum {ADPS_STOW_ENABLE, ADPS_STOW_INHIBIT} STOW_ENABLE_STATE;
public:
	virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
	double GetTotalTemperature(int side) const;
	double GetDynamicPressure(int side) const;
	virtual double GetStaticPressure(int side) const;
	
	virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
	bool IsDeployed(int side) const;
	virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
	AirDataProbeSystem(AtlantisSubsystemDirector* _director);
	virtual ~AirDataProbeSystem();

	virtual void Realize();

	virtual bool OnParseLine(const char* line);
	virtual void OnSaveState(FILEHANDLE scn) const;
	virtual bool SingleParamParseLine() const {return true;};
private:
	//void SetDeployMode(int side, DEPLOY_MODE mode);

	double right_deploy[2];
	double left_deploy[2];
	DEPLOY_MODE left_mode[2];
	DEPLOY_MODE right_mode[2];
	DEPLOY_STATE left_state[2];
	DEPLOY_STATE right_state[2];
	//STOW_ENABLE_STATE left_stow_state[2];
	//STOW_ENABLE_STATE right_stow_state[2];

	DiscInPort StowEnable[2], Deploy[2], DeployHeat[2];
};

#endif // !defined(AFX_AIRDATAPROBESYSTEM_H__74A5598A_2DB5_47F6_AC02_B106E9C86F81__INCLUDED_)
