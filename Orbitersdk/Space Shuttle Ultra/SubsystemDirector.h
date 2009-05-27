/****************************************************************************
  This file is part of Space Shuttle Ultra

  Subsystem Director



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
#ifndef ___ATLANTIS_ULTRA_SUBSYSTEM_DIRECTOR_H___
#define ___ATLANTIS_ULTRA_SUBSYSTEM_DIRECTOR_H___

#include <vector>
#include "OrbiterAPI.h"
#include "dps/dps_defs.h"
#include "discsignals/DiscreteBundleManager.h"

using namespace std;

class Atlantis;
class AtlantisSubsystem;

using class discsignals::DiscreteBundleManager;
using class dps::ShuttleBusManager;

class SubsystemDirector
{
protected:
	Atlantis* psts;
	vector<AtlantisSubsystem*> subsystems;
public:
	virtual Atlantis* STS();
	SubsystemDirector(Atlantis* _sts);
	~SubsystemDirector();
	bool AddSubsystem(AtlantisSubsystem* pSubsys);
	AtlantisSubsystem* ReplaceSubsystem(AtlantisSubsystem* pCurrentSubsys, AtlantisSubsystem* pBySubsys);
	bool ExistsSubsystem(const std::string& name) const;
	AtlantisSubsystem* GetSubsystemByName(const std::string& name) const;

	void SetClassCaps(FILEHANDLE cfg);
	bool ParseScenarioLine(FILEHANDLE scn, char* line);
	bool PlaybackEvent(double fSimT, double fEventT, const char* event_t, const char* event);
	bool SaveState(FILEHANDLE scn);
	bool PostStep(double fSimT, double fDeltaT, double fMJD);
	bool PreStep(double fSimT, double fDeltaT, double fMJD);
	bool WriteLog(const AtlantisSubsystem* src, char* message);
	virtual DiscreteBundleManager* BundleManager() const;


	virtual dps::ShuttleBusManager* BusManager() const;
	
	/**
	 * 
	 */
	bool RealizeAll();
	/**
	 * @param usMPSNo numerical ID of the SSME
	 * @param fThrust0 Vacuum thrust
	 * @param fISP0 Vacuum ISP
	 * @param fISP1 Sealevel ISP
	 * @return false for failure, true for success
	 */
	bool SetSSMEParams(unsigned short usMPSNo, double fThrust0, double fISP0, double fISP1);
	/**
	 * @param usMPSNo numerical ID of the SSME
	 * @param dir direction vector of the SSME force vector
	 * @return false for failure, true for success
	 */
	bool SetSSMEDir(unsigned short usMPSNo, const VECTOR3& dir);
};

#endif