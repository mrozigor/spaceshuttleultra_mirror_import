// MasterTimingUnit.cpp: Implementierung der Klasse MasterTimingUnit.
//
//////////////////////////////////////////////////////////////////////

#include "MasterTimingUnit.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

MasterTimingUnit::MasterTimingUnit(SubsystemDirector* _director)
: AtlantisSubsystem(_director, "MTU")
{
	int i;
	for(i=0;i<3; i++)
	{
		fGMT[i] = 0.0;
		fMET[i] = 0.0;
	}
	fEvent[0] = 0.0;
	fEvent[1] = 0.0;
	event_mode[0] = COUNT_DOWN;
	event_mode[1] = COUNT_DOWN;
	event_control[0] = COUNT_STOPPED;
	event_control[1] = COUNT_STOPPED;
}

MasterTimingUnit::~MasterTimingUnit()
{

}

short MasterTimingUnit::GetEventTimerSec(MTU_EVTTMR_INDEX timer) const
{
	return sEventSeconds[timer];
}

short MasterTimingUnit::GetEventTimerMin(MTU_EVTTMR_INDEX timer) const
{
	return sEventMinutes[timer];
}

void MasterTimingUnit::SetEventTimer(MTU_EVTTMR_INDEX timer, short Minutes, short Seconds)
{
	sEventMinutes[timer] = Minutes;
	sEventSeconds[timer] = Seconds;
	fEvent[timer] = Minutes * 60.0 + Seconds;
}

void MasterTimingUnit::StartEventTimer(MTU_EVTTMR_INDEX timer)
{
	event_control[timer] = COUNT_STARTED;
}

void MasterTimingUnit::StopEventTimer(MTU_EVTTMR_INDEX timer)
{
	event_control[timer] = COUNT_STOPPED;
}
	
void MasterTimingUnit::SetEventTimerMode(MTU_EVTTMR_INDEX timer, EVENT_TIMER_MODE mode)
{
	event_mode[timer] = mode;
}

short MasterTimingUnit::GetMETMilli(MTU_ACCU_INDEX accu_index) const
{
	return sMETMillis[accu_index];
}

short MasterTimingUnit::GetMETSec(MTU_ACCU_INDEX accu_index) const
{
	return sMETSeconds[accu_index];
}

short MasterTimingUnit::GetMETMin(MTU_ACCU_INDEX accu_index) const
{
	return sMETMinutes[accu_index];
}

short MasterTimingUnit::GetMETHour(MTU_ACCU_INDEX accu_index) const
{
	return sMETHours[accu_index];
}

short MasterTimingUnit::GetMETDay(MTU_ACCU_INDEX accu_index) const
{
	return sMETDays[accu_index];
}

short MasterTimingUnit::GetGMTMilli(MTU_ACCU_INDEX accu_index) const
{
	return sGMTMillis[accu_index];
}

short MasterTimingUnit::GetGMTSec(MTU_ACCU_INDEX accu_index) const
{
	return sGMTSeconds[accu_index];;
}

short MasterTimingUnit::GetGMTMin(MTU_ACCU_INDEX accu_index) const
{
	return sGMTMinutes[accu_index];;
}

short MasterTimingUnit::GetGMTHour(MTU_ACCU_INDEX accu_index) const
{
	return sGMTHours[accu_index];;
}

short MasterTimingUnit::GetGMTDay(MTU_ACCU_INDEX accu_index) const
{
	return sGMTDays[accu_index];;
}

void MasterTimingUnit::ResetEventTimer(MTU_EVTTMR_INDEX timer)
{
	fEvent[timer] = 0.0;
}

