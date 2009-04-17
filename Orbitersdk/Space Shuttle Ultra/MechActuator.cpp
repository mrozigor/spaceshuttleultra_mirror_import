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
	fMechMI = 1.0;
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
	char cCP = ' ';

#ifdef DEBUG_PROPAGATE
	oapiWriteLog("(MechActuator::OnPropagate) Enter.");
#endif

	
#ifdef DEBUG_PROPAGATE
	oapiWriteLog("(MechActuator::OnPropagate) Line 59");
#endif

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
		fAcceleration = (pow(fSpeed,2) / (2 * fOutPos));
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

#ifdef DEBUG_PROPAGATE
	oapiWriteLog("(MechActuator::OnPropagate) Line 92");
#endif

	if(fOutPos >= 1.0) {
		fOutPos = 1.0;
		fSpeed = 0.0;
	}

	if(fOutPos <= 0.0) {
		fOutPos = 0.0;
		fSpeed = 0.0;
	}

	double fL1 = 0;
	double fL2 = 0;
	double fL3 = 0;
	double fL4 = 0;
	double fL = 0;


	for(int i = 0; i<2; ++i)
	{
		fL1 += system[i].GetTorque(system[i].GetSpeed());
	}

	for(int i = 0; i<2; ++i)
	{
		fL2 += system[i].GetTorque(system[i].GetSpeed() + 0.5 * fDeltaT * fL1/fMechMI);
	}

	for(int i = 0; i<2; ++i)
	{
		fL3 += system[i].GetTorque(system[i].GetSpeed() + 0.5 * fDeltaT * fL2/fMechMI);
	}
	for(int i = 0; i<2; ++i)
	{
		fL4 += system[i].GetTorque(system[i].GetSpeed() + fDeltaT * fL3/fMechMI);
	}


	fL += 1/6.0 * (fL1 + 2*fL2 + 2*fL3 + fL4);
	

	// -------------------------------------------------------
	// Set output discretes
	// -------------------------------------------------------

	char cP0 = ' ', cP1 = ' ';

	if(fOutPos <= fLimit0Max)
	{
		cP0 ='X';
		PosLimit0.SetLine();
	}
	else
		PosLimit0.ResetLine();
#ifdef DEBUG_PROPAGATE
	oapiWriteLog("(MechActuator::OnPropagate) Line 118");
#endif

	if(fOutPos >= fLimit1Min) 
	{
		cP1 = 'X';
		PosLimit1.SetLine();
	}
	else
		PosLimit1.ResetLine();
#ifdef DEBUG_PROPAGATE
	oapiWriteLog("(MechActuator::OnPropagate) Line 128");
#endif

	sprintf_s(oapiDebugString(), 255, "(MCA{%s}::OnPropagate) POS: %5.2f%% SPEED: %5.2f%%/s (%c [%c %c])",
		GetQualifiedIdentifier().c_str(), fOutPos*100.0, fSpeed * 100.0,
		cCP, cP0, cP1);
#ifdef DEBUG_PROPAGATE
	oapiWriteLog("(MechActuator::OnPropagate) Exit.");
#endif
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