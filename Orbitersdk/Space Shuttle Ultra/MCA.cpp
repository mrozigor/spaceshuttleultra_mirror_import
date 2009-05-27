#include "MCA.h"
#include "Atlantis.h"

MCAModule::MCAModule()
: mc("MC")
{
}

MCAModule::~MCAModule()
{
}

void MCAModule::OnPropagate(double fSimT, double fDeltaT)
{
	//oapiWriteLog("(MCAModule::OnPropagate) Enter.");
	if(!CmdLogic.IsSet()) {
		OutPos0.ResetLine();
		OutPos1.ResetLine();
	} else { 

		if(MotorInPos0.IsSet())
			OutPos0.SetLine();
		else
			OutPos0.ResetLine();


		if(MotorInPos1.IsSet())
			OutPos1.SetLine();
		else
			OutPos1.ResetLine();
	}
	//oapiWriteLog("(MCAModule::OnPropagate) Exit.");
	mc.acPropagate(fSimT, fDeltaT);
}

MCA::MCA(SubsystemDirector *_direct, const std::string &name)
: AtlantisSubsystem(_direct, name)
{
	oapiWriteLog("(MCA::MCA)");
}

MCA::~MCA()
{
}

void MCA::OnPreStep(double fSimT, double fDeltaT, double fMJD)
{
}

void MCA::OnPropagate(double fSimT, double fDeltaT, double fMJD)
{
	
	//char pszBuffer[400];
	//sprintf_s(pszBuffer, 400, "(MCA{%s}::OnPropagate) Enter.",
	//	this->GetQualifiedIdentifier().c_str());
	//oapiWriteLog(pszBuffer);
	for(unsigned int i = 0; i<8; i++) {
		module[i].OnPropagate(fSimT, fDeltaT);
		
	}
	//oapiWriteLog("(MCA::OnPropagate) Exit.");
}

void MCA::OnPostStep(double fSimT, double fDeltaT, double fMJD)
{
}