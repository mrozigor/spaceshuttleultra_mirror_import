#include "APU.h"
#include "Atlantis.h"
#include <OrbiterSoundSDK40.h>

APU::APU(AtlantisSubsystemDirector *_director, const std::string &_ident, int _ID)
	: AtlantisSubsystem(_director, _ident), ID(_ID), phTank(NULL)
{
	State=OFF;
	FuelLevel[0]=FuelLevel[1]=0.0;
	HydraulicPressure[0]=HydraulicPressure[1]=0.0;
	APUSpeed[0]=APUSpeed[1]=0.0;
	FuelPress[0]=FuelPress[1]=0.0;	
}

APU::~APU()
{
}

/*void APU::CreateTanks()
{
	phTank=STS()->CreatePropellantResource(APU_FUEL_TANK_MASS);
}*/

void APU::DefineTank(PROPELLANT_HANDLE _phTank)
{
	if(!phTank) phTank=_phTank;
}

double APU::GetHydraulicPressure() const
{
	return HydraulicPressure[0];
}

double APU::GetFuelPressure() const
{
	return FuelPress[0];
}

double APU::GetFuelLevel() const
{
	return FuelLevel[0];
}

double APU::GetAPUSpeed() const
{
	return APUSpeed[0];
}

bool APU::IsRunning() const
{
	return (State==ON);
}

void APU::Realize()
{
	std::string name="R2_To_"+GetIdentifier();
	DiscreteBundle *pBundle=STS()->BundleManager()->CreateBundle(name, 16);

	APU_Run.Connect(pBundle, 0);
	APU_CntlrPwr.Connect(pBundle, 1);
	APU_FuelTankValves.Connect(pBundle, 2);
	APU_HydPumpPress.Connect(pBundle, 3);
	APU_HydraulicPress.Connect(pBundle, 4);
	APU_ReadyToStart.Connect(pBundle, 5);

	char cbuf[255];
	sprintf_s(cbuf, 255, "WSB%d", ID);
	pBundle=BundleManager()->CreateBundle(cbuf, 16);
	WSB_Ready.Connect(pBundle, 0);

	FuelLevel[0]=FuelLevel[1]=STS()->GetPropellantMass(phTank);
}

void APU::OnPreStep(double SimT, double DeltaT, double MJD)
{
	switch(State) {
		case SHUTDOWN:
			// sound only plays in external view, so keep calling PlayVesselWave to make sure sound plays if we switch to external view
			if(STS()->GetSoundID()!=-1 && IsPlaying(STS()->GetSoundID(), APU_SHUTDOWN)) {
				PlayVesselWave(STS()->GetSoundID(), APU_SHUTDOWN, NOLOOP);
			}
		case OFF:
			if(APUSpeed[1]>5) APUSpeed[1]=max(APUSpeed[1]-15.0*DeltaT, 0.0);
			else {
				APUSpeed[1]=0.0;
				State=OFF;
			}
			if(FuelPress[1]>25) FuelPress[1] -= 200*DeltaT;
			else FuelPress[1] = 0.0;
			if(HydraulicPressure[1]>50) HydraulicPressure[1] -= 200*DeltaT;
			else HydraulicPressure[1] = 0.0;

			if(ReadyToStart()) {
				APU_ReadyToStart.SetLine();
				if(APU_Run) {
					State=START;
					if(STS()->GetSoundID()!=-1) PlayVesselWave(STS()->GetSoundID(), APU_START, NOLOOP);
				}
			}
			else APU_ReadyToStart.ResetLine();

			break;
		case START:
		case ON:
			// sound only plays in external view, so keep calling PlayVesselWave to make sure sound plays if we switch to external view
			if(STS()->GetSoundID()!=-1) {
				if(State==START && IsPlaying(STS()->GetSoundID(), APU_START)) PlayVesselWave(STS()->GetSoundID(), APU_START, NOLOOP);
				else PlayVesselWave(STS()->GetSoundID(), APU_RUNNING, LOOP);
			}

			if(FuelLevel[0]<=0.0) State=SHUTDOWN;
			if(!APU_Run) {
				State=SHUTDOWN;
				if(STS()->GetSoundID()!=-1) PlayVesselWave(STS()->GetSoundID(), APU_SHUTDOWN, NOLOOP);
			}

			if(APU_HydPumpPress) 
				FuelLevel[1]-=APU_FUEL_TANK_FLOWRATE[0]*DeltaT;
			else FuelLevel[1]-=APU_FUEL_TANK_FLOWRATE[1]*DeltaT;

			if(State==START) {
				if(APUSpeed[1]>=95.0) {
					APUSpeed[1]=103.0+(oapiRand()-0.5);
					State=ON;
				}
				else {
					APUSpeed[1]+=15.0*DeltaT;
				}

				APU_ReadyToStart.SetLine(); //in START sequence, RTS talkback is still gray
			}
			else {
				APUSpeed[1]+=0.75*(oapiRand()-0.5);

				APU_ReadyToStart.ResetLine(); //if APU is on, RTS talkback displays barberpole
			}

			if(FuelPress[1]<1400) FuelPress[1]+=200*DeltaT;
			else if(FuelPress[1]>1500) FuelPress[1]-=200*DeltaT;
			else if(FuelPress[1]<1425 || FuelPress[1]>1480)
				FuelPress[1]=1455 - 10.0 * oapiRand();
			else FuelPress[1] += (2.0 * oapiRand() - 1.0);

			if(APU_HydPumpPress) {
				if(HydraulicPressure[1]<2950) {
					if(HydraulicPressure[1]>2900)
						HydraulicPressure[1]=3000-(10*oapiRand());
					else 
						HydraulicPressure[1]+=200*DeltaT;
				}
				else if(HydraulicPressure[1]>3050)
					HydraulicPressure[1]-=200*DeltaT;
				else
					HydraulicPressure[1]+=2.0 * oapiRand() - 1.0;
			}
			else {
				if(HydraulicPressure[1]<850) {
					if(HydraulicPressure[1]>800) 
						HydraulicPressure[1]=900-10*oapiRand();
					else HydraulicPressure[1]+=200*DeltaT;
				}
				else if(HydraulicPressure[1]>950)
					HydraulicPressure[1]-=200*DeltaT;
				else {
					HydraulicPressure[1]+=(2.0*oapiRand() - 1.0);
				}
			}

			break;
	}
	//sprintf_s(oapiDebugString(), 255, "APU Fuel: %f %f APU State: %d", FuelLevel[0], FuelLevel[1], State);
}

