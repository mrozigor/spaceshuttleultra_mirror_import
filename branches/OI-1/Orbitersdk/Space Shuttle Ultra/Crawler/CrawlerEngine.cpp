#include "CrawlerEngine.h"
#include "Crawler.h"
#include <OrbiterSoundSDK35.h>

CrawlerEngine::CrawlerEngine(SubsystemDirector<Crawler>* _director)
: Subsystem(_director, "Engine"),
engineState(OFF)
{
	//targetSpeed = 0.0;
	currentAcceleration = 0.0;
	enginePower = 0.0;
	//maxSpeed = MAX_UNLOADED_SPEED;
	bReverse = false;

	//increaseTgtSpeed = false;
	//decreaseTgtSpeed = false;
}

CrawlerEngine::~CrawlerEngine()
{
}

double CrawlerEngine::GetAcceleration() const
{
	return currentAcceleration;
}

/*void CrawlerEngine::SetMaxSpeed(double speed)
{
	maxSpeed = speed;
}

void CrawlerEngine::IncreaseTgtSpeed(bool _inc)
{
	increaseTgtSpeed=_inc;
	decreaseTgtSpeed=false;
}

void CrawlerEngine::DecreaseTgtSpeed(bool _dec)
{
	decreaseTgtSpeed=_dec;
	increaseTgtSpeed=false;
}*/

void CrawlerEngine::Realize()
{
	DiscreteBundle* pBundle = V()->BundleManager()->CreateBundle("CRAWLER_SPEED", 16);
	currentSpeed.Connect(pBundle, 0);
	tgtSpeed.Connect(pBundle, 1);

	pBundle = V()->BundleManager()->CreateBundle("CRAWLER_ENGINE", 16);
	commandVoltage.Connect(pBundle, 0);
	commandVoltage.SetLine(0.0f);
	for(unsigned int i=0;i<3;i++)
		engineDirection[i].Connect(pBundle, i+1);
}

void CrawlerEngine::OnPreStep(double SimT, double SimDT, double MJD)
{
	// set target speed
	/*if(increaseTgtSpeed) {
		double dv = SimDT*0.1;
		targetSpeed = range(0.0, targetSpeed+dv, maxSpeed);
	}
	else if(decreaseTgtSpeed) {
		double dv = SimDT*-0.1;
		targetSpeed = range(0.0, targetSpeed+dv, maxSpeed);
	}*/

	// use FWD/NEUT/REV PBIs to start/shutdown engine and play correct sounds
	// use startup/shutdown sound length to determine when startup or shutdown is complete
	if(engineDirection[FWD] || engineDirection[REV]) {
		if(engineState==OFF || engineState==SHUTDOWN) {
			engineState = STARTING;
			PlayVesselWave3(V()->GetSoundID(), ENGINE_START_SOUND_ID);
		}
		else if(engineState==STARTING && !IsPlaying3(V()->GetSoundID(), ENGINE_START_SOUND_ID)) {
			engineState = ON;
		}
		else if(engineState==ON) {
			PlayVesselWave3(V()->GetSoundID(), ENGINE_SOUND_ID, LOOP);
		}
	}
	else { // NEUT
		if(engineState==ON || engineState==STARTING) {
			engineState = SHUTDOWN;
			PlayVesselWave3(V()->GetSoundID(), ENGINE_STOP_SOUND_ID);
			StopVesselWave3(V()->GetSoundID(), ENGINE_SOUND_ID);
		}
		else if(engineState==SHUTDOWN && !IsPlaying3(V()->GetSoundID(), ENGINE_STOP_SOUND_ID)) {
			engineState = OFF;
		}
	}
	
	// simplistic implementation of engine: acc is proportional to engine power
	if(engineState==ON) {
		// calculate throttle setting
		double dPower;
		double tgtPower;
		double actualSpeed = currentSpeed.GetVoltage();
		double targetSpeed = tgtSpeed.GetVoltage();
		enginePower = fabs(enginePower); // for calculations, use positive power
		if(!Eq(targetSpeed, 0.0, 0.01)) {
			//double err=targetSpeed-actualSpeed;
			double err=0.0;
			if(engineDirection[FWD]) err=targetSpeed-actualSpeed;
			else err=-targetSpeed-actualSpeed;
			if(fabs(err) > 0.005) {
				tgtPower = range(0.30, fabs(err)*10.0, 1.0);
				if((actualSpeed>0.0 && err<0.0) || (actualSpeed<0.0 && err>0.0)) {
					tgtPower = (1.0-tgtPower)*(0.20/0.70);
				}
				if(fabs(err)<0.01 && oapiGetTimeAcceleration()>20.0) {
					tgtPower = 0.25;
				}
				dPower = range(-0.05, tgtPower-enginePower, 0.05);
			}
			else { // hold speed
				dPower = range(-0.05, 0.25-enginePower, 0.05);
				tgtPower = 0.25;
			}
		}
		else { // shutdown engine
			dPower = -0.05;
			tgtPower = 0.0;
		}
		dPower*=SimDT;
		if(abs(dPower) > abs(tgtPower-enginePower)) enginePower = tgtPower;
		else enginePower = range(0.0, enginePower+dPower, 1.0);
		// correct sign of enginePower
		if(engineDirection[REV]) enginePower = -enginePower;

		sprintf_s(oapiDebugString(), 255, "Target Speed: %f", targetSpeed*MPS2MPH);
	}
	else {
		enginePower = 0.0;
	}
	currentAcceleration = enginePower*0.01;
	commandVoltage.SetLine(enginePower);

}

bool CrawlerEngine::OnParseLine(const char* keyword, const char* line)
{
	std::string strKey = keyword;
	/*if(strKey == "TGT_VELOCITY") {
		sscanf_s(line, "%lf", &targetSpeed);
		return true;
	}*/
	if(strKey == "ENGINE_POWER") {
		sscanf_s(line, "%lf", &enginePower);
		return true;
	}
	else if(strKey == "ENGINE_STATE") {
		int temp;
		sscanf_s(line, "%d", &temp);
		engineState = static_cast<ENGINE_STATE>(temp);
		return true;
	}
	return false;
}

void CrawlerEngine::OnSaveState(FILEHANDLE scn) const
{
	//oapiWriteScenario_float(scn, "VELOCITY", actualSpeed);
	//oapiWriteScenario_float(scn, "TGT_VELOCITY", targetSpeed);
	oapiWriteScenario_float(scn, "ENGINE_POWER", enginePower);
	oapiWriteScenario_int(scn, "ENGINE_STATE", engineState);
}