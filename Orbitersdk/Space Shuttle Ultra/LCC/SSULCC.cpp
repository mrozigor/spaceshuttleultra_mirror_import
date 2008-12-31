#include "SSULCC.h"
#include "../Atlantis.h"

#define ORBITER_MODULE

SSULCC::SSULCC(OBJHANDLE hVessel, int flightmodel)
	: VESSEL2(hVessel, flightmodel)
{
	launch_mjd=-1.0;
	lastTTL=-1.0;
	pFSS=NULL;
	pSSU=NULL;

	sprintf_s(PadName, 256, "");
	sprintf_s(ShuttleName, 256, "");
}

SSULCC::~SSULCC()
{
}

void SSULCC::clbkPostCreation()
{
	VESSEL2::clbkPostCreation();

	//get pointer to SSUPad vessel
	OBJHANDLE hPad=oapiGetVesselByName(PadName);
	if(hPad!=NULL) {
		VESSEL* pVessel=oapiGetVesselInterface(hPad);
		if(pVessel && !_strnicmp(pVessel->GetClassNameA(), "SSU_Pad", 7)) {
			pFSS=(SSUPad*)pVessel;
		}
		else pFSS=NULL;
	}

	//set launch_mjd of shuttle
	OBJHANDLE hSSU=oapiGetVesselByName(ShuttleName);
	if(hSSU) {
		VESSEL* pVessel=oapiGetVesselInterface(hSSU);
		if(pVessel && !_stricmp(pVessel->GetClassNameA(), "SpaceShuttleUltra")) {
			pSSU=static_cast<Atlantis*>(pVessel);
		}
	}
}

void SSULCC::clbkPreStep(double simt, double simdt, double mjd)
{
	VESSEL2::clbkPreStep(simt, simdt, mjd);

	double timeToLaunch=(launch_mjd-mjd)*86400.0; //time to launch in seconds
	if(timeToLaunch>31.0) sprintf(oapiDebugString(),"T %f",-timeToLaunch);

	if(pFSS) {
		if(timeToLaunch<=ACCESS_ARM_RETRACT_TIME && lastTTL>=ACCESS_ARM_RETRACT_TIME) //retract orbiter access arm
		{
			pFSS->MoveOrbiterAccessArm(AnimState::CLOSING);
		}
		else if(timeToLaunch<=GOX_ARM_RETRACT_TIME && lastTTL>=GOX_ARM_RETRACT_TIME) //retract GOX arm
		{
			pFSS->MoveGOXArm(AnimState::CLOSING);
		}
		else if(timeToLaunch<=0.0 && lastTTL>=0.0) pFSS->OnT0();
	}
	if(pSSU) {
		if(timeToLaunch<=RSLS_SEQUENCE_START_TIME && lastTTL>=RSLS_SEQUENCE_START_TIME)
		{
			oapiWriteLog("LCC: T-31");
			pSSU->SynchronizeCountdown(launch_mjd);
			pSSU->StartRSLSSequence();
		}
	}

	lastTTL=timeToLaunch;
}

void SSULCC::clbkSaveState(FILEHANDLE scn)
{
	char cbuf[255];
	VESSEL2::clbkSaveState(scn);

	//oapiWriteScenario_float(scn, "LAUNCH_MJD", launch_mjd);
	//oapiWriteScenario_float cannot be used for launch mjd; too many decimals are lost
	sprintf_s(cbuf, 255, "%f", launch_mjd);
	oapiWriteScenario_string(scn, "LAUNCH_MJD", cbuf);
	oapiWriteScenario_string(scn, "PAD_NAME", PadName);
	oapiWriteScenario_string(scn, "SHUTTLE_NAME", ShuttleName);
}

void SSULCC::clbkLoadStateEx(FILEHANDLE scn, void *status)
{
	char *line;
	
	while(oapiReadScenario_nextline(scn, line)) {
		if(!_strnicmp(line, "LAUNCH_MJD", 10)) {
			sscanf_s(line+10, "%lf", &launch_mjd);
		}
		else if(!_strnicmp(line, "PAD_NAME", 8)) {
			sscanf_s(line+8, "%s", PadName);
		}
		else if(!_strnicmp(line, "SHUTTLE_NAME", 12)) {
			sscanf_s(line+12, "%s", ShuttleName);
		}
		else ParseScenarioLineEx(line, status);
	}
}

DLLCLBK void InitModule(HINSTANCE hDLL)
{
}

DLLCLBK void ExitModule(HINSTANCE hDLL)
{
}

DLLCLBK VESSEL* ovcInit(OBJHANDLE hVessel, int iFlightModel)
{
	return new SSULCC(hVessel, iFlightModel);
}

DLLCLBK void ovcExit(VESSEL* pVessel)
{
	delete static_cast<SSULCC*>(pVessel);
}
