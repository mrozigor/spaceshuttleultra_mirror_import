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

bool SimpleGPCSoftware::ItemInput(int spec, int item, const char* Data, bool &IllegalEntry )
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

bool SimpleGPCSoftware::OnDrawHUD(const HUDPAINTSPEC* hps, oapi::Sketchpad* skp) const
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
	sprintf_s(cbuf, 200, "%03d1/%03s/%3s  %-16s  %2s %1d %03d/%02d:%02d:%02d", 
		STS()->pSimpleGPC->GetMajorMode(),
		cspecbuf, 
		cdispbuf, 
		header, 
		cUplink, 
		0, 
		usDay, usHour, usMinute, usSecond);


	pMDU->mvprint(1, 0, cbuf);
}

bool SimpleGPCSoftware::GetIntegerSigned( const char *data, int &num ) const
{
	int itmp = 0;
	char* stmp = NULL;
	itmp = strtol( data, &stmp, 10 );

	if ((strlen( data ) > 0) && (strlen( stmp ) == 0))
	{
		num = itmp;
		return true;
	}
	else return false;
}

bool SimpleGPCSoftware::GetIntegerUnsigned( const char *data, int &num ) const
{
	bool ret = GetIntegerSigned( data, num );
	num = abs( num );
	return ret;
}

bool SimpleGPCSoftware::GetDoubleSigned( const char *data, double &num ) const
{
	double dtmp = 0.0;
	char* stmp = NULL;
	dtmp = strtod( data, &stmp );

	if ((strlen( data ) > 0) && (strlen( stmp ) == 0))
	{
		num = dtmp;
		return true;
	}
	else return false;
}

bool SimpleGPCSoftware::GetDoubleUnsigned( const char *data, double &num ) const
{
	bool ret = GetDoubleSigned( data, num );
	num = fabs( num );
	return ret;
}

};