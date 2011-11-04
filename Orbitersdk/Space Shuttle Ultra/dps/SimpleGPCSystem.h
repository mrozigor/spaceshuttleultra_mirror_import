#ifndef SIMPLEGPCSYSTEM_H_6EEC6BE1_40A2_40EE_A47C_D7A7F5FA469F
#define SIMPLEGPCSYSTEM_H_6EEC6BE1_40A2_40EE_A47C_D7A7F5FA469F
#pragma once

#include "../AtlantisSubsystem.h"
#include "dps_defs.h"
#include "../vc/vc_defs.h"
#include <vector>

namespace dps
{

/**
 * Simple class to simulate GPC and associated software.
 * Will eventually be replaced by proper GPC simulation
 * Similar to SubsystemDirector/Subsystem structure; GPC calls functions on different software classes.
 */
class SimpleGPCSystem : public AtlantisSubsystem
{
	std::vector<SimpleGPCSoftware*> vSoftware; // all software
	std::vector<SimpleGPCSoftware*> vActiveSoftware; // software used in current major mode

	unsigned int majorMode;
public:
	SimpleGPCSystem(AtlantisSubsystemDirector* _director);
	virtual ~SimpleGPCSystem();

	void SetMajorMode(unsigned int newMM);

	unsigned int GetMajorMode() const { return majorMode; };

	virtual void Realize();

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	virtual void OnPostStep(double SimT, double DeltaT, double MJD);
	virtual void OnPropagate(double SimT, double DeltaT, double MJD);

	virtual bool OnReadState(FILEHANDLE scn);
	virtual void OnSaveState(FILEHANDLE scn) const;

	/**
	 * Handles Item entry on shuttle's keyboard.
	 * Returns true if item/spec are supported, false otherwise.
	 * @param spec spec currently displayed
	 * @param item ITEM number
	 * @param Data string containing data entered
	 */
	bool ItemInput(int spec, int item, const char* Data);
	/**
	 * Called when EXEC is pressed and no data has been entered.
	 * Returns true if keypress was handled.
	 */
	bool ExecPressed(int spec);
	/**
	 * Draws display on MDU.
	 * Returns true if data was drawn; false otherwise
	 */
	bool OnPaint(int spec, vc::MDU* pMDU) const;
	bool OnDrawHUD(const HUDPAINTSPEC* hps, oapi::Sketchpad* skp) const;

	SimpleGPCSoftware* FindSoftware(const std::string& identifier) const;
};

};

#endif