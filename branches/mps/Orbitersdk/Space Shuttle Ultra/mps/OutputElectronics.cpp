#include "OutputElectronics.h"
#include "SSMEController.h"
#include "PowerSupplyElectronics.h"
#include "MPSdefs.h"


namespace mps
{
	OutputElectronics::OutputElectronics( int ch, SSME* eng, SSMEController* Controller )
	{
#ifdef _MPSDEBUG
		char buffer[100];	
		sprintf_s( buffer, 100, " OutputElectronics::OutputElectronics in || ch:%d", ch );
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

		FuelSystemPurge_SV = new DiscOutPort;
		BleedValvesControl_SV = new DiscOutPort;
		EmergencyShutdown_SV = new DiscOutPort;
		ShutdownPurge_SV = new DiscOutPort;
		HPOTPISPurge_SV = new DiscOutPort;
		AFV_SV = new DiscOutPort;
		HPV_SV = new DiscOutPort;

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " OutputElectronics::OutputElectronics out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	OutputElectronics::~OutputElectronics( void )
	{
		delete FuelSystemPurge_SV;
		delete BleedValvesControl_SV;
		delete EmergencyShutdown_SV;
		delete ShutdownPurge_SV;
		delete HPOTPISPurge_SV;
		return;
	}

	void OutputElectronics::OnSaveState( FILEHANDLE scn ) const
	{
		__OnSaveState( scn );// write derived class
		return;
	}

	bool OutputElectronics::OnParseLine( const char* line )
	{
		return __OnParseLine( line );// check if derived class wants line
	}

	void OutputElectronics::Realize( discsignals::DiscreteBundle* bundle )
	{
		PSE = Controller->PSE[ch];
		CIE[chA] = Controller->CIE[chA];
		CIE[chB] = Controller->CIE[chB];

		FuelSystemPurge_SV->Connect( bundle, 0 + ch );
		BleedValvesControl_SV->Connect( bundle, 2 + ch );
		EmergencyShutdown_SV->Connect( bundle, 4 + ch );
		ShutdownPurge_SV->Connect( bundle, 6 + ch );
		HPOTPISPurge_SV->Connect( bundle, 8 + ch );
		AFV_SV->Connect( bundle, 10 + ch );
		HPV_SV->Connect( bundle, 12 + ch );
		return;
	}
}