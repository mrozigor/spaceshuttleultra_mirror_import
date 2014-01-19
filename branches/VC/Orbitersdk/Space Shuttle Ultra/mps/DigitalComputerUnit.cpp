#include "DigitalComputerUnit.h"
#include "ComputerInterfaceElectronics.h"
#include "MPSdefs.h"


namespace mps
{
	DigitalComputerUnit::DigitalComputerUnit( int ch, int ramsize, const std::string& sw, SSMEController* Controller )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " DigitalComputerUnit::DigitalComputerUnit in || ch:%d|ramsize:%d|sw:%s", ch, ramsize, sw.c_str() );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

		funct = NULL;
		this->ramsize = ramsize;
		RAM = new unsigned short[ramsize]();

		this->ch = ch;
		this->Controller = Controller;

		// HACK improve this
		/*if (!strcmp( sw.c_str(), "AD06" ))
		this->sw = new SSMEControllerSW_AD08( this );
		else if (!strcmp( sw.c_str(), "AD07" ))
		this->sw = new SSMEControllerSW_AD08( this );
		else if (!strcmp( sw.c_str(), "AD08" ))
		this->sw = new SSMEControllerSW_AD08( this );
		else*/
		this->sw = new SSMEControllerSW_AD08( this );

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " DigitalComputerUnit::DigitalComputerUnit out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	DigitalComputerUnit::~DigitalComputerUnit( void )
	{
		delete[] RAM;
		delete sw;
		return;
	}

	void DigitalComputerUnit::OnSaveState( FILEHANDLE scn ) const
	{
		//int count = 0;
		char cbuf[255];
		//char cbuf_ch[255];

		sprintf_s( cbuf, 255, "DCU_ch%c SW", ch + 65 );
		if (funct == NULL)// PROM
		{
			oapiWriteScenario_int( scn, cbuf, 0 );
		}
		else if (funct == &SSMEControllerSW::Checkout_HydraulicConditioning)
		{
			oapiWriteScenario_int( scn, cbuf, 1 );
		}
		else if (funct == &SSMEControllerSW::Checkout_Standby)
		{
			oapiWriteScenario_int( scn, cbuf, 2 );
		}
		else if (funct == &SSMEControllerSW::Checkout_ActuatorCheckout)
		{
			oapiWriteScenario_int( scn, cbuf, 3 );
		}
		else if (funct == &SSMEControllerSW::Checkout_EngineLeakDetection)
		{
			oapiWriteScenario_int( scn, cbuf, 4 );
		}
		else if (funct == &SSMEControllerSW::Checkout_IgniterCheckout)
		{
			oapiWriteScenario_int( scn, cbuf, 5 );
		}
		else if (funct == &SSMEControllerSW::Checkout_PneumaticCheckout)
		{
			oapiWriteScenario_int( scn, cbuf, 6 );
		}
		else if (funct == &SSMEControllerSW::Checkout_SensorCheckout)
		{
			oapiWriteScenario_int( scn, cbuf, 7 );
		}
		else if (funct == &SSMEControllerSW::Checkout_ControllerCheckout)
		{
			oapiWriteScenario_int( scn, cbuf, 8 );
		}
		else if (funct == &SSMEControllerSW::StartPrep_PSN1)
		{
			oapiWriteScenario_int( scn, cbuf, 9 );
		}
		else if (funct == &SSMEControllerSW::StartPrep_PSN2)
		{
			oapiWriteScenario_int( scn, cbuf, 10 );
		}
		else if (funct == &SSMEControllerSW::StartPrep_PSN3)
		{
			oapiWriteScenario_int( scn, cbuf, 11 );
		}
		else if (funct == &SSMEControllerSW::StartPrep_PSN4)
		{
			oapiWriteScenario_int( scn, cbuf, 12 );
		}
		else if (funct == &SSMEControllerSW::StartPrep_EngineReady)
		{
			oapiWriteScenario_int( scn, cbuf, 13 );
		}
		else if (funct == &SSMEControllerSW::Start_StartInitiation)
		{
			oapiWriteScenario_int( scn, cbuf, 14 );
		}
		else if (funct == &SSMEControllerSW::Start_ThrustBuildup)
		{
			oapiWriteScenario_int( scn, cbuf, 15 );
		}
		else if (funct == &SSMEControllerSW::Mainstage_NormalControl)
		{
			oapiWriteScenario_int( scn, cbuf, 16 );
		}
		else if (funct == &SSMEControllerSW::Mainstage_FixedDensity)
		{
			oapiWriteScenario_int( scn, cbuf, 17 );
		}
		else if (funct == &SSMEControllerSW::Mainstage_ThrustLimiting)
		{
			oapiWriteScenario_int( scn, cbuf, 18 );
		}
		else if (funct == &SSMEControllerSW::Mainstage_HydraulicLockup)
		{
			oapiWriteScenario_int( scn, cbuf, 19 );
		}
		else if (funct == &SSMEControllerSW::Mainstage_ElectricalLockup)
		{
			oapiWriteScenario_int( scn, cbuf, 20 );
		}
		else if (funct == &SSMEControllerSW::Shutdown_ThrottleTo0)
		{
			oapiWriteScenario_int( scn, cbuf, 21 );
		}
		else if (funct == &SSMEControllerSW::Shutdown_PropellantValvesClosed)
		{
			oapiWriteScenario_int( scn, cbuf, 22 );
		}
		else if (funct == &SSMEControllerSW::Shutdown_FailSafePneumatic)
		{
			oapiWriteScenario_int( scn, cbuf, 23 );
		}
		else if (funct == &SSMEControllerSW::PostShutdown_Standby)
		{
			oapiWriteScenario_int( scn, cbuf, 24 );
		}
		else if (funct == &SSMEControllerSW::PostShutdown_OxidizerDump)
		{
			oapiWriteScenario_int( scn, cbuf, 25 );
		}
		else if (funct == &SSMEControllerSW::PostShutdown_TerminateSequence)
		{
			oapiWriteScenario_int( scn, cbuf, 26 );
		}

		/*sprintf_s( cbuf_ch, 255, "DCU_ch%c RAM", ch + 65 );
		while (count < 18)//this->ramsize)
		{// ^^ try to save only what's needed
			if (RAM[count])
			{
				sprintf_s( cbuf, 255, "%d %d", count, RAM[count] );
				oapiWriteScenario_string( scn, cbuf_ch, cbuf );
			}
			count++;
		}*/

		__OnSaveState( scn );// write derived class

		// TODO call sw??
		return;
	}

	bool DigitalComputerUnit::OnParseLine( const char* line )
	{
		int read_i1 = 0;
		int read_i2 = 0;
		char cbuf_ch[255];
#ifdef _MPSDEBUG
		char buffer[150];
#endif// _MPSDEBUG

		sprintf_s( cbuf_ch, 255, "DCU_ch%c", ch + 65 );

		if (!_strnicmp( line, cbuf_ch, 7 ))
		{
			if (!_strnicmp( line + 8, "SW", 2 ))
			{
				sscanf_s( line + 10, "%d", &read_i1 );

				switch (read_i1)
				{
					case 0:// PROM
						funct = NULL;
						break;
					case 1:
						funct = &SSMEControllerSW::Checkout_HydraulicConditioning;
						break;
					case 2:
						funct = &SSMEControllerSW::Checkout_Standby;
						break;
					case 3:
						funct = &SSMEControllerSW::Checkout_ActuatorCheckout;
						break;
					case 4:
						funct = &SSMEControllerSW::Checkout_EngineLeakDetection;
						break;
					case 5:
						funct = &SSMEControllerSW::Checkout_IgniterCheckout;
						break;
					case 6:
						funct = &SSMEControllerSW::Checkout_PneumaticCheckout;
						break;
					case 7:
						funct = &SSMEControllerSW::Checkout_SensorCheckout;
						break;
					case 8:
						funct = &SSMEControllerSW::Checkout_ControllerCheckout;
						break;
					case 9:
						funct = &SSMEControllerSW::StartPrep_PSN1;
						break;
					case 10:
						funct = &SSMEControllerSW::StartPrep_PSN2;
						break;
					case 11:
						funct = &SSMEControllerSW::StartPrep_PSN3;
						break;
					case 12:
						funct = &SSMEControllerSW::StartPrep_PSN4;
						break;
					case 13:
						funct = &SSMEControllerSW::StartPrep_EngineReady;
						break;
					case 14:
						funct = &SSMEControllerSW::Start_StartInitiation;
						break;
					case 15:
						funct = &SSMEControllerSW::Start_ThrustBuildup;
						break;
					case 16:
						funct = &SSMEControllerSW::Mainstage_NormalControl;
						break;
					case 17:
						funct = &SSMEControllerSW::Mainstage_FixedDensity;
						break;
					case 18:
						funct = &SSMEControllerSW::Mainstage_ThrustLimiting;
						break;
					case 19:
						funct = &SSMEControllerSW::Mainstage_HydraulicLockup;
						break;
					case 20:
						funct = &SSMEControllerSW::Mainstage_ElectricalLockup;
						break;
					case 21:
						funct = &SSMEControllerSW::Shutdown_ThrottleTo0;
						break;
					case 22:
						funct = &SSMEControllerSW::Shutdown_PropellantValvesClosed;
						break;
					case 23:
						funct = &SSMEControllerSW::Shutdown_FailSafePneumatic;
						break;
					case 24:
						funct = &SSMEControllerSW::PostShutdown_Standby;
						break;
					case 25:
						funct = &SSMEControllerSW::PostShutdown_OxidizerDump;
						break;
					case 26:
						funct = &SSMEControllerSW::PostShutdown_TerminateSequence;
						break;
				}
#ifdef _MPSDEBUG
				sprintf_s( buffer, 150, " DigitalComputerUnit::OnParseLine || %s SW:%d", cbuf_ch, read_i1 );
				oapiWriteLog( buffer );
#endif// _MPSDEBUG
				return true;
			}
			/*else if (!_strnicmp( line + 24, "RAM", 3 ))
			{
				sscanf_s( line + 27, "%d %d", &read_i1, &read_i2 );
				RAM[read_i1] = read_i2;
#ifdef _MPSDEBUG
				sprintf_s( buffer, 150, " DigitalComputerUnit::OnParseLine || %s RAM:%d|%d", cbuf_ch, read_i1, read_i2 );
				oapiWriteLog( buffer );
#endif// _MPSDEBUG
				return true;
			}*/
		}
		if (__OnParseLine( line )) return true;// check if derived class wants line
		// TODO call sw??
		return false;
	}

	void DigitalComputerUnit::GiveRefs( ComputerInterfaceElectronics* CIE )
	{
		this->CIE = CIE;
		return;
	}

	unsigned short DigitalComputerUnit::DIO_in( unsigned short device )
	{
		return CIE->DIO( device /*+ 0x0000*/, 0 );
	}

	void DigitalComputerUnit::DIO_out( unsigned short device, unsigned short data )
	{
		CIE->DIO( device + 0x8000, data );
		return;
	}

	void DigitalComputerUnit::DMA_write( unsigned short selectcontrol, unsigned short writeaddress, unsigned short writelength )
	{
		CIE->DMA_ch3( selectcontrol, writeaddress, writelength );
		return;
	}

	void DigitalComputerUnit::DMA_read( unsigned short selectcontrol, unsigned short readaddress, unsigned short readlength )
	{
		if (selectcontrol == 1)// assume it's 1 or 2 only
		{
			CIE->DMA_ch1( selectcontrol, readaddress, readlength );
		}
		else
		{
			CIE->DMA_ch2( selectcontrol, readaddress, readlength );
		}
		return;
	}

	void DigitalComputerUnit::PowerFailureSense( void )
	{
		if (sw != NULL) sw->PowerFailureSense();
		return;
	}
}