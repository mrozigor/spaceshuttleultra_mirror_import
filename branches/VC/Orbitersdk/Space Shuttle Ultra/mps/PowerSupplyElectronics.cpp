#include "PowerSupplyElectronics.h"
#include "SSMEController.h"
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

		AC = new DiscInPort;
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
		delete AC;
		return;
	}

	void PowerSupplyElectronics::OnSaveState( FILEHANDLE scn ) const
	{
		__OnSaveState( scn );// write derived class
		return;
	}

	bool PowerSupplyElectronics::OnParseLine( const char* line )
	{
		if (__OnParseLine( line )) return true;// check if derived class wants line
		return false;
	}

	void PowerSupplyElectronics::tmestp( double time )
	{
		if (AC->IsSet() == false)
		{
			if (PowerOn == true)
			{
				// power failure on the way
				Controller->DCU_PowerFailureSense( ch );
				// TODO PBDN
			}
		}
		PowerOn = AC->IsSet();
		return;
	}

	void PowerSupplyElectronics::ConnectPower( DiscreteBundle* AC )
	{
		this->AC->Connect( AC, ch );
		return;
	}

	bool PowerSupplyElectronics::Power( void ) const
	{
		return PowerOn;
	}
}