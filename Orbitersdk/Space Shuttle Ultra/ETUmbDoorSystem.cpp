#include "ETUmbDoorSystem.h"
#include "SSUMath.h"

ETUmbDoorSystem::ETUmbDoorSystem(SubsystemDirector* _director)
	: AtlantisSubsystem(_director, "ET Umb Doors")
{
	LDoorPos[0]=LDoorPos[1]=0.0; // closed
	RDoorPos[0]=RDoorPos[1]=0.0; // closed
	LLatchPos[0]=LLatchPos[1]=0.0; // latched
	RLatchPos[0]=RLatchPos[1]=0.0; // latched
	CLLatchPos[0]=CLLatchPos[1]=1.0; // released
}

ETUmbDoorSystem::~ETUmbDoorSystem()
{
}

void ETUmbDoorSystem::Realize()
{
	DiscreteBundle* pBundle=BundleManager()->CreateBundle("ETUmbDoors", 16);
	LUmbDoorClose.Connect(pBundle, 0);
	LUmbDoorOpen.Connect(pBundle, 1);
	LUmbDoorLatch.Connect(pBundle, 2);
	LUmbDoorRelease.Connect(pBundle, 3);
	RUmbDoorClose.Connect(pBundle, 4);
	RUmbDoorOpen.Connect(pBundle, 5);
	RUmbDoorLatch.Connect(pBundle, 6);
	RUmbDoorRelease.Connect(pBundle, 7);
	CenterlineRelease.Connect(pBundle, 8);

	pBundle=BundleManager()->CreateBundle("ETUmbDoor microswitches", 16);
	LUmbDoorClosed.Connect(pBundle, 0);
	LUmbDoorOpened.Connect(pBundle, 1);
	LUmbDoorLatched.Connect(pBundle, 2);
	LUmbDoorReleased.Connect(pBundle, 3);
	RUmbDoorClosed.Connect(pBundle, 4);
	RUmbDoorOpened.Connect(pBundle, 5);
	RUmbDoorLatched.Connect(pBundle, 6);
	RUmbDoorReleased.Connect(pBundle, 7);
	CenterlineReleased.Connect(pBundle, 8);
}

void ETUmbDoorSystem::OnPreStep(double SimT, double DeltaT, double MJD)
{
	if(LUmbDoorClose) {
		if(!Eq(LDoorPos[0], 1.0, 0.05) || CLLatchPos[0]>0.95) {
			LDoorPos[1] = max(0.0, LDoorPos[0]-ET_UMB_DOOR_OPERATING_SPEED*DeltaT);
		}
	}
	else if(LUmbDoorOpen) {
		if(!Eq(LDoorPos[0], 0.0, 0.05) || LLatchPos[0]>0.95) {
			LDoorPos[1] = min(1.0, LDoorPos[0]+ET_UMB_DOOR_OPERATING_SPEED*DeltaT);
		}
	}
	if(LUmbDoorLatch) {
		LLatchPos[1] = max(0.0, LLatchPos[0]-LATCH_OPERATING_SPEED*DeltaT);
	}
	else if(LUmbDoorRelease) {
		LLatchPos[1] = min(1.0, LLatchPos[0]+LATCH_OPERATING_SPEED*DeltaT);
	}

	if(RUmbDoorClose) {
		if(!Eq(RDoorPos[0], 1.0, 0.05) || CLLatchPos[0]>0.95) {
			RDoorPos[1] = max(0.0, RDoorPos[0]-ET_UMB_DOOR_OPERATING_SPEED*DeltaT);
		}
	}
	else if(RUmbDoorOpen) {
		if(!Eq(RDoorPos[0], 0.0, 0.05) || RLatchPos[0]>0.95) {
			RDoorPos[1] = min(1.0, RDoorPos[0]+ET_UMB_DOOR_OPERATING_SPEED*DeltaT);
		}
	}
	if(RUmbDoorLatch) {
		RLatchPos[1] = max(0.0, RLatchPos[0]-LATCH_OPERATING_SPEED*DeltaT);
	}
	else if(RUmbDoorRelease) {
		RLatchPos[1] = min(1.0, RLatchPos[0]+LATCH_OPERATING_SPEED*DeltaT);
	}

	if(CenterlineRelease) {
		CLLatchPos[1] = min(1.0, CLLatchPos[0]+LATCH_OPERATING_SPEED*DeltaT);
	}
}

