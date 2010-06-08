#include "CrawlerPBI.h"

namespace vc
{

CrawlerPBI::CrawlerPBI(Crawler* _v, const std::string& _ident)
: CrawlerPanelLight(_v, _ident), bInitialState(false), bAllowReset(false), usGroupCount(0)
{
}

CrawlerPBI::~CrawlerPBI()
{
}

void CrawlerPBI::Realize()
{
	if(bInitialState) OnPress();
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

bool CrawlerPBI::OnParseLine(const char *line)
{
	oapiWriteLog((char*)line);
	if(!_strnicmp(line, "SET", 3)) {
		bInitialState = true;
		return true;
	}
	return false;
}

bool CrawlerPBI::GetStateString(unsigned long ulBufferSize, char* pszBuffer)
{
	if(input) {
		sprintf_s(pszBuffer, ulBufferSize, "SET");
		return true;
	}
	return false;
}

};