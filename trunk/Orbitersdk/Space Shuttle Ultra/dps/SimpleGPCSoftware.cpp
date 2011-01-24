#include "SimpleGPCSoftware.h"
#include "../vc/MDU.h"
#include "IDP.h"

namespace dps
{

SimpleGPCSoftware::SimpleGPCSoftware(SimpleGPCSystem* _gpc, const std::string& _ident)
: pGPC(_gpc), ident(_ident)
{
}

SimpleGPCSoftware::~SimpleGPCSoftware()
{
}

void SimpleGPCSoftware::Realize()
{
}

void SimpleGPCSoftware::OnPreStep(double SimT, double DeltaT, double MJD)
{
}

void SimpleGPCSoftware::OnPostStep(double SimT, double DeltaT, double MJD)
{
}

void SimpleGPCSoftware::OnPropagate(double SimT, double DeltaT, double MJD)
{
}

bool SimpleGPCSoftware::OnMajorModeChange(unsigned int newMajorMode)
{
	return false;
}

bool SimpleGPCSoftware::OnParseLine(const char* keyword, const char* value)
{
	return false;
}

void SimpleGPCSoftware::OnSaveState(FILEHANDLE scn) const
{
}

bool SimpleGPCSoftware::ItemInput(int spec, int item, const char* Data)
{
	return false;
}

bool SimpleGPCSoftware::ExecPressed(int spec)
{
	return false;
}

bool SimpleGPCSoftware::OnPaint(int spec, vc::MDU* pMDU) const
{
	return false;
}

SimpleGPCSoftware* SimpleGPCSoftware::FindSoftware(const std::string& identifier) const
{
	return pGPC->FindSoftware(identifier);
}

void SimpleGPCSoftware::PrintCommonHeader(const char* header, vc::MDU* pMDU) const
{
	char cbuf[200];
	char cspecbuf[4];
	char cdispbuf[4];
	char cUplink[3];
	unsigned short usDay, usHour, usMinute, usSecond;
	strcpy_s(cUplink, "  ");
	strcpy_s(cspecbuf, "   ");
	strcpy_s(cdispbuf, "   ");

	if(pMDU->GetIDP()->GetSpec() != dps::MODE_UNDEFINED)
	{
		sprintf_s(cspecbuf, 4, "%03d", pMDU->GetIDP()->GetSpec());
	}
	if(pMDU->GetIDP()->GetDisp() != dps::MODE_UNDEFINED)
	{
		sprintf_s(cdispbuf, 4, "%03d", pMDU->GetIDP()->GetDisp());
	}

	STS()->GetGPCMET(1, usDay, usHour, usMinute, usSecond);

	//Todo: GPC count their own MET independent of the MTU
	sprintf(cbuf,"%03d1/%03s/%3s  %16s  %2s  %1d %03d/%02d:%02d:%02d", 
		STS()->pSimpleGPC->GetMajorMode(),
		cspecbuf, 
		cdispbuf, 
		header, 
		cUplink, 
		0, 
		usDay, usHour, usMinute, usSecond);


	pMDU->mvprint(0, 0, cbuf);
}
};