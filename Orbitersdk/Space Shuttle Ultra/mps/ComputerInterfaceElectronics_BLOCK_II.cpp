#include "ComputerInterfaceElectronics_BLOCK_II.h"
#include "VehicleInterfaceElectronics.h"
#include "MPSdefs.h"


namespace mps
{
	ComputerInterfaceElectronics_BLOCK_II::ComputerInterfaceElectronics_BLOCK_II( int ch, SSMEController* Controller ):ComputerInterfaceElectronics( ch, Controller )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " ComputerInterfaceElectronics_BLOCK_II::ComputerInterfaceElectronics_BLOCK_II in" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " ComputerInterfaceElectronics_BLOCK_II::ComputerInterfaceElectronics_BLOCK_II out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	ComputerInterfaceElectronics_BLOCK_II::~ComputerInterfaceElectronics_BLOCK_II( void )
	{
		return;
	}

	void ComputerInterfaceElectronics_BLOCK_II::__OnSaveState( FILEHANDLE scn ) const
	{
		return;
	}

	bool ComputerInterfaceElectronics_BLOCK_II::__OnParseLine( const char* line )
	{
		return false;
	}
}