/****************************************************************************
  This file is part of Space Shuttle Ultra

  Mission file handling definition



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
#ifndef ___ORBITER_SPACE_SHUTTLE_ULTRA_MISSION_H_INCLUDED___
#define ___ORBITER_SPACE_SHUTTLE_ULTRA_MISSION_H_INCLUDED___

#include <string>

const double DEFAULT_PAYLOAD_ZPOS[16] = 
{ 8.0, 0.0, -8.0,		//Center active
  4.0, 2.0, -6.0,		//Center passive
  7.0, 3.0, -2.0, -8.0,	//Port
  7.0, 3.0, -2.0, -8.0,	//Starboard
  0.0, 0.0};			//spare

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
		bool bHasMPMs;

		bool bUseOMSAssist;
		unsigned short usExtAirlockMode;

		double fPayloadZPos[16];

		
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

		/**
		 * @return target equ inclination at MECO in radians
		 */
		virtual double GetMECOInc() const;
		virtual double GetMECOAlt() const;
		virtual double GetMECOVel() const;
		virtual double GetMECOFPA() const;

		virtual unsigned int GetNumberOfOMSBurns() const;
		
		virtual const std::string& GetOrbiter() const;
		virtual const std::string& GetMissionName() const;
		virtual double GetPayloadZPos(unsigned int iIndex) const;
		
		virtual bool HasKuBandAntenna() const;
		virtual bool HasRMS() const;
		virtual bool HasSTBDMPMs() const;
		

		virtual bool UseDirectAscent() const;
		virtual bool UseOMSAssist() const;

		
	};

};

#endif