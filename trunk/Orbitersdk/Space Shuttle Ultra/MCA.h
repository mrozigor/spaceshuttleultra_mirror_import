#pragma once

#include "AtlantisSubsystem.h"
#include "discsignals\discsignals.h"
#include "eps\eps_defs.h"



class MCAModule {
public:
	MCAModule();
	~MCAModule();

	void OnPropagate(double fSimT, double fDeltaT);

	discsignals::DiscInPort CmdLogic;
	discsignals::DiscOutPort OutPos0;
	discsignals::DiscOutPort OutPos1;

	//Status signals from mechanics.
	discsignals::DiscInPort MotorInPos0;
	discsignals::DiscInPort MotorInPos1;

	eps::ACMotorController mc;
};

class MCA: public AtlantisSubsystem 
{
public:
	MCA(SubsystemDirector* _direct, const std::string& name);
	virtual ~MCA();
	virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
	virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
	virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
	//@todo Add AC power interface for all modules

	MCAModule module[8];
	
	discsignals::DiscInPort logic;
};