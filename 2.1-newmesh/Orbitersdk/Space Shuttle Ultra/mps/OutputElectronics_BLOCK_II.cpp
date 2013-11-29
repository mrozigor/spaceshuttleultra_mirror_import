#include "OutputElectronics_BLOCK_II.h"
#include "SSME.h"
#include "MPSdefs.h"


namespace mps
{
	OutputElectronics_BLOCK_II::OutputElectronics_BLOCK_II( int ch, SSME* eng, SSMEController* Controller ):OutputElectronics( ch, eng, Controller )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " OutputElectronics_BLOCK_II::OutputElectronics_BLOCK_II in" );
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
		// check power supply
		if (Controller->PSE_Power( ch ) == false) return;// no power -> no play

		// if both DCUs out -> do nothing
		if ((Controller->CIE_CheckWDTOwn( chA, 0 ) || Controller->CIE_CheckWDTOwn( chA, 1 )) && (Controller->CIE_CheckWDTOwn( chB, 0 ) || Controller->CIE_CheckWDTOwn( chB, 1 ))) return;

		this->time = time;

		// TODO "issue" on/off cmds
		// HACK
		eng->Igniter_MCC[ch] = GetBoolFromInt( GetMaskVal( ONOFFCommandRegister[0], IGNITER_MCC ) >> 4 );
		eng->Igniter_FPB[ch] = GetBoolFromInt( GetMaskVal( ONOFFCommandRegister[0], IGNITER_FPB ) >> 5 );
		eng->Igniter_OPB[ch] = GetBoolFromInt( GetMaskVal( ONOFFCommandRegister[0], IGNITER_OPB ) >> 6 );

		// TODO monitor igniters

		// get valve position
		POS[0] = eng->ptrCCV->GetPos();
		POS[1] = eng->ptrMFV->GetPos();
		POS[2] = eng->ptrMOV->GetPos();
		POS[3] = eng->ptrFPOV->GetPos();
		POS[4] = eng->ptrOPOV->GetPos();

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
		if (!Controller->CIE_CheckWDTOwn( chA, 0 ) && !Controller->CIE_CheckWDTOwn( chA, 1 ))
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
			if (!Controller->CIE_CheckWDTOwn( chB, 0 ) && !Controller->CIE_CheckWDTOwn( chB, 1 ))
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
		return ONOFFCommandRegister[num - 1];// TODO check bounds
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

		memcpy( data, POS, 5 * sizeof(double) );
		return;
	}
}