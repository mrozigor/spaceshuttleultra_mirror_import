#ifndef CRAWLERBARGAUGE_H_218BB240_B716_4506_9A6C_C084B6D9E439
#define CRAWLERBARGAUGE_H_218BB240_B716_4506_9A6C_C084B6D9E439
#pragma once

#include "Crawler_vc_defs.h"
#include "BasicVCComponent.h"
#include "discsignals.h"

namespace vc
{
	using discsignals::DiscInPort;
	using discsignals::DiscreteBundle;

class CrawlerBarGauge : public BasicVCComponent<Crawler>
{
	DiscInPort input;

	bool bVertical;

	double max, min;
	float fOldValue;
	float fScale;

	POINT redrawBase;
	unsigned short usWidth;
	unsigned short usHeight;
	unsigned short usBarWidth;
public:
	CrawlerBarGauge(Crawler* _v, const std::string& _ident, bool IsVertical);
	virtual ~CrawlerBarGauge();
	
	virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	virtual bool OnVCRedrawEvent(int id, int _event, SURFHANDLE surf);

	void SetScaleValue(float _fScale);
	void SetRange(double _min, double _max);
	void SetDimensions(unsigned short _usWidth, unsigned short _usHeight, unsigned short _usBarCount);
	void SetBaseCoords(unsigned short usX, unsigned short usY);
	void ConnectPort(DiscreteBundle* pBundle, unsigned short usLine);
};

};

#endif