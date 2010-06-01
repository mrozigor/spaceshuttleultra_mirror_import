#ifndef CRAWLERPBI_H_C614EBD5_5D80_4c0b_8513_0A2A23CF6DCC
#define CRAWLERPBI_H_C614EBD5_5D80_4c0b_8513_0A2A23CF6DCC
#pragma once

#include "BasicVCComponent.h"
#include "CrawlerPanelLight.h"
#include "Crawler_vc_defs.h"

namespace vc
{

class CrawlerPBI : public CrawlerPanelLight
{
	bool bAllowReset;
	DiscOutPort output;
public:
	CrawlerPBI(Crawler* _v, const std::string& _ident);
	virtual ~CrawlerPBI();

	virtual bool OnMouseEvent(int _event, float x, float y);
	void OnPress();

	void AllowReset(bool allow);
	virtual void ConnectPort(DiscreteBundle* pBundle, unsigned short usLine);
};

};

#endif