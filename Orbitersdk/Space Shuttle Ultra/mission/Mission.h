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
{ 8.0, 0.0, -8.0,		//Center active (attachments 5, 6, 7)
  4.0, 2.0, -6.0, -6.0,		//Center passive (attachments 8, 9, 10, 11)
  7.0, 3.0, -2.0, -8.0,	//Port (attachments 12, 13, 14, 15)
  7.0, 3.0, -2.0, -8.0,	//Starboard (attachments 16, 17, 18, 19)
  0.0};			//spare


// Z-position (in OrbiterSim coordinates of ODS/AL mesh)
const double EXTERNAL_AIRLOCK_Z_POSITION = 8.25;
const double TAA_EXTERNAL_AIRLOCK_Z_POSITION = 6.13;

const double TAA_FORWARD_POSITION = 8.5;// between bulkhead and ExtAL
const double TAA_AFT_POSITION = 5.83;// aft of ExtAL


/**
 * Contains all class definitions related to Mission Data Files and 
 * handling mission data in the simulation
 */
namespace mission {


	class Mission {
	protected:
		std::string strFileName;
		std::string strMissionName;
		std::string strOrbiter;
		std::string strOrbiterTexName;
		//std::string strLOMSPodMeshName;
		//std::string strROMSPodMeshName;
		int iETVersion;
		int iBranchCode;
		int iRevisionCode;

		//double fLaunchTimeMJD;
		//double fLandTimeMJD;
		double fMECOAlt;
		double fMECOVel;
		double fMECOFPA;
		double fTargetInc;
		double fTHdown;
		double fTHup;
		
		bool bUseRMS;
		bool bHasMPMs;
		bool bHasODS;
		bool bHasExtAL;
		bool bHasTAA;
		bool bAftTAA;
		bool bHasKUBand;
		bool bHasBulkheadFloodlights;
		bool bHasDragChute;

		bool bPerformRTHU;
		bool bUseOMSAssist;
		double OMSAssistDuration;// sec
		double fMaxSSMEThrust;
		unsigned short usExtAirlockMode;

		double OVmass;

		double fPayloadZPos[16];

		bool bHasBridgerail[13]; // true if bridgerail is present, false otherwise

		bool bUseSILTS;

		bool bLogSSMEData;

		bool bUseASE_IUS;
		bool bASE_IUS_Aft_Location;

		bool bUseCISS;
		bool bCISS_GPrime;

		/**
		 * Initializes parameters to default values
		 */
		void SetDefaultValues();
	public:
		/**
		 * Loads data from specified file.
		 * If strMission is empty string, returns default values.
		 */
		Mission(const std::string& strMission);
		virtual ~Mission();

		virtual bool LoadMission(const std::string& strMission);
		virtual bool SaveMission(const std::string& strMission) const;
		virtual bool SaveMission() const;
		/**
		 * @return Planned time of the first nominal landing 
		 * opportunity in seconds MET.
		 */
		//virtual double GetFirstLandingMET() const;
		
		/**
		 * @return Launch azimuth in radians
		 */
		virtual double GetLaunchAzimuth() const;
		/**
		 * @return Initially planned lift-off time (L-0) as MJD
		 */
		//virtual double GetLaunchMJD() const;
		/**
		 * @return
		 * 1 = KSC
		 * 2 = Vandenberg
		 */
		//virtual unsigned int GetLaunchSite() const;

		/**
		 * @return target equ inclination at MECO in radians
		 */
		virtual double GetMECOInc() const;
		virtual double GetMECOAlt() const;
		virtual double GetMECOVel() const;
		virtual double GetMECOFPA() const;

		/**
		 * @return maximum SSME thrust during standard launch profile
		 */
		virtual double GetMaxSSMEThrust() const;
		
		virtual const std::string& GetOrbiter() const;
		virtual const std::string& GetMissionName() const;
		virtual const std::string& GetMissionFileName() const;
		/**
		 * @return Name of texture file specified in mission file
		 * Empty string if no file is specified
		 */
		virtual const std::string& GetOrbiterTextureName() const;
		
		//virtual const std::string& GetLOMSPodMeshName() const;
		//virtual const std::string& GetROMSPodMeshName() const;
		
		virtual double GetPayloadZPos(unsigned int iIndex) const;
		virtual double GetExternalAirlockZPos() const;
		virtual double GetTunnelAdapterAssemblyZPos() const;

		virtual bool HasBridgerail(unsigned int index) const;
		
		virtual bool HasRMS() const;
		virtual bool HasSTBDMPMs() const;
		virtual bool HasODS() const;
		virtual bool HasExtAL() const;
		virtual bool HasTAA() const;
		virtual bool AftTAA() const;
		virtual bool HasBulkheadFloodlights() const;
		virtual bool HasDragChute() const;

		/**
		 * returns mass of the defined Orbiter Vehicle.
		 * @return	mass of Orbiter Vehicle (kg)
		 */
		virtual double GetOrbiterMass( void ) const;
		/**
		 * returns enable status of OMS Assist
		 */
		virtual bool UseOMSAssist() const;
		/**
		 * returns duration of OMS Assist
		 */
		virtual double GetOMSAssistDuration() const;
		/**
		 * Returns true if roll to heads up should be performed
		 */
		virtual bool PerformRTHU() const;
		/**
		 * Returns MaxQ throttle down velocity in fps
		 */
		virtual double GetTHdownVelocity() const;
		/**
		 * Returns MaxQ throttle up velocity in fps
		 */
		virtual double GetTHupVelocity() const;
		/**
		 * Returns true if KU-band antenna is installed.
		 */
		virtual bool HasKUBand() const;
		/**
		 * Returns true if SILTS pod is installed.
		 */
		virtual bool UseSILTS() const;
		/**
		 * Returns true if SSME data arriving at the EIU is to be logged.
		 */
		virtual bool LogSSMEData() const;

		/**
		 * Returns true is the ASE for the IUS is used.
		 */
		bool UseASE_IUS() const;

		/**
		 * Returns true if the ASE for the IUS is located in its aft position in the payload bay (ignored if the ASE is not used).
		 */
		bool IsASELocationAft() const;

		/**
		 * Returns true if the CISS is used.
		 */
		virtual bool UseCISS() const;

		/**
		 * Returns true if the CISS used is the G Prime version.
		 */
		virtual bool IsCISSGPrime() const;
	};

};

#endif
