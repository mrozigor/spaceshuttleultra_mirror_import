#ifndef ___ATLANTIS_ULTRA_SUBSYSTEM_DIRECTOR_H___
#define ___ATLANTIS_ULTRA_SUBSYSTEM_DIRECTOR_H___

#include <vector>
#include "OrbiterAPI.h"

using namespace std;

class Atlantis;
class AtlantisSubsystem;

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
};

#endif