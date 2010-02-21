#ifndef __CRAWLERENGINE_H_F05A8448_9C86_481c_B632_0641CA218CF0
#define __CRAWLERENGINE_H_F05A8448_9C86_481c_B632_0641CA218CF0
#pragma once

#include <orbitersdk.h>

//const double MAX_POWER = 4000.0e3;
// total power (Watts) of all 4 generators

class Crawler;

class CrawlerEngine
{
public:
	CrawlerEngine();
	~CrawlerEngine();

	double GetSpeed() const;
	//void SetTargetSpeed(double speed);

	void Accelerate(bool _acc);
	void Brake(bool _brake);

	bool ParseLine(const char* line);
	void SaveState(FILEHANDLE scn) const;

	void OnPreStep(double SimT, double SimDT, double MJD);
private:
	// speed in m/s
	double targetSpeed;
	double actualSpeed;
	bool bReverse;

	bool accelerate;
	bool brake;

	//double acceleration;

	double enginePower; // between 0 and 1
};

#endif //__CRAWLERENGINE_H_F05A8448_9C86_481c_B632_0641CA218CF0