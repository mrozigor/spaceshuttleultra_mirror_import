#include "BLOCK_II.h"
#include "..\Atlantis.h"


namespace mps
{
	BLOCK_II::BLOCK_II( SubsystemDirector* _director, const string& _ident, int nID ):SSME( _director, _ident, nID )
	{
		MPL = BLOCK_II_MPL;
		FPL = BLOCK_II_FPL;
		FPL_THRUST = BLOCK_II_FPL_THRUST;
		RPL_PC_PRESS = BLOCK_II_RPL_PC_PRESS;
		ISP0 = BLOCK_II_ISP0;
		ISP1 = BLOCK_II_ISP1;

		////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////

		// load CCV ignition position table
		CCVSIindex = new short;
		CCVScheduleIgnition_Time = new double[4];
		CCVScheduleIgnition_Position = new double[4];
		CCVScheduleIgnition_Rate = new double[4];
		*CCVSIindex = 3;// # of vlv cmds
		CCVScheduleIgnition_Time[0] = 1.47;
		CCVScheduleIgnition_Time[1] = 2.4;
		CCVScheduleIgnition_Time[2] = 2.85;
		CCVScheduleIgnition_Time[3] = 100;// fake
		CCVScheduleIgnition_Position[0] = 0.7;
		CCVScheduleIgnition_Position[1] = 0.51;
		CCVScheduleIgnition_Position[2] = 1;
		CCVScheduleIgnition_Position[3] = 0;// fake
		CCVScheduleIgnition_Rate[0] = 96.77;
		CCVScheduleIgnition_Rate[1] = 42.22;
		CCVScheduleIgnition_Rate[2] = 56.32;
		CCVScheduleIgnition_Rate[3] = 0;// fake

		// load MOV ignition position table
		MOVSIindex = new short;
		MOVScheduleIgnition_Time = new double[3];
		MOVScheduleIgnition_Position = new double[3];
		MOVScheduleIgnition_Rate = new double[3];
		*MOVSIindex = 2;// # of vlv cmds
		MOVScheduleIgnition_Time[0] = 0.2;
		MOVScheduleIgnition_Time[1] = 2.49;
		MOVScheduleIgnition_Time[2] = 100;// fake
		MOVScheduleIgnition_Position[0] = 0.59;
		MOVScheduleIgnition_Position[1] = 1;
		MOVScheduleIgnition_Position[2] = 0;// fake
		MOVScheduleIgnition_Rate[0] = 54.128;
		MOVScheduleIgnition_Rate[1] = 28.276;
		MOVScheduleIgnition_Rate[2] = 0;// fake

		// load MFV ignition position table
		MFVSIindex = new short;
		MFVScheduleIgnition_Time = new double[2];
		MFVScheduleIgnition_Position = new double[2];
		MFVScheduleIgnition_Rate = new double[2];
		*MFVSIindex = 1;// # of vlv cmds
		MFVScheduleIgnition_Time[0] = 0;
		MFVScheduleIgnition_Time[1] = 100;// fake
		MFVScheduleIgnition_Position[0] = 1;
		MFVScheduleIgnition_Position[1] = 0;// fake
		MFVScheduleIgnition_Rate[0] = 140.845;
		MFVScheduleIgnition_Rate[1] = 0;// fake

		// load FPOV ignition position table
		FPOVSIindex = new short;
		FPOVScheduleIgnition_Time = new double[13];
		FPOVScheduleIgnition_Position = new double[13];
		FPOVScheduleIgnition_Rate = new double[13];
		*FPOVSIindex = 12;// # of vlv cmds
		FPOVScheduleIgnition_Time[0] = 0.1;
		FPOVScheduleIgnition_Time[1] = 0.72;
		FPOVScheduleIgnition_Time[2] = 0.79;
		FPOVScheduleIgnition_Time[3] = 1.15;
		FPOVScheduleIgnition_Time[4] = 1.52;
		FPOVScheduleIgnition_Time[5] = 1.77;
		FPOVScheduleIgnition_Time[6] = 2.05;
		FPOVScheduleIgnition_Time[7] = 2.42;
		FPOVScheduleIgnition_Time[8] = 2.48;
		FPOVScheduleIgnition_Time[9] = 2.77;
		FPOVScheduleIgnition_Time[10] = 3.38;
		FPOVScheduleIgnition_Time[11] = 3.75;
		FPOVScheduleIgnition_Time[12] = 100;// fake
		FPOVScheduleIgnition_Position[0] = 0.56;
		FPOVScheduleIgnition_Position[1] = 0.47;
		FPOVScheduleIgnition_Position[2] = 0.54;
		FPOVScheduleIgnition_Position[3] = 0.51;
		FPOVScheduleIgnition_Position[4] = 0.48;
		FPOVScheduleIgnition_Position[5] = 0.53;
		FPOVScheduleIgnition_Position[6] = 0.50;
		FPOVScheduleIgnition_Position[7] = 0.565;
		FPOVScheduleIgnition_Position[8] = 0.58;
		FPOVScheduleIgnition_Position[9] = 0.67;
		FPOVScheduleIgnition_Position[10] = 0.76;
		FPOVScheduleIgnition_Position[11] = 0.78298;
		FPOVScheduleIgnition_Position[12] = 0;// fake
		FPOVScheduleIgnition_Rate[0] = 169.967;
		FPOVScheduleIgnition_Rate[1] = 128.571;
		FPOVScheduleIgnition_Rate[2] = 43.75;
		FPOVScheduleIgnition_Rate[3] = 50;
		FPOVScheduleIgnition_Rate[4] = 8.108;
		FPOVScheduleIgnition_Rate[5] = 31.25;
		FPOVScheduleIgnition_Rate[6] = 33.333;
		FPOVScheduleIgnition_Rate[7] = 93.333;
		FPOVScheduleIgnition_Rate[8] = 5.172;
		FPOVScheduleIgnition_Rate[9] = 14.754;
		FPOVScheduleIgnition_Rate[10] = 24.324;
		FPOVScheduleIgnition_Rate[11] = 4.178;
		FPOVScheduleIgnition_Rate[12] = 0;// fake

		// load OPOV ignition position table
		OPOVSIindex = new short;
		OPOVScheduleIgnition_Time = new double[14];
		OPOVScheduleIgnition_Position = new double[14];
		OPOVScheduleIgnition_Rate = new double[14];
		*OPOVSIindex = 13;// # of vlv cmds
		OPOVScheduleIgnition_Time[0] = 0.12;
		OPOVScheduleIgnition_Time[1] = 0.3;
		OPOVScheduleIgnition_Time[2] = 0.74;
		OPOVScheduleIgnition_Time[3] = 1.08;
		OPOVScheduleIgnition_Time[4] = 1.16;
		OPOVScheduleIgnition_Time[5] = 1.55;
		OPOVScheduleIgnition_Time[6] = 1.77;
		OPOVScheduleIgnition_Time[7] = 2.09;
		OPOVScheduleIgnition_Time[8] = 2.4;
		OPOVScheduleIgnition_Time[9] = 2.46;
		OPOVScheduleIgnition_Time[10] = 2.84;
		OPOVScheduleIgnition_Time[11] = 3.37;
		OPOVScheduleIgnition_Time[12] = 3.79;
		OPOVScheduleIgnition_Time[13] = 100;// fake
		OPOVScheduleIgnition_Position[0] = 0.28;
		OPOVScheduleIgnition_Position[1] = 0.38;
		OPOVScheduleIgnition_Position[2] = 0.525;
		OPOVScheduleIgnition_Position[3] = 0.42;
		OPOVScheduleIgnition_Position[4] = 0.40;
		OPOVScheduleIgnition_Position[5] = 0.375;
		OPOVScheduleIgnition_Position[6] = 0.34;
		OPOVScheduleIgnition_Position[7] = 0.44;
		OPOVScheduleIgnition_Position[8] = 0.48;
		OPOVScheduleIgnition_Position[9] = 0.50;
		OPOVScheduleIgnition_Position[10] = 0.57;
		OPOVScheduleIgnition_Position[11] = 0.63;
		OPOVScheduleIgnition_Position[12] = 0.63797;
		OPOVScheduleIgnition_Position[13] = 0;// fake
		OPOVScheduleIgnition_Rate[0] = 155.556;
		OPOVScheduleIgnition_Rate[1] = 22.727;
		OPOVScheduleIgnition_Rate[2] = 90.625;
		OPOVScheduleIgnition_Rate[3] = 131.25;
		OPOVScheduleIgnition_Rate[4] = 5.128;
		OPOVScheduleIgnition_Rate[5] = 50;
		OPOVScheduleIgnition_Rate[6] = 10.938;
		OPOVScheduleIgnition_Rate[7] = 111.111;
		OPOVScheduleIgnition_Rate[8] = 66.667;
		OPOVScheduleIgnition_Rate[9] = 5.263;
		OPOVScheduleIgnition_Rate[10] = 13.208;
		OPOVScheduleIgnition_Rate[11] = 14.286;
		OPOVScheduleIgnition_Rate[12] = 1.594;
		OPOVScheduleIgnition_Rate[13] = 0;// fake

		////////////////////////////////////////////////////////////////////////////////////////////

		// load CCV shutdown position table (from 100%)
		CCVSSindex = new short;
		CCVScheduleShutdown_Time = new double[4];
		CCVScheduleShutdown_Position = new double[4];
		CCVScheduleShutdown_Rate = new double[4];
		*CCVSSindex = 3;// # of vlv cmds
		CCVScheduleShutdown_Time[0] = 0;
		CCVScheduleShutdown_Time[1] = 0.605;
		CCVScheduleShutdown_Time[2] = 4.593;
		CCVScheduleShutdown_Time[3] = 100;// fake
		CCVScheduleShutdown_Position[0] = 0.69545;
		CCVScheduleShutdown_Position[1] = 0.74848;
		CCVScheduleShutdown_Position[2] = 0;
		CCVScheduleShutdown_Position[3] = 0;// fake
		CCVScheduleShutdown_Rate[0] = 93.42;
		CCVScheduleShutdown_Rate[1] = 50.99;
		CCVScheduleShutdown_Rate[2] = 103.811;
		CCVScheduleShutdown_Rate[3] = 0;// fake

		// load MOV shutdown position table (from 100%)
		MOVSSindex = new short;
		MOVScheduleShutdown_Time = new double[3];
		MOVScheduleShutdown_Position = new double[3];
		MOVScheduleShutdown_Rate = new double[3];
		*MOVSSindex = 2;// # of vlv cmds
		MOVScheduleShutdown_Time[0] = 0;
		MOVScheduleShutdown_Time[1] = 0.368;
		MOVScheduleShutdown_Time[2] = 100;// fake
		MOVScheduleShutdown_Position[0] = 0.94063;
		MOVScheduleShutdown_Position[1] = 0;
		MOVScheduleShutdown_Position[2] = 0;// fake
		MOVScheduleShutdown_Rate[0] = 16.133;
		MOVScheduleShutdown_Rate[1] = 40;
		MOVScheduleShutdown_Rate[2] = 0;// fake

		// load MFV shutdown position table (from 100%)
		MFVSSindex = new short;
		MFVScheduleShutdown_Time = new double[3];
		MFVScheduleShutdown_Position = new double[3];
		MFVScheduleShutdown_Rate = new double[3];
		*MFVSSindex = 2;// # of vlv cmds
		MFVScheduleShutdown_Time[0] = 1.329;
		MFVScheduleShutdown_Time[1] = 4.559;
		MFVScheduleShutdown_Time[2] = 100;// fake
		MFVScheduleShutdown_Position[0] = 0.61406;
		MFVScheduleShutdown_Position[1] = 0;
		MFVScheduleShutdown_Position[2] = 0;// fake
		MFVScheduleShutdown_Rate[0] = 17.631;
		MFVScheduleShutdown_Rate[1] = 73.015;
		MFVScheduleShutdown_Rate[2] = 0;// fake

		// load FPOV shutdown position table (from 100%)
		FPOVSSindex = new short;
		FPOVScheduleShutdown_Time = new double[4];
		FPOVScheduleShutdown_Position = new double[4];
		FPOVScheduleShutdown_Rate = new double[4];
		*FPOVSSindex = 3;// # of vlv cmds
		FPOVScheduleShutdown_Time[0] = 0;
		FPOVScheduleShutdown_Time[1] = 0.713;
		FPOVScheduleShutdown_Time[2] = 1.862;
		FPOVScheduleShutdown_Time[3] = 100;// fake
		FPOVScheduleShutdown_Position[0] = 0.59545;
		FPOVScheduleShutdown_Position[1] = 0.38485;
		FPOVScheduleShutdown_Position[2] = 0;
		FPOVScheduleShutdown_Position[3] = 0;// fake
		FPOVScheduleShutdown_Rate[0] = 26.302;
		FPOVScheduleShutdown_Rate[1] = 18.329;
		FPOVScheduleShutdown_Rate[2] = 76.97;
		FPOVScheduleShutdown_Rate[3] = 0;// fake

		// load OPOV shutdown position table (from 100%)
		OPOVSSindex = new short;
		OPOVScheduleShutdown_Time = new double[4];
		OPOVScheduleShutdown_Position = new double[4];
		OPOVScheduleShutdown_Rate = new double[4];
		*OPOVSSindex = 3;// # of vlv cmds
		OPOVScheduleShutdown_Time[0] = 0;
		OPOVScheduleShutdown_Time[1] = 0.299;
		OPOVScheduleShutdown_Time[2] = 1.425;
		OPOVScheduleShutdown_Time[3] = 100;// fake
		OPOVScheduleShutdown_Position[0] = 0.5106;
		OPOVScheduleShutdown_Position[1] = 0.1697;
		OPOVScheduleShutdown_Position[2] = 0;
		OPOVScheduleShutdown_Position[3] = 0;// fake
		OPOVScheduleShutdown_Rate[0] = 42.599;
		OPOVScheduleShutdown_Rate[1] = 30.275;
		OPOVScheduleShutdown_Rate[2] = 209.506;
		OPOVScheduleShutdown_Rate[3] = 0;// fake

		////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////
		return;
	}

