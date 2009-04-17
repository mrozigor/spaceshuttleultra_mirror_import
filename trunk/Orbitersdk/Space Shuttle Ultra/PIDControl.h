/****************************************************************************
  This file is part of Space Shuttle Ultra

  PID Control loop definition



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/
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

	/**
	 * Resets lastValue and integral variables from previous timesteps.
	 * Generally called after gains are changed.
	 */
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