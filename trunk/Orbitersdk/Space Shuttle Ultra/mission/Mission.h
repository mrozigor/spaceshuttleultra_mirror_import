#ifndef ___ORBITER_SPACE_SHUTTLE_ULTRA_MISSION_H_INCLUDED___
#define ___ORBITER_SPACE_SHUTTLE_ULTRA_MISSION_H_INCLUDED___

#include "../Atlantis.h"
#include <string>

/**
 * Contains all class definitions related to Mission Data Files and 
 * handling mission data in the simulation
 */
namespace mission {

	using namespace std;

	class Mission {
	protected:
		string strMissionName;
		string strOrbiter;
		int iETVersion;
		int iBranchCode;
		int iRevisionCode;

		double fLTimeMJD;
		bool bUseRMS;
	public:
		Mission();
		virtual ~Mission();

		virtual bool LoadMission(const string& strFilename);
		virtual bool SaveMission(const string& strFilename) const;
		virtual bool SaveMission() const;
		virtual Mission* CloneNewBranch() const;
		virtual Mission* CloneRevision() const;

		virtual const string& GetOrbiter() const;
		virtual const string& GetMissionName() const;

		virtual bool CheckValidity() const;

	};

};

#endif