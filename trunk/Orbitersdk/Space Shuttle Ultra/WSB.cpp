#include "WSB.h"

WSB::WSB(SubsystemDirector *_director, const std::string &_ident, unsigned short _ID)
	: AtlantisSubsystem(_director, _ident), usID(_ID)
{
	waterMass[0] = waterMass[1] = 0.0;
}

WSB::~WSB()
{
}

void WSB::Realize()
{ 
	DiscreteBundle* pBundle=BundleManager()->CreateBundle(GetIdentifier(), 16);
	Ready.Connect(pBundle, 0);
	ControllerA.Connect(pBundle, 1);
	ControllerB.Connect(pBundle, 2);
	BoilerN2Supply.Connect(pBundle, 3);
}

void WSB::OnPreStep(double SimT, double DeltaT, double MJD)
{
	// indicate if WSB is ready for APU start
	if(BoilerN2Supply && (ControllerA || ControllerB)) {
		Ready.SetLine();
		//sprintf_s(oapiDebugString(), 255, "WSB%d Ready", usID);
	}
	else {
		Ready.ResetLine();
		//sprintf_s(oapiDebugString(), 255, "WSB%d Not Ready", usID);
		//if(BoilerN2Supply) sprintf_s(oapiDebugString(), 255, "%s Supply ON", oapiDebugString());
	}
}

void WSB::OnPostStep(double SimT, double DeltaT, double MJD)
{
}

void WSB::OnPropagate(double SimT, double DeltaT, double MJD)
{
}

void WSB::OnSaveState(FILEHANDLE scn) const
{
}

bool WSB::OnParseLine(const char* keyword, const char* line)
{
	return AtlantisSubsystem::OnParseLine(keyword, line);
}