	BLOCK_II::~BLOCK_II( void )
	{
		delete CCVSIindex;
		delete[] CCVScheduleIgnition_Time;
		delete[] CCVScheduleIgnition_Position;
		delete[] CCVScheduleIgnition_Rate;

		delete MOVSIindex;
		delete[] MOVScheduleIgnition_Time;
		delete[] MOVScheduleIgnition_Position;
		delete[] MOVScheduleIgnition_Rate;

		delete MFVSIindex;
		delete[] MFVScheduleIgnition_Time;
		delete[] MFVScheduleIgnition_Position;
		delete[] MFVScheduleIgnition_Rate;

		delete FPOVSIindex;
		delete[] FPOVScheduleIgnition_Time;
		delete[] FPOVScheduleIgnition_Position;
		delete[] FPOVScheduleIgnition_Rate;

		delete OPOVSIindex;
		delete[] OPOVScheduleIgnition_Time;
		delete[] OPOVScheduleIgnition_Position;
		delete[] OPOVScheduleIgnition_Rate;

		delete CCVSSindex;
		delete[] CCVScheduleShutdown_Time;
		delete[] CCVScheduleShutdown_Position;
		delete[] CCVScheduleShutdown_Rate;

		delete MOVSSindex;
		delete[] MOVScheduleShutdown_Time;
		delete[] MOVScheduleShutdown_Position;
		delete[] MOVScheduleShutdown_Rate;

		delete MFVSSindex;
		delete[] MFVScheduleShutdown_Time;
		delete[] MFVScheduleShutdown_Position;
		delete[] MFVScheduleShutdown_Rate;

		delete FPOVSSindex;
		delete[] FPOVScheduleShutdown_Time;
		delete[] FPOVScheduleShutdown_Position;
		delete[] FPOVScheduleShutdown_Rate;

		delete OPOVSSindex;
		delete[] OPOVScheduleShutdown_Time;
		delete[] OPOVScheduleShutdown_Position;
		delete[] OPOVScheduleShutdown_Rate;
		// the end
	}

