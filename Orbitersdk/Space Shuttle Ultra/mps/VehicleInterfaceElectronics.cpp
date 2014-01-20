#include "VehicleInterfaceElectronics.h"
#include "PowerSupplyElectronics.h"
#include "ComputerInterfaceElectronics.h"
#include "SSMEController.h"
#include "MPSdefs.h"


namespace mps
{
	VehicleInterfaceElectronics::VehicleInterfaceElectronics( SSMEController* Controller )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " VehicleInterfaceElectronics::VehicleInterfaceElectronics in" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

		Command[0] = 0;
		Command[1] = 0;
		Command[2] = 0;
		WDTstate = true;
		VehicleDataSwitch = false;

		this->Controller = Controller;

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " VehicleInterfaceElectronics::VehicleInterfaceElectronics out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	VehicleInterfaceElectronics::~VehicleInterfaceElectronics( void )
	{
		return;
	}

	void VehicleInterfaceElectronics::OnSaveState( FILEHANDLE scn ) const
	{
		int config = 1;

		if (WDTstate == true) config = 2;

		oapiWriteScenario_int( scn, "VIE config", config );
		
		__OnSaveState( scn );// write derived class
		return;
	}

	bool VehicleInterfaceElectronics::OnParseLine( const char* line )
	{
		int read_i = 0;
#ifdef _MPSDEBUG
		char buffer[150];
#endif// _MPSDEBUG

		if (!_strnicmp( line, "VIE config", 10 ))
		{
			sscanf_s( line + 12, "%d", &read_i );

			if (read_i == 1)
			{
				WDTstate = false;
				VehicleDataSwitch = true;
			}
			else// 2
			{
				WDTstate = true;
				VehicleDataSwitch = false;
			}
#ifdef _MPSDEBUG
			sprintf_s( buffer, 150, " VehicleInterfaceElectronics::OnParseLine || VehicleInterfaceElectronics_config:%d", read_i );
			oapiWriteLog( buffer );
#endif// _MPSDEBUG
			return true;
		}
		
		return __OnParseLine( line );// check if derived class wants line
	}

	void VehicleInterfaceElectronics::Realize( void )
	{
		PSE[chA] = Controller->PSE[chA];
		PSE[chB] = Controller->PSE[chB];
		CIE[chA] = Controller->CIE[chA];
		CIE[chB] = Controller->CIE[chB];
		return;
	}

	void VehicleInterfaceElectronics::CommandDataConverter_write( int ch, unsigned short cmd )
	{
		assert( (ch >= 0) && (ch <= 2) && "VehicleInterfaceElectronics::CommandDataConverter_write.ch" );

		// check power bus is working
		if (ch == chC)
		{
			if ((!PSE[chA]->Power()) && (!PSE[chB]->Power())) return;
		}
		else
		{
			if (!PSE[ch]->Power()) return;
		}

		// TODO check for errors and non 0 word
		Command[ch] = cmd;
		return;
	}

	unsigned short VehicleInterfaceElectronics::CommandDataConverter_read( int ch ) const
	{
		assert( (ch >= 0) && (ch <= 2) && "VehicleInterfaceElectronics::CommandDataConverter_read.ch" );

		// check power bus is working
		if (ch == chC)
		{
			if ((!PSE[chA]->Power()) && (!PSE[chB]->Power())) return 0;
		}
		else
		{
			if (!PSE[ch]->Power()) return 0;
		}

		return Command[ch];
	}

	void VehicleInterfaceElectronics::RecorderDataConverter_chA( unsigned short* data, int ch )
	{
		if (!PSE[chA]->Power()) return;// check power bus is working

		// send data to CIA 1

		// vehicle data switch
		if ((CIE[chA]->CheckWDTOwn( 0 ) || CIE[chA]->CheckWDTOwn( 1 )) != WDTstate)
		{
			VehicleDataSwitch = WDTstate;
			WDTstate = !WDTstate;
		}

		if (VehicleDataSwitch == true)
		{
			// A only
			if (ch == chA) Controller->EIU_CIA( 1, data );//pEIU->CIA( 1, data );
		}
		else
		{
			// B only
			if (ch == chB) Controller->EIU_CIA( 1, data );//pEIU->CIA( 1, data );
		}
		return;
	}

	void VehicleInterfaceElectronics::RecorderDataConverter_chB( unsigned short* data, int ch )
	{
		if (!PSE[chB]->Power()) return;// check power bus is working

		// send data to CIA 2

		// vehicle data switch
		if ((CIE[chA]->CheckWDTOwn( 0 ) || CIE[chA]->CheckWDTOwn( 1 )) != WDTstate)
		{
			VehicleDataSwitch = WDTstate;
			WDTstate = !WDTstate;
		}

		if (VehicleDataSwitch == true)
		{
			// A only
			if (ch == chA) Controller->EIU_CIA( 2, data );//pEIU->CIA( 2, data );
		}
		else
		{
			// B only
			if (ch == chB) Controller->EIU_CIA( 2, data );//pEIU->CIA( 2, data );
		}
		return;
	}
}