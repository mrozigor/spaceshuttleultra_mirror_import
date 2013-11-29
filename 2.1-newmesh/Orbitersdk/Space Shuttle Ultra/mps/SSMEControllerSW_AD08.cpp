#include "SSMEControllerSW_AD08.h"
#include "DigitalComputerUnit.h"
#include "ComputerInterfaceElectronics.h"
#include <UltraMath.h>
#include "MPSdefs.h"


namespace mps
{
	// AD08
	// SSME Controller Block II
	// STS-125
	SSMEControllerSW_AD08::SSMEControllerSW_AD08( DigitalComputerUnit* DCU ):SSMEControllerSW( DCU )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " SSMEControllerSW_AD08::SSMEControllerSW_AD08 in" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

		LoadStartValveSchedule();// HACK "load" ignition valve commands
		LoadShutdownValveSchedule();// HACK "load" shutdown valve commands (from 100%)

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " SSMEControllerSW_AD08::SSMEControllerSW_AD08 out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	SSMEControllerSW_AD08::~SSMEControllerSW_AD08( void )
	{
		return;
	}

	void SSMEControllerSW_AD08::Checkout_HydraulicConditioning( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case EGND:
				RotateCommand();
				break;
			case EFLT:
				RotateCommand();
				break;
			case EFRT:
				RotateCommand();
				break;
			case ETWO:
				RotateCommand();
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case AFRT:
				RotateCommand();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCT:
				RotateCommand();
				//DCU->funct = &SSMEControllerSW::Checkout_Standby;
				//Checkout_Standby_B();
				return;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case IOHA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOHB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOLA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOLB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOSA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOSB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case TMSQ:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::PostShutdown_TerminateSequence;
				PostShutdown_TerminateSequence_B();
				return;
			case COSY:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			case DAVL:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		Checkout_HydraulicConditioning_B();
		return;
	}

	void SSMEControllerSW_AD08::Checkout_Standby( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case EGND:
				RotateCommand();
				break;
			case EFLT:
				RotateCommand();
				break;
			case EFRT:
				RotateCommand();
				break;
			case ETWO:
				RotateCommand();
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case AFRT:
				RotateCommand();
				break;
			case XFRT:
				RotateCommand();
				break;
			case HYDC:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_HydraulicConditioning;
				Checkout_HydraulicConditioning_B();
				return;
			case ERCK:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_ControllerCheckout;
				Checkout_ControllerCheckout_B();
				return;
			case RSCT:
				RotateCommand();
				//DCU->funct = &SSMEControllerSW::Checkout_Standby;
				//Checkout_Standby_B();
				return;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case IOHA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOHB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOLA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOLB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOSA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOSB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case MRC1:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case MRC2:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case PSN1:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::StartPrep_PSN1;
				StartPrep_PSN1_B();
				return;
			case TMSQ:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::PostShutdown_TerminateSequence;
				PostShutdown_TerminateSequence_B();
				return;
			case DAVL:
				RotateCommand();
				//DCU->funct = &SSMEControllerSW::Checkout_Standby;
				//Checkout_Standby_B();
				return;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		Checkout_Standby_B();
		return;
	}

	void SSMEControllerSW_AD08::Checkout_ActuatorCheckout( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case EGND:
				RotateCommand();
				break;
			case EFLT:
				RotateCommand();
				break;
			case EFRT:
				RotateCommand();
				break;
			case ETWO:
				RotateCommand();
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case AFRT:
				RotateCommand();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCT:
				RotateCommand();
				//DCU->funct = &SSMEControllerSW::Checkout_Standby;
				//Checkout_Standby_B();
				return;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case IOHA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOHB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOLA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOLB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOSA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOSB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case TMSQ:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::PostShutdown_TerminateSequence;
				PostShutdown_TerminateSequence_B();
				return;
			case COSY:
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			case DAVL:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		Checkout_ActuatorCheckout_B();
		return;
	}

	void SSMEControllerSW_AD08::Checkout_EngineLeakDetection( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case EGND:
				RotateCommand();
				break;
			case EFLT:
				RotateCommand();
				break;
			case EFRT:
				RotateCommand();
				break;
			case ETWO:
				RotateCommand();
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case AFRT:
				RotateCommand();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCT:
				RotateCommand();
				//DCU->funct = &SSMEControllerSW::Checkout_Standby;
				//Checkout_Standby_B();
				return;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case IOHA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOHB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOLA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOLB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOSA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOSB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case TMSQ:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::PostShutdown_TerminateSequence;
				PostShutdown_TerminateSequence_B();
				return;
			case COSY:
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			case DAVL:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		Checkout_EngineLeakDetection_B();
		return;
	}

	void SSMEControllerSW_AD08::Checkout_IgniterCheckout( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case EGND:
				RotateCommand();
				break;
			case EFLT:
				RotateCommand();
				break;
			case EFRT:
				RotateCommand();
				break;
			case ETWO:
				RotateCommand();
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case AFRT:
				RotateCommand();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCT:
				RotateCommand();
				//DCU->funct = &SSMEControllerSW::Checkout_Standby;
				//Checkout_Standby_B();
				return;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case IOHA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOHB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOLA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOLB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOSA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOSB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case TMSQ:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::PostShutdown_TerminateSequence;
				PostShutdown_TerminateSequence_B();
				return;
			case COSY:
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			case DAVL:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		Checkout_IgniterCheckout_B();
		return;
	}

	void SSMEControllerSW_AD08::Checkout_PneumaticCheckout( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case EGND:
				RotateCommand();
				break;
			case EFLT:
				RotateCommand();
				break;
			case EFRT:
				RotateCommand();
				break;
			case ETWO:
				RotateCommand();
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case AFRT:
				RotateCommand();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCT:
				RotateCommand();
				//DCU->funct = &SSMEControllerSW::Checkout_Standby;
				//Checkout_Standby_B();
				return;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case IOHA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOHB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOLA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOLB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOSA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOSB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case TMSQ:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::PostShutdown_TerminateSequence;
				PostShutdown_TerminateSequence_B();
				return;
			case COSY:
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			case DAVL:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		Checkout_PneumaticCheckout_B();
		return;
	}

	void SSMEControllerSW_AD08::Checkout_SensorCheckout( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case EGND:
				RotateCommand();
				break;
			case EFLT:
				RotateCommand();
				break;
			case EFRT:
				RotateCommand();
				break;
			case ETWO:
				RotateCommand();
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case AFRT:
				RotateCommand();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCT:
				RotateCommand();
				//DCU->funct = &SSMEControllerSW::Checkout_Standby;
				//Checkout_Standby_B();
				return;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case IOHA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOHB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOLA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOLB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOSA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOSB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case TMSQ:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::PostShutdown_TerminateSequence;
				PostShutdown_TerminateSequence_B();
				return;
			case COSY:
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			case DAVL:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		Checkout_SensorCheckout_B();
		return;
	}

	void SSMEControllerSW_AD08::Checkout_ControllerCheckout( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case EGND:
				RotateCommand();
				break;
			case EFLT:
				RotateCommand();
				break;
			case EFRT:
				RotateCommand();
				break;
			case ETWO:
				RotateCommand();
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case AFRT:
				RotateCommand();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCT:
				RotateCommand();
				//DCU->funct = &SSMEControllerSW::Checkout_Standby;
				//Checkout_Standby_B();
				return;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case IOHA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOHB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOLA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOLB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOSA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOSB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case TMSQ:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::PostShutdown_TerminateSequence;
				PostShutdown_TerminateSequence_B();
				return;
			case COSY:
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			case DAVL:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		Checkout_ControllerCheckout_B();
		return;
	}

	void SSMEControllerSW_AD08::StartPrep_PSN1( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCT:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case IOHA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOHB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOLA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOLB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOSA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOSB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case MRC1:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case MRC2:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case PSN2:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::StartPrep_PSN2;
				StartPrep_PSN2_B();
				return;
			case SDEN:
				RotateCommand();
				DCU->RAM[RAM_AD08_SHUTDOWN_ENA] = 1;// TODO check for repeated command?
				break;
			case STDN:// TODO finish logic
				if (DCU->RAM[RAM_AD08_SHUTDOWN_ENA] == 1)
				{
					RotateCommand();
					DCU->RAM[RAM_AD08_TIME_STDN] = 0;// init timer
					DCU->funct = &SSMEControllerSW::Shutdown_FailSafePneumatic;
					Shutdown_FailSafePneumatic_B();
					return;
				}
				else
				{
					DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				}
				break;
			case TMSQ:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::PostShutdown_TerminateSequence;
				PostShutdown_TerminateSequence_B();
				return;
			case COSY:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		StartPrep_PSN1_B();
		return;
	}

