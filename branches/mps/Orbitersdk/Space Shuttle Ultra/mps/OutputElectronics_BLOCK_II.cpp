#include "OutputElectronics_BLOCK_II.h"
#include "SSMEController.h"
#include "PowerSupplyElectronics.h"
#include "ComputerInterfaceElectronics.h"
#include "SSME.h"
#include "MPSdefs.h"


namespace mps
{
	OutputElectronics_BLOCK_II::OutputElectronics_BLOCK_II( int ch, SSME* eng, SSMEController* Controller ):OutputElectronics( ch, eng, Controller )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " OutputElectronics_BLOCK_II::OutputElectronics_BLOCK_II in || ch:%d", ch );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " OutputElectronics_BLOCK_II::OutputElectronics_BLOCK_II out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	OutputElectronics_BLOCK_II::~OutputElectronics_BLOCK_II( void )
	{
		return;
	}

	void OutputElectronics_BLOCK_II::__OnSaveState( FILEHANDLE scn ) const
	{
		return;
	}

	bool OutputElectronics_BLOCK_II::__OnParseLine( const char* line )
	{
		return false;
	}

	void OutputElectronics_BLOCK_II::tmestp( double time )
	{
		// check power supply and at least one good DCU
		if ((PSE->Power() == false) || ((CIE[chA]->CheckWDTOwn( 0 ) || CIE[chA]->CheckWDTOwn( 1 )) && (CIE[chB]->CheckWDTOwn( 0 ) || CIE[chB]->CheckWDTOwn( 1 ))))
		{
			HPOTPISPurge_SV->ResetLine();
			EmergencyShutdown_SV->ResetLine();
			ShutdownPurge_SV->ResetLine();
			FuelSystemPurge_SV->ResetLine();
			BleedValvesControl_SV->ResetLine();
			AFV_SV->ResetLine();
			HPV_SV->ResetLine();
			return;
		}

		this->time = time;

		// "issue" on/off cmds
		eng->Igniter_MCC[ch] = GetBoolFromInt( GetMaskVal( ONOFFCommandRegister[1], IGNITER_MCC ) >> 4 );
		eng->Igniter_OPB[ch] = GetBoolFromInt( GetMaskVal( ONOFFCommandRegister[1], IGNITER_OPB ) >> 5 );
		eng->Igniter_FPB[ch] = GetBoolFromInt( GetMaskVal( ONOFFCommandRegister[1], IGNITER_FPB ) >> 6 );

		if (GetBoolFromInt( GetMaskVal( ONOFFCommandRegister[0], SV_HPOTPISPURGE ) >> 4 ) == 1)
		{
			HPOTPISPurge_SV->SetLine();
		}
		else
		{
			HPOTPISPurge_SV->ResetLine();
		}

		if (GetBoolFromInt( GetMaskVal( ONOFFCommandRegister[0], SV_EMERGENCYSHUTDOWN ) >> 5 ) == 1)
		{
			EmergencyShutdown_SV->SetLine();
		}
		else
		{
			EmergencyShutdown_SV->ResetLine();
		}

		if (GetBoolFromInt( GetMaskVal( ONOFFCommandRegister[0], SV_SHUTDOWNPURGE ) >> 6 ) == 1)
		{
			ShutdownPurge_SV->SetLine();
		}
		else
		{
			ShutdownPurge_SV->ResetLine();
		}

		if (GetBoolFromInt( GetMaskVal( ONOFFCommandRegister[0], SV_FUELSYSTEMPURGE ) >> 8 ) == 1)
		{
			FuelSystemPurge_SV->SetLine();
		}
		else
		{
			FuelSystemPurge_SV->ResetLine();
		}

		if (GetBoolFromInt( GetMaskVal( ONOFFCommandRegister[0], SV_BLEEDVALVESCONTROL ) >> 9 ) == 1)
		{
			BleedValvesControl_SV->SetLine();
		}
		else
		{
			BleedValvesControl_SV->ResetLine();
		}
		
		if (GetBoolFromInt( GetMaskVal( ONOFFCommandRegister[0], AFV ) >> 10 ) == 1)
		{
			AFV_SV->SetLine();
		}
		else
		{
			AFV_SV->ResetLine();
		}

		if (GetBoolFromInt( GetMaskVal( ONOFFCommandRegister[0], HPV ) >> 11 ) == 1)
		{
			HPV_SV->SetLine();
		}
		else
		{
			HPV_SV->ResetLine();
		}

		// TODO monitor igniters

		// get valve position
		POS[0] = eng->ptrCCV->GetPos();
		POS[1] = eng->ptrMFV->GetPos();
		POS[2] = eng->ptrMOV->GetPos();
		POS[3] = eng->ptrFPOV->GetPos();
		POS[4] = eng->ptrOPOV->GetPos();
		POS[5] = eng->ptrFBV->GetPos();
		POS[6] = eng->ptrOBV->GetPos();
		POS[7] = eng->ptrAFV->GetPos();
		POS[8] = eng->ptrRIV->GetPos();

		// TODO servovalve drivers and models

		eng->ptrCCV->Move( SH[0] );
		eng->ptrMFV->Move( SH[1] );
		eng->ptrMOV->Move( SH[2] );
		eng->ptrFPOV->Move( SH[3] );
		eng->ptrOPOV->Move( SH[4] );
		return;
	}

	void OutputElectronics_BLOCK_II::StorageRegister_write( unsigned short data, int ch )
	{
		// check WDT, listen to who is working
		if (!CIE[chA]->CheckWDTOwn( 0 ) && !CIE[chA]->CheckWDTOwn( 1 ))
		{
			// do chA
			if (ch == chA)
			{
				StorageRegister = data;
				CommandDecoder();
			}
		}
		else
		{
			if (!CIE[chB]->CheckWDTOwn( 0 ) && !CIE[chB]->CheckWDTOwn( 1 ))
			{
				// do chB
				if (ch == chB)
				{
					StorageRegister = data;
					CommandDecoder();
				}
			}
		}
		return;
	}

	unsigned short OutputElectronics_BLOCK_II::StorageRegister_read( void ) const
	{
		return StorageRegister;
	}

	unsigned short OutputElectronics_BLOCK_II::ONOFFCommandRegister_read( int num ) const
	{
		assert( (num >= 1) && (num <= 2) && "OutputElectronics_BLOCK_IIONOFFCommandRegister_read.num" );
		return ONOFFCommandRegister[num - 1];
	}

	void OutputElectronics_BLOCK_II::CommandDecoder( void )
	{
		switch (GetMaskVal( StorageRegister, 0x000F ))
		{
			case CD_ONOFF_1:// ON/OFF 1
				ONOFFCommandRegister[0] = GetMaskVal( StorageRegister, 0xFFF0 );
				break;
			case CD_ONOFF_2:// ON/OFF 2
				ONOFFCommandRegister[1] = GetMaskVal( StorageRegister, 0xFFF0 );
				break;
			case CD_CCV:// CCV
				SH[0] = (double)(GetMaskVal( StorageRegister, 0xFFF0 ) >> 4) / 4095;
				break;
			case CD_MFV:// MFV
				SH[1] = (double)(GetMaskVal( StorageRegister, 0xFFF0 ) >> 4) / 4095;
				break;
			case CD_MOV:// MOV
				SH[2] = (double)(GetMaskVal( StorageRegister, 0xFFF0 ) >> 4) / 4095;
				break;
			case CD_FPOV:// FPOV
				SH[3] = (double)(GetMaskVal( StorageRegister, 0xFFF0 ) >> 4) / 4095;
				break;
			case CD_OPOV:// OPOV
				SH[4] = (double)(GetMaskVal( StorageRegister, 0xFFF0 ) >> 4) / 4095;
				break;
			default:
				char buffer[100];
				sprintf_s( buffer, 100, " OutputElectronics_BLOCK_II::CommandDecoder || %d", StorageRegister );
				oapiWriteLog( buffer );
		}
		return;
	}

	void OutputElectronics_BLOCK_II::GetSH( double* data )
	{
		memcpy( data, SH, 5 * sizeof(double) );
		return;
	}

	void OutputElectronics_BLOCK_II::GetPOS( double* data )
	{
		// HACK fix the delay on vlv pos
		// get valve position
		POS[0] = eng->ptrCCV->GetPos();
		POS[1] = eng->ptrMFV->GetPos();
		POS[2] = eng->ptrMOV->GetPos();
		POS[3] = eng->ptrFPOV->GetPos();
		POS[4] = eng->ptrOPOV->GetPos();
		POS[5] = eng->ptrFBV->GetPos();
		POS[6] = eng->ptrOBV->GetPos();
		POS[7] = eng->ptrAFV->GetPos();
		POS[8] = eng->ptrRIV->GetPos();

		memcpy( data, POS, 9 * sizeof(double) );
		return;
	}
}