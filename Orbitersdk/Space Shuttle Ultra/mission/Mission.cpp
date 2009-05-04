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
		FILEHANDLE hFile = oapiOpenFile(filename.c_str(), FILE_IN);

		if(hFile == NULL)
			return false;

		if(oapiReadItem_string(hFile, "Name", buffer))
		{
			strMissionName = buffer;
		}

		if(oapiReadItem_string(hFile, "Orbiter", buffer))
		{
			strOrbiter = buffer;
		}

		if(!oapiReadItem_float(hFile, "LTime", fLTimeMJD))
		{
			fLTimeMJD = -1.0;
		}

		
		if(!oapiReadItem_float(hFile, "FirstReturnOpport", fLandTimeMJD))
		{
			fLandTimeMJD = -1.0;
		}

		if(!oapiReadItem_float(hFile, "TargetInc", fTargetInc))
			fTargetInc = 28.5;

		if(!oapiReadItem_float(hFile, "MECOAlt", fMECOAlt))
			fMECOAlt = 105000;
		if(!oapiReadItem_float(hFile, "MECOVel", fMECOVel))
			fMECOVel = 7869.635088;
		if(!oapiReadItem_float(hFile, "MECOFPA", fMECOFPA))
			fMECOFPA = 0.747083;

		fMECOFPA *= RAD;
		fTargetInc *= RAD;
		
		if(!oapiReadItem_bool(hFile, "UseOMSAssist", bUseOMSAssist))
		{
			bUseOMSAssist = false;
		}

		if(!oapiReadItem_bool(hFile, "UseRMS", bUseRMS))
		{
			bUseRMS = false;
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

	unsigned int Mission::GetNumberOfOMSBurns() const
	{
		return 0;
	}
		
	const std::string& Mission::GetOrbiter() const
	{
		return strOrbiter;
	}
	
	const std::string& Mission::GetMissionName() const
	{
		return strMissionName;
	}
	
	bool Mission::HasKuBandAntenna() const
	{
		return true;
	}

	bool Mission::UseDirectAscent() const
	{
		return true;
	}
	
	bool Mission::UseOMSAssist() const
	{
		return false;
	}
};