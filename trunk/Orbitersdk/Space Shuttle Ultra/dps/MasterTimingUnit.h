/****************************************************************************
  This file is part of Space Shuttle Ultra

  Master Timing Unit subsystem definition



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
#if !defined(AFX_MASTERTIMINGUNIT_H__FEF22983_033D_412B_B3A9_876EE3A3EC04__INCLUDED_)
#define AFX_MASTERTIMINGUNIT_H__FEF22983_033D_412B_B3A9_876EE3A3EC04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dps_defs.h"
#include "..\AtlantisSubsystem.h"

namespace dps {

typedef short MTU_ACCU_INDEX;
typedef short MTU_EVTTMR_INDEX;

const MTU_EVTTMR_INDEX TIMER_FORWARD = 0;
const MTU_EVTTMR_INDEX TIMER_AFT = 1;

typedef enum {
	COUNT_UP,
	COUNT_DOWN,
	COUNT_TEST
} EVENT_TIMER_MODE;

typedef enum {
	COUNT_STOPPED,
	COUNT_STARTED
} EVENT_TIMER_CONTROL;

class TimePort {
	unsigned short usPulseCount;
	unsigned short usPulsesPerTimeStep;
	IRIGB_FRAME fdat;
public:
	TimePort();
	unsigned short GetDay() const;
	unsigned short GetHours() const;
	unsigned short GetMinutes() const; 
	unsigned short GetSeconds() const;
};

class TimeBus {
public:
	TimeBus(MasterTimingUnit* pMTU, TimePort* pTarget);
};


class MasterTimingUnit : public AtlantisSubsystem  
{
	double fGMT[3][2];
	double fMET[3][2];
	double fEvent[2][2];	//Accumulator
	short sEventMinutes[2];
	short sEventSeconds[2];

	short sGMTMillis[3];
	short sGMTSeconds[3];
	short sGMTMinutes[3];
	short sGMTHours[3];
	short sGMTDays[3];

	short sMETMillis[3];
	short sMETSeconds[3];
	short sMETMinutes[3];
	short sMETHours[3];
	short sMETDays[3];
	
	EVENT_TIMER_MODE event_mode[2][2];
	EVENT_TIMER_CONTROL event_control[2][2];

	bool bMETCounting[2];
public:
	MasterTimingUnit(SubsystemDirector* _director);
	virtual ~MasterTimingUnit();

	short GetEventTimerSec(MTU_EVTTMR_INDEX timer) const;
	short GetEventTimerMin(MTU_EVTTMR_INDEX timer) const;

	void SetEventTimer(MTU_EVTTMR_INDEX timer, short Minutes, short Seconds);
	void StartEventTimer(MTU_EVTTMR_INDEX timer);
	void StopEventTimer(MTU_EVTTMR_INDEX timer);
	
	void SetEventTimerMode(MTU_EVTTMR_INDEX timer, EVENT_TIMER_MODE mode);

	void ResetEventTimer(MTU_EVTTMR_INDEX timer);

	short GetMETMilli(MTU_ACCU_INDEX accu_index) const;
	short GetMETSec(MTU_ACCU_INDEX accu_index) const;
	short GetMETMin(MTU_ACCU_INDEX accu_index) const;
	short GetMETHour(MTU_ACCU_INDEX accu_index) const;
	short GetMETDay(MTU_ACCU_INDEX accu_index) const;

	void ResetMET();
	void StartMET();

	short GetGMTMilli(MTU_ACCU_INDEX accu_index) const;
	short GetGMTSec(MTU_ACCU_INDEX accu_index) const;
	short GetGMTMin(MTU_ACCU_INDEX accu_index) const;
	short GetGMTHour(MTU_ACCU_INDEX accu_index) const;
	short GetGMTDay(MTU_ACCU_INDEX accu_index) const;

	virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
	virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
	virtual void OnSaveState(FILEHANDLE scn) const;
	virtual bool OnParseLine(const char* keyword, const char* line);

};

};

#endif // !defined(AFX_MASTERTIMINGUNIT_H__FEF22983_033D_412B_B3A9_876EE3A3EC04__INCLUDED_)
