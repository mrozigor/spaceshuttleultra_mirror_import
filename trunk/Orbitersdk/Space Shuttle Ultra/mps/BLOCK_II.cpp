#include "BLOCK_II.h"
#include "MPSdefs.h"


namespace mps
{
	SSME_BLOCK_II::SSME_BLOCK_II( AtlantisSubsystemDirector* _director, const string& _ident, unsigned short nID, int controllertype, const string& sw ):SSME( _director, _ident, nID, controllertype, sw, SSME_BLOCK_II_SENSOR_P, SSME_BLOCK_II_SENSOR_T, SSME_BLOCK_II_SENSOR_S, SSME_BLOCK_II_SENSOR_F )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " SSME_BLOCK_II::SSME_BLOCK_II in || nID:%d|controllertype:%d|sw:%s", nID, controllertype, sw.c_str() );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

		// set specific SSME BLOCK II parameters
		MPL = SSME_BLOCK_II_MPL;
		FPL = SSME_BLOCK_II_FPL;
		FPL_THRUST = SSME_BLOCK_II_FPL_THRUST;
		RPL_PC_PRESS = SSME_BLOCK_II_RPL_PC_PRESS;
		ISP0 = SSME_BLOCK_II_ISP0;
		ISP1 = SSME_BLOCK_II_ISP1;

		posMFV = 0;
		posMOV = 0;

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " SSME_BLOCK_II::SSME_BLOCK_II out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	SSME_BLOCK_II::~SSME_BLOCK_II( void )
	{
		return;
	}

	void SSME_BLOCK_II::__OnSaveState( FILEHANDLE scn ) const
	{
		char cbuf[255];

		sprintf_s( cbuf, 255, "%lf %lf", posMFV, posMOV );
		oapiWriteScenario_string( scn, "SSME_BLOCK_II_pos", cbuf );

		return;
	}

	bool SSME_BLOCK_II::__OnParseLine( const char* line )
	{
		double read_f1 = 0;
		double read_f2 = 0;
#ifdef _MPSDEBUG
		char buffer[100];
#endif// _MPSDEBUG

		if (!_strnicmp( line, "SSME_BLOCK_II_pos", 17 ))
		{
			sscanf_s( line + 17, "%lf %lf", &read_f1, &read_f2 );
			posMFV = read_f1;
			posMOV = read_f2;
#ifdef _MPSDEBUG
			sprintf_s( buffer, 100, " SSME_BLOCK_II::__OnParseLine || SSME_BLOCK_II_pos:%lf|%lf", posMFV, posMOV );
			oapiWriteLog( buffer );
#endif// _MPSDEBUG
			return true;
		}
		return false;
	}


	void SSME_BLOCK_II::RUN1( double time, double dt )// pre-start
	{
		// TODO do purges and stuff....
		// TODO "MFV moves" AFTER propellant drop (it moves before in testing)
		if (ptrMFV->GetPos() != 0)// MFV moves -> RUN2
		{
			modeltime = time;
			modelmode = 2;
		}

		// set sensor readings

		// press
		SENSOR_P[SSME_BLOCK_II_FPB_PP] = 0;
		SENSOR_P[SSME_BLOCK_II_OPB_PP] = 0;
		SENSOR_P[SSME_BLOCK_II_LPOT_DP_A] = 0;
		SENSOR_P[SSME_BLOCK_II_LPOT_DP_B] = 0;
		SENSOR_P[SSME_BLOCK_II_LPFT_DP_A] = 0;
		SENSOR_P[SSME_BLOCK_II_LPFT_DP_B] = 0;
		SENSOR_P[SSME_BLOCK_II_HPOTP_DP_A] = 0;
		SENSOR_P[SSME_BLOCK_II_HPOTP_DP_B] = 0;
		SENSOR_P[SSME_BLOCK_II_HPFTP_DP_A] = 0;
		SENSOR_P[SSME_BLOCK_II_HPFTP_DP_B] = 0;
		SENSOR_P[SSME_BLOCK_II_HPOTPBP_DP_A] = 0;
		SENSOR_P[SSME_BLOCK_II_HPOTPBP_DP_B] = 0;
		SENSOR_P[SSME_BLOCK_II_FS_PP_A] = 0;
		SENSOR_P[SSME_BLOCK_II_FS_PP_B] = 0;
		SENSOR_P[SSME_BLOCK_II_MCC_P_A1] = 0;
		SENSOR_P[SSME_BLOCK_II_MCC_P_B1] = 0;
		SENSOR_P[SSME_BLOCK_II_MCC_P_A2] = 0;
		SENSOR_P[SSME_BLOCK_II_MCC_P_B2] = 0;
		SENSOR_P[SSME_BLOCK_II_HPOTP_IS_A] = 0;
		SENSOR_P[SSME_BLOCK_II_HPOTP_IS_B] = 0;

		// temp
		SENSOR_T[SSME_BLOCK_II_HPFT_DT_A2] = 512;
		SENSOR_T[SSME_BLOCK_II_HPFT_DT_B2] = 511;
		SENSOR_T[SSME_BLOCK_II_HPFT_DT_A3] = 510;
		SENSOR_T[SSME_BLOCK_II_HPFT_DT_B3] = 509;
		SENSOR_T[SSME_BLOCK_II_HPOT_DT_A2] = 508;
		SENSOR_T[SSME_BLOCK_II_HPOT_DT_B2] = 507;
		SENSOR_T[SSME_BLOCK_II_HPOT_DT_A3] = 506;
		SENSOR_T[SSME_BLOCK_II_HPOT_DT_B3] = 505;
		SENSOR_T[SSME_BLOCK_II_PBP_DT_A] = 504;
		SENSOR_T[SSME_BLOCK_II_PBP_DT_B] = 503;
		SENSOR_T[SSME_BLOCK_II_LPFP_DT_A] = 502;
		SENSOR_T[SSME_BLOCK_II_LPFP_DT_B] = 501;
		SENSOR_T[SSME_BLOCK_II_MOV_HT_A] = 500;
		SENSOR_T[SSME_BLOCK_II_MOV_HT_B] = 499;
		SENSOR_T[SSME_BLOCK_II_MFV_HT_A] = 498;
		SENSOR_T[SSME_BLOCK_II_MFV_HT_B] = 497;

		// speed
		SENSOR_S[SSME_BLOCK_II_LPFTP_SS_A] = 0;
		SENSOR_S[SSME_BLOCK_II_LPFTP_SS_B] = 0;
		SENSOR_S[SSME_BLOCK_II_HPFTP_SS_A] = 0;
		SENSOR_S[SSME_BLOCK_II_HPFTP_SS_B] = 0;

		// flow
		SENSOR_F[SSME_BLOCK_II_FF_A1] = 0;
		SENSOR_F[SSME_BLOCK_II_FF_B1] = 0;
		SENSOR_F[SSME_BLOCK_II_FF_A2] = 0;
		SENSOR_F[SSME_BLOCK_II_FF_B2] = 0;

		STS()->SetSSMEThrustLevel( ID, 0 );// no thrust allowed
		return;
	}

	void SSME_BLOCK_II::RUN2( double time, double dt )// ignition
	{
		// can go to RUN3 (NOM) or RUN4 (SD, if MOV and/or MFV starts closing)
		// TODO check vlv pos, and improve SD stuff...
		
		double pc = dcPC_ESC( time - modeltime );
		STS()->SetSSMEThrustLevel( ID, pc );

		// set sensor readings
		pc = PCfromPCTtoPSI( pc );
		SENSOR_P[SSME_BLOCK_II_MCC_P_A1] = PCfromPCTtoPSI( STS()->GetSSMEThrustLevel( ID ) );// HACK
		SENSOR_P[SSME_BLOCK_II_MCC_P_A2] = PCfromPCTtoPSI( STS()->GetSSMEThrustLevel( ID ) );// HACK
		SENSOR_P[SSME_BLOCK_II_MCC_P_B1] = pc;
		SENSOR_P[SSME_BLOCK_II_MCC_P_B2] = pc;

		// is the controller commanding shutdown?
		if ((ptrMFV->GetPos() < posMFV) || (ptrMOV->GetPos() < posMOV))// MFV/MOV closing?
		{
			// goto shudown
			modeltime = time - AdjCOTime( STS()->GetSSMEThrustLevel( ID ) );
			modelmode = 4;
			return;
		}
		// if not save pos for next time
		posMOV = ptrMOV->GetPos();
		posMFV = ptrMFV->GetPos();

		// are we done here?
		if ((time - modeltime) >= 5)// it's on -> RUN3
		{
			modeltime = time;
			modelmode = 3;
		}
		return;
	}

	void SSME_BLOCK_II::RUN3( double time, double dt )// mainstage
	{
		// MOV moves -> RUN4

		// is the controller commanding shutdown?
		if (ptrMOV->GetPos() < 1)// MOV closing?
		{
			modeltime = time - AdjCOTime( STS()->GetSSMEThrustLevel( ID ) );
			modelmode = 4;
			RUN4( time, dt );// go to shutdown
			return;
		}

		// set pc from OPOV pos
		double pc = 125 * (0.3679 + (0.126491 * sqrt( (ptrOPOV->GetPos() * 100) - 52.5646 )));
		STS()->SetSSMEThrustLevel( ID, pc );

		// set sensor readings
		pc = PCfromPCTtoPSI( pc );
		SENSOR_P[SSME_BLOCK_II_MCC_P_A1] = PCfromPCTtoPSI( STS()->GetSSMEThrustLevel( ID ) );// HACK
		SENSOR_P[SSME_BLOCK_II_MCC_P_A2] = PCfromPCTtoPSI( STS()->GetSSMEThrustLevel( ID ) );// HACK
		SENSOR_P[SSME_BLOCK_II_MCC_P_B1] = pc;
		SENSOR_P[SSME_BLOCK_II_MCC_P_B2] = pc;
		return;
	}

	void SSME_BLOCK_II::RUN4( double time, double dt )// shutdown
	{
		// MFV ?or? CCV = 0 -> RUN 5

		double pc = dcPC_CO( time - modeltime );
		STS()->SetSSMEThrustLevel( ID, pc );

		// set sensor readings
		pc = PCfromPCTtoPSI( pc );
		SENSOR_P[SSME_BLOCK_II_MCC_P_A1] = pc;
		SENSOR_P[SSME_BLOCK_II_MCC_P_A2] = pc;
		SENSOR_P[SSME_BLOCK_II_MCC_P_B1] = pc;
		SENSOR_P[SSME_BLOCK_II_MCC_P_B2] = pc;

		// are we done here?
		if ((time - modeltime) >= 6)// it's off -> RUN5
		{
			modeltime = time;
			modelmode = 5;

			// LOX dump
			phLOXdump = STS()->CreatePropellantResource( 700 );
			thLOXdump = STS()->CreateThruster( posLOXdump, dirLOXdump, 4000, phLOXdump, 120, 120 );
			PARTICLESTREAMSPEC psLOXdump = {
				0,
				2,
				0.06,
				20,
				0.5,
				0.3,
				25,
				5,
				PARTICLESTREAMSPEC::DIFFUSE,
				PARTICLESTREAMSPEC::LVL_PLIN,
				0, 1,
				PARTICLESTREAMSPEC::ATM_FLAT,
				1, 1,
				0
			};
			
			STS()->AddExhaustStream( thLOXdump, &psLOXdump );
		}
		return;
	}

	void SSME_BLOCK_II::RUN5( double time, double dt )// post-shutdown
	{
		STS()->SetSSMEThrustLevel( ID, 0 );// no thrust allowed

		// LOX dump
		// 5400 lbs total prop -> ~2100 Kg LOX (700Kg per engine)
		// dv -> 9-11 fps
		if (thLOXdump != NULL) STS()->SetThrusterLevel( thLOXdump, ptrMOV->GetPos() * STS()->GetPropellantLevel( phLOXdump ) / 100 );
		return;
	}
}