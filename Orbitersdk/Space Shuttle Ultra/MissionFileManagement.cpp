#include "Atlantis.h"
#include <map>

std::map<std::string, mission::Mission*> ssu_mission_hashmap;

DLLCLBK mission::Mission* ssuGetMission(const std::string& mission_name)
{
	if(ssu_mission_hashmap.find(mission_name) != ssu_mission_hashmap.end())
	{
		return ssu_mission_hashmap[mission_name];
	} else {
		mission::Mission* new_mission = new mission::Mission(mission_name);
		ssu_mission_hashmap.insert(std::make_pair(mission_name, new_mission));
		return new_mission;
	}
}

void ClearMissionManagementMemory()
{
	std::map<std::string, mission::Mission*>::iterator iter
		= ssu_mission_hashmap.begin();
	while(iter != ssu_mission_hashmap.end())
	{
		if(iter->second)
		{
			delete iter->second;
			iter->second = NULL;
		}
		iter++;
	}
}