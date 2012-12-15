#include "InputElectronics.h"
#include "SSME.h"
#include <UltraMath.h>
#include "MPSdefs.h"


namespace mps
{
	InputElectronics::InputElectronics( int ch, SSME* eng, SSMEController* Controller, int numP, int numT, int numS, int numF, int FD_P, int FD_T, int FD_S, int FD_F )
	{
#ifdef _MPSDEBUG
		char buffer[150];
		sprintf_s( buffer, 150, " InputElectronics::InputElectronics in || ch:%d|numP:%d|numT:%d|numS:%d|numF:%d|FD_P:%d|FD_T:%d|FD_S:%d|FD_F:%d", ch, numP, numT, numS, numF, FD_P, FD_T, FD_S, FD_F );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

		this->eng = eng;
		this->ch = ch;
		this->Controller = Controller;
		this->numP = numP;
		this->numT = numT;
		this->numS = numS;
		this->numF = numF;
		this->FD_P = FD_P;
		this->FD_T = FD_T;
		this->FD_S = FD_S;
		this->FD_F = FD_F;

		press = new double[numP];
		temp = new double[numT];
		speed = new double[numS];
		flow = new double[numF];
		actpos = new double[5];
		samplehold = new double[5];

#ifdef _MPSDEBUG
		sprintf_s( buffer, 150, " InputElectronics::InputElectronics out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	InputElectronics::~InputElectronics( void )
	{
		delete[] press;
		delete[] temp;
		delete[] speed;
		delete[] flow;
		delete[] actpos;
		delete[] samplehold;
		return;
	}

	void InputElectronics::OnSaveState( FILEHANDLE scn ) const
	{
		__OnSaveState( scn );// write derived class
		return;
	}

	bool InputElectronics::OnParseLine( const char* line )
	{
		if (__OnParseLine( line )) return true;// check if derived class wants line
		return false;
	}

	void InputElectronics::CopySensorData( int count, int startsource, int startdest, double* source, double* dest )
	{
		int countA = startdest;
		int countB = startsource;

		while (countB < count)
		{
			dest[countA] = source[countB];
			countA++;
			countB += 2;
		}
		return;
	}

	void InputElectronics::GetData( unsigned short device, unsigned short* data )
	{
		// TODO move to derived class?
		if (GetMaskVal( device, 0x0F00 ) == 0x0F00)// assume it's only 0x0F00 or 0x0000
		{
			// A/D data
			switch (GetMaskVal( device, 0x00FF ))// TODO update to only use 10b, create sensor and ADC classes
			{
				case 0x0000:// P1
					*data = (unsigned short)round( press[0] );
					break;
				case 0x0001:// P2
					*data = (unsigned short)round( press[1] );
					break;
				case 0x0002:// P3
					*data = (unsigned short)round( press[2] );
					break;
				case 0x0003:// P4
					*data = (unsigned short)round( press[3] );
					break;
				case 0x0004:// P5
					*data = (unsigned short)round( press[4] );
					break;
				case 0x0005:// P6
					*data = (unsigned short)round( press[5] );
					break;
				case 0x0006:// P7
					*data = (unsigned short)round( press[6] );
					break;
				case 0x0007:// P8
					*data = (unsigned short)round( press[7] );
					break;
				case 0x0008:// P9
					*data = (unsigned short)round( press[8] );
					break;
				case 0x0009:// P10
					*data = (unsigned short)round( press[9] );
					break;
				case 0x000A:// P11
					*data = (unsigned short)round( press[10] );
					break;
				case 0x000B:// P12
					*data = (unsigned short)round( press[11] );
					break;
				case 0x000C:// P13
					*data = (unsigned short)round( press[12] );
					break;
				case 0x000D:// P14
					*data = (unsigned short)round( press[13] );
					break;
				case 0x000E:// P15
					*data = (unsigned short)round( press[14] );
					break;
				case 0x000F:// T1
					*data = (unsigned short)round( temp[0] );
					break;
				case 0x0010:// T2
					*data = (unsigned short)round( temp[1] );
					break;
				case 0x0011:// T3
					*data = (unsigned short)round( temp[2] );
					break;
				case 0x0012:// T4
					*data = (unsigned short)round( temp[3] );
					break;
				case 0x0013:// T5
					*data = (unsigned short)round( temp[4] );
					break;
				case 0x0014:// T6
					*data = (unsigned short)round( temp[5] );
					break;
				case 0x0015:// T7
					*data = (unsigned short)round( temp[6] );
					break;
				case 0x0016:// T8
					*data = (unsigned short)round( temp[7] );
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
				case 0x001C:// SH1
					*data = (unsigned short)round( samplehold[0] * 4095 );
					break;
				case 0x001D:// SH2
					*data = (unsigned short)round( samplehold[1] * 4095 );
					break;
				case 0x001E:// SH3
					*data = (unsigned short)round( samplehold[2] * 4095 );
					break;
				case 0x001F:// SH4
					*data = (unsigned short)round( samplehold[3] * 4095 );
					break;
				case 0x0020:// SH5
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