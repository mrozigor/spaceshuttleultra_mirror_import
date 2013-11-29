#include "PowerSupplyElectronics_BLOCK_II.h"
#include "MPSdefs.h"


namespace mps
{
	PowerSupplyElectronics_BLOCK_II::PowerSupplyElectronics_BLOCK_II( int ch, SSMEController* Controller ):PowerSupplyElectronics( ch, Controller )
	{
#ifdef _MPSDEBUG
		char buffer[100];	
		sprintf_s( buffer, 100, " PowerSupplyElectronics_BLOCK_II::PowerSupplyElectronics_BLOCK_II in" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " PowerSupplyElectronics_BLOCK_II::PowerSupplyElectronics_BLOCK_II out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	PowerSupplyElectronics_BLOCK_II::~PowerSupplyElectronics_BLOCK_II( void )
	{
		return;
	}

	void PowerSupplyElectronics_BLOCK_II::__OnSaveState( FILEHANDLE scn ) const
	{
		return;
	}

	bool PowerSupplyElectronics_BLOCK_II::__OnParseLine( const char* line )
	{
		return false;
	}
}
