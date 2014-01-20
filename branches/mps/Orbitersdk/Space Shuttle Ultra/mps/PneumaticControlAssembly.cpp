#include "PneumaticControlAssembly.h"
#include "SSME.h"
#include "HeliumSystem.h"


namespace mps
{
	PneumaticControlAssembly::PneumaticControlAssembly( SSME* eng, HeSysEng* HeSys )
	{
		FuelSystemPurge_SV = new SolenoidValve( 0, RATE_FSP_SV, true, HeSys, nullptr );
		BleedValvesControl_SV = new SolenoidValve( 1, RATE_BVC_SV, true, HeSys, nullptr );
		EmergencyShutdown_SV = new SolenoidValve( 0, RATE_ES_SV, false, HeSys, nullptr );
		
		PurgeSequenceValve_PAV = new PressureActuatedValve( 1, RATE_PSV_PAV, nullptr, EmergencyShutdown_SV, EmergencyShutdown_SV, nullptr );

		ShutdownPurge_SV = new SolenoidValve( 0, RATE_SP_SV, true, HeSys, PurgeSequenceValve_PAV );
		HPOTPISPurge_SV = new SolenoidValve( 1, RATE_HPOTPISP_SV, true, HeSys, PurgeSequenceValve_PAV );
		
		FuelSystemPurge_PAV = new PressureActuatedValve( 0, RATE_FSP_PAV, FuelSystemPurge_SV, nullptr, HeSys, nullptr );
		OxidizerBleedValve_PAV = new PressureActuatedValve( 1, RATE_OBV_PAV, BleedValvesControl_SV, nullptr, HeSys, nullptr );
		EmergencyShutdown_PAV = new PressureActuatedValve( 0, RATE_ES_PAV, EmergencyShutdown_SV, nullptr, HeSys, nullptr );
		OxidizerSystemPurge_PAV = new PressureActuatedValve( 0, RATE_OSP_PAV, EmergencyShutdown_SV, nullptr, HeSys, nullptr );// TODO change EmergencyShutdown_SV and HeSys to GSE N2
		FuelPreburnerPurge_PAV = new PressureActuatedValve( 0, RATE_FPP_PAV, ShutdownPurge_SV, nullptr, HeSys, nullptr );//, OxidizerSystemPurge_PAV );
		OxidizerPreburnerPurge_PAV = new PressureActuatedValve( 0, RATE_OPP_PAV, ShutdownPurge_SV, nullptr, HeSys, nullptr );//, OxidizerSystemPurge_PAV );
		HPOTPISPurge_PAV = new PressureActuatedValve( 1, RATE_HPOTPISP_PAV, HPOTPISPurge_SV, nullptr, HeSys, nullptr );//, OxidizerSystemPurge_PAV );

		pressFuelSystemPurge = 0;
		pressEmergencyShutdown = 0;
		pressFuelPreburnerPurge = 0;
		pressOxidizerPreburnerPurge = 0;
		pressHPOTPISPurge = 0;

		this->eng = eng;
		this->HeSys = HeSys;
		return;
	}

	PneumaticControlAssembly::~PneumaticControlAssembly( void )
	{
		delete FuelSystemPurge_SV;
		delete BleedValvesControl_SV;
		delete EmergencyShutdown_SV;
		delete ShutdownPurge_SV;
		delete HPOTPISPurge_SV;

		delete FuelSystemPurge_PAV;
		delete OxidizerBleedValve_PAV;
		delete EmergencyShutdown_PAV;
		delete FuelPreburnerPurge_PAV;
		delete OxidizerPreburnerPurge_PAV;
		delete HPOTPISPurge_PAV;
		delete OxidizerSystemPurge_PAV;
		delete PurgeSequenceValve_PAV;
		return;
	}