void ETUmbDoorSystem::OnPostStep(double SimT, double DeltaT, double MJD)
{
	STS()->SetETUmbDoorPosition(LDoorPos[0], 0);
	STS()->SetETUmbDoorPosition(RDoorPos[0], 1);

	if(Eq(LDoorPos[0], 0.0, 0.01)) {
		LUmbDoorClosed.SetLine();
		LUmbDoorOpened.ResetLine();
	}
	else if(Eq(LDoorPos[0], 1.0, 0.01)) {
		LUmbDoorClosed.ResetLine();
		LUmbDoorOpened.SetLine();
	}
	else {
		LUmbDoorClosed.ResetLine();
		LUmbDoorOpened.ResetLine();
	}

	if(Eq(RDoorPos[0], 0.0, 0.01)) {
		RUmbDoorClosed.SetLine();
		RUmbDoorOpened.ResetLine();
	}
	else if(Eq(RDoorPos[0], 1.0, 0.01)) {
		RUmbDoorClosed.ResetLine();
		RUmbDoorOpened.SetLine();
	}
	else {
		RUmbDoorClosed.ResetLine();
		RUmbDoorOpened.ResetLine();
	}

	if(Eq(LLatchPos[0], 0.0, 0.01)) {
		LUmbDoorLatched.SetLine();
		LUmbDoorReleased.ResetLine();
	}
	else if(Eq(LLatchPos[0], 1.0, 0.01)) {
		LUmbDoorLatched.ResetLine();
		LUmbDoorReleased.SetLine();
	}
	else {
		LUmbDoorLatched.ResetLine();
		LUmbDoorReleased.ResetLine();
	}

	if(Eq(RLatchPos[0], 0.0, 0.01)) {
		RUmbDoorLatched.SetLine();
		RUmbDoorReleased.ResetLine();
	}
	else if(Eq(RLatchPos[0], 1.0, 0.01)) {
		RUmbDoorLatched.ResetLine();
		RUmbDoorReleased.SetLine();
	}
	else {
		RUmbDoorLatched.ResetLine();
		RUmbDoorReleased.ResetLine();
	}

	if(Eq(CLLatchPos[0], 1.0, 0.01)) CenterlineReleased.SetLine();
	else CenterlineReleased.ResetLine();
}

void ETUmbDoorSystem::OnPropagate(double SimT, double DeltaT, double MJD)
{
	LDoorPos[0]=LDoorPos[1];
	LLatchPos[0]=LLatchPos[1];
	RDoorPos[0]=RDoorPos[1];
	RLatchPos[0]=RLatchPos[1];
	CLLatchPos[0]=CLLatchPos[1];
}

void ETUmbDoorSystem::OnSaveState(FILEHANDLE scn) const
{
	char cbuf[255];
	sprintf_s(cbuf, 255, "%f %f", LDoorPos[0], RDoorPos[0]);
	oapiWriteScenario_string (scn, "ET_DOORS", cbuf);
	sprintf_s(cbuf, 255, "%f %f %f", CLLatchPos[0], LLatchPos[0], RLatchPos[0]);
	oapiWriteScenario_string (scn, "ET_DOOR_LATCHES", cbuf);
}

bool ETUmbDoorSystem::OnParseLine(const char *keyword, const char *line)
{
	if(!strncmp(keyword, "ET_DOORS", 8)) {
		sscanf_s(line, "%lf%lf", &LDoorPos[0], &RDoorPos[0]);
		return true;
	}
	else if(!strncmp(keyword, "ET_DOOR_LATCHES", 14)) {
		sscanf_s(line, "%lf%lf%lf", &CLLatchPos[0], &LLatchPos[0], &RLatchPos[0]);
		return true;
	}
	return false;
}