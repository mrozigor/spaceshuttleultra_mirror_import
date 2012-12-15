#include "SSMEControllerSW.h"
#include "DigitalComputerUnit.h"
#include "MPSdefs.h"


namespace mps
{
	SSMEControllerSW::SSMEControllerSW( DigitalComputerUnit* DCU )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " SSMEControllerSW::SSMEControllerSW in" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

		this->DCU = DCU;

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " SSMEControllerSW::SSMEControllerSW out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	SSMEControllerSW::~SSMEControllerSW( void )
	{
		return;
	}

	void SSMEControllerSW::GetCommands( void )// TODO fix consts (AD08, ....)
	{
		DCU->RAM[RAM_AD08_CMD1] = DCU->DIO_in( DEV_CIE_VIE_CMD1 );
		DCU->RAM[RAM_AD08_CMD2] = DCU->DIO_in( DEV_CIE_VIE_CMD2 );
		DCU->RAM[RAM_AD08_CMD3] = DCU->DIO_in( DEV_CIE_VIE_CMD3 );
		return;
	}

	void SSMEControllerSW::ValidateCommands( void )// TODO fix consts (AD08, ....)
	{
		// TODO joint with GetCommands?
		if (DCU->RAM[RAM_AD08_CMD1] == DCU->RAM[RAM_AD08_CMD2])
		{
			if (DCU->RAM[RAM_AD08_CMD2] == DCU->RAM[RAM_AD08_CMD3])
			{
				// 1 good
				// 2 good
				// 3 good
				DCU->RAM[RAM_AD08_VALIDCMD] = DCU->RAM[RAM_AD08_CMD3];
			}
			else
			{
				// 1 good
				// 2 good
				// 3 bad
				DCU->RAM[RAM_AD08_VALIDCMD] = DCU->RAM[RAM_AD08_CMD2];
			}
		}
		else
		{
			if (DCU->RAM[RAM_AD08_CMD2] == DCU->RAM[RAM_AD08_CMD3])
			{
				// 1 bad
				// 2 good
				// 3 good
				DCU->RAM[RAM_AD08_VALIDCMD] = DCU->RAM[RAM_AD08_CMD3];
			}
			else
			{
				if (DCU->RAM[RAM_AD08_CMD1] == DCU->RAM[RAM_AD08_CMD3])
				{
					// 1 good
					// 2 bad
					// 3 good
					DCU->RAM[RAM_AD08_VALIDCMD] = DCU->RAM[RAM_AD08_CMD3];
				}
				else
				{
					// 1 bad
					// 2 bad
					// 3 bad
					DCU->RAM[RAM_AD08_VALIDCMD] = NOP;
					DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				}
			}
		}
		return;
	}

	void SSMEControllerSW::SensorInput( void )// TODO fix consts (AD08, ....)
	{
		// first sensor is enough, it's all sequencial
		// IE chA
		DCU->DMA_write( 0x1000, RAM_AD08_SENSOR_A + 19, 2 );// flow
		DCU->DMA_write( 0x1004, RAM_AD08_SENSOR_A + 21, 2 );// speed
		DCU->DMA_write( 0x1F00, RAM_AD08_SENSOR_A, 11 );// press
		DCU->DMA_write( 0x1F0F, RAM_AD08_SENSOR_A + 11, 8 );// temp

		// IE chB
		DCU->DMA_write( 0x2000, RAM_AD08_SENSOR_B + 19, 2 );// flow
		DCU->DMA_write( 0x2004, RAM_AD08_SENSOR_B + 21, 2 );// speed
		DCU->DMA_write( 0x2F00, RAM_AD08_SENSOR_B, 11 );// press
		DCU->DMA_write( 0x2F0F, RAM_AD08_SENSOR_B + 11, 8 );// temp

		if (DCU->RAM[RAM_AD08_CH] == chA)// HACK assuming OE and DCU always fail together
		{
			// read a
			DCU->DMA_write( 0x1F17, RAM_AD08_CCV_POS, 5 );// actuator position
			DCU->DMA_write( 0x1F1C, RAM_AD08_CCV_SH, 5 );// sample & hold
		}
		else
		{
			if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
			{
				// read b
				DCU->DMA_write( 0x2F17, RAM_AD08_CCV_POS, 5 );// actuator position
				DCU->DMA_write( 0x2F1C, RAM_AD08_CCV_SH, 5 );// sample & hold
			}
			else
			{
				// read a
				DCU->DMA_write( 0x1F17, RAM_AD08_CCV_POS, 5 );// actuator position
				DCU->DMA_write( 0x1F1C, RAM_AD08_CCV_SH, 5 );// sample & hold
			}
		}
		return;
	}
}