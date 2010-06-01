#ifndef CRAWLERPANELLIGHT_H_7ADEE92B_D3B6_46ab_8DD2_DEB8C804DE96
#define CRAWLERPANELLIGHT_H_7ADEE92B_D3B6_46ab_8DD2_DEB8C804DE96
#pragma once

#include "BasicVCComponent.h"
#include "Crawler_vc_defs.h"
#include "discsignals.h"

namespace vc
{
	using discsignals::DiscInPort;
	using discsignals::DiscOutPort;
	using discsignals::DiscreteBundle;

class CrawlerPanelLight : public BasicVCComponent<Crawler>
{
protected:
	DiscInPort test, input;
public:
	CrawlerPanelLight(Crawler* _v, const std::string& _ident);
	virtual ~CrawlerPanelLight();
	
	virtual void ConnectTestPort(DiscreteBundle* pBundle, unsigned short usLine);
	virtual void ConnectPort(DiscreteBundle* pBundle, unsigned short usLine);
};

};

#endif