#include "VehicleInterfaceElectronics.h"
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
		VehicleDataSwitch = false;// or false???

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
		//char cbuf[255];

		/*sprintf_s( cbuf, 255, "%d %d %d", Command[0], Command[1], Command[2] );
		oapiWriteScenario_string( scn, "VehicleInterfaceElectronics_Command", cbuf );*/

		oapiWriteScenario_int( scn, "VIE_WDTstate", WDTstate );

		oapiWriteScenario_int( scn, "VIE_VehicleDataSwitch", VehicleDataSwitch );

		__OnSaveState( scn );// write derived class
		return;
	}

	bool VehicleInterfaceElectronics::OnParseLine( const char* line )
	{
		int read_i1 = 0;
		//int read_i2 = 0;
		//int read_i3 = 0;
#ifdef _MPSDEBUG
		char buffer[150];
#endif// _MPSDEBUG

		/*if (!_strnicmp( line, "VehicleInterfaceElectronics_Command", 35 ))
		{
			sscanf_s( line + 35, "%d %d %d", &read_i1, &read_i2, &read_i3 );
			Command[0] = read_i1;
			Command[1] = read_i2;
			Command[2] = read_i3;
#ifdef _MPSDEBUG
			sprintf_s( buffer, 150, " VehicleInterfaceElectronics::OnParseLine || VehicleInterfaceElectronics_Command:%d|%d|%d", Command[0], Command[1], Command[2] );
			oapiWriteLog( buffer );
#endif// _MPSDEBUG
			return true;
		}
		else */if (!_strnicmp( line, "VIE_WDTstate", 12 ))
		{
			sscanf_s( line + 12, "%d", &read_i1 );
			WDTstate = GetBoolFromInt( read_i1 );
#ifdef _MPSDEBUG
			sprintf_s( buffer, 150, " VehicleInterfaceElectronics::OnParseLine || VehicleInterfaceElectronics_WDTstate:%d", WDTstate );
			oapiWriteLog( buffer );
#endif// _MPSDEBUG
			return true;
		}
		else if (!_strnicmp( line, "VIE_VehicleDataSwitch", 21 ))
		{
			sscanf_s( line + 21, "%d", &read_i1 );
			VehicleDataSwitch = GetBoolFromInt( read_i1 );
#ifdef _MPSDEBUG
			sprintf_s( buffer, 150, " VehicleInterfaceElectronics::OnParseLine || VehicleInterfaceElectronics_VehicleDataSwitch:%d", VehicleDataSwitch );
			oapiWriteLog( buffer );
#endif// _MPSDEBUG
			return true;
		}
		if (__OnParseLine( line )) return true;// check if derived class wants line
		return false;
	}

	void VehicleInterfaceElectronics::CommandDataConverter_write( int ch, unsigned short cmd )
	{
		// TODO check bounds
		if (!Controller->PSE_Power( ch )) return;// check power bus is working

		// TODO check for errors and non 0 word
		Command[ch - 1] = cmd;// TODO check bounds
		return;
	}

	unsigned short VehicleInterfaceElectronics::CommandDataConverter_read( int ch ) const
	{
		// TODO check bounds
		if (!Controller->PSE_Power( ch )) return 0;// check power bus is working

		return Command[ch - 1];// TODO check bounds
	}

	void VehicleInterfaceElectronics::RecorderDataConverter_chA( unsigned short* data, int ch )
	{
		if (!Controller->PSE_Power( chA )) return;// check power bus is working

		// send data to CIA 1

		// vehicle data switch
		if ((Controller->CIE_CheckWDTOwn( chA, 0 ) || Controller->CIE_CheckWDTOwn( chA, 1 )) != WDTstate)
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
		if (!Controller->PSE_Power( chB )) return;// check power bus is working

		// send data to CIA 2

		// vehicle data switch
		if ((Controller->CIE_CheckWDTOwn( chA, 0 ) || Controller->CIE_CheckWDTOwn( chA, 1 )) != WDTstate)
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