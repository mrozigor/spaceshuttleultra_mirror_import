#include "CrawlerEngine.h"
#include "Crawler.h"

CrawlerEngine::CrawlerEngine(SubsystemDirector<Crawler>* _director)
: Subsystem(_director, "Engine")
{
	targetSpeed = 0.0;
	actualSpeed = 0.0;
	enginePower = 0.0;
	maxSpeed = MAX_UNLOADED_SPEED;
	bReverse = false;

	accelerate = false;
	brake = false;
}

CrawlerEngine::~CrawlerEngine()
{
}

double CrawlerEngine::GetSpeed() const
{
	return actualSpeed;
}

void CrawlerEngine::SetMaxSpeed(double speed)
{
	maxSpeed = speed;
}

void CrawlerEngine::Accelerate(bool _acc)
{
	accelerate=_acc;
	brake=false;
}

void CrawlerEngine::Brake(bool _brake)
{
	brake=_brake;
	accelerate=false;
}

void CrawlerEngine::OnPreStep(double SimT, double SimDT, double MJD)
{
	// calculate target velocity
	if(accelerate) {
		double dv = SimDT * 0.1;
		if (targetSpeed < 0 && targetSpeed + dv >= 0) {
			targetSpeed = 0;
			accelerate=brake=false;
		} else {
			targetSpeed += dv;
		}
		if (targetSpeed > maxSpeed) targetSpeed = maxSpeed;
	}
	else if(brake) {
		double dv = SimDT * -0.1;
		if (targetSpeed > 0 && targetSpeed + dv <= 0) {
			targetSpeed = 0;
			brake=accelerate=false;
		} else {
			targetSpeed += dv;
		}
		if (targetSpeed < -maxSpeed) targetSpeed = -maxSpeed;
	}

	// simplistic implementation of engine: acc is proportional to engine power
	double acceleration = (enginePower-0.25)*0.01;
	if(!Eq(actualSpeed, 0.0, 0.01) || acceleration>0.0) {
		if((Eq(actualSpeed, 0.0, 0.01) && targetSpeed<0.0) || actualSpeed<0.0) {
			actualSpeed-=acceleration*SimDT;
		}
		else actualSpeed+=acceleration*SimDT;
	}
	else actualSpeed = 0.0;

	// calculate throttle setting
	double dPower;
	double tgtPower;
	if(!Eq(targetSpeed, 0.0, 0.01)) {
		double err=targetSpeed-actualSpeed;
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

	sprintf_s(oapiDebugString(), 255, "Tgt Velocity: %f MPH Velocity: %f MPH Pwr: %f %f", targetSpeed*MPS2MPH, actualSpeed*MPS2MPH, enginePower, tgtPower);
}

bool CrawlerEngine::OnParseLine(const char* keyword, const char* line)
{
	std::string strKey = keyword;
	if(strKey == "VELOCITY") {
		sscanf_s(line, "%lf", &actualSpeed);
		return true;
	}
	else if(strKey == "TGT_VELOCITY") {
		sscanf_s(line, "%lf", &targetSpeed);
		return true;
	}
	else if(strKey == "ENGINE_POWER") {
		sscanf_s(line, "%lf", &enginePower);
		return true;
	}
	return false;
}

void CrawlerEngine::OnSaveState(FILEHANDLE scn) const
{
	oapiWriteScenario_float(scn, "VELOCITY", actualSpeed);
	oapiWriteScenario_float(scn, "TGT_VELOCITY", targetSpeed);
	oapiWriteScenario_float(scn, "ENGINE_POWER", enginePower);	
}