#include "PowerSupplyElectronics.h"
#include "SSMEController.h"
#include "DigitalComputerUnit.h"
#include "ComputerInterfaceElectronics.h"
#include "MPSdefs.h"


namespace mps
{
	PowerSupplyElectronics::PowerSupplyElectronics( int ch, SSMEController* Controller )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " PowerSupplyElectronics::PowerSupplyElectronics in" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

		this->ch = ch;
		this->Controller = Controller;
		PowerOn = true;

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " PowerSupplyElectronics::PowerSupplyElectronics out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	PowerSupplyElectronics::~PowerSupplyElectronics( void )
	{
		return;
	}

	void PowerSupplyElectronics::OnSaveState( FILEHANDLE scn ) const
	{
		__OnSaveState( scn );// write derived class
		return;
	}

	bool PowerSupplyElectronics::OnParseLine( const char* line )
	{
		return __OnParseLine( line );// check if derived class wants line
	}

	void PowerSupplyElectronics::Realize( void )
	{
		DCU = Controller->DCU[ch];
		if (ch == chA) CIEOpposite = Controller->CIE[chB];
		else CIEOpposite = Controller->CIE[chA];
		return;
	}

	void PowerSupplyElectronics::tmestp( double time )
	{
		if (AC.IsSet() == false)
		{
			if (PowerOn == true)
			{
				// power failure on the way
				DCU->PowerFailureSense();
				CIEOpposite->PowerBusDown();
			}
		}
		PowerOn = AC.IsSet();
		return;
	}

	void PowerSupplyElectronics::ConnectPower( DiscreteBundle* AC )
	{
		this->AC.Connect( AC, ch );
		return;
	}

	bool PowerSupplyElectronics::Power( void ) const
	{
		return PowerOn;
	}
}