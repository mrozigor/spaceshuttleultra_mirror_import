#include "BLOCK_II.h"
#include "../Atlantis.h"


namespace mps
{
	BLOCK_II::BLOCK_II( SubsystemDirector* _director, const string& _ident, 
		int nID )
		:SSME( _director, _ident, nID )
	{
		MPL = 67;
		FPL = 109;

		FPL_THRUST = BLOCK_II_FPL_THRUST;
		ISP0 = BLOCK_II_ISP0;
		ISP1 = BLOCK_II_ISP1;

		PSN4time = -1;

		SSME_DATA_TABLE->cmdPC = 0;
		SSME_DATA_TABLE->StartEnable = false;
		SSME_DATA_TABLE->ShutdownEnable = false;
		SSME_DATA_TABLE->powerlevel = 0;
		SSME_DATA_TABLE->timeESC = -1;
		SSME_DATA_TABLE->timeCO = -1;
		SSME_DATA_TABLE->STATUS = STARTPREPARATION_PURGESEQUENCE3;
		SSME_DATA_TABLE->posOPOV = 0;
		SSME_DATA_TABLE->posFPOV = 0;
		SSME_DATA_TABLE->posMFV = 0;
		SSME_DATA_TABLE->posMOV = 0;
		SSME_DATA_TABLE->posCCV = 0;
		/*SSME_DATA_TABLE->MR = 0;
		SSME_DATA_TABLE->rpmLPFTP = 0;
		SSME_DATA_TABLE->rpmHPFTP = 0;
		SSME_DATA_TABLE->rpmLPOTP = 0;
		SSME_DATA_TABLE->rpmHPOTP = 0;
		SSME_DATA_TABLE->tempHPFTdis_chA = 520;// ????
		SSME_DATA_TABLE->tempHPOTdis_chA = 520;// ????
		SSME_DATA_TABLE->pressHPOTPintseal = 0;// ????
		SSME_DATA_TABLE->pressHPOTPsecseal = 0;// ????*/
		return;
	}

	BLOCK_II::~BLOCK_II( void )
	{
		// the end
	}

	bool BLOCK_II::OnParseLine( const char* line )// TODO val checks
	{
		int idata1 = 0;
		int idata2 = 0;
		int idata3 = 0;
		int idata4 = 0;

		char IDstr[6];

		sprintf( IDstr, "SSME%d", ID );

		if (!_strnicmp( line, IDstr, 5 ))
		{
			sscanf( line + 5, "%f %d %f %f %d %d %d %f %f %f %f %f %f %f",
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
				&SSME_DATA_TABLE->posMFV,
				&SSME_DATA_TABLE->posMOV, 
				&SSME_DATA_TABLE->posCCV );

			SSME_DATA_TABLE->STATUS = (ENGINE_STATUS)idata1;
			if (idata2 == 1) SSME_DATA_TABLE->ShutdownEnable = true;
			if (idata3 == 1) SSME_DATA_TABLE->StartEnable = true;
			activeDCU = (DCU)idata4;

			STS()->SetThrusterLevel( thSSME, PCfromSTStoOSFS( SSME_DATA_TABLE->powerlevel ) );
			return true;
		}
		return false;
	}

	void BLOCK_II::OnSaveState( FILEHANDLE scn ) const
	{
		char sdata[128];

		sprintf( sdata, "  SSME%d %f %d %f %f %d %d %d %f %f %f %f %f %f %f",
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
			SSME_DATA_TABLE->posMFV,
			SSME_DATA_TABLE->posMOV, 
			SSME_DATA_TABLE->posCCV );
		//oapiWriteLine( scn, sdata );
		return;
	}


	/*double BLOCK_II::PCfromOSFStoSTS( double pcOSFS )
	{
		return (pcOSFS * FPL);
	}


	double BLOCK_II::PCfromSTStoOSFS( double pcSTS )
	{
		return (pcSTS / FPL);
	}

	double BLOCK_II::PCfromPCTtoPSI( double pcPCT )
	{
		// TODO PCT psi
		return ((pcPCT * 3000) / FPL);
	}*/


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

	double BLOCK_II::dcMOV_ESC( double tme )
	{
		double pos;

		if (tme <= 0.2)
		{
			pos = 0;
		}
		else
		{
			if (tme <= 1.29)
			{
				pos = ((tme * 59) - 11.8) / 1.09;
			}
			else
			{
				if (tme <= 2.49)
				{
					pos = 59;
				}
				else
				{
					if (tme <= 3.94)
					{
						pos = ((tme * 41) - 16.54) / 1.45;
					}
					else
					{
						pos = 100;
					}
				}
			}
		}
		return pos / 100;
	}

