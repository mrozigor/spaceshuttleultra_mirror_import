#include "HeliumSystem.h"
#include "MPSdefs.h"


namespace mps
{
	HeSysEng::HeSysEng( AtlantisSubsystemDirector* _director, const string& _ident, int ID ):AtlantisSubsystem( _director, _ident )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " HeSysEng::HeSysEng in || ID:%d", ID );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

		this->ID = ID;
		HeMass = INITIAL_HE_MASS_ENG;// + (INITIAL_HE_MASS_ENG * 0.1 * (oapiRand() - 0.5));// +/-5% variance
		TankPress = (((HeMass / AR_HE) * R * TANK_TEMP) / TANK_VOL_ENG) / 6894.757;
		RegPress = 785;
		TotalFlow = 0;

		vlvISOL_A = new SolenoidValve( 1, 1000, true, nullptr, nullptr );
		vlvISOL_B = new SolenoidValve( 1, 1000, true, nullptr, nullptr );

		vlvInIC = new SolenoidValve( 0, 1000, true, nullptr, nullptr );
		vlvOutIC = new SolenoidValve( 0, 1000, true, nullptr, nullptr );

		TankPressure = Sensor( 0, 5000 );
		RegPressure[0] = Sensor( 0, 1000 );
		RegPressure[1] = Sensor( 0, 1000 );

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " HeSysEng::HeSysEng out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	HeSysEng::~HeSysEng( void )
	{
		delete vlvISOL_A;
		delete vlvISOL_B;

		delete vlvInIC;
		delete vlvOutIC;
		return;
	}

	void HeSysEng::Realize( void )
	{
		discsignals::DiscreteBundle* bundle;
		switch (ID)
		{
			case 1:
				bundle = BundleManager()->CreateBundle( "MPS_LV_A", 16 );// LV1 - LV16
				vlvISOL_A->Connect( 0, bundle, 0 );
				vlvISOL_B->Connect( 0, bundle, 1 );

				bundle = BundleManager()->CreateBundle( "MPS_LV_D", 16 );// LV49 - LV64
				vlvInIC->Connect( 0, bundle, 10 );
				vlvOutIC->Connect( 0, bundle, 11 );

				bundle = BundleManager()->CreateBundle( "MPS_HE_SENSORS", 12 );
				TankPressure.Connect( bundle, 0 );
				RegPressure[0].Connect( bundle, 1 );
				RegPressure[1].Connect( bundle, 2 );

				sys1 = static_cast<HeSysEng*>(director->GetSubsystemByName( "HeEng_L" ));
				sys2 = static_cast<HeSysEng*>(director->GetSubsystemByName( "HeEng_R" ));
				break;
			case 2:
				bundle = BundleManager()->CreateBundle( "MPS_LV_A", 16 );// LV1 - LV16
				vlvISOL_A->Connect( 0, bundle, 2 );
				vlvISOL_B->Connect( 0, bundle, 3 );

				bundle = BundleManager()->CreateBundle( "MPS_LV_D", 16 );// LV49 - LV64
				vlvInIC->Connect( 0, bundle, 12 );
				vlvOutIC->Connect( 0, bundle, 13 );

				bundle = BundleManager()->CreateBundle( "MPS_HE_SENSORS", 12 );
				TankPressure.Connect( bundle, 3 );
				RegPressure[0].Connect( bundle, 4 );
				RegPressure[1].Connect( bundle, 5 );

				sys1 = static_cast<HeSysEng*>(director->GetSubsystemByName( "HeEng_C" ));
				sys2 = static_cast<HeSysEng*>(director->GetSubsystemByName( "HeEng_R" ));
				break;
			case 3:
				bundle = BundleManager()->CreateBundle( "MPS_LV_A", 16 );// LV1 - LV16
				vlvISOL_A->Connect( 0, bundle, 4 );
				vlvISOL_B->Connect( 0, bundle, 5 );

				bundle = BundleManager()->CreateBundle( "MPS_LV_D", 16 );// LV49 - LV64
				vlvInIC->Connect( 0, bundle, 14 );
				vlvOutIC->Connect( 0, bundle, 15 );

				bundle = BundleManager()->CreateBundle( "MPS_HE_SENSORS", 12 );
				TankPressure.Connect( bundle, 6 );
				RegPressure[0].Connect( bundle, 7 );
				RegPressure[1].Connect( bundle, 8 );

				sys1 = static_cast<HeSysEng*>(director->GetSubsystemByName( "HeEng_C" ));
				sys2 = static_cast<HeSysEng*>(director->GetSubsystemByName( "HeEng_L" ));
				break;
		}

		sysP = static_cast<HeSysPneu*>(director->GetSubsystemByName( "HePneu" ));
		return;
	}

	void HeSysEng::OnSaveState( FILEHANDLE scn ) const
	{
		char cbuf[128];
		
		sprintf_s( cbuf, 128, "PRESS" );
		oapiWriteScenario_float( scn, cbuf, TankPress );

		sprintf_s( cbuf, 128, "%f %f %f %f", vlvISOL_A->GetPos(), vlvISOL_B->GetPos(), vlvInIC->GetPos(), vlvOutIC->GetPos() );
		oapiWriteScenario_string( scn, "VALVES", cbuf );
		return;
	}

	bool HeSysEng::OnParseLine( const char* line )
	{
		double read_f1 = 0;
		double read_f2 = 0;
		double read_f3 = 0;
		double read_f4 = 0;

#ifdef _MPSDEBUG
		char buffer[128];
#endif// _MPSDEBUG

		if (!_strnicmp( line, "PRESS", 5 ))
		{
			sscanf_s( line + 5, "%lf", &read_f1 );

			if ((read_f1 >= 0) && (read_f1 <= 4500))
			{
				TankPress = read_f1;
				HeMass = (TankPress * 6894.757 * TANK_VOL_ENG * AR_HE) / ( R * TANK_TEMP);
			}
#ifdef _MPSDEBUG
			sprintf_s( buffer, 128, " HeSysEng::OnParseLine || TankPress:%f HeMass:%f", TankPress, HeMass );
			oapiWriteLog( buffer );
#endif// _MPSDEBUG
			return true;
		}
		else if (!_strnicmp( line, "VALVES", 6 ))
		{
			sscanf_s( line + 6, "%lf %lf %lf %lf", &read_f1, &read_f2, &read_f3, &read_f4 );

			if ((read_f1 >= 0) && (read_f1 <= 1))
			{
				vlvISOL_A->_backdoor( read_f1 );
			}

			if ((read_f2 >= 0) && (read_f2 <= 1))
			{
				vlvISOL_B->_backdoor( read_f2 );
			}

			if ((read_f3 >= 0) && (read_f3 <= 1))
			{
				vlvInIC->_backdoor( read_f3 );
			}

			if ((read_f4 >= 0) && (read_f4 <= 1))
			{
				vlvOutIC->_backdoor( read_f4 );
			}
#ifdef _MPSDEBUG
			sprintf_s( buffer, 128, " HeSysEng::OnParseLine || vlvISOL_A:%f vlvISOL_B:%f vlvInIC:%f vlvOutIC:%f", vlvISOL_A->GetPos(), vlvISOL_B->GetPos(), vlvInIC->GetPos(), vlvOutIC->GetPos() );
			oapiWriteLog( buffer );
#endif// _MPSDEBUG
			return true;
		}
		return false;
	}

	void HeSysEng::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		// update vlvs
		vlvISOL_A->tmestp( fDeltaT );
		vlvISOL_B->tmestp( fDeltaT );

		vlvInIC->tmestp( fDeltaT );
		vlvOutIC->tmestp( fDeltaT );

		if ((ID == 2) && (sysP->vlvLEngXOVR->GetPos() == 1))
		{
			double XOVRratio = sysP->GetRegPress() / (RegPress + sysP->GetRegPress());
			sysP->Use( TotalFlow * XOVRratio );
			TotalFlow = TotalFlow * (1 - XOVRratio);
		}

		double maxregflow = (vlvISOL_A->GetPos() + vlvISOL_B->GetPos()) * MAX_REG_FLOW;
		if (maxregflow == 0)
		{
			// TODO should opening isol vlvs look like this?
			RegPress -= 400 * fDeltaT;// to show a decay upon closing the isol vlvs
			if (RegPress < 0) RegPress = 0;
			TotalFlow = 0;

			TankPressure.SetValue( TankPress );
			RegPressure[0].SetValue( RegPress );
			RegPressure[1].SetValue( RegPress );
			return;
		}

		double regheadpress = 0;

		if (vlvInIC->GetPos() == 1)// TODO change to != 0???
		{
			double usedmass = TotalFlow * fDeltaT;
			double totalpress = 0;
			double inpress[4] = {0, 0, 0, 0};

			if (TankPress > sysP->GetTankPress())
			{
				inpress[0] = TankPress;
			}
			else
			{
				inpress[1] = sysP->GetTankPress();
				if ((sys1->IsOutICOpen() == 1) && (sys1->GetTankPress() > inpress[1])) inpress[2] = sys1->GetTankPress();
				if ((sys2->IsOutICOpen() == 1) && (sys2->GetTankPress() > inpress[1])) inpress[3] = sys2->GetTankPress();
			}

			totalpress = inpress[0] + inpress[1] + inpress[2] + inpress[3];
			
			inpress[0] = UseTank( (inpress[0] / totalpress) * usedmass );
			regheadpress = inpress[0];
			
			inpress[1] = sysP->UseTank( (inpress[1] / totalpress) * usedmass );
			if (regheadpress < inpress[1]) regheadpress = inpress[1];
			
			inpress[2] = sys1->UseTank( (inpress[2] / totalpress) * usedmass );
			if (regheadpress < inpress[2]) regheadpress = inpress[2];
			
			inpress[3] = sys2->UseTank( (inpress[3] / totalpress) * usedmass );
			if (regheadpress < inpress[3]) regheadpress = inpress[3];
		}
		else
		{
			UseTank( TotalFlow * fDeltaT );
			regheadpress = TankPress;
		}

		RegPress = 730 + (((maxregflow - TotalFlow) / maxregflow) * 55);
		if (regheadpress < RegPress) RegPress = regheadpress;
		TotalFlow = 0;

		TankPressure.SetValue( TankPress );
		RegPressure[0].SetValue( RegPress );
		RegPressure[1].SetValue( RegPress );
		return;
	}

	double HeSysEng::Use( double flow )
	{
		TotalFlow += flow;

		if ((ID == 2) && (sysP->vlvLEngXOVR->GetPos() == 1) && (RegPress < sysP->GetRegPress())) return sysP->GetRegPress();
		return RegPress;
	}

	double HeSysEng::UseTank( double mass )
	{
		HeMass -= mass;
		if (HeMass < 0) HeMass = 0;
		TankPress = (((HeMass / 4.002602) * 8.314 * TANK_TEMP) / TANK_VOL_ENG) / 6894.757;
		return TankPress;
	}

	double HeSysEng::FillTank( double mass )
	{
		HeMass += mass;
		TankPress = (((HeMass / 4.002602) * 8.314 * TANK_TEMP) / TANK_VOL_ENG) / 6894.757;
		return TankPress;
	}

	int HeSysEng::GetTankPress( void ) const
	{
		return (int)TankPress;//((int)(TankPress / 10)) * 10;
	}

	int HeSysEng::GetRegPress( void ) const
	{
		return (int)RegPress;
	}

	double HeSysEng::IsOutICOpen( void ) const
	{
		return vlvOutIC->GetPos();
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////


	HeSysPneu::HeSysPneu( AtlantisSubsystemDirector* _director, const string& _ident ):AtlantisSubsystem( _director, _ident )
	{
		HeMass = INITIAL_HE_MASS_PNEU;// + (INITIAL_HE_MASS_PNEU * 0.1 * (oapiRand() - 0.5));// +/-5% variance
		TankPress = (((HeMass / AR_HE) * R * TANK_TEMP) / TANK_VOL_PNEU) / 6894.757;
		RegPress = 770;
		TotalFlow = 0;

		vlvISOL_A = new SolenoidValve( 1, 1000, true, nullptr, nullptr );
		vlvISOL_B = new SolenoidValve( 1, 1000, true, nullptr, nullptr );

		vlvLEngXOVR = new SolenoidValve( 0, 1000, true, nullptr, nullptr );

		TankPressure = Sensor( 0, 5000 );
		RegPressure = Sensor( 0, 1000 );
		return;
	}

	HeSysPneu::~HeSysPneu( void )
	{
		delete vlvISOL_A;
		delete vlvISOL_B;

		delete vlvLEngXOVR;
		return;
	}

	void HeSysPneu::Realize( void )
	{
		discsignals::DiscreteBundle* bundle = BundleManager()->CreateBundle( "MPS_LV_A", 16 );// LV1 - LV16
		vlvISOL_A->Connect( 0, bundle, 6 );
		vlvISOL_B->Connect( 0, bundle, 7 );
		vlvLEngXOVR->Connect( 0, bundle, 9 );

		bundle = BundleManager()->CreateBundle( "MPS_HE_SENSORS", 12 );
		TankPressure.Connect( bundle, 9 );
		RegPressure.Connect( bundle, 10 );

		sys1 = static_cast<HeSysEng*>(director->GetSubsystemByName( "HeEng_C" ));
		sys2 = static_cast<HeSysEng*>(director->GetSubsystemByName( "HeEng_L" ));
		sys3 = static_cast<HeSysEng*>(director->GetSubsystemByName( "HeEng_R" ));
		return;
	}

	void HeSysPneu::OnSaveState( FILEHANDLE scn ) const
	{
		char cbuf[128];
		
		sprintf_s( cbuf, 128, "PRESS" );
		oapiWriteScenario_float( scn, cbuf, TankPress );

		sprintf_s( cbuf, 128, "%f %f %f", vlvISOL_A->GetPos(), vlvISOL_B->GetPos(), vlvLEngXOVR->GetPos() );
		oapiWriteScenario_string( scn, "VALVES", cbuf );
		return;
	}

	bool HeSysPneu::OnParseLine( const char* line )
	{
		double read_f1 = 0;
		double read_f2 = 0;
		double read_f3 = 0;

#ifdef _MPSDEBUG
		char buffer[128];
#endif// _MPSDEBUG

		if (!_strnicmp( line, "PRESS", 5 ))
		{
			sscanf_s( line + 5, "%lf", &read_f1 );

			if ((read_f1 >= 0) && (read_f1 <= 4500))
			{
				TankPress = read_f1;
				HeMass = (TankPress * 6894.757 * TANK_VOL_PNEU * AR_HE) / ( R * TANK_TEMP);
			}
#ifdef _MPSDEBUG
			sprintf_s( buffer, 128, " HeSysPneu::OnParseLine || TankPress:%f HeMass:%f", TankPress, HeMass );
			oapiWriteLog( buffer );
#endif// _MPSDEBUG
			return true;
		}
		else if (!_strnicmp( line, "VALVES", 6 ))
		{
			sscanf_s( line + 6, "%lf %lf %lf", &read_f1, &read_f2, &read_f3 );

			if ((read_f1 >= 0) && (read_f1 <= 1))
			{
				vlvISOL_A->_backdoor( read_f1 );
			}

			if ((read_f2 >= 0) && (read_f2 <= 1))
			{
				vlvISOL_B->_backdoor( read_f2 );
			}

			if ((read_f3 >= 0) && (read_f3 <= 1))
			{
				vlvLEngXOVR->_backdoor( read_f3 );
			}
#ifdef _MPSDEBUG
			sprintf_s( buffer, 128, " HeSysPneu::OnParseLine || vlvISOL_A:%f vlvISOL_B:%f vlvLEngXOVR:%f", vlvISOL_A->GetPos(), vlvISOL_B->GetPos(), vlvLEngXOVR->GetPos() );
			oapiWriteLog( buffer );
#endif// _MPSDEBUG
			return true;
		}
		return false;
	}

	void HeSysPneu::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		// update vlvs
		vlvISOL_A->tmestp( fDeltaT );
		vlvISOL_B->tmestp( fDeltaT );

		vlvLEngXOVR->tmestp( fDeltaT );

		if (vlvLEngXOVR->GetPos() == 1)
		{
			double XOVRratio = sys2->GetRegPress() / (RegPress + sys2->GetRegPress());
			sys2->Use( TotalFlow * XOVRratio );
			TotalFlow = TotalFlow * (1 - XOVRratio);
		}

		double maxregflow = (vlvISOL_A->GetPos() + vlvISOL_B->GetPos()) * MAX_REG_FLOW;
		if (maxregflow == 0)
		{
			RegPress -= 400 * fDeltaT;// to show a decay upon closing the isol vlvs
			if (RegPress < 0) RegPress = 0;
			TotalFlow = 0;
		}

		// I/C
		double inpress[3] = {0, 0, 0};

		if ((sys1->IsOutICOpen() == 1) && (sys1->GetTankPress() > TankPress)) inpress[0] = sys1->GetTankPress();
		if ((sys2->IsOutICOpen() == 1) && (sys2->GetTankPress() > TankPress)) inpress[1] = sys2->GetTankPress();
		if ((sys3->IsOutICOpen() == 1) && (sys3->GetTankPress() > TankPress)) inpress[2] = sys3->GetTankPress();
		
		double totalpress = inpress[0] + inpress[1] + inpress[2];

		if (totalpress > 0)
		{
			// TODO it would be nice to equalize press perfectly, but it's not that bad with current "brute force" approach
			double ICmass = MAX_TANK_FLOW * fDeltaT;

			inpress[0] = sys1->UseTank( (inpress[0] / totalpress) * ICmass );
			inpress[1] = sys2->UseTank( (inpress[1] / totalpress) * ICmass );
			inpress[2] = sys3->UseTank( (inpress[2] / totalpress) * ICmass );

			UseTank( (TotalFlow * fDeltaT) - ICmass );

			//sprintf_s( oapiDebugString(), 255, "%f|%f|%f|%f", totalpress, inpress[0] / totalpress, inpress[1] / totalpress, inpress[2] / totalpress );
		}
		else
		{
			UseTank( TotalFlow * fDeltaT );
		}
		
		if (maxregflow > 0)// to not divide by 0 if ISOLs are closed
		{
			RegPress = 715 + (((maxregflow - TotalFlow) / maxregflow) * 55);
			if (TankPress < RegPress) RegPress = TankPress;
		}
		TotalFlow = 0;

		TankPressure.SetValue( TankPress );
		RegPressure.SetValue( RegPress );
		return;
	}

	double HeSysPneu::Use( double flow )
	{
		TotalFlow += flow;

		if ((vlvLEngXOVR->GetPos() == 1) && (RegPress < sys2->GetRegPress())) return sys2->GetRegPress();
		return RegPress;
	}

	double HeSysPneu::UseTank( double mass )
	{
		HeMass -= mass;
		if (HeMass < 0) HeMass = 0;
		TankPress = (((HeMass / AR_HE) * R * TANK_TEMP) / TANK_VOL_PNEU) / 6894.757;
		return TankPress;
	}

	double HeSysPneu::FillTank( double mass )
	{
		HeMass += mass;
		TankPress = (((HeMass / AR_HE) * R * TANK_TEMP) / TANK_VOL_PNEU) / 6894.757;
		return TankPress;
	}

	int HeSysPneu::GetTankPress( void ) const
	{
		return (int)TankPress;//((int)(TankPress / 10)) * 10;
	}

	int HeSysPneu::GetRegPress( void ) const
	{
		return (int)RegPress;
	}
}
