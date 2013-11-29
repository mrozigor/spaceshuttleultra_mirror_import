#include "OutputElectronics.h"
#include "MPSdefs.h"


namespace mps
{
	OutputElectronics::OutputElectronics( int ch, SSME* eng, SSMEController* Controller )
	{
#ifdef _MPSDEBUG
		char buffer[100];	
		sprintf_s( buffer, 100, " OutputElectronics::OutputElectronics in" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

		this->eng = eng;
		this->ch = ch;
		this->Controller = Controller;
		time = 0;
		StorageRegister = 0;
		ONOFFCommandRegister[0] = 0;
		ONOFFCommandRegister[1] = 0;
		SH[0] = 0;
		SH[1] = 0;
		SH[2] = 0;
		SH[3] = 0;
		SH[4] = 0;

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " OutputElectronics::OutputElectronics out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	OutputElectronics::~OutputElectronics( void )
	{
		return;
	}

	void OutputElectronics::OnSaveState( FILEHANDLE scn ) const
	{
		char cbuf[255];
		char cbuf_ch[255];

		sprintf_s( cbuf_ch, 255, "OE_ch%c StorageRegister", ch + 65 );
		oapiWriteScenario_int( scn, cbuf_ch, StorageRegister );

		sprintf_s( cbuf_ch, 255, "OE_ch%c ONOFFCommandRegister", ch + 65 );
		sprintf_s( cbuf, 255, "%d %d", ONOFFCommandRegister[0], ONOFFCommandRegister[1] );
		oapiWriteScenario_string( scn, cbuf_ch, cbuf );

		sprintf_s( cbuf_ch, 255, "OE_ch%c SH", ch + 65 );
		sprintf_s( cbuf, 255, "%lf %lf %lf %lf %lf", SH[0], SH[1], SH[2], SH[3], SH[4] );
		oapiWriteScenario_string( scn, cbuf_ch, cbuf );

		__OnSaveState( scn );// write derived class
		return;
	}

	bool OutputElectronics::OnParseLine( const char* line )
	{
		int read_i1 = 0;
		int read_i2 = 0;
		double read_f1 = 0;
		double read_f2 = 0;
		double read_f3 = 0;
		double read_f4 = 0;
		double read_f5 = 0;
		char cbuf_ch[255];
#ifdef _MPSDEBUG
		char buffer[150];
#endif// _MPSDEBUG

		sprintf_s( cbuf_ch, 255, "OE_ch%c", ch + 65 );

		if (!_strnicmp( line, cbuf_ch, 6 ))
		{
			if (!_strnicmp( line + 7, "StorageRegister", 15 ))
			{
				sscanf_s( line + 22, "%d", &read_i1 );
				StorageRegister = read_i1;
#ifdef _MPSDEBUG
				sprintf_s( buffer, 150, " OutputElectronics::OnParseLine || %s StorageRegister:%d", cbuf_ch, StorageRegister );
				oapiWriteLog( buffer );
#endif// _MPSDEBUG
				return true;
			}
			else if (!_strnicmp( line + 7, "ONOFFCommandRegister", 20 ))
			{
				sscanf_s( line + 27, "%d %d", &read_i1, &read_i2 );
				ONOFFCommandRegister[0] = read_i1;
				ONOFFCommandRegister[1] = read_i2;
#ifdef _MPSDEBUG
				sprintf_s( buffer, 150, " OutputElectronics::OnParseLine || %s ONOFFCommandRegister:%d|%d", cbuf_ch, ONOFFCommandRegister[0], ONOFFCommandRegister[1] );
				oapiWriteLog( buffer );
#endif// _MPSDEBUG
				return true;
			}
			else if (!_strnicmp( line + 7, "SH", 2 ))
			{
				sscanf_s( line + 9, "%lf %lf %lf %lf %lf", &read_f1, &read_f2, &read_f3, &read_f4, &read_f5 );
				SH[0] = read_f1;
				SH[1] = read_f2;
				SH[2] = read_f3;
				SH[3] = read_f4;
				SH[4] = read_f5;
#ifdef _MPSDEBUG
				sprintf_s( buffer, 150, " OutputElectronics::OnParseLine || %s SH:%lf|%lf|%lf|%lf|%lf", cbuf_ch, SH[0], SH[1], SH[2], SH[3], SH[4] );
				oapiWriteLog( buffer );
#endif// _MPSDEBUG
				return true;
			}
		}
		if (__OnParseLine( line )) return true;// check if derived class wants line
		return false;
	}
}