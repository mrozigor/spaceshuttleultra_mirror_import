#include "Mission.h"
#include "OrbiterAPI.h"
#include "UltraUtils.h"
#include "..\ParameterValues.h"
#include <limits>
#include <vector>

namespace mission {

	Mission::Mission(const std::string& strMission)
	{
		SetDefaultValues();
		if(!strMission.empty())
		{
			if(!LoadMission(strMission))
			{
				oapiWriteLog("(Mission) failed creating mission from loaded file.");
			}
		}
	}

	Mission::~Mission()
	{
	}

	void Mission::SetDefaultValues()
	{
		//strLOMSPodMeshName = "SSU\\LOMS_pod_standard";
		//strROMSPodMeshName = "SSU\\ROMS_pod_standard";
		//fLaunchTimeMJD = -1.0;
		//fLandTimeMJD = -1.0;

		fTargetInc = 28.5*RAD;
		fMECOAlt = 105564;
		fMECOVel = 7869.635088;
		fMECOFPA = 0.747083*RAD;

		bUseOMSAssist = false;
		OMSAssistDuration = 102;// standard 4000lbs OMS assist
		bPerformRTHU = false;
		fMaxSSMEThrust = 104.5;
		//fTHdown = 834.0;
		//fTHup = 1174.0;
		fTHdown = 792.0;
		fTHup = 1304.0;

		OVmass = ORBITER_EMPTY_MASS_OV104;// default to Atlantis

		bUseRMS = false;
		bHasKUBand = true;
		bHasMPMs = false;
		bHasODS = false;
		bHasExtAL = false;
		bHasTAA = false;
		bAftTAA = false;
		bHasBulkheadFloodlights = false;
		bHasDragChute = true;

		bUseSILTS = false;

		for(int i=0;i<16;i++) fPayloadZPos[i] = DEFAULT_PAYLOAD_ZPOS[i];

		for(int i=0;i<13;i++) bHasBridgerail[i] = false;

		bLogSSMEData = false;

		bUseASE_IUS = false;
		bASE_IUS_Aft_Location = false;

		bUseCISS = false;
		bCISS_GPrime = true;
	}

