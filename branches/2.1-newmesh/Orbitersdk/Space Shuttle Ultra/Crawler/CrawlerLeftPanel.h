#ifndef CRAWLERLEFTPANEL_H_20FF5D63_8788_4ffb_8F4A_505FED8EEFFD
#define CRAWLERLEFTPANEL_H_20FF5D63_8788_4ffb_8F4A_505FED8EEFFD
#pragma once

#include "Crawler_vc_defs.h"
#include "CrawlerPBI.h"
#include "CrawlerPanelLight.h"
#include "CrawlerDigitalDisplay.h"

namespace vc
{

class CrawlerLeftPanel : public CrawlerPanel
{
	CrawlerPBI* pParkingBrakePBI;
	CrawlerPBI* pFwdPBI;
	CrawlerPBI* pRevPBI;
	CrawlerPBI* pNeutPBI;
	CrawlerPanelLight* pParkingBrakeLight;
	CrawlerDigitalDisplay* pCommandVoltage;

	MGROUP_ROTATE* pSpeedKnobAnimation;
	UINT anim_SpeedKnob;

	DiscInPort port_TargetSpeed;
public:
	CrawlerLeftPanel(Crawler* _v, const std::string& _ident, CRAWLER_CAB _cab);
	virtual ~CrawlerLeftPanel();

	virtual void DefineVCAnimations (UINT vcidx);
	virtual void RegisterVC();
	virtual void DefineVC();

	virtual void OnPreStep(double SimT, double SimDT, double MJD);
	virtual void Realize();
private:
	void DefineEnginePBI(CrawlerPBI* pPBI);
};

};

#endif