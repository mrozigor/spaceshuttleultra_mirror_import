#include "eps_defs.h"

namespace eps {

	const double FIELD_SPEED_CONSTANT = 2.0/8.0;
	const double MOTOR_SPEED_CONSTANT = 0.25;
	const double MOTOR_TORQUE_CONSTANT = 4.0;
	const double MOTOR_BRAKE_CONSTANT = 50.0;

	ACMotor::ACMotor()
		: pACSource(NULL)
	{
		fMotorSpeed = 0.0;
	}

	double ACMotor::GetVoltage(ACPHASE phase) const 
	{
		return 0.0;
	}

	double ACMotor::GetPhaseAngle(ACPHASE phase) const 
	{
		return 0.0;
	}

	double ACMotor::GetPhaseDrift(ACPHASE phase) const 
	{
		return 0.0;
	}
	/** 
	 * either amps produced by a power source or amps flowing 
	 * through a node
	 */
	double ACMotor::GetAmps(ACPHASE phase) const 
	{
		return 0.0;
	}

	bool ACMotor::DrawPower(ACPHASE phase, double fPower) 
	{
		return false;
	}

	bool ACMotor::DrawPower(double fPower) {
		return false;
	}

	bool ACMotor::ConnectToSource(IACElement* pSource)
	{
		if(pACSource != NULL)
			return false;
		else {
			pACSource = pSource;
			return true;
		}
	}

	void ACMotor::acPropagate(double fSimT, double fDeltaT) 
	{

	}

	double ACMotor::GetTorque(double fSpeed) const 
	{
		
		if(pACSource == NULL)
			return 0.0;

		double fMaxSpeed = pACSource->GetPhaseDrift(PHASE1) * MOTOR_SPEED_CONSTANT;
		double fT = pACSource->GetVoltage(PHASE1) * MOTOR_TORQUE_CONSTANT;
		fT *= (1.0 - fSpeed/fMaxSpeed);

		//Brake torque
		if(length(_V(pACSource->GetVoltage(PHASE1), pACSource->GetVoltage(PHASE2), pACSource->GetVoltage(PHASE3))) < 50.0) 
		{
			fT = -fSpeed * MOTOR_BRAKE_CONSTANT;
		}

		return fT;
	}

	void ACMotor::SetSpeed(double fSpeed, double fTorque0, 
		double fTorque1, double fDeltaT)
	{
		if(pACSource == NULL)
			return;

		double fMagneticFieldVel = pACSource->GetPhaseDrift(PHASE1) * FIELD_SPEED_CONSTANT;

		double fPower = 0.25 * (fSpeed + fMotorSpeed) * (fTorque0 + fTorque1) * fDeltaT;
		
		fMotorSpeed = fSpeed;
	}

	double ACMotor::GetSpeed() const
	{
		return fMotorSpeed;
	}


};