	void SSMEControllerSW_AD08::StartPrep_PSN2( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCT:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case IOHA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOHB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOLA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOLB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOSA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOSB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case MRC1:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case MRC2:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case PSN1:
				RotateCommand();
				DCU->funct= &SSMEControllerSW::StartPrep_PSN1;
				StartPrep_PSN1_B();
				return;
			case PSN3:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::StartPrep_PSN3;
				StartPrep_PSN3_B();
				return;
			case SDEN:
				RotateCommand();
				DCU->RAM[RAM_AD08_SHUTDOWN_ENA] = 1;// TODO check for repeated command?
				break;
			case STDN:// TODO finish logic
				if (DCU->RAM[RAM_AD08_SHUTDOWN_ENA] == 1)
				{
					RotateCommand();
					DCU->RAM[RAM_AD08_TIME_STDN] = 0;// init timer
					DCU->funct = &SSMEControllerSW::Shutdown_FailSafePneumatic;
					Shutdown_FailSafePneumatic_B();
					return;
				}
				else
				{
					DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				}
				break;
			case TMSQ:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::PostShutdown_TerminateSequence;
				PostShutdown_TerminateSequence_B();
				return;
			case COSY:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		StartPrep_PSN2_B();
		return;
	}

	void SSMEControllerSW_AD08::StartPrep_PSN3( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCT:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case IOHA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOHB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOLA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOLB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOSA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOSB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case MRC1:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case MRC2:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case PSN4:
				RotateCommand();
				// HACK for now stay 180s in PSN4 and then goto engine ready
				DCU->RAM[15] = DCU->RAM[RAM_AD08_TREF1];// TODO move to psn4_b, and make it like start/shutdown time counter?
				DCU->funct = &SSMEControllerSW::StartPrep_PSN4;
				StartPrep_PSN4_B();
				return;
			case SDEN:
				RotateCommand();
				DCU->RAM[RAM_AD08_SHUTDOWN_ENA] = 1;// TODO check for repeated command?
				break;
			case STDN:// TODO finish logic
				if (DCU->RAM[RAM_AD08_SHUTDOWN_ENA] == 1)
				{
					RotateCommand();
					DCU->RAM[RAM_AD08_TIME_STDN] = 0;// init timer
					DCU->funct = &SSMEControllerSW::Shutdown_FailSafePneumatic;
					Shutdown_FailSafePneumatic_B();
					return;
				}
				else
				{
					DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				}
				break;
			case TMSQ:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::PostShutdown_TerminateSequence;
				PostShutdown_TerminateSequence_B();
				return;
			case COSY:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		StartPrep_PSN3_B();
		return;
	}

	void SSMEControllerSW_AD08::StartPrep_PSN4( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCT:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case IOHA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOHB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOLA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOLB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOSA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOSB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case MRC1:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case MRC2:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case PSN3:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::StartPrep_PSN3;
				StartPrep_PSN3_B();
				return;
			case SDEN:
				RotateCommand();
				DCU->RAM[RAM_AD08_SHUTDOWN_ENA] = 1;// TODO check for repeated command?
				break;
			case STDN:// TODO finish logic
				if (DCU->RAM[RAM_AD08_SHUTDOWN_ENA] == 1)
				{
					RotateCommand();
					DCU->RAM[RAM_AD08_TIME_STDN] = 0;// init timer
					DCU->funct = &SSMEControllerSW::Shutdown_FailSafePneumatic;
					Shutdown_FailSafePneumatic_B();
					return;
				}
				else
				{
					DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				}
				break;
			case TMSQ:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::PostShutdown_TerminateSequence;
				PostShutdown_TerminateSequence_B();
				return;
			case COSY:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		StartPrep_PSN4_B();
		return;
	}

	void SSMEControllerSW_AD08::StartPrep_EngineReady( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCT:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case IOHA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOHB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOLA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOLB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOSA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOSB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case MRC1:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case MRC2:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case STEN:
				RotateCommand();
				DCU->RAM[RAM_AD08_START_ENA] = 1;// TODO check for repeated command?
				break;
				// TODO read below...
				/*Each controller normally requires two of three valid command paths
				from the GPC’s to control the SSME (start commands require three of three functional command
				paths). SSME controller software change RCN 4354 implemented in version OI-6 and subs added the
				capability for the engine to accept a shutdown enable/shutdown command pair on a single channel
				under special circumstances: an internal timer has expired (currently set at 512.86 seconds from engine
				start); limits have never been inhibited; the shutdown enable/shutdown command pair come in on the
				same channel, sequentially (with no other command in-between); and a valid command is not
				concurrently being received on the other two channels.*/
			case IGNT:
				if (DCU->RAM[RAM_AD08_START_ENA] == 1)
				{
					RotateCommand();
					DCU->RAM[RAM_AD08_TIME_ESC] = 0;// init timer
					DCU->funct = &SSMEControllerSW::Start_StartInitiation;
					Start_StartInitiation_B();
					return;
				}
				else
				{
					DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				}
				break;
			case SDEN:
				RotateCommand();
				DCU->RAM[RAM_AD08_SHUTDOWN_ENA] = 1;// TODO check for repeated command?
				break;
			case STDN:// TODO finish logic
				if (DCU->RAM[RAM_AD08_SHUTDOWN_ENA] == 1)
				{
					RotateCommand();
					DCU->RAM[RAM_AD08_TIME_STDN] = 0;// init timer
					DCU->funct = &SSMEControllerSW::Shutdown_FailSafePneumatic;
					Shutdown_FailSafePneumatic_B();
					return;
				}
				else
				{
					DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				}
				break;
			case TMSQ:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::PostShutdown_TerminateSequence;
				PostShutdown_TerminateSequence_B();
				return;
			case COSY:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		StartPrep_EngineReady_B();
		return;
	}

	void SSMEControllerSW_AD08::Start_StartInitiation( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		DCU->RAM[RAM_AD08_TIME_ESC] += (unsigned short)round( DCU->dt * 10000 );// increment time from ESC

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case SDEN:
				RotateCommand();
				DCU->RAM[RAM_AD08_SHUTDOWN_ENA] = 1;// TODO check for repeated command?
				break;
			case STDN:// TODO finish logic
				if (DCU->RAM[RAM_AD08_SHUTDOWN_ENA] == 1)
				{
					RotateCommand();
					DCU->RAM[RAM_AD08_TIME_STDN] = 0;// init timer
					DCU->funct = &SSMEControllerSW::Shutdown_FailSafePneumatic;
					Shutdown_FailSafePneumatic_B();
					return;
				}
				else
				{
					DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				}
				break;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		Start_StartInitiation_B();
		return;
	}

	void SSMEControllerSW_AD08::Start_ThrustBuildup( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		DCU->RAM[RAM_AD08_TIME_ESC] += (unsigned short)round( DCU->dt * 10000 );// increment time from ESC

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case SDEN:
				RotateCommand();
				DCU->RAM[RAM_AD08_SHUTDOWN_ENA] = 1;// TODO check for repeated command?
				break;
			case STDN:// TODO finish logic
				if (DCU->RAM[RAM_AD08_SHUTDOWN_ENA] == 1)
				{
					RotateCommand();
					DCU->RAM[RAM_AD08_TIME_STDN] = 0;// init timer
					DCU->funct = &SSMEControllerSW::Shutdown_FailSafePneumatic;
					Shutdown_FailSafePneumatic_B();
					return;
				}
				else
				{
					DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				}
				break;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		Start_ThrustBuildup_B();
		return;
	}

