#ifndef ___ORBITER_SPACE_SHUTTLE_ULTRA_MISSION_H_INCLUDED___
#define ___ORBITER_SPACE_SHUTTLE_ULTRA_MISSION_H_INCLUDED___

#include <string>

/**
 * Contains all class definitions related to Mission Data Files and 
 * handling mission data in the simulation
 */
namespace mission {


	class Mission {
	protected:
		std::string strMissionName;
		std::string strOrbiter;
		int iETVersion;
		int iBranchCode;
		int iRevisionCode;

		double fLTimeMJD;
		double fLandTimeMJD;
		double fMECOAlt;
		double fMECOVel;
		double fMECOFPA;
		double fTargetInc;
		
		bool bUseRMS;
		bool bUseOMSAssist;

		
	public:
		Mission();
		Mission(const std::string& strMission);
		virtual ~Mission();

		virtual bool LoadMission(const std::string& strMission);
		virtual bool SaveMission(const std::string& strMission) const;
		virtual bool SaveMission() const;
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

		virtual unsigned int GetNumberOfOMSBurns() const;
		
		virtual const std::string& GetOrbiter() const;
		virtual const std::string& GetMissionName() const;
		virtual bool HasKuBandAntenna() const;

		virtual bool UseDirectAscent() const;
		virtual bool UseOMSAssist() const;
	};

};

#endif