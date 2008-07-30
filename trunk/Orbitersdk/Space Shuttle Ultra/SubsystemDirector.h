#ifndef ___ATLANTIS_ULTRA_SUBSYSTEM_DIRECTOR_H___
#define ___ATLANTIS_ULTRA_SUBSYSTEM_DIRECTOR_H___

#include <vector>
#include "OrbiterAPI.h"
#include "discsignals/DiscreteBundleManager.h"

using namespace std;

class Atlantis;
class AtlantisSubsystem;

using discsignals::DiscreteBundleManager;

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
	void SetClassCaps(FILEHANDLE cfg);
	bool ParseScenarioLine(char* line);
	bool PlaybackEvent(double fSimT, double fEventT, const char* event_t, const char* event);
	bool SaveState(FILEHANDLE scn);
	bool PostStep(double fSimT, double fDeltaT, double fMJD);
	bool PreStep(double fSimT, double fDeltaT, double fMJD);
	bool WriteLog(const AtlantisSubsystem* src, char* message);
	virtual DiscreteBundleManager* BundleManager() const;
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