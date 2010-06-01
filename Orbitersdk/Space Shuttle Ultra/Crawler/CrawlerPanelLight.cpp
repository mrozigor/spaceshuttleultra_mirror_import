#include "CrawlerPanelLight.h"

namespace vc
{

CrawlerPanelLight::CrawlerPanelLight(Crawler* _v, const std::string& _ident)
: BasicVCComponent(_v, _ident)
{
}

CrawlerPanelLight::~CrawlerPanelLight()
{
}

void CrawlerPanelLight::ConnectTestPort(discsignals::DiscreteBundle *pBundle, unsigned short usLine)
{
	test.Connect(pBundle, usLine);
}

void CrawlerPanelLight::ConnectPort(discsignals::DiscreteBundle *pBundle, unsigned short usLine)
{
	input.Connect(pBundle, usLine);
}

};