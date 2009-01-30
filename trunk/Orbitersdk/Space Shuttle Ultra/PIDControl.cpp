#include "PIDControl.h"

PIDControl::PIDControl(double pGain, double dGain, double iGain)
	: integral(0.0), lastValue(0.0), firstStep(true)
{
	SetGains(pGain, dGain, iGain);
}

PIDControl::~PIDControl()
{
}

void PIDControl::Reset()
{
	integral=0.0;
	lastValue=0.0;
	firstStep=true;
}

double PIDControl::Step(double value, double DeltaT)
{
	double derivative=0.0;
	
	if(!firstStep) {
		derivative=(value-lastValue)/DeltaT;
		//for the moment, use trapezoidal rule to calculate integral
		integral+=DeltaT*((value+lastValue)/2.0);
	}
	else firstStep=false;
	//store value for next timestep
	lastValue=value;

	double output=proportionalGain*value+derivativeGain*derivative+integralGain*integral;
	return output;
}

void PIDControl::SetGains(double pGain, double dGain, double iGain)
{
	proportionalGain=pGain;
	derivativeGain=dGain;
	integralGain=iGain;
}