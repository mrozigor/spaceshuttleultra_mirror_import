#include "IDP.h"

namespace dps {

	IDP::IDP(SubsystemDirector* _direct, const std::string& _ident, 
		unsigned short _usIDPID)
		: AtlantisSubsystem(_direct, _ident), usIDPID(_usIDPID)
	{
		usSPEC=0;
		usDISP=0;
	}

	IDP::~IDP()
	{
	}

	unsigned short IDP::GetIDPID() const {
		return usIDPID;
	}

	unsigned short IDP::GetDisp() const
	{
		return usDISP;
	}

	unsigned short IDP::GetSpec() const
	{
		return usSPEC;
	}

	void IDP::SetDisp(unsigned short disp)
	{
		usDISP=disp;
	}

	void IDP::SetSpec(unsigned short spec)
	{
		usSPEC=spec;
	}

	void IDP::ConnectToMDU(vc::PMDU pMDU, bool bPrimary)
	{
		if(pMDU) {
			if(bPrimary) {
				pMDU->SetPrimaryIDP(this);
			}
			else pMDU->SetSecondaryIDP(this);
		}
	}

	void IDP::OnSaveState(FILEHANDLE scn) const
	{
		char cbuf[255];
		sprintf_s(cbuf, 255, "IDP%d SPEC", usIDPID);
		oapiWriteScenario_int(scn, cbuf, usSPEC);
	}

	bool IDP::OnParseLine(const char* line)
	{
		char IDP[10];
		sprintf_s(IDP, 10, "IDP%d", usIDPID);
		if(!_strnicmp(IDP, line, 4)) {
			if(!_strnicmp(line+5, "SPEC", 4)) {
				sscanf(line+9, "%d", &usSPEC);
				return true;
			}
		}
		return false;
	}
};
