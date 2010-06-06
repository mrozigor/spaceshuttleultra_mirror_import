#include "CrawlerPBI.h"

namespace vc
{

CrawlerPBI::CrawlerPBI(Crawler* _v, const std::string& _ident)
: CrawlerPanelLight(_v, _ident), bAllowReset(false), usGroupCount(0)
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
	if(bAllowReset && input) output.ResetLine();
	else {
		output.SetLine();
		for(unsigned short i=0;i<usGroupCount;i++) groupPorts[i].ResetLine();
	}
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

void CrawlerPBI::ConnectGroupPort(DiscreteBundle *pBundle, unsigned short usLine)
{
	groupPorts[usGroupCount].Connect(pBundle, usLine);
	++usGroupCount;
}

};