// MasterTimingUnit.h: Schnittstelle für die Klasse MasterTimingUnit.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASTERTIMINGUNIT_H__FEF22983_033D_412B_B3A9_876EE3A3EC04__INCLUDED_)
#define AFX_MASTERTIMINGUNIT_H__FEF22983_033D_412B_B3A9_876EE3A3EC04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtlantisSubsystem.h"

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

	bool fMET_counting[2];
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

	void ResetMET(MTU_ACCU_INDEX accu_index) const;

	short GetGMTMilli(MTU_ACCU_INDEX accu_index) const;
	short GetGMTSec(MTU_ACCU_INDEX accu_index) const;
	short GetGMTMin(MTU_ACCU_INDEX accu_index) const;
	short GetGMTHour(MTU_ACCU_INDEX accu_index) const;
	short GetGMTDay(MTU_ACCU_INDEX accu_index) const;

	virtual void PreStep(double fSimT, double fDeltaT);
	virtual void Propagate(double fSimT, double fDeltaT);
	virtual void SaveState(FILEHANDLE scn);
	virtual bool ParseLine(const char* line);

};

#endif // !defined(AFX_MASTERTIMINGUNIT_H__FEF22983_033D_412B_B3A9_876EE3A3EC04__INCLUDED_)
