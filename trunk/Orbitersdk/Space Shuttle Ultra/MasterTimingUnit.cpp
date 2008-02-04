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

	double fMJD = oapiGetSimMJD();

	double fSimGMT = (fmod(fMJD - 43874.5, 365)) * 86400.0;
	for(i=0;i<3; i++)
	{
		fGMT[i][0] = fSimGMT;
		fGMT[i][1] = fSimGMT;
		fMET[i][0] = 0.0;
		fMET[i][1] = 0.0;
	}
	fEvent[0][0] = 0.0;
	fEvent[0][1] = 0.0;
	fEvent[1][0] = 0.0;
	fEvent[1][1] = 0.0;
	event_mode[0][0] = COUNT_DOWN;
	event_mode[0][1] = COUNT_DOWN;
	event_mode[1][0] = COUNT_DOWN;
	event_mode[1][1] = COUNT_DOWN;
	event_control[0][0] = COUNT_STOPPED;
	event_control[0][1] = COUNT_STOPPED;
	event_control[1][0] = COUNT_STOPPED;
	event_control[1][1] = COUNT_STOPPED;
	bMETCounting[1] = false;
	bMETCounting[0] = false;
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
	fEvent[timer][1] = Minutes * 60.0 + Seconds;
}

void MasterTimingUnit::StartEventTimer(MTU_EVTTMR_INDEX timer)
{
	event_control[timer][1] = COUNT_STARTED;
}

void MasterTimingUnit::StopEventTimer(MTU_EVTTMR_INDEX timer)
{
	event_control[timer][1] = COUNT_STOPPED;
}
	
void MasterTimingUnit::SetEventTimerMode(MTU_EVTTMR_INDEX timer, EVENT_TIMER_MODE mode)
{
	event_mode[timer][1] = mode;
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

void MasterTimingUnit::ResetMET()
{
	fMET[0][1] = 0;
	fMET[1][1] = 0;
	fMET[2][1] = 0;
}

void MasterTimingUnit::StartMET()
{
	bMETCounting[1] = true;
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
	fEvent[timer][1] = 0.0;
}

void MasterTimingUnit::PreStep(double fSimT, double fDeltaT)
{
	int timer;

	for(timer=0; timer<3; timer++)
	{
		fGMT[timer][1] = fGMT[timer][0] + fDeltaT;

		if(fGMT[timer][1] > 34560000.0) {
			fGMT[timer][1] -= 34560000.0;
		}


		if(bMETCounting[timer])
		{
			fMET[timer][1] = fMET[timer][0] + fDeltaT;
			if(fMET[timer][0] > 34560000.0) {
				fMET[timer][1] -= 34560000.0;
			}
		}

		
		
		
		
		
	}
	
	for(timer = 0; timer<2; timer++)
	{

		switch(event_control[timer][0])
		{
		case COUNT_STOPPED:
			break;
		case COUNT_STARTED:
			switch(event_mode[timer][0])
			{
			case COUNT_UP:
				fEvent[timer][1] = fEvent[timer][0] + fDeltaT;
				break;
			case COUNT_DOWN:
				if(fEvent[timer][0] > fDeltaT) {
					fEvent[timer][1] = fEvent[timer][0] - fDeltaT;
				} else
				{
					fEvent[timer][1] = 0.0;
					event_control[timer][1] = COUNT_STOPPED;
				}
				break;
			}
			break;
		}

		
		
	}
	//sprintf(oapiDebugString(), "GMT: %03d:%02d:%02d:%02d.%03d MET: %03d:%02d:%02d:%02d.%03d",
	//	sGMTDays[0], sGMTHours[0], sGMTMinutes[0], sGMTSeconds[0], sGMTMillis[0],
	//	sMETDays[0], sMETHours[0], sMETMinutes[0], sMETSeconds[0], sMETMillis[0]);

	//sprintf(oapiDebugString(), "EVENT TIMER: %02d:%02d",
	//	sEventMinutes[TIMER_FORWARD], sEventSeconds[TIMER_FORWARD]);
}

void MasterTimingUnit::Propagate(double fSimT, double fDeltaT)
{
	int timer;
	long lTime;
	double fSeconds;

	for(timer = 0; timer<3; timer++)
	{
		fGMT[timer][0] = fGMT[timer][1];
		fMET[timer][0] = fMET[timer][1];

		//Update cache variables

		sGMTMillis[timer] = (short)fmod(fGMT[timer][0] * 1000.0, 1000.0);

		lTime = (long)fGMT[timer][0];
		sGMTSeconds[timer] = lTime%60;
		lTime/=60;
		sGMTMinutes[timer] = lTime%60;
		lTime/=60;
		sGMTHours[timer] = lTime%24;
		lTime/=24;
		sGMTDays[timer] = lTime%400 + 1; 


		sMETMillis[timer] = (short)fmod(fMET[timer][0] * 1000.0, 1000.0);
	
		lTime = (long)fMET[timer][0];
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

		fEvent[timer][0] = fEvent[timer][1];
		event_mode[timer][0] = event_mode[timer][1];
		event_control[timer][0] = event_control[timer][1];


		fSeconds = fmod(fEvent[timer][0], 60.0);
		sEventMinutes[timer] = (short)(fEvent[timer][0] - fSeconds)/60.0;
		sEventSeconds[timer] = fSeconds;

		if(event_mode[timer][0] == COUNT_TEST) {
			sEventMinutes[timer] = 88;
			sEventSeconds[timer] = 88;
		}
	}
	bMETCounting[0] = bMETCounting[1];
}

void MasterTimingUnit::SaveState(FILEHANDLE scn)
{
	char pszBuffer[128];
	char pszTempA[40];
	char pszTempB[40];

	int i;
	for(i = 0; i<2; i++)
	{
		switch(event_mode[i][0])
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

		switch(event_control[i][0])
		{
		case COUNT_STARTED:
			strcpy(pszTempB, "STARTED");
			break;
		case COUNT_STOPPED:
			strcpy(pszTempB, "STOPPED");
			break;
		}

		sprintf(pszBuffer, "   EVENT_TIMER%1d %f %s %s", i, fEvent[i][0], pszTempA, pszTempB);

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
			//fEvent[iTmpA][0] = fTmpA;
			fEvent[iTmpA][1] = fTmpA;
			if(!stricmp(pszTempA, "DOWN")) {
				event_mode[iTmpA][0] = COUNT_DOWN;
				event_mode[iTmpA][1] = COUNT_DOWN;
			} else if(!stricmp(pszTempA, "UP")) {
				event_mode[iTmpA][0] = COUNT_UP;
				event_mode[iTmpA][1] = COUNT_UP;
			} else if(!stricmp(pszTempA, "TEST")) {
				event_mode[iTmpA][0] = COUNT_TEST;
				event_mode[iTmpA][1] = COUNT_TEST;
			}

			if(!stricmp(pszTempB, "STARTED")) {
				event_control[iTmpA][0] = COUNT_STARTED;
				event_control[iTmpA][1] = COUNT_STARTED;
			} else if(!stricmp(pszTempB, "STOPPED")) {
				event_control[iTmpA][0] = COUNT_STOPPED;
				event_control[iTmpA][1] = COUNT_STOPPED;
			}

			sEventMinutes[iTmpA] = ((int)fEvent[iTmpA][0])/60;
			sEventSeconds[iTmpA] = ((int)fEvent[iTmpA][0])%60;
		}
		return true;
	}
	return false;
}

