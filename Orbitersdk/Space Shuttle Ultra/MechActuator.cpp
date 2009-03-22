#include "MechActuator.h"
#include "Atlantis.h"

MechActuator::MechActuator(SubsystemDirector *_pDirect, 
						   const std::string &_ident, 
						   double fStandardTime)
: AtlantisSubsystem(_pDirect, _ident)
{
	fOutPos = 0.0;
	fSpeed = 0.0;
	fLimit0Max = 0.01;
	fLimit1Min = 0.99;
	fHardStopMin = 0.005;
	fHardStopMax = 0.995;
	fAccConstant = 1.0;
	fMaxSpeed = 0.05;
	object_anim = 0xFFFF;
	fSingleMotorFlag = false;
	if(fStandardTime != 0.0)
	{
		SetSingleMotorTime(fStandardTime);
	}
}

MechActuator::~MechActuator()
{
}

void MechActuator::OnPreStep(double fSimT, double fDeltaT, double fMJD)
{
}

void MechActuator::OnPostStep(double fSimT, double fDeltaT, double fMJD)
{
	if(object_anim != 0xFFFF)
	{
		STS()->SetAnimation(object_anim, fOutPos);
	}
}

void MechActuator::OnPropagate(double fSimT, double fDeltaT, double fMJD)
{
	double fAcceleration = 0.0;
	if(CmdDriveFwd.IsSet())
	{
		fAcceleration += fAccConstant;
	} 

	if(CmdDriveRwd.IsSet())
	{
		fAcceleration -= fAccConstant;
	}

	if(fSpeed >= fMaxSpeed)
	{
		fAcceleration = max(fAcceleration, 0.0);
	}

	if(fSpeed <= -fMaxSpeed)
	{
		fAcceleration = min(fAcceleration, 0.0);
	}

	if(fSpeed < 0 && fOutPos <= fHardStopMin)
	{
		fAcceleration = (fSpeed * fSpeed / (2 * fOutPos));
	}

	if(fSpeed > 0 && fOutPos >= fHardStopMax)
	{
		fAcceleration = -(fSpeed * fSpeed / (2 * (1.0 - fOutPos)));
	}

	double fOldSpeed = fSpeed;

	fSpeed += fAcceleration * fDeltaT;
	
	if(fSpeed > fMaxSpeed)
		fSpeed = fMaxSpeed;
	if(fSpeed < -fMaxSpeed)
		fSpeed = -fMaxSpeed;

	fOutPos = (fOldSpeed + 0.5 * fAcceleration * fDeltaT) * fDeltaT;

	if(fOutPos >= 1.0) {
		fOutPos = 1.0;
		fSpeed = 0.0;
	}

	if(fOutPos <= 0.0) {
		fOutPos = 0.0;
		fSpeed = 0.0;
	}

	// -------------------------------------------------------
	// Set output discretes
	// -------------------------------------------------------

	if(fOutPos <= fLimit0Max) 
		PosLimit0.SetLine();
	else
		PosLimit0.ResetLine();

	if(fOutPos >= fLimit1Min) 
		PosLimit1.SetLine();
	else
		PosLimit1.ResetLine();

}

void MechActuator::SetMicroSwitchPositions(double fLimit0, double fLimit1)
{
	fLimit0Max = fLimit0;
	fLimit1Min = fLimit1;
}

void MechActuator::SetObjectAnim(UINT anim)
{
	object_anim = anim;
}

void MechActuator::SetSingleMotorTime(double fTimeSingle)
{
	if(fTimeSingle > 0.0)
	{
		fMaxSpeed = 1.0/fTimeSingle;
	} 
}

void MechActuator::SetState(double fState0)
{
	fOutPos = fState0;
}