#include "BLOCK_II.h"
#include "PneumaticControlAssembly.h"
#include "MPS.h"
#include "MPSdefs.h"


namespace mps
{
	SSME_BLOCK_II::SSME_BLOCK_II( AtlantisSubsystemDirector* _director, const string& _ident, unsigned short nID, int controllertype, const string& sw, HeSysEng* HeSys ):SSME( _director, _ident, nID, controllertype, sw, HeSys )
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

		IMSLHeUsage = 20.8 + (0.5 * oapiRand());// IMSL max 21.31884

		Sensor_Press[0] = Sensor( 5000, 0 );
		Sensor_Press[1] = Sensor( 5000, 0 );
		Sensor_Press[2] = Sensor( 5000, 0 );
		Sensor_Press[3] = Sensor( 5000, 0 );
		Sensor_Press[4] = Sensor( 5000, 0 );
		Sensor_Press[5] = Sensor( 5000, 0 );
		Sensor_Press[6] = Sensor( 5000, 0 );
		Sensor_Press[7] = Sensor( 5000, 0 );
		Sensor_Press[8] = Sensor( 5000, 0 );
		Sensor_Press[9] = Sensor( 5000, 0 );
		Sensor_Press[10] = Sensor( 5000, 0 );
		Sensor_Press[11] = Sensor( 5000, 0 );
		Sensor_Press[12] = Sensor( 5000, 0 );
		Sensor_Press[13] = Sensor( 5000, 0 );
		Sensor_Press[14] = Sensor( 5000, 0 );
		Sensor_Press[15] = Sensor( 5000, 0 );
		Sensor_Press[16] = Sensor( 5000, 0 );
		Sensor_Press[17] = Sensor( 5000, 0 );
		Sensor_Press[18] = Sensor( 5000, 0 );
		Sensor_Press[19] = Sensor( 5000, 0 );
		Sensor_Temp[0] = Sensor( 2450, 0 );
		Sensor_Temp[1] = Sensor( 2450, 0 );
		Sensor_Temp[2] = Sensor( 2450, 0 );
		Sensor_Temp[3] = Sensor( 2450, 0 );
		Sensor_Temp[4] = Sensor( 2450, 0 );
		Sensor_Temp[5] = Sensor( 2450, 0 );
		Sensor_Temp[6] = Sensor( 2450, 0 );
		Sensor_Temp[7] = Sensor( 2450, 0 );
		Sensor_Temp[8] = Sensor( 2450, 0 );
		Sensor_Temp[9] = Sensor( 2450, 0 );
		Sensor_Temp[10] = Sensor( 2450, 0 );
		Sensor_Temp[11] = Sensor( 2450, 0 );
		Sensor_Temp[12] = Sensor( 2450, 0 );
		Sensor_Temp[13] = Sensor( 2450, 0 );
		Sensor_Temp[14] = Sensor( 2450, 0 );
		Sensor_Temp[15] = Sensor( 2450, 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_A1] = Sensor( 5000, 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_B1] = Sensor( 5000, 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_A2] = Sensor( 5000, 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_B2] = Sensor( 5000, 0 );
		Sensor_Speed[SSME_BLOCK_II_LPFTP_SS_A] = Sensor( 5000, 0 );
		Sensor_Speed[SSME_BLOCK_II_LPFTP_SS_B] = Sensor( 5000, 0 );
		Sensor_Speed[SSME_BLOCK_II_HPFTP_SS_A] = Sensor( 5000, 0 );
		Sensor_Speed[SSME_BLOCK_II_HPFTP_SS_B] = Sensor( 5000, 0 );

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
		return;
	}

	bool SSME_BLOCK_II::__OnParseLine( const char* line )
	{
		return false;
	}

	void SSME_BLOCK_II::ConnectSensors( DiscreteBundle* IEchA_Press, DiscreteBundle* IEchB_Press, DiscreteBundle* IEchA_Temp, DiscreteBundle* IEchB_Temp, DiscreteBundle* IEchA_Flow, DiscreteBundle* IEchB_Flow, DiscreteBundle* IEchA_Speed, DiscreteBundle* IEchB_Speed )
	{
		Sensor_Press[SSME_BLOCK_II_FPB_PP].Connect( IEchA_Press, 0 );// HACK (a big one), just on chA for now
		Sensor_Press[SSME_BLOCK_II_OPB_PP].Connect( IEchA_Press, 1 );// HACK (a big one), just on chA for now
		Sensor_Press[SSME_BLOCK_II_LPOT_DP_A].Connect( IEchA_Press, 2 );
		Sensor_Press[SSME_BLOCK_II_LPOT_DP_B].Connect( IEchB_Press, 2 );
		Sensor_Press[SSME_BLOCK_II_LPFT_DP_A].Connect( IEchA_Press, 3 );
		Sensor_Press[SSME_BLOCK_II_LPFT_DP_B].Connect( IEchB_Press, 3 );
		Sensor_Press[SSME_BLOCK_II_HPOTP_DP_A].Connect( IEchA_Press, 4 );
		Sensor_Press[SSME_BLOCK_II_HPOTP_DP_B].Connect( IEchB_Press, 4 );
		Sensor_Press[SSME_BLOCK_II_HPFTP_DP_A].Connect( IEchA_Press, 5 );
		Sensor_Press[SSME_BLOCK_II_HPFTP_DP_B].Connect( IEchB_Press, 5 );
		Sensor_Press[SSME_BLOCK_II_HPOTPBP_DP_A].Connect( IEchA_Press, 6 );
		Sensor_Press[SSME_BLOCK_II_HPOTPBP_DP_B].Connect( IEchB_Press, 6 );
		Sensor_Press[SSME_BLOCK_II_FS_PP_A].Connect( IEchA_Press, 7 );
		Sensor_Press[SSME_BLOCK_II_FS_PP_B].Connect( IEchB_Press, 7 );
		Sensor_Press[SSME_BLOCK_II_MCC_P_A1].Connect( IEchA_Press, 8 );
		Sensor_Press[SSME_BLOCK_II_MCC_P_B1].Connect( IEchB_Press, 8 );
		Sensor_Press[SSME_BLOCK_II_MCC_P_A2].Connect( IEchA_Press, 9 );
		Sensor_Press[SSME_BLOCK_II_MCC_P_B2].Connect( IEchB_Press, 9 );
		Sensor_Press[SSME_BLOCK_II_HPOTP_IS_A].Connect( IEchA_Press, 10 );
		Sensor_Press[SSME_BLOCK_II_HPOTP_IS_B].Connect( IEchB_Press, 10 );

		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_A2].Connect( IEchA_Temp, 0 );
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_B2].Connect( IEchB_Temp, 0 );
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_A3].Connect( IEchA_Temp, 1 );
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_B3].Connect( IEchB_Temp, 1 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_A2].Connect( IEchA_Temp, 2 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_B2].Connect( IEchB_Temp, 2 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_A3].Connect( IEchA_Temp, 3 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_B3].Connect( IEchB_Temp, 3 );
		Sensor_Temp[SSME_BLOCK_II_PBP_DT_A].Connect( IEchA_Temp, 4 );
		Sensor_Temp[SSME_BLOCK_II_PBP_DT_B].Connect( IEchB_Temp, 4 );
		Sensor_Temp[SSME_BLOCK_II_LPFP_DT_A].Connect( IEchA_Temp, 5 );
		Sensor_Temp[SSME_BLOCK_II_LPFP_DT_B].Connect( IEchB_Temp, 5 );
		Sensor_Temp[SSME_BLOCK_II_MOV_HT_A].Connect( IEchA_Temp, 6 );
		Sensor_Temp[SSME_BLOCK_II_MOV_HT_B].Connect( IEchB_Temp, 6 );
		Sensor_Temp[SSME_BLOCK_II_MFV_HT_A].Connect( IEchA_Temp, 7 );
		Sensor_Temp[SSME_BLOCK_II_MFV_HT_B].Connect( IEchB_Temp, 7 );

		Sensor_Flow[SSME_BLOCK_II_FF_A1].Connect( IEchA_Flow, 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_B1].Connect( IEchB_Flow, 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_A2].Connect( IEchA_Flow, 1 );
		Sensor_Flow[SSME_BLOCK_II_FF_B2].Connect( IEchB_Flow, 1 );

		Sensor_Speed[SSME_BLOCK_II_LPFTP_SS_A].Connect( IEchA_Speed, 0 );
		Sensor_Speed[SSME_BLOCK_II_LPFTP_SS_B].Connect( IEchB_Speed, 0 );
		Sensor_Speed[SSME_BLOCK_II_HPFTP_SS_A].Connect( IEchA_Speed, 1 );
		Sensor_Speed[SSME_BLOCK_II_HPFTP_SS_B].Connect( IEchB_Speed, 1 );
		return;
	}

	void SSME_BLOCK_II::RUN1( double time, double dt )// pre-start
	{
		// TODO do purges and stuff....
		// TODO "MFV moves" AFTER propellant drop (it moves before in testing)
		double temp = 0;
		if ((ptrMFV->GetPos() != 0) &&// MFV moves -> RUN2
			((Igniter_FPB[0] == true) || (Igniter_FPB[1] == true)) && // at least 1 of the 2 igniters in each chamber must be on
			((Igniter_OPB[0] == true) || (Igniter_OPB[1] == true)) && 
			((Igniter_MCC[0] == true) || (Igniter_MCC[1] == true)))
		{
			modeltime = time;
			modelmode = 2;
		}
		
		// set sensor readings
		// press
		temp = PCA->GetPressFuelPreburnerPurge();
		Sensor_Press[SSME_BLOCK_II_FPB_PP].SetValue( temp );
		temp = PCA->GetPressOxidizerPreburnerPurge();
		Sensor_Press[SSME_BLOCK_II_OPB_PP].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_LPOT_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_LPOT_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_LPFT_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_LPFT_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPOTP_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPOTP_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPFTP_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPFTP_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPOTPBP_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPOTPBP_DP_B].SetValue( 0 );
		temp = PCA->UseFuelSystemPurge( 220 );
		Sensor_Press[SSME_BLOCK_II_FS_PP_A].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_FS_PP_B].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_MCC_P_A1].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_MCC_P_B1].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_MCC_P_A2].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_MCC_P_B2].SetValue( 0 );
		temp = PCA->UseHPOTPISPurge( IMSLHeUsage );
		Sensor_Press[SSME_BLOCK_II_HPOTP_IS_A].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_HPOTP_IS_B].SetValue( temp );

		// temp
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_A2].SetValue( 512 );
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_A3].SetValue( 510 );
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_B2].SetValue( 511 );
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_B3].SetValue( 509 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_A2].SetValue( 508 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_A3].SetValue( 506 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_B2].SetValue( 507 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_B3].SetValue( 505 );
		Sensor_Temp[SSME_BLOCK_II_PBP_DT_A].SetValue( 504 );
		Sensor_Temp[SSME_BLOCK_II_PBP_DT_B].SetValue( 503 );
		Sensor_Temp[SSME_BLOCK_II_LPFP_DT_A].SetValue( 502 );
		Sensor_Temp[SSME_BLOCK_II_LPFP_DT_B].SetValue( 501 );
		Sensor_Temp[SSME_BLOCK_II_MOV_HT_A].SetValue( 500 );
		Sensor_Temp[SSME_BLOCK_II_MOV_HT_B].SetValue( 499 );
		Sensor_Temp[SSME_BLOCK_II_MFV_HT_A].SetValue( 498 );
		Sensor_Temp[SSME_BLOCK_II_MFV_HT_B].SetValue( 497 );

		// flow
		Sensor_Flow[SSME_BLOCK_II_FF_A1].SetValue( 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_A2].SetValue( 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_B1].SetValue( 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_B2].SetValue( 0 );

		// speed
		Sensor_Speed[SSME_BLOCK_II_LPFTP_SS_A].SetValue( 0 );
		Sensor_Speed[SSME_BLOCK_II_LPFTP_SS_B].SetValue( 0 );
		Sensor_Speed[SSME_BLOCK_II_HPFTP_SS_A].SetValue( 0 );
		Sensor_Speed[SSME_BLOCK_II_HPFTP_SS_B].SetValue( 0 );

		STS()->SetSSMEThrustLevel( ID, 0 );// no thrust allowed
		return;
	}

	void SSME_BLOCK_II::RUN2( double time, double dt )// ignition
	{
		// can go to RUN3 (NOM) or RUN4 (SD, if MOV and/or MFV starts closing)
		// TODO check vlv pos, and improve SD stuff...
		double temp = 0;
		double pc = dcPC_ESC( time - modeltime );
		STS()->SetSSMEThrustLevel( ID, pc );

		// set sensor readings
		// press
		temp = PCA->GetPressFuelPreburnerPurge();
		Sensor_Press[SSME_BLOCK_II_FPB_PP].SetValue( temp );
		temp = PCA->GetPressOxidizerPreburnerPurge();
		Sensor_Press[SSME_BLOCK_II_OPB_PP].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_LPOT_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_LPOT_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_LPFT_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_LPFT_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPOTP_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPOTP_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPFTP_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPFTP_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPOTPBP_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPOTPBP_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_FS_PP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_FS_PP_B].SetValue( 0 );
		temp = /*PCfromPCTtoPSI( pc );*/PCfromPCTtoPSI( STS()->GetSSMEThrustLevel( ID ) );// HACK
		Sensor_Press[SSME_BLOCK_II_MCC_P_A1].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_MCC_P_A2].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_MCC_P_B1].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_MCC_P_B2].SetValue( temp );
		temp = PCA->UseHPOTPISPurge( IMSLHeUsage );
		Sensor_Press[SSME_BLOCK_II_HPOTP_IS_A].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_HPOTP_IS_B].SetValue( temp );

		// temp
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_A2].SetValue( 1000 );
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_A3].SetValue( 1000 );
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_B2].SetValue( 1000 );
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_B3].SetValue( 1000 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_A2].SetValue( 1000 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_A3].SetValue( 1000 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_B2].SetValue( 1000 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_B3].SetValue( 1000 );
		Sensor_Temp[SSME_BLOCK_II_PBP_DT_A].SetValue( 504 );
		Sensor_Temp[SSME_BLOCK_II_PBP_DT_B].SetValue( 503 );
		Sensor_Temp[SSME_BLOCK_II_LPFP_DT_A].SetValue( 502 );
		Sensor_Temp[SSME_BLOCK_II_LPFP_DT_B].SetValue( 501 );
		Sensor_Temp[SSME_BLOCK_II_MOV_HT_A].SetValue( 500 );
		Sensor_Temp[SSME_BLOCK_II_MOV_HT_B].SetValue( 499 );
		Sensor_Temp[SSME_BLOCK_II_MFV_HT_A].SetValue( 498 );
		Sensor_Temp[SSME_BLOCK_II_MFV_HT_B].SetValue( 497 );

		// flow
		Sensor_Flow[SSME_BLOCK_II_FF_A1].SetValue( 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_A2].SetValue( 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_B1].SetValue( 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_B2].SetValue( 0 );

		// speed
		Sensor_Speed[SSME_BLOCK_II_LPFTP_SS_A].SetValue( 0 );
		Sensor_Speed[SSME_BLOCK_II_LPFTP_SS_B].SetValue( 0 );
		Sensor_Speed[SSME_BLOCK_II_HPFTP_SS_A].SetValue( 0 );
		Sensor_Speed[SSME_BLOCK_II_HPFTP_SS_B].SetValue( 0 );

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
		double temp = 0;
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
		// press
		temp = PCA->GetPressFuelPreburnerPurge();
		Sensor_Press[SSME_BLOCK_II_FPB_PP].SetValue( temp );
		temp = PCA->GetPressOxidizerPreburnerPurge();
		Sensor_Press[SSME_BLOCK_II_OPB_PP].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_LPOT_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_LPOT_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_LPFT_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_LPFT_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPOTP_DP_A].SetValue( 3804 );// Block IIA
		Sensor_Press[SSME_BLOCK_II_HPOTP_DP_B].SetValue( 3804 );// Block IIA
		Sensor_Press[SSME_BLOCK_II_HPFTP_DP_A].SetValue( 5666 );// Block IIA
		Sensor_Press[SSME_BLOCK_II_HPFTP_DP_B].SetValue( 5666 );// Block IIA
		Sensor_Press[SSME_BLOCK_II_HPOTPBP_DP_A].SetValue( 6910 );// Block IIA
		Sensor_Press[SSME_BLOCK_II_HPOTPBP_DP_B].SetValue( 6910 );// Block IIA
		Sensor_Press[SSME_BLOCK_II_FS_PP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_FS_PP_B].SetValue( 0 );
		temp = /*PCfromPCTtoPSI( pc );*/PCfromPCTtoPSI( STS()->GetSSMEThrustLevel( ID ) );// HACK
		Sensor_Press[SSME_BLOCK_II_MCC_P_A1].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_MCC_P_A2].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_MCC_P_B1].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_MCC_P_B2].SetValue( temp );
		temp = PCA->UseHPOTPISPurge( IMSLHeUsage );
		Sensor_Press[SSME_BLOCK_II_HPOTP_IS_A].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_HPOTP_IS_B].SetValue( temp );

		// temp
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_A2].SetValue( 1597 );// Block IIA
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_A3].SetValue( 1597 );// Block IIA
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_B2].SetValue( 1604 );// Block IIA
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_B3].SetValue( 1604 );// Block IIA
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_A2].SetValue( 1214 );// Block IIA
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_A3].SetValue( 1214 );// Block IIA
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_B2].SetValue( 1228 );// Block IIA
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_B3].SetValue( 1228 );// Block IIA
		Sensor_Temp[SSME_BLOCK_II_PBP_DT_A].SetValue( 504 );
		Sensor_Temp[SSME_BLOCK_II_PBP_DT_B].SetValue( 503 );
		Sensor_Temp[SSME_BLOCK_II_LPFP_DT_A].SetValue( 502 );
		Sensor_Temp[SSME_BLOCK_II_LPFP_DT_B].SetValue( 501 );
		Sensor_Temp[SSME_BLOCK_II_MOV_HT_A].SetValue( 500 );
		Sensor_Temp[SSME_BLOCK_II_MOV_HT_B].SetValue( 499 );
		Sensor_Temp[SSME_BLOCK_II_MFV_HT_A].SetValue( 498 );
		Sensor_Temp[SSME_BLOCK_II_MFV_HT_B].SetValue( 497 );

		// flow
		Sensor_Flow[SSME_BLOCK_II_FF_A1].SetValue( 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_A2].SetValue( 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_B1].SetValue( 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_B2].SetValue( 0 );

		// speed
		Sensor_Speed[SSME_BLOCK_II_LPFTP_SS_A].SetValue( 15680 );// Block IIA
		Sensor_Speed[SSME_BLOCK_II_LPFTP_SS_B].SetValue( 15680 );// Block IIA
		Sensor_Speed[SSME_BLOCK_II_HPFTP_SS_A].SetValue( 34293 );// Block IIA
		Sensor_Speed[SSME_BLOCK_II_HPFTP_SS_B].SetValue( 34293 );// Block IIA
		return;
	}

	void SSME_BLOCK_II::RUN4( double time, double dt )// shutdown
	{
		// MFV ?or? CCV = 0 -> RUN 5
		double temp = 0;
		double pc = dcPC_CO( time - modeltime );
		STS()->SetSSMEThrustLevel( ID, pc );

		// set sensor readings
		// press
		temp = PCA->GetPressFuelPreburnerPurge();
		Sensor_Press[SSME_BLOCK_II_FPB_PP].SetValue( temp );
		temp = PCA->GetPressOxidizerPreburnerPurge();
		Sensor_Press[SSME_BLOCK_II_OPB_PP].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_LPOT_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_LPOT_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_LPFT_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_LPFT_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPOTP_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPOTP_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPFTP_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPFTP_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPOTPBP_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPOTPBP_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_FS_PP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_FS_PP_B].SetValue( 0 );
		temp = /*PCfromPCTtoPSI( pc );*/PCfromPCTtoPSI( STS()->GetSSMEThrustLevel( ID ) );// HACK
		Sensor_Press[SSME_BLOCK_II_MCC_P_A1].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_MCC_P_A2].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_MCC_P_B1].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_MCC_P_B2].SetValue( temp );
		temp = PCA->UseHPOTPISPurge( IMSLHeUsage );
		Sensor_Press[SSME_BLOCK_II_HPOTP_IS_A].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_HPOTP_IS_B].SetValue( temp );

		// temp
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_A2].SetValue( 512 );
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_A3].SetValue( 510 );
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_B2].SetValue( 511 );
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_B3].SetValue( 509 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_A2].SetValue( 508 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_A3].SetValue( 506 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_B2].SetValue( 507 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_B3].SetValue( 505 );
		Sensor_Temp[SSME_BLOCK_II_PBP_DT_A].SetValue( 504 );
		Sensor_Temp[SSME_BLOCK_II_PBP_DT_B].SetValue( 503 );
		Sensor_Temp[SSME_BLOCK_II_LPFP_DT_A].SetValue( 502 );
		Sensor_Temp[SSME_BLOCK_II_LPFP_DT_B].SetValue( 501 );
		Sensor_Temp[SSME_BLOCK_II_MOV_HT_A].SetValue( 500 );
		Sensor_Temp[SSME_BLOCK_II_MOV_HT_B].SetValue( 499 );
		Sensor_Temp[SSME_BLOCK_II_MFV_HT_A].SetValue( 498 );
		Sensor_Temp[SSME_BLOCK_II_MFV_HT_B].SetValue( 497 );

		// flow
		Sensor_Flow[SSME_BLOCK_II_FF_A1].SetValue( 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_A2].SetValue( 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_B1].SetValue( 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_B2].SetValue( 0 );

		// speed
		Sensor_Speed[SSME_BLOCK_II_LPFTP_SS_A].SetValue( 0 );
		Sensor_Speed[SSME_BLOCK_II_LPFTP_SS_B].SetValue( 0 );
		Sensor_Speed[SSME_BLOCK_II_HPFTP_SS_A].SetValue( 0 );
		Sensor_Speed[SSME_BLOCK_II_HPFTP_SS_B].SetValue( 0 );

		// are we done here?
		if ((time - modeltime) >= 6)// it's off -> RUN5
		{
			modeltime = time;
			modelmode = 5;
		}
		return;
	}

	void SSME_BLOCK_II::RUN5( double time, double dt )// post-shutdown
	{
		double temp = 0;

		STS()->SetSSMEThrustLevel( ID, 0 );// no thrust allowed

		// set sensor readings
		// press
		temp = PCA->GetPressFuelPreburnerPurge();
		Sensor_Press[SSME_BLOCK_II_FPB_PP].SetValue( temp );
		temp = PCA->GetPressOxidizerPreburnerPurge();
		Sensor_Press[SSME_BLOCK_II_OPB_PP].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_LPOT_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_LPOT_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_LPFT_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_LPFT_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPOTP_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPOTP_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPFTP_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPFTP_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPOTPBP_DP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_HPOTPBP_DP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_FS_PP_A].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_FS_PP_B].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_MCC_P_A1].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_MCC_P_A2].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_MCC_P_B1].SetValue( 0 );
		Sensor_Press[SSME_BLOCK_II_MCC_P_B2].SetValue( 0 );
		temp = PCA->UseHPOTPISPurge( IMSLHeUsage );
		Sensor_Press[SSME_BLOCK_II_HPOTP_IS_A].SetValue( temp );
		Sensor_Press[SSME_BLOCK_II_HPOTP_IS_B].SetValue( temp );

		// temp
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_A2].SetValue( 512 );
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_A3].SetValue( 510 );
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_B2].SetValue( 511 );
		Sensor_Temp[SSME_BLOCK_II_HPFT_DT_B3].SetValue( 509 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_A2].SetValue( 508 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_A3].SetValue( 506 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_B2].SetValue( 507 );
		Sensor_Temp[SSME_BLOCK_II_HPOT_DT_B3].SetValue( 505 );
		Sensor_Temp[SSME_BLOCK_II_PBP_DT_A].SetValue( 504 );
		Sensor_Temp[SSME_BLOCK_II_PBP_DT_B].SetValue( 503 );
		Sensor_Temp[SSME_BLOCK_II_LPFP_DT_A].SetValue( 502 );
		Sensor_Temp[SSME_BLOCK_II_LPFP_DT_B].SetValue( 501 );
		Sensor_Temp[SSME_BLOCK_II_MOV_HT_A].SetValue( 500 );
		Sensor_Temp[SSME_BLOCK_II_MOV_HT_B].SetValue( 499 );
		Sensor_Temp[SSME_BLOCK_II_MFV_HT_A].SetValue( 498 );
		Sensor_Temp[SSME_BLOCK_II_MFV_HT_B].SetValue( 497 );

		// flow
		Sensor_Flow[SSME_BLOCK_II_FF_A1].SetValue( 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_A2].SetValue( 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_B1].SetValue( 0 );
		Sensor_Flow[SSME_BLOCK_II_FF_B2].SetValue( 0 );

		// speed
		Sensor_Speed[SSME_BLOCK_II_LPFTP_SS_A].SetValue( 0 );
		Sensor_Speed[SSME_BLOCK_II_LPFTP_SS_B].SetValue( 0 );
		Sensor_Speed[SSME_BLOCK_II_HPFTP_SS_A].SetValue( 0 );
		Sensor_Speed[SSME_BLOCK_II_HPFTP_SS_B].SetValue( 0 );
		
		STS()->SetMPSDumpLevel( ID - 1, ptrMOV->GetPos() *  pMPS->GetLOXPVPos( ID ) * min( (pMPS->GetLOXManifPress() / 80) * (pMPS->GetLOXManifPress() / 80), 1 ) );
		return;
	}
}