	bool BLOCK_II::OnParseLine( const char* line )// waiting for .scn format standard
	{
		/*int idata1 = 0;
		int idata2 = 0;
		int idata3 = 0;
		int idata4 = 0;

		char IDstr[6];

		sprintf( IDstr, "SSME%d", ID );

		if (!_strnicmp( line, IDstr, 5 ))
		{
			sscanf( line + 5, "%f %d %f %f %d %d %d %f %f %f %f %f",
				&SSME_DATA_TABLE->timeTAG,
				&idata1,
				&SSME_DATA_TABLE->powerlevel,
				&SSME_DATA_TABLE->cmdPC,
				&idata2,
				&idata3,
				&idata4,
				&SSME_DATA_TABLE->timeESC,
				&SSME_DATA_TABLE->timeCO,
				&SSME_DATA_TABLE->posOPOV, 
				&SSME_DATA_TABLE->posFPOV,
				&SSME_DATA_TABLE->posMFV );

			SSME_DATA_TABLE->STATUS = (ENGINE_STATUS)idata1;
			if (idata2 == 1) SSME_DATA_TABLE->ShutdownEnable = true;
			if (idata3 == 1) SSME_DATA_TABLE->StartEnable = true;
			activeDCU = (DCU)idata4;

			STS()->SetThrusterLevel( thSSME, PCfromSTStoOSFS( SSME_DATA_TABLE->powerlevel ) );
			return true;
		}*/
		return false;
	}

