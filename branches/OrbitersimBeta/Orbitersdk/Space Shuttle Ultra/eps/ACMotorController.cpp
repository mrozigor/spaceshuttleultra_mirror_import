#include "eps_defs.h"

namespace eps {
	
	ACMotorController::ACMotorController(const std::string &name)
		:strName (name)
	{
		fPowerP1[0] = fPowerP1[1] = 0.0;
		fPowerP2[0] = fPowerP2[1] = 0.0;
		fPowerP3[0] = fPowerP3[1] = 0.0;;
	}

	ACMotorController::~ACMotorController()
	{
	}

	void ACMotorController::acPropagate(double fSimT, double fDeltaT)
	{
		fPowerP1[0] = fPowerP1[1];
		fPowerP2[0] = fPowerP2[1];
		fPowerP3[0] = fPowerP3[1];

		fPowerP1[1] = 0.0;
		fPowerP2[1] = 0.0;
		fPowerP3[1] = 0.0;
	}

	bool ACMotorController::ConnectToSource(IACElement *pSource)
	{
		sources.push_back(pSource);
		return true;
	}

	bool ACMotorController::DrawPower(double fPower)
	{
		return DrawPower(PHASE1, fPower/3) && DrawPower(PHASE2, fPower/3) && DrawPower(PHASE3, fPower/3);
	}

	bool ACMotorController::DrawPower(ACPHASE phase, double fPower)
	{
		double fTotalVolt = 0;
		double fFactor = 0;

		if(sources.empty())
			return false;

		std::vector<IACElement*>::const_iterator iter = sources.begin();

		while(iter != sources.end())
		{
			fTotalVolt += fabs((*iter)->GetVoltage(phase));
			iter++;
		}

		//Take power
		iter = sources.begin();

		while(iter != sources.end()) {
			fFactor = fabs((*iter)->GetVoltage(phase)) / fTotalVolt;
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

	double ACMotorController::GetAmps(eps::ACPHASE phase) const 
	{
		double fV = GetVoltage(phase) * GetFlowDirection();
		if(fV == 0.0)
		{
			return 0.0;
		}

		switch(phase)
		{
		case PHASE1:
			return fPowerP1[0] / fV;
		case PHASE2:
			return fPowerP2[0] / fV;
		case PHASE3:
			return fPowerP3[0] / fV;
		default:
			return 0.0;
		}
	}

	double ACMotorController::GetPhaseAngle(eps::ACPHASE phase) const
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
		return (fPhaseAngle * GetFlowDirection())/sources.size();
	}

	double ACMotorController::GetPhaseDrift(eps::ACPHASE phase) const
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
		return (fDeltaPhase * GetFlowDirection())/sources.size();
	}

	double ACMotorController::GetVoltage(eps::ACPHASE phase) const 
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

			return (fVoltage * GetFlowDirection())/sources.size();
		}
		else
			return 0.0;
	}

	float ACMotorController::GetFlowDirection() const
	{
		if((CmdFwd && CmdRwd) || (!CmdFwd && !CmdRwd))
			return 0.0;
		else if (CmdFwd)
			return 1.0;
		else if(CmdRwd)
			return -1.0;
		else
			return 0.0;

	}
};