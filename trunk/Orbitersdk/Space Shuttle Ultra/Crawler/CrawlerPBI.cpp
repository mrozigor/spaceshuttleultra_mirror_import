#include "CrawlerPBI.h"

namespace vc
{

CrawlerPBI::CrawlerPBI(Crawler* _v, const std::string& _ident)
: CrawlerPanelLight(_v, _ident), bAllowReset(false)
{
}

CrawlerPBI::~CrawlerPBI()
{
}

bool CrawlerPBI::OnMouseEvent(int _event, float x, float y)
{
	if(_event & PANEL_MOUSE_LBDOWN) {
		OnPress();
		return true;
	}

	return false;
}

void CrawlerPBI::OnPress()
{
	if(bAllowReset && input) output.SetLine();
	else output.ResetLine();
}

void CrawlerPBI::AllowReset(bool allow)
{
	bAllowReset = allow;
}

void CrawlerPBI::ConnectPort(DiscreteBundle* pBundle, unsigned short usLine)
{
	CrawlerPanelLight::ConnectPort(pBundle, usLine);
	output.Connect(pBundle, usLine);
}

};