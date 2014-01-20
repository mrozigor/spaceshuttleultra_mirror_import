#include "InputElectronics.h"
#include "SSME.h"
#include "SSMEController.h"
#include "PowerSupplyElectronics.h"
#include <UltraMath.h>
#include "MPSdefs.h"


namespace mps
{
	InputElectronics::InputElectronics( int ch, SSMEController* Controller )
	{
#ifdef _MPSDEBUG
		char buffer[150];
		sprintf_s( buffer, 150, " InputElectronics::InputElectronics in || ch:%d", ch );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

		this->ch = ch;
		this->Controller = Controller;

#ifdef _MPSDEBUG
		sprintf_s( buffer, 150, " InputElectronics::InputElectronics out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	InputElectronics::~InputElectronics( void )
	{
		return;
	}

	void InputElectronics::OnSaveState( FILEHANDLE scn ) const
	{
		__OnSaveState( scn );// write derived class
		return;
	}

	bool InputElectronics::OnParseLine( const char* line )
	{
		return __OnParseLine( line );// check if derived class wants line
	}

	void InputElectronics::Realize( DiscreteBundle* Press, DiscreteBundle* Temp, DiscreteBundle* Flow, DiscreteBundle* Speed )
	{
		for (int i = 0; i < 15; i++)
		{
			press[i].Connect( Press, i );
		}

		for (int i = 0; i < 8; i++)
		{
			temp[i].Connect( Temp, i );
		}

		for (int i = 0; i < 4; i++)
		{
			flow[i].Connect( Flow, i );
		}

		for (int i = 0; i < 3; i++)
		{
			speed[i].Connect( Speed, i );
		}

		PSE = Controller->PSE[ch];
		OE = Controller->OE[ch];
		return;
	}

	void InputElectronics::GetData( unsigned short device, unsigned short* data )
	{
		// check power supply
		if (PSE->Power() == false) return;

		// TODO move to derived class?
		if (GetMaskVal( device, 0x0F00 ) == 0x0F00)// HACK assume it's only 0x0F00 or 0x0000
		{
			// A/D data
			switch (GetMaskVal( device, 0x00FF ))// TODO update to only use 10b, create ADC class
			{
				case 0x0000:// P1
					*data = (unsigned short)round( press[0].GetVoltage() * 1000 );
					break;
				case 0x0001:// P2
					*data = (unsigned short)round( press[1].GetVoltage() * 1000 );
					break;
				case 0x0002:// P3
					*data = (unsigned short)round( press[2].GetVoltage() * 1000 );
					break;
				case 0x0003:// P4
					*data = (unsigned short)round( press[3].GetVoltage() * 1000 );
					break;
				case 0x0004:// P5
					*data = (unsigned short)round( press[4].GetVoltage() * 1000 );
					break;
				case 0x0005:// P6
					*data = (unsigned short)round( press[5].GetVoltage() * 1000 );
					break;
				case 0x0006:// P7
					*data = (unsigned short)round( press[6].GetVoltage() * 1000 );
					break;
				case 0x0007:// P8
					*data = (unsigned short)round( press[7].GetVoltage() * 1000 );
					break;
				case 0x0008:// P9
					*data = (unsigned short)round( press[8].GetVoltage() * 1000 );
					break;
				case 0x0009:// P10
					*data = (unsigned short)round( press[9].GetVoltage() * 1000 );
					break;
				case 0x000A:// P11
					*data = (unsigned short)round( press[10].GetVoltage() * 1000 );
					break;
				case 0x000B:// P12
					*data = (unsigned short)round( press[11].GetVoltage() * 1000 );
					break;
				case 0x000C:// P13
					*data = (unsigned short)round( press[12].GetVoltage() * 1000 );
					break;
				case 0x000D:// P14
					*data = (unsigned short)round( press[13].GetVoltage() * 1000 );
					break;
				case 0x000E:// P15
					*data = (unsigned short)round( press[14].GetVoltage() * 1000 );
					break;
				case 0x000F:// T1
					*data = (unsigned short)round( temp[0].GetVoltage() * 490 );
					break;
				case 0x0010:// T2
					*data = (unsigned short)round( temp[1].GetVoltage() * 490 );
					break;
				case 0x0011:// T3
					*data = (unsigned short)round( temp[2].GetVoltage() * 490 );
					break;
				case 0x0012:// T4
					*data = (unsigned short)round( temp[3].GetVoltage() * 490 );
					break;
				case 0x0013:// T5
					*data = (unsigned short)round( temp[4].GetVoltage() * 490 );
					break;
				case 0x0014:// T6
					*data = (unsigned short)round( temp[5].GetVoltage() * 490 );
					break;
				case 0x0015:// T7
					*data = (unsigned short)round( temp[6].GetVoltage() * 490 );
					break;
				case 0x0016:// T8
					*data = (unsigned short)round( temp[7].GetVoltage() * 490 );
					break;
				case 0x0017:// AP1
					*data = (unsigned short)round( actpos[0] * 4095 );
					break;
				case 0x0018:// AP2
					*data = (unsigned short)round( actpos[1] * 4095 );
					break;
				case 0x0019:// AP3
					*data = (unsigned short)round( actpos[2] * 4095 );
					break;
				case 0x001A:// AP4
					*data = (unsigned short)round( actpos[3] * 4095 );
					break;
				case 0x001B:// AP5
					*data = (unsigned short)round( actpos[4] * 4095 );
					break;
				case 0x001C:// AP6
					*data = (unsigned short)round( actpos[5] * 4095 );
					break;
				case 0x001D:// AP7
					*data = (unsigned short)round( actpos[6] * 4095 );
					break;
				case 0x001E:// AP8
					*data = (unsigned short)round( actpos[7] * 4095 );
					break;
				case 0x001F:// AP9
					*data = (unsigned short)round( actpos[8] * 4095 );
					break;
				case 0x0020:// SH1
					*data = (unsigned short)round( samplehold[0] * 4095 );
					break;
				case 0x0021:// SH2
					*data = (unsigned short)round( samplehold[1] * 4095 );
					break;
				case 0x0022:// SH3
					*data = (unsigned short)round( samplehold[2] * 4095 );
					break;
				case 0x0023:// SH4
					*data = (unsigned short)round( samplehold[3] * 4095 );
					break;
				case 0x0024:// SH5
					*data = (unsigned short)round( samplehold[4] * 4095 );
					break;
			}
		}
		else
		{
			// PRC data
			switch (GetMaskVal( device, 0x00FF ))
			{
				case 0x0000:// F1
					*data = (unsigned short)flow[0];
					break;
				case 0x0001:// F2
					*data = (unsigned short)flow[1];
					break;
				case 0x0002:// F3
					*data = (unsigned short)flow[2];
					break;
				case 0x0003:// F4
					*data = (unsigned short)flow[3];
					break;
				case 0x0004:// S1
					*data = (unsigned short)speed[0];
					break;
				case 0x0005:// S2
					*data = (unsigned short)speed[1];
					break;
				case 0x0006:// S3
					*data = (unsigned short)speed[2];
					break;
			}
		}
		return;
	}
}