void MasterTimingUnit::PreStep(double fSimT, double fDeltaT)
{
	int timer;
	long lTime;
	double fSeconds;

	for(timer=0; timer<3; timer++)
	{
		fGMT[timer] += fDeltaT;
		fMET[timer] += fDeltaT;

		if(fGMT[timer] > 34560000.0) {
			fGMT[timer] -= 34560000.0;
		}

		if(fMET[timer] > 34560000.0) {
			fMET[timer] -= 34560000.0;
		}
		
		sGMTMillis[timer] = (short)fmod(fGMT[timer] * 1000.0, 1000.0);

		lTime = (long)fGMT[timer];
		sGMTSeconds[timer] = lTime%60;
		lTime/=60;
		sGMTMinutes[timer] = lTime%60;
		lTime/=60;
		sGMTHours[timer] = lTime%24;
		lTime/=24;
		sGMTDays[timer] = lTime%400 + 1; 

		sMETMillis[timer] = (short)fmod(fMET[timer] * 1000.0, 1000.0);

		lTime = (long)fMET[timer];
		sMETSeconds[timer] = lTime%60;
		lTime/=60;
		sMETMinutes[timer] = lTime%60;
		lTime/=60;
		sMETHours[timer] = lTime%24;
		lTime/=24;
		sMETDays[timer] = lTime%400;
	}
	
	for(timer = 0; timer<2; timer++)
	{

		switch(event_control[timer])
		{
		case COUNT_STOPPED:
			break;
		case COUNT_STARTED:
			switch(event_mode[timer])
			{
			case COUNT_UP:
				fEvent[timer] += fDeltaT;
				break;
			case COUNT_DOWN:
				if(fEvent[timer] > fDeltaT) {
					fEvent[timer] -= fDeltaT;
				} else
				{
					fEvent[timer] = 0;
					event_control[timer] = COUNT_STOPPED;
				}
				break;
			}
			break;
		}

		fSeconds = fmod(fEvent[timer], 60.0);
		sEventMinutes[timer] = (short)(fEvent[timer] - fSeconds)/60.0;
		sEventSeconds[timer] = fSeconds;

		if(event_mode[timer] == COUNT_TEST) {
			sEventMinutes[timer] = 88;
			sEventSeconds[timer] = 88;
		}
		
	}
	//sprintf(oapiDebugString(), "GMT: %03d:%02d:%02d:%02d.%03d MET: %03d:%02d:%02d:%02d.%03d",
	//	sGMTDays[0], sGMTHours[0], sGMTMinutes[0], sGMTSeconds[0], sGMTMillis[0],
	//	sMETDays[0], sMETHours[0], sMETMinutes[0], sMETSeconds[0], sMETMillis[0]);

	//sprintf(oapiDebugString(), "EVENT TIMER: %02d:%02d",
	//	sEventMinutes[TIMER_FORWARD], sEventSeconds[TIMER_FORWARD]);
}

void MasterTimingUnit::SaveState(FILEHANDLE scn)
{
	char pszBuffer[128];
	char pszTempA[40];
	char pszTempB[40];

	int i;
	for(i = 0; i<2; i++)
	{
		switch(event_mode[i])
		{
		case COUNT_DOWN:
			strcpy(pszTempA, "DOWN");
			break;
		case COUNT_UP:
			strcpy(pszTempA, "UP");
			break;
		case COUNT_TEST:
			strcpy(pszTempA, "TEST");
			break;
		}

		switch(event_control[i])
		{
		case COUNT_STARTED:
			strcpy(pszTempB, "STARTED");
			break;
		case COUNT_STOPPED:
			strcpy(pszTempB, "STOPPED");
			break;
		}

		sprintf(pszBuffer, "   EVENT_TIMER%1d %f %s %s", i, fEvent[i], pszTempA, pszTempB);

		oapiWriteLine(scn, pszBuffer);
	}
}

bool MasterTimingUnit::ParseLine(const char* line)
{
	char pszTempA[40] = "";
	char pszTempB[40] = "";
	int iTmpA = 0, iTmpB = 0;
	float fTmpA = 0.0;

	if(!strnicmp(line, "EVENT_TIMER", 11)) {
		const char* pLine2 = line+11;
		sscanf(pLine2, "%d %f %s %s",
			&iTmpA, &fTmpA, pszTempA, pszTempB);
		if(iTmpA >=0 && iTmpA < 2)
		{
			fEvent[iTmpA] = fTmpA;
			if(!stricmp(pszTempA, "DOWN")) {
				event_mode[iTmpA] = COUNT_DOWN;
			} else if(!stricmp(pszTempA, "UP")) {
				event_mode[iTmpA] = COUNT_UP;
			} else if(!stricmp(pszTempA, "TEST")) {
				event_mode[iTmpA] = COUNT_TEST;
			}

			if(!stricmp(pszTempB, "STARTED")) {
				event_control[iTmpA] = COUNT_STARTED;
			} else if(!stricmp(pszTempB, "STOPPED")) {
				event_control[iTmpA] = COUNT_STOPPED;
			}

			sEventMinutes[iTmpA] = ((int)fEvent[iTmpA])/60;
			sEventSeconds[iTmpA] = ((int)fEvent[iTmpA])%60;
		}
		return true;
	}
	return false;
}