	void BLOCK_II::OnSaveState( FILEHANDLE scn ) const// waiting for .scn format standard
	{
		/*char sdata[128];

		sprintf( sdata, "  SSME%d %f %d %f %f %d %d %d %f %f %f %f %f",
			ID,
			SSME_DATA_TABLE->timeTAG, 
			SSME_DATA_TABLE->STATUS,
			SSME_DATA_TABLE->powerlevel,
			SSME_DATA_TABLE->cmdPC, 
			SSME_DATA_TABLE->ShutdownEnable,
			SSME_DATA_TABLE->StartEnable,
			activeDCU, 
			SSME_DATA_TABLE->timeESC,
			SSME_DATA_TABLE->timeCO,
			SSME_DATA_TABLE->posOPOV, 
			SSME_DATA_TABLE->posFPOV,
			SSME_DATA_TABLE->posMFV );
		//oapiWriteLine( scn, sdata );*/
		return;
	}

	void BLOCK_II::ValveShutdownTableUpdate( double dPC )
	{
		// update CCV shutdown position table
		//CCVScheduleShutdown_Time[0] = (dPC * 0) + CCVScheduleShutdown_Time[0];
		CCVScheduleShutdown_Time[1] = (dPC * -0.0075) + CCVScheduleShutdown_Time[1];
		CCVScheduleShutdown_Time[2] = (dPC * -0.0082) + CCVScheduleShutdown_Time[2];
		CCVScheduleShutdown_Position[0] = (dPC * -0.3745) + CCVScheduleShutdown_Position[0];
		CCVScheduleShutdown_Position[1] = (dPC * -0.0046) + CCVScheduleShutdown_Position[1];
		//CCVScheduleShutdown_Position[2] = (dPC * 0) + CCVScheduleShutdown_Position[2];
		CCVScheduleShutdown_Rate[0] = (dPC * -1.1235) + CCVScheduleShutdown_Rate[0];
		CCVScheduleShutdown_Rate[1] = (dPC * 0.6761) + CCVScheduleShutdown_Rate[1];
		CCVScheduleShutdown_Rate[2] = (dPC * -0.0325) + CCVScheduleShutdown_Rate[2];

		// update MOV shutdown position table
		//MOVScheduleShutdown_Time[0] = (dPC * 0) + MOVScheduleShutdown_Time[0];
		MOVScheduleShutdown_Time[1] = (dPC * -0.0034) + MOVScheduleShutdown_Time[1];
		MOVScheduleShutdown_Position[0] = (dPC * 0.0095) + MOVScheduleShutdown_Position[0];
		//MOVScheduleShutdown_Position[1] = (dPC * 0) + MOVScheduleShutdown_Position[1];
		MOVScheduleShutdown_Rate[0] = (dPC * 0.1744) + MOVScheduleShutdown_Rate[0];
		//MOVScheduleShutdown_Rate[1] = (dPC * 0) + MOVScheduleShutdown_Rate[1];

		// update MFV shutdown position table
		MFVScheduleShutdown_Time[0] = (dPC * 0.0019) + MFVScheduleShutdown_Time[0];
		MFVScheduleShutdown_Time[1] = (dPC * -0.0067) + MFVScheduleShutdown_Time[1];
		MFVScheduleShutdown_Position[0] = (dPC * 0.0048) + MFVScheduleShutdown_Position[0];
		//MFVScheduleShutdown_Position[1] = (dPC * 0) + MFVScheduleShutdown_Position[1];
		MFVScheduleShutdown_Rate[0] = (dPC * 0.7528) + MFVScheduleShutdown_Rate[0];
		MFVScheduleShutdown_Rate[1] = (dPC * 0.1281) + MFVScheduleShutdown_Rate[1];

		// update FPOV shutdown position table
		//FPOVScheduleShutdown_Time[0] = (dPC * 0) + FPOVScheduleShutdown_Time[0];
		FPOVScheduleShutdown_Time[1] = (dPC * -0.0142) + FPOVScheduleShutdown_Time[1];
		FPOVScheduleShutdown_Time[2] = (dPC * -0.0147) + FPOVScheduleShutdown_Time[2];
		FPOVScheduleShutdown_Position[0] = (dPC * 0.028) + FPOVScheduleShutdown_Position[0];
		FPOVScheduleShutdown_Position[1] = (dPC * 0.0175) + FPOVScheduleShutdown_Position[1];
		//FPOVScheduleShutdown_Position[2] = (dPC * 0) + FPOVScheduleShutdown_Position[2];
		FPOVScheduleShutdown_Rate[0] = (dPC * 0.1577) + FPOVScheduleShutdown_Rate[0];
		FPOVScheduleShutdown_Rate[1] = (dPC * 0.0161) + FPOVScheduleShutdown_Rate[1];
		FPOVScheduleShutdown_Rate[2] = (dPC * 0.0982) + FPOVScheduleShutdown_Rate[2];

		// update OPOV shutdown position table
		//OPOVScheduleShutdown_Time[0] = (dPC * 0) + OPOVScheduleShutdown_Time[0];
		//OPOVScheduleShutdown_Time[1] = (dPC * 0) + OPOVScheduleShutdown_Time[1];
		OPOVScheduleShutdown_Time[2] = (dPC * -0.0014) + OPOVScheduleShutdown_Time[2];
		OPOVScheduleShutdown_Position[0] = (dPC * -0.1897) + OPOVScheduleShutdown_Position[0];
		OPOVScheduleShutdown_Position[1] = (dPC * -0.2112) + OPOVScheduleShutdown_Position[1];
		//OPOVScheduleShutdown_Position[2] = (dPC * 0) + OPOVScheduleShutdown_Position[2];
		OPOVScheduleShutdown_Rate[0] = (dPC * -0.3136) + OPOVScheduleShutdown_Rate[0];
		OPOVScheduleShutdown_Rate[1] = (dPC * 0.0599) + OPOVScheduleShutdown_Rate[1];
		OPOVScheduleShutdown_Rate[2] = (dPC * -0.737) + OPOVScheduleShutdown_Rate[2];
		return;
	}

