#ifndef _CRAWLERCENTERPANEL_H_0473345F_48A7_4b64_851E_609BA1380DD3
#define _CRAWLERCENTERPANEL_H_0473345F_48A7_4b64_851E_609BA1380DD3
#pragma once

#include <orbitersdk.h>
#include <vector>
#include "Crawler_vc_defs.h"
#include "CrawlerDigitalDisplay.h"
#include "CrawlerBarGauge.h"
#include "DiscInPort.h"
#include "DiscreteBundle.h"

namespace vc
{
	using discsignals::DiscInPort;

class CrawlerCenterPanel : public CrawlerPanel
{
public:
	CrawlerCenterPanel(Crawler* _v, const std::string& _ident, CRAWLER_CAB _cab);
	~CrawlerCenterPanel();

	virtual void DefineVCAnimations (UINT vcidx);
	virtual void RegisterVC();
	virtual void DefineVC();

	virtual void OnPreStep(double SimT, double SimDT, double MJD);
	virtual void Realize();
private:
	vector<MGROUP_TRANSFORM*> vpAnimations;

	CrawlerBarGauge* pSpeedGauge;
	CrawlerDigitalDisplay* pSpeedDisplay;

	UINT anim_steering;

	DiscInPort steering;
	DiscInPort steering2DegRange;
};

};

#endif