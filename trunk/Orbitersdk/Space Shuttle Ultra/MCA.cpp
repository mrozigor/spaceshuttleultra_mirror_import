#include "MCA.h"
#include "Atlantis.h"

MCAModule::MCAModule()
{
}

MCAModule::~MCAModule()
{
}

void MCAModule::OnPropagate()
{
	//oapiWriteLog("(MCAModule::OnPropagate) Enter.");
	if(!CmdLogic.IsSet()) {
		OutPos0.ResetLine();
		OutPos1.ResetLine();
		MotorFwd.ResetLine();
		MotorRwd.ResetLine();
	} else { 

		if(MotorInPos0.IsSet())
			OutPos0.SetLine();
		else
			OutPos0.ResetLine();


		if(MotorInPos1.IsSet())
			OutPos1.SetLine();
		else
			OutPos1.ResetLine();

		if(CmdFwdA || CmdFwdB)
			MotorFwd.SetLine();
		else
			MotorFwd.ResetLine();

		if(CmdRwdA || CmdRwdB)
			MotorRwd.SetLine();
		else
			MotorRwd.ResetLine();
	}
	//oapiWriteLog("(MCAModule::OnPropagate) Exit.");
}

MCA::MCA(SubsystemDirector *_direct, const std::string &name)
: AtlantisSubsystem(_direct, name)
{

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
	for(unsigned int i = 0; i<8; i++)
		module[i].OnPropagate();
	//oapiWriteLog("(MCA::OnPropagate) Exit.");
}

void MCA::OnPostStep(double fSimT, double fDeltaT, double fMJD)
{
}