	void BLOCK_II::VDTUpdate( double time )
	{
		VDT->DW1 = (float)time;
		VDT->DW2 = (float)ID;
		VDT->DW3 = (float)STATUSWORD;
		//VDT->DW4 = 0;
		//VDT->DW5 = 0;
		VDT->DW6 = (float)((PRESS_MCC_A1 + PRESS_MCC_A1) / 2);
		VDT->DW7 = (float)PRESS_MCC_B1;
		//VDT->DW8 = 0;
		//VDT->DW9 = 0;
		//VDT->DW10 = 0;
		//VDT->DW11 = 0;
		//VDT->DW12 = 0;
		//VDT->DW13 = PC_CMD;//
		//VDT->DW14 = PC_REF;//
		//VDT->DW15 = 0;
		//VDT->DW16 = 0;
		//VDT->DW17 = 0;
		//VDT->DW18 = 0;
		//VDT->DW19 = 0;
		//VDT->DW20 = 0;
		//VDT->DW21 = 0;
		//VDT->DW22 = ptrCCV->GetPos();//
		//VDT->DW23 = ptrMOV->GetPos();//
		//VDT->DW24 = ptrMFV->GetPos();//
		//VDT->DW25 = ptrFPOV->GetPos();//
		//VDT->DW26 = ptrOPOV->GetPos();//
		//VDT->DW27 = ptrOBV->GetPos();//
		//VDT->DW28 = ptrFBV->GetPos();//
		//VDT->DW29 = 0;
		//VDT->DW30 = 0;
		//VDT->DW31 = 0;
		//VDT->DW32 = 0;
		return;
	}


