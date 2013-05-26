#include "SSME.h"
#include "SSMEController_BLOCK_II.h"
#include "MPSdefs.h"


namespace mps
{
	SSME::SSME( AtlantisSubsystemDirector* _director, const string& _ident, unsigned short ID, int controllertype, const string& sw, int numP, int numT, int numS, int numF ):AtlantisSubsystem( _director, _ident )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " SSME::SSME in || name:%s|ID:%d|controllertype:%d|sw:%s|numP:%d|numT:%d|numS:%d|numF:%d", _ident.c_str(), ID, controllertype, sw.c_str(), numP, numT, numS, numF );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

		this->ID = ID;
		this->numP = numP;
		this->numT = numT;
		this->numS = numS;
		this->numF = numF;

		// create sensor arrays with correct size for the engine model
		SENSOR_P = new double[numP];
		SENSOR_T = new double[numT];
		SENSOR_S = new double[numS];
		SENSOR_F = new double[numF];

		// make valves
		ptrCCV = new BasicValve( 0, MAX_RATE_CCV );
		ptrMOV = new BasicValve( 0, MAX_RATE_MOV );
		ptrMFV = new BasicValve( 0, MAX_RATE_MFV );
		ptrFPOV = new BasicValve( 0, MAX_RATE_FPOV );
		ptrOPOV = new BasicValve( 0, MAX_RATE_OPOV );
		ptrOBV = new ValveTypeBool( 0, MAX_RATE_OBV );
		ptrFBV = new ValveTypeBool( 0, MAX_RATE_FBV );
		ptrAFV = new ValveTypeBool( 0, MAX_RATE_AFV );

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
		// TODO move to realize???
		/*if (controllertype == 1)// TODO improve/add error checking
		{
			Controller = new SSMEController_BLOCK_I( this, sw );
		}
		else
		{*/
			Controller = new SSMEController_BLOCK_II( this, sw );
		//}
		
		// LOX dump stuff
		thLOXdump = NULL;
		phLOXdump = NULL;
		switch (ID)
		{
			case 1:
				posLOXdump = _V(0.0, 3.387,-14.8485); // position relative to center of orbiter mesh
				dirLOXdump = _V( 0.0, -0.37489, 0.92707 );
				break;
			case 2:
				posLOXdump = _V(-1.458, 0.548, -15.8735); // position relative to center of orbiter mesh
				dirLOXdump = _V( 0.065, -0.2447, 0.9674 );
				break;
			case 3:
				posLOXdump = _V(1.458, 0.548, -15.8735); // position relative to center of orbiter mesh
				dirLOXdump = _V( -0.065, -0.2447, 0.9674 );
				break;
		}

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
		delete ptrOBV;
		delete ptrFBV;
		delete ptrAFV;
		delete Controller;

		delete[] SENSOR_P;
		delete[] SENSOR_T;
		delete[] SENSOR_S;
		delete[] SENSOR_F;

		if (thLOXdump != NULL)
		{
			STS()->DelThruster( thLOXdump );
			STS()->DelPropellantResource( phLOXdump );
		}
		return;
	}

	void SSME::Realize()
	{
		STS()->SetSSMEParams( ID, FPL_THRUST, ISP0, ISP1 );

		Controller->Realize();
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

	/*void SSME::OnPreStep( double fSimT, double fDeltaT, double fMJD )
	{
		return;
	}*/

	void SSME::OnPostStep( double fSimT, double fDeltaT, double fMJD )
	{
		// valves first
		ptrCCV->tmestp( fDeltaT );
		ptrMOV->tmestp( fDeltaT );
		ptrMFV->tmestp( fDeltaT );
		ptrFPOV->tmestp( fDeltaT );
		ptrOPOV->tmestp( fDeltaT );
		ptrOBV->tmestp( fDeltaT );
		ptrFBV->tmestp( fDeltaT );

		// engine model
		SSMERUN( fSimT, fDeltaT );

		// run controller
		Controller->tmestp( fSimT, fDeltaT );
		return;
	}

	void SSME::OnSaveState( FILEHANDLE scn ) const
	{
		char cbuf[255];

		sprintf_s( cbuf, 255, "%d", modelmode );
		oapiWriteScenario_string( scn, "SSME_model", cbuf );

		sprintf_s( cbuf, 255, "%d %d %d %d %d %d", Igniter_MCC[0], Igniter_MCC[1], Igniter_FPB[0], Igniter_FPB[1], Igniter_OPB[0], Igniter_OPB[1] );
		oapiWriteScenario_string( scn, "SSME_Igniters", cbuf );

		sprintf_s( cbuf, 255, "%lf %lf %lf %lf %lf %d %d %d", ptrCCV->GetPos(), ptrMFV->GetPos(), ptrMOV->GetPos(), ptrFPOV->GetPos(), ptrOPOV->GetPos(), ptrFBV->GetPos(), ptrOBV->GetPos(), ptrAFV->GetPos() );
		oapiWriteScenario_string( scn, "SSME_Valves", cbuf );

		__OnSaveState( scn );// write derived class
		Controller->__OnSaveState( scn );// write controller
		return;
	}

	bool SSME::OnParseLine( const char* line )
	{
		int read_i1 = 0;
		int read_i2 = 0;
		int read_i3 = 0;
		int read_i4 = 0;
		int read_i5 = 0;
		int read_i6 = 0;
		double read_f1 = 0;
		double read_f2 = 0;
		double read_f3 = 0;
		double read_f4 = 0;
		double read_f5 = 0;
#ifdef _MPSDEBUG
		char buffer[100];
#endif// _MPSDEBUG

		if (!_strnicmp( line, "SSME_model", 10 ))
		{
			sscanf_s( line + 10, "%d", &read_i1 );
			modelmode = read_i1;
#ifdef _MPSDEBUG
			sprintf_s( buffer, 100, " SSME::OnParseLine || SSME_model:%d", modelmode );
			oapiWriteLog( buffer );
#endif// _MPSDEBUG
			return true;
		}
		else if (!_strnicmp( line, "SSME_Igniters", 13 ))
		{
			sscanf_s( line + 13, "%d %d %d %d %d %d", &read_i1, &read_i2, &read_i3, &read_i4, &read_i5, &read_i6 );
			Igniter_MCC[0] = GetBoolFromInt( read_i1 );
			Igniter_MCC[1] = GetBoolFromInt( read_i2 );
			Igniter_FPB[0] = GetBoolFromInt( read_i3 );
			Igniter_FPB[1] = GetBoolFromInt( read_i4 );
			Igniter_OPB[0] = GetBoolFromInt( read_i5 );
			Igniter_OPB[1] = GetBoolFromInt( read_i6 );
#ifdef _MPSDEBUG
			sprintf_s( buffer, 100, " SSME::OnParseLine || SSME_Igniters:%d|%d|%d|%d|%d|%d", Igniter_MCC[0], Igniter_MCC[1], Igniter_FPB[0], Igniter_FPB[1], Igniter_OPB[0], Igniter_OPB[1] );
			oapiWriteLog( buffer );
#endif// _MPSDEBUG
			return true;
		}
		else if (!_strnicmp( line, "SSME_Valves", 11 ))
		{
			sscanf_s( line + 11, "%lf %lf %lf %lf %lf %d %d %d", &read_f1, &read_f2, &read_f3, &read_f4, &read_f5, &read_i1, &read_i2, &read_i3 );
			ptrCCV->_backdoor( read_f1 );
			ptrMFV->_backdoor( read_f2 );
			ptrMOV->_backdoor( read_f3 );
			ptrFPOV->_backdoor( read_f4 );
			ptrOPOV->_backdoor( read_f5 );
			ptrFBV->_backdoor( GetBoolFromInt( read_i1 ) );
			ptrOBV->_backdoor( GetBoolFromInt( read_i2 ) );
			ptrAFV->_backdoor( GetBoolFromInt( read_i3 ) );
#ifdef _MPSDEBUG
			sprintf_s( buffer, 100, " SSME::OnParseLine || SSME_Valves:%lf|%lf|%lf|%lf|%lf|%d|%d|%d", ptrCCV->GetPos(), ptrMFV->GetPos(), ptrMOV->GetPos(), ptrFPOV->GetPos(), ptrOPOV->GetPos(), ptrFBV->GetPos(), ptrOBV->GetPos(), ptrAFV->GetPos() );
			oapiWriteLog( buffer );
#endif// _MPSDEBUG
			return true;
		}

		if (__OnParseLine( line )) return true;// check if derived class wants line
		if (Controller->__OnParseLine( line )) return true;// check if controller wants line
		return false;
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

	void SSME::PCA( void )
	{// HACK for pneumatic shutdown for now, to develop into full PCA class
		// HACK vlvs don't close at full rate
		ptrCCV->Close();
		ptrMFV->Close();
		ptrMOV->Close();
		ptrFPOV->Close();
		ptrOPOV->Close();
		return;
	}

	// data cookup
	// ESC
	double SSME::dcPC_ESC( double tme )
	{
		double pc;

		if (tme <= 0.95)
		{
			pc = 0;
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