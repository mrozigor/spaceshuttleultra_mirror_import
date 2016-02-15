#include "SSULCC.h"
#include "../Pad/SSUPad.h"
#include "../SLC6/SLC6.h"
#include "../Atlantis.h"

#define ORBITER_MODULE

SSULCC::SSULCC(OBJHANDLE hVessel, int flightmodel)
	: VESSEL2(hVessel, flightmodel)
{
	launch_mjd=-1.0;
	lastTTL=-1.0;

	pFSS=NULL;
	pSSU=NULL;

	_firstrun = true;

	MPSHeSupply = true;
	MPSHeSupplyPressure1 = 2000;
	MPSHeSupplyPressure2 = 4100 + (400 * oapiRand());// 4100 - 4500

	MPSTVCProfile = false;

	RSLSabort = false;
	t_abort = -1;

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
		if(pVessel) {
			if(!_strnicmp(pVessel->GetClassName(), "SSU_Pad", 7)) pFSS = static_cast<SSUPad*>(pVessel);
			if(!_strnicmp(pVessel->GetClassName(), "SSU_SLC6", 8)) pFSS = static_cast<SLC6*>(pVessel);
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
	if (_firstrun == true)// bypass first run as it messes up lastTTL and causes all events before current time to run as well
	{
		// setup MPS He Supply initial status
		double timeToLaunch = (launch_mjd - mjd) * 86400.0;
		if ((timeToLaunch > MPS_HE_SUPPLY_START_TIME) || (timeToLaunch < MPS_HE_SUPPLY_END_TIME)) MPSHeSupply = false;

		_firstrun = false;
		return;
	}

	if (RSLSabort == true)
	{
		if (pFSS)
		{
			if (((t_abort + OAA_EXTEND_ABORT_DELAY) <= simt) && ((t_abort + OAA_EXTEND_ABORT_DELAY) > lastTTL))// extend orbiter access arm
			{
				oapiWriteLog( "LCC: OAA extend" );
				pFSS->ExtendOrbiterAccessArm( OAA_RATE_EMERGENCY );
			}
		}
		lastTTL = simt;
		return;
	}

	VESSEL2::clbkPreStep(simt, simdt, mjd);

	double timeToLaunch=(launch_mjd-mjd)*86400.0; //time to launch in seconds
	if(timeToLaunch < 0.0 && lastTTL < 0.0) return; // nothing to do

	//if(timeToLaunch>31.0) sprintf(oapiDebugString(),"T %f",-timeToLaunch);
	int hours = static_cast<int>(floor(timeToLaunch/3600));
	int minutes = static_cast<int>(floor((timeToLaunch - (hours*3600)) / 60));
	double seconds = timeToLaunch - (hours*3600) - (minutes*60);
	if (timeToLaunch >= 0.0) sprintf_s( oapiDebugString(), 256, "T-%02i:%02i:%04.1f",hours,minutes,seconds);
	else sprintf_s( oapiDebugString(), 256, "" );

	if(pFSS) {
		if(timeToLaunch<=ACCESS_ARM_RETRACT_TIME && lastTTL>ACCESS_ARM_RETRACT_TIME) //retract orbiter access arm
		{
			oapiWriteLog("LCC: OAA retract");
			pFSS->RetractOrbiterAccessArm();
		}
		else if(timeToLaunch<=GOX_ARM_RETRACT_TIME && lastTTL>GOX_ARM_RETRACT_TIME) //retract GOX arm
		{
			oapiWriteLog("LCC: GVA retract");
			pFSS->RetractGOXArmAndHood();
		}
		else if(timeToLaunch<=0.0 && lastTTL>=0.0) pFSS->OnT0();
	}
	if(pSSU) {
		// these steps should really be done by GLS class, but we don't have one yet.
		if(timeToLaunch<=APU_CHECK_TIME /*&& lastTTL>=RSLS_SEQUENCE_START_TIME*/)
		{
			if(!pSSU->HydraulicsOK())
			{
				sprintf_s(oapiDebugString(), 255, "Countdown clock will hold at T-4 minutes due to a failure");
				timeToLaunch = mjd-1.0; // set launch time to date in past
			}
		}
		if ((timeToLaunch <= PSN4_TIME) && (lastTTL > PSN4_TIME))
		{
			oapiWriteLog( "LCC: PSN4" );
			pSSU->PSN4();
		}

		if ((timeToLaunch <= MPS_TVC_ACTR_PROFILE_TIME) && (lastTTL > MPS_TVC_ACTR_PROFILE_TIME))
		{
			MPSTVCProfile = true;
			// TODO remove override
			oapiWriteLog( "LCC: MPS Engine TVC Actuator Profile started" );
		}

		if(timeToLaunch<=RSLS_SEQUENCE_START_TIME && lastTTL>RSLS_SEQUENCE_START_TIME)
		{
			oapiWriteLog("LCC: T-31");
			pSSU->SynchronizeCountdown(launch_mjd);
			pSSU->StartRSLSSequence();
		}
		if ((timeToLaunch <= MPS_HE_SUPPLY_START_TIME) && (lastTTL > MPS_HE_SUPPLY_START_TIME))
		{
			MPSHeSupply = true;
			oapiWriteLog( "LCC: MPS He Supply started" );
		}
		if ((timeToLaunch <= MPS_HE_SUPPLY_END_TIME) && (lastTTL > MPS_HE_SUPPLY_END_TIME))
		{
			MPSHeSupply = false;
			oapiWriteLog( "LCC: MPS He Supply terminated" );
		}

		/////////// MPS He Supply ///////////
		if (MPSHeSupply == true)
		{
			if (timeToLaunch <= 12000)// T-3h20m
			{
				// eng
				for (int i = 1; i <= 3; i++)
				{
					if (pSSU->GetHeTankPress( i ) < MPSHeSupplyPressure2) pSSU->HeFillTank( i, 30 * simdt );// add mass
				}

				// pneu
				if (pSSU->GetHeTankPress( 0 ) < MPSHeSupplyPressure2) pSSU->HeFillTank( 0, 2 * simdt );// add mass
			}
			else
			{
				// eng
				for (int i = 1; i <= 3; i++)
				{
					if (pSSU->GetHeTankPress( i ) < MPSHeSupplyPressure1) pSSU->HeFillTank( i, 30 * simdt );// add mass
				}

				// pneu
				if (pSSU->GetHeTankPress( 0 ) < MPSHeSupplyPressure1) pSSU->HeFillTank( 0, 2 * simdt ); // add mass
			}
		}
		/////////// MPS He Supply ///////////

		/////////// MPS TVC Profile ///////////
		if (MPSTVCProfile == true)
		{
			if (timeToLaunch > 207)
			{
				// hold null
				pSSU->SetSSMEActPos( 1, 0, 0 );
				pSSU->SetSSMEActPos( 2, 0, 0 );
				pSSU->SetSSMEActPos( 3, 0, 0 );
			}
			else if (timeToLaunch > 205)
			{
				// move to "left-down" at 3�/s
				pSSU->SetSSMEActPos( 1, (207 - timeToLaunch) * 3, (207 - timeToLaunch) * (-3) );
				pSSU->SetSSMEActPos( 2, (207 - timeToLaunch) * 3, (207 - timeToLaunch) * (-3) );
				pSSU->SetSSMEActPos( 3, (207 - timeToLaunch) * 3, (207 - timeToLaunch) * (-3) );
			}
			else if (timeToLaunch > 200)
			{
				// hold 5s
				pSSU->SetSSMEActPos( 1, 6, -6 );
				pSSU->SetSSMEActPos( 2, 6, -6 );
				pSSU->SetSSMEActPos( 3, 6, -6 );
			}
			else if (timeToLaunch > 198)
			{
				// move to null at 3�/s
				pSSU->SetSSMEActPos( 1, (timeToLaunch - 198) * 3, (timeToLaunch - 198) * (-3) );
				pSSU->SetSSMEActPos( 2, (timeToLaunch - 198) * 3, (timeToLaunch - 198) * (-3) );
				pSSU->SetSSMEActPos( 3, (timeToLaunch - 198) * 3, (timeToLaunch - 198) * (-3) );
			}
			else if (timeToLaunch > 197)
			{
				// hold 1s
				pSSU->SetSSMEActPos( 1, 0, 0 );
				pSSU->SetSSMEActPos( 2, 0, 0 );
				pSSU->SetSSMEActPos( 3, 0, 0 );
			}
			else if (timeToLaunch > 195)
			{
				// move to "right-up" at 3�/s
				pSSU->SetSSMEActPos( 1, (197 - timeToLaunch) * (-3), (197 - timeToLaunch) * 3 );
				pSSU->SetSSMEActPos( 2, (197 - timeToLaunch) * (-3), (197 - timeToLaunch) * 3 );
				pSSU->SetSSMEActPos( 3, (197 - timeToLaunch) * (-3), (197 - timeToLaunch) * 3 );
			}
			else if (timeToLaunch > 190)
			{
				// hold 5s
				pSSU->SetSSMEActPos( 1, -6, 6 );
				pSSU->SetSSMEActPos( 2, -6, 6 );
				pSSU->SetSSMEActPos( 3, -6, 6 );
			}
			else if (timeToLaunch > 188)
			{
				// move to null at 3�/s
				pSSU->SetSSMEActPos( 1, (timeToLaunch - 188) * (-3), (timeToLaunch - 188) * 3 );
				pSSU->SetSSMEActPos( 2, (timeToLaunch - 188) * (-3), (timeToLaunch - 188) * 3 );
				pSSU->SetSSMEActPos( 3, (timeToLaunch - 188) * (-3), (timeToLaunch - 188) * 3 );
			}
			else if (timeToLaunch > 135)
			{
				// hold null
				pSSU->SetSSMEActPos( 1, 0, 0 );
				pSSU->SetSSMEActPos( 2, 0, 0 );
				pSSU->SetSSMEActPos( 3, 0, 0 );
			}
			else
			{
				// move to start config
				pSSU->SetSSMEActPos( 1, -3, 0 );
				pSSU->SetSSMEActPos( 2, -9, 0 );
				pSSU->SetSSMEActPos( 3, -9, 0 );
				MPSTVCProfile = false;
				oapiWriteLog( "LCC: MPS Engine TVC Actuator Profile terminated" );
			}
		}
		/////////// MPS TVC Profile ///////////

		RSLSabort = pSSU->GetRSLSAbortFlag();
		if (RSLSabort == true) t_abort = simt;
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
			sscanf_s(line+8, "%s", PadName, sizeof(PadName));
		}
		else if(!_strnicmp(line, "SHUTTLE_NAME", 12)) {
			sscanf_s(line+12, "%s", ShuttleName, sizeof(ShuttleName));
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