	double BLOCK_II::dcMFV_ESC( double tme )
	{
		double pos;

		if (tme <= 0.71)
		{
			pos = (100 * tme) / 0.71;
		}
		else
		{
			pos = 100;
		}
		return pos / 100;
	}

	double BLOCK_II::dcCCV_ESC( double tme )
	{
		double pos;

		if (tme <= 1.47)
		{
			pos = 100;
		}
		else
		{
			if (tme <= 1.78)
			{
				pos = (75.1 - (30 * tme)) / 0.31;
			}
			else
			{
				if (tme <= 2.4)
				{
					pos = 70;
				}
				else
				{
					if (tme <= 2.85)
					{
						pos = (77.1 - (19 * tme)) / 0.45;
					}
					else
					{
						if (tme <= 3.72)
						{
							pos = ((49 * tme) - 95.28) / 0.87;
						}
						else
						{
							pos = 100;
						}
					}
				}
			}
		}
		return pos / 100;
	}

	double BLOCK_II::dcFPOV_ESC( double tme )
	{
		double pos;

		if (tme <= 0.1)
		{
			pos = 0;
		}
		else if (tme <= 0.43)
		{
			pos = ((56 * tme) - 5.6) / 0.33;
		}
		else if (tme <= 0.72)
		{
			pos = 56;
		}
		else if (tme <= 0.79)
		{
			pos = (10.4 - (9 * tme)) / 0.07;
		}
		else if (tme <= 0.95)
		{
			pos = (43.75 * tme) + 12.4375;
		}
		else if (tme <= 1.15)
		{
			pos = 54;
		}
		else if (tme <= 1.21)
		{
			pos = 111.5 - (50 * tme);
		}
		else if (tme <= 1.52)
		{
			pos = 51;
		}
		else if (tme <= 1.68)
		{
			pos = 79.5 - (18.75 * tme);
		}
		else if (tme <= 1.77)
		{
			pos = ((5 * tme) - 4.08) / 0.09;
		}
		else if (tme <= 2.05)
		{
			pos = (20.15 - (3 * tme)) / 0.28;
		}
		else if (tme <= 2.42)
		{
			pos = 50;
		}
		else if (tme <= 2.48)
		{
			pos = ((6.5 * tme) - 12.73) / 0.06;
		}
		else if (tme <= 2.77)
		{
			pos = ((3 * tme) + 25.33) / 0.58;
		}
		else if (tme <= 3.38)
		{
			pos = ((9 * tme) + 10.45) / 0.61;
		}
		else if (tme <= 3.75)
		{
			pos = ((9 * tme) - 5.63) / 0.37;
		}
		else if (tme <= 4.3)
		{
			pos = ((2.298 * tme) + 33.1825) / 0.55;
		}
		else
		{
			pos = 78.298;
		}
		return pos / 100;
	}

