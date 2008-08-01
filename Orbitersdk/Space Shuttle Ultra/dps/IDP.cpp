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
				bool Success=pMDU->SetPrimaryIDP(this);
				if(Success) oapiWriteLog("IDP connected to MDU");
			}
			else pMDU->SetSecondaryIDP(this);
			pMDU->prim_idp2=this;
		}
		else oapiWriteLog("ERROR: IDP unable to connect to MDU");
	}
};
