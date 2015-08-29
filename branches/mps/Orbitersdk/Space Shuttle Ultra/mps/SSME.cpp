#include "SSME.h"
#include "SSMEController_BLOCK_II.h"
#include "PneumaticControlAssembly.h"
#include "MPS.h"
#include "MPSdefs.h"


namespace mps
{
	SSME::SSME( AtlantisSubsystemDirector* _director, const string& _ident, unsigned short ID, int controllertype, const string& sw, HeSysEng* HeSys ):AtlantisSubsystem( _director, _ident )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " SSME::SSME in || name:%s|ID:%d|controllertype:%d|sw:%s", _ident.c_str(), ID, controllertype, sw.c_str() );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

		this->ID = ID;

		PCA = new PneumaticControlAssembly( this, HeSys );
		
		// make valves
		ptrOPOV = new HydraulicActuatedValve( 0, MAX_RATE_OPOV, 30, PCA->EmergencyShutdown_PAV, 0.57 );// HACK no clue about correct sequence valve value
		ptrMOV = new HydraulicActuatedValve( 0, MAX_RATE_MOV, 40, ptrOPOV );
		ptrFPOV = new HydraulicActuatedValve( 0, MAX_RATE_FPOV, 30, ptrOPOV, 0.4 );// HACK no clue about correct sequence valve value
		ptrMFV = new HydraulicActuatedValve( 0, MAX_RATE_MFV, 25, ptrFPOV );
		ptrCCV = new HydraulicActuatedValve( 0, MAX_RATE_CCV, 23, ptrFPOV, 0.05 );// HACK no clue about correct sequence valve value
		PCA->PurgeSequenceValve_PAV->SetPressureSources( nullptr, ptrCCV, nullptr, nullptr );

		ptrAFV = new SolenoidValve( 0, RATE_AFV, true, nullptr, nullptr );
		ptrHPV_SV = new SolenoidValve( 0, RATE_HPV_SV, true, HeSys, PCA->PurgeSequenceValve_PAV );
		ptrOBV = new PressureActuatedValve( 1, RATE_OBV, PCA->OxidizerBleedValve_PAV, nullptr, nullptr, nullptr );
		ptrFBV = new PressureActuatedValve( 1, RATE_FBV, PCA->BleedValvesControl_SV, nullptr, nullptr, nullptr );
		ptrHPV = new PressureActuatedValve( 0, RATE_HPV, ptrHPV_SV, nullptr, HeSys, nullptr );
		ptrGCV = new PressureActuatedValve( 1, RATE_GCV, nullptr, ptrHPV_SV, nullptr, nullptr );
		ptrRIV = new PressureActuatedValve( 0, RATE_RIV, nullptr, PCA->OxidizerBleedValve_PAV, nullptr, nullptr );

		// hardware model stuff
		modelmode = 1;
		modeltime = 0;

		// igniters
		Igniter_MCC[0] = false;
		Igniter_MCC[1] = false;
		Igniter_FPB[0] = false;
		Igniter_FPB[1] = false;
		Igniter_OPB[0] = false;
		Igniter_OPB[1] = false;

		// make controller
		/*if (controllertype == 1)// TODO improve/add error checking
		{
			Controller = new SSMEController_BLOCK_I( this, sw );
		}
		else
		{*/
			Controller = new SSMEController_BLOCK_II( this, sw );
		//}

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " SSME::SSME out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	SSME::~SSME( void )
	{
		delete ptrCCV;
		delete ptrMOV;
		delete ptrMFV;
		delete ptrFPOV;
		delete ptrOPOV;
		delete ptrAFV;
		delete ptrHPV_SV;
		delete ptrOBV;
		delete ptrFBV;
		delete ptrHPV;
		delete ptrGCV;
		delete ptrRIV;

		delete Controller;

		delete PCA;
		return;
	}

	void SSME::Realize()
	{
		pMPS = static_cast<MPS*>(director->GetSubsystemByName( "MPS" ));

		STS()->SetSSMEParams( ID, FPL_THRUST, ISP0, ISP1 );

		// connect AC to PSE
		discsignals::DiscreteBundle* bundle_hyd;
		discsignals::DiscreteBundle* bundle_power;
		discsignals::DiscreteBundle* bundle_OEout;// TODO separate chs?
		discsignals::DiscreteBundle* bundle_OEoutCCV;
		discsignals::DiscreteBundle* bundle_OEoutMFV;
		discsignals::DiscreteBundle* bundle_OEoutMOV;
		discsignals::DiscreteBundle* bundle_OEoutFPOV;
		discsignals::DiscreteBundle* bundle_OEoutOPOV;
		discsignals::DiscreteBundle* bundle_IEchA_Press;
		discsignals::DiscreteBundle* bundle_IEchB_Press;
		discsignals::DiscreteBundle* bundle_IEchA_Temp;
		discsignals::DiscreteBundle* bundle_IEchB_Temp;
		discsignals::DiscreteBundle* bundle_IEchA_Flow;
		discsignals::DiscreteBundle* bundle_IEchB_Flow;
		discsignals::DiscreteBundle* bundle_IEchA_Speed;
		discsignals::DiscreteBundle* bundle_IEchB_Speed;

		switch (ID)
		{
			case 1:
				bundle_hyd = BundleManager()->CreateBundle( "R2_To_APU1", 16 );
				bundle_power = BundleManager()->CreateBundle( "SSMEC_R2_SWITCHES", 8 );
				break;
			case 2:
				bundle_hyd = BundleManager()->CreateBundle( "R2_To_APU2", 16 );
				bundle_power = BundleManager()->CreateBundle( "SSMEL_R2_SWITCHES", 8 );
				break;
			case 3:
				bundle_hyd = BundleManager()->CreateBundle( "R2_To_APU3", 16 );
				bundle_power = BundleManager()->CreateBundle( "SSMER_R2_SWITCHES", 8 );
				break;
		}

		char cbuf[32];
		sprintf_s( cbuf, 32, "OE_SSME_%d", ID );
		bundle_OEout = BundleManager()->CreateBundle( cbuf, 14 );

		sprintf_s( cbuf, 32, "OE_SSME_%d_CCV", ID );
		bundle_OEoutCCV = BundleManager()->CreateBundle( cbuf, 6 );

		sprintf_s( cbuf, 32, "OE_SSME_%d_MFV", ID );
		bundle_OEoutMFV = BundleManager()->CreateBundle( cbuf, 6 );

		sprintf_s( cbuf, 32, "OE_SSME_%d_MOV", ID );
		bundle_OEoutMOV = BundleManager()->CreateBundle( cbuf, 6 );

		sprintf_s( cbuf, 32, "OE_SSME_%d_FPOV", ID );
		bundle_OEoutFPOV = BundleManager()->CreateBundle( cbuf, 6 );

		sprintf_s( cbuf, 32, "OE_SSME_%d_OPOV", ID );
		bundle_OEoutOPOV = BundleManager()->CreateBundle( cbuf, 6 );

		sprintf_s( cbuf, 32, "SSME_%d_IEchA_Press", ID );
		bundle_IEchA_Press = BundleManager()->CreateBundle( cbuf, 15 );

		sprintf_s( cbuf, 32, "SSME_%d_IEchB_Press", ID );
		bundle_IEchB_Press = BundleManager()->CreateBundle( cbuf, 15 );

		sprintf_s( cbuf, 32, "SSME_%d_IEchA_Temp", ID );
		bundle_IEchA_Temp = BundleManager()->CreateBundle( cbuf, 8 );

		sprintf_s( cbuf, 32, "SSME_%d_IEchB_Temp", ID );
		bundle_IEchB_Temp = BundleManager()->CreateBundle( cbuf, 8 );

		sprintf_s( cbuf, 32, "SSME_%d_IEchA_Flow", ID );
		bundle_IEchA_Flow = BundleManager()->CreateBundle( cbuf, 4 );

		sprintf_s( cbuf, 32, "SSME_%d_IEchB_Flow", ID );
		bundle_IEchB_Flow = BundleManager()->CreateBundle( cbuf, 4 );

		sprintf_s( cbuf, 32, "SSME_%d_IEchA_Speed", ID );
		bundle_IEchA_Speed = BundleManager()->CreateBundle( cbuf, 3 );

		sprintf_s( cbuf, 32, "SSME_%d_IEchB_Speed", ID );
		bundle_IEchB_Speed = BundleManager()->CreateBundle( cbuf, 3 );

		Controller->Realize( bundle_power, bundle_OEout, bundle_OEoutCCV, bundle_OEoutMFV, bundle_OEoutMOV, bundle_OEoutFPOV, bundle_OEoutOPOV, bundle_IEchA_Press, bundle_IEchB_Press, bundle_IEchA_Temp, bundle_IEchB_Temp, bundle_IEchA_Flow, bundle_IEchB_Flow, bundle_IEchA_Speed, bundle_IEchB_Speed );

		ConnectSensors( bundle_IEchA_Press, bundle_IEchB_Press, bundle_IEchA_Temp, bundle_IEchB_Temp, bundle_IEchA_Flow, bundle_IEchB_Flow, bundle_IEchA_Speed, bundle_IEchB_Speed );

		PCA->Realize( bundle_OEout );

		ptrAFV->Connect( 0, bundle_OEout, 10 );
		ptrAFV->Connect( 1, bundle_OEout, 11 );
		ptrHPV_SV->Connect( 0, bundle_OEout, 12 );
		ptrHPV_SV->Connect( 1, bundle_OEout, 13 );

		ptrCCV->Connect( bundle_OEoutCCV, bundle_hyd );
		ptrMFV->Connect( bundle_OEoutMFV, bundle_hyd );
		ptrMOV->Connect( bundle_OEoutMOV, bundle_hyd );
		ptrFPOV->Connect( bundle_OEoutFPOV, bundle_hyd );
		ptrOPOV->Connect( bundle_OEoutOPOV, bundle_hyd );
		return;
	}

	double SSME::PCfromPCTtoPSI( double pcPCT ) const
	{
		return ((pcPCT * RPL_PC_PRESS) / 100);
	}

	double SSME::PCfromPSItoPCT( double pcPSI ) const
	{
		return ((pcPSI * 100) / RPL_PC_PRESS);
	}

	void SSME::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		// valves first
		ptrCCV->tmestp( fDeltaT );
		ptrMOV->tmestp( fDeltaT );
		ptrMFV->tmestp( fDeltaT );
		ptrFPOV->tmestp( fDeltaT );
		ptrOPOV->tmestp( fDeltaT );
		ptrAFV->tmestp( fDeltaT );
		ptrHPV_SV->tmestp( fDeltaT );
		ptrOBV->tmestp( fDeltaT );
		ptrFBV->tmestp( fDeltaT );
		ptrHPV->tmestp( fDeltaT );
		ptrGCV->tmestp( fDeltaT );
		ptrRIV->tmestp( fDeltaT );
		
		// engine model
		SSMERUN( fSimT, fDeltaT );

		// run controller
		Controller->tmestp( fSimT, fDeltaT );

		// run PCA
		PCA->tmestp( fDeltaT );

		ptrHPV->Use( 30 );// HACK no clue what value should be, probably much less than this
		return;
	}

	void SSME::OnSaveState( FILEHANDLE scn ) const
	{
		char cbuf[512];

		oapiWriteScenario_int( scn, "config", modelmode );

		sprintf_s( cbuf, 512, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", 
			ptrCCV->GetPos(), 
			ptrMFV->GetPos(), 
			ptrMOV->GetPos(), 
			ptrFPOV->GetPos(), 
			ptrOPOV->GetPos(), 
			ptrAFV->GetPos(), 
			ptrHPV_SV->GetPos(), 
			ptrFBV->GetPos(), 
			ptrOBV->GetPos(), 
			ptrHPV->GetPos(), 
			ptrGCV->GetPos(), 
			ptrRIV->GetPos(), 
			PCA->FuelSystemPurge_SV->GetPos(), 
			PCA->BleedValvesControl_SV->GetPos(), 
			PCA->EmergencyShutdown_SV->GetPos(), 
			PCA->PurgeSequenceValve_PAV->GetPos(), 
			PCA->ShutdownPurge_SV->GetPos(), 
			PCA->HPOTPISPurge_SV->GetPos(), 
			PCA->FuelSystemPurge_PAV->GetPos(), 
			PCA->OxidizerBleedValve_PAV->GetPos(), 
			PCA->EmergencyShutdown_PAV->GetPos(), 
			PCA->OxidizerSystemPurge_PAV->GetPos(), 
			PCA->FuelPreburnerPurge_PAV->GetPos(), 
			PCA->OxidizerPreburnerPurge_PAV->GetPos(), 
			PCA->HPOTPISPurge_PAV->GetPos() );
		oapiWriteScenario_string( scn, "valves", cbuf );

		__OnSaveState( scn );// write derived class
		Controller->__OnSaveState( scn );// write controller
		return;
	}

	bool SSME::OnParseLine( const char* line )
	{
		if (!_strnicmp( line, "config", 6 ))
		{
			sscanf_s( line + 6, "%d", &modelmode );
#ifdef _MPSDEBUG
			char buffer[100];
			sprintf_s( buffer, 100, " SSME::OnParseLine || SSME_config:%d", modelmode );
			oapiWriteLog( buffer );
#endif// _MPSDEBUG
			return true;
		}
		else if (!_strnicmp( line, "valves", 6 ))
		{
			float ccv = 0;
			float mfv = 0;
			float mov = 0;
			float fpov = 0;
			float opov = 0;
			float afv = 0;
			float hpvsv = 0;
			float fbv = 0;
			float obv = 0;
			float hpv = 0;
			float gcv = 0;
			float riv = 0;
			float fspsv = 0;
			float bvcsv = 0;
			float essv = 0;
			float psvpav = 0;
			float spsv = 0;
			float hispsv = 0;
			float fsppav = 0;
			float obvpav = 0;
			float espav = 0;
			float osppav = 0;
			float fpppav = 0;
			float opppav = 0;
			float hisppav = 0;
			sscanf_s( line + 6, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", 
				&ccv, 
				&mfv, 
				&mov, 
				&fpov, 
				&opov, 
				&afv, 
				&hpvsv, 
				&fbv, 
				&obv, 
				&hpv, 
				&gcv, 
				&riv, 
				&fspsv, 
				&bvcsv, 
				&essv, 
				&psvpav, 
				&spsv, 
				&hispsv, 
				&fsppav, 
				&obvpav, 
				&espav, 
				&osppav, 
				&fpppav, 
				&opppav, 
				&hisppav );

			ptrCCV->_backdoor( ccv );
			ptrMFV->_backdoor( mfv );
			ptrMOV->_backdoor( mov );
			ptrFPOV->_backdoor( fpov );
			ptrOPOV->_backdoor( opov );
			ptrAFV->_backdoor( afv );
			ptrHPV_SV->_backdoor( hpvsv );
			ptrFBV->_backdoor( fbv );
			ptrOBV->_backdoor( obv );
			ptrHPV->_backdoor( hpv );
			ptrGCV->_backdoor( gcv );
			ptrRIV->_backdoor( riv );
			PCA->FuelSystemPurge_SV->_backdoor( fspsv );
			PCA->BleedValvesControl_SV->_backdoor( bvcsv );
			PCA->EmergencyShutdown_SV->_backdoor( essv );
			PCA->PurgeSequenceValve_PAV->_backdoor( psvpav );
			PCA->ShutdownPurge_SV->_backdoor( spsv );
			PCA->HPOTPISPurge_SV->_backdoor( hispsv );
			PCA->FuelSystemPurge_PAV->_backdoor( fsppav );
			PCA->OxidizerBleedValve_PAV->_backdoor( obvpav );
			PCA->EmergencyShutdown_PAV->_backdoor( espav );
			PCA->OxidizerSystemPurge_PAV->_backdoor( osppav );
			PCA->FuelPreburnerPurge_PAV->_backdoor( fpppav );
			PCA->OxidizerPreburnerPurge_PAV->_backdoor( opppav );
			PCA->HPOTPISPurge_PAV->_backdoor( hisppav );
			return true;
		}

		if (__OnParseLine( line )) return true;// check if derived class wants line
		return Controller->__OnParseLine( line );// check if controller wants line
	}


	void SSME::SSMERUN( double time, double dt )
	{
		switch (modelmode)
		{
			case 1:// pre-start
				RUN1( time, dt );
				break;
			case 2:// ignition
				RUN2( time, dt );
				break;
			case 3:// mainstage
				RUN3( time, dt );
				break;
			case 4:// cutoff
				RUN4( time, dt );
				break;
			case 5:// post-shutdown
				RUN5( time, dt );
				break;
		}
		return;
	}

	// data cookup
	// ESC
	double SSME::dcPC_ESC( double tme )
	{
		double pc;

		if (tme <= 0.1)
		{
			pc = 0;
		}
		else if (tme <= 0.95)
		{
			pc = 0.1;
		}
		else if (tme <= 1.5)
		{
			pc = abs( (5.09 * tme) - 4.84 );// abs here due to double precision prob
		}
		else if (tme <= 1.62)
		{
			pc = (96.67 * tme) - 142.2;
		}
		else if (tme <= 2.14)
		{
			pc = (22.5 * tme) - 22.05;
		}
		else if (tme <= 2.46)
		{
			pc = 26.1;
		}
		else if (tme <= 3.8)
		{
			pc = (49.78 * tme) - 96.35;
		}
		else if (tme <= 5)
		{
			pc = (6 * tme) + 70;
		}
		else
		{
			pc = 100;
		}
		return pc;
	}


	// CO
	double SSME::dcPC_CO( double tme )
	{
		double pc;

		if (tme <= 0.5)
		{
			pc = 100 - (122 * tme);
		}
		else
		{
			if (tme <= 1.8)
			{
				pc = 50.92 - (23.85 * tme);
			}
			else
			{
				if (tme <= 6)
				{
					pc = 11.43 - (1.9 * tme);
				}
				else
				{
					pc = 0;
				}
			}
		}
		return pc;
	}

	double SSME::AdjCOTime( double pc )
	{
		// pc > 100% -> < 0
		// pc = 100% -> = 0
		// pc < 100% -> > 0
		if (pc >= 39)
		{
			return (100 - pc) / 122;
		}
		else
		{
			if (pc >= 8)
			{
				return ((50.92 - pc) * 2.6) / 62;
			}
			else
			{
				return ((11.43 - pc) * 4.2) / 8;
			}
		}
	}
}