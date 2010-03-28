#include "eps_defs.h"

namespace eps {
	ACBusSystem::ACBusSystem(SubsystemDirector* _direct)
		: AtlantisSubsystem(_direct, "ACBusSystem")
	{
	}

	ACBusSystem::~ACBusSystem()
	{
		std::vector<ACBus*>::iterator iter = bus_systems.begin();
		while(iter != bus_systems.end())
		{
			delete (*iter);
			iter++;
		}
		bus_systems.clear();
	}

	void ACBusSystem::OnPreStep(double fSimT, double fDeltaT, double fMJD)
	{
	}

	void ACBusSystem::OnPostStep(double fSimT, double fDeltaT, double fMJD)
	{
	}

	void ACBusSystem::OnPropagate(double fSimT, double fDeltaT, double fMJD)
	{
		std::vector<ACBus*>::iterator iter = bus_systems.begin();
		while(iter != bus_systems.end())
		{
			(*iter)->acPropagate(fSimT, fDeltaT);
			iter++;
		}
	}

	ACBus* ACBusSystem::CreateBus(const std::string &name)
	{
		ACBus* pBus = new ACBus(name);
		bus_systems.push_back(pBus);
		return pBus;
	}

	ACBus* ACBusSystem::GetBus(const std::string& name) 
	{
		ACBus* pBus = FindBus(name);
		if(pBus)
		{
			return pBus;
		} else {
			return CreateBus(name);
		}
		
	}

	ACBus* ACBusSystem::FindBus(const std::string &name) const
	{
		std::vector<ACBus*>::const_iterator iter = bus_systems.begin();
		while(iter != bus_systems.end()) {
			if((*iter)->GetName() == name)
			{
				return *iter;
			}
			iter++;
		}
		return NULL;
	}

	bool ACBusSystem::ExistsBus(const std::string &name) const
	{
		std::vector<ACBus*>::const_iterator iter = bus_systems.begin();
		while(iter != bus_systems.end()) {
			if((*iter)->GetName() == name)
			{
				return true;
			}
			iter++;
		}
		return false;
	}
};