	// data cookup
	// ESC
	double BLOCK_II::dcPC_ESC( double tme )
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
	double BLOCK_II::dcPC_CO( double tme )
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


	// Mainstage
	double BLOCK_II::dcPC_MS( double dtime )
	{
		double pc;

		if (PC_CMD < PRESS_MCC_A1)
		{
			// ---
			pc = PCfromPSItoPCT( PRESS_MCC_A1 ) - (10 * dtime);
			if (pc < PCfromPSItoPCT( PC_CMD )) pc = PCfromPSItoPCT( PC_CMD );
		}
		else
		{
			// +++
			pc = PCfromPSItoPCT( PRESS_MCC_A1 ) + (10 * dtime);
			if (pc > PCfromPSItoPCT( PC_CMD )) pc = PCfromPSItoPCT( PC_CMD );
		}
		return pc;
	}


	// Valve Schedules

	void BLOCK_II::ValveScheduleThrottle( double tgtpc )
	{
		// CCV Throttle Schedule
		if (tgtpc >= 100)
		{
			ptrCCV->Move( 1, 9.606 );
		}
		else
		{
			ptrCCV->Move( ((31.7 * tgtpc) + 130) / 3300, 9.606 );
		}

		// MOV Throttle Schedule
		ptrMOV->Open();

		// MFV Throttle Schedule
		ptrMFV->Open();

		// FPOV Throttle Schedule
		ptrFPOV->Move( ((0.0035 * tgtpc * tgtpc) - (0.3168 * tgtpc) + 74.978) / 100, 2.782 );

		// OPOV Throttle Schedule
		ptrOPOV->Move( ((0.0042 * tgtpc * tgtpc) - (0.4179 * tgtpc) + 63.587) / 100, 2.961 );

		return;
	}
}