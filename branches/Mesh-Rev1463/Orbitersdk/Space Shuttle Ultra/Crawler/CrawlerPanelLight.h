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
	SURFHANDLE shSource;
	POINT sourceOn;
	POINT sourceOff;
	POINT redrawBase;
	unsigned short usWidth;
	unsigned short usHeight;

	bool bOldState;
	bool bCurrentState;
	bool bHasSourceImage;
protected:
	DiscInPort test, input;
public:
	CrawlerPanelLight(Crawler* _v, const std::string& _ident);
	virtual ~CrawlerPanelLight();

	virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
	virtual bool OnVCRedrawEvent(int id, int _event, SURFHANDLE surf);
	
	virtual void ConnectTestPort(DiscreteBundle* pBundle, unsigned short usLine);
	virtual void ConnectPort(DiscreteBundle* pBundle, unsigned short usLine);

	void SetSourceImage(SURFHANDLE _shSource);
	void SetDimensions(unsigned short _usWidth, unsigned short _usHeight);
	void SetBaseCoords(unsigned short usX, unsigned short usY);
	void SetSourceCoords(bool onState, unsigned short usX, unsigned short usY);
};

};

#endif