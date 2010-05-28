#ifndef __CRAWLERENGINE_H_F05A8448_9C86_481c_B632_0641CA218CF0
#define __CRAWLERENGINE_H_F05A8448_9C86_481c_B632_0641CA218CF0
#pragma once

#include <orbitersdk.h>
#include "Subsystem.h"

//const double MAX_POWER = 4000.0e3;
// total power (Watts) of all 4 generators

class Crawler;

class CrawlerEngine : public Subsystem<Crawler>
{
public:
	CrawlerEngine(SubsystemDirector<Crawler>* _director);
	~CrawlerEngine();
	
	virtual void OnPreStep(double SimT, double SimDT, double MJD);

	virtual bool OnParseLine(const char* keyword, const char* line);
	virtual void OnSaveState(FILEHANDLE scn) const;

	double GetSpeed() const;
	//void SetTargetSpeed(double speed);
	void SetMaxSpeed(double speed);

	void Accelerate(bool _acc);
	void Brake(bool _brake);
private:
	// speed in m/s
	double maxSpeed;
	double targetSpeed;
	double actualSpeed;
	bool bReverse;

	bool accelerate;
	bool brake;

	//double acceleration;

	double enginePower; // between 0 and 1
};

#endif //__CRAWLERENGINE_H_F05A8448_9C86_481c_B632_0641CA218CF0