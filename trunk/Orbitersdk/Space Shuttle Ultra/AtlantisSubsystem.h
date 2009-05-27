/****************************************************************************
  This file is part of Space Shuttle Ultra

  Basic subsystem definitions



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/

#if !defined(AFX_ATLANTISSUBSYSTEM_H__6A9F0F48_D391_4E11_9536_F359182CA558__INCLUDED_)
#define AFX_ATLANTISSUBSYSTEM_H__6A9F0F48_D391_4E11_9536_F359182CA558__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "OrbiterAPI.h"
#include "ISubsystem.h"
#include "dps/dps_defs.h"
#include "discsignals\DiscreteBundleManager.h"

using namespace std;
using namespace discsignals;
using class dps::ShuttleBusManager;

class Atlantis;
class SubsystemDirector;
/**
 * base class for all subsystem simulations. 
 */
class AtlantisSubsystem: public ISubsystem 
{
protected:
	SubsystemDirector* director;
	string ident;
public:
	/**
	 * Create a new Atlantis subsystem object
	 */
	AtlantisSubsystem(SubsystemDirector* _director, const string& _ident);
	/**
	 * Destructor
	 */
	virtual ~AtlantisSubsystem();
	/**
	 * Get a reference to the Space Shuttle external physical model.
	 * @return reference to the space shuttle object. If possible.
	 */
	Atlantis* STS() const;
	/**
	 * Add new meshes to the visual. 
	 * @deprecated Don't use, instead, use Realize() to tell the Atlantis 
	 * class which meshes it should use for special slots.
	 */
	virtual void AddMeshes(const VECTOR3& ofs);
	/**
	 * Create the physical representation of the subsystem. This means:
	 * - creation of thrusters
	 * - discrete line connections to VC components
	 * - discrete line connections to other subsystems
	 * - connection to Shuttle Bus systems
	 * - hydraulic connections
	 * - coolant loop connections
	 * - connections to logical propellant resources
	 * - 
	 */
	virtual void Realize();
	/**
	 * Register a subsystem component for use inside this subsystem for automated 
	 * handling. Not in use now.
	 * @warning Does not have an effect.
	 */
	virtual bool RegisterComponent(SubsystemComponent* pComponent);
	/** 
	 * Delete a subsystem component, if no longer used
	 * @warning Does not have an effect.
	 */
	virtual bool DelComponent(SubsystemComponent* pComponent);
	/**
	 * Count the number of subsystem components. 
	 * @return always zero, as subsystem component handling is not active.
	 */
	virtual unsigned long CountComponents() const;
	/**
	 * Prepare and execute calculations before the propagation of the 
	 * state vector
	 * @param fSimT current simulation time
	 * @param fDeltaT Simulation time step length
	 * @param fMJD current modified julian date
	 */
	virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
	/**
	 * Finish calculations after the propagation of the 
	 * state vector
	 * @param fSimT current simulation time
	 * @param fDeltaT Simulation time step length
	 * @param fMJD current modified julian date
	 */
	virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
	/**
	 * Propagate the state vector from old state to new state.
	 * @param fSimT current simulation time
	 * @param fDeltaT Simulation time step length
	 * @param fMJD current modified julian date
	 */
	virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
	virtual void OnPlaybackEvent(double fSimT, double fEventT, const char* event_t, const char* event);
	/**
	 * Parse a line of the scenario file. 
	 * @warning
	 * Should get replaced by a block oriented scenario file 
	 * format, like NASSP, making this function deprecated. 
	 */
	virtual bool OnParseLine(const char* line);
	/**
	 * Save the state of the subsystem
	 */
	virtual void OnSaveState(FILEHANDLE scn) const;

	//Callbacks used for subsampling 
	virtual void OnSubPreStep(double fSimT, double fDeltaT, double fMJD);
	virtual void OnSubPropagate(double fSimT, double fDeltaT, double fMJD);
	virtual void OnSubPostStep(double fSimT, double fDeltaT, double fMJD);
	/**
	 * Dump the current state and error conditions into the Orbiter.log or 
	 * a special SSU.log. Don't modificate state.
	 */
	virtual void OnDumpToLog() const;
	/**
	 * get the full identifier of the subsystem, including parent container, 
	 * if needed. Usually the same as GetIdentifier.
	 */
	virtual const string& GetQualifiedIdentifier() const;
	/**
	 * get the identifier of the subsystem. This identifier can be used for 
	 * locating the subsystem inside the physical model AND the scenario file. 
	 * should not contain white spaces.
	 */
	virtual const string& GetIdentifier() const;
	/**
	 * Entry point for triggering timed failures. Will be used for 
	 * failure simulation.
	 */
	virtual bool OnFail(const string& strFailureCode);
	/**
	 * Get the dry mass of the subsystem, if this subsystem is optional and 
	 * causes changes to the standard orbiter dry mass. For example RMS, 
	 * external airlock/ODS, etc.
	 * @return The subsystem dry mass in kg.
	 */
	virtual double GetSubsystemEmptyMass() const {return 0.0;};

	/**
	 * Remove subsystem from Shuttle and visual
	 */
	virtual void UnloadSubsystem();

	/** 
	 * Return the currently used discrete bundle managing object. 
	 * @return pointer to the bundle manager
	 */
	DiscreteBundleManager* BundleManager() const;
	/** 
	 * Return the currently used data bus managing object. 
	 * @return pointer to the bus manager
	 */
	ShuttleBusManager* BusManager() const;

	virtual bool OnParseLine(const char* keyword, const char* line);
	virtual bool OnReadState (FILEHANDLE scn);
	virtual void OnSetClassCaps();
	virtual bool SingleParamParseLine() const {return false;};
};

class EmptySubsystem: AtlantisSubsystem {
public:
	EmptySubsystem(SubsystemDirector* _director);
	virtual ~EmptySubsystem();
};

#endif // !defined(AFX_ATLANTISSUBSYSTEM_H__6A9F0F48_D391_4E11_9536_F359182CA558__INCLUDED_)
