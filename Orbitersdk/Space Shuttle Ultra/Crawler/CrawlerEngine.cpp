#include "CrawlerEngine.h"
#include "Crawler.h"

CrawlerEngine::CrawlerEngine(SubsystemDirector<Crawler>* _director)
: Subsystem(_director, "Engine")
{
	targetSpeed = 0.0;
	currentAcceleration = 0.0;
	enginePower = 0.0;
	maxSpeed = MAX_UNLOADED_SPEED;
	bReverse = false;

	increaseTgtSpeed = false;
	decreaseTgtSpeed = false;
}

CrawlerEngine::~CrawlerEngine()
{
}

double CrawlerEngine::GetAcceleration() const
{
	return currentAcceleration;
}

void CrawlerEngine::SetMaxSpeed(double speed)
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
}

void CrawlerEngine::Realize()
{
	DiscreteBundle* pBundle = V()->BundleManager()->CreateBundle("CRAWLER_SPEED", 16);
	currentSpeed.Connect(pBundle, 0);

	pBundle = V()->BundleManager()->CreateBundle("CRAWLER_ENGINE", 16);
	commandVoltage.Connect(pBundle, 0);
	commandVoltage.SetLine(0.0f);
}

void CrawlerEngine::OnPreStep(double SimT, double SimDT, double MJD)
{
	// calculate target velocity
	if(increaseTgtSpeed) {
		double dv = SimDT * 0.1;
		if (targetSpeed < 0 && targetSpeed + dv >= 0) {
			targetSpeed = 0;
			increaseTgtSpeed=decreaseTgtSpeed=false;
		} else {
			targetSpeed += dv;
		}
		if (targetSpeed > maxSpeed) targetSpeed = maxSpeed;
	}
	else if(decreaseTgtSpeed) {
		double dv = SimDT * -0.1;
		if (targetSpeed > 0 && targetSpeed + dv <= 0) {
			targetSpeed = 0;
			decreaseTgtSpeed=increaseTgtSpeed=false;
		} else {
			targetSpeed += dv;
		}
		if (targetSpeed < -maxSpeed) targetSpeed = -maxSpeed;
	}

	// simplistic implementation of engine: acc is proportional to engine power
	currentAcceleration = enginePower*0.01;

	// calculate throttle setting
	double dPower;
	double tgtPower;
	double actualSpeed = currentSpeed.GetVoltage();
	enginePower = fabs(enginePower); // for calculations, use positive power
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
	// correct sign of enginePower
	if(targetSpeed < 0.0) enginePower = -enginePower;

	commandVoltage.SetLine(enginePower);
	sprintf_s(oapiDebugString(), 255, "Tgt Velocity: %f Power: %f", targetSpeed*MPS2MPH, enginePower);
}

bool CrawlerEngine::OnParseLine(const char* keyword, const char* line)
{
	std::string strKey = keyword;
	/*if(strKey == "VELOCITY") {
		sscanf_s(line, "%lf", &actualSpeed);
		return true;
	}
	else */if(strKey == "TGT_VELOCITY") {
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
	//oapiWriteScenario_float(scn, "VELOCITY", actualSpeed);
	oapiWriteScenario_float(scn, "TGT_VELOCITY", targetSpeed);
	oapiWriteScenario_float(scn, "ENGINE_POWER", enginePower);	
}