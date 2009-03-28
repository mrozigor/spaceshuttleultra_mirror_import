#include "eps_defs.h"

namespace eps {

	ACBus::ACBus(const std::string& name)
		: strName(name)
	{
	}

	ACBus::~ACBus()
	{
	}

	double ACBus::GetVoltage(ACPHASE phase) const
	{
		if(!sources.empty()) 
		{
			double fVoltage = 0.0;

			std::vector<IACElement*>::const_iterator iter = sources.begin();
			while(iter != sources.end())
			{
				fVoltage += (*iter)->GetVoltage(phase);
				iter++;
			}

			return fVoltage/sources.size();
		}
		else
			return 0.0;
	}
	
	double ACBus::GetPhaseAngle(ACPHASE phase) const
	{
		double fPhaseAngle = 0.0;

		if(sources.empty())
			return 0.0;

		std::vector<IACElement*>::const_iterator iter = sources.begin();
		while(iter != sources.end())
		{
			fPhaseAngle += (*iter)->GetPhaseAngle(phase);
			iter++;
		}
		return fPhaseAngle/sources.size();
	}

	double ACBus::GetPhaseDrift(ACPHASE phase) const
	{
		double fDeltaPhase = 0.0;
		if(sources.empty())
			return 0.0;

		std::vector<IACElement*>::const_iterator iter = sources.begin();
		while(iter != sources.end())
		{
			fDeltaPhase += (*iter)->GetPhaseDrift(phase);
			iter++;
		}
		return fDeltaPhase/sources.size();
	}

		/** 
		 * either amps produced by a power source or amps flowing 
		 * through a node
		 */
	double ACBus::GetAmps(ACPHASE phase) const
	{
		double fV = GetVoltage(phase);
		if(fV == 0.0)
			return 0.0;

		switch(phase)
		{
		case PHASE1:
			return fPowerP1[0]/fV;
		case PHASE2:
			return fPowerP2[0]/fV;
		case PHASE3:
			return fPowerP3[0]/fV;
		default:
			return 0.0;
		}
	}

	bool ACBus::DrawPower(ACPHASE phase, double fPower)
	{
		double fTotalVolt = 0;
		double fFactor = 0;

		if(sources.empty())
			return false;

		std::vector<IACElement*>::const_iterator iter = sources.begin();

		while(iter != sources.end())
		{
			fTotalVolt += (*iter)->GetVoltage(phase);
			iter++;
		}

		//Take power
		iter = sources.begin();

		while(iter != sources.end()) {
			fFactor = (*iter)->GetVoltage(phase) / fTotalVolt;
			(*iter)->DrawPower(phase, fPower * fFactor);
			iter++;
		}

		switch(phase)
		{
		case PHASE1:
			fPowerP1[1] += fPower;
			return true;
		case PHASE2:
			fPowerP2[1] += fPower;
			return true;
		case PHASE3:
			fPowerP3[1] += fPower;
			return true;
		}
		return false;
	}

	bool ACBus::DrawPower(double fPower)
	{
		return DrawPower(PHASE1, fPower/3) && DrawPower(PHASE2, fPower/3) && DrawPower(PHASE3, fPower/3);
	}

	bool ACBus::ConnectToSource(IACElement* pSource)
	{
		sources.push_back(pSource);
		return true;
	}

	void ACBus::acPropagate(double fSimT, double fDeltaT)
	{
		fPowerP1[0] = fPowerP1[1];
		fPowerP2[0] = fPowerP2[1];
		fPowerP3[0] = fPowerP3[1];

		fPowerP1[1] = 0.0;
		fPowerP2[1] = 0.0;
		fPowerP3[1] = 0.0;
	}

	const std::string& ACBus::GetName() const
	{
		return strName;
	}
};