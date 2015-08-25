#include "CrawlerPanelLight.h"

namespace vc
{

CrawlerPanelLight::CrawlerPanelLight(Crawler* _v, const std::string& _ident)
: BasicVCComponent(_v, _ident), bHasSourceImage(false), bOldState(false), bCurrentState(false),
usWidth(0), usHeight(0)	
{
}

CrawlerPanelLight::~CrawlerPanelLight()
{
}

void CrawlerPanelLight::OnPostStep(double SimT, double DeltaT, double MJD)
{
	bCurrentState = (input || test);

	if(bCurrentState != bOldState) {
		bOldState = bCurrentState;
		TriggerRedraw();
	}
}

bool CrawlerPanelLight::OnVCRedrawEvent(int id, int _event, SURFHANDLE surf)
{
	if(bHasSourceImage) {
		if(bCurrentState) {
			oapiBlt(surf, shSource, redrawBase.x, redrawBase.y,
				sourceOn.x, sourceOn.y, usWidth, usHeight);
		}
		else {
			oapiBlt(surf, shSource, redrawBase.x, redrawBase.y,
				sourceOff.x, sourceOff.y, usWidth, usHeight);
		}
		return true;
	}
	return false;
}

void CrawlerPanelLight::ConnectTestPort(discsignals::DiscreteBundle *pBundle, unsigned short usLine)
{
	test.Connect(pBundle, usLine);
}

void CrawlerPanelLight::ConnectPort(discsignals::DiscreteBundle *pBundle, unsigned short usLine)
{
	input.Connect(pBundle, usLine);
}

void CrawlerPanelLight::SetSourceImage(SURFHANDLE _shSource)
{
	shSource = _shSource;
	bHasSourceImage = (shSource!=NULL);
}

void CrawlerPanelLight::SetDimensions(unsigned short _usWidth, unsigned short _usHeight)
{
	usWidth = _usWidth;
	usHeight = _usHeight;
}

void CrawlerPanelLight::SetBaseCoords(unsigned short usX, unsigned short usY)
{
	redrawBase.x = usX;
	redrawBase.y = usY;
}

void CrawlerPanelLight::SetSourceCoords(bool onState, unsigned short usX, unsigned short usY)
{
	if(onState) {
		sourceOn.x = usX;
		sourceOn.y = usY;
	}
	else {
		sourceOff.x = usX;
		sourceOff.y = usY;
	}
}

};