void APU::OnPropagate(double SimT, double DeltaT, double MJD)
{
	FuelLevel[0]=FuelLevel[1];
	FuelPress[0]=FuelPress[1];
	HydraulicPressure[0]=HydraulicPressure[1];
	APUSpeed[0]=APUSpeed[1];
}

void APU::OnPostStep(double SimT, double DeltaT, double MJD)
{
	if(State!=OFF) STS()->SetPropellantMass(phTank, FuelLevel[0]);
	APU_HydraulicPress.SetLine((float)(HydraulicPressure[0]/1000.0));
}

void APU::OnSaveState(FILEHANDLE scn) const
{
	char cbuf[255];

	sprintf_s(cbuf, 255, "%s_State", GetIdentifier().c_str());
	oapiWriteScenario_int(scn, cbuf, State);
	sprintf_s(cbuf, 255, "%s_FuelPress", GetIdentifier().c_str());
	oapiWriteScenario_float(scn, cbuf, FuelPress[0]);
	sprintf_s(cbuf, 255, "%s_HydPress", GetIdentifier().c_str());
	oapiWriteScenario_float(scn, cbuf, HydraulicPressure[0]);
	sprintf_s(cbuf, 255, "%s_Speed", GetIdentifier().c_str());
	oapiWriteScenario_float(scn, cbuf, APUSpeed[0]);
}

bool APU::OnParseLine(const char *line)
{
	if(!_strnicmp(line, (GetIdentifier()+"_State").c_str(), (GetIdentifier()+"_State").size())) {
		sscanf_s(line+(GetIdentifier()+"_State").size(), "%d", &State);
		return true;
	}
	else if(!_strnicmp(line, (GetIdentifier()+"_FuelPress").c_str(), (GetIdentifier()+"_FuelPress").size())) {
		sscanf_s(line+(GetIdentifier()+"_FuelPress").size(), "%lf", &FuelPress[0]);
		FuelPress[1]=FuelPress[0];
		return true;
	}
	else if(!_strnicmp(line, (GetIdentifier()+"_HydPress").c_str(), (GetIdentifier()+"_HydPress").size())) {
		sscanf_s(line+(GetIdentifier()+"_HydPress").size(), "%lf", &HydraulicPressure[0]);
		HydraulicPressure[1]=HydraulicPressure[0];
		return true;
	}
	else if(!_strnicmp(line, (GetIdentifier()+"_Speed").c_str(), (GetIdentifier()+"_Speed").size())) {
		sscanf_s(line+(GetIdentifier()+"_Speed").size(), "%lf", &APUSpeed[0]);
		APUSpeed[1]=APUSpeed[0];
		return true;
	}
	return false;
}

bool APU::ReadyToStart() const
{
	return (FuelLevel[0]>0.0 && APU_CntlrPwr && APU_FuelTankValves && !APU_HydPumpPress && WSB_Ready);
}
