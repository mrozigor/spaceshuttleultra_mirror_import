#include "DigitalComputerUnit_BLOCK_II.h"
#include "ComputerInterfaceElectronics.h"
#include "MPSdefs.h"


namespace mps
{
	DigitalComputerUnit_BLOCK_II::DigitalComputerUnit_BLOCK_II( int ch, const std::string& sw, SSMEController* Controller ):DigitalComputerUnit( ch, RAMSIZE_DCU_BII, sw, Controller )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " DigitalComputerUnit_BLOCK_II::DigitalComputerUnit_BLOCK_II in || ch:%d|sw:%s", ch, sw.c_str() );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " DigitalComputerUnit_BLOCK_II::DigitalComputerUnit_BLOCK_II out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	DigitalComputerUnit_BLOCK_II::~DigitalComputerUnit_BLOCK_II( void )
	{
		return;
	}

	void DigitalComputerUnit_BLOCK_II::__OnSaveState( FILEHANDLE scn ) const
	{
		return;
	}

	bool DigitalComputerUnit_BLOCK_II::__OnParseLine( const char* line )
	{
		return false;
	}

	void DigitalComputerUnit_BLOCK_II::tmestp( double time, double tmestp )
	{
		this->time = time;
		this->dt = tmestp;

		// check power supply
		if (Controller->PSE_Power( ch ) == true)
		{
			if (funct == NULL)
			{
				PROM();// run PROM
			}
			else
			{
				(sw->*funct)();// run SW
			}
		}
		else
		{
			// do nothing
			// set funcptr to PROM, so it's ready when power on
			funct = NULL;
		}
		return;
	}

	void DigitalComputerUnit_BLOCK_II::PROM( void )
	{
		// clear ESW 
		RAM[RAM_PROM_BII_ESW] = 0;

		// set phase/mode
		RAM[RAM_PROM_BII_ESW] += ESW_PROM + ESW_Standby;

		// get commands
		RAM[RAM_PROM_BII_CMD1] = DIO_in( DEV_CIE_VIE_CMD1 );
		RAM[RAM_PROM_BII_CMD2] = DIO_in( DEV_CIE_VIE_CMD2 );
		RAM[RAM_PROM_BII_CMD3] = DIO_in( DEV_CIE_VIE_CMD3 );

		if (RAM[RAM_PROM_BII_CMD1] == RAM[RAM_PROM_BII_CMD2])
		{
			if (RAM[RAM_PROM_BII_CMD2] == RAM[RAM_PROM_BII_CMD3])
			{
				// 1 good
				// 2 good
				// 3 good
				RAM[RAM_PROM_BII_VALIDCMD] = RAM[RAM_PROM_BII_CMD3];
			}
			else
			{
				// 1 good
				// 2 good
				// 3 bad
				RAM[RAM_PROM_BII_VALIDCMD] = RAM[RAM_PROM_BII_CMD2];
			}
		}
		else
		{
			if (RAM[RAM_PROM_BII_CMD2] == RAM[RAM_PROM_BII_CMD3])
			{
				// 1 bad
				// 2 good
				// 3 good
				RAM[RAM_PROM_BII_VALIDCMD] = RAM[RAM_PROM_BII_CMD3];
			}
			else
			{
				if (RAM[RAM_PROM_BII_CMD1] == RAM[RAM_PROM_BII_CMD3])
				{
					// 1 good
					// 2 bad
					// 3 good
					RAM[RAM_PROM_BII_VALIDCMD] = RAM[RAM_PROM_BII_CMD3];
				}
				else
				{
					// 1 bad
					// 2 bad
					// 3 bad
					RAM[RAM_PROM_BII_VALIDCMD] = NOP;
					RAM[RAM_PROM_BII_ESW] += ESW_Rejected;
				}
			}
		}

		switch (RAM[RAM_PROM_BII_VALIDCMD])
		{
			case NOP:// do nothing
				break;
			case RVRC:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				CIE->RestoreVRC();
				break;
			case SVRC:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				CIE->SwitchVRC();
				break;
			case XFRT:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case MLDA:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				CIE->InitializeWDT( 0 );
				CIE->InitializeWDT( 1 );
				break;
			case MLDB:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case RSCA:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case RSCB:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case HELA:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case HELB:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case PSCA:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case PSCB:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case IOHA:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case IOHB:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case IOLA:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case IOLB:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case IOSA:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case IOSB:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case ENFA:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case ENFB:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case ROFA:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case ROFB:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case RWRA:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case RWRB:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case RSKA:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case RSKB:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case EXPM:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				funct = &SSMEControllerSW::PostShutdown_Standby;// TODO first go to pneumatic shutdown then p/s stby
				break;
			case MRC1:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			case MRC2:
				RAM[RAM_PROM_BII_PRVCMD] = RAM[RAM_PROM_BII_CURCMD];
				RAM[RAM_PROM_BII_CURCMD] = RAM[RAM_PROM_BII_VALIDCMD];
				break;
			default:
				RAM[RAM_PROM_BII_ESW] += ESW_Rejected;
				break;
		}

		// reset own WDT
		CIE->ResetWDT( 0 );
		CIE->ResetWDT( 1 );

		memset( &RAM[RAM_PROM_BII_DVDT_1], 0, 128 * sizeof(unsigned short) );// zeroing

		// check other channel
		RAM[32] = CIE->CheckWDTOpposite( 0 );
		RAM[33] = CIE->CheckWDTOpposite( 1 );

		// check own channel while we're at it
		RAM[30] = CIE->CheckWDTOwn( 0 );
		RAM[31] = CIE->CheckWDTOwn( 1 );

		RAM[26] = RAM[RAM_PROM_BII_CMD1];
		RAM[27] = RAM[RAM_PROM_BII_CMD2];
		RAM[28] = RAM[RAM_PROM_BII_CMD3];
		RAM[29] = RAM[RAM_PROM_BII_VALIDCMD];

		// update TREF 1
		RAM[RAM_PROM_BII_DVDT_1] = (unsigned short)(time * 100);

		// write ch
		RAM[RAM_PROM_BII_DVDT_2] = ch;

		// built DVDT
		// this is from Phase II PROM, not sure if changed since
		//RAM[RAM_PROM_BII_DVDT_1] = 1;// ID word 1
		//RAM[RAM_PROM_BII_DVDT_1] = 2;// ID word 2
		RAM[RAM_PROM_BII_DVDT_3] = RAM[RAM_PROM_BII_ESW];// Engine Status Word
		RAM[RAM_PROM_BII_DVDT_5] = 5;// FID/Delimiter
		RAM[RAM_PROM_BII_DVDT_90] = 90;// Inhibit Counter/PROM Rev.
		RAM[RAM_PROM_BII_DVDT_98] = RAM[RAM_PROM_BII_CURCMD];// Current Command
		RAM[RAM_PROM_BII_DVDT_99] = RAM[RAM_PROM_BII_PRVCMD];// Previous Command
		RAM[RAM_PROM_BII_DVDT_100] = 100;// FID/Delimiter
		RAM[RAM_PROM_BII_DVDT_103] = 103;// Parameter

		// output DVDT
		DMA_read( 1, RAM_PROM_BII_DVDT_1, 128 );
		DMA_read( 2, RAM_PROM_BII_DVDT_1, 128 );
		return;
	}
}