	double BLOCK_II::dcOPOV_ESC( double tme )
	{
		double pos;

		if (tme <= 0.12)
		{
			pos = 0;
		}
		else if (tme <= 0.3)
		{
			pos = ((14 * tme) - 1.68) / 0.09;
		}
		else if (tme <= 0.74)
		{
			pos = ((10 * tme) + 9.32) / 0.44;
		}
		else if (tme <= 0.9)
		{
			pos = ((14.5 * tme) - 4.65) / 0.16;
		}
		else if (tme <= 1.08)
		{
			pos = 52.5;
		}
		else if (tme <= 1.16)
		{
			pos = 194.25 - (131.25 * tme);
		}
		else if (tme <= 1.55)
		{
			pos = (18.7 - (2 * tme)) / 0.39;
		}
		else if (tme <= 1.6)
		{
			pos = 117.5 - (50 * tme);
		}
		else if (tme <= 1.77)
		{
			pos = 37.5;
		}
		else if (tme <= 2.09)
		{
			pos = (18.195 - (3.5 * tme)) / 0.32;
		}
		else if (tme <= 2.18)
		{
			pos = ((10 * tme) - 17.84) / 0.09;
		}
		else if (tme <= 2.4)
		{
			pos = 44;
		}
		else if (tme <= 2.46)
		{
			pos = ((4 * tme) - 6.96) / 0.06;
		}
		else if (tme <= 2.84)
		{
			pos = ((2 * tme) + 13.32) / 0.38;
		}
		else if (tme <= 3.37)
		{
			pos = ((7 * tme) + 6.62) / 0.53;
		}
		else if (tme <= 3.79)
		{
			pos = ((6 * tme) + 3.72) / 0.42;
		}
		else if (tme <= 4.29)
		{
			pos = ((0.797 * tme) + 28.47937) / 0.5;
		}
		else
		{
			pos = 63.797;
		}
		return pos / 100;
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

	double BLOCK_II::dcMOV_CO( double tme )
	{
		double pos;

		if (tme <= 0.27)
		{
			pos = (27 - (3 * tme)) / 0.27;
		}
		else
		{
			if (tme <= 2.68)
			{
				pos = (259.96 - (97 * tme)) / 2.41;
			}
			else
			{
				pos = 0;
			}
		}
		return pos / 100;
	}

	double BLOCK_II::dcMFV_CO( double tme )
	{
		double pos;

		if (tme <= 1.3)
		{
			pos = 100;
		}
		else
		{
			if (tme <= 3.53)
			{
				pos = (272.4 - (38 * tme)) / 2.23;
			}
			else
			{
				if (tme <= 4.57)
				{
					pos = 62;
				}
				else
				{
					if (tme <= 5.4)
					{
						pos = (334.8 - (62 * tme)) / 0.83;
					}
					else
					{
						pos = 0;
					}
				}
			}
		}
		return pos / 100;
	}

	double BLOCK_II::dcCCV_CO( double tme )
	{
		double pos;

		if (tme <= 0)
		{
			pos = 100;
		}
		else if (tme <= 0.27)
		{
			pos = (27 - (31.7 * tme)) / 0.27;
		}
		else if (tme <= 0.58)
		{
			pos = 68.3;
		}
		else if (tme <= 0.71)
		{
			pos = ((6.7 * tme) + 4.993) / 0.13;
		}
		else if (tme <= 4.58)
		{
			pos = 75;
		}
		else if (tme <= 5.33)
		{
			pos = 533 - (100 * tme);
		}
		else
		{
			pos = 0;
		}
		return pos / 100;
	}

	double BLOCK_II::dcFPOV_CO( double tme, double POS1 )
	{
		double pos;

		double T1 = (POS1 - 60) / 26;

		if (tme <= T1)
		{
			pos = POS1 - (26 * tme);
		}
		else
		{
			if (tme <= (T1 + 1.15))
			{
				pos = (69 - (20 * (tme - T1))) / 1.15;
			}
			else
			{
				if (tme <= (T1 + 1.65))
				{
					pos = (40 * (T1 + 1.65 - tme)) / 0.5;
				}
				else
				{
					pos = 0;
				}
			}
		}
		return pos / 100;
	}

	double BLOCK_II::dcOPOV_CO( double tme, double POS1 )
	{
		double pos;

		double T1 = (52 - POS1) / 45;

		if (tme <= T1)
		{
			pos = POS1 - (45 * tme);
		}
		else
		{
			if (tme <= 1.4)
			{
				pos = (52 + (31.25 * T1)) - (31.25 * tme);
			}
			else
			{
				if (tme <= 1.5)
				{
					double POS2 = 8.25 + (31.25 * T1);
					pos = (POS2 * (1.5 - tme)) / 0.1;
				}
				else
				{
					pos = 0;
				}
			}
		}
		return pos / 100;
	}

	// Mainstage
	double BLOCK_II::dcPC_MS( double dtime )
	{
		double pc;

		if (SSME_DATA_TABLE->cmdPC < SSME_DATA_TABLE->powerlevel)
		{
			// ---
			pc = SSME_DATA_TABLE->powerlevel - (10 * dtime);
			if (pc < SSME_DATA_TABLE->cmdPC) pc = SSME_DATA_TABLE->cmdPC;
		}
		else
		{
			// +++
			pc = SSME_DATA_TABLE->powerlevel + (10 * dtime);
			if (pc > SSME_DATA_TABLE->cmdPC) pc = SSME_DATA_TABLE->cmdPC;
		}
		return pc;
	}

	double BLOCK_II::dcCCV_MS( double pc )
	{
		double pos;

		if (pc >= 100)
		{
			pos = 100;
		}
		else
		{
			pos = ((31.7 * pc) + 130) / 33;
		}
		return pos / 100;
	}

	double BLOCK_II::dcFPOV_MS( double pc )
	{
		return ((0.0035 * pc * pc) - (0.3168 * pc) + 74.978) / 100;
	}

	double BLOCK_II::dcOPOV_MS( double pc )
	{
		return ((0.0042 * pc * pc) - (0.4179 * pc) + 63.587) / 100;
	}
}