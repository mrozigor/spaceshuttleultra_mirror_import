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

		virtual bool LoadMission(const string& strMission);
		virtual bool SaveMission(const string& strMission) const;
		virtual bool SaveMission() const;
		virtual Mission* CloneNewBranch() const;
		virtual Mission* CloneRevision() const;

		/**
		 * @return Planned time of the first nominal landing 
		 * opportunity in seconds MET.
		 */
		virtual double GetFirstLandingMET() const;
		
		/**
		 * @return Launch azimuth in radians
		 */
		virtual double GetLaunchAzimuth() const;
		/**
		 * @return Initially planned lift-off time (L-0) as MJD
		 */
		virtual double GetLaunchMJD() const;
		/**
		 * @return
		 * 1 = KSC
		 * 2 = Vandenberg
		 */
		virtual unsigned int GetLaunchSite() const;

		virtual double GetMECOAlt() const;
		virtual double GetMECOVel() const;
		virtual double GetMECOFPA() const;

		virtual int GetNumberOfOMSBurns() const;
		
		virtual const string& GetOrbiter() const;
		virtual const string& GetMissionName() const;
		virtual bool HasKuBandAntenna() const;

		virtual bool UseDirectAscent() const;
		virtual bool UseOMSAssist() const;
	};

};

#endif