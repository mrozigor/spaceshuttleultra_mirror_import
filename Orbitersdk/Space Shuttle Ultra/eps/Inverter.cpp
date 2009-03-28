#include "eps_defs.h"

namespace eps {

	Inverter::Inverter(SubsystemDirector* _direct, const std::string& name)
		: AtlantisSubsystem(_direct, name)
	{
		fPowerP1[0] = fPowerP1[1] = 0.0;
		fPowerP2[0] = fPowerP2[1] = 0.0;
		fPowerP3[0] = fPowerP3[1] = 0.0;
	}

	Inverter::~Inverter()
	{
	}

	
	void Inverter::acPropagate(double fSimT, double fDeltaT)
	{
		fPowerP1[0] = fPowerP1[1];
		fPowerP2[0] = fPowerP2[1];
		fPowerP3[0] = fPowerP3[1];

		fPowerP1[1] = 0.0;
		fPowerP2[1] = 0.0;
		fPowerP3[1] = 0.0;
	}

	bool Inverter::ConnectToSource(eps::IACElement *pSource)
	{
		//Inverter is ultimative AC source
		return false;
	}

	bool Inverter::DrawPower(double fPower) {
		return DrawPower(PHASE1, fPower/3) 
			&& DrawPower(PHASE2, fPower/3) 
			&& DrawPower(PHASE3, fPower/3);
	}

	bool Inverter::DrawPower(ACPHASE phase, double fPower)
	{
		//Always deliver full power
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
		default:
			return false;
		}
	}

	double Inverter::GetAmps(ACPHASE phase) const 
	{
		switch(phase)
		{
		case PHASE1:
			return fPowerP1[0]/120.0;
		case PHASE2:
			return fPowerP2[0]/120.0;
		case PHASE3:
			return fPowerP3[0]/120.0;
		default:
			return 0.0;
		}
	}

	double Inverter::GetPhaseAngle(ACPHASE phase) const 
	{
		return 0.0;
	}

	double Inverter::GetPhaseDrift(eps::ACPHASE phase) const
	{
		return 400.0*2*PI;
	}


	double Inverter::GetVoltage(eps::ACPHASE phase) const {
		//constant voltage output
		return 120.0;
	}

	void Inverter::OnPostStep(double fSimT, double fDeltaT, double fMJD)
	{
	}

	void Inverter::OnPreStep(double fSimT, double fDeltaT, double fMJD)
	{
	}

	void Inverter::OnPropagate(double fSimT, double fDeltaT, double fMJD)
	{
		acPropagate(fSimT, fDeltaT);
	}

};