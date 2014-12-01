#include "Mission.h"
#include "OrbiterAPI.h"
#include <limits>

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
		strLOMSPodMeshName = "SSU\\LOMS_pod_standard";
		strROMSPodMeshName = "SSU\\ROMS_pod_standard";
		fLaunchTimeMJD = -1.0;
		fLandTimeMJD = -1.0;

		fTargetInc = 28.5*RAD;
		fMECOAlt = 105000;
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

		bUseRMS = false;
		bHasKUBand = true;
		bHasMPMs = false;
		bHasODS = false;
		bHasExtAL = false;
		bHasBulkheadFloodlights = false;
		bHasDragChute = true;

		bUseSILTS = false;

		for(int i=0;i<16;i++) fPayloadZPos[i] = DEFAULT_PAYLOAD_ZPOS[i];
		fODSZPos = 8.25;
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

		if(oapiReadItem_string(hFile, "OrbiterTexture", buffer))
		{
			strOrbiterTexName = "SSU\\" + std::string(buffer) + ".dds";
			oapiWriteLog((char*)strOrbiterTexName.c_str());
		}

		if(oapiReadItem_string(hFile, "LOMSPodMesh", buffer))
		{
			strLOMSPodMeshName = "SSU\\" + std::string(buffer);
		}
		if(oapiReadItem_string(hFile, "ROMSPodMesh", buffer))
		{
			strROMSPodMeshName = "SSU\\" + std::string(buffer);
		}

		oapiReadItem_float(hFile, "LTime", fLaunchTimeMJD);
		
		oapiReadItem_float(hFile, "FirstReturnOpport", fLandTimeMJD);

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
		double fTemp;
		if(oapiReadItem_float(hFile, "RollToHeadsUpStartVelocity", fTemp)) bPerformRTHU = true; // hack to handle old mission files (when RTHU velocity was specified in the mission file)
		
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
		oapiReadItem_bool(hFile, "HasBulkheadFloodlights", bHasBulkheadFloodlights);
		oapiReadItem_bool(hFile, "HasDragChute", bHasDragChute);

		for(int i = 0; i<16; i++)
		{
			double x;
			sprintf_s(buffer, "PayloadZPos%d", i);
			if(oapiReadItem_float(hFile, buffer, x))
			{
				fPayloadZPos[i] = x;
			}
		}
		oapiReadItem_float(hFile, "ODSZPos", fODSZPos);

		if (strOrbiter == "Columbia") oapiReadItem_bool( hFile, "SILTS", bUseSILTS );

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
	

	double Mission::GetFirstLandingMET() const
	{
		return std::numeric_limits<double>::infinity();
	}

	double Mission::GetLaunchAzimuth() const 
	{
		return 90.0 * RAD;
	}

	double Mission::GetLaunchMJD() const
	{
		return fLaunchTimeMJD;
	}

	unsigned int Mission::GetLaunchSite() const
	{
		return 0;
	}

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

	unsigned int Mission::GetNumberOfOMSBurns() const
	{
		return 0;
	}

	double Mission::GetPayloadZPos(unsigned int iIndex) const
	{
		if(iIndex > 15)
			return 0.0;
		else
			return fPayloadZPos[iIndex];
	}

	double Mission::GetODSZPos() const
	{
		return fODSZPos;
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

	const std::string& Mission::GetLOMSPodMeshName() const
	{
		return strLOMSPodMeshName;
	}

	const std::string& Mission::GetROMSPodMeshName() const
	{
		return strROMSPodMeshName;
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

	bool Mission::HasBulkheadFloodlights() const
	{
		return bHasBulkheadFloodlights;
	}

	bool Mission::HasDragChute() const
	{
		return bHasDragChute;
	}

	bool Mission::UseDirectAscent() const
	{
		return true;
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
};
