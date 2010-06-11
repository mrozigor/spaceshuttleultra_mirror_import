#ifndef CRAWLERRIGHTPANEL_H_B50D0092_63B0_46ed_9BAC_9C3CFC05C2D5
#define CRAWLERRIGHTPANEL_H_B50D0092_63B0_46ed_9BAC_9C3CFC05C2D5
#pragma once

#include "Crawler_vc_defs.h"
#include "CrawlerPBI.h"
#include "CrawlerDigitalDisplay.h"
#include "CrawlerBarGauge.h"

namespace vc
{

class CrawlerRightPanel : public CrawlerPanel
{
	CrawlerPBI* pGreatCircle;
	CrawlerPBI* pIndependent;
	CrawlerPBI* pCrab;

	CrawlerDigitalDisplay* pLeftSteeringAngleValue; // shows angle for this cab
	CrawlerDigitalDisplay* pRightSteeringAngleValue; // shows angle for other cab
	CrawlerDigitalDisplay* pDesSteeringAngleValue;

	CrawlerBarGauge* pLeftSteeringAngleGauge; // shows angle for this cab
	CrawlerBarGauge* pRightSteeringAngleGauge; // shows angle for other cab
	CrawlerBarGauge* pDesSteeringAngleGauge;

	void DefineSteeringModePBI(CrawlerPBI* pPBI) const;
	void DefineSteeringAngleGauge(CrawlerBarGauge* pGauge) const;
	void DefineSteeringAngleDisplay(CrawlerDigitalDisplay* pDisplay) const;
public:
	CrawlerRightPanel(Crawler* _v, const std::string& _ident, CRAWLER_CAB _cab);
	virtual ~CrawlerRightPanel();

	//virtual void DefineVCAnimations (UINT vcidx);
	virtual void RegisterVC();
	virtual void DefineVC();
	virtual void Realize();

	//virtual void OnPreStep(double SimT, double SimDT, double MJD);
};

};

#endif