	bool Mission::LoadMission(const std::string& strMission)
	{
		char buffer[800];
		std::string filename;
		filename = "Missions\\SSU\\" + strMission + ".cfg";
		strFileName = strMission;

		sprintf_s(buffer, 800, "(Mission) Loading mission %s from file %s",
			strMission.c_str(), filename.c_str());
		oapiWriteLog(buffer);
		FILEHANDLE hFile = oapiOpenFile(filename.c_str(), FILE_IN);

	

		if(hFile == NULL)
		{
			oapiWriteLog("(Mission) ERROR: Can't open file.");
			return false;
		}

		if(oapiReadItem_string(hFile, "Name", buffer))
		{
			strMissionName = buffer;
		}

		if(oapiReadItem_string(hFile, "Orbiter", buffer))
		{
			strOrbiter = buffer;
		}
		if (strOrbiter == "Columbia") OVmass = ORBITER_EMPTY_MASS_OV102;
		else if (strOrbiter == "Challenger") OVmass = ORBITER_EMPTY_MASS_OV099;
		else if (strOrbiter == "Discovery") OVmass = ORBITER_EMPTY_MASS_OV103;
		//else if (strOrbiter == "Atlantis") OVmass = ORBITER_EMPTY_MASS_OV104;
		else if (strOrbiter == "Endeavour") OVmass = ORBITER_EMPTY_MASS_OV105;
		// default already loaded ORBITER_EMPTY_MASS_OV104

		if(oapiReadItem_string(hFile, "OrbiterTexture", buffer))
		{
			strOrbiterTexName = "SSU\\" + std::string(buffer) + ".dds";
			oapiWriteLog((char*)strOrbiterTexName.c_str());
		}

		/*if(oapiReadItem_string(hFile, "LOMSPodMesh", buffer))
		{
			strLOMSPodMeshName = "SSU\\" + std::string(buffer);
		}
		if(oapiReadItem_string(hFile, "ROMSPodMesh", buffer))
		{
			strROMSPodMeshName = "SSU\\" + std::string(buffer);
		}*/

		//oapiReadItem_float(hFile, "LTime", fLaunchTimeMJD);
		
		//oapiReadItem_float(hFile, "FirstReturnOpport", fLandTimeMJD);

		if(oapiReadItem_float(hFile, "TargetInc", fTargetInc))
		{
			fTargetInc *= RAD;
		}

		oapiReadItem_float(hFile, "MECOAlt", fMECOAlt);
		oapiReadItem_float(hFile, "MECOVel", fMECOVel);
		if(oapiReadItem_float(hFile, "MECOFPA", fMECOFPA))
		{
			fMECOFPA *= RAD;
		}

		oapiReadItem_bool(hFile, "PerformRollToHeadsUp", bPerformRTHU);

		oapiReadItem_bool( hFile, "OMSAssistEnable", bUseOMSAssist );
		oapiReadItem_float( hFile, "OMSAssistDuration", OMSAssistDuration );

		oapiReadItem_float(hFile, "MaxSSMEThrust", fMaxSSMEThrust);
		oapiReadItem_float(hFile, "ThrottleDown", fTHdown);
		oapiReadItem_float(hFile, "ThrottleUp", fTHup);

		oapiReadItem_bool(hFile, "UseRMS", bUseRMS);
		oapiReadItem_bool(hFile, "UseKUBand", bHasKUBand);
		oapiReadItem_bool(hFile, "UseSTBDMPM", bHasMPMs);
		oapiReadItem_bool(hFile, "UseODS", bHasODS);
		oapiReadItem_bool(hFile, "UseExtAL", bHasExtAL);
		oapiReadItem_bool( hFile, "UseTAA", bHasTAA );
		oapiReadItem_bool( hFile, "AftTAA", bAftTAA );
		oapiReadItem_bool(hFile, "HasBulkheadFloodlights", bHasBulkheadFloodlights);
		oapiReadItem_bool(hFile, "HasDragChute", bHasDragChute);

		for(int i = 0; i<16; i++)
		{
			double x;
			sprintf_s(buffer, "PayloadZPos%d", i + 5);
			if(oapiReadItem_float(hFile, buffer, x))
			{
				fPayloadZPos[i] = x;
			}
		}

		if(oapiReadItem_string(hFile, "Bridgerails", buffer)) {
			std::vector<int> bridgerails;
			ReadCSVLine(buffer, bridgerails);
			for(unsigned int i=0; i<bridgerails.size(); i++) {
				if(bridgerails[i] >= 0 && bridgerails[i] < 13)
					bHasBridgerail[bridgerails[i]] = true;
			}
		}

		if (strOrbiter == "Columbia") oapiReadItem_bool( hFile, "SILTS", bUseSILTS );

		oapiReadItem_bool( hFile, "LogSSMEData", bLogSSMEData );

		oapiReadItem_bool( hFile, "UseASE_IUS", bUseASE_IUS );
		oapiReadItem_bool( hFile, "ASE_IUS_AftLocation", bASE_IUS_Aft_Location );

		if (!bUseASE_IUS)// can't have IUS and Centaur together, IUS has priority
		{
			oapiReadItem_bool( hFile, "UseCISS", bUseCISS );
			oapiReadItem_bool( hFile, "CISS_GPrime", bCISS_GPrime );
		}

		oapiCloseFile(hFile, FILE_IN);
		return true;
	}

	bool Mission::SaveMission(const std::string& strMission) const
	{
		std::string filename;
		filename = "Missions\\SSU\\" + strMission + ".cfg";
		return false;
	}

	bool Mission::SaveMission() const
	{
		return SaveMission(strMissionName);
	}
	

	/*double Mission::GetFirstLandingMET() const
	{
		return std::numeric_limits<double>::infinity();
	}*/