	void PneumaticControlAssembly::tmestp( double fDeltaT )
	{
		FuelSystemPurge_SV->tmestp( fDeltaT );
		BleedValvesControl_SV->tmestp( fDeltaT );
		EmergencyShutdown_SV->tmestp( fDeltaT );
		ShutdownPurge_SV->tmestp( fDeltaT );
		HPOTPISPurge_SV->tmestp( fDeltaT );

		FuelSystemPurge_PAV->tmestp( fDeltaT );
		OxidizerBleedValve_PAV->tmestp( fDeltaT );
		EmergencyShutdown_PAV->tmestp( fDeltaT );
		OxidizerSystemPurge_PAV->tmestp( fDeltaT );
		PurgeSequenceValve_PAV->tmestp( fDeltaT );
		FuelPreburnerPurge_PAV->tmestp( fDeltaT );
		OxidizerPreburnerPurge_PAV->tmestp( fDeltaT );
		HPOTPISPurge_PAV->tmestp( fDeltaT );

		if (EmergencyShutdown_PAV->GetPos() >= 0.2)
		{// TODO "the pneumatic shutdown requires more helium"
			// close vlvs
			eng->ptrMOV->Close();// TODO improve the closing speed
			eng->ptrMFV->Close();
			eng->ptrCCV->Close();
			eng->ptrOPOV->Close();
			eng->ptrFPOV->Close();
		}

		if (FuelPreburnerPurge_PAV->GetPos() > 0)
		{
			pressFuelPreburnerPurge = 0.3 * FuelPreburnerPurge_PAV->GetPos() * HeSys->Use( 100 );// use He
		}
		else
		{
			pressFuelPreburnerPurge = 0;
		}
		if (OxidizerPreburnerPurge_PAV->GetPos() > 0)
		{
			pressOxidizerPreburnerPurge = OxidizerPreburnerPurge_PAV->GetPos() * HeSys->Use( 100 );// use He
		}
		else
		{
			pressOxidizerPreburnerPurge = 0;
		}

		//sprintf( oapiDebugString(), "IS%.1f|%.1f OBV%.1f|%.1f FSP%.1f OSP%.1f ESD%.1f FPBP%.1f OPBP%.1f PSV%.1f", HPOTPISPurge_PAV->GetPos(), HPOTPISPurge_SV->GetPos(), OxidizerBleedValve_PAV->GetPos(), BleedValvesControl_SV->GetPos(), FuelSystemPurge_PAV->GetPos(), OxidizerSystemPurge_PAV->GetPos(), EmergencyShutdown_PAV->GetPos(), FuelPreburnerPurge_PAV->GetPos(), OxidizerPreburnerPurge_PAV->GetPos(), PurgeSequenceValve_PAV->GetPos() );
		return;
	}

	void PneumaticControlAssembly::Realize( discsignals::DiscreteBundle* bundle )
	{
		FuelSystemPurge_SV->Connect( 0, bundle, 0 );
		FuelSystemPurge_SV->Connect( 1, bundle, 1 );

		BleedValvesControl_SV->Connect( 0, bundle, 2 );
		BleedValvesControl_SV->Connect( 1, bundle, 3 );

		EmergencyShutdown_SV->Connect( 0, bundle, 4 );
		EmergencyShutdown_SV->Connect( 1, bundle, 5 );

		ShutdownPurge_SV->Connect( 0, bundle, 6 );
		ShutdownPurge_SV->Connect( 1, bundle, 7 );

		HPOTPISPurge_SV->Connect( 0, bundle, 8 );
		HPOTPISPurge_SV->Connect( 1, bundle, 9 );
		return;
	}

	double PneumaticControlAssembly::UseHPOTPISPurge( double flow )
	{// TODO get GSE press
		pressHPOTPISPurge = 0.27604167 * HPOTPISPurge_PAV->Use( flow );// HACK no clue about the "regulator"
		return pressHPOTPISPurge;
	}
	// expected shutdown value of 518 psia for the Phase II SSME,
	// or 576 psia for the BLOCK I SSME, BLOCK IIA SSME, and the BLOCK II SSME

	double PneumaticControlAssembly::UseFuelSystemPurge( double flow )
	{
		pressFuelSystemPurge = 0.5 * FuelSystemPurge_PAV->Use( flow );// HACK no clue about the "regulator"
		return pressFuelSystemPurge;
	}

	double PneumaticControlAssembly::GetPressFuelSystemPurge() const
	{
		return pressFuelSystemPurge;
	}

	double PneumaticControlAssembly::GetPressEmergencyShutdown() const
	{
		return pressEmergencyShutdown;
	}

	double PneumaticControlAssembly::GetPressFuelPreburnerPurge() const
	{
		return pressFuelPreburnerPurge;
	}

	double PneumaticControlAssembly::GetPressOxidizerPreburnerPurge() const
	{
		return pressOxidizerPreburnerPurge;
	}

	double PneumaticControlAssembly::GetPressHPOTPISPurge() const
	{
		return pressHPOTPISPurge;
	}
}