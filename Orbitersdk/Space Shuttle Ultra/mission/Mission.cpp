#include "Mission.h"
#include "OrbiterAPI.h"
#include <limits>

namespace mission {

	Mission::Mission()
	{
	}

	Mission::Mission(const std::string& strMission)
	{
		if(!LoadMission(strMission))
		{
			oapiWriteLog("(Mission) failed creating mission from loaded file.");
		}
	}

	Mission::~Mission()
	{
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
		else strOrbiter = "";

		if(!oapiReadItem_bool(hFile, "EnableWingPainting", bEnableWingPainting))
		{
			 bEnableWingPainting = false;
		}

		if(oapiReadItem_string(hFile, "OrbiterTexture", buffer))
		{
			strOrbiterTexName = "SSU\\" + std::string(buffer) + ".dds";
			oapiWriteLog((char*)strOrbiterTexName.c_str());
		}
		else strOrbiterTexName = "";

		if(!oapiReadItem_float(hFile, "LTime", fLTimeMJD))
		{
			fLTimeMJD = -1.0;
		}

		
		if(!oapiReadItem_float(hFile, "FirstReturnOpport", fLandTimeMJD))
		{
			fLandTimeMJD = -1.0;
		}

		if(!oapiReadItem_float(hFile, "TargetInc", fTargetInc))
		{
			oapiWriteLog("(Mission) MECO inclination not found, using default value.");
			fTargetInc = 28.5;
		}

		if(!oapiReadItem_float(hFile, "MECOAlt", fMECOAlt))
			fMECOAlt = 105000;
		if(!oapiReadItem_float(hFile, "MECOVel", fMECOVel))
			fMECOVel = 7869.635088;
		if(!oapiReadItem_float(hFile, "MECOFPA", fMECOFPA))
			fMECOFPA = 0.747083;

		fMECOFPA *= RAD;
		fTargetInc *= RAD;
		
		/*if(!oapiReadItem_bool(hFile, "UseOMSAssist", bUseOMSAssist))
		{
			bUseOMSAssist = false;
		}*/

		if(oapiReadItem_float(hFile, "OMSAssistStart", OMSAssistStart) && oapiReadItem_float(hFile, "OMSAssistEnd", OMSAssistEnd))
		{
			bUseOMSAssist = true;
		}
		else bUseOMSAssist = false;

		if(!oapiReadItem_float(hFile, "RollToHeadsUpStartVelocity", RTHUVelocity))
		{
			RTHUVelocity = 100000.0; // velocity faster than orbital speed; RTHU will not occur
		}

		if(!oapiReadItem_float(hFile, "MaxSSMEThrust", fMaxSSMEThrust))
		{
			fMaxSSMEThrust = 104.5;
		}

		if(!oapiReadItem_float(hFile, "ThrottleDown", fTHdown))
		{
			fTHdown = 834.0;
		}

		if(!oapiReadItem_float(hFile, "ThrottleUp", fTHup))
		{
			fTHup = 1174.0;
		}

		if(!oapiReadItem_bool(hFile, "UseRMS", bUseRMS))
		{
			bUseRMS = false;
		}

		if(!oapiReadItem_bool(hFile, "UseKUBand", bHasKUBand))
		{
			bHasKUBand = true;
		}

		if(!oapiReadItem_bool(hFile, "UseSTBDMPM", bHasMPMs))
		{
			bHasMPMs = false;
		}
		if(!oapiReadItem_bool(hFile, "UseODS", bHasODS))
		{
			bHasODS = false;
		}
		if(!oapiReadItem_bool(hFile, "UseExtAL", bHasExtAL))
		{
			bHasExtAL = false;
		}

		for(int i = 0; i<16; i++)
		{
			double x;
			sprintf_s(buffer, "PayloadZPos%d", i);
			if(!oapiReadItem_float(hFile, buffer, x))
			{
				fPayloadZPos[i] = DEFAULT_PAYLOAD_ZPOS[i];
			} else {
				fPayloadZPos[i] = x;
			}
		}
		if(!oapiReadItem_float(hFile, "ODSZPos", fODSZPos)) {
			fODSZPos = 10.1529;
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
		return fLTimeMJD;
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

	bool Mission::WingPaintingEnabled() const
	{
		return bEnableWingPainting;
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

	bool Mission::UseDirectAscent() const
	{
		return true;
	}
	
	bool Mission::UseOMSAssist() const
	{
		return bUseOMSAssist;
	}

	double Mission::GetOMSAssistStart() const
	{
		return OMSAssistStart;
	}

	double Mission::GetOMSAssistEnd() const
	{
		return OMSAssistEnd;
	}

	double Mission::GetRTHUVelocity() const
	{
		return RTHUVelocity;
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
};