#ifndef SIMPLEGPCSOFTWARE_H_C7DDF614_D4FA_4F2E_9CF1_8E21388BC790
#define SIMPLEGPCSOFTWARE_H_C7DDF614_D4FA_4F2E_9CF1_8E21388BC790
#pragma once

#include "SimpleGPCSystem.h"
#include "../vc/vc_defs.h"
#include <string>

namespace dps
{

class SimpleGPCSoftware
{
	SimpleGPCSystem* pGPC;
	std::string ident;
public:
	SimpleGPCSoftware(SimpleGPCSystem* _gpc, const std::string& _ident);
	virtual ~SimpleGPCSoftware();

	const std::string& GetIdentifier() const { return ident; };
	Atlantis* STS() const { return pGPC->STS(); };
	DiscreteBundleManager* BundleManager() const { return pGPC->BundleManager(); };
	unsigned int GetMajorMode() const { return pGPC->GetMajorMode(); };
	void SetMajorMode(unsigned int newMajorMode) const { pGPC->SetMajorMode(newMajorMode); };

	virtual void Realize();

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	virtual void OnPostStep(double SimT, double DeltaT, double MJD);
	virtual void OnPropagate(double SimT, double DeltaT, double MJD);

	/**
	 * Returns true if software is active, false if software should not be used in this MM
	 * Called BEFORE MajorMode is changed (GetMajorMode() will return old value)
	 */
	virtual bool OnMajorModeChange(unsigned int newMajorMode);

	/**
	 * Parses single line in scenario
	 * @param keyword string contaning item id
	 * @param value value of parameter(s)
	 */
	virtual bool OnParseLine(const char* keyword, const char* value);
	/**
	 * Save the state of the software
	 */
	virtual void OnSaveState(FILEHANDLE scn) const;

	/**
	 * Handles Item entry on shuttle's keyboard.
	 * Returns true if item/spec are supported, false otherwise.
	 * @param spec spec currently displayed
	 * @param item ITEM number
	 * @param Data string containing data entered
	 * @param IllegalEntry	if an data entry is illegal, this should be set to true
	 */
	virtual bool ItemInput(int spec, int item, const char* Data, bool &IllegalEntry );
	/**
	 * Called when EXEC is pressed and no data has been entered.
	 * Returns true if keypress was handled.
	 */
	virtual bool ExecPressed(int spec);
	/**
	 * Draws display on MDU.
	 * Will only be called for active software
	 * Returns true if data was drawn; false otherwise
	 */
	virtual bool OnPaint(int spec, vc::MDU* pMDU) const;
	virtual bool OnDrawHUD(const HUDPAINTSPEC* hps, oapi::Sketchpad* skp) const;
	SimpleGPCSoftware* FindSoftware(const std::string& identifier) const;
protected:
	void PrintCommonHeader(const char* header, vc::MDU* pMDU) const;
	
	/**
	 * Converts a string into a signed integer, checking that the string is not empty, the number in the string is an integer and that no further chars exist.
	 * @param data	string to process
	 * @param num	variable where to save the number
	 * @return	true if the string only contains an integer, false otherwise.
	 */
	bool GetIntegerSigned( const char *data, int &num ) const;

	/**
	 * Converts a string into an unsigned integer (by ignoring the sign), checking that the string is not empty, the number in the string is an integer and that no further chars exist.
	 * @param data	string to process
	 * @param num	variable where to save the number
	 * @return	true if the string only contains an integer, false otherwise.
	 */
	bool GetIntegerUnsigned( const char *data, int &num ) const;

	/**
	 * Converts a string into a signed double, checking that the string is not empty, the number in the string is a double and that no further chars exist.
	 * @param data	string to process
	 * @param num	variable where to save the number
	 * @return	true if the string only contains a double, false otherwise.
	 */
	bool GetDoubleSigned( const char *data, double &num ) const;

	/**
	 * Converts a string into an unsigned double (by ignoring the sign), checking that the string is not empty, the number in the string is a double and that no further chars exist.
	 * @param data	string to process
	 * @param num	variable where to save the number
	 * @return	true if the string only contains a double, false otherwise.
	 */
	bool GetDoubleUnsigned( const char *data, double &num ) const;
};

};

#endif