	void SSMEControllerSW_AD08::Mainstage_NormalControl( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case SDEN:
				RotateCommand();
				DCU->RAM[RAM_AD08_SHUTDOWN_ENA] = 1;// TODO check for repeated command?
				break;
			case STDN:// TODO finish logic
				if (DCU->RAM[RAM_AD08_SHUTDOWN_ENA] == 1)
				{
					RotateCommand();
					DCU->RAM[RAM_AD08_TIME_STDN] = 0;// init timer
					DCU->funct = &SSMEControllerSW::Shutdown_ThrottleTo0;
					Shutdown_ThrottleTo0_B();
					return;
				}
				else
				{
					DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				}
				break;
			case THRT:
				{
					double temp = (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0x03FF ) * 109) / 1023;
					if ((temp < 67) || (temp > 109))
					{
						DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
					}
					else
					{
						RotateCommand();
						DCU->RAM[RAM_AD08_PC_CMD] = (unsigned short)round( (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0x03FF ) * 2994) / 1023 );
					}
				}
				break;
			case ENLS:
				RotateCommand();
				break;
			case INLS:
				RotateCommand();
				break;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		Mainstage_NormalControl_B();
		return;
	}

	void SSMEControllerSW_AD08::Mainstage_FixedDensity( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		Mainstage_FixedDensity_B();
		return;
	}

	void SSMEControllerSW_AD08::Mainstage_ThrustLimiting( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		Mainstage_ThrustLimiting_B();
		return;
	}

	void SSMEControllerSW_AD08::Mainstage_HydraulicLockup( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		Mainstage_HydraulicLockup_B();
		return;
	}

	void SSMEControllerSW_AD08::Mainstage_ElectricalLockup( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		Mainstage_ElectricalLockup_B();
		return;
	}

	void SSMEControllerSW_AD08::Shutdown_ThrottleTo0( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		DCU->RAM[RAM_AD08_TIME_STDN] += (unsigned short)round( DCU->dt * 10000 );// increment time from STDN

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		Shutdown_ThrottleTo0_B();
		return;
	}

	void SSMEControllerSW_AD08::Shutdown_PropellantValvesClosed( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		Shutdown_PropellantValvesClosed_B();
		return;
	}

	void SSMEControllerSW_AD08::Shutdown_FailSafePneumatic( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		Shutdown_FailSafePneumatic_B();
		return;
	}

	void SSMEControllerSW_AD08::PostShutdown_Standby( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCT:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case IOHA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOHB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOLA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOLB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOSA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOSB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case MRC1:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case MRC2:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case LOXD:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::PostShutdown_OxidizerDump;
				PostShutdown_OxidizerDump_B();
				return;
			case XPOW:
				RotateCommand();
				break;
			case TMSQ:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::PostShutdown_TerminateSequence;
				PostShutdown_TerminateSequence_B();
				return;
			case COSY:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}
		
		PostShutdown_Standby_B();
		return;
	}

	void SSMEControllerSW_AD08::PostShutdown_OxidizerDump( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case IOHA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOHB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOLA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOLB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOSA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOSB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case MRC1:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case MRC2:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case TMSQ:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::PostShutdown_TerminateSequence;
				PostShutdown_TerminateSequence_B();
				return;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		PostShutdown_OxidizerDump_B();
		return;
	}

	void SSMEControllerSW_AD08::PostShutdown_TerminateSequence( void )
	{
		// get time
		GetTime();

		// get ch
		DCU->RAM[RAM_AD08_CH] = DCU->ch;

		// clear ESW 
		DCU->RAM[RAM_AD08_ESW] = 0;

		GetCommands();
		ValidateCommands();

		switch (GetMaskVal( DCU->RAM[RAM_AD08_VALIDCMD], 0xFC00 ))
		{
			case NOP:// do nothing
				break;
			case RVRC:
				RotateCommand();
				DCU->CIE->RestoreVRC();
				break;
			case SVRC:
				RotateCommand();
				DCU->CIE->SwitchVRC();
				break;
			case XFRT:
				RotateCommand();
				break;
			case RSCT:
				RotateCommand();
				DCU->funct = &SSMEControllerSW::Checkout_Standby;
				Checkout_Standby_B();
				return;
			case RSCA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case RSCB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				if (true)// TODO only accept if DCU is halted
				{
					//RotateCommand();
					DCU->funct = NULL;// go to PROM
					return;
				}
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
			case IOHA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOHB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOLA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOLB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case IOSA:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case IOSB:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			case MRC1:
				if (DCU->RAM[RAM_AD08_CH] == chB) break;
				RotateCommand();
				break;
			case MRC2:
				if (DCU->RAM[RAM_AD08_CH] == chA) break;
				RotateCommand();
				break;
			default:
				DCU->RAM[RAM_AD08_ESW] += ESW_Rejected;
				break;
		}

		PostShutdown_TerminateSequence_B();
		return;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	void SSMEControllerSW_AD08::Checkout_HydraulicConditioning_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Checkout + ESW_Hydraulic_Conditioning;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::Checkout_Standby_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Checkout + ESW_Standby;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::Checkout_ActuatorCheckout_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Checkout + ESW_Actuator_Checkout;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::Checkout_EngineLeakDetection_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Checkout + ESW_Engine_Leak_Detection;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::Checkout_IgniterCheckout_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Checkout + ESW_Igniter_Checkout;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::Checkout_PneumaticCheckout_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Checkout + ESW_Pneumatic_Checkout;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::Checkout_SensorCheckout_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Checkout + ESW_Sensor_Checkout;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::Checkout_ControllerCheckout_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Checkout + ESW_Controller_Checkout;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)//if (DCU->CIE->CheckWDTOpposite( 0 ) || DCU->CIE->CheckWDTOpposite( 1 ))
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::StartPrep_PSN1_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Start_Prep + ESW_PSN_1;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data

		// TODO purge Oxidizer System
		// TODO purge intermediate seal

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::StartPrep_PSN2_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Start_Prep + ESW_PSN_2;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data

		// TODO purge Fuel System + PSN1

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::StartPrep_PSN3_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Start_Prep + ESW_PSN_3;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data

		// TODO propellant recirculation
		// open bleed vlvs

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::StartPrep_PSN4_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Start_Prep + ESW_PSN_4;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data

		// open CCV now?
		DCU->RAM[RAM_AD08_CCV_CMD] = 4095;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_CCV_CMD] << 4) + DEV_OE_CCV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_CCV_CMD] << 4) + DEV_OE_CCV );

		DCU->RAM[RAM_AD08_MFV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_MFV_CMD] << 4) + DEV_OE_MFV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_MFV_CMD] << 4) + DEV_OE_MFV );
		
		DCU->RAM[RAM_AD08_MOV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_MOV_CMD] << 4) + DEV_OE_MOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_MOV_CMD] << 4) + DEV_OE_MOV );
		
		DCU->RAM[RAM_AD08_FPOV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_FPOV_CMD] << 4) + DEV_OE_FPOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_FPOV_CMD] << 4) + DEV_OE_FPOV );
		
		DCU->RAM[RAM_AD08_OPOV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_OPOV_CMD] << 4) + DEV_OE_OPOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_OPOV_CMD] << 4) + DEV_OE_OPOV );


		// TODO purge Fuel System after propellant drop
		// TODO when ready go to Engine Ready
		// i-seal he purge psn4+120
		// HPOTP  Minimum Redline: 175, Maximum Redline: 225
		// HPOTP/AT  Minimum Redline: 164, Maximum Redline: 225
		// fuel sys purge for 180sec every 60min

		// HACK for now stay 180s in PSN4 and then goto engine ready
		if (DCU->RAM[RAM_AD08_TREF1] - DCU->RAM[15] >= 180)
		{
			// engine ready
			DCU->funct = &SSMEControllerSW::StartPrep_EngineReady;
		}

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::StartPrep_EngineReady_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Start_Prep + ESW_Engine_Ready;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data

		// TODO if needed go back to psn4

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::Start_StartInitiation_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Start + ESW_Start_Initiation;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data
		
		// 2.4 CL thrust control (OPOV)
		// use PC_REF to reach PC_CMD

		DCU->RAM[RAM_AD08_PC_CMD] = 2747;

		if (DCU->RAM[RAM_AD08_TIME_ESC] > 30000)// TODO check ignition confirmed vals
		{// HACK for now stay here for 3 sec
			// go to Start phase ThrustBuildup mode
			DCU->funct = &SSMEControllerSW::Start_ThrustBuildup;
		}

		// run valve ignition schedules
		ValveSchedule( RAM_AD08_IGNT_CCV_POS, RAM_AD08_CCV_CMD, RAM_AD08_TIME_ESC, RAM_AD08_CCV_POS );
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_CCV_CMD] << 4) + DEV_OE_CCV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_CCV_CMD] << 4) + DEV_OE_CCV );

		ValveSchedule( RAM_AD08_IGNT_MFV_POS, RAM_AD08_MFV_CMD, RAM_AD08_TIME_ESC, RAM_AD08_MFV_POS );
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_MFV_CMD] << 4) + DEV_OE_MFV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_MFV_CMD] << 4) + DEV_OE_MFV );
		
		ValveSchedule( RAM_AD08_IGNT_MOV_POS, RAM_AD08_MOV_CMD, RAM_AD08_TIME_ESC, RAM_AD08_MOV_POS );
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_MOV_CMD] << 4) + DEV_OE_MOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_MOV_CMD] << 4) + DEV_OE_MOV );
		
		ValveSchedule( RAM_AD08_IGNT_FPOV_POS, RAM_AD08_FPOV_CMD, RAM_AD08_TIME_ESC, RAM_AD08_FPOV_POS );
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_FPOV_CMD] << 4) + DEV_OE_FPOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_FPOV_CMD] << 4) + DEV_OE_FPOV );
		
		ValveSchedule( RAM_AD08_IGNT_OPOV_POS, RAM_AD08_OPOV_CMD, RAM_AD08_TIME_ESC, RAM_AD08_OPOV_POS );
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_OPOV_CMD] << 4) + DEV_OE_OPOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_OPOV_CMD] << 4) + DEV_OE_OPOV );

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::Start_ThrustBuildup_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Start + ESW_Thrust_Buildup;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data

		// 3.8 CL mix control (FPOV)
		// use PC_REF to reach PC_CMD
		// check when PC_CMD achieved then go to mainstage

		DCU->RAM[RAM_AD08_PC_CMD] = 2747;

		if (DCU->RAM[RAM_AD08_TIME_ESC] > 50000)
		{// HACK for now stay here until esc + 5 sec
			// go to mainstage phase normalcontrol mode
			DCU->funct = &SSMEControllerSW::Mainstage_NormalControl;
			DCU->RAM[RAM_AD08_PC_REF] = 2747;// HACK use this here
		}

		// run valve ignition schedules
		ValveSchedule( RAM_AD08_IGNT_CCV_POS, RAM_AD08_CCV_CMD, RAM_AD08_TIME_ESC, RAM_AD08_CCV_POS );
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_CCV_CMD] << 4) + DEV_OE_CCV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_CCV_CMD] << 4) + DEV_OE_CCV );

		ValveSchedule( RAM_AD08_IGNT_MFV_POS, RAM_AD08_MFV_CMD, RAM_AD08_TIME_ESC, RAM_AD08_MFV_POS );
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_MFV_CMD] << 4) + DEV_OE_MFV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_MFV_CMD] << 4) + DEV_OE_MFV );
		
		ValveSchedule( RAM_AD08_IGNT_MOV_POS, RAM_AD08_MOV_CMD, RAM_AD08_TIME_ESC, RAM_AD08_MOV_POS );
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_MOV_CMD] << 4) + DEV_OE_MOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_MOV_CMD] << 4) + DEV_OE_MOV );
		
		ValveSchedule( RAM_AD08_IGNT_FPOV_POS, RAM_AD08_FPOV_CMD, RAM_AD08_TIME_ESC, RAM_AD08_FPOV_POS );
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_FPOV_CMD] << 4) + DEV_OE_FPOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_FPOV_CMD] << 4) + DEV_OE_FPOV );
		
		ValveSchedule( RAM_AD08_IGNT_OPOV_POS, RAM_AD08_OPOV_CMD, RAM_AD08_TIME_ESC, RAM_AD08_OPOV_POS );
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_OPOV_CMD] << 4) + DEV_OE_OPOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_OPOV_CMD] << 4) + DEV_OE_OPOV );

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::Mainstage_NormalControl_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Mainstage + ESW_Normal_Control;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data

		// rate limit
		// HACK using 10%/sec instead of 480lbs/msec
		if (DCU->RAM[RAM_AD08_PC_CMD] > DCU->RAM[RAM_AD08_PC_REF])// throttle up
		{
			DCU->RAM[RAM_AD08_PC_REF] += (unsigned short)round( DCU->dt * PC_100 * 10 );
			if (DCU->RAM[RAM_AD08_PC_REF] > DCU->RAM[RAM_AD08_PC_CMD]) DCU->RAM[RAM_AD08_PC_REF] = DCU->RAM[RAM_AD08_PC_CMD];
		}
		if (DCU->RAM[RAM_AD08_PC_CMD] < DCU->RAM[RAM_AD08_PC_REF])// throttle down
		{
			DCU->RAM[RAM_AD08_PC_REF] -= (unsigned short)round( DCU->dt * PC_100 * 10 );
			if (DCU->RAM[RAM_AD08_PC_REF] < DCU->RAM[RAM_AD08_PC_CMD]) DCU->RAM[RAM_AD08_PC_REF] = DCU->RAM[RAM_AD08_PC_CMD];
		}

		// CCV thrust command to position schedule
		double temp;
		DCU->RAM[RAM_AD08_CCV_CMD] = DCU->RAM[RAM_AD08_CCV_POS];
		if (DCU->RAM[RAM_AD08_PC_REF] >= PC_100 * 100)// 100%
		{
			if (DCU->RAM[RAM_AD08_CCV_POS] < 4095)// open
			{
				DCU->RAM[RAM_AD08_CCV_CMD] += (unsigned short)round( DCU->dt * 393.3657 );
				if (DCU->RAM[RAM_AD08_CCV_CMD] > 4095) DCU->RAM[RAM_AD08_CCV_CMD] = 4095;
			}
		}
		else
		{
			temp = (((31.7 * (DCU->RAM[RAM_AD08_PC_REF] / PC_100)) + 130) / 33) * 40.95;
			if (DCU->RAM[RAM_AD08_CCV_POS] < temp)// open
			{
				DCU->RAM[RAM_AD08_CCV_CMD] += (unsigned short)round( DCU->dt * 393.3657 );
				if (DCU->RAM[RAM_AD08_CCV_CMD] > temp) DCU->RAM[RAM_AD08_CCV_CMD] = (unsigned short)round( temp );
			}
			if (DCU->RAM[RAM_AD08_CCV_POS] > temp)// close
			{
				DCU->RAM[RAM_AD08_CCV_CMD] -= (unsigned short)round( DCU->dt * 393.3657 );
				if (DCU->RAM[RAM_AD08_CCV_CMD] < temp) DCU->RAM[RAM_AD08_CCV_CMD] = (unsigned short)round( temp );
			}
		}
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_CCV_CMD] << 4) + DEV_OE_CCV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_CCV_CMD] << 4) + DEV_OE_CCV );

		DCU->RAM[RAM_AD08_MFV_CMD] = 4095;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_MFV_CMD] << 4) + DEV_OE_MFV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_MFV_CMD] << 4) + DEV_OE_MFV );
		
		DCU->RAM[RAM_AD08_MOV_CMD] = 4095;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_MOV_CMD] << 4) + DEV_OE_MOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_MOV_CMD] << 4) + DEV_OE_MOV );
		
		//pFPOV = ((0.0035 * tgtpc * tgtpc) - (0.3168 * tgtpc) + 74.978) / 100;
		//rFPOV = 2.782;
		//DCU->RAM[RAM_AD08_FPOV_CMD] = 3206;// 78.298 * 40.95;
		DCU->RAM[RAM_AD08_FPOV_CMD] = DCU->RAM[RAM_AD08_FPOV_POS];
		temp = ((0.0035 * (DCU->RAM[RAM_AD08_PC_REF] / PC_100) * (DCU->RAM[RAM_AD08_PC_REF] / PC_100)) - (0.3168 * (DCU->RAM[RAM_AD08_PC_REF] / PC_100)) + 74.978) * 40.95;
		if (DCU->RAM[RAM_AD08_FPOV_POS] < temp)// open
		{
			DCU->RAM[RAM_AD08_FPOV_CMD] += (unsigned short)round( DCU->dt * 4095 * ((0.007 * (DCU->RAM[RAM_AD08_PC_REF] / PC_100)) - 0.3168) );
			if (DCU->RAM[RAM_AD08_FPOV_CMD] > temp) DCU->RAM[RAM_AD08_FPOV_CMD] = (unsigned short)round( temp );
		}
		if (DCU->RAM[RAM_AD08_FPOV_POS] > temp)// close
		{
			DCU->RAM[RAM_AD08_FPOV_CMD] -= (unsigned short)round( DCU->dt * 4095 * ((0.007 * (DCU->RAM[RAM_AD08_PC_REF] / PC_100)) - 0.3168) );
			if (DCU->RAM[RAM_AD08_FPOV_CMD] < temp) DCU->RAM[RAM_AD08_FPOV_CMD] = (unsigned short)round( temp );
		}
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_FPOV_CMD] << 4) + DEV_OE_FPOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_FPOV_CMD] << 4) + DEV_OE_FPOV );

		//pOPOV = ((0.0042 * tgtpc * tgtpc) - (0.4179 * tgtpc) + 63.587) / 100;
		//rOPOV = 2.961;
		//DCU->RAM[RAM_AD08_OPOV_CMD] = 2612;// 63.797 * 40.95;
		DCU->RAM[RAM_AD08_OPOV_CMD] = DCU->RAM[RAM_AD08_OPOV_POS];
		temp = DCU->RAM[RAM_AD08_PC_REF] / PC_100;
		temp = ((0.004 * temp * temp) - (0.3679 * temp) + 61.024) * 40.95;
		if (DCU->RAM[RAM_AD08_OPOV_POS] < temp)// open
		{
			DCU->RAM[RAM_AD08_OPOV_CMD] += (unsigned short)round( DCU->dt * 4095 * ((0.008 * (DCU->RAM[RAM_AD08_PC_REF] / PC_100)) - 0.3679) );
			if (DCU->RAM[RAM_AD08_OPOV_CMD] > temp) DCU->RAM[RAM_AD08_OPOV_CMD] = (unsigned short)round( temp );
		}
		if (DCU->RAM[RAM_AD08_OPOV_POS] > temp)// close
		{
			DCU->RAM[RAM_AD08_OPOV_CMD] -= (unsigned short)round( DCU->dt * 4095 * ((0.008 * (DCU->RAM[RAM_AD08_PC_REF] / PC_100)) - 0.3679) );
			if (DCU->RAM[RAM_AD08_OPOV_CMD] < temp) DCU->RAM[RAM_AD08_OPOV_CMD] = (unsigned short)round( temp );
		}
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_OPOV_CMD] << 4) + DEV_OE_OPOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_OPOV_CMD] << 4) + DEV_OE_OPOV );

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::Mainstage_FixedDensity_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Mainstage + ESW_Fixed_Density;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::Mainstage_ThrustLimiting_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Mainstage + ESW_Thrust_Limiting;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::Mainstage_HydraulicLockup_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Mainstage + ESW_Hydraulic_Lockup;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::Mainstage_ElectricalLockup_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Mainstage + ESW_Electrical_Lockup;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::Shutdown_ThrottleTo0_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Shutdown + ESW_Throttle_To_0;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		if (DCU->RAM[RAM_AD08_TIME_STDN] == 0) UpdateShutdownValveSchedule( (PC_100 * 100) - DCU->RAM[RAM_AD08_SENSOR_A + 8] );// TODO fix currentPC

		ValveSchedule( RAM_AD08_STDN_CCV_POS, RAM_AD08_CCV_CMD, RAM_AD08_TIME_STDN, RAM_AD08_CCV_POS );
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_CCV_CMD] << 4) + DEV_OE_CCV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_CCV_CMD] << 4) + DEV_OE_CCV );

		ValveSchedule( RAM_AD08_STDN_MFV_POS, RAM_AD08_MFV_CMD, RAM_AD08_TIME_STDN, RAM_AD08_MFV_POS );
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_MFV_CMD] << 4) + DEV_OE_MFV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_MFV_CMD] << 4) + DEV_OE_MFV );
		
		ValveSchedule( RAM_AD08_STDN_MOV_POS, RAM_AD08_MOV_CMD, RAM_AD08_TIME_STDN, RAM_AD08_MOV_POS );
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_MOV_CMD] << 4) + DEV_OE_MOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_MOV_CMD] << 4) + DEV_OE_MOV );
		
		ValveSchedule( RAM_AD08_STDN_FPOV_POS, RAM_AD08_FPOV_CMD, RAM_AD08_TIME_STDN, RAM_AD08_FPOV_POS );
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_FPOV_CMD] << 4) + DEV_OE_FPOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_FPOV_CMD] << 4) + DEV_OE_FPOV );
		
		ValveSchedule( RAM_AD08_STDN_OPOV_POS, RAM_AD08_OPOV_CMD, RAM_AD08_TIME_STDN, RAM_AD08_OPOV_POS );
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_OPOV_CMD] << 4) + DEV_OE_OPOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_OPOV_CMD] << 4) + DEV_OE_OPOV );

		if (DCU->RAM[RAM_AD08_CCV_POS] + DCU->RAM[RAM_AD08_MFV_POS] + DCU->RAM[RAM_AD08_MOV_POS] + DCU->RAM[RAM_AD08_FPOV_POS] + DCU->RAM[RAM_AD08_OPOV_POS] == 0)
		{
			// go to shutdown prop vlv closed
			DCU->funct = &SSMEControllerSW::Shutdown_PropellantValvesClosed;
		}

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::Shutdown_PropellantValvesClosed_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Shutdown + ESW_Propellant_Valves_Closed;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		// TODO go to p/s stby after purges
		DCU->funct = &SSMEControllerSW::PostShutdown_Standby;

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::Shutdown_FailSafePneumatic_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Shutdown + ESW_Fail_Safe_Pneumatic;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		// TODO close valves with PCA
		// HACK close all valves and do to p/s stby
		DCU->RAM[RAM_AD08_CCV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_CCV_CMD] << 4) + DEV_OE_CCV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_CCV_CMD] << 4) + DEV_OE_CCV );

		DCU->RAM[RAM_AD08_MFV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_MFV_CMD] << 4) + DEV_OE_MFV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_MFV_CMD] << 4) + DEV_OE_MFV );
		
		DCU->RAM[RAM_AD08_MOV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_MOV_CMD] << 4) + DEV_OE_MOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_MOV_CMD] << 4) + DEV_OE_MOV );
		
		DCU->RAM[RAM_AD08_FPOV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_FPOV_CMD] << 4) + DEV_OE_FPOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_FPOV_CMD] << 4) + DEV_OE_FPOV );
		
		DCU->RAM[RAM_AD08_OPOV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_OPOV_CMD] << 4) + DEV_OE_OPOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_OPOV_CMD] << 4) + DEV_OE_OPOV );

		DCU->funct = &SSMEControllerSW::PostShutdown_Standby;

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::PostShutdown_Standby_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Post_Shutdown + ESW_Standby;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::PostShutdown_OxidizerDump_B( void )
	{
		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Post_Shutdown + ESW_Oxidizer_Dump;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		DCU->RAM[RAM_AD08_CCV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_CCV_CMD] << 4) + DEV_OE_CCV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_CCV_CMD] << 4) + DEV_OE_CCV );

		DCU->RAM[RAM_AD08_MFV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_MFV_CMD] << 4) + DEV_OE_MFV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_MFV_CMD] << 4) + DEV_OE_MFV );
		
		// open MOV
		DCU->RAM[RAM_AD08_MOV_CMD] = 4095;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_MOV_CMD] << 4) + DEV_OE_MOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_MOV_CMD] << 4) + DEV_OE_MOV );
		
		DCU->RAM[RAM_AD08_FPOV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_FPOV_CMD] << 4) + DEV_OE_FPOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_FPOV_CMD] << 4) + DEV_OE_FPOV );
		
		DCU->RAM[RAM_AD08_OPOV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_OPOV_CMD] << 4) + DEV_OE_OPOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_OPOV_CMD] << 4) + DEV_OE_OPOV );

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	void SSMEControllerSW_AD08::PostShutdown_TerminateSequence_B( void )
	{
		//All valves are being closed while a purge or dump sequence is being terminated. All solenoid and servoswitch vales are then deenergized.

		// set phase/mode
		DCU->RAM[RAM_AD08_ESW] += ESW_Post_Shutdown + ESW_Terminate_Sequence;

		// reset own WDT
		DCU->CIE->ResetWDT( 0 );
		DCU->CIE->ResetWDT( 1 );

		// check other channel
		DCU->RAM[RAM_AD08_WDT1] = DCU->CIE->CheckWDTOpposite( 0 );
		DCU->RAM[RAM_AD08_WDT2] = DCU->CIE->CheckWDTOpposite( 1 );
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			DCU->RAM[RAM_AD08_ESW] += ESW_MCF;// MCF
			// TODO FID?
		}

		SensorInput();// TODO check IEs are working before asking data?

		// close any open vlvs, then goto p/s stby
		// TODO solenoid and igniters

		DCU->RAM[RAM_AD08_CCV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_CCV_CMD] << 4) + DEV_OE_CCV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_CCV_CMD] << 4) + DEV_OE_CCV );

		DCU->RAM[RAM_AD08_MFV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_MFV_CMD] << 4) + DEV_OE_MFV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_MFV_CMD] << 4) + DEV_OE_MFV );
		
		DCU->RAM[RAM_AD08_MOV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_MOV_CMD] << 4) + DEV_OE_MOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_MOV_CMD] << 4) + DEV_OE_MOV );
		
		DCU->RAM[RAM_AD08_FPOV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_FPOV_CMD] << 4) + DEV_OE_FPOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_FPOV_CMD] << 4) + DEV_OE_FPOV );
		
		DCU->RAM[RAM_AD08_OPOV_CMD] = 0;
		DCU->DIO_out( DEV_CIE_OEchA, (DCU->RAM[RAM_AD08_OPOV_CMD] << 4) + DEV_OE_OPOV );
		DCU->DIO_out( DEV_CIE_OEchB, (DCU->RAM[RAM_AD08_OPOV_CMD] << 4) + DEV_OE_OPOV );

		if (DCU->RAM[RAM_AD08_CCV_POS] + DCU->RAM[RAM_AD08_MFV_POS] + DCU->RAM[RAM_AD08_MOV_POS] + DCU->RAM[RAM_AD08_FPOV_POS] + DCU->RAM[RAM_AD08_OPOV_POS] == 0)
		{
			// go to p/s stby
			DCU->funct = &SSMEControllerSW::PostShutdown_Standby;
		}

		// built VRC
		BuiltVDT();

		// output VRC
		DCU->DMA_read( 1, RAM_AD08_VRC_1, 128 );
		DCU->DMA_read( 2, RAM_AD08_VRC_1, 128 );
		return;
	}

	////////////////////

	void SSMEControllerSW_AD08::GetTime( void )
	{
		DCU->RAM[RAM_AD08_TREF1] = (unsigned short)DCU->time;
		DCU->RAM[RAM_AD08_TREF2] = (unsigned short)((DCU->time - DCU->RAM[RAM_AD08_TREF1]) * 10000);
		return;
	}

	void SSMEControllerSW_AD08::BuiltVDT( void )
	{
		memset( &DCU->RAM[RAM_AD08_VRC_1], 0, 128 * sizeof(unsigned short) );// zeroing

		DCU->RAM[RAM_AD08_VRC_1] = DCU->RAM[RAM_AD08_TREF1];// TREF Word 1
		DCU->RAM[RAM_AD08_VRC_2] = DCU->RAM[RAM_AD08_TREF2];// TREF Word 2
		DCU->RAM[RAM_AD08_VRC_3] = DCU->RAM[RAM_AD08_CH];// ID
		DCU->RAM[RAM_AD08_VRC_4] = DCU->RAM[RAM_AD08_ESW];// Engine Status Word
		DCU->RAM[RAM_AD08_VRC_5] = 5;// FID Word
		
		if (DCU->RAM[RAM_AD08_CH] == chA)// HACK assuming IE and DCU always fail together
		{
			if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
			{
				// ch B failed
				DCU->RAM[RAM_AD08_VRC_6] = (unsigned short)(DCU->RAM[RAM_AD08_SENSOR_A + 8] + DCU->RAM[RAM_AD08_SENSOR_A + 9]) / 2;// MCC Press (qualified average)
			}
			else
			{
				DCU->RAM[RAM_AD08_VRC_6] = (unsigned short)(DCU->RAM[RAM_AD08_SENSOR_A + 8] + DCU->RAM[RAM_AD08_SENSOR_A + 9] + DCU->RAM[RAM_AD08_SENSOR_B + 8] + DCU->RAM[RAM_AD08_SENSOR_B + 9]) / 4;// MCC Press (qualified average)
			}
		}
		else
		{
			if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
			{
				// ch A failed
				DCU->RAM[RAM_AD08_VRC_6] = (unsigned short)(DCU->RAM[RAM_AD08_SENSOR_B + 8] + DCU->RAM[RAM_AD08_SENSOR_B + 9]) / 2;// MCC Press (qualified average)
			}
			else
			{
				DCU->RAM[RAM_AD08_VRC_6] = (unsigned short)(DCU->RAM[RAM_AD08_SENSOR_A + 8] + DCU->RAM[RAM_AD08_SENSOR_A + 9] + DCU->RAM[RAM_AD08_SENSOR_B + 8] + DCU->RAM[RAM_AD08_SENSOR_B + 9]) / 4;// MCC Press (qualified average)
			}
		}
		
		DCU->RAM[RAM_AD08_VRC_7] = 7;// Fuel Flowmeter (qualified average)

		//////////////////////////////////////////////////////////////////////////////////////////
		DCU->RAM[RAM_AD08_VRC_14] = DCU->RAM[330];// CCV Actuator cmd
		DCU->RAM[RAM_AD08_VRC_15] = DCU->RAM[331];// MFV Actuator cmd
		DCU->RAM[RAM_AD08_VRC_16] = DCU->RAM[332];// MOV Actuator cmd
		DCU->RAM[RAM_AD08_VRC_17] = DCU->RAM[333];// FPOV Actuator cmd
		DCU->RAM[RAM_AD08_VRC_18] = DCU->RAM[334];// OPOV Actuator cmd
		//////////////////////////////////////////////////////////////////////////////////////////
		DCU->RAM[RAM_AD08_VRC_19] = DCU->RAM[RAM_AD08_CCV_SH];// CCV Actuator cmd s&h
		DCU->RAM[RAM_AD08_VRC_20] = DCU->RAM[RAM_AD08_MFV_SH];// MFV Actuator cmd s&h
		DCU->RAM[RAM_AD08_VRC_21] = DCU->RAM[RAM_AD08_MOV_SH];// MOV Actuator cmd s&h
		DCU->RAM[RAM_AD08_VRC_22] = DCU->RAM[RAM_AD08_FPOV_SH];// FPOV Actuator cmd s&h
		DCU->RAM[RAM_AD08_VRC_23] = DCU->RAM[RAM_AD08_OPOV_SH];// OPOV Actuator cmd s&h
		//////////////////////////////////////////////////////////////////////////////////////////
		DCU->RAM[RAM_AD08_VRC_24] = DCU->RAM[RAM_AD08_CCV_POS];// CCV Actuator Pos
		DCU->RAM[RAM_AD08_VRC_25] = DCU->RAM[RAM_AD08_MFV_POS];// MFV Actuator Pos
		DCU->RAM[RAM_AD08_VRC_26] = DCU->RAM[RAM_AD08_MOV_POS];// MOV Actuator Pos
		DCU->RAM[RAM_AD08_VRC_27] = DCU->RAM[RAM_AD08_FPOV_POS];// FPOV Actuator Pos
		DCU->RAM[RAM_AD08_VRC_28] = DCU->RAM[RAM_AD08_OPOV_POS];// OPOV Actuator Pos

		DCU->RAM[RAM_AD08_VRC_41] = DCU->RAM[RAM_AD08_SENSOR_A + 8];// MCC Press Ch A1
		DCU->RAM[RAM_AD08_VRC_42] = DCU->RAM[RAM_AD08_SENSOR_A + 9];// MCC Press Ch A2
		DCU->RAM[RAM_AD08_VRC_43] = DCU->RAM[RAM_AD08_SENSOR_B + 8];// MCC Press Ch B1
		DCU->RAM[RAM_AD08_VRC_44] = DCU->RAM[RAM_AD08_SENSOR_B + 9];// MCC Press Ch B2

		DCU->RAM[RAM_AD08_VRC_45] = DCU->RAM[RAM_AD08_SENSOR_A + 11];// HPFT Disch Temp Ch A2
		DCU->RAM[RAM_AD08_VRC_46] = DCU->RAM[RAM_AD08_SENSOR_B + 11];// HPFT Disch Temp Ch B2
		DCU->RAM[RAM_AD08_VRC_47] = DCU->RAM[RAM_AD08_SENSOR_A + 13];// HPOT Disch Temp Ch A2
		DCU->RAM[RAM_AD08_VRC_48] = DCU->RAM[RAM_AD08_SENSOR_B + 13];// HPOT Disch Temp Ch B2

		DCU->RAM[RAM_AD08_VRC_64] = DCU->RAM[RAM_AD08_SENSOR_A + 12];// HPFT Disch Temp Ch A3

		DCU->RAM[RAM_AD08_VRC_69] = DCU->RAM[RAM_AD08_SENSOR_B + 12];// HPFT Disch Temp Ch B3

		/*
		74	FPB Purge Press
		75	OPB Purge Press
		*/

		DCU->RAM[RAM_AD08_VRC_90] = 90;// Inhibit Counter/PROM Rev.

		DCU->RAM[RAM_AD08_VRC_98] = DCU->RAM[RAM_AD08_CURCMD];// Current Command
		DCU->RAM[RAM_AD08_VRC_99] = DCU->RAM[RAM_AD08_PRVCMD];// Previous Command
		DCU->RAM[RAM_AD08_VRC_100] = 100;// FID/Delimiter

		DCU->RAM[RAM_AD08_VRC_103] = 103;// Parameter
		
		DCU->RAM[RAM_AD08_VRC_122] = DCU->RAM[RAM_AD08_SENSOR_A + 14];// HPOT Disch Temp Ch A3
		DCU->RAM[RAM_AD08_VRC_123] = DCU->RAM[RAM_AD08_SENSOR_B + 14];// HPOT Disch Temp Ch B3
		/*
		124	Fuel Flowrate Ch A2
		125	Fuel Flowrate Ch B2
		126	
		127	LPFP Disch Press Ch B
		128	LPFP Disch Temp Ch B
		*/
		DCU->RAM[RAM_AD08_VRC_128] = 128;
		return;
	}

	void SSMEControllerSW_AD08::RotateCommand( void )
	{
		DCU->RAM[RAM_AD08_PRVCMD] = DCU->RAM[RAM_AD08_CURCMD];
		DCU->RAM[RAM_AD08_CURCMD] = DCU->RAM[RAM_AD08_VALIDCMD];
		return;
	}

	void SSMEControllerSW_AD08::ValveSchedule( int tableaddr, int cmdaddr, int timeaddr, int currentposaddr )
	{
		int count = tableaddr;

		while (count <= tableaddr + 48)
		{
			if ((DCU->RAM[count] <= DCU->RAM[timeaddr]) && (DCU->RAM[count + 3] > DCU->RAM[timeaddr])) break;
			count += 3;
		}

		DCU->RAM[cmdaddr] = DCU->RAM[currentposaddr];
		if (DCU->RAM[count + 2] > DCU->RAM[timeaddr])
		{
			if (DCU->RAM[currentposaddr] < DCU->RAM[count + 1])// open
			{
				DCU->RAM[cmdaddr] += (unsigned short)round( DCU->dt * 10000 * ((double)(DCU->RAM[count + 1] - DCU->RAM[currentposaddr]) / (DCU->RAM[count + 2] - DCU->RAM[timeaddr])) );
				if (DCU->RAM[cmdaddr] > DCU->RAM[count + 1]) DCU->RAM[cmdaddr] = DCU->RAM[count + 1];
			}
			if (DCU->RAM[currentposaddr] > DCU->RAM[count + 1])// close
			{
				DCU->RAM[cmdaddr] -= (unsigned short)round( DCU->dt * 10000 * ((double)(DCU->RAM[currentposaddr] - DCU->RAM[count + 1]) / (DCU->RAM[count + 2] - DCU->RAM[timeaddr])) );
				if ((signed short)DCU->RAM[cmdaddr] < DCU->RAM[count + 1]) DCU->RAM[cmdaddr] = DCU->RAM[count + 1];
			}
		}
		else
		{
			DCU->RAM[cmdaddr] = DCU->RAM[count + 1];
		}
		return;
	}

	void SSMEControllerSW_AD08::LoadStartValveSchedule( void )
	{
		// HACK "load" ignition valve commands
		// space for 16 cmds per vlv
		// cmd time (from ESC, sec * 10000) / cmd pos / time to be at pos (from ESC, sec * 10000)

		// CCV
		DCU->RAM[RAM_AD08_IGNT_CCV_POS] = 0;
		DCU->RAM[RAM_AD08_IGNT_CCV_POS + 1] = 4095;// 100 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_CCV_POS + 2] = 1;

		DCU->RAM[RAM_AD08_IGNT_CCV_POS + 3] = 14700;
		DCU->RAM[RAM_AD08_IGNT_CCV_POS + 4] = 2867;// 70 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_CCV_POS + 5] = 17800;

		DCU->RAM[RAM_AD08_IGNT_CCV_POS + 6] = 24000;
		DCU->RAM[RAM_AD08_IGNT_CCV_POS + 7] = 2088;// 51 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_CCV_POS + 8] = 26000;

		DCU->RAM[RAM_AD08_IGNT_CCV_POS + 9] = 28500;
		DCU->RAM[RAM_AD08_IGNT_CCV_POS + 10] = 4095;// 100 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_CCV_POS + 11] = 37200;

		DCU->RAM[RAM_AD08_IGNT_CCV_POS + 12] = 65535;// final (max) time

		// MFV
		DCU->RAM[RAM_AD08_IGNT_MFV_POS] = 0;
		DCU->RAM[RAM_AD08_IGNT_MFV_POS + 1] = 4095;// 100 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_MFV_POS + 2] = 7100;

		DCU->RAM[RAM_AD08_IGNT_MFV_POS + 3] = 65535;

		// MOV
		DCU->RAM[RAM_AD08_IGNT_MOV_POS] = 0;
		DCU->RAM[RAM_AD08_IGNT_MOV_POS + 1] = 0;// 0 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_MOV_POS + 2] = 1;

		DCU->RAM[RAM_AD08_IGNT_MOV_POS + 3] = 2000;
		DCU->RAM[RAM_AD08_IGNT_MOV_POS + 4] = 2416;// 59 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_MOV_POS + 5] = 12900;

		DCU->RAM[RAM_AD08_IGNT_MOV_POS + 6] = 24900;
		DCU->RAM[RAM_AD08_IGNT_MOV_POS + 7] = 4095;// 100 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_MOV_POS + 8] = 39400;

		DCU->RAM[RAM_AD08_IGNT_MOV_POS + 9] = 65535;

		// FPOV
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS] = 0;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 1] = 0;// 0 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 2] = 1;

		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 3] = 1000;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 4] = 2293;// 56 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 5] = 4290;

		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 6] = 7200;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 7] = 1925;// 47 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 8] = 7900;

		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 9] = 7900;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 10] = 2211;// 54 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 11] = 9500;

		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 12] = 11500;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 13] = 2088;// 51 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 14] = 12100;

		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 15] = 15200;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 16] = 1966;// 48 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 17] = 17700;// ?????

		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 18] = 17700;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 19] = 2170;// 53 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 20] = 19300;

		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 21] = 20500;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 22] = 2048;// 50 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 23] = 21400;

		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 24] = 24200;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 25] = 2318;// 56.6 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 26] = 24800;

		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 27] = 24800;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 28] = 2375;// 58 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 29] = 27700;

		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 30] = 27700;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 31] = 2744;// 67 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 32] = 33800;

		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 33] = 33800;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 34] = 3112;// 76 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 35] = 37500;

		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 36] = 37500;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 37] = 3206;// 78.298 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 38] = 43000;

		DCU->RAM[RAM_AD08_IGNT_FPOV_POS + 39] = 65535;

		// OPOV
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS] = 0;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 1] = 0;// 0 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 2] = 1;

		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 3] = 1200;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 4] = 1147;// 28 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 5] = 3000;

		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 6] = 3000;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 7] = 1556;// 38 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 8] = 7400;

		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 9] = 7400;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 10] = 2150;// 52.5 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 11] = 9000;

		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 12] = 10800;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 13] = 1720;// 42 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 14] = 11600;

		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 15] = 11600;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 16] = 1638;// 40 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 17] = 15500;

		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 18] = 15500;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 19] = 1536;// 37.5 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 20] = 16000;

		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 21] = 17700;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 22] = 1392;// 34 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 23] = 20900;

		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 24] = 20900;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 25] = 1802;// 44 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 26] = 21800;

		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 27] = 24000;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 28] = 1966;// 48 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 29] = 24600;

		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 30] = 24600;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 31] = 2048;// 50 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 32] = 28400;

		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 33] = 28400;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 34] = 2334;// 57 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 35] = 33700;

		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 36] = 33700;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 37] = 2580;// 63 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 38] = 37900;

		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 39] = 37900;
		//DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 40] = 2612;// 63.797 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 40] = 2630;// 64.234 * 40.95;
		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 41] = 42900;

		DCU->RAM[RAM_AD08_IGNT_OPOV_POS + 42] = 65535;
		return;
	}

	void SSMEControllerSW_AD08::LoadShutdownValveSchedule( void )
	{
		// HACK "load" shutdown valve commands (from 100%)
		// space for 16 cmds per vlv
		// cmd time (from STDN, sec * 10000) / cmd pos / time to be at pos (from STDN, sec * 10000)

		// CCV
		DCU->RAM[RAM_AD08_STDN_CCV_POS] = 0;
		DCU->RAM[RAM_AD08_STDN_CCV_POS + 1] = 2848;// 69.545 * 40.95;
		DCU->RAM[RAM_AD08_STDN_CCV_POS + 2] = 3260;

		DCU->RAM[RAM_AD08_STDN_CCV_POS + 3] = 6050;
		DCU->RAM[RAM_AD08_STDN_CCV_POS + 4] = 3065;// 74.848 * 40.95;
		DCU->RAM[RAM_AD08_STDN_CCV_POS + 5] = 7090;

		DCU->RAM[RAM_AD08_STDN_CCV_POS + 6] = 45930;
		DCU->RAM[RAM_AD08_STDN_CCV_POS + 7] = 0;// 0 * 40.95;
		DCU->RAM[RAM_AD08_STDN_CCV_POS + 8] = 53140;

		DCU->RAM[RAM_AD08_STDN_CCV_POS + 9] = 65535;// final (max) time

		// MFV
		DCU->RAM[RAM_AD08_STDN_MFV_POS] = 0;
		DCU->RAM[RAM_AD08_STDN_MFV_POS + 1] = 4095;// 100 * 40.95;
		DCU->RAM[RAM_AD08_STDN_MFV_POS + 2] = 1;

		DCU->RAM[RAM_AD08_STDN_MFV_POS + 3] = 13290;
		DCU->RAM[RAM_AD08_STDN_MFV_POS + 4] = 2515;// 61.406 * 40.95;
		DCU->RAM[RAM_AD08_STDN_MFV_POS + 5] = 35180;

		DCU->RAM[RAM_AD08_STDN_MFV_POS + 6] = 45590;
		DCU->RAM[RAM_AD08_STDN_MFV_POS + 7] = 0;// 0 * 40.95;
		DCU->RAM[RAM_AD08_STDN_MFV_POS + 8] = 54000;

		DCU->RAM[RAM_AD08_STDN_MFV_POS + 9] = 65535;// final (max) time

		// MOV
		DCU->RAM[RAM_AD08_STDN_MOV_POS] = 0;
		DCU->RAM[RAM_AD08_STDN_MOV_POS + 1] = 3852;// 94.063 * 40.95;
		DCU->RAM[RAM_AD08_STDN_MOV_POS + 2] = 3680;

		DCU->RAM[RAM_AD08_STDN_MOV_POS + 3] = 3680;
		DCU->RAM[RAM_AD08_STDN_MOV_POS + 4] = 0;// 0 * 40.95;
		DCU->RAM[RAM_AD08_STDN_MOV_POS + 5] = 27200;

		DCU->RAM[RAM_AD08_STDN_MOV_POS + 6] = 65535;// final (max) time

		// FPOV
		DCU->RAM[RAM_AD08_STDN_FPOV_POS] = 0;
		DCU->RAM[RAM_AD08_STDN_FPOV_POS + 1] = 2438;// 59.545 * 40.95;
		DCU->RAM[RAM_AD08_STDN_FPOV_POS + 2] = 7130;

		DCU->RAM[RAM_AD08_STDN_FPOV_POS + 3] = 7130;
		DCU->RAM[RAM_AD08_STDN_FPOV_POS + 4] = 1576;// 38.485 * 40.95;
		DCU->RAM[RAM_AD08_STDN_FPOV_POS + 5] = 18620;

		DCU->RAM[RAM_AD08_STDN_FPOV_POS + 6] = 18620;
		DCU->RAM[RAM_AD08_STDN_FPOV_POS + 7] = 0;// 0 * 40.95;
		DCU->RAM[RAM_AD08_STDN_FPOV_POS + 8] = 23620;

		DCU->RAM[RAM_AD08_STDN_FPOV_POS + 9] = 65535;// final (max) time

		// OPOV
		DCU->RAM[RAM_AD08_STDN_OPOV_POS] = 0;
		DCU->RAM[RAM_AD08_STDN_OPOV_POS + 1] = 2091;// 51.06 * 40.95;
		DCU->RAM[RAM_AD08_STDN_OPOV_POS + 2] = 2990;

		DCU->RAM[RAM_AD08_STDN_OPOV_POS + 3] = 2990;
		DCU->RAM[RAM_AD08_STDN_OPOV_POS + 4] = 695;// 16.97 * 40.95;
		DCU->RAM[RAM_AD08_STDN_OPOV_POS + 5] = 14250;

		DCU->RAM[RAM_AD08_STDN_OPOV_POS + 6] = 14250;
		DCU->RAM[RAM_AD08_STDN_OPOV_POS + 7] = 0;// 0 * 40.95;
		DCU->RAM[RAM_AD08_STDN_OPOV_POS + 8] = 15060;

		DCU->RAM[RAM_AD08_STDN_OPOV_POS + 9] = 65535;// final (max) time
		return;
	}
	
	void SSMEControllerSW_AD08::UpdateShutdownValveSchedule( double dPC )
	{
		// CCV
		//DCU->RAM[RAM_AD08_STDN_CCV_POS] = 0;
		DCU->RAM[RAM_AD08_STDN_CCV_POS + 1] -= (unsigned short)round( 0.55825 * dPC );// TODO only do this under 100%
		DCU->RAM[RAM_AD08_STDN_CCV_POS + 2] -= (unsigned short)round( 1.42094 * dPC );

		DCU->RAM[RAM_AD08_STDN_CCV_POS + 3] -= (unsigned short)round( 2.72494 * dPC );
		DCU->RAM[RAM_AD08_STDN_CCV_POS + 4] += (unsigned short)round( 0.00687 * dPC );
		DCU->RAM[RAM_AD08_STDN_CCV_POS + 5] -= (unsigned short)round( 1.19147 * dPC );

		DCU->RAM[RAM_AD08_STDN_CCV_POS + 6] -= (unsigned short)round( 2.96765 * dPC );
		//DCU->RAM[RAM_AD08_STDN_CCV_POS + 7] = 0;
		DCU->RAM[RAM_AD08_STDN_CCV_POS + 8] -= (unsigned short)round( 2.86836 * dPC );

		//DCU->RAM[RAM_AD08_STDN_CCV_POS + 9] = 65535;

		// MFV
		//DCU->RAM[RAM_AD08_STDN_MFV_POS] = 0;
		//DCU->RAM[RAM_AD08_STDN_MFV_POS + 1] = 4095;
		//DCU->RAM[RAM_AD08_STDN_MFV_POS + 2] = 1;

		DCU->RAM[RAM_AD08_STDN_MFV_POS + 3] += (unsigned short)round( 0.69503 * dPC );
		DCU->RAM[RAM_AD08_STDN_MFV_POS + 4] += (unsigned short)round( 0.00709 * dPC );
		DCU->RAM[RAM_AD08_STDN_MFV_POS + 5] -= (unsigned short)round( 13.47015 * dPC );

		DCU->RAM[RAM_AD08_STDN_MFV_POS + 6] -= (unsigned short)round( 2.43811 * dPC );
		//DCU->RAM[RAM_AD08_STDN_MFV_POS + 7] = 0;
		DCU->RAM[RAM_AD08_STDN_MFV_POS + 8] -= (unsigned short)round( 2.92352 * dPC );

		//DCU->RAM[RAM_AD08_STDN_MFV_POS + 9] = 65535;

		// MOV
		//DCU->RAM[RAM_AD08_STDN_MOV_POS] = 0;
		DCU->RAM[RAM_AD08_STDN_MOV_POS + 1] += (unsigned short)round( 0.0141 * dPC );
		DCU->RAM[RAM_AD08_STDN_MOV_POS + 2] -= (unsigned short)round( 1.22457 * dPC );

		DCU->RAM[RAM_AD08_STDN_MOV_POS + 3] -= (unsigned short)round( 1.22457 * dPC );
		//DCU->RAM[RAM_AD08_STDN_MOV_POS + 4] = 0;
		DCU->RAM[RAM_AD08_STDN_MOV_POS + 5] += (unsigned short)round( 0.08605 * dPC );

		//DCU->RAM[RAM_AD08_STDN_MOV_POS + 6] = 65535;

		// FPOV
		//DCU->RAM[RAM_AD08_STDN_FPOV_POS] = 0;
		DCU->RAM[RAM_AD08_STDN_FPOV_POS + 1] += (unsigned short)round( 0.04174 * dPC );
		DCU->RAM[RAM_AD08_STDN_FPOV_POS + 2] -= (unsigned short)round( 5.18512 * dPC );

		DCU->RAM[RAM_AD08_STDN_FPOV_POS + 3] -= (unsigned short)round( 5.18512 * dPC );
		DCU->RAM[RAM_AD08_STDN_FPOV_POS + 4] += (unsigned short)round( 0.02611 * dPC );
		DCU->RAM[RAM_AD08_STDN_FPOV_POS + 5] -= (unsigned short)round( 5.33957 * dPC );

		DCU->RAM[RAM_AD08_STDN_FPOV_POS + 6] -= (unsigned short)round( 5.33957 * dPC );
		//DCU->RAM[RAM_AD08_STDN_FPOV_POS + 7] = 0;
		DCU->RAM[RAM_AD08_STDN_FPOV_POS + 8] -= (unsigned short)round( 5.48299 * dPC );

		//DCU->RAM[RAM_AD08_STDN_FPOV_POS + 9] = 65535;

		// OPOV
		//DCU->RAM[RAM_AD08_STDN_OPOV_POS] = 0;
		DCU->RAM[RAM_AD08_STDN_OPOV_POS + 1] -= (unsigned short)round( 0.28285 * dPC );
		//DCU->RAM[RAM_AD08_STDN_OPOV_POS + 2] = 2990;

		//DCU->RAM[RAM_AD08_STDN_OPOV_POS + 3] = 2990;
		DCU->RAM[RAM_AD08_STDN_OPOV_POS + 4] -= (unsigned short)round( 0.31488 * dPC );
		DCU->RAM[RAM_AD08_STDN_OPOV_POS + 5] -= (unsigned short)round( 0.51851 * dPC );

		DCU->RAM[RAM_AD08_STDN_OPOV_POS + 6] -= (unsigned short)round( 0.51851 * dPC );
		//DCU->RAM[RAM_AD08_STDN_OPOV_POS + 7] = 0;
		DCU->RAM[RAM_AD08_STDN_OPOV_POS + 8] -= (unsigned short)round( 0.81638 * dPC );

		//DCU->RAM[RAM_AD08_STDN_OPOV_POS + 9] = 65535;
		return;
	}

	void SSMEControllerSW_AD08::PowerFailureSense( void )
	{
		// TODO power failure sense

		// check if other ch down
		if ((DCU->RAM[RAM_AD08_WDT1] + DCU->RAM[RAM_AD08_WDT2]) != 0)
		{
			// emergency pneumatic shutdown
			DCU->Controller->PCA();
		}
		return;
	}
}
