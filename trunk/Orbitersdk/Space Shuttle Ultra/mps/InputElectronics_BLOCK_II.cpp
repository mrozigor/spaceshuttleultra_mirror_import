#include "InputElectronics_BLOCK_II.h"
#include "SSME.h"
#include "SSMEController.h"
#include "PowerSupplyElectronics.h"
#include "OutputElectronics.h"
#include "MPSdefs.h"


namespace mps
{
	InputElectronics_BLOCK_II::InputElectronics_BLOCK_II( int ch, SSMEController* Controller ):InputElectronics( ch, Controller )
	{
#ifdef _MPSDEBUG
		char buffer[100];
		sprintf_s( buffer, 100, " InputElectronics_BLOCK_II::InputElectronics_BLOCK_II in || ch:%d", ch );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG

#ifdef _MPSDEBUG
		sprintf_s( buffer, 100, " InputElectronics_BLOCK_II::InputElectronics_BLOCK_II out" );
		oapiWriteLog( buffer );
#endif// _MPSDEBUG
		return;
	}

	InputElectronics_BLOCK_II::~InputElectronics_BLOCK_II( void )
	{
		return;
	}

	void InputElectronics_BLOCK_II::__OnSaveState( FILEHANDLE scn ) const
	{
		return;
	}

	bool InputElectronics_BLOCK_II::__OnParseLine( const char* line )
	{
		return false;
	}

	void InputElectronics_BLOCK_II::tmestp( double time )
	{
		// check power supply
		if (PSE->Power() == false) return;

		OE->GetSH( samplehold );
		OE->GetPOS( actpos );
		return;
	}
}