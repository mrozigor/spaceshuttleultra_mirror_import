#ifndef __PIDCONTROL_H
#define __PIDCONTROL_H
#pragma once

/**
 * Performs math required for a PID control system.
 */
class PIDControl
{
private:
	double lastValue;
	double integral;
	double proportionalGain, derivativeGain, integralGain;
	bool firstStep;
public:
	PIDControl(double pGain=0.0, double dGain=0.0, double iGain=0.0);
	~PIDControl();

	void Reset();
	/**
	 * Returns control output for the current step.
	 * Uses trapezoidal rule to calculate integral.
	 * @param value value of input variable
	 * @param DeltaT time since last step
	 */
	double Step(double value, double DeltaT);

	void SetGains(double pGain, double dGain, double iGain);
};

#endif //__PIDCONTROL_H