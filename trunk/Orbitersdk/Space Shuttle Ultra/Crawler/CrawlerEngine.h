#ifndef __CRAWLERENGINE_H_F05A8448_9C86_481c_B632_0641CA218CF0
#define __CRAWLERENGINE_H_F05A8448_9C86_481c_B632_0641CA218CF0
#pragma once

#include <orbitersdk.h>
#include "Subsystem.h"
#include "DiscreteBundle.h"
#include "DiscInPort.h"
#include "DiscOutPort.h"

//const double MAX_POWER = 4000.0e3;
// total power (Watts) of all 4 generators

class Crawler;

class CrawlerEngine : public Subsystem<Crawler>
{
public:
	CrawlerEngine(SubsystemDirector<Crawler>* _director);
	~CrawlerEngine();

	virtual void Realize();
	
	virtual void OnPreStep(double SimT, double SimDT, double MJD);

	virtual bool OnParseLine(const char* keyword, const char* line);
	virtual void OnSaveState(FILEHANDLE scn) const;

	/**
	 * Returns acceleration of crawler due to engine (in m/s^2)
	 */
	double GetAcceleration() const;
	//void SetTargetSpeed(double speed);
	void SetMaxSpeed(double speed);

	void IncreaseTgtSpeed(bool _inc);
	void DecreaseTgtSpeed(bool _dec);
private:
	// speed in m/s
	double maxSpeed;
	double targetSpeed;
	//double actualSpeed;
	double currentAcceleration;
	bool bReverse;

	//bool accelerate;
	//bool brake;
	bool increaseTgtSpeed;
	bool decreaseTgtSpeed;

	//double acceleration;

	double enginePower; // between -1 (reverse) and 1

	//DiscOutPort currentSpeedPort;
	DiscInPort currentSpeed;
	DiscOutPort commandVoltage;
};

#endif //__CRAWLERENGINE_H_F05A8448_9C86_481c_B632_0641CA218CF0