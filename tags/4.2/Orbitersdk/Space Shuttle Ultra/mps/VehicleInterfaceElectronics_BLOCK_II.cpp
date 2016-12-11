#include "VehicleInterfaceElectronics_BLOCK_II.h"
#include "MPSdefs.h"


namespace mps
{
	VehicleInterfaceElectronics_BLOCK_II::VehicleInterfaceElectronics_BLOCK_II( SSMEController* Controller ):VehicleInterfaceElectronics( Controller )
	{
#ifdef _MPSDEBUG
		char buffer[100];	
		sprintf_s( buffer, 100, " VehicleInterfaceElectronics_BLOCK_II::VehicleInterfaceElectronics_BLOCK_II in" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " VehicleInterfaceElectronics_BLOCK_II::VehicleInterfaceElectronics_BLOCK_II out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	VehicleInterfaceElectronics_BLOCK_II::~VehicleInterfaceElectronics_BLOCK_II( void )
	{
		return;
	}

	void VehicleInterfaceElectronics_BLOCK_II::__OnSaveState( FILEHANDLE scn ) const
	{
		return;
	}

	bool VehicleInterfaceElectronics_BLOCK_II::__OnParseLine( const char* line )
	{
		return false;
	}

	void VehicleInterfaceElectronics_BLOCK_II::tmestp( double time )
	{
		int count = 0;

		while (count < 3)// TODO find if/how/when the registers are reset, for now it stays like this
		{
			// reset "register"
			Command[count] = 0;
			count++;
		}

		return;
	}

	void VehicleInterfaceElectronics_BLOCK_II::SwitchVRC( void )
	{
		VehicleDataSwitch = false;
		return;
	}

	void VehicleInterfaceElectronics_BLOCK_II::RestoreVRC( void )
	{
		VehicleDataSwitch = true;
		return;
	}
}