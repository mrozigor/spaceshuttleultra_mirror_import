#pragma once

#include "AtlantisSubsystem.h"
#include "discsignals\discsignals.h"


class MCAModule {
public:
	MCAModule();
	~MCAModule();

	void OnPropagate();

	discsignals::DiscInPort CmdFwd;
	discsignals::DiscInPort CmdRwd;
	discsignals::DiscInPort CmdLogic;
	discsignals::DiscOutPort OutPos0;
	discsignals::DiscOutPort OutPos1;

	//Status signals from mechanics.
	discsignals::DiscInPort MotorInPos0;
	discsignals::DiscInPort MotorInPos1;

	//@todo replace these discretes by AC power lines
	discsignals::DiscOutPort MotorFwd;
	discsignals::DiscOutPort MotorRwd;
	
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