	double Mission::GetLaunchAzimuth() const 
	{
		return 90.0 * RAD;
	}

	/*double Mission::GetLaunchMJD() const
	{
		return fLaunchTimeMJD;
	}*/

	/*unsigned int Mission::GetLaunchSite() const
	{
		return 0;
	}*/

	double Mission::GetMECOInc() const
	{
		return fTargetInc;
	}

	double Mission::GetMECOAlt() const
	{
		return fMECOAlt;
	}

	double Mission::GetMECOVel() const
	{
		return fMECOVel;
	}

	double Mission::GetMECOFPA() const
	{
		return fMECOFPA;
	}

	double Mission::GetMaxSSMEThrust() const
	{
		return fMaxSSMEThrust;
	}

	double Mission::GetPayloadZPos(unsigned int iIndex) const
	{
		if(iIndex > 15)
			return 0.0;
		else
			return fPayloadZPos[iIndex];
	}

	double Mission::GetExternalAirlockZPos() const
	{
		if (bHasTAA & !bAftTAA) return TAA_EXTERNAL_AIRLOCK_Z_POSITION;
		else return EXTERNAL_AIRLOCK_Z_POSITION;
	}

	double Mission::GetTunnelAdapterAssemblyZPos() const
	{
		if (bAftTAA) return TAA_AFT_POSITION;
		else return TAA_FORWARD_POSITION;
	}
		
	const std::string& Mission::GetOrbiter() const
	{
		return strOrbiter;
	}
	
	const std::string& Mission::GetMissionName() const
	{
		return strMissionName;
	}

	const std::string& Mission::GetMissionFileName() const
	{
		return strFileName;
	}

	const std::string& Mission::GetOrbiterTextureName() const
	{
		return strOrbiterTexName;
	}

	/*const std::string& Mission::GetLOMSPodMeshName() const
	{
		return strLOMSPodMeshName;
	}

	const std::string& Mission::GetROMSPodMeshName() const
	{
		return strROMSPodMeshName;
	}*/

	bool Mission::HasBridgerail(unsigned int index) const
	{
		if(index >= 13) return false;
		return bHasBridgerail[index];
	}

	bool Mission::HasRMS() const
	{
		return bUseRMS;
	}

	bool Mission::HasSTBDMPMs() const
	{
		return bHasMPMs;
	}

	bool Mission::HasODS() const
	{
		return bHasODS;
	}

	bool Mission::HasExtAL() const
	{
		return (bHasExtAL || bHasODS);
	}

	bool Mission::HasTAA() const
	{
		return bHasTAA;
	}

	bool Mission::AftTAA() const
	{
		return bAftTAA;
	}

	bool Mission::HasBulkheadFloodlights() const
	{
		return bHasBulkheadFloodlights;
	}

	bool Mission::HasDragChute() const
	{
		return bHasDragChute;
	}

	double Mission::GetOrbiterMass( void ) const
	{
		return OVmass;
	}
	
	bool Mission::UseOMSAssist() const
	{
		return bUseOMSAssist;
	}

	double Mission::GetOMSAssistDuration() const
	{
		return OMSAssistDuration;
	}

	bool Mission::PerformRTHU() const
	{
		return bPerformRTHU;
	}

	double Mission::GetTHdownVelocity() const
	{
		return fTHdown;
	}

	double Mission::GetTHupVelocity() const
	{
		return fTHup;
	}

	bool Mission::HasKUBand() const
	{
		return bHasKUBand;
	}

	bool Mission::UseSILTS() const
	{
		return bUseSILTS;
	}

	bool Mission::LogSSMEData() const
	{
		return bLogSSMEData;
	}

	bool Mission::UseASE_IUS() const
	{
		return bUseASE_IUS;
	}

	bool Mission::IsASELocationAft() const
	{
		return bASE_IUS_Aft_Location;
	}

	bool Mission::UseCISS() const
	{
		return bUseCISS;
	}

	bool Mission::IsCISSGPrime() const
	{
		return bCISS_GPrime;
	}
};
