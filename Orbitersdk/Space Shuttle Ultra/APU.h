/****************************************************************************
  This file is part of Space Shuttle Ultra

  APU simulation definition



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
#ifndef __APU_H
#define __APU_H
#pragma once

#include "AtlantisSubsystem.h"
#include "discsignals/DiscInPort.h"
#include "discsignals/DiscOutPort.h"

using class ::discsignals::DiscInPort;

class APU: public AtlantisSubsystem
{
public:
	APU(SubsystemDirector* _director, const string& _ident, int _ID);
	~APU();

	//functions for CRT MFD APU/HYD display
	virtual double GetHydraulicPressure() const;
	virtual double GetFuelLevel() const;
	virtual double GetFuelPressure() const;
	virtual double GetAPUSpeed() const;
	bool IsRunning() const;
	virtual bool SingleParamParseLine() const {return true;};

	void CreateTanks();
	virtual void Realize();

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	virtual void OnPostStep(double SimT, double DeltaT, double MJD);
	virtual void OnPropagate(double SimT, double DeltaT, double MJD);

	virtual void OnSaveState(FILEHANDLE scn) const;
	virtual bool OnParseLine(const char* line);

	typedef enum {OFF, START, ON, SHUTDOWN} APU_STATE;
private:
	bool ReadyToStart();

	int ID;

	double FuelLevel[2];
	double HydraulicPressure[2];
	double FuelPress[2];
	double APUSpeed[2];

	APU_STATE State;
	PROPELLANT_HANDLE phTank;

	DiscInPort APU_Run, APU_HydPumpPress;
	DiscInPort APU_CntlrPwr, APU_FuelTankValves;
	DiscOutPort APU_HydraulicPress;
	DiscOutPort APU_ReadyToStart;
};

#endif //__APU_H