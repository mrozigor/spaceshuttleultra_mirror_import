#include "ComputerInterfaceElectronics.h"
#include "VehicleInterfaceElectronics.h"
#include "DigitalComputerUnit.h"
#include "InputElectronics.h"
#include "SSMEController.h"
#include "MPSdefs.h"


namespace mps
{
	ComputerInterfaceElectronics::ComputerInterfaceElectronics( int ch, SSMEController* Controller )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " ComputerInterfaceElectronics::ComputerInterfaceElectronics in || ch:%d", ch );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

		this->ch = ch;
		this->Controller = Controller;

		dWDT[0] = 0;
		dWDT[1] = 0;
		bWDT[0] = true;// start timed out
		bWDT[1] = true;

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " ComputerInterfaceElectronics::ComputerInterfaceElectronics out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	ComputerInterfaceElectronics::~ComputerInterfaceElectronics( void )
	{
		return;
	}

	void ComputerInterfaceElectronics::OnSaveState( FILEHANDLE scn ) const
	{
		char cbuf[255];
		char cbuf_ch[255];

		/*sprintf_s( cbuf_ch, 255, "ComputerInterfaceElectronics_ch%d dWDT", ch );
		sprintf_s( cbuf, 255, "%lf %lf", dWDT[0], dWDT[1] );
		oapiWriteScenario_string( scn, cbuf_ch, cbuf );*/

		sprintf_s( cbuf_ch, 255, "CIE_ch%c bWDT", ch + 65 );
		sprintf_s( cbuf, 255, "%d %d", bWDT[0], bWDT[1] );
		oapiWriteScenario_string( scn, cbuf_ch, cbuf );

		__OnSaveState( scn );// write derived class
		return;
	}

	bool ComputerInterfaceElectronics::OnParseLine( const char* line )
	{
		int read_i1 = 0;
		int read_i2 = 0;
		double read_f1 = 0;
		double read_f2 = 0;
		char cbuf_ch[255];
#ifdef _MPSDEBUG
		char buffer[150];
#endif// _MPSDEBUG

		sprintf_s( cbuf_ch, 255, "CIE_ch%c", ch + 65 );

		if (!_strnicmp( line, cbuf_ch, 7 ))
		{
			/*if (!_strnicmp( line + 8, "dWDT", 4 ))
			{
				sscanf_s( line + 12, "%lf %lf", &read_f1, &read_f2 );
				dWDT[0] = read_f1;
				dWDT[1] = read_f2;
#ifdef _MPSDEBUG
				sprintf_s( buffer, 150, " ComputerInterfaceElectronics::OnParseLine || %s dWDT:%lf|%lf", cbuf_ch, dWDT[0], dWDT[1] );
				oapiWriteLog( buffer );
#endif// _MPSDEBUG
				return true;
			}
			else */if (!_strnicmp( line + 8, "bWDT", 4 ))
			{
				sscanf_s( line + 12, "%d %d", &read_i1, &read_i2 );
				bWDT[0] = GetBoolFromInt( read_i1 );
				bWDT[1] = GetBoolFromInt( read_i2 );
#ifdef _MPSDEBUG
				sprintf_s( buffer, 150, " ComputerInterfaceElectronics::OnParseLine || %s bWDT:%d|%d", cbuf_ch, bWDT[0], bWDT[1] );
				oapiWriteLog( buffer );
#endif// _MPSDEBUG
				return true;
			}
		}
		if (__OnParseLine( line )) return true;// check if derived class wants line
		return false;
	}

	void ComputerInterfaceElectronics::tmestp( double time, double dt )
	{
		this->time = time;
		this->dt = dt;
		return;
	}

	void ComputerInterfaceElectronics::GiveRefs( DigitalComputerUnit* DCU, InputElectronics* IEchA, InputElectronics* IEchB, ComputerInterfaceElectronics* CIEOpposite )
	{
		this->DCU = DCU;
		this->IE[chA] = IEchA;
		this->IE[chB] = IEchB;
		this->CIEOpposite = CIEOpposite;
		return;
	}

	void ComputerInterfaceElectronics::DMA_ch1( unsigned short selectcontrol, unsigned short readaddress, unsigned short readlength )
	{// TODO remove selectcontrol here?
		// read data to VIE
		// data recorder A
		unsigned short data[128];
		int count = 0;

		while (readlength > 0)// HACK assume readlength is always 128
		{
			data[count] = DCU->RAM[readaddress];
			readlength--;
			readaddress++;
			count++;
		}
		Controller->VIE_RecorderDataConverter_chA( data, ch );
		return;
	}

	void ComputerInterfaceElectronics::DMA_ch2( unsigned short selectcontrol, unsigned short readaddress, unsigned short readlength )
	{// TODO remove selectcontrol here?
		// read data to VIE
		// data recorder B
		unsigned short data[128];
		int count = 0;

		while (readlength > 0)// HACK assume readlength is always 128
		{
			data[count] = DCU->RAM[readaddress];
			readlength--;
			readaddress++;
			count++;
		}
		Controller->VIE_RecorderDataConverter_chB( data, ch );
		return;
	}

	void ComputerInterfaceElectronics::DMA_ch3( unsigned short selectcontrol, unsigned short writeaddress, unsigned short writelength )
	{
		// write data from IE
		unsigned short data;
		int IEch = GetMaskVal( selectcontrol, 0xF000 ) >> 13;// TODO check bounds

		while (writelength > 0)
		{
			IE[IEch]->GetData( selectcontrol, &data );
			DCU->RAM[writeaddress] = data;
			writelength--;
			writeaddress++;
			selectcontrol++;
		}
		return;
	}

	unsigned short ComputerInterfaceElectronics::DIO( unsigned short control, unsigned short data )
	{
		switch (GetMaskVal( control, 0x8000 ))
		{
			case DIO_DEV_INPUT:
				switch (GetMaskVal( control, 0x7000 ))
				{
					case DIO_DEV_OEchA:
						switch (GetMaskVal( control, 0x0F00 ))
						{
							case DIO_DEV_OE_STORAGE_REG:
								return Controller->OE_StorageRegister_read( chA );
							case DIO_DEV_OE_ONOFF_REG_1:
								return Controller->OE_ONOFFCommandRegister_read( chA, 1 );
							case DIO_DEV_OE_ONOFF_REG_2:
								return Controller->OE_ONOFFCommandRegister_read( chA, 2 );
							default:
								char buffer[100];
								sprintf_s( buffer, 100, " ComputerInterfaceElectronics::DIO || DIO_DEV_OEchA || %d %d", control, data );
								oapiWriteLog( buffer );
						}
						break;
					case DIO_DEV_OEchB:
						switch (GetMaskVal( control, 0x0F00 ))
						{
							case DIO_DEV_OE_STORAGE_REG:
								return Controller->OE_StorageRegister_read( chB );
							case DIO_DEV_OE_ONOFF_REG_1:
								return Controller->OE_ONOFFCommandRegister_read( chB, 1 );
							case DIO_DEV_OE_ONOFF_REG_2:
								return Controller->OE_ONOFFCommandRegister_read( chB, 2 );
							default:
								char buffer[100];
								sprintf_s( buffer, 100, " ComputerInterfaceElectronics::DIO || DIO_DEV_OEchB || %d %d", control, data );
								oapiWriteLog( buffer );
						}
						break;
					case DIO_DEV_VIE:
						switch (GetMaskVal( control, 0x0F00 ))
						{
							case DIO_DEV_VIE_CMD1:
								return Controller->VIE_CommandDataConverter_read( 1 );
							case DIO_DEV_VIE_CMD2:
								return Controller->VIE_CommandDataConverter_read( 2 );
							case DIO_DEV_VIE_CMD3:
								return Controller->VIE_CommandDataConverter_read( 3 );
							default:
								char buffer[100];
								sprintf_s( buffer, 100, " ComputerInterfaceElectronics::DIO || DIO_DEV_VIE || %d %d", control, data );
								oapiWriteLog( buffer );
						}
						break;
					default:
						char buffer[100];
						sprintf_s( buffer, 100, " ComputerInterfaceElectronics::DIO || DIO_DEV_INPUT || %d %d", control, data );
						oapiWriteLog( buffer );
				}
				break;
			case DIO_DEV_OUTPUT:
				switch (GetMaskVal( control, 0x7000 ))
				{
					case DIO_DEV_OEchA:
						Controller->OE_StorageRegister_write( chA, data, ch );
						break;
					case DIO_DEV_OEchB:
						Controller->OE_StorageRegister_write( chB, data, ch );
						break;
					default:
						char buffer[100];
						sprintf_s( buffer, 100, " ComputerInterfaceElectronics::DIO || DIO_DEV_OUTPUT || %d %d", control, data );
						oapiWriteLog( buffer );
				}
				break;
			default:
				char buffer[100];
				sprintf_s( buffer, 100, " ComputerInterfaceElectronics::DIO || %d %d", control, data );
				oapiWriteLog( buffer );
		}
		return 0;
	}

	void ComputerInterfaceElectronics::InitializeWDT( int nWDT )
	{// TODO check bounds
		// TODO use both DCU and OE
		bWDT[nWDT] = false;
		dWDT[nWDT] = time;
		return;
	}

	void ComputerInterfaceElectronics::ResetWDT( int nWDT )
	{// TODO check bounds
		dWDT[nWDT] = time;
		return;
	}

	bool ComputerInterfaceElectronics::CheckWDTOwn( int nWDT )// TODO improve WDTs
	{// TODO check bounds
		if (bWDT[nWDT] == false)
		{
			if (time == dWDT[nWDT]) 
			{
				return false;
			}
			else
			{
				bWDT[nWDT] = true;
				return true;
			}
		}
		return true;
	}

	bool ComputerInterfaceElectronics::CheckWDTOpposite( int nWDT )
	{// TODO check bounds
		if (ch == chA)
		{
			if (CIEOpposite->bWDT[nWDT] == false)
			{
				if ((time - CIEOpposite->dWDT[nWDT]) > (dt + 0.00001))// add something due to uncertainties
				{
					CIEOpposite->bWDT[nWDT] = true;
					return true;
				}
				else
				{
					return false;
				}
			}
			return true;
		}
		else
		{
			if (CIEOpposite->bWDT[nWDT] == false)
			{
				if ((CIEOpposite->dWDT[nWDT] - time) > (0 + 0.00001))// add something due to uncertainties
				{
					CIEOpposite->bWDT[nWDT] = true;
					return true;
				}
				else
				{
					return false;
				}
			}
			return true;
		}
	}

	void ComputerInterfaceElectronics::SwitchVRC( void )
	{
		Controller->VIE_SwitchVRC();
		return;
	}

	void ComputerInterfaceElectronics::RestoreVRC( void )
	{
		Controller->VIE_RestoreVRC();
		return;
	}
}