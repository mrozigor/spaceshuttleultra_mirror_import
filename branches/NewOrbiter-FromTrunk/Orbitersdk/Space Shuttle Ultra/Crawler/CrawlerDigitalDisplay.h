#ifndef CRAWLERDIGITALDISPLAY_H_A9204225_AC97_4fe0_9ED3_7DDB927BB5DE
#define CRAWLERDIGITALDISPLAY_H_A9204225_AC97_4fe0_9ED3_7DDB927BB5DE
#pragma once

#include "Crawler_vc_defs.h"
#include "BasicVCComponent.h"
#include "discsignals.h"

namespace vc
{
	using discsignals::DiscInPort;
	using discsignals::DiscreteBundle;

/**
 * Class for displaying digital value (e.g. Steering angle)
 * Uses IDB_7SEGDIGITS resource for digit bitmaps
 */
class CrawlerDigitalDisplay : public BasicVCComponent<Crawler>
{
	DiscInPort input;
	float fOldValue;
	float fScale;

	POINT redrawBase;
	unsigned short usWidth;
	unsigned short usHeight;
	unsigned short usDigitCount;
	unsigned short usDigitWidth;
public:
	CrawlerDigitalDisplay(Crawler* _v, const std::string& _ident);
	virtual ~CrawlerDigitalDisplay();

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	virtual bool OnVCRedrawEvent(int id, int _event, SURFHANDLE surf);

	void ConnectPort(DiscreteBundle* pBundle, unsigned short usLine);

	void SetScaleValue(float _fScale);
	void SetDimensions(unsigned short _usWidth, unsigned short _usHeight, unsigned short _usDigitCount);
	void SetBaseCoords(unsigned short usX, unsigned short usY